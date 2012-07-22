/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "inventory.h"
#include "item.h"
#include "inputmanager.h"
#include "keydata.h"
#include "keyevent.h"
#include "playerinfo.h"
#include "units.h"

#include "gui/equipmentwindow.h"
#include "gui/gui.h"
#include "gui/itemamountwindow.h"
#include "gui/outfitwindow.h"
#include "gui/setup.h"
#include "gui/sdlinput.h"
#include "gui/shopwindow.h"
#include "gui/theme.h"
#include "gui/tradewindow.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabstrip.h"
#include "gui/widgets/textfield.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>
#include <guichan/mouseinput.hpp>

#include <string>

#include "debug.h"

const char *SORT_NAME[6] =
{
    N_("default"),
    N_("by name"),
    N_("by id"),
    N_("by weight"),
    N_("by amount"),
    N_("by type")
};

class SortListModel : public gcn::ListModel
{
public:
    virtual ~SortListModel()
    { }

    virtual int getNumberOfElements()
    { return 6; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(SORT_NAME[i]);
    }
};

InventoryWindow::WindowList InventoryWindow::instances;

InventoryWindow::InventoryWindow(Inventory *inventory):
    Window("Inventory", false, nullptr, "inventory.xml"),
    mInventory(inventory),
    mDropButton(nullptr),
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
    setMinWidth(316);
    setMinHeight(179);
    addKeyListener(this);

    mItems = new ItemContainer(mInventory);
    mItems->addSelectionListener(this);

    gcn::ScrollArea *invenScroll = new ScrollArea(mItems);
    invenScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mSlotsLabel = new Label(_("Slots:"));
    mSlotsBar = new ProgressBar(0.0f, 100, 20, Theme::PROG_INVY_SLOTS);

    int size = config.getIntValue("fontSize");
    mFilter = new TabStrip("filter_" + getWindowName(), size + 8);
    mFilter->addActionListener(this);
    mFilter->setActionEventId("tag_");

    mSortModel = new SortListModel();
    mSortDropDown = new DropDown(mSortModel, this, "sort");
    mSortDropDown->setSelected(0);

    mFilterLabel = new Label(_("Filter:"));
    mNameFilter = new TextField("", true, this, "namefilter", true);

    StringVect tags = ItemDB::getTags();
    for (unsigned f = 0; f < tags.size(); f ++)
        mFilter->addButton(tags[f]);

    if (isMainInventory())
    {
        std::string equip = _("Equip");
        std::string use = _("Use");
        std::string unequip = _("Unequip");

        std::string longestUseString = getFont()->getWidth(equip) >
                                       getFont()->getWidth(use) ? equip : use;

        if (getFont()->getWidth(longestUseString) <
            getFont()->getWidth(unequip))
        {
            longestUseString = unequip;
        }

        mUseButton = new Button(longestUseString, "use", this);
        mDropButton = new Button(_("Drop..."), "drop", this);
        mSplitButton = new Button(_("Split"), "split", this);
        mOutfitButton = new Button(_("Outfits"), "outfit", this);
        mShopButton = new Button(_("Shop"), "shop", this);
        mEquipmentButton = new Button(_("Equipment"), "equipment", this);

        mWeightLabel = new Label(_("Weight:"));
        mWeightBar = new ProgressBar(0.0f, 100, 20, Theme::PROG_WEIGHT);

        place(0, 0, mWeightLabel, 1).setPadding(3);
        place(1, 0, mWeightBar, 3);
        place(4, 0, mSlotsLabel, 1).setPadding(3);
        mSlotsBarCell = &place(5, 0, mSlotsBar, 3);
        mSortDropDownCell = &place(8, 0, mSortDropDown, 3);

        place(0, 1, mFilterLabel, 1).setPadding(3);
        mFilterCell = &place(1, 1, mFilter, 8).setPadding(3);
        mNameFilterCell = &place(8, 1, mNameFilter, 3);

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
        mStoreButton = new Button(_("Store"), "store", this);
        mRetrieveButton = new Button(_("Retrieve"), "retrieve", this);
        mCloseButton = new Button(_("Close"), "close", this);

        place(0, 0, mSlotsLabel).setPadding(3);
        mSlotsBarCell = &place(1, 0, mSlotsBar, 5);
        mSortDropDownCell = &place(6, 0, mSortDropDown, 1);

        place(0, 1, mFilterLabel, 1).setPadding(3);
        mFilterCell = &place(1, 1, mFilter, 6).setPadding(3);
        mNameFilterCell = &place(6, 1, mNameFilter, 1);

        place(0, 2, invenScroll, 7, 4);
        place(0, 6, mStoreButton);
        place(1, 6, mRetrieveButton);
        place(6, 6, mCloseButton);
    }

    Layout &layout = getLayout();
    layout.setRowHeight(2, Layout::AUTO_SET);

    mInventory->addInventoyListener(this);

    instances.push_back(this);

    if (inventory->isMainInventory())
    {
        updateDropButton();
    }
    else
    {
        if (!instances.empty())
            instances.front()->updateDropButton();
    }

    loadWindowState();
    slotsChanged(mInventory);

    widgetResized(nullptr);
    if (!isMainInventory())
        setVisible(true);
}

