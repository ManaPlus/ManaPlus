/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/windows/shopwindow.h"

#include "const/resources/currency.h"

#ifdef TMWA_SUPPORT
#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/confirmdialog.h"
#include "gui/windows/shopselldialog.h"
#include "gui/windows/tradewindow.h"

#include "const/gui/chat.h"
#endif  // TMWA_SUPPORT

#include "enums/gui/layouttype.h"

#include "gui/windows/editdialog.h"

#include "gui/windows/itemamountwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/tabstrip.h"

#include "listeners/shoprenamelistener.h"

#ifdef TMWA_SUPPORT
#include "actormanager.h"
#include "soundmanager.h"
#endif  // TMWA_SUPPORT
#include "configuration.h"
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#ifdef TMWA_SUPPORT
#include "being/playerrelations.h"
#include "net/chathandler.h"
#endif  // TMWA_SUPPORT
#include "net/buyingstorehandler.h"
#include "net/vendinghandler.h"
#include "net/net.h"
#ifdef TMWA_SUPPORT
#include "net/tradehandler.h"
#endif  // TMWA_SUPPORT

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#ifdef TMWA_SUPPORT
#include "resources/iteminfo.h"
#endif  // TMWA_SUPPORT

#include "resources/inventory/inventory.h"

#include "resources/item/shopitem.h"

#include <sys/stat.h>

#include <sstream>

#include "debug.h"

ShopWindow *shopWindow = nullptr;
extern std::string tradePartnerName;
ShopWindow::DialogList ShopWindow::instances;

ShopWindow::ShopWindow() :
    // TRANSLATORS: shop window name
    Window(_("Personal Shop"), Modal_false, nullptr, "shop.xml"),
    VendingModeListener(),
    VendingSlotsListener(),
    BuyingStoreModeListener(),
    BuyingStoreSlotsListener(),
    ActionListener(),
    SelectionListener(),
    // TRANSLATORS: shop window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    mBuyShopItems(new ShopItems(false, DEFAULT_CURRENCY)),
    mSellShopItems(new ShopItems(false, DEFAULT_CURRENCY)),
    mTradeItem(nullptr),
    mBuyShopItemList(CREATEWIDGETR(ShopListBox,
        this, mBuyShopItems, mBuyShopItems, ShopListBoxType::BuyShop)),
    mSellShopItemList(CREATEWIDGETR(ShopListBox,
        this, mSellShopItems, mSellShopItems, ShopListBoxType::SellShop)),
    mCurrentShopItemList(nullptr),
    mScrollArea(new ScrollArea(this, mBuyShopItemList,
        fromBool(getOptionBool("showbuybackground"), Opaque),
        "shop_buy_background.xml")),
    // TRANSLATORS: shop window label
    mAddButton(new Button(this, _("Add"), "add", this)),
    // TRANSLATORS: shop window label
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    mAnnounceButton(nullptr),
    mPublishButton(nullptr),
    mRenameButton(nullptr),
    mAnnounceLinks(nullptr),
    mTabs(nullptr),
    mAcceptPlayer(),
    mTradeNick(),
    mSellShopName(serverConfig.getStringValue("sellShopName")),
    mSelectedItem(-1),
    mAnnonceTime(0),
    mLastRequestTimeList(0),
    mLastRequestTimeItem(0),
    mRandCounter(0),
    mTradeMoney(0),
    mSellShopSize(0),
    mBuyShopSize(0),
    isBuySelected(true),
    mHaveVending(Net::getNetworkType() != ServerType::TMWATHENA),
    mEnableBuyingStore(false),
    mEnableVending(false)
{
    setWindowName("Personal Shop");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(300);
    setMinHeight(220);
    if (mainGraphics->mWidth > 600)
        setDefaultSize(500, 300, ImagePosition::CENTER);
    else
        setDefaultSize(380, 300, ImagePosition::CENTER);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    const int size = config.getIntValue("fontSize")
        + getOption("tabHeightAdjust", 16);
    mTabs = new TabStrip(this, "shop", size);
    mTabs->addActionListener(this);
    mTabs->setActionEventId("tab_");
    // TRANSLATORS: shop window tab name
    mTabs->addButton(_("Buy"), "buy", true);
    // TRANSLATORS: shop window tab name
    mTabs->addButton(_("Sell"), "sell", false);
    mTabs->setSelectable(false);
    mScrollArea->setSelectable(false);

    loadList();

    mBuyShopItemList->setPriceCheck(false);
    mSellShopItemList->setPriceCheck(false);

    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mBuyShopItemList->addSelectionListener(this);
    mSellShopItemList->addSelectionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mTabs, 8).setPadding(3);

    if (mHaveVending)
    {
        // TRANSLATORS: shop window button
        mPublishButton = new Button(this, _("Publish"), "publish", this);
        // TRANSLATORS: shop window button
        mRenameButton = new Button(this, _("Rename"), "rename", this);
        placer(2, 6, mPublishButton);
        placer(3, 6, mRenameButton);
    }
    else
    {
        // TRANSLATORS: shop window button
        mAnnounceButton = new Button(this, _("Announce"), "announce", this);
        // TRANSLATORS: shop window checkbox
        mAnnounceLinks = new CheckBox(this, _("Show links in announce"), false,
            this, "link announce");

        placer(2, 6, mAnnounceButton);
        placer(0, 7, mAnnounceLinks, 7);
    }

    placer(0, 1, mScrollArea, 8, 5).setPadding(3);
    placer(0, 6, mAddButton);
    placer(1, 6, mDeleteButton);
    placer(7, 6, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    center();
    loadWindowState();
    updateShopName();
    instances.push_back(this);
}

