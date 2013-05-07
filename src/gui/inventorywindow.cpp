/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/inventorywindow.h"

#include "configuration.h"
#include "inputmanager.h"
#include "keyevent.h"
#include "playerinfo.h"
#include "units.h"

#include "gui/equipmentwindow.h"
#include "gui/gui.h"
#include "gui/itemamountwindow.h"
#include "gui/outfitwindow.h"
#include "gui/setup.h"
#include "gui/shopwindow.h"
#include "gui/tradewindow.h"
#include "gui/viewport.h"

#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"
#include "gui/widgets/textfield.h"

#include "resources/iteminfo.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include <guichan/font.hpp>
#include <guichan/mouseinput.hpp>

#include <string>

#include "debug.h"

static const char *SORT_NAME_INVENTORY[6] =
{
    // TRANSLATORS: inventory sort mode
    N_("default"),
    // TRANSLATORS: inventory sort mode
    N_("by name"),
    // TRANSLATORS: inventory sort mode
    N_("by id"),
    // TRANSLATORS: inventory sort mode
    N_("by weight"),
    // TRANSLATORS: inventory sort mode
    N_("by amount"),
    // TRANSLATORS: inventory sort mode
    N_("by type")
};

class SortListModelInv final : public gcn::ListModel
{
public:
    virtual ~SortListModelInv()
    { }

    virtual int getNumberOfElements() override
    { return 6; }

    virtual std::string getElementAt(int i) override
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";

        return gettext(SORT_NAME_INVENTORY[i]);
    }
};

InventoryWindow::WindowList InventoryWindow::invInstances;

