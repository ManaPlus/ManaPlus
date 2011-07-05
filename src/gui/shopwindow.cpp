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

#include "gui/shopwindow.h"

#include "gui/buydialog.h"
#include "gui/itemamountwindow.h"
#include "gui/selldialog.h"
#include "gui/setup.h"
#include "gui/tradewindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shopitems.h"
#include "gui/widgets/shoplistbox.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/tradetab.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "confirmdialog.h"
#include "inventory.h"
#include "item.h"
#include "localplayer.h"
#include "playerinfo.h"
#include "playerrelations.h"
#include "shopitem.h"
#include "sound.h"
#include "units.h"

#include "net/net.h"
#include "net/chathandler.h"
#include "net/npchandler.h"
#include "net/tradehandler.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <sstream>

#include <sys/stat.h>

#include "debug.h"

extern std::string tradePartnerName;
ShopWindow::DialogList ShopWindow::instances;

ShopWindow::ShopWindow():
    Window(_("Personal Shop")),
    mSelectedItem(-1),
    mAnnonceTime(0),
    mLastRequestTimeList(0),
    mLastRequestTimeItem(0),
    mRandCounter(0),
    mAcceptPlayer(""),
    mTradeItem(0),
    mTradeNick(""),
    mTradeMoney(0)
{
    setWindowName("Personal Shop");
    setResizable(true);
    setCloseButton(true);
    setMinWidth(260);
    setMinHeight(230);
    setDefaultSize(380, 300, ImageRect::CENTER);

    mBuyShopItems = new ShopItems;
    mSellShopItems = new ShopItems;

    mAnnounceCounter[BUY] = 0;
    mAnnounceCounter[SELL] = 0;

    loadList();

    mBuyShopItemList = new ShopListBox(mBuyShopItems, mBuyShopItems);
    mSellShopItemList = new ShopListBox(mSellShopItems, mSellShopItems);

    mBuyShopItemList->setPriceCheck(false);
    mSellShopItemList->setPriceCheck(false);

    mBuyScrollArea = new ScrollArea(mBuyShopItemList);
    mBuyScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mSellScrollArea = new ScrollArea(mSellShopItemList);
    mSellScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mCloseButton = new Button(_("Close"), "close", this);

    mBuyShopItemList->addSelectionListener(this);
    mSellShopItemList->addSelectionListener(this);

    mBuyLabel = new Label(_("Buy items"));
    mSellLabel = new Label(_("Sell items"));

    mBuyAddButton = new Button(_("Add"), "add buy", this);
    mBuyDeleteButton = new Button(_("Delete"), "delete buy", this);
    mBuyAnnounceButton = new Button(_("Announce"), "announce buy", this);
    mSellAddButton = new Button(_("Add"), "add sell", this);
    mSellDeleteButton = new Button(_("Delete"), "delete sell", this);
    mSellAnnounceButton = new Button(_("Announce"), "announce sell", this);
    mAnnounceLinks = new CheckBox(_("Show links in announce"), false,
                                  this, "link announce");

    ContainerPlacer place;
    place = getPlacer(0, 0);

    place(0, 0, mBuyLabel, 8).setPadding(3);
    place(8, 0, mSellLabel, 8).setPadding(3);
    place(0, 1, mBuyScrollArea, 8, 5).setPadding(3);
    place(8, 1, mSellScrollArea, 8, 5).setPadding(3);
    place(0, 6, mBuyAddButton);
    place(1, 6, mBuyDeleteButton);
    place(3, 6, mBuyAnnounceButton);
    place(8, 6, mSellAddButton);
    place(9, 6, mSellDeleteButton);
    place(11, 6, mSellAnnounceButton);
    place(0, 7, mAnnounceLinks, 8);
    place(15, 7, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    center();
    loadWindowState();

    instances.push_back(this);
    setVisible(false);

    updateButtonsAndLabels();
}

ShopWindow::~ShopWindow()
{
    saveList();

    delete mBuyShopItems;
    mBuyShopItems = 0;

    delete mSellShopItems;
    mSellShopItems = 0;

    instances.remove(this);
}

void ShopWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
    {
        close();
        return;
    }

    if (event.getId() == "yes")
    {
        startTrade();
    }
    else if (event.getId() == "no")
    {
        mTradeNick = "";
    }
    else if (event.getId() == "ignore")
    {
        player_relations.ignoreTrade(mTradeNick);
        mTradeNick = "";
    }
    else if (event.getId() == "delete buy" && mBuyShopItemList
             && mBuyShopItemList->getSelected() >= 0)
    {
        mBuyShopItems->del(mBuyShopItemList->getSelected());
    }
    else if (event.getId() == "delete sell" && mSellShopItemList
             && mSellShopItemList->getSelected() >= 0)
    {
        mSellShopItems->del(mSellShopItemList->getSelected());
    }
    else if (event.getId() == "announce buy" && mBuyShopItems
             && mBuyShopItems->getNumberOfElements() > 0)
    {
        announce(mBuyShopItems, BUY);
    }
    else if (event.getId() == "announce sell" && mSellShopItems
             && mSellShopItems->getNumberOfElements() > 0)
    {
        announce(mSellShopItems, SELL);
    }

    if (mSelectedItem < 1)
        return;

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    //+++ need support for colors
    Item *item = inv->findItem(mSelectedItem, 0);
    if (item)
    {
        if (event.getId() == "add buy")
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::ShopBuyAdd,
                                         this, item, sumAmount(item));
        }
        else if (event.getId() == "add sell")
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::ShopSellAdd,
                                         this, item, sumAmount(item));
        }
    }

}