void ShopWindow::postInit()
{
    Window::postInit();
    setVisible(Visible_false);
    enableVisibleSound(true);
    updateSelection();
}

ShopWindow::~ShopWindow()
{
    saveList();

    delete2(mBuyShopItemList);
    delete2(mSellShopItemList);
    delete2(mBuyShopItems);
    delete2(mSellShopItems);
    delete2(mTradeItem);

    instances.remove(this);
}

void ShopWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        close();
        return;
    }
#ifdef TMWA_SUPPORT
    else if (eventId == "yes")
    {
        startTrade();
    }
    else if (eventId == "no")
    {
        mTradeNick.clear();
    }
    else if (eventId == "ignore")
    {
        playerRelations.ignoreTrade(mTradeNick);
        mTradeNick.clear();
    }
    else if (eventId == "announce")
    {
        if (isBuySelected)
        {
            if (mBuyShopItems->getNumberOfElements() > 0)
                announce(mBuyShopItems, BUY);
        }
        else if (mSellShopItems->getNumberOfElements() > 0)
        {
            announce(mSellShopItems, SELL);
        }
    }
#endif  // TMWA_SUPPORT
    else if (eventId == "delete")
    {
        if (isBuySelected)
        {
            if (mBuyShopItemList != nullptr &&
                mBuyShopItemList->getSelected() >= 0)
            {
                mBuyShopItems->del(mBuyShopItemList->getSelected());
                if (isShopEmpty() && (localPlayer != nullptr))
                    localPlayer->updateStatus();
            }
        }
        else if ((mSellShopItemList != nullptr)
                 && mSellShopItemList->getSelected() >= 0)
        {
            mSellShopItems->del(mSellShopItemList->getSelected());
            if (isShopEmpty() && (localPlayer != nullptr))
                localPlayer->updateStatus();
        }
    }
    else if (eventId == "tab_buy")
    {
        isBuySelected = true;
        updateSelection();
    }
    else if (eventId == "tab_sell")
    {
        isBuySelected = false;
        updateSelection();
    }
    else if (eventId == "publish")
    {
        if (isBuySelected)
        {
            if (mEnableBuyingStore)
            {
                buyingStoreHandler->close();
                BuyingStoreModeListener::distributeEvent(false);
            }
            else
            {
                STD_VECTOR<ShopItem*> &items = mBuyShopItems->items();
                if (!items.empty())
                {
                    buyingStoreHandler->create(mSellShopName,
                        PlayerInfo::getAttribute(Attributes::MONEY),
                        true,
                        items);
                }
            }
        }
        else
        {
            if (mEnableVending)
            {
                vendingHandler->close();
                VendingModeListener::distributeEvent(false);
            }
            else
            {
                STD_VECTOR<ShopItem*> &oldItems = mSellShopItems->items();
                STD_VECTOR<ShopItem*> items;
                const Inventory *const inv = PlayerInfo::getCartInventory();
                if (inv == nullptr)
                    return;
                FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, oldItems)
                {
                    ShopItem *const item = *it;
                    if (item == nullptr)
                        continue;
                    const Item *const cartItem = inv->findItem(item->getId(),
                        item->getColor());
                    if (cartItem == nullptr)
                        continue;
                    item->setInvIndex(cartItem->getInvIndex());
                    const int amount = cartItem->getQuantity();
                    if (amount == 0)
                        continue;
                    if (item->getQuantity() > amount)
                        item->setQuantity(amount);
                    items.push_back(item);
                    if (static_cast<signed int>(items.size()) >= mSellShopSize)
                        break;
                }
                if (!items.empty())
                    vendingHandler->createShop(mSellShopName, true, items);
            }
        }
    }
    else if (eventId == "rename")
    {
        EditDialog *const dialog = CREATEWIDGETR(EditDialog,
            // TRANSLATORS: shop rename dialog title
            _("Please enter new shop name"),
            mSellShopName,
            "OK",
            300,
            nullptr,
            Modal_true);
        shopRenameListener.setDialog(dialog);
        dialog->addActionListener(&shopRenameListener);
    }

    if (mSelectedItem < 1)
        return;

    const Inventory *const inv = mHaveVending && !isBuySelected
        ? PlayerInfo::getCartInventory() : PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    // +++ need support for colors
    Item *const item = inv->findItem(mSelectedItem, ItemColor_zero);
    if (item != nullptr)
    {
        if (eventId == "add")
        {
            if (isBuySelected)
            {
                ItemAmountWindow::showWindow(ItemAmountWindowUsage::ShopBuyAdd,
                    this,
                    item,
                    sumAmount(item));
            }
            else
            {
                ItemAmountWindow::showWindow(
                    ItemAmountWindowUsage::ShopSellAdd,
                    this,
                    item,
                    sumAmount(item));
            }
        }
    }
}

