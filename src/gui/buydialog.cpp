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

#include "gui/buydialog.h"

#include "shopitem.h"
#include "units.h"

#include "gui/setup.h"
#include "gui/tradewindow.h"

#include "gui/widgets/dropdown.h"
#include "gui/widgets/inttextfield.h"
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

#include <algorithm>

#include "debug.h"

static const char *SORT_NAME_BUY[7] =
{
    // TRANSLATORS: buy dialog sort type.
    N_("unsorted"),
    // TRANSLATORS: buy dialog sort type.
    N_("by price"),
    // TRANSLATORS: buy dialog sort type.
    N_("by name"),
    // TRANSLATORS: buy dialog sort type.
    N_("by id"),
    // TRANSLATORS: buy dialog sort type.
    N_("by weight"),
    // TRANSLATORS: buy dialog sort type.
    N_("by amount"),
    // TRANSLATORS: buy dialog sort type.
    N_("by type")
};

class SortListModelBuy final : public gcn::ListModel
{
public:
    virtual ~SortListModelBuy()
    { }

    virtual int getNumberOfElements()
    { return 7; }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";
        return gettext(SORT_NAME_BUY[i]);
    }
};

class SortItemPriceFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const int price1 = item1->getPrice();
            const int price2 = item2->getPrice();
            if (price1 == price2)
                return item1->getDisplayName() < item2->getDisplayName();
            return price1 < price2;
        }
} itemPriceBuySorter;

class SortItemNameFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const std::string &name1 = item1->getDisplayName();
            const std::string &name2 = item2->getDisplayName();
            if (name1 == name2)
                return item1->getPrice() < item2->getPrice();
            return name1 < name2;
        }
} itemNameBuySorter;

class SortItemIdFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const int id1 = item1->getId();
            const int id2 = item2->getId();
            if (id1 == id2)
                return item1->getPrice() < item2->getPrice();
            return id1 < id2;
        }
} itemIdBuySorter;

class SortItemWeightFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const int weight1 = item1->getInfo().getWeight();
            const int weight2 = item2->getInfo().getWeight();
            if (weight1 == weight2)
                return item1->getPrice() < item2->getPrice();
            return weight1 < weight2;
        }
} itemWeightBuySorter;

class SortItemAmountFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const int amount1 = item1->getQuantity();
            const int amount2 = item2->getQuantity();
            if (amount1 == amount2)
                return item1->getPrice() < item2->getPrice();
            return amount1 < amount2;
        }
} itemAmountBuySorter;

class SortItemTypeFunctor final
{
    public:
        bool operator() (const ShopItem *const item1,
                         const ShopItem *const item2) const
        {
            if (!item1 || !item2)
                return false;

            const int type1 = item1->getInfo().getType();
            const int type2 = item2->getInfo().getType();
            if (type1 == type2)
                return item1->getPrice() < item2->getPrice();
            return type1 < type2;
        }
} itemTypeBuySorter;

BuyDialog::DialogList BuyDialog::instances;

BuyDialog::BuyDialog(const int npcId) :
    // TRANSLATORS: buy dialog name
    Window(_("Buy"), false, nullptr, "buy.xml"),
    gcn::ActionListener(),
    gcn::SelectionListener(),
    mNpcId(npcId), mMoney(0), mAmountItems(0), mMaxItems(0), mNick(),
    mSortModel(nullptr),
    mSortDropDown(nullptr)
{
    init();
}

BuyDialog::BuyDialog(std::string nick) :
    // TRANSLATORS: buy dialog name
    Window(_("Buy"), false, nullptr, "buy.xml"),
    gcn::ActionListener(),
    gcn::SelectionListener(),
    mNpcId(-1), mMoney(0), mAmountItems(0), mMaxItems(0), mNick(nick),
    mSortModel(new SortListModelBuy),
    mSortDropDown(new DropDown(this, mSortModel, false, false, this, "sort"))
{
    init();
    logger->log("BuyDialog::BuyDialog nick:" + mNick);
}