void ShopWindow::startTrade()
{
    if (!actorSpriteManager || !tradeWindow)
        return;

    Being *being = actorSpriteManager->findBeingByName(
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
    mTradeNick = "";
}

void ShopWindow::valueChanged(const gcn::SelectionEvent &event _UNUSED_)
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

void ShopWindow::addBuyItem(Item *item, int amount, int price)
{
    if (!mBuyShopItems || !item)
        return;
    mBuyShopItems->addItemNoDup(item->getId(),
        item->getColor(), amount, price);
    updateButtonsAndLabels();
}

void ShopWindow::addSellItem(Item *item, int amount, int price)
{
    if (!mBuyShopItems || !item)
        return;
    mSellShopItems->addItemNoDup(item->getId(),
        item->getColor(), amount, price);
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

    std::string shopListName = Client::getServerConfigDirectory()
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
            std::string str = line;
            if (!str.empty())
            {
                std::vector<int> tokens;

                std::stringstream ss(str);

                while (ss >> buf)
                    tokens.push_back(atoi(buf.c_str()));

                if (tokens.size() == 5 && tokens[0])
                {
                    //+++ need impliment colors?
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

void ShopWindow::saveList()
{
    if (!mBuyShopItems || !mSellShopItems)
        return;

    std::ofstream shopFile;
    std::string shopListName = Client::getServerConfigDirectory()
        + "/shoplist.txt";
    std::list<int> procesList;
    std::map<int, ShopItem*> mapItems;

    shopFile.open(shopListName.c_str(), std::ios::binary);
    if (!shopFile.is_open())
    {
        logger->log1("Unable to open shoplist.txt for writing");
        return;
    }

    std::vector<ShopItem*> items = mBuyShopItems->items();
    std::vector<ShopItem*>::iterator it;
    for (it = items.begin(); it != items.end(); ++it)
    {
        ShopItem *item = *(it);
        if (item)
            mapItems[item->getId()] = item;
    }

    items = mSellShopItems->items();
    for (it = items.begin(); it != items.end(); ++it)
    {
        ShopItem *sellItem = *(it);
        ShopItem *buyItem = mapItems[sellItem->getId()];

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

        if (sellItem)
        {
            shopFile << strprintf("%d %d", sellItem->getQuantity(),
                                  sellItem->getPrice()) << std::endl;
        }
    }

    std::map<int, ShopItem*>::iterator mapIt;
    for (mapIt = mapItems.begin(); mapIt != mapItems.end(); ++mapIt)
    {
        ShopItem *buyItem = (*mapIt).second;
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

void ShopWindow::announce(ShopItems *list, int mode)
{
    if (!list)
        return;

    std::string data = "\302\202";
    if (mode == BUY)
        data += "Buy ";
    else
        data += "Sell ";

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
    std::vector<ShopItem*>::iterator it;

    for (it = items.begin(); it != items.end(); ++it)
    {
        ShopItem *item = *(it);
        if (item->getQuantity() > 1)
        {
            if (mAnnounceLinks->isSelected())
            {
                data += strprintf("[@@%d|%s@@] (%dGP) %d, ", item->getId(),
                                  item->getInfo().getName().c_str(),
                                  item->getPrice(), item->getQuantity());
            }
            else
            {
                data += strprintf("%s (%dGP) %d, ",
                                  item->getInfo().getName().c_str(),
                                  item->getPrice(), item->getQuantity());
            }
        }
        else
        {
            if (mAnnounceLinks->isSelected())
            {
                data += strprintf("[@@%d|%s@@] (%dGP), ", item->getId(),
                                  item->getInfo().getName().c_str(),
                                  item->getPrice());
            }
            else
            {
                data += strprintf("%s (%dGP), ",
                                  item->getInfo().getName().c_str(),
                                  item->getPrice());
            }
        }
    }

    Net::getChatHandler()->talk(data);
}

void ShopWindow::giveList(const std::string &nick, int mode)
{
    if (!checkFloodCounter(mLastRequestTimeList))
        return;

    std::string data = "\302\202";

    ShopItems *list;
    if (mode == BUY)
    {
        list = mBuyShopItems;
        data += "S1";
    }
    else
    {
        list = mSellShopItems;
        data += "B1";
    }
    if (!list)
        return;

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    std::vector<ShopItem*> items = list->items();
    std::vector<ShopItem*>::iterator it;

    for (it = items.begin(); it != items.end(); ++it)
    {
        ShopItem *item = *(it);
        if (!item)
            continue;

        if (mode == SELL)
        {
            //+++ need support for colors
            Item *item2 = inv->findItem(item->getId(), 0);
            if (item2)
            {
                int amount = item->getQuantity();
                if (item2->getQuantity() < amount)
                    amount = item2->getQuantity();

                if (amount)
                {
                    data += strprintf("%s%s%s",
                        encodeStr(item->getId(), 2).c_str(),
                        encodeStr(item->getPrice(), 4).c_str(),
                        encodeStr(amount, 3).c_str());
                }
            }
        }
        else
        {
            int amount = item->getQuantity();
            if (item->getPrice() * amount > PlayerInfo::getAttribute(MONEY))
                amount = PlayerInfo::getAttribute(MONEY) / item->getPrice();

            if (amount > 0)
            {
                data += strprintf("%s%s%s",
                                  encodeStr(item->getId(), 2).c_str(),
                                  encodeStr(item->getPrice(), 4).c_str(),
                                  encodeStr(amount, 3).c_str());
            }
        }
    }
    sendMessage(nick, data, true);
}

void ShopWindow::sendMessage(const std::string &nick,
                             std::string data, bool random)
{
    if (!chatWindow)
        return;

    if (random)
    {
        mRandCounter ++;
        if (mRandCounter > 200)
            mRandCounter = 0;
        data += encodeStr(mRandCounter, 2);
    }

    if (config.getBoolValue("hideShopMessages"))
        Net::getChatHandler()->privateMessage(nick, data);
    else if (chatWindow)
        chatWindow->whisper(nick, data, BY_PLAYER);
//here was true
}

void ShopWindow::showList(const std::string &nick, std::string data)
{
    BuyDialog *buyDialog = 0;
    SellDialog *sellDialog = 0;
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

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    if (buyDialog)
        buyDialog->setMoney(PlayerInfo::getAttribute(MONEY));
    if (sellDialog)
        sellDialog->setMoney(PlayerInfo::getAttribute(MONEY));

    for (unsigned f = 0; f < data.length(); f += 9)
    {
        if (f + 9 > data.length())
            break;

        int id = decodeStr(data.substr(f, 2));
        int price = decodeStr(data.substr(f + 2, 4));
        int amount = decodeStr(data.substr(f + 6, 3));
        //+++ need impliment colors?
        if (buyDialog && amount > 0)
            buyDialog->addItem(id, 1, amount, price);
        if (sellDialog)
        {
            //+++ need support for colors
            Item *item = inv->findItem(id, 0);
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
}

void ShopWindow::processRequest(std::string nick, std::string data, int mode)
{
    if (!player_node || !mTradeNick.empty() || PlayerInfo::isTrading()
        || !actorSpriteManager
        || !actorSpriteManager->findBeingByName(nick, Being::PLAYER))
    {
        return;
    }

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    unsigned long idx = 0;

    idx = data.find(" ");
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
    //+++ need impliment colors?
    mTradeItem = new ShopItem(-1, id, 1, amount, price);

    if (mode == BUY)
    {
        //+++ need support for colors
        Item *item2 = inv->findItem(mTradeItem->getId(), 0);
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
        sound.playGuiSfx("system/newmessage.ogg");
        startTrade();
    }
    else
    {
        ConfirmDialog *confirmDlg = new ConfirmDialog(_("Request for Trade"),
            strprintf(_("%s wants to %s %s do you "
            "accept?"), nick.c_str(), msg.c_str(),
            mTradeItem->getInfo().getName().c_str()), true);
        confirmDlg->addActionListener(this);
    }
}

void ShopWindow::updateTimes()
{
    if (mAnnonceTime + (2 * 60) < cur_time
        || mAnnonceTime > cur_time)
    {
        mBuyAnnounceButton->setEnabled(true);
        mSellAnnounceButton->setEnabled(true);
    }
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

bool ShopWindow::findShopItem(ShopItem *shopItem, int mode)
{
    if (!shopItem)
        return false;

    std::vector<ShopItem*> items;
    std::vector<ShopItem*>::iterator it;
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

    for (it = items.begin(); it != items.end(); ++it)
    {
        ShopItem *item = *(it);
        if (!item)
            continue;

        if (item && item->getId() == shopItem->getId()
            && item->getPrice() == shopItem->getPrice()
            && item->getQuantity() >= shopItem->getQuantity())
        {
            return true;
        }
    }
    return false;
}

int ShopWindow::sumAmount(Item *shopItem)
{
    if (!player_node || !shopItem)
        return 0;

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return 0;
    int sum = 0;

    for (unsigned f = 0; f < inv->getSize(); f ++)
    {
        Item *item = inv->getItem(f);
        if (item && item->getId() == shopItem->getId())
            sum += item->getQuantity();
    }
    return sum;
}