InventoryWindow::InventoryWindow(Inventory *const inventory):
    Window("Inventory", false, nullptr, "inventory.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    gcn::SelectionListener(),
    InventoryListener(),
    mInventory(inventory),
    mItems(new ItemContainer(this, mInventory)),
    mWeight(),
    mSlots(),
    mUseButton(nullptr),
    mDropButton(nullptr),
    mSplitButton(nullptr),
    mOutfitButton(nullptr),
    mShopButton(nullptr),
    mEquipmentButton(nullptr),
    mStoreButton(nullptr),
    mRetrieveButton(nullptr),
    mInvCloseButton(nullptr),
    mWeightBar(nullptr),
    mSlotsBar(new ProgressBar(this, 0.0f, 100, 0, Theme::PROG_INVY_SLOTS)),
    mFilter(nullptr),
    mSortModel(new SortListModelInv),
    mSortDropDown(new DropDown(this, mSortModel, false, false, this, "sort")),
    mNameFilter(new TextField(this, "", true, this, "namefilter", true)),
    mSortDropDownCell(nullptr),
    mNameFilterCell(nullptr),
    mFilterCell(nullptr),
    mSlotsBarCell(nullptr),
    mSplit(false),
    mCompactMode(false)
{
    if (inventory)
    {
        setCaption(gettext(inventory->getName().c_str()));
        setWindowName(inventory->getName());
    }
    else
    {
        // TRANSLATORS: inventory window name
        setCaption(_("Inventory"));
        setWindowName("Inventory");
    }

    listen(CHANNEL_ATTRIBUTES);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    setDefaultSize(387, 307, ImageRect::CENTER);
    setMinWidth(310);
    setMinHeight(179);
    addKeyListener(this);

    mItems->addSelectionListener(this);

    gcn::ScrollArea *const invenScroll = new ScrollArea(
        mItems, getOptionBool("showbackground"), "inventory_background.xml");
    invenScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    const int size = config.getIntValue("fontSize");
    mFilter = new TabStrip(this, "filter_" + getWindowName(), size + 16);
    mFilter->addActionListener(this);
    mFilter->setActionEventId("tag_");

    mSortDropDown->setSelected(0);

    StringVect tags = ItemDB::getTags();
    for (unsigned f = 0; f < tags.size(); f ++)
        mFilter->addButton(tags[f]);

    if (isMainInventory())
    {
        // TRANSLATORS: inventory button
        const std::string equip = _("Equip");
        // TRANSLATORS: inventory button
        const std::string use = _("Use");
        // TRANSLATORS: inventory button
        const std::string unequip = _("Unequip");

        std::string longestUseString = getFont()->getWidth(equip) >
            getFont()->getWidth(use) ? equip : use;

        if (getFont()->getWidth(longestUseString) <
            getFont()->getWidth(unequip))
        {
            longestUseString = unequip;
        }

        mUseButton = new Button(this, longestUseString, "use", this);
        // TRANSLATORS: inventory button
        mDropButton = new Button(this, _("Drop..."), "drop", this);
        // TRANSLATORS: inventory button
        mSplitButton = new Button(this, _("Split"), "split", this);
        // TRANSLATORS: inventory button
        mOutfitButton = new Button(this, _("Outfits"), "outfit", this);
        // TRANSLATORS: inventory button
        mShopButton = new Button(this, _("Shop"), "shop", this);
        // TRANSLATORS: inventory button
        mEquipmentButton = new Button(this, _("Equipment"), "equipment", this);
        mWeightBar = new ProgressBar(this, 0.0f, 100, 0, Theme::PROG_WEIGHT);

        place(0, 0, mWeightBar, 4);
        mSlotsBarCell = &place(4, 0, mSlotsBar, 5);
        mSortDropDownCell = &place(9, 0, mSortDropDown, 2);

        mFilterCell = &place(0, 1, mFilter, 10).setPadding(3);
        mNameFilterCell = &place(9, 1, mNameFilter, 2);

        place(0, 2, invenScroll, 11).setPadding(3);
        place(0, 3, mUseButton);
        place(1, 3, mDropButton);
        place(8, 2, mSplitButton);
        place(8, 3, mShopButton);
        place(9, 3, mOutfitButton);
        place(10, 3, mEquipmentButton);

        updateWeight();
    }
    else
    {
        // TRANSLATORS: storage button
        mStoreButton = new Button(this, _("Store"), "store", this);
        // TRANSLATORS: storage button
        mRetrieveButton = new Button(this, _("Retrieve"), "retrieve", this);
        // TRANSLATORS: storage button
        mInvCloseButton = new Button(this, _("Close"), "close", this);

        mSlotsBarCell = &place(0, 0, mSlotsBar, 6);
        mSortDropDownCell = &place(6, 0, mSortDropDown, 1);

        mFilterCell = &place(0, 1, mFilter, 7).setPadding(3);
        mNameFilterCell = &place(6, 1, mNameFilter, 1);

        place(0, 2, invenScroll, 7, 4);
        place(0, 6, mStoreButton);
        place(1, 6, mRetrieveButton);
        place(6, 6, mInvCloseButton);
    }

    Layout &layout = getLayout();
    layout.setRowHeight(2, Layout::AUTO_SET);

    mInventory->addInventoyListener(this);

    invInstances.push_back(this);

    if (inventory && inventory->isMainInventory())
    {
        updateDropButton();
    }
    else
    {
        if (!invInstances.empty())
            invInstances.front()->updateDropButton();
    }

    loadWindowState();
    enableVisibleSound(true);
    slotsChanged(mInventory);

    widgetResized(gcn::Event(nullptr));
    if (!isMainInventory())
        setVisible(true);
}

InventoryWindow::~InventoryWindow()
{
    invInstances.remove(this);
    mInventory->removeInventoyListener(this);
    if (!invInstances.empty())
        invInstances.front()->updateDropButton();
    delete mSortModel;
    mSortModel = nullptr;
}

void InventoryWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "outfit")
    {
        if (outfitWindow)
        {
            outfitWindow->setVisible(!outfitWindow->isWindowVisible());
            if (outfitWindow->isWindowVisible())
                outfitWindow->requestMoveToTop();
        }
    }
    else if (eventId == "shop")
    {
        if (shopWindow)
        {
            shopWindow->setVisible(!shopWindow->isWindowVisible());
            if (shopWindow->isWindowVisible())
                shopWindow->requestMoveToTop();
        }
    }
    else if (eventId == "equipment")
    {
        if (equipmentWindow)
        {
            equipmentWindow->setVisible(!equipmentWindow->isWindowVisible());
            if (equipmentWindow->isWindowVisible())
                equipmentWindow->requestMoveToTop();
        }
    }
    else if (eventId == "close")
    {
        close();
    }
    else if (eventId == "store")
    {
        if (!inventoryWindow || !inventoryWindow->isWindowVisible())
            return;

        Item *const item = inventoryWindow->getSelectedItem();

        if (!item)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd, this, item);
    }
    else if (eventId == "sort")
    {
        mItems->setSortType(mSortDropDown->getSelected());
        return;
    }
    else if (eventId == "namefilter")
    {
        mItems->setName(mNameFilter->getText());
        mItems->updateMatrix();
    }
    else if (!eventId.find("tag_"))
    {
        std::string tagName = event.getId().substr(4);
        mItems->setFilter(ItemDB::getTagId(tagName));
        return;
    }

    Item *const item = mItems->getSelectedItem();

    if (!item)
        return;

    if (eventId == "use")
    {
        if (item->isEquipment())
        {
            if (item->isEquipped())
                Net::getInventoryHandler()->unequipItem(item);
            else
                Net::getInventoryHandler()->equipItem(item);
        }
        else
        {
            Net::getInventoryHandler()->useItem(item);
        }
    }
    if (eventId == "equip")
    {
        if (!item->isEquipment())
        {
            if (item->isEquipped())
                Net::getInventoryHandler()->unequipItem(item);
            else
                Net::getInventoryHandler()->equipItem(item);
        }
        else
        {
            Net::getInventoryHandler()->useItem(item);
        }
    }
    else if (eventId == "drop")
    {
        if (isStorageActive())
        {
            Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
                    item->getInvIndex(), item->getQuantity(),
                    Inventory::STORAGE);
        }
        else
        {
            if (inputManager.isActionActive(static_cast<int>(Input::KEY_MOD)))
            {
                Net::getInventoryHandler()->dropItem(
                    item, item->getQuantity());
            }
            else
            {
                ItemAmountWindow::showWindow(ItemAmountWindow::ItemDrop,
                    this, item);
            }
        }
    }
    else if (eventId == "split")
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit, this, item,
            (item->getQuantity() - 1));
    }
    else if (eventId == "retrieve")
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove,
            this, item);
    }
}

