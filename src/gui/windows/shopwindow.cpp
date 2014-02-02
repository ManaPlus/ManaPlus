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

#include "gui/windows/buydialog.h"
#include "gui/windows/confirmdialog.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/selldialog.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shopitems.h"
#include "gui/widgets/shoplistbox.h"

#include "gui/widgets/tabs/chattab.h"

#include "actormanager.h"
#include "auctionmanager.h"
#include "client.h"
#include "configuration.h"
#include "inventory.h"
#include "item.h"
#include "shopitem.h"
#include "soundconsts.h"
#include "soundmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "net/net.h"
#include "net/chathandler.h"
#include "net/tradehandler.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"

#include <sstream>

#include <sys/stat.h>

#include "debug.h"

extern std::string tradePartnerName;
ShopWindow::DialogList ShopWindow::instances;

ShopWindow::ShopWindow():
    // TRANSLATORS: shop window name
    Window(_("Personal Shop"), false, nullptr, "shop.xml"),
    gcn::ActionListener(),
    gcn::SelectionListener(),
    // TRANSLATORS: shop window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    mBuyShopItems(new ShopItems),
    mSellShopItems(new ShopItems),
    mBuyShopItemList(new ShopListBox(this, mBuyShopItems, mBuyShopItems)),
    mSellShopItemList(new ShopListBox(this, mSellShopItems, mSellShopItems)),
    mBuyScrollArea(new ScrollArea(mBuyShopItemList,
        getOptionBool("showbuybackground"), "shop_buy_background.xml")),
    mSellScrollArea(new ScrollArea(mSellShopItemList,
        getOptionBool("showsellbackground"), "shop_sell_background.xml")),
    // TRANSLATORS: shop window label
    mBuyLabel(new Label(this, _("Buy items"))),
    // TRANSLATORS: shop window label
    mSellLabel(new Label(this, _("Sell items"))),
    // TRANSLATORS: shop window label
    mBuyAddButton(new Button(this, _("Add"), "add buy", this)),
    // TRANSLATORS: shop window label
    mBuyDeleteButton(new Button(this, _("Delete"), "delete buy", this)),
    // TRANSLATORS: shop window label
    mBuyAnnounceButton(new Button(this, _("Announce"), "announce buy", this)),
    mBuyAuctionButton(nullptr),
    // TRANSLATORS: shop window button
    mSellAddButton(new Button(this, _("Add"), "add sell", this)),
    // TRANSLATORS: shop window button
    mSellDeleteButton(new Button(this, _("Delete"), "delete sell", this)),
    // TRANSLATORS: shop window button
    mSellAnnounceButton(new Button(this, _("Announce"),
        "announce sell", this)),
    mSellAuctionButton(nullptr),
    // TRANSLATORS: shop window checkbox
    mAnnounceLinks(new CheckBox(this, _("Show links in announce"), false,
        this, "link announce")),
    mSelectedItem(-1),
    mAnnonceTime(0),
    mLastRequestTimeList(0),
    mLastRequestTimeItem(0),
    mRandCounter(0),
    mAcceptPlayer(""),
    mTradeItem(nullptr),
    mTradeNick(""),
    mTradeMoney(0)
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

    mAnnounceCounter[BUY] = 0;
    mAnnounceCounter[SELL] = 0;

    loadList();

    mBuyShopItemList->setPriceCheck(false);
    mSellShopItemList->setPriceCheck(false);

    mBuyScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mSellScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mBuyShopItemList->addSelectionListener(this);
    mSellShopItemList->addSelectionListener(this);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mBuyLabel, 8).setPadding(3);
    placer(8, 0, mSellLabel, 8).setPadding(3);
    placer(0, 1, mBuyScrollArea, 8, 5).setPadding(3);
    placer(8, 1, mSellScrollArea, 8, 5).setPadding(3);
    placer(0, 6, mBuyAddButton);
    placer(1, 6, mBuyDeleteButton);
    placer(3, 6, mBuyAnnounceButton);
    placer(8, 6, mSellAddButton);
    placer(9, 6, mSellDeleteButton);
    placer(11, 6, mSellAnnounceButton);
    placer(0, 7, mAnnounceLinks, 8);
    placer(15, 7, mCloseButton);

    if (auctionManager && auctionManager->getEnableAuctionBot())
    {
        mBuyAuctionButton = new Button(this,
            // TRANSLATORS: shop window button
            _("Auction"), "auction buy", this);
        mSellAuctionButton = new Button(this,
            // TRANSLATORS: shop window button
            _("Auction"), "auction sell", this);
        placer(4, 6, mBuyAuctionButton);
        placer(12, 6, mSellAuctionButton);
    }
    else
    {
        mBuyAuctionButton = nullptr;
        mSellAuctionButton = nullptr;
    }

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    center();
    loadWindowState();

    instances.push_back(this);
}

void ShopWindow::postInit()
{
    setVisible(false);
    enableVisibleSound(true);
    updateButtonsAndLabels();
}

ShopWindow::~ShopWindow()
{
    saveList();

    delete mBuyShopItems;
    mBuyShopItems = nullptr;

    delete mSellShopItems;
    mSellShopItems = nullptr;

    instances.remove(this);
}