void ShopWindow::valueChanged(const SelectionEvent &event A_UNUSED)
{
    updateButtonsAndLabels();
}

void ShopWindow::updateButtonsAndLabels()
{
    bool allowDel(false);
    bool allowAdd(false);
    const bool sellNotEmpty = mSellShopItems->getNumberOfElements() > 0;
    if (isBuySelected)
    {
        allowAdd = !mEnableBuyingStore;
        allowDel = !mEnableBuyingStore
            && mBuyShopItemList->getSelected() != -1
            && mBuyShopItems->getNumberOfElements() > 0;
        if (mPublishButton != nullptr)
        {
            if (mEnableBuyingStore)
            {
                // TRANSLATORS: unpublish shop button
                mPublishButton->setCaption(_("Unpublish"));
            }
            else
            {
                // TRANSLATORS: publish shop button
                mPublishButton->setCaption(_("Publish"));
            }
            mPublishButton->adjustSize();
            if (mBuyShopSize > 0)
                mPublishButton->setEnabled(true);
            else
                mPublishButton->setEnabled(false);
        }
    }
    else
    {
        allowAdd = !mEnableVending && mSelectedItem != -1;
        allowDel = !mEnableVending
            && mSellShopItemList->getSelected() != -1
            && sellNotEmpty;
        if (mPublishButton != nullptr)
        {
            if (mEnableVending)
            {
                // TRANSLATORS: unpublish shop button
                mPublishButton->setCaption(_("Unpublish"));
            }
            else
            {
                // TRANSLATORS: publish shop button
                mPublishButton->setCaption(_("Publish"));
            }
            mPublishButton->adjustSize();
            if (sellNotEmpty
                && mSellShopSize > 0
                && (localPlayer != nullptr)
                && localPlayer->getHaveCart())
            {
                mPublishButton->setEnabled(true);
            }
            else
            {
                mPublishButton->setEnabled(false);
            }
        }
    }
    mAddButton->setEnabled(allowAdd);
    mDeleteButton->setEnabled(allowDel);
    if (mRenameButton != nullptr)
        mRenameButton->setEnabled(!mEnableVending);
}

void ShopWindow::setVisible(Visible visible)
{
    Window::setVisible(visible);
}

