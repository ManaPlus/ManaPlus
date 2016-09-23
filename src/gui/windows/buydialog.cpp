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

#include "gui/windows/buydialog.h"

#include "actormanager.h"
#include "configuration.h"
#include "units.h"

#include "being/being.h"

#include "gui/windows/setupwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/models/shopitems.h"
#include "gui/models/sortlistmodelbuy.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/slider.h"

#include "net/adminhandler.h"
#include "net/buysellhandler.h"
#include "net/cashshophandler.h"
#include "net/markethandler.h"
#include "net/vendinghandler.h"
#include "net/serverfeatures.h"
#include "net/npchandler.h"

#include "resources/iteminfo.h"

#include "utils/delete2.h"

#include <algorithm>

#include "debug.h"

namespace
{
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

                const ItemDbTypeT type1 = item1->getInfo().getType();
                const ItemDbTypeT type2 = item2->getInfo().getType();
                if (type1 == type2)
                    return item1->getPrice() < item2->getPrice();
                return type1 < type2;
            }
    } itemTypeBuySorter;
}  // namespace

BuyDialog::DialogList BuyDialog::instances;

BuyDialog::BuyDialog() :
    // TRANSLATORS: buy dialog name
    Window(_("Create items"), Modal_false, nullptr, "buy.xml"),
    ActionListener(),
    SelectionListener(),
    mSortModel(nullptr),
    mSortDropDown(nullptr),
    mFilterTextField(new TextField(this, "", LoseFocusOnTab_true,
        this, "namefilter", true)),
    mFilterLabel(nullptr),
    mNick(),
    mNpcId(fromInt(Items, BeingId)),
    mMoney(0),
    mAmountItems(0),
    mMaxItems(0),
    mAdvanced(false)
{
    init();
}

BuyDialog::BuyDialog(const BeingId npcId) :
    // TRANSLATORS: buy dialog name
    Window(_("Buy"), Modal_false, nullptr, "buy.xml"),
    ActionListener(),
    SelectionListener(),
    mSortModel(nullptr),
    mSortDropDown(nullptr),
    mFilterTextField(new TextField(this, "", LoseFocusOnTab_true,
        this, "namefilter", true)),
    mFilterLabel(nullptr),
    mNick(),
    mNpcId(npcId),
    mMoney(0),
    mAmountItems(0),
    mMaxItems(0),
    mAdvanced(serverFeatures ? serverFeatures->haveAdvancedBuySell() : false)
{
    init();
}

#ifdef TMWA_SUPPORT
BuyDialog::BuyDialog(std::string nick) :
    // TRANSLATORS: buy dialog name
    Window(_("Buy"), Modal_false, nullptr, "buy.xml"),
    ActionListener(),
    SelectionListener(),
    mSortModel(new SortListModelBuy),
    mSortDropDown(new DropDown(this, mSortModel, false,
        Modal_false, this, "sort")),
    mFilterTextField(new TextField(this, "", LoseFocusOnTab_true,
        this, "namefilter", true)),
    mFilterLabel(nullptr),
    mNick(nick),
    mNpcId(fromInt(Nick, BeingId)),
    mMoney(0),
    mAmountItems(0),
    mMaxItems(0),
    mAdvanced(false)
{
    init();
}
#endif  // TMWA_SUPPORT

BuyDialog::BuyDialog(const Being *const being) :
    // TRANSLATORS: buy dialog name
    Window(_("Buy"), Modal_false, nullptr, "buy.xml"),
    ActionListener(),
    SelectionListener(),
    mSortModel(new SortListModelBuy),
    mSortDropDown(new DropDown(this, mSortModel, false,
        Modal_false, this, "sort")),
    mFilterTextField(new TextField(this, "", LoseFocusOnTab_true,
        this, "namefilter", true)),
    mFilterLabel(nullptr),
    mNick(being ? being->getName() : std::string()),
    mNpcId(fromInt(Vending, BeingId)),
    mMoney(0),
    mAmountItems(0),
    mMaxItems(0),
    mAdvanced(true)
{
    init();
}