InventoryWindow::~InventoryWindow()
{
    instances.remove(this);
    mInventory->removeInventoyListener(this);
    if (!instances.empty())
        instances.front()->updateDropButton();
    delete mSortModel;
    mSortModel = nullptr;
}

void InventoryWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "outfit")
    {
        if (outfitWindow)
        {
            outfitWindow->setVisible(!outfitWindow->isVisible());
            if (outfitWindow->isVisible())
                outfitWindow->requestMoveToTop();
        }
    }
    else if (event.getId() == "shop")
    {
        if (shopWindow)
        {
            shopWindow->setVisible(!shopWindow->isVisible());
            if (shopWindow->isVisible())
                shopWindow->requestMoveToTop();
        }
    }
    else if (event.getId() == "equipment")
    {
        if (equipmentWindow)
        {
            equipmentWindow->setVisible(!equipmentWindow->isVisible());
            if (equipmentWindow->isVisible())
                equipmentWindow->requestMoveToTop();
        }
    }
    else if (event.getId() == "close")
    {
        close();
    }
    else if (event.getId() == "store")
    {
        if (!inventoryWindow || !inventoryWindow->isVisible())
            return;

        Item *item = inventoryWindow->getSelectedItem();

        if (!item)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd, this, item);
    }
    else if (event.getId() == "sort")
    {
        mItems->setSortType(mSortDropDown->getSelected());
        return;
    }
    else if (event.getId() == "namefilter")
    {
        mItems->setName(mNameFilter->getText());
        mItems->updateMatrix();
    }
    else if (!event.getId().find("tag_") && mItems)
    {
        std::string tagName = event.getId().substr(4);
        mItems->setFilter(ItemDB::getTagId(tagName));
        return;
    }

    Item *item = mItems->getSelectedItem();

    if (!item)
        return;

    if (event.getId() == "use")
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
    if (event.getId() == "equip")
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
    else if (event.getId() == "drop")
    {
        if (isStorageActive())
        {
            Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
                    item->getInvIndex(), item->getQuantity(),
                    Inventory::STORAGE);
        }
        else
        {
            if (inputManager.isActionActive(Input::KEY_MOD))
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
    else if (event.getId() == "split")
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit, this, item,
            (item->getQuantity() - 1));
    }
    else if (event.getId() == "retrieve")
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
    if (mItems)
        mItems->hidePopup();
}

void InventoryWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    const int clicks = event.getClickCount();

    if (clicks == 2 && gui)
        gui->resetClickCount();

    const bool mod = (isStorageActive() && inputManager.isActionActive(
        Input::KEY_MOD));

    const bool mod2 = (tradeWindow && tradeWindow->isVisible()
        && inputManager.isActionActive(Input::KEY_MOD));

    if (!mod && !mod2 && event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Item *item = mItems->getSelectedItem();

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
        Item *item = mItems->getSelectedItem();

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
                else if (tradeWindow && tradeWindow->isVisible())
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
    if (static_cast<KeyEvent*>(&event)->getActionId() == Input::KEY_GUI_MOD)
        mSplit = true;
}