void BuyDialog::init()
{
    setWindowName("Buy");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    setDefaultSize(260, 230, ImageRect::CENTER);

    mShopItems = new ShopItems;

    mShopItemList = new ShopListBox(this, mShopItems, mShopItems);
    mScrollArea = new ScrollArea(mShopItemList,
        getOptionBool("showbackground"), "buy_background.xml");
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mSlider = new Slider(1.0);
    mQuantityLabel = new Label(this, strprintf(
        "%d / %d", mAmountItems, mMaxItems));
    mQuantityLabel->setAlignment(gcn::Graphics::CENTER);
    // TRANSLATORS: buy dialog label
    mMoneyLabel = new Label(this, strprintf(
        _("Price: %s / Total: %s"), "", ""));

    mAmountField = new IntTextField(this, 1, 1, 123);
    mAmountField->setActionEventId("amount");
    mAmountField->addActionListener(this);
    mAmountField->setSendAlwaysEvents(true);
    mAmountField->setEnabled(false);

    // TRANSLATORS: buy dialog label
    mAmountLabel = new Label(this, _("Amount:"));
    mAmountLabel->adjustSize();

    // TRANSLATORS: This is a narrow symbol used to denote 'increasing'.
    // You may change this symbol if your language uses another.
    mIncreaseButton = new Button(this, _("+"), "inc", this);
    // TRANSLATORS: This is a narrow symbol used to denote 'decreasing'.
    // You may change this symbol if your language uses another.
    mDecreaseButton = new Button(this, _("-"), "dec", this);
    // TRANSLATORS: buy dialog button
    mBuyButton = new Button(this, _("Buy"), "buy", this);
    // TRANSLATORS: buy dialog button
    mQuitButton = new Button(this, _("Quit"), "quit", this);
    // TRANSLATORS: buy dialog button
    mAddMaxButton = new Button(this, _("Max"), "max", this);

    mDecreaseButton->adjustSize();
    mDecreaseButton->setWidth(mIncreaseButton->getWidth());

    mIncreaseButton->setEnabled(false);
    mDecreaseButton->setEnabled(false);
    mBuyButton->setEnabled(false);
    mSlider->setEnabled(false);

    mSlider->setActionEventId("slider");
    mSlider->addActionListener(this);

    mShopItemList->setDistributeMousePressed(false);
    mShopItemList->setActionEventId("buy");
    mShopItemList->addActionListener(this);
    mShopItemList->addSelectionListener(this);

    ContainerPlacer placer = getPlacer(0, 0);
    placer(0, 0, mScrollArea, 9, 5).setPadding(3);
    placer(0, 5, mDecreaseButton);
    placer(1, 5, mSlider, 4);
    placer(5, 5, mIncreaseButton);
    placer(6, 5, mQuantityLabel, 2);
    placer(8, 5, mAddMaxButton);
    placer(0, 6, mAmountLabel, 2);
    placer(2, 6, mAmountField, 2);
    placer(0, 7, mMoneyLabel, 8);
    if (mSortDropDown)
        placer(0, 8, mSortDropDown, 2);
    placer(7, 8, mBuyButton);
    placer(8, 8, mQuitButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    center();
    loadWindowState();
    enableVisibleSound(true);

    instances.push_back(this);
    setVisible(true);
}

BuyDialog::~BuyDialog()
{
    delete mShopItems;
    mShopItems = nullptr;

    instances.remove(this);
}

void BuyDialog::setMoney(const int amount)
{
    mMoney = amount;
    mShopItemList->setPlayersMoney(amount);

    updateButtonsAndLabels();
}

void BuyDialog::reset()
{
    mShopItems->clear();
    mShopItemList->adjustSize();

    // Reset previous selected items to prevent failing asserts
    mShopItemList->setSelected(-1);
    mSlider->setValue(0);

    setMoney(0);
}

void BuyDialog::addItem(const int id, const unsigned char color,
                        const int amount, const int price)
{
    mShopItems->addItem(id, color, amount, price);
    mShopItemList->adjustSize();
}

void BuyDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "quit")
    {
        close();
        return;
    }
    else if (eventId == "sort")
    {
        if (mSortDropDown && mShopItems)
        {
            std::vector<ShopItem*> &items = mShopItems->items();
            switch (mSortDropDown->getSelected())
            {
                case 1:
                    std::sort(items.begin(), items.end(), itemPriceBuySorter);
                    break;
                case 2:
                    std::sort(items.begin(), items.end(), itemNameBuySorter);
                    break;
                case 3:
                    std::sort(items.begin(), items.end(), itemIdBuySorter);
                    break;
                case 4:
                    std::sort(items.begin(), items.end(), itemWeightBuySorter);
                    break;
                case 5:
                    std::sort(items.begin(), items.end(), itemAmountBuySorter);
                    break;
                case 6:
                    std::sort(items.begin(), items.end(), itemTypeBuySorter);
                    break;
                case 0:
                default:
                    break;
            }
        }
        return;
    }

    const int selectedItem = mShopItemList->getSelected();

    // The following actions require a valid selection
    if (selectedItem < 0 || selectedItem >= mShopItems->getNumberOfElements())
        return;

    if (eventId == "slider")
    {
        mAmountItems = static_cast<int>(mSlider->getValue());
        mAmountField->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "inc" && mAmountItems < mMaxItems)
    {
        mAmountItems++;
        mSlider->setValue(mAmountItems);
        mAmountField->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "dec" && mAmountItems > 1)
    {
        mAmountItems--;
        mSlider->setValue(mAmountItems);
        mAmountField->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "max")
    {
        mAmountItems = mMaxItems;
        mSlider->setValue(mAmountItems);
        mAmountField->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "amount")
    {
        mAmountItems = mAmountField->getValue();
        mSlider->setValue(mAmountItems);
        updateButtonsAndLabels();
    }
    else if (eventId == "buy" && mAmountItems > 0 && mAmountItems <= mMaxItems)
    {
        if (mNpcId != -1)
        {
            const ShopItem *const item = mShopItems->at(selectedItem);
            Net::getNpcHandler()->buyItem(mNpcId, item->getId(),
                item->getColor(), mAmountItems);

            // Update money and adjust the max number of items
            // that can be bought
            mMaxItems -= mAmountItems;
            setMoney(mMoney -
                     mAmountItems * mShopItems->at(selectedItem)->getPrice());

            // Reset selection
            mAmountItems = 1;
            mSlider->setValue(1);
            mSlider->setScale(1, mMaxItems);
        }
        else if (tradeWindow)
        {
            const ShopItem *const item = mShopItems->at(selectedItem);
            if (item)
            {
                Net::getBuySellHandler()->sendBuyRequest(mNick,
                    item, mAmountItems);
//                logger->log("buy button mNick:" + mNick);
                if (tradeWindow)
                {
                    tradeWindow->addAutoMoney(mNick,
                        item->getPrice() * mAmountItems);
                }
            }
        }
    }
}