void ShopWindow::action(const gcn::ActionEvent &event)
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
    else if (eventId == "delete buy" && mBuyShopItemList
             && mBuyShopItemList->getSelected() >= 0)
    {
        mBuyShopItems->del(mBuyShopItemList->getSelected());
        if (isShopEmpty() && player_node)
            player_node->updateStatus();
    }
    else if (eventId == "delete sell" && mSellShopItemList
             && mSellShopItemList->getSelected() >= 0)
    {
        mSellShopItems->del(mSellShopItemList->getSelected());
        if (isShopEmpty() && player_node)
            player_node->updateStatus();
    }
    else if (eventId == "announce buy" && mBuyShopItems
             && mBuyShopItems->getNumberOfElements() > 0)
    {
        announce(mBuyShopItems, BUY);
    }
    else if (eventId == "announce sell" && mSellShopItems
             && mSellShopItems->getNumberOfElements() > 0)
    {
        announce(mSellShopItems, SELL);
    }
    else if (eventId == "auction buy" && mBuyShopItems
             && mBuyShopItems->getNumberOfElements() > 0)
    {
        Net::getChatHandler()->privateMessage("AuctionBot", "!pull4144 seek");
    }
    else if (eventId == "auction sell" && mSellShopItems
             && mSellShopItems->getNumberOfElements() > 0)
    {
        Net::getChatHandler()->privateMessage("AuctionBot", "!pull4144 offer");
    }

    if (mSelectedItem < 1)
        return;

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    // +++ need support for colors
    Item *const item = inv->findItem(mSelectedItem, 0);
    if (item)
    {
        if (eventId == "add buy")
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::ShopBuyAdd,
                                         this, item, sumAmount(item));
        }
        else if (eventId == "add sell")
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::ShopSellAdd,
                                         this, item, sumAmount(item));
        }
    }
}

void ShopWindow::startTrade()
{
    if (!actorManager || !tradeWindow)
        return;

    const Being *const being = actorManager->findBeingByName(
        mTradeNick, Being::PLAYER);
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
    Net::getTradeHandler()->request(being);
    tradePartnerName = mTradeNick;
    mTradeNick.clear();
}

void ShopWindow::valueChanged(const gcn::SelectionEvent &event A_UNUSED)
{
    updateButtonsAndLabels();
}

void ShopWindow::updateButtonsAndLabels()
{
    mBuyAddButton->setEnabled(mSelectedItem != -1);
    mSellAddButton->setEnabled(mSelectedItem != -1);
    mBuyDeleteButton->setEnabled(
            mBuyShopItemList->getSelected() != -1
            && mBuyShopItems->getNumberOfElements() > 0);
    mSellDeleteButton->setEnabled(
            mSellShopItemList->getSelected() != -1
            && mSellShopItems->getNumberOfElements() > 0);
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
        item->getColor(), amount, price);
    if (emp && player_node)
        player_node->updateStatus();

    updateButtonsAndLabels();
}

void ShopWindow::addSellItem(const Item *const item, const int amount,
                             const int price)
{
    if (!mBuyShopItems || !item)
        return;
    const bool emp = isShopEmpty();
    mSellShopItems->addItemNoDup(item->getId(),
        item->getColor(), amount, price);
    if (emp && player_node)
        player_node->updateStatus();

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

    const std::string shopListName = client->getServerConfigDirectory()
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
                            tokens[0], 1, tokens[1], tokens[2]);
                    }
                    if (tokens[3] && tokens[4] && mSellShopItems)
                    {
                        mSellShopItems->addItem(
                            tokens[0], 1, tokens[3], tokens[4]);
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
    const std::string shopListName = client->getServerConfigDirectory()
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

    std::string data("\302\202");
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
    if (mBuyAnnounceButton)
        mBuyAnnounceButton->setEnabled(false);
    if (mSellAnnounceButton)
        mSellAnnounceButton->setEnabled(false);

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

    Net::getChatHandler()->talk(data, GENERAL_CHANNEL);
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
                PlayerInfo::MONEY))
            {
                amount = PlayerInfo::getAttribute(PlayerInfo::MONEY)
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
        Net::getChatHandler()->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, BY_PLAYER);
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
        sellDialog = new SellDialog(nick);
    }
    else
    {
        return;
    }

    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    if (buyDialog)
        buyDialog->setMoney(PlayerInfo::getAttribute(PlayerInfo::MONEY));
    if (sellDialog)
        sellDialog->setMoney(PlayerInfo::getAttribute(PlayerInfo::MONEY));

    for (unsigned f = 0; f < data.length(); f += 9)
    {
        if (f + 9 > data.length())
            break;

        const int id = decodeStr(data.substr(f, 2));
        const int price = decodeStr(data.substr(f + 2, 4));
        int amount = decodeStr(data.substr(f + 6, 3));
        // +++ need impliment colors?
        if (buyDialog && amount > 0)
            buyDialog->addItem(id, 1, amount, price);
        if (sellDialog)
        {
            // +++ need support for colors
            const Item *const item = inv->findItem(id, 0);
            if (item)
            {
                if (item->getQuantity() < amount)
                    amount = item->getQuantity();
                if (amount > 0)
                    sellDialog->addItem(id, 1, amount, price);
                else
                    sellDialog->addItem(id, 1, -1, price);
            }
        }
    }
    if (buyDialog)
        buyDialog->sort();
}

void ShopWindow::processRequest(const std::string &nick, std::string data,
                                const int mode)
{
    if (!player_node || !mTradeNick.empty() || PlayerInfo::isTrading()
        || !actorManager
        || !actorManager->findBeingByName(nick, Being::PLAYER))
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
    mTradeItem = new ShopItem(-1, id, 1, amount, price);

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
    if (mAnnonceTime + (2 * 60) < cur_time
        || mAnnonceTime > cur_time)
    {
        mBuyAnnounceButton->setEnabled(true);
        mSellAnnounceButton->setEnabled(true);
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
    if (!player_node || !shopItem)
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