void InventoryWindow::keyReleased(gcn::KeyEvent &event)
{
    if (static_cast<KeyEvent*>(&event)->getActionId() == Input::KEY_GUI_MOD)
        mSplit = false;
}

void InventoryWindow::valueChanged(const gcn::SelectionEvent &event A_UNUSED)
{
    if (!mInventory || !mInventory->isMainInventory())
        return;

    Item *item = mItems->getSelectedItem();

    if (mSplit && item && Net::getInventoryHandler()->
        canSplit(mItems->getSelectedItem()))
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit, this, item,
                                     (item->getQuantity() - 1));
    }
    updateButtons(item);
}

void InventoryWindow::updateButtons(Item *item)
{
    if (!mInventory || !mInventory->isMainInventory())
        return;

    Item *selectedItem = mItems->getSelectedItem();
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

    if (mUseButton && item && item->isEquipment())
    {
        if (item->isEquipped())
            mUseButton->setCaption(_("Unequip"));
        else
            mUseButton->setCaption(_("Equip"));
    }
    else
    {
        mUseButton->setCaption(_("Use"));
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

void InventoryWindow::setSplitAllowed(bool allowed)
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
        Net::getInventoryHandler()->closeStorage(Inventory::STORAGE);
        scheduleDelete();
    }
}

void InventoryWindow::processEvent(Channels channel A_UNUSED,
                                   const DepricatedEvent &event)
{
    if (event.getName() == EVENT_UPDATEATTRIBUTE)
    {
        int id = event.getInt("id");
        if (id == PlayerInfo::TOTAL_WEIGHT || id == PlayerInfo::MAX_WEIGHT)
            updateWeight();
    }
}

void InventoryWindow::updateWeight()
{
    if (!isMainInventory())
        return;

    int total = PlayerInfo::getAttribute(PlayerInfo::TOTAL_WEIGHT);
    int max = PlayerInfo::getAttribute(PlayerInfo::MAX_WEIGHT);

    if (max <= 0)
        return;

    // Adjust progress bar
    mWeightBar->setProgress(static_cast<float>(total)
        / static_cast<float>(max));
    mWeightBar->setText(strprintf("%s/%s", Units::formatWeight(total).c_str(),
                        Units::formatWeight(max).c_str()));
}

void InventoryWindow::slotsChanged(Inventory* inventory)
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
        if (mItems)
            mItems->updateMatrix();
    }
}

void InventoryWindow::updateDropButton()
{
    if (!mDropButton)
        return;

    if (isStorageActive())
    {
        mDropButton->setCaption(_("Store"));
    }
    else
    {
        Item *item = nullptr;
        if (mItems)
            item = mItems->getSelectedItem();

        if (item && item->getQuantity() > 1)
            mDropButton->setCaption(_("Drop..."));
        else
            mDropButton->setCaption(_("Drop"));
    }
}

bool InventoryWindow::isInputFocused() const
{
    return mNameFilter && mNameFilter->isFocused();
}

bool InventoryWindow::isAnyInputFocused()
{
    for (WindowList::const_iterator it = instances.begin(),
         it_end = instances.end(); it != it_end; ++it)
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
            mSortDropDown->setVisible(false);
            mNameFilter->setVisible(false);
            mSortDropDownCell->setType(LayoutCell::NONE);
            mNameFilterCell->setType(LayoutCell::NONE);
            mFilterCell->setWidth(mFilterCell->getWidth() + 3);
            mSlotsBarCell->setWidth(mSlotsBarCell->getWidth() + 3);
            mCompactMode = true;
        }
    }
    else if (mCompactMode)
    {
        mSortDropDown->setVisible(true);
        mNameFilter->setVisible(true);
        mSortDropDownCell->setType(LayoutCell::WIDGET);
        mNameFilterCell->setType(LayoutCell::WIDGET);
        mFilterCell->setWidth(mFilterCell->getWidth() - 3);
        mSlotsBarCell->setWidth(mSlotsBarCell->getWidth() - 3);
        mCompactMode = false;
    }
}