void ShopWindow::addBuyItem(const Item *const item, const int amount,
                            const int price)
{
    if (item == nullptr)
        return;
    const bool emp = isShopEmpty();
    mBuyShopItems->addItemNoDup(item->getId(),
        item->getType(),
        item->getColor(),
        amount,
        price);
    if (emp && (localPlayer != nullptr))
        localPlayer->updateStatus();

    updateButtonsAndLabels();
}

void ShopWindow::addSellItem(const Item *const item, const int amount,
                             const int price)
{
    if (item == nullptr)
        return;
    const bool emp = isShopEmpty();
    mSellShopItems->addItemNoDup(item->getId(),
        item->getType(),
        item->getColor(),
        amount,
        price);
    if (emp && (localPlayer != nullptr))
        localPlayer->updateStatus();

    updateButtonsAndLabels();
}

void ShopWindow::loadList()
{
    std::ifstream shopFile;
    struct stat statbuf;

    mBuyShopItems->clear();
    mSellShopItems->clear();

    const std::string shopListName = settings.serverConfigDir
        + "/shoplist.txt";

    if (stat(shopListName.c_str(), &statbuf) == 0 &&
        S_ISREG(statbuf.st_mode))
    {
        shopFile.open(shopListName.c_str(), std::ios::in);
        if (!shopFile.is_open())
        {
            reportAlways("Error opening file for reading: %s",
                shopListName.c_str());
            shopFile.close();
            return;
        }
        char line[101];
        while (shopFile.getline(line, 100))
        {
            std::string buf;
            const std::string str = line;
            if (!str.empty())
            {
                STD_VECTOR<int> tokens;
                std::stringstream ss(str);

                while (ss >> buf)
                    tokens.push_back(atoi(buf.c_str()));

                if (tokens.size() == 5 && (tokens[0] != 0))
                {
                    // +++ need impliment colors?
                    if ((tokens[1] != 0) && (tokens[2] != 0))
                    {
                        mBuyShopItems->addItem(
                            tokens[0],
                            ItemType::Unknown,
                            ItemColor_one,
                            tokens[1],
                            tokens[2]);
                    }
                    if ((tokens[3] != 0) && (tokens[4] != 0))
                    {
                        mSellShopItems->addItem(
                            tokens[0],
                            ItemType::Unknown,
                            ItemColor_one,
                            tokens[3],
                            tokens[4]);
                    }
                }
            }
        }
        shopFile.close();
    }
}

void ShopWindow::saveList() const
{
    std::ofstream shopFile;
    const std::string shopListName = settings.serverConfigDir
        + "/shoplist.txt";
    std::map<int, ShopItem*> mapItems;

    shopFile.open(shopListName.c_str(), std::ios::binary);
    if (!shopFile.is_open())
    {
        reportAlways("Error opening file writing: %s",
            shopListName.c_str());
        return;
    }

    STD_VECTOR<ShopItem*> items = mBuyShopItems->items();
    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
    {
        ShopItem *const item = *(it);
        if (item != nullptr)
            mapItems[item->getId()] = item;
    }

    items = mSellShopItems->items();
    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
    {
        if ((*it) == nullptr)
            continue;
        const ShopItem *const sellItem = *(it);
        const ShopItem *const buyItem = mapItems[sellItem->getId()];

        shopFile << sellItem->getId();
        if (buyItem != nullptr)
        {
            shopFile << strprintf(" %d %d ", buyItem->getQuantity(),
                buyItem->getPrice());
            mapItems.erase(sellItem->getId());
        }
        else
        {
            shopFile << " 0 0 ";
        }

        shopFile << strprintf("%d %d", sellItem->getQuantity(),
            sellItem->getPrice()) << std::endl;
    }

    for (std::map<int, ShopItem*>::const_iterator mapIt = mapItems.begin(),
         mapIt_fend = mapItems.end();
         mapIt != mapIt_fend;
         ++mapIt)
    {
        const ShopItem *const buyItem = (*mapIt).second;
        if (buyItem != nullptr)
        {
            shopFile << buyItem->getId();
            shopFile << strprintf(" %d %d ", buyItem->getQuantity(),
                                  buyItem->getPrice());
            shopFile << "0 0" << std::endl;
        }
    }

    shopFile.close();
}