Item *InventoryWindow::getSelectedItem() const
{
    return mItems->getSelectedItem();
}

void InventoryWindow::unselectItem()
{
    mItems->selectNone();
}

void InventoryWindow::widgetHidden(const gcn::Event &event)
{
    Window::widgetHidden(event);
    mItems->hidePopup();
}

void InventoryWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    const int clicks = event.getClickCount();

    if (clicks == 2 && gui)
        gui->resetClickCount();

    const bool mod = (isStorageActive() && inputManager.isActionActive(
        static_cast<int>(Input::KEY_MOD)));

    const bool mod2 = (tradeWindow && tradeWindow->isWindowVisible()
        && inputManager.isActionActive(static_cast<int>(Input::KEY_MOD)));

    if (!mod && !mod2 && event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Item *const item = mItems->getSelectedItem();

        if (!item)
            return;

        /* Convert relative to the window coordinates to absolute screen
         * coordinates.
         */
        const int mx = event.getX() + getX();
        const int my = event.getY() + getY();

        if (viewport)
            viewport->showPopup(this, mx, my, item, isMainInventory());
    }

    if (!mInventory)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT
        || event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Item *const item = mItems->getSelectedItem();

        if (!item)
            return;

        if (mod)
        {
            if (mInventory->isMainInventory())
            {
                if (event.getButton() == gcn::MouseEvent::RIGHT)
                {
                    ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd,
                        inventoryWindow, item);
                }
                else
                {
                    Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
                        item->getInvIndex(), item->getQuantity(),
                        Inventory::STORAGE);
                }
            }
            else
            {
                if (event.getButton() == gcn::MouseEvent::RIGHT)
                {
                    ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove,
                        inventoryWindow, item);
                }
                else
                {
                    Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
                        item->getInvIndex(), item->getQuantity(),
                        Inventory::INVENTORY);
                }
            }
        }
        else if (mod2 && mInventory->isMainInventory())
        {
            if (event.getButton() == gcn::MouseEvent::RIGHT)
            {
                ItemAmountWindow::showWindow(ItemAmountWindow::TradeAdd,
                    tradeWindow, item);
            }
            else
            {
                if (tradeWindow)
                    tradeWindow->tradeItem(item, item->getQuantity(), true);
            }
        }
        else if (clicks == 2)
        {
            if (mInventory->isMainInventory())
            {
                if (isStorageActive())
                {
                    ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd,
                        inventoryWindow, item);
                }
                else if (tradeWindow && tradeWindow->isWindowVisible())
                {
                    ItemAmountWindow::showWindow(ItemAmountWindow::TradeAdd,
                        tradeWindow, item);
                }
                else
                {
                    if (item->isEquipment())
                    {
                        if (item->isEquipped())
                            Net::getInventoryHandler()->unequipItem(item);
                        else
                            Net::getInventoryHandler()->equipItem(item);
                    }
                    else
                    {
                        Net::getInventoryHandler()->useItem(item);
                    }
                }
            }
            else
            {
                if (isStorageActive())
                {
                    ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove,
                        inventoryWindow, item);
                }
            }
        }
    }
}

void InventoryWindow::keyPressed(gcn::KeyEvent &event)
{
    if (static_cast<KeyEvent*>(&event)->getActionId()
        == static_cast<int>(Input::KEY_GUI_MOD))
    {
        mSplit = true;
    }
}

void InventoryWindow::keyReleased(gcn::KeyEvent &event)
{
    if (static_cast<KeyEvent*>(&event)->getActionId()
        == static_cast<int>(Input::KEY_GUI_MOD))
    {
        mSplit = false;
    }
}

void InventoryWindow::valueChanged(const gcn::SelectionEvent &event A_UNUSED)
{
    if (!mInventory || !mInventory->isMainInventory())
        return;

    Item *const item = mItems->getSelectedItem();

    if (mSplit && item && Net::getInventoryHandler()->
        canSplit(mItems->getSelectedItem()))
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit,
            this, item, item->getQuantity() - 1);
    }
    updateButtons(item);
}