void BuyDialog::valueChanged(const gcn::SelectionEvent &event A_UNUSED)
{
    // Reset amount of items and update labels
    mAmountItems = 1;
    mSlider->setValue(1);

    updateButtonsAndLabels();
    mSlider->setScale(1, mMaxItems);
    mAmountField->setRange(1, mMaxItems);
    mAmountField->setValue(1);
}

void BuyDialog::updateButtonsAndLabels()
{
    const int selectedItem = mShopItemList->getSelected();
    int price = 0;

    if (selectedItem > -1)
    {
        const ShopItem *const item = mShopItems->at(selectedItem);
        if (item)
        {
            const int itemPrice = item->getPrice();

            // Calculate how many the player can afford
            if (itemPrice)
                mMaxItems = mMoney / itemPrice;
            else
                mMaxItems = 1;

            if (item->getQuantity() > 0 && mMaxItems > item->getQuantity())
                mMaxItems = item->getQuantity();

            if (mAmountItems > mMaxItems)
                mAmountItems = mMaxItems;

            price = mAmountItems * itemPrice;
        }
    }
    else
    {
        mMaxItems = 0;
        mAmountItems = 0;
    }

    mIncreaseButton->setEnabled(mAmountItems < mMaxItems);
    mDecreaseButton->setEnabled(mAmountItems > 1);
    mBuyButton->setEnabled(mAmountItems > 0);
    mSlider->setEnabled(mMaxItems > 1);
    mAmountField->setEnabled(mAmountItems > 0);

    mQuantityLabel->setCaption(strprintf("%d / %d", mAmountItems, mMaxItems));
    // TRANSLATORS: buy dialog label
    mMoneyLabel->setCaption(strprintf(_("Price: %s / Total: %s"),
        Units::formatCurrency(price).c_str(),
        Units::formatCurrency(mMoney - price).c_str()));
}

void BuyDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible && mShopItemList)
        mShopItemList->requestFocus();
    else
        scheduleDelete();
}

void BuyDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
    {
        if (*it)
            (*it)->close();
    }
}
