/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/windows/selldialog.h"

#include "shopitem.h"
#include "units.h"

#include "being/playerinfo.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shopitems.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/slider.h"

#include "net/buysellhandler.h"
#include "net/net.h"
#include "net/npchandler.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"

#include "debug.h"

SellDialog::DialogList SellDialog::instances;

SellDialog::SellDialog(const int npcId) :
    // TRANSLATORS: sell dialog name
    Window(_("Sell"), false, nullptr, "sell.xml"),
    gcn::ActionListener(),
    gcn::SelectionListener(),
    mNpcId(npcId), mMaxItems(0), mAmountItems(0), mNick("")
{
    init();
}

SellDialog::SellDialog(const std::string &nick):
    // TRANSLATORS: sell dialog name
    Window(_("Sell"), false, nullptr, "sell.xml"),
    gcn::ActionListener(),
    gcn::SelectionListener(),
    mNpcId(-1), mMaxItems(0), mAmountItems(0), mNick(nick)
{
    init();
}

void SellDialog::init()
{
    setWindowName("Sell");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    setDefaultSize(260, 230, ImageRect::CENTER);

    // Create a ShopItems instance, that is aware of duplicate entries.
    mShopItems = new ShopItems(true);

    mShopItemList = new ShopListBox(this, mShopItems, mShopItems);
    mShopItemList->postInit();
    mShopItemList->setProtectItems(true);
    mScrollArea = new ScrollArea(mShopItemList,
        getOptionBool("showbackground"), "sell_background.xml");
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mSlider = new Slider(1.0);

    mQuantityLabel = new Label(this, strprintf(
        "%d / %d", mAmountItems, mMaxItems));
    mQuantityLabel->setAlignment(gcn::Graphics::CENTER);
    // TRANSLATORS: sell dialog label
    mMoneyLabel = new Label(this, strprintf(_("Price: %s / Total: %s"),
                                      "", ""));

    // TRANSLATORS: sell dialog button
    mIncreaseButton = new Button(this, _("+"), "inc", this);
    // TRANSLATORS: sell dialog button
    mDecreaseButton = new Button(this, _("-"), "dec", this);
    // TRANSLATORS: sell dialog button
    mSellButton = new Button(this, _("Sell"), "presell", this);
    // TRANSLATORS: sell dialog button
    mQuitButton = new Button(this, _("Quit"), "quit", this);
    // TRANSLATORS: sell dialog button
    mAddMaxButton = new Button(this, _("Max"), "max", this);

    mDecreaseButton->adjustSize();
    mDecreaseButton->setWidth(mIncreaseButton->getWidth());

    mIncreaseButton->setEnabled(false);
    mDecreaseButton->setEnabled(false);
    mSellButton->setEnabled(false);
    mSlider->setEnabled(false);

    mShopItemList->setDistributeMousePressed(false);
    mShopItemList->setPriceCheck(false);
    mShopItemList->addSelectionListener(this);
    mShopItemList->setActionEventId("sell");
    mShopItemList->addActionListener(this);

    mSlider->setActionEventId("slider");
    mSlider->addActionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mScrollArea, 8, 5).setPadding(3);
    placer(0, 5, mDecreaseButton);
    placer(1, 5, mSlider, 3);
    placer(4, 5, mIncreaseButton);
    placer(5, 5, mQuantityLabel, 2);
    placer(7, 5, mAddMaxButton);
    placer(0, 6, mMoneyLabel, 8);
    placer(6, 7, mSellButton);
    placer(7, 7, mQuitButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    center();
    loadWindowState();

    instances.push_back(this);
    setVisible(true);
    enableVisibleSound(true);
}

SellDialog::~SellDialog()
{
    delete mShopItems;
    mShopItems = nullptr;
    instances.remove(this);
}

void SellDialog::reset()
{
    mShopItems->clear();
    mSlider->setValue2(0);
    mShopItemList->setSelected(-1);
    updateButtonsAndLabels();
}

void SellDialog::addItem(const Item *const item, const int price)
{
    if (!item)
        return;

    mShopItems->addItem2(item->getInvIndex(), item->getId(),
        item->getColor(), item->getQuantity(), price);

    mShopItemList->adjustSize();
}

void SellDialog::addItem(const int id, const unsigned char color,
                         const int amount, const int price)
{
    mShopItems->addItem(id, color, amount, price);
    mShopItemList->adjustSize();
}


void SellDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();

    if (eventId == "quit")
    {
        close();
        return;
    }

    const int selectedItem = mShopItemList->getSelected();

    // The following actions require a valid item selection
    if (selectedItem == -1
        || selectedItem >= mShopItems->getNumberOfElements())
    {
        return;
    }

    if (eventId == "slider")
    {
        mAmountItems = static_cast<int>(mSlider->getValue());
        updateButtonsAndLabels();
    }
    else if (eventId == "inc" && mAmountItems < mMaxItems)
    {
        mAmountItems++;
        mSlider->setValue2(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "dec" && mAmountItems > 1)
    {
        mAmountItems--;
        mSlider->setValue2(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "max")
    {
        mAmountItems = mMaxItems;
        mSlider->setValue2(mAmountItems);
        updateButtonsAndLabels();
    }
    else if ((eventId == "presell" || eventId == "sell" || eventId == "yes")
             && mAmountItems > 0 && mAmountItems <= mMaxItems)
    {
        if (mNpcId != -1)
        {
            ShopItem *const item = mShopItems->at(selectedItem);
            if (PlayerInfo::isItemProtected(item->getId()))
                return;
            if (eventId == "presell")
            {
                const ItemInfo &info = ItemDB::get(item->getId());
                if (info.isProtected())
                {
                    ConfirmDialog *const dialog = new ConfirmDialog(
                        // TRANSLATORS: sell confirmation header
                        _("sell item"),
                        // TRANSLATORS: sell confirmation message
                        strprintf(_("Do you really want to sell %s?"),
                        info.getName().c_str()), SOUND_REQUEST, false, true);
                    dialog->postInit();
                    dialog->addActionListener(this);
                    return;
                }
            }
            // Attempt sell
            mPlayerMoney +=
                mAmountItems * mShopItems->at(selectedItem)->getPrice();
            mMaxItems -= mAmountItems;
            while (mAmountItems > 0)
            {
#ifdef MANASERV_SUPPORT
                // This order is important, item->getCurrentInvIndex() would
                // return the inventory index of the next Duplicate otherwise.
                int itemIndex = item->getCurrentInvIndex();
                const int sellCount = item->sellCurrentDuplicate(mAmountItems);
                // For Manaserv, the Item id is to be given as index.
                if ((Net::getNetworkType() == ServerInfo::MANASERV))
                    itemIndex = item->getId();
#else
                // This order is important, item->getCurrentInvIndex() would
                // return the inventory index of the next Duplicate otherwise.
                const int itemIndex = item->getCurrentInvIndex();
                const int sellCount = item->sellCurrentDuplicate(mAmountItems);
#endif
                Net::getNpcHandler()->sellItem(mNpcId, itemIndex, sellCount);
                mAmountItems -= sellCount;
            }

            mPlayerMoney +=
                mAmountItems * mShopItems->at(selectedItem)->getPrice();
            mAmountItems = 1;
            mSlider->setValue2(0);

            if (mMaxItems)
            {
                updateButtonsAndLabels();
            }
            else
            {
                // All were sold
                mShopItemList->setSelected(-1);
                delete mShopItems->at(selectedItem);
                mShopItems->erase(selectedItem);

                gcn::Rectangle scroll;
                scroll.y = mShopItemList->getRowHeight() * (selectedItem + 1);
                scroll.height = mShopItemList->getRowHeight();
                mShopItemList->showPart(scroll);
            }
        }
        else
        {
            ShopItem *const item = mShopItems->at(selectedItem);
            Net::getBuySellHandler()->sendSellRequest(mNick,
                    item, mAmountItems);

            if (tradeWindow)
                tradeWindow->addAutoItem(mNick, item, mAmountItems);
        }
    }
}

void SellDialog::valueChanged(const gcn::SelectionEvent &event A_UNUSED)
{
    // Reset amount of items and update labels
    mAmountItems = 1;
    mSlider->setValue2(0);

    updateButtonsAndLabels();
    mSlider->setScale(1, mMaxItems);
}

void SellDialog::setMoney(const int amount)
{
    mPlayerMoney = amount;
    mShopItemList->setPlayersMoney(amount);
}

void SellDialog::updateButtonsAndLabels()
{
    const int selectedItem = mShopItemList->getSelected();
    int income = 0;
    ShopItem *item = nullptr;

    if (selectedItem > -1 && mShopItems->at(selectedItem))
    {
        item = mShopItems->at(selectedItem);
        if (item)
        {
            mMaxItems = item->getQuantity();
            if (mAmountItems > mMaxItems)
                mAmountItems = mMaxItems;
            income = mAmountItems * mShopItems->at(selectedItem)->getPrice();
        }
        else
        {
            mMaxItems = 0;
            mAmountItems = 0;
        }
    }
    else
    {
        mMaxItems = 0;
        mAmountItems = 0;
    }

    // Update Buttons and slider
    mSellButton->setEnabled(mAmountItems > 0);
    mDecreaseButton->setEnabled(mAmountItems > 1);
    mIncreaseButton->setEnabled(mAmountItems < mMaxItems);
    mSlider->setEnabled(mMaxItems > 1);

    // Update the quantity and money labels
    mQuantityLabel->setCaption(strprintf("%d / %d", mAmountItems, mMaxItems));
    // TRANSLATORS: sell dialog label
    mMoneyLabel->setCaption(strprintf(_("Price: %s / Total: %s"),
                    Units::formatCurrency(income).c_str(),
                    Units::formatCurrency(mPlayerMoney + income).c_str()));
    if (item)
        item->update();
}

void SellDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible)
    {
        if (mShopItemList)
            mShopItemList->requestFocus();
    }
    else
    {
        scheduleDelete();
    }
}

void SellDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
        (*it)->close();
}