void InventoryWindow::updateButtons(const Item *item)
{
    if (!mInventory || !mInventory->isMainInventory())
        return;

    const Item *const selectedItem = mItems->getSelectedItem();
    if (item && selectedItem != item)
        return;

    if (!item)
        item = selectedItem;

    if (!item || item->getQuantity() == 0)
    {
        if (mUseButton)
            mUseButton->setEnabled(true);
        if (mDropButton)
            mDropButton->setEnabled(true);
        return;
    }

    if (mUseButton)
        mUseButton->setEnabled(true);
    if (mDropButton)
        mDropButton->setEnabled(true);

    if (mUseButton)
    {
        if (item && item->isEquipment())
        {
            if (item->isEquipped())
            {
                // TRANSLATORS: inventory button
                mUseButton->setCaption(_("Unequip"));
            }
            else
            {
                // TRANSLATORS: inventory button
                mUseButton->setCaption(_("Equip"));
            }
        }
        else
        {
            // TRANSLATORS: inventory button
            mUseButton->setCaption(_("Use"));
        }
    }

    updateDropButton();

    if (mSplitButton)
    {
        if (Net::getInventoryHandler()->canSplit(item))
            mSplitButton->setEnabled(true);
        else
            mSplitButton->setEnabled(false);
    }
}

void InventoryWindow::setSplitAllowed(const bool allowed)
{
    mSplitButton->setVisible(allowed);
}

void InventoryWindow::close()
{
    if (this == inventoryWindow)
    {
        setVisible(false);
    }
    else
    {
        if (Net::getInventoryHandler())
            Net::getInventoryHandler()->closeStorage(Inventory::STORAGE);
        scheduleDelete();
    }
}

void InventoryWindow::processEvent(Channels channel A_UNUSED,
                                   const DepricatedEvent &event)
{
    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        const int id = event.getInt("id");
        if (id == PlayerInfo::TOTAL_WEIGHT || id == PlayerInfo::MAX_WEIGHT)
            updateWeight();
    }
}

void InventoryWindow::updateWeight()
{
    if (!isMainInventory())
        return;

    const int total = PlayerInfo::getAttribute(PlayerInfo::TOTAL_WEIGHT);
    const int max = PlayerInfo::getAttribute(PlayerInfo::MAX_WEIGHT);

    if (max <= 0)
        return;

    // Adjust progress bar
    mWeightBar->setProgress(static_cast<float>(total)
        / static_cast<float>(max));
    mWeightBar->setText(strprintf("%s/%s", Units::formatWeight(total).c_str(),
                        Units::formatWeight(max).c_str()));
}

void InventoryWindow::slotsChanged(Inventory *const inventory)
{
    if (inventory == mInventory)
    {
        const int usedSlots = mInventory->getNumberOfSlotsUsed();
        const int maxSlots = mInventory->getSize();

        if (maxSlots)
        {
            mSlotsBar->setProgress(static_cast<float>(usedSlots)
                / static_cast<float>(maxSlots));
        }

        mSlotsBar->setText(strprintf("%d/%d", usedSlots, maxSlots));
        mItems->updateMatrix();
    }
}

void InventoryWindow::updateDropButton()
{
    if (!mDropButton)
        return;

    if (isStorageActive())
    {
        // TRANSLATORS: inventory button
        mDropButton->setCaption(_("Store"));
    }
    else
    {
        const Item *const item = mItems->getSelectedItem();
        if (item && item->getQuantity() > 1)
        {
            // TRANSLATORS: inventory button
            mDropButton->setCaption(_("Drop..."));
        }
        else
        {
            // TRANSLATORS: inventory button
            mDropButton->setCaption(_("Drop"));
        }
    }
}

bool InventoryWindow::isInputFocused() const
{
    return mNameFilter && mNameFilter->isFocused();
}

bool InventoryWindow::isAnyInputFocused()
{
    FOR_EACH (WindowList::const_iterator, it, invInstances)
    {
        if ((*it) && (*it)->isInputFocused())
            return true;
    }
    return false;
}

void InventoryWindow::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    if (!isMainInventory())
        return;

    if (getWidth() < 600)
    {
        if (!mCompactMode)
        {
            mNameFilter->setVisible(false);
            mNameFilterCell->setType(LayoutCell::NONE);
            mFilterCell->setWidth(mFilterCell->getWidth() + 2);
            mCompactMode = true;
        }
    }
    else if (mCompactMode)
    {
        mNameFilter->setVisible(true);
        mNameFilterCell->setType(LayoutCell::WIDGET);
        mFilterCell->setWidth(mFilterCell->getWidth() - 2);
        mCompactMode = false;
    }
}
