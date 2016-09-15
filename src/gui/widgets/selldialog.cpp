/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "gui/widgets/selldialog.h"

#include "units.h"

#include "gui/windows/setupwindow.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/slider.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/item/shopitem.h"

#include "debug.h"

SellDialog::DialogList SellDialog::instances;

SellDialog::SellDialog(const IsSell isSell,
                       const Advanced advanced) :
    // TRANSLATORS: sell dialog name
    Window(_("Sell"), Modal_false, nullptr, "sell.xml"),
    ActionListener(),
    SelectionListener(),
    mSellButton(nullptr),
    mQuitButton(nullptr),
    mConfirmButton(nullptr),
    mAddMaxButton(nullptr),
    mIncreaseButton(nullptr),
    mDecreaseButton(nullptr),
    mShopItemList(nullptr),
    mScrollArea(nullptr),
    mMoneyLabel(nullptr),
    mQuantityLabel(nullptr),
    mSlider(nullptr),
    mShopItems(nullptr),
    mPlayerMoney(0),
    mMaxItems(0),
    mAmountItems(0),
    mIsSell(isSell),
    mAdvanced(advanced)
{
}

void SellDialog::postInit()
{
    setWindowName("Sell");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    setDefaultSize(260, 230, ImagePosition::CENTER);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    // Create a ShopItems instance, that is aware of duplicate entries.
    mShopItems = new ShopItems(true);

    if (mAdvanced == Advanced_true)
        mShopItems->setMergeDuplicates(false);

    mShopItemList = CREATEWIDGETR(ShopListBox,
        this,
        mShopItems,
        mShopItems,
        ShopListBoxType::Unknown);
    mShopItemList->setProtectItems(true);
    mScrollArea = new ScrollArea(this, mShopItemList,
        getOptionBool("showbackground"), "sell_background.xml");
    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mSellButton = new Button(this,
        // TRANSLATORS: sell dialog button
        mAdvanced == Advanced_true ? _("Add") : _("Sell"),
        "presell",
        this);
    // TRANSLATORS: sell dialog button
    mQuitButton = new Button(this, _("Quit"), "quit", this);

    initButtons();

    mSellButton->setEnabled(false);

    mShopItemList->setDistributeMousePressed(false);
    mShopItemList->setPriceCheck(false);
    mShopItemList->addSelectionListener(this);
    mShopItemList->setActionEventId("sell");
    mShopItemList->addActionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    if (mIsSell == IsSell_true)
    {
        // TRANSLATORS: sell dialog button
        mIncreaseButton = new Button(this, _("+"), "inc", this);
        // TRANSLATORS: sell dialog button
        mDecreaseButton = new Button(this, _("-"), "dec", this);
        // TRANSLATORS: sell dialog button
        mAddMaxButton = new Button(this, _("Max"), "max", this);
        mSlider = new Slider(this, 1.0, 1.0);

        mQuantityLabel = new Label(this, strprintf(
            "%d / %d", mAmountItems, mMaxItems));
        mQuantityLabel->setAlignment(Graphics::CENTER);
        // TRANSLATORS: sell dialog label
        mMoneyLabel = new Label(this, strprintf(_("Price: %s / Total: %s"),
            "", ""));
        if (mAdvanced == Advanced_true)
        {
            // TRANSLATORS: sell dialog button
            mConfirmButton = new Button(this, _("Sell"), "confirm", this);
            mConfirmButton->setEnabled(false);
        }

        mDecreaseButton->adjustSize();
        mDecreaseButton->setWidth(mIncreaseButton->getWidth());
        mIncreaseButton->setEnabled(false);
        mDecreaseButton->setEnabled(false);
        mSlider->setEnabled(false);
        mSlider->setActionEventId("slider");
        mSlider->addActionListener(this);

        placer(0, 0, mScrollArea, 8, 5).setPadding(3);
        placer(0, 5, mDecreaseButton);
        placer(1, 5, mSlider, 3);
        placer(4, 5, mIncreaseButton);
        placer(5, 5, mQuantityLabel, 2);
        placer(7, 5, mAddMaxButton);
        placer(0, 6, mMoneyLabel, 8);
        if (mAdvanced == Advanced_true)
        {
            placer(5, 7, mSellButton);
            placer(6, 7, mConfirmButton);
        }
        else
        {
            placer(6, 7, mSellButton);
        }
        placer(7, 7, mQuitButton);
    }
    else
    {
        placer(0, 0, mScrollArea, 8, 5).setPadding(3);
        placer(6, 5, mSellButton);
        placer(7, 5, mQuitButton);
    }

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    center();
    loadWindowState();

    instances.push_back(this);
    setVisible(Visible_true);
    enableVisibleSound(true);
}

SellDialog::~SellDialog()
{
    delete2(mShopItems);
    instances.remove(this);
}

void SellDialog::reset()
{
    mShopItems->clear();
    if (mSlider)
        mSlider->setValue(0);
    mShopItemList->setSelected(-1);
    updateButtonsAndLabels();
}

void SellDialog::addItem(const Item *const item, const int price)
{
    if (!item)
        return;

    mShopItems->addItem2(item->getInvIndex(),
        item->getId(),
        item->getType(),
        item->getColor(),
        item->getQuantity(),
        price);

    mShopItemList->adjustSize();
}

ShopItem *SellDialog::addItem(const int id,
                              const ItemTypeT type,
                              const ItemColor color,
                              const int amount,
                              const int price)
{
    ShopItem *const item = mShopItems->addItem(id,
        type,
        color,
        amount,
        price);
    mShopItemList->adjustSize();
    return item;
}


void SellDialog::action(const ActionEvent &event)
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
        if (mSlider)
        {
            mAmountItems = CAST_S32(mSlider->getValue());
            updateButtonsAndLabels();
        }
    }
    else if (eventId == "inc" && mSlider && mAmountItems < mMaxItems)
    {
        mAmountItems++;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "dec" && mSlider && mAmountItems > 1)
    {
        mAmountItems--;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "max" && mSlider)
    {
        mAmountItems = mMaxItems;
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "presell" ||
             eventId == "sell" ||
             eventId == "yes" ||
             eventId == "confirm")
    {
        sellAction(event);
    }
}

void SellDialog::valueChanged(const SelectionEvent &event A_UNUSED)
{
    // Reset amount of items and update labels
    mAmountItems = 1;
    if (mSlider)
    {
        mSlider->setValue(0);
        mSlider->setScale(1, mMaxItems);
    }
    updateButtonsAndLabels();
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
    if (mDecreaseButton)
        mDecreaseButton->setEnabled(mAmountItems > 1);
    if (mIncreaseButton)
        mIncreaseButton->setEnabled(mAmountItems < mMaxItems);
    if (mSlider)
        mSlider->setEnabled(mMaxItems > 1);

    if (mQuantityLabel)
    {
        // Update the quantity and money labels
        mQuantityLabel->setCaption(strprintf("%d / %d",
            mAmountItems, mMaxItems));
    }
    if (mMoneyLabel)
    {
        // TRANSLATORS: sell dialog label
        mMoneyLabel->setCaption(strprintf(_("Price: %s / Total: %s"),
            Units::formatCurrency(income).c_str(),
            Units::formatCurrency(mPlayerMoney + income).c_str()));
    }
    if (item)
        item->update();
}

void SellDialog::setVisible(Visible visible)
{
    Window::setVisible(visible);

    if (visible == Visible_true)
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
