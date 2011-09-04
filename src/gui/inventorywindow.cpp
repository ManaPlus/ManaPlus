/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "inventory.h"
#include "item.h"
#include "units.h"
#include "keyboardconfig.h"
#include "playerinfo.h"

#include "gui/itemamountwindow.h"
#include "gui/setup.h"
#include "gui/sdlinput.h"
#include "gui/shopwindow.h"
#include "gui/theme.h"
#include "gui/tradewindow.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/inventoryfilter.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/scrollarea.h"

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

InventoryWindow::WindowList InventoryWindow::instances;

InventoryWindow::InventoryWindow(Inventory *inventory):
    Window(),
    mInventory(inventory),
    mDropButton(0),
    mSplit(false)
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

    listen(Mana::CHANNEL_ATTRIBUTES);

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

    mFilter = new InventoryFilter("filter_" + getWindowName(), 20, 5);
    mFilter->addActionListener(this);
    mFilter->setActionEventId("tag_");

    mSorter = new InventoryFilter("sorter_" + getWindowName(), 20, 0);
    mSorter->addActionListener(this);
    mSorter->setActionEventId("sort_");

    mFilterLabel = new Label(_("Filter:"));
    mSorterLabel = new Label(_("Sort:"));

    std::vector<std::string> tags = ItemDB::getTags();
    for (unsigned f = 0; f < tags.size(); f ++)
        mFilter->addButton(tags[f]);

    mSorter->addButton(_("na"), "na");
    mSorter->addButton(_("az"), "az");
    mSorter->addButton(_("id"), "id");

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
        mUseButton2 = new Button(longestUseString, "equip", this);
        mDropButton = new Button(_("Drop..."), "drop", this);
        mSplitButton = new Button(_("Split"), "split", this);
        mOutfitButton = new Button(_("Outfits"), "outfit", this);
        mShopButton = new Button(_("Shop"), "shop", this);

        mWeightLabel = new Label(_("Weight:"));
        mWeightBar = new ProgressBar(0.0f, 100, 20, Theme::PROG_WEIGHT);

        place(0, 0, mWeightLabel, 1).setPadding(3);
        place(1, 0, mWeightBar, 3);
        place(4, 0, mSlotsLabel, 1).setPadding(3);
        place(5, 0, mSlotsBar, 2);
        place(7, 0, mSorterLabel, 1);
        place(8, 0, mSorter, 3);

        place(0, 1, mFilterLabel, 1).setPadding(3);
        place(1, 1, mFilter, 10).setPadding(3);

        place(0, 2, invenScroll, 11).setPadding(3);
        place(0, 3, mUseButton);
        place(1, 3, mUseButton2);
        place(2, 3, mDropButton);
        place(8, 3, mSplitButton);
        place(9, 3, mShopButton);
        place(10, 3, mOutfitButton);

        updateWeight();
    }
    else
    {
        mStoreButton = new Button(_("Store"), "store", this);
        mRetrieveButton = new Button(_("Retrieve"), "retrieve", this);
        mCloseButton = new Button(_("Close"), "close", this);

        place(0, 0, mSlotsLabel).setPadding(3);
        place(1, 0, mSlotsBar, 3);
        place(4, 0, mSorterLabel, 1);
        place(5, 0, mSorter, 2);

        place(0, 1, mFilterLabel, 1).setPadding(3);
        place(1, 1, mFilter, 6).setPadding(3);

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

    if (!isMainInventory())
        setVisible(true);
}

InventoryWindow::~InventoryWindow()
{
    instances.remove(this);
    mInventory->removeInventoyListener(this);
    if (!instances.empty())
        instances.front()->updateDropButton();
}

void InventoryWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "outfit")
    {
        extern Window *outfitWindow;
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
    else if (!event.getId().find("tag_") && mItems)
    {
        std::string tagName = event.getId().substr(4);
        mItems->setFilter(ItemDB::getTagId(tagName));
    }
    else if (!event.getId().find("sort_") && mItems)
    {
        int sortType = 0;
        std::string str = event.getId().substr(5).c_str();
        if (str == "na")
            sortType = 0;
        else if (str == "az")
            sortType = 1;
        else if (str == "id")
            sortType = 2;
        mItems->setSortType(sortType);
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
            Net::getInventoryHandler()->useItem(item);
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
            Net::getInventoryHandler()->useItem(item);
    }
    else if (event.getId() == "drop")
    {
        if (isStorageActive())
        {
            Net::getInventoryHandler()->moveItem(Inventory::INVENTORY,
                    item->getInvIndex(), item->getQuantity(),
                    Inventory::STORAGE);
        }
        else
        {
            if (keyboard.isKeyActive(keyboard.KEY_MOD))
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
        Item *item = mItems->getSelectedItem();

        if (!item)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove, this,
                                     item);
    }
}

Item *InventoryWindow::getSelectedItem() const
{
    return mItems->getSelectedItem();
}

void InventoryWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    const bool mod = (isStorageActive() && keyboard.isKeyActive(
        keyboard.KEY_MOD));

    const bool mod2 = (tradeWindow && tradeWindow->isVisible()
        && keyboard.isKeyActive(keyboard.KEY_MOD));

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
                    Net::getInventoryHandler()->moveItem(Inventory::INVENTORY,
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
                    Net::getInventoryHandler()->moveItem(Inventory::STORAGE,
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
    }
}

void InventoryWindow::keyPressed(gcn::KeyEvent &event)
{
    switch (event.getKey().getValue())
    {
        case Key::LEFT_SHIFT:
        case Key::RIGHT_SHIFT:
            mSplit = true;
            break;
        default:
            break;
    }
}

void InventoryWindow::keyReleased(gcn::KeyEvent &event)
{
    switch (event.getKey().getValue())
    {
        case Key::LEFT_SHIFT:
        case Key::RIGHT_SHIFT:
            mSplit = false;
            break;
        default:
            break;
    }
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
        if (mUseButton2)
            mUseButton2->setEnabled(true);
        if (mDropButton)
            mDropButton->setEnabled(true);
        return;
    }

    if (mUseButton)
        mUseButton->setEnabled(true);
    if (mUseButton2)
        mUseButton2->setEnabled(true);
    if (mDropButton)
        mDropButton->setEnabled(true);

    if (mUseButton && item && item->isEquipment())
    {
        if (item->isEquipped())
            mUseButton->setCaption(_("Unequip"));
        else
            mUseButton->setCaption(_("Equip"));
        mUseButton2->setCaption(_("Use"));
    }
    else if (mUseButton2)
    {
        mUseButton->setCaption(_("Use"));
        if (item->isEquipped())
            mUseButton2->setCaption(_("Unequip"));
        else
            mUseButton2->setCaption(_("Equip"));
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

void InventoryWindow::event(Mana::Channels channel A_UNUSED,
                            const Mana::Event &event)
{
    if (event.getName() == Mana::EVENT_UPDATEATTRIBUTE)
    {
        int id = event.getInt("id");
        if (id == TOTAL_WEIGHT || id == MAX_WEIGHT)
            updateWeight();
    }
}

void InventoryWindow::updateWeight()
{
    if (!isMainInventory())
        return;

    int total = PlayerInfo::getAttribute(TOTAL_WEIGHT);
    int max = PlayerInfo::getAttribute(MAX_WEIGHT);

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
        Item *item = 0;
        if (mItems)
            item = mItems->getSelectedItem();

        if (item && item->getQuantity() > 1)
            mDropButton->setCaption(_("Drop..."));
        else
            mDropButton->setCaption(_("Drop"));
    }
}