#ifdef TMWA_SUPPORT
void ShopWindow::announce(ShopItems *const list, const int mode)
{
    if (list == nullptr)
        return;

    std::string data;
    if (mode == BUY)
        data.append("Buy ");
    else
        data.append("Sell ");

    if (mAnnonceTime != 0 &&
        (mAnnonceTime + (2 * 60) > cur_time || mAnnonceTime > cur_time))
    {
        return;
    }

    mAnnonceTime = cur_time;
    if (mAnnounceButton != nullptr)
        mAnnounceButton->setEnabled(false);

    STD_VECTOR<ShopItem*> items = list->items();

    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *(it);
        if (item->getQuantity() > 1)
        {
            if (mAnnounceLinks->isSelected())
            {
                data.append(strprintf("[@@%d|%s@@] (%dGP) %d, ", item->getId(),
                    item->getInfo().getName().c_str(),
                    item->getPrice(), item->getQuantity()));
            }
            else
            {
                data.append(strprintf("%s (%dGP) %d, ",
                    item->getInfo().getName().c_str(),
                    item->getPrice(), item->getQuantity()));
            }
        }
        else
        {
            if (mAnnounceLinks->isSelected())
            {
                data.append(strprintf("[@@%d|%s@@] (%dGP), ", item->getId(),
                    item->getInfo().getName().c_str(), item->getPrice()));
            }
            else
            {
                data.append(strprintf("%s (%dGP), ",
                    item->getInfo().getName().c_str(), item->getPrice()));
            }
        }
    }

    chatHandler->channelMessage(TRADE_CHANNEL, data);
}

void ShopWindow::startTrade()
{
    if (actorManager == nullptr ||
        tradeWindow == nullptr)
    {
        return;
    }

    const Being *const being = actorManager->findBeingByName(
        mTradeNick, ActorType::Player);
    tradeWindow->clear();
    if (mTradeMoney != 0)
    {
        tradeWindow->addAutoMoney(mTradeNick, mTradeMoney);
    }
    else
    {
        tradeWindow->addAutoItem(mTradeNick, mTradeItem,
            mTradeItem->getQuantity());
    }
    tradeHandler->request(being);
    tradePartnerName = mTradeNick;
    mTradeNick.clear();
}

void ShopWindow::giveList(const std::string &nick, const int mode)
{
    if (!checkFloodCounter(mLastRequestTimeList))
        return;

    std::string data("\302\202");

    ShopItems *list;
    if (mode == BUY)
    {
        list = mBuyShopItems;
        data.append("S1");
    }
    else
    {
        list = mSellShopItems;
        data.append("B1");
    }
    if (list == nullptr)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    STD_VECTOR<ShopItem*> items = list->items();

    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *(it);
        if (item == nullptr)
            continue;

        if (mode == SELL)
        {
            const Item *const item2 = inv->findItem(item->getId(),
                ItemColor_zero);
            if (item2 != nullptr)
            {
                int amount = item->getQuantity();
                if (item2->getQuantity() < amount)
                    amount = item2->getQuantity();

                if (amount != 0)
                {
                    data.append(strprintf("%s%s%s",
                        encodeStr(item->getId(), 2).c_str(),
                        encodeStr(item->getPrice(), 4).c_str(),
                        encodeStr(amount, 3).c_str()));
                }
            }
        }
        else
        {
            int amount = item->getQuantity();
            if (item->getPrice() * amount > PlayerInfo::getAttribute(
                Attributes::MONEY))
            {
                amount = PlayerInfo::getAttribute(Attributes::MONEY)
                    / item->getPrice();
            }

            if (amount > 0)
            {
                data.append(strprintf("%s%s%s",
                    encodeStr(item->getId(), 2).c_str(),
                    encodeStr(item->getPrice(), 4).c_str(),
                    encodeStr(amount, 3).c_str()));
            }
        }
    }
    sendMessage(nick, data, true);
}

void ShopWindow::sendMessage(const std::string &nick,
                             std::string data,
                             const bool random)
{
    if (chatWindow == nullptr)
        return;

    if (random)
    {
        mRandCounter ++;
        if (mRandCounter > 200)
            mRandCounter = 0;
        data.append(encodeStr(mRandCounter, 2));
    }

    if (config.getBoolValue("hideShopMessages"))
        chatHandler->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, ChatMsgType::BY_PLAYER);
}

