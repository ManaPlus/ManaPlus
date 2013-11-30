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

#include "gui/windows/itemamountwindow.h"

#include "inventory.h"
#include "item.h"

#include "being/playerinfo.h"

#include "input/keyboardconfig.h"

#include "net/inventoryhandler.h"
#include "net/net.h"
#include "gui/viewport.h"

#include "gui/popups/itempopup.h"

#include "gui/windows/shopwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/label.h"
#include "gui/widgets/slider.h"

#include "utils/gettext.h"

#include "debug.h"

class ItemsModal final : public gcn::ListModel
{
public:
    ItemsModal() :
        mStrings()
    {
        const std::map<int, ItemInfo*> &items = ItemDB::getItemInfos();
        std::list<std::string> tempStrings;

        for (std::map<int, ItemInfo*>::const_iterator
             i = items.begin(), i_end = items.end();
             i != i_end; ++i)
        {
            if (i->first < 0)
                continue;

            const ItemInfo &info = *i->second;
            const std::string name = info.getName();
            if (name != "unnamed" && !info.getName().empty()
                && info.getName() != "unnamed")
            {
                tempStrings.push_back(name);
            }
        }
        tempStrings.sort();
        FOR_EACH (std::list<std::string>::const_iterator, i, tempStrings)
            mStrings.push_back(*i);
    }

    A_DELETE_COPY(ItemsModal)

    ~ItemsModal()
    { }

    int getNumberOfElements() override final
    {
        return static_cast<int>(mStrings.size());
    }

    std::string getElementAt(int i) override final
    {
        if (i < 0 || i >= getNumberOfElements())
            return "???";
        return mStrings.at(i);
    }
private:
    StringVect mStrings;
};

void ItemAmountWindow::finish(Item *const item, const int amount,
                              const int price, const Usage usage)
{
    switch (usage)
    {
        case TradeAdd:
            if (tradeWindow)
                tradeWindow->tradeItem(item, amount);
            break;
        case ItemDrop:
            PlayerInfo::dropItem(item, amount, true);
            break;
        case ItemSplit:
            Net::getInventoryHandler()->splitItem(item, amount);
            break;
        case StoreAdd:
            Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
                item->getInvIndex(), amount, Inventory::STORAGE);
            break;
        case StoreRemove:
            Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
                item->getInvIndex(), amount, Inventory::INVENTORY);
            break;
        case ShopBuyAdd:
            if (shopWindow)
                shopWindow->addBuyItem(item, amount, price);
            break;
        case ShopSellAdd:
            if (shopWindow)
                shopWindow->addSellItem(item, amount, price);
            break;
        default:
            break;
    }
}

ItemAmountWindow::ItemAmountWindow(const Usage usage, Window *const parent,
                                   Item *const item, const int maxRange) :
    Window("", false, parent, "amount.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mItemAmountTextField(new IntTextField(this, 1)),
    mItemPriceTextField(nullptr),
    mGPLabel(nullptr),
    mItem(item),
    mItemIcon(new Icon(this, item ? item->getImage() : nullptr)),
    mMax(maxRange),
    mUsage(usage),
    mItemPopup(new ItemPopup),
    mItemAmountSlide(new Slider(1.0, mMax)),
    mItemPriceSlide(nullptr),
    mItemDropDown(nullptr),
    mItemsModal(nullptr),
    mPrice(0),
    mEnabledKeyboard(keyboard.isEnabled())
{
    mItemPopup->postInit();

    if (!mItem)
        return;

    if (usage == ShopBuyAdd)
        mMax = 10000;
    else if (!mMax)
        mMax = mItem->getQuantity();

    keyboard.setEnabled(false);

    mItemAmountTextField->setRange(1, mMax);
    mItemAmountTextField->setWidth(35);
    mItemAmountTextField->addKeyListener(this);

    mItemAmountSlide->setHeight(10);
    mItemAmountSlide->setActionEventId("slide");
    mItemAmountSlide->addActionListener(this);

    if (mUsage == ShopBuyAdd || mUsage == ShopSellAdd)
    {
        mItemPriceTextField = new IntTextField(this, 1);
        mItemPriceTextField->setRange(1, 10000000);
        mItemPriceTextField->setWidth(35);
        mItemPriceTextField->addKeyListener(this);

        mItemPriceSlide = new Slider(1.0, 10000000);
        mItemPriceSlide->setHeight(10);
        mItemPriceSlide->setActionEventId("slidePrice");
        mItemPriceSlide->addActionListener(this);

        mGPLabel = new Label(this, " GP");
    }

    if (mUsage == ShopBuyAdd)
    {
        mItemsModal = new ItemsModal;
        mItemDropDown = new DropDown(this, mItemsModal);
        mItemDropDown->setActionEventId("itemType");
        mItemDropDown->addActionListener(this);
    }

    // Buttons
    // TRANSLATORS: item amount window button
    Button *const minusAmountButton = new Button(this, _("-"), "dec", this);
    // TRANSLATORS: item amount window button
    Button *const plusAmountButton = new Button(this, _("+"), "inc", this);
    // TRANSLATORS: item amount window button
    Button *const okButton = new Button(this, _("OK"), "ok", this);
    // TRANSLATORS: item amount window button
    Button *const cancelButton = new Button(this, _("Cancel"), "cancel", this);
    // TRANSLATORS: item amount window button
    Button *const addAllButton = new Button(this, _("All"), "all", this);

    minusAmountButton->adjustSize();
    minusAmountButton->setWidth(plusAmountButton->getWidth());

    // Set positions
    ContainerPlacer placer;
    placer = getPlacer(0, 0);
    int n = 0;
    if (mUsage == ShopBuyAdd)
    {
        placer(0, n, mItemDropDown, 8);
        n++;
    }
    placer(1, n, minusAmountButton);
    placer(2, n, mItemAmountTextField, 3);
    placer(5, n, plusAmountButton);
    placer(6, n, addAllButton);

    placer(0, n, mItemIcon, 1, 3);
    placer(1, n + 1, mItemAmountSlide, 7);

    if (mUsage == ShopBuyAdd || mUsage == ShopSellAdd)
    {
        Button *const minusPriceButton = new Button(
            // TRANSLATORS: item amount window button
            this, _("-"), "decPrice", this);
        Button *const plusPriceButton = new Button(
            // TRANSLATORS: item amount window button
            this, _("+"), "incPrice", this);
        minusPriceButton->adjustSize();
        minusPriceButton->setWidth(plusPriceButton->getWidth());

        placer(1, n + 2, minusPriceButton);
        placer(2, n + 2, mItemPriceTextField, 3);
        placer(5, n + 2, plusPriceButton);
        placer(6, n + 2, mGPLabel);

        placer(1, n + 3, mItemPriceSlide, 7);
        placer(4, n + 5, cancelButton);
        placer(5, n + 5, okButton);
    }
    else
    {
        placer(4, n + 2, cancelButton);
        placer(5, n + 2, okButton);
    }

    reflowLayout(225, 0);

    resetAmount();

    switch (usage)
    {
        case TradeAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to trade."));
            break;
        case ItemDrop:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to drop."));
            break;
        case StoreAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to store."));
            break;
        case StoreRemove:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to retrieve."));
            break;
        case ItemSplit:
            // TRANSLATORS: amount window message
            setCaption(_("Select amount of items to split."));
            break;
        case ShopBuyAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Add to buy shop."));
            break;
        case ShopSellAdd:
            // TRANSLATORS: amount window message
            setCaption(_("Add to sell shop."));
            break;
        default:
            // TRANSLATORS: amount window message
            setCaption(_("Unknown."));
            break;
    }

    setLocationRelativeTo(getParentWindow());

    mItemIcon->addMouseListener(this);
}

void ItemAmountWindow::postInit()
{
    setVisible(mItem);
}

ItemAmountWindow::~ItemAmountWindow()
{
    delete mItemPopup;
    mItemPopup = nullptr;
}

// Show ItemTooltip
void ItemAmountWindow::mouseMoved(gcn::MouseEvent &event)
{
    Window::mouseMoved(event);

    if (!viewport || !mItemPopup)
        return;

    if (event.getSource() == mItemIcon)
    {
        mItemPopup->setItem(mItem);
        mItemPopup->position(viewport->getMouseX(), viewport->getMouseY());
    }
}

// Hide ItemTooltip
void ItemAmountWindow::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
}

