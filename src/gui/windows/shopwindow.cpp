/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "gui/chatconsts.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/confirmdialog.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/shopselldialog.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/models/shopitems.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/tabstrip.h"

#include "actormanager.h"
#include "configuration.h"
#include "inventory.h"
#include "item.h"
#include "settings.h"
#include "shopitem.h"
#include "soundmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "enums/being/attributes.h"

#include "net/chathandler.h"
#include "net/serverfeatures.h"
#include "net/tradehandler.h"
#include "net/vendinghandler.h"

#include "resources/iteminfo.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <sstream>

#include <sys/stat.h>

#include "debug.h"

ShopWindow *shopWindow = nullptr;
extern std::string tradePartnerName;
ShopWindow::DialogList ShopWindow::instances;

ShopWindow::ShopWindow() :
    // TRANSLATORS: shop window name
    Window(_("Personal Shop"), false, nullptr, "shop.xml"),
    ActionListener(),
    SelectionListener(),
    VendingModeListener(),
    VendingSlotsListener(),
    // TRANSLATORS: shop window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    mBuyShopItems(new ShopItems),
    mSellShopItems(new ShopItems),
    mTradeItem(nullptr),
    mBuyShopItemList(new ShopListBox(this, mBuyShopItems, mBuyShopItems)),
    mSellShopItemList(new ShopListBox(this, mSellShopItems, mSellShopItems)),
    mCurrentShopItemList(nullptr),
    mScrollArea(new ScrollArea(this, mBuyShopItemList,
        getOptionBool("showbuybackground"), "shop_buy_background.xml")),
    // TRANSLATORS: shop window label
    mAddButton(new Button(this, _("Add"), "add", this)),
    // TRANSLATORS: shop window label
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    mAnnounceButton(nullptr),
    mPublishButton(nullptr),
    mAnnounceLinks(nullptr),
    mTabs(nullptr),
    mAcceptPlayer(),
    mTradeNick(),
    mSelectedItem(-1),
    mAnnonceTime(0),
    mLastRequestTimeList(0),
    mLastRequestTimeItem(0),
    mRandCounter(0),
    mTradeMoney(0),
    mSellShopSize(0),
    isBuySelected(true),
    mHaveVending(serverFeatures->haveVending()),
    mEnableVending(false)
{
    mBuyShopItemList->postInit();
    mSellShopItemList->postInit();

    setWindowName("Personal Shop");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setMinWidth(260);
    setMinHeight(220);
    if (mainGraphics->mWidth > 600)
        setDefaultSize(500, 300, ImageRect::CENTER);
    else
        setDefaultSize(380, 300, ImageRect::CENTER);

    if (setupWindow)
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


    loadList();

    mBuyShopItemList->setPriceCheck(false);
    mSellShopItemList->setPriceCheck(false);

    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mBuyShopItemList->addSelectionListener(this);
    mSellShopItemList->addSelectionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mTabs, 8).setPadding(3);

    if (isBuySelected)
    {
        // TRANSLATORS: shop window button
        mPublishButton = new Button(this, _("Publish"), "publish", this);
        placer(2, 6, mPublishButton);
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
    placer(7, 7, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    center();
    loadWindowState();

    instances.push_back(this);
}

void ShopWindow::postInit()
{
    setVisible(false);
    enableVisibleSound(true);
    updateSelection();
}

ShopWindow::~ShopWindow()
{
    saveList();

    delete2(mBuyShopItems);
    delete2(mSellShopItems);

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
        player_relations.ignoreTrade(mTradeNick);
        mTradeNick.clear();
    }
    else if (eventId == "delete")
    {
        if (isBuySelected)
        {
            if (mBuyShopItemList && mBuyShopItemList->getSelected() >= 0)
            {
                mBuyShopItems->del(mBuyShopItemList->getSelected());
                if (isShopEmpty() && localPlayer)
                    localPlayer->updateStatus();
            }
        }
        else if (mSellShopItemList
                 && mSellShopItemList->getSelected() >= 0)
        {
            mSellShopItems->del(mSellShopItemList->getSelected());
            if (isShopEmpty() && localPlayer)
                localPlayer->updateStatus();
        }
    }
    else if (eventId == "announce")
    {
        if (isBuySelected)
        {
            if (mBuyShopItems && mBuyShopItems->getNumberOfElements() > 0)
                announce(mBuyShopItems, BUY);
        }
        else if (mSellShopItems && mSellShopItems->getNumberOfElements() > 0)
        {
            announce(mSellShopItems, SELL);
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
        if (mEnableVending)
        {
            vendingHandler->close();
            VendingModeListener::distributeEvent(false);
        }
        else
        {
            std::vector<ShopItem*> &oldItems = mSellShopItems->items();
            std::vector<ShopItem*> items;
            Inventory *const inv = PlayerInfo::getCartInventory();
            if (!inv)
                return;
            FOR_EACH (std::vector<ShopItem*>::iterator, it, oldItems)
            {
                ShopItem *const item = *it;
                if (!item)
                    continue;
                // +++ need add colors
                Item *const cartItem = inv->findItem(item->getId(), 1);
                if (!cartItem)
                    continue;
                item->setInvIndex(cartItem->getInvIndex());
                const int amount = cartItem->getQuantity();
                if (!amount)
                    continue;
                if (item->getQuantity() > amount)
                    item->setQuantity(amount);
                items.push_back(item);
                if (static_cast<signed int>(items.size()) >= mSellShopSize)
                    break;
            }
            if (!items.empty())
                vendingHandler->createShop("test shop", true, items);
        }
    }

    if (mSelectedItem < 1)
        return;

    const Inventory *const inv = mHaveVending
        ? PlayerInfo::getCartInventory() : PlayerInfo::getInventory();
    if (!inv)
        return;

    // +++ need support for colors
    Item *const item = inv->findItem(mSelectedItem, 0);
    if (item)
    {
        if (eventId == "add")
        {
            if (isBuySelected)
            {
                ItemAmountWindow::showWindow(ItemAmountWindow::ShopBuyAdd,
                    this, item, sumAmount(item));
            }
            else
            {
                ItemAmountWindow::showWindow(ItemAmountWindow::ShopSellAdd,
                    this, item, sumAmount(item));
            }
        }
    }
}

void ShopWindow::startTrade()
{
    if (!actorManager || !tradeWindow)
        return;

    const Being *const being = actorManager->findBeingByName(
        mTradeNick, ActorType::Player);
    tradeWindow->clear();
    if (mTradeMoney)
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

void ShopWindow::valueChanged(const SelectionEvent &event A_UNUSED)
{
    updateButtonsAndLabels();
}

void ShopWindow::updateButtonsAndLabels()
{
    mAddButton->setEnabled(mSelectedItem != -1);
    bool allowDel(false);
    const bool sellNotEmpty = mSellShopItems->getNumberOfElements() > 0;
    if (isBuySelected)
    {
        allowDel = mBuyShopItemList->getSelected() != -1
            && mBuyShopItems->getNumberOfElements() > 0;
    }
    else
    {
        allowDel = mSellShopItemList->getSelected() != -1 && sellNotEmpty;
    }
    mDeleteButton->setEnabled(allowDel);
    if (mPublishButton)
    {
        if (mEnableVending)
            mPublishButton->setCaption(_("Close shop"));
        else
            mPublishButton->setCaption(_("Publish"));
        mPublishButton->adjustSize();
        if (!isBuySelected
            && sellNotEmpty
            && mSellShopSize > 0
            && localPlayer
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

void ShopWindow::setVisible(bool visible)
{
    Window::setVisible(visible);
}

void ShopWindow::addBuyItem(const Item *const item, const int amount,
                            const int price)
{
    if (!mBuyShopItems || !item)
        return;
    const bool emp = isShopEmpty();
    mBuyShopItems->addItemNoDup(item->getId(),
        item->getType(),
        item->getColor(),
        amount,
        price);
    if (emp && localPlayer)
        localPlayer->updateStatus();

    updateButtonsAndLabels();
}

void ShopWindow::addSellItem(const Item *const item, const int amount,
                             const int price)
{
    if (!mBuyShopItems || !item)
        return;
    const bool emp = isShopEmpty();
    mSellShopItems->addItemNoDup(item->getId(),
        item->getType(),
        item->getColor(),
        amount,
        price);
    if (emp && localPlayer)
        localPlayer->updateStatus();

    updateButtonsAndLabels();
}

void ShopWindow::loadList()
{
    if (!mBuyShopItems || !mSellShopItems)
        return;

    std::ifstream shopFile;
    struct stat statbuf;

    mBuyShopItems->clear();
    mSellShopItems->clear();

    const std::string shopListName = settings.serverConfigDir
        + "/shoplist.txt";

    if (!stat(shopListName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        shopFile.open(shopListName.c_str(), std::ios::in);
        if (!shopFile.is_open())
        {
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
                std::vector<int> tokens;
                std::stringstream ss(str);

                while (ss >> buf)
                    tokens.push_back(atoi(buf.c_str()));

                if (tokens.size() == 5 && tokens[0])
                {
                    // +++ need impliment colors?
                    if (tokens[1] && tokens[2] && mBuyShopItems)
                    {
                        mBuyShopItems->addItem(
                            tokens[0], 0, 1, tokens[1], tokens[2]);
                    }
                    if (tokens[3] && tokens[4] && mSellShopItems)
                    {
                        mSellShopItems->addItem(
                            tokens[0], 0, 1, tokens[3], tokens[4]);
                    }
                }
            }
        }
        shopFile.close();
    }
}

void ShopWindow::saveList() const
{
    if (!mBuyShopItems || !mSellShopItems)
        return;

    std::ofstream shopFile;
    const std::string shopListName = settings.serverConfigDir
        + "/shoplist.txt";
    std::map<int, ShopItem*> mapItems;

    shopFile.open(shopListName.c_str(), std::ios::binary);
    if (!shopFile.is_open())
    {
        logger->log1("Unable to open shoplist.txt for writing");
        return;
    }

    std::vector<ShopItem*> items = mBuyShopItems->items();
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        ShopItem *const item = *(it);
        if (item)
            mapItems[item->getId()] = item;
    }

    items = mSellShopItems->items();
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        if (!(*it))
            continue;
        const ShopItem *const sellItem = *(it);
        const ShopItem *const buyItem = mapItems[sellItem->getId()];

        shopFile << sellItem->getId();
        if (buyItem)
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
         mapIt_end = mapItems.end(); mapIt != mapIt_end; ++mapIt)
    {
        const ShopItem *const buyItem = (*mapIt).second;
        if (buyItem)
        {
            shopFile << buyItem->getId();
            shopFile << strprintf(" %d %d ", buyItem->getQuantity(),
                                  buyItem->getPrice());
            shopFile << "0 0" << std::endl;
        }
    }

    shopFile.close();
}

void ShopWindow::announce(ShopItems *const list, const int mode)
{
    if (!list)
        return;

    std::string data;
    if (mode == BUY)
        data.append("Buy ");
    else
        data.append("Sell ");

    if (mAnnonceTime && (mAnnonceTime + (2 * 60) > cur_time
        || mAnnonceTime > cur_time))
    {
        return;
    }

    mAnnonceTime = cur_time;
    if (mAnnounceButton)
        mAnnounceButton->setEnabled(false);

    std::vector<ShopItem*> items = list->items();

    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
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
    if (!list)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    std::vector<ShopItem*> items = list->items();

    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *(it);
        if (!item)
            continue;

        if (mode == SELL)
        {
            // +++ need support for colors
            const Item *const item2 = inv->findItem(item->getId(), 0);
            if (item2)
            {
                int amount = item->getQuantity();
                if (item2->getQuantity() < amount)
                    amount = item2->getQuantity();

                if (amount)
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
                             std::string data, const bool random)
{
    if (!chatWindow)
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
    BuyDialog *buyDialog = nullptr;
    SellDialog *sellDialog = nullptr;
    if (data.find("B1") == 0)
    {
        data = data.substr(2);
        buyDialog = new BuyDialog(nick);
    }
    else if (data.find("S1") == 0)
    {
        data = data.substr(2);
        sellDialog = new ShopSellDialog(nick);
        sellDialog->postInit();
    }
    else
    {
        return;
    }

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    if (buyDialog)
        buyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));
    if (sellDialog)
        sellDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    for (unsigned f = 0; f < data.length(); f += 9)
    {
        if (f + 9 > data.length())
            break;

        const int id = decodeStr(data.substr(f, 2));
        const int price = decodeStr(data.substr(f + 2, 4));
        int amount = decodeStr(data.substr(f + 6, 3));
        // +++ need impliment colors?
        if (buyDialog && amount > 0)
            buyDialog->addItem(id, 0, 1, amount, price);
        if (sellDialog)
        {
            // +++ need support for colors
            const Item *const item = inv->findItem(id, 0);
            if (item)
            {
                if (item->getQuantity() < amount)
                    amount = item->getQuantity();
                if (amount > 0)
                    sellDialog->addItem(id, 0, 1, amount, price);
                else
                    sellDialog->addItem(id, 0, 1, -1, price);
            }
        }
    }
    if (buyDialog)
        buyDialog->sort();
}

void ShopWindow::processRequest(const std::string &nick, std::string data,
                                const int mode)
{
    if (!localPlayer || !mTradeNick.empty() || PlayerInfo::isTrading()
        || !actorManager
        || !actorManager->findBeingByName(nick, ActorType::Player))
    {
        return;
    }

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const size_t idx = data.find(" ");
    if (idx == std::string::npos)
        return;

    if (!checkFloodCounter(mLastRequestTimeItem))
        return;

    if (!mTradeNick.empty())
    {
        sendMessage(nick, "error: player busy ", true);
        return;
    }

    data = data.substr(idx + 1);

    std::string part1;
    std::string part2;
    std::string part3;
    std::stringstream ss(data);
    std::string msg;
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
    mTradeItem = new ShopItem(-1, id, 0, 1, amount, price);

    if (mode == BUY)
    {
        // +++ need support for colors
        const Item *const item2 = inv->findItem(mTradeItem->getId(), 0);
        if (!item2 || item2->getQuantity() < amount
            || !findShopItem(mTradeItem, SELL))
        {
            sendMessage(nick, "error: Can't sell this item ", true);
            return;
        }
        msg = "buy";
        mTradeMoney = 0;
    }
    else
    {
        if (!findShopItem(mTradeItem, BUY))
        {
            sendMessage(nick, "error: Can't buy this item ", true);
            return;
        }
        msg = "sell";
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
        ConfirmDialog *const confirmDlg = new ConfirmDialog
            // TRANSLATORS: shop window dialog
            (_("Request for Trade"), strprintf(_("%s wants to %s %s do you "
            "accept?"), nick.c_str(), msg.c_str(),
            mTradeItem->getInfo().getName().c_str()), SOUND_REQUEST, true);
        confirmDlg->postInit();
        confirmDlg->addActionListener(this);
    }
}

void ShopWindow::updateTimes()
{
    BLOCK_START("ShopWindow::updateTimes")
    if (!mAnnounceButton)
    {
        BLOCK_END("ShopWindow::updateTimes")
        return;
    }
    if (mAnnonceTime + (2 * 60) < cur_time
        || mAnnonceTime > cur_time)
    {
        mAnnounceButton->setEnabled(true);
    }
    BLOCK_END("ShopWindow::updateTimes")
}

bool ShopWindow::checkFloodCounter(int &counterTime)
{
    if (!counterTime || counterTime > cur_time)
        counterTime = cur_time;
    else if (counterTime + 10 > cur_time)
        return false;

    counterTime = cur_time;
    return true;
}

bool ShopWindow::findShopItem(const ShopItem *const shopItem,
                              const int mode) const
{
    if (!shopItem)
        return false;

    std::vector<ShopItem*> items;
    if (mode == SELL)
    {
        if (!mSellShopItems)
            return false;
        items = mSellShopItems->items();
    }
    else
    {
        if (!mBuyShopItems)
            return false;
        items = mBuyShopItems->items();
    }

    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *(it);
        if (!item)
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

int ShopWindow::sumAmount(const Item *const shopItem)
{
    if (!localPlayer || !shopItem)
        return 0;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return 0;
    int sum = 0;

    for (unsigned f = 0; f < inv->getSize(); f ++)
    {
        const Item *const item = inv->getItem(f);
        if (item && item->getId() == shopItem->getId())
            sum += item->getQuantity();
    }
    return sum;
}

bool ShopWindow::isShopEmpty() const
{
    if (!mBuyShopItems || !mSellShopItems)
        return true;
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

void ShopWindow::vendingSlotsChanged(const int slots)
{
    mSellShopSize = slots;
}

void ShopWindow::vendingEnabled(const bool b)
{
    mEnableVending = b;
    updateButtonsAndLabels();
    localPlayer->enableShop(b);
}