void ShopWindow::showList(const std::string &nick, std::string data)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    BuyDialog *buyDialog = nullptr;
    SellDialog *sellDialog = nullptr;
    if (data.find("B1") == 0)
    {
        data = data.substr(2);
        CREATEWIDGETV(buyDialog, BuyDialog, nick, DEFAULT_CURRENCY);
    }
    else if (data.find("S1") == 0)
    {
        data = data.substr(2);
        CREATEWIDGETV(sellDialog, ShopSellDialog, nick);
    }
    else
    {
        return;
    }

    if (buyDialog != nullptr)
        buyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));
    if (sellDialog != nullptr)
        sellDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    for (unsigned f = 0; f < data.length(); f += 9)
    {
        if (f + 9 > data.length())
            break;

        const int id = decodeStr(data.substr(f, 2));
        const int price = decodeStr(data.substr(f + 2, 4));
        int amount = decodeStr(data.substr(f + 6, 3));
        if (buyDialog != nullptr && amount > 0)
        {
            buyDialog->addItem(id,
                ItemType::Unknown,
                ItemColor_one,
                amount,
                price);
        }
        if (sellDialog != nullptr)
        {
            const Item *const item = inv->findItem(id, ItemColor_zero);
            if (item != nullptr)
            {
                if (item->getQuantity() < amount)
                    amount = item->getQuantity();
            }
            else
            {
                amount = 0;
            }
            ShopItem *const shopItem = sellDialog->addItem(id,
                ItemType::Unknown,
                ItemColor_one,
                amount,
                price);

            if (shopItem != nullptr && amount <= 0)
                shopItem->setDisabled(true);
        }
    }
    if (buyDialog != nullptr)
        buyDialog->sort();
}

void ShopWindow::processRequest(const std::string &nick,
                                std::string data,
                                const int mode)
{
    if (localPlayer == nullptr ||
        !mTradeNick.empty() ||
        PlayerInfo::isTrading() == Trading_true ||
        actorManager == nullptr ||
        actorManager->findBeingByName(nick, ActorType::Player) == nullptr)
    {
        return;
    }

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    const size_t idx = data.find(' ');
    if (idx == std::string::npos)
        return;

    if (!checkFloodCounter(mLastRequestTimeItem))
        return;

    if (!mTradeNick.empty())
    {
        sendMessage(nick,
            // TRANSLATORS: error buy/sell shop request
            _("error: player busy") + std::string(" "),
            true);
        return;
    }

    data = data.substr(idx + 1);

    std::string part1;
    std::string part2;
    std::string part3;
    std::stringstream ss(data);
    int id;
    int price;
    int amount;

    if (!(ss >> part1))
        return;

    if (!(ss >> part2))
        return;

    if (!(ss >> part3))
        return;

    id = atoi(part1.c_str());
    price = atoi(part2.c_str());
    amount = atoi(part3.c_str());

    delete mTradeItem;
    // +++ need impliment colors?
    mTradeItem = new ShopItem(-1,
        id,
        ItemType::Unknown,
        ItemColor_one,
        amount,
        price,
        DEFAULT_CURRENCY);

    if (mode == BUY)
    {
        // +++ need support for colors
        const Item *const item2 = inv->findItem(mTradeItem->getId(),
            ItemColor_zero);
        if (item2 == nullptr ||
            item2->getQuantity() < amount ||
            !findShopItem(mTradeItem, SELL))
        {
            sendMessage(nick,
                // TRANSLATORS: error buy/sell shop request
                _("error: Can't sell this item") + std::string(" "),
                true);
            return;
        }
        mTradeMoney = 0;
    }
    else
    {
        if (!findShopItem(mTradeItem, BUY))
        {
            sendMessage(nick,
                // TRANSLATORS: error buy/sell shop request
                _("error: Can't buy this item") + std::string(" "),
                true);
            return;
        }
        mTradeMoney = mTradeItem->getPrice() * mTradeItem->getQuantity();
    }

    mTradeNick = nick;

    if (config.getBoolValue("autoShop"))
    {
        soundManager.playGuiSound(SOUND_TRADE);
        startTrade();
    }
    else
    {
        std::string msg;
        if (mode == BUY)
        {
            // TRANSLATORS: buy shop request (nick, item)
            msg = strprintf(_("%s wants to buy %s do you accept?"),
                nick.c_str(),
                mTradeItem->getInfo().getName().c_str());
        }
        else
        {
            // TRANSLATORS: sell shop request (nick, item)
            msg = strprintf(_("%s wants to sell %s do you accept?"),
                nick.c_str(),
                mTradeItem->getInfo().getName().c_str());
        }

        ConfirmDialog *const confirmDlg = CREATEWIDGETR(ConfirmDialog,
            // TRANSLATORS: shop window dialog
            _("Request for Trade"),
            msg,
            SOUND_REQUEST,
            true,
            Modal_false,
            nullptr);
        confirmDlg->addActionListener(this);
    }
}