void BuyDialog::init()
{
    setWindowName("Buy");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    setDefaultSize(260, 230, ImagePosition::CENTER);

    // reset advance flag for personal shops and cash shop
    if (mAdvanced && (
#ifdef TMWA_SUPPORT
        mNpcId == fromInt(Nick, BeingId) ||
#endif  // TMWA_SUPPORT
        mNpcId == fromInt(Cash, BeingId)))
    {
        mAdvanced = false;
    }

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    mShopItems = new ShopItems;

    CREATEWIDGETV(mShopItemList, ShopListBox, this,
        mShopItems, mShopItems, ShopListBoxType::Unknown);
    mScrollArea = new ScrollArea(this, mShopItemList,
        fromBool(getOptionBool("showbackground"), Opaque),
        "buy_background.xml");
    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mSlider = new Slider(this, 1.0, 1.0);
    mQuantityLabel = new Label(this, strprintf(
        "%d / %d", mAmountItems, mMaxItems));
    mQuantityLabel->setAlignment(Graphics::CENTER);
    mMoneyLabel = new Label(this, strprintf(
        // TRANSLATORS: buy dialog label
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
    mBuyButton = new Button(this, mNpcId == fromInt(Items, BeingId)
        // TRANSLATORS: buy dialog button
        ? _("Create") : (mAdvanced ? _("Add") : _("Buy")), "buy", this);
    if (mAdvanced)
    {
        // TRANSLATORS: buy dialog button
        mConfirmButton = new Button(this, _("Buy"), "confirm", this);
        mConfirmButton->setEnabled(false);
    }
    else
    {
        mConfirmButton = nullptr;
    }
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

    mFilterTextField->setWidth(100);

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
    {
        placer(0, 8, mSortDropDown, 2);
    }
    else
    {
        // TRANSLATORS: buy dialog label
        mFilterLabel = new Label(this, _("Filter:"));
        mFilterLabel->adjustSize();
        placer(0, 8, mFilterLabel, 2);
    }
    placer(2, 8, mFilterTextField, 2);
    if (mAdvanced)
    {
        placer(6, 8, mBuyButton);
        placer(7, 8, mConfirmButton);
    }
    else
    {
        placer(7, 8, mBuyButton);
    }
    placer(8, 8, mQuitButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    center();
    loadWindowState();
    enableVisibleSound(true);

    instances.push_back(this);
    setVisible(Visible_true);

    if (mSortDropDown)
        mSortDropDown->setSelected(config.getIntValue("buySortOrder"));
}

BuyDialog::~BuyDialog()
{
    delete2(mShopItems);
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

ShopItem *BuyDialog::addItem(const int id,
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

void BuyDialog::sort()
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
}

void BuyDialog::close()
{
    switch (toInt(mNpcId, int))
    {
#ifdef TMWA_SUPPORT
        case Nick:
#endif  // TMWA_SUPPORT
        case Items:
            break;
        case Market:
            marketHandler->close();
            break;
        case Cash:
            cashShopHandler->close();
            break;
        case Vending:
            vendingHandler->close();
            break;
        default:
            buySellHandler->close();
            break;
    }
    Window::close();
}

void BuyDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "quit")
    {
        close();
        return;
    }
    else if (eventId == "sort")
    {
        sort();
        if (mSortDropDown)
            config.setValue("buySortOrder", mSortDropDown->getSelected());
        return;
    }
    else if (eventId == "namefilter")
    {
        applyNameFilter(mFilterTextField->getText());
    }

    const int selectedItem = mShopItemList->getSelected();

    // The following actions require a valid selection
    if (selectedItem < 0 || selectedItem >= mShopItems->getNumberOfElements())
        return;

    if (eventId == "slider")
    {
        mAmountItems = CAST_S32(mSlider->getValue());
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
        ShopItem *const item = mShopItems->at(selectedItem);
        if (!item)
            return;
        if (mNpcId == fromInt(Items, BeingId))
        {
            adminHandler->createItems(item->getId(),
                item->getColor(),
                mAmountItems);
        }
#ifdef TMWA_SUPPORT
        else if (mNpcId == fromInt(Nick, BeingId))
        {
            if (tradeWindow)
            {
                buySellHandler->sendBuyRequest(mNick,
                    item, mAmountItems);
                tradeWindow->addAutoMoney(mNick,
                    item->getPrice() * mAmountItems);
            }
        }
#endif  // TMWA_SUPPORT
        else if (mNpcId == fromInt(Vending, BeingId))
        {
            item->increaseUsedQuantity(mAmountItems);
            item->update();
            if (mConfirmButton)
                mConfirmButton->setEnabled(true);
        }
#ifdef TMWA_SUPPORT
        else if (mNpcId != fromInt(Nick, BeingId))
#else
        else
#endif  // TMWA_SUPPORT
        {
            if (mAdvanced)
            {
                item->increaseUsedQuantity(mAmountItems);
                item->update();
                if (mConfirmButton)
                    mConfirmButton->setEnabled(true);
            }
            else if (mNpcId == fromInt(Market, BeingId))
            {
                marketHandler->buyItem(item->getId(),
                    item->getType(),
                    item->getColor(),
                    mAmountItems);
                item->increaseQuantity(-mAmountItems);
                item->update();
            }
            else if (mNpcId == fromInt(Cash, BeingId))
            {
                cashShopHandler->buyItem(item->getPrice(),
                    item->getId(),
                    item->getColor(),
                    mAmountItems);
            }
            else
            {
                npcHandler->buyItem(mNpcId,
                    item->getId(),
                    item->getColor(),
                    mAmountItems);
            }

            updateSlider(selectedItem);
        }
    }
    else if (eventId == "confirm")
    {
        std::vector<ShopItem*> &items = mShopItems->allItems();

        if (mNpcId == fromInt(Market, BeingId))
        {
            marketHandler->buyItems(items);
        }
        else if (mNpcId == fromInt(Vending, BeingId))
        {
            const Being *const being = actorManager->findBeingByName(
                mNick,
                ActorType::Player);
            if (being)
            {
                vendingHandler->buyItems(being,
                    items);
            }
        }
        else
        {
            npcHandler->buyItems(items);
        }
        close();
    }
}

void BuyDialog::updateSlider(const int selectedItem)
{
    // Update money and adjust the max number of items
    // that can be bought
    mMaxItems -= mAmountItems;
    const ShopItem *const item = mShopItems->at(selectedItem);
    if (item)
        setMoney(mMoney - mAmountItems * item->getPrice());
    else
        setMoney(mMoney);

    // Reset selection
    mAmountItems = 1;
    mSlider->setScale(1, mMaxItems);
    mSlider->setValue(1);
}

void BuyDialog::valueChanged(const SelectionEvent &event A_UNUSED)
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
            if (mNpcId == fromInt(Items, BeingId))
                mMaxItems = 100;
            else if (itemPrice)
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

void BuyDialog::setVisible(Visible visible)
{
    Window::setVisible(visible);

    if (visible == Visible_true && mShopItemList)
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

void BuyDialog::applyNameFilter(const std::string &filter)
{
    std::vector<ShopItem*> &items = mShopItems->allItems();
    std::string filterStr = filter;
    toLower(filterStr);
    FOR_EACH (std::vector<ShopItem*>::iterator, it, items)
    {
        ShopItem *const item = *it;
        if (!item)
            continue;
        std::string name = item->getName();
        toLower(name);
        if (name.find(filterStr) != std::string::npos)
            item->setVisible(true);
        else
            item->setVisible(false);
    }
    mShopItems->updateList();
}
