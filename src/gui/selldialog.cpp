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

#include "gui/selldialog.h"

#include "shopitem.h"
#include "units.h"

#include "gui/setup.h"
#include "gui/tradewindow.h"

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
#include "utils/stringutils.h"

#include "debug.h"

SellDialog::DialogList SellDialog::instances;

SellDialog::SellDialog(int npcId):
    Window(_("Sell"), false, nullptr, "sell.xml"),
    mNpcId(npcId), mMaxItems(0), mAmountItems(0), mNick("")
{
    init();
}

SellDialog::SellDialog(std::string nick):
    Window(_("Sell"), false, nullptr, "sell.xml"),
    mNpcId(-1), mMaxItems(0), mAmountItems(0), mNick(nick)
{
    init();
}

void SellDialog::init()
{
    setWindowName("Sell");
    //setupWindow->registerWindowForReset(this);
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(230);
    setDefaultSize(260, 230, ImageRect::CENTER);

    // Create a ShopItems instance, that is aware of duplicate entries.
    mShopItems = new ShopItems(true);

    mShopItemList = new ShopListBox(mShopItems, mShopItems);
    mScrollArea = new ScrollArea(mShopItemList);
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mSlider = new Slider(1.0);

    mQuantityLabel = new Label(strprintf("%d / %d", mAmountItems, mMaxItems));
    mQuantityLabel->setAlignment(gcn::Graphics::CENTER);
    mMoneyLabel = new Label(strprintf(_("Price: %s / Total: %s"),
                                      "", ""));

    mIncreaseButton = new Button(_("+"), "inc", this);
    mDecreaseButton = new Button(_("-"), "dec", this);
    mSellButton = new Button(_("Sell"), "sell", this);
    mQuitButton = new Button(_("Quit"), "quit", this);
    mAddMaxButton = new Button(_("Max"), "max", this);

    mDecreaseButton->adjustSize();
    mDecreaseButton->setWidth(mIncreaseButton->getWidth());

    mIncreaseButton->setEnabled(false);
    mDecreaseButton->setEnabled(false);
    mSellButton->setEnabled(false);
    mSlider->setEnabled(false);

    mShopItemList->setPriceCheck(false);
    mShopItemList->addSelectionListener(this);
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
    mSlider->setValue(0);

    // Reset previous selected item to prevent failing asserts
    mShopItemList->setSelected(-1);

    updateButtonsAndLabels();
}

void SellDialog::addItem(const Item *item, int price)
{
    if (!item)
        return;

    mShopItems->addItem2(item->getInvIndex(), item->getId(),
        item->getColor(), item->getQuantity(), price);

    mShopItemList->adjustSize();
}

void SellDialog::addItem(int id, unsigned char color, int amount, int price)
{
    mShopItems->addItem(id, color, amount, price);
    mShopItemList->adjustSize();
}


void SellDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "quit")
    {
        close();
        return;
    }

    int selectedItem = mShopItemList->getSelected();

    // The following actions require a valid item selection
    if (selectedItem == -1 ||
        selectedItem >= static_cast<int>(mShopItems->getNumberOfElements()))
    {
        return;
    }

    if (event.getId() == "slider")
    {
        mAmountItems = static_cast<int>(mSlider->getValue());
        updateButtonsAndLabels();
    }
    else if (event.getId() == "inc" && mAmountItems < mMaxItems)
    {
        mAmountItems++;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (event.getId() == "dec" && mAmountItems > 1)
    {
        mAmountItems--;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (event.getId() == "max")
    {
        mAmountItems = mMaxItems;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (event.getId() == "sell" && mAmountItems > 0
             && mAmountItems <= mMaxItems)
    {
        if (mNpcId != -1)
        {
            // Attempt sell
            ShopItem *item = mShopItems->at(selectedItem);
            int sellCount, itemIndex;
            mPlayerMoney +=
                mAmountItems * mShopItems->at(selectedItem)->getPrice();
            mMaxItems -= mAmountItems;
            while (mAmountItems > 0)
            {
                // This order is important, item->getCurrentInvIndex() would return
                // the inventory index of the next Duplicate otherwise.
                itemIndex = item->getCurrentInvIndex();
                sellCount = item->sellCurrentDuplicate(mAmountItems);

#ifdef MANASERV_SUPPORT
                // For Manaserv, the Item id is to be given as index.
                if ((Net::getNetworkType() == ServerInfo::MANASERV))
                    itemIndex = item->getId();
#endif

                Net::getNpcHandler()->sellItem(mNpcId, itemIndex, sellCount);
                mAmountItems -= sellCount;
            }

            mPlayerMoney +=
                mAmountItems * mShopItems->at(selectedItem)->getPrice();
            mAmountItems = 1;
            mSlider->setValue(0);

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
            ShopItem *item = mShopItems->at(selectedItem);

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
    mSlider->setValue(0);

    updateButtonsAndLabels();
    mSlider->gcn::Slider::setScale(1, mMaxItems);
}

void SellDialog::setMoney(int amount)
{
    mPlayerMoney = amount;
    mShopItemList->setPlayersMoney(amount);
}

void SellDialog::updateButtonsAndLabels()
{
    int selectedItem = mShopItemList->getSelected();
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
    DialogList::const_iterator it = instances.begin();
    DialogList::const_iterator it_end = instances.end();

    for (; it != it_end; ++it)
        (*it)->close();
}