void ShopWindow::updateTimes()
{
    BLOCK_START("ShopWindow::updateTimes")
    if (mAnnounceButton == nullptr)
    {
        BLOCK_END("ShopWindow::updateTimes")
        return;
    }
    if (mAnnonceTime + (2 * 60) < cur_time ||
        mAnnonceTime > cur_time)
    {
        mAnnounceButton->setEnabled(true);
    }
    BLOCK_END("ShopWindow::updateTimes")
}

bool ShopWindow::checkFloodCounter(time_t &counterTime)
{
    if (counterTime == 0 || counterTime > cur_time)
        counterTime = cur_time;
    else if (counterTime + 10 > cur_time)
        return false;
    else
        counterTime = cur_time;
    return true;
}

bool ShopWindow::findShopItem(const ShopItem *const shopItem,
                              const int mode) const
{
    if (shopItem == nullptr)
        return false;

    STD_VECTOR<ShopItem*> items;
    if (mode == SELL)
        items = mSellShopItems->items();
    else
        items = mBuyShopItems->items();

    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *(it);
        if (item == nullptr)
            continue;

        if (item->getId() == shopItem->getId()
            && item->getPrice() == shopItem->getPrice()
            && item->getQuantity() >= shopItem->getQuantity())
        {
            return true;
        }
    }
    return false;
}
#endif  // TMWA_SUPPORT

int ShopWindow::sumAmount(const Item *const shopItem)
{
    if ((localPlayer == nullptr) || (shopItem == nullptr))
        return 0;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return 0;
    int sum = 0;

    for (unsigned f = 0; f < inv->getSize(); f ++)
    {
        const Item *const item = inv->getItem(f);
        if ((item != nullptr) && item->getId() == shopItem->getId())
            sum += item->getQuantity();
    }
    return sum;
}

bool ShopWindow::isShopEmpty() const
{
    if (mBuyShopItems->empty() && mSellShopItems->empty())
        return true;
    return false;
}

void ShopWindow::updateSelection()
{
    if (isBuySelected)
        mCurrentShopItemList = mBuyShopItemList;
    else
        mCurrentShopItemList = mSellShopItemList;
    mScrollArea->setContent(mCurrentShopItemList);
    updateButtonsAndLabels();
}

void ShopWindow::updateShopName()
{
    if (mSellShopName.empty())
    {
        // TRANSLATORS: shop window name
        setCaption(_("Personal Shop"));
    }
    else
    {
        // TRANSLATORS: shop window name
        setCaption(strprintf(_("Personal Shop - %s"), mSellShopName.c_str()));
    }
}

void ShopWindow::setShopName(const std::string &name)
{
    mSellShopName = name;
    serverConfig.setValue("sellShopName", mSellShopName);
    updateShopName();
}

void ShopWindow::vendingSlotsChanged(const int slots)
{
    mSellShopSize = slots;
    updateButtonsAndLabels();
}

void ShopWindow::vendingEnabled(const bool b)
{
    mEnableVending = b;
    localPlayer->enableShop(b);
    if (!b)
        mSellShopSize = 0;
    updateButtonsAndLabels();
}

void ShopWindow::buyingStoreSlotsChanged(const int slots)
{
    mBuyShopSize = slots;
    updateButtonsAndLabels();
}

void ShopWindow::buyingStoreEnabled(const bool b)
{
    mEnableBuyingStore = b;
    localPlayer->enableShop(b);
    if (!b)
        mBuyShopSize = 0;
    updateButtonsAndLabels();
}