void ItemAmountWindow::resetAmount()
{
    mItemAmountTextField->setValue(1);
}

void ItemAmountWindow::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        close();
        return;
    }
    else if (eventId == "ok")
    {
        if (mItemPriceTextField)
        {
            finish(mItem, mItemAmountTextField->getValue(),
                   mItemPriceTextField->getValue(), mUsage);
        }
        else
        {
            finish(mItem, mItemAmountTextField->getValue(),
                   0, mUsage);
        }
        close();
        return;
    }
    else if (eventId == "itemType")
    {
        if (!mItemDropDown || !mItemsModal)
            return;

        const int id = ItemDB::get(mItemsModal->getElementAt(
            mItemDropDown->getSelected())).getId();

        mItem = new Item(id, 10000);

        if (mUsage == ShopBuyAdd)
            mMax = 10000;
        else if (!mMax)
            mMax = mItem->getQuantity();

        mItemIcon->setImage(mItem->getImage());
    }

    int amount = mItemAmountTextField->getValue();

    if (eventId == "inc" && amount < mMax)
        amount++;
    else if (eventId == "dec" && amount > 1)
        amount--;
    else if (eventId == "all")
        amount = mMax;
    else if (eventId == "slide")
        amount = static_cast<int>(mItemAmountSlide->getValue());
    mItemAmountTextField->setValue(amount);
    mItemAmountSlide->setValue2(amount);

    if (mItemPriceTextField && mItemPriceSlide)
    {
        if (mPrice > 7)
            mPrice = 7;
        else if (mPrice < 0)
            mPrice = 0;

        int price = 0;

        if (eventId == "incPrice")
        {
            mPrice++;
            price = static_cast<int>(pow(10.0, mPrice));
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue2(price);
        }
        else if (eventId == "decPrice")
        {
            mPrice--;
            price = static_cast<int>(pow(10.0, mPrice));
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue2(price);
        }
        else if (eventId == "slidePrice")
        {
            price = static_cast<int>(mItemPriceSlide->getValue());
            if (price)
                mPrice = static_cast<int>(log(static_cast<float>(price)));
            else
                mPrice = 0;
            mItemPriceTextField->setValue(price);
            mItemPriceSlide->setValue2(price);
        }
    }
}

void ItemAmountWindow::close()
{
    keyboard.setEnabled(mEnabledKeyboard);
    scheduleDelete();
}

void ItemAmountWindow::keyReleased(gcn::KeyEvent &keyEvent A_UNUSED)
{
    mItemAmountSlide->setValue2(mItemAmountTextField->getValue());
}

void ItemAmountWindow::showWindow(const Usage usage, Window *const parent,
                                  Item *const item, int maxRange)
{
    if (!item)
        return;

    if (!maxRange)
        maxRange = item->getQuantity();

    if (usage != ShopBuyAdd && usage != ShopSellAdd && maxRange <= 1)
        finish(item, maxRange, 0, usage);
    else
        (new ItemAmountWindow(usage, parent, item, maxRange))->postInit();
}
