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

#include "gui/popupmenu.h"

#include "actorspritemanager.h"
#include "being.h"
#include "dropshortcut.h"
#include "guild.h"
#include "flooritem.h"
#include "graphics.h"
#include "item.h"
#include "itemshortcut.h"
#include "localplayer.h"
#include "log.h"
#include "map.h"
#include "party.h"
#include "playerinfo.h"
#include "playerrelations.h"
#include "spellmanager.h"

#include "gui/buydialog.h"
#include "gui/chatwindow.h"
#include "gui/inventorywindow.h"
#include "gui/itemamountwindow.h"
#include "gui/ministatus.h"
#include "gui/outfitwindow.h"
#include "gui/sell.h"
#include "gui/socialwindow.h"
#include "gui/textcommandeditor.h"
#include "gui/textdialog.h"
#include "gui/trade.h"
#include "gui/viewport.h"
#include "gui/windowmenu.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/whispertab.h"

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/buysellhandler.h"
#include "net/guildhandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/partyhandler.h"
#include "gui/shortcutwindow.h"
#include "net/tradehandler.h"

#include "resources/itemdb.h"
#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <cassert>

std::string tradePartnerName("");

PopupMenu::PopupMenu():
    Popup("PopupMenu"),
    mBeingId(0),
    mFloorItem(0),
    mItem(0),
    mItemId(0),
    mMapItem(0),
    mTab(0),
    mSpell(0),
    mDialog(0),
    mButton(0),
    mNick("")
{
    mBrowserBox = new BrowserBox;
    mBrowserBox->setPosition(4, 4);
    mBrowserBox->setHighlightMode(BrowserBox::BACKGROUND);
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mRenameListener.setMapItem(0);
    mRenameListener.setDialog(0);

    add(mBrowserBox);
}

void PopupMenu::showPopup(int x, int y, Being *being)
{
    if (!being || !player_node)
        return;

    mBeingId = being->getId();
    mNick = being->getName();
    mBrowserBox->clearRows();

    const std::string &name = mNick;

    mBrowserBox->addRow(name);

    switch (being->getType())
    {
        case ActorSprite::PLAYER:
            {
                // Players can be traded with.
                mBrowserBox->addRow(_("@@trade|Trade@@"));
                // TRANSLATORS: Attacking a player.
                mBrowserBox->addRow(_("@@attack|Attack@@"));
                // TRANSLATORS: Whispering a player.
                mBrowserBox->addRow(_("@@whisper|Whisper@@"));

                mBrowserBox->addRow("##3---");

                mBrowserBox->addRow(_("@@heal|Heal@@"));
                mBrowserBox->addRow("##3---");

                switch (player_relations.getRelation(name))
                {
                    case PlayerRelation::NEUTRAL:
                        mBrowserBox->addRow(_("@@friend|Be friend@@"));
                        mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                        mBrowserBox->addRow(_("@@ignore|Ignore@@"));
                        mBrowserBox->addRow(_("@@erase|Erase@@"));
                        break;

                    case PlayerRelation::FRIEND:
                        mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                        mBrowserBox->addRow(_("@@ignore|Ignore@@"));
                        mBrowserBox->addRow(_("@@erase|Erase@@"));
                        break;

                    case PlayerRelation::DISREGARDED:
                        mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                        mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
                        mBrowserBox->addRow(_("@@erase|Erase@@"));
                        break;

                    case PlayerRelation::IGNORED:
                        mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                        mBrowserBox->addRow(_("@@erase|Erase@@"));
                        break;

                    case PlayerRelation::ERASED:
                        mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                        mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                        mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
                        break;

                    default:
                        break;
                }

                mBrowserBox->addRow("##3---");
                mBrowserBox->addRow(_("@@follow|Follow@@"));
                mBrowserBox->addRow(_("@@imitation|Imitation@@"));

                if (player_node->isInParty())
                {
                    if (player_node->getParty())
                    {
                        if (player_node->getParty()->getName()
                            != being->getPartyName())
                        {
                            mBrowserBox->addRow(
                                _("@@party|Invite to party@@"));
                        }
                        else
                        {
                            mBrowserBox->addRow(
                                _("@@kick party|Kick from party@@"));
                        }
                        mBrowserBox->addRow("##3---");
                    }
                }

                Guild *guild1 = being->getGuild();
                Guild *guild2 = player_node->getGuild();
                if (guild2)
                {
                    if (guild1)
                    {
                        if (guild1->getId() == guild2->getId())
                        {
                            mBrowserBox->addRow(
                                _("@@guild-kick|Kick from guild@@"));
                            mBrowserBox->addRow(
                                _("@@guild-pos|Change pos in guild >@@"));
                        }
                    }
                    else
                    {
                        mBrowserBox->addRow(_("@@guild|Invite to guild@@"));
                    }
                }

                if (player_node->isGM())
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow(_("@@admin-kick|Kick player@@"));
                }
                mBrowserBox->addRow(_("@@nuke|Nuke@@"));
                mBrowserBox->addRow(_("@@move|Move@@"));
                mBrowserBox->addRow(_("@@undress|Undress@@"));

                if (player_relations.getDefault() & PlayerRelation::TRADE)
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow(_("@@buy|Buy@@"));
                    mBrowserBox->addRow(_("@@sell|Sell@@"));
                }
            }
            break;

        case ActorSprite::NPC:
            // NPCs can be talked to (single option, candidate for removal
            // unless more options would be added)
            mBrowserBox->addRow(_("@@talk|Talk@@"));

            mBrowserBox->addRow(_("@@buy|Buy@@"));
            mBrowserBox->addRow(_("@@sell|Sell@@"));
            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow(_("@@move|Move@@"));
            break;

        case ActorSprite::MONSTER:
            {
                // Monsters can be attacked
                mBrowserBox->addRow(_("@@attack|Attack@@"));

                if (player_node->isGM())
                    mBrowserBox->addRow(_("@@admin-kick|Kick@@"));
            }
            break;

        case ActorSprite::UNKNOWN:
        case ActorSprite::FLOOR_ITEM:
        case ActorSprite::PORTAL:
        default:
            /* Other beings aren't interesting... */
            return;
    }
    mBrowserBox->addRow(_("@@name|Add name to chat@@"));

    //mBrowserBox->addRow(strprintf("@@look|%s@@", _("Look To")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, std::list<Being*> &beings)
{
    mBrowserBox->clearRows();
    mBrowserBox->addRow("Players");
    std::list<Being*>::iterator it, it_end;
    for (it = beings.begin(), it_end = beings.end(); it != it_end; ++it)
    {
        Being *being = *it;
        if (!being->getName().empty())
        {
            mBrowserBox->addRow(strprintf(_("@@player_%u|%s >@@"),
                being->getId(), being->getName().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));
    showPopup(x, y);
}

void PopupMenu::showPlayerPopup(int x, int y, std::string nick)
{
    if (nick.empty() || !player_node)
        return;

    mNick = nick;
    mBeingId = 0;
    mBrowserBox->clearRows();

    const std::string &name = mNick;

    mBrowserBox->addRow(name);

    mBrowserBox->addRow(_("@@whisper|Whisper@@"));
    mBrowserBox->addRow("##3---");

    switch (player_relations.getRelation(name))
    {
        case PlayerRelation::NEUTRAL:
            mBrowserBox->addRow(_("@@friend|Be friend@@"));
            mBrowserBox->addRow(_("@@disregard|Disregard@@"));
            mBrowserBox->addRow(_("@@ignore|Ignore@@"));
            mBrowserBox->addRow(_("@@erase|Erase@@"));
            break;

        case PlayerRelation::FRIEND:
            mBrowserBox->addRow(_("@@disregard|Disregard@@"));
            mBrowserBox->addRow(_("@@ignore|Ignore@@"));
            mBrowserBox->addRow(_("@@erase|Erase@@"));
            break;

        case PlayerRelation::DISREGARDED:
            mBrowserBox->addRow(_("@@unignore|Unignore@@"));
            mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
            mBrowserBox->addRow(_("@@erase|Erase@@"));
            break;

        case PlayerRelation::IGNORED:
            mBrowserBox->addRow(_("@@unignore|Unignore@@"));
            mBrowserBox->addRow(_("@@erase|Erase@@"));
            break;

        case PlayerRelation::ERASED:
            mBrowserBox->addRow(_("@@unignore|Unignore@@"));
            mBrowserBox->addRow(_("@@disregard|Disregard@@"));
            mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
            break;

        default:
            break;
    }

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@follow|Follow@@"));
    mBrowserBox->addRow(_("@@imitation|Imitation@@"));


    if (player_node->isInParty() && player_node->getParty())
    {
        PartyMember *member = player_node->getParty()->getMember(mNick);
        if (member)
        {
            mBrowserBox->addRow(_("@@kick party|Kick from party@@"));
            mBrowserBox->addRow("##3---");
        }
    }

    Guild *guild2 = player_node->getGuild();
    if (guild2)
    {
        if (guild2->getMember(mNick))
        {
            mBrowserBox->addRow(_("@@guild-kick|Kick from guild@@"));
            mBrowserBox->addRow(_("@@guild-pos|Change pos in guild >@@"));
        }
        else
        {
            mBrowserBox->addRow(_("@@guild|Invite to guild@@"));
        }
    }

    mBrowserBox->addRow("##3---");
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow(_("@@buy|Buy@@"));
        mBrowserBox->addRow(_("@@sell|Sell@@"));
    }

    mBrowserBox->addRow(_("@@name|Add name to chat@@"));

    //mBrowserBox->addRow(strprintf("@@look|%s@@", _("Look To")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);

}

void PopupMenu::showPopup(int x, int y, FloorItem *floorItem)
{
    if (!floorItem)
        return;

    mFloorItem = floorItem;
    ItemInfo info = floorItem->getInfo();
    mBrowserBox->clearRows();
    std::string name;

    // Floor item can be picked up (single option, candidate for removal)
    if (serverVersion > 0)
        name = info.getName(floorItem->getColor());
    else
        name = info.getName();

    mBrowserBox->addRow(name);
    mBrowserBox->addRow(_("@@pickup|Pick up@@"));
    mBrowserBox->addRow(_("@@chat|Add to chat@@"));

    //mBrowserBox->addRow(strprintf("@@look|%s@@", _("Look To")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, MapItem *mapItem)
{
    if (!mapItem)
        return;

    mMapItem = mapItem;

    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Map Item"));
    mBrowserBox->addRow(_("@@rename map|Rename@@"));
    mBrowserBox->addRow(_("@@remove map|Remove@@"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);
}

void PopupMenu::showOutfitsPopup(int x, int y)
{
    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Outfits"));
    mBrowserBox->addRow(_("@@load old outfits|Load old outfits@@"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);
}

void PopupMenu::showSpellPopup(int x, int y, TextCommand *cmd)
{
    if (!cmd)
        return;

    mBrowserBox->clearRows();

    mSpell = cmd;
    mBrowserBox->addRow(_("Spells"));
    mBrowserBox->addRow(_("@@load old spells|Load old spells@@"));
    mBrowserBox->addRow(_("@@edit spell|Edit spell@@"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(_("@@cancel|Cancel@@"));

    showPopup(x, y);
}

void PopupMenu::showChatPopup(int x, int y, ChatTab *tab)
{
    if (!tab || !actorSpriteManager || !player_node)
        return;

    mTab = tab;

    mBrowserBox->clearRows();

    if (tab->getType() == ChatTab::TAB_WHISPER)
        mBrowserBox->addRow(_("@@chat close|Close@@"));

    mBrowserBox->addRow(strprintf("@@chat clear|%s@@", _("Clear")));
    mBrowserBox->addRow("##3---");

    if (tab->getAllowHighlight())
    {
        mBrowserBox->addRow(strprintf("@@disable highlight|%s@@",
                            _("Disable highlight")));
    }
    else
    {
        mBrowserBox->addRow(strprintf("@@enable highlight|%s@@",
                            _("Enable highlight")));
    }
    if (tab->getRemoveNames())
    {
        mBrowserBox->addRow(strprintf("@@dont remove name|%s@@",
                            _("Dont remove name")));
    }
    else
    {
        mBrowserBox->addRow(strprintf("@@remove name|%s@@",
                            _("Remove name")));
    }
    if (tab->getNoAway())
    {
        mBrowserBox->addRow(strprintf("@@enable away|%s@@",
                            _("Enable away")));
    }
    else
    {
        mBrowserBox->addRow(strprintf("@@disable away|%s@@",
                            _("Disable away")));
    }
    mBrowserBox->addRow("##3---");

    if (tab->getType() == ChatTab::TAB_PARTY)
    {
        mBrowserBox->addRow(_("@@leave party|Leave@@"));
        mBrowserBox->addRow("##3---");
    }

    if (tab->getType() == ChatTab::TAB_WHISPER)
    {
        WhisperTab *wTab = static_cast<WhisperTab*>(tab);
        std::string name = wTab->getNick();

        Being* being  = actorSpriteManager->findBeingByName(
            name, Being::PLAYER);

        if (being)
        {
            mBeingId = being->getId();
            mNick = being->getName();

            mBrowserBox->addRow(_("@@trade|Trade@@"));
            mBrowserBox->addRow(_("@@attack|Attack@@"));

            mBrowserBox->addRow("##3---");

            mBrowserBox->addRow(_("@@heal|Heal@@"));
            mBrowserBox->addRow("##3---");

            switch (player_relations.getRelation(name))
            {
                case PlayerRelation::NEUTRAL:
                    mBrowserBox->addRow(_("@@friend|Be friend@@"));
                    mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                    mBrowserBox->addRow(_("@@ignore|Ignore@@"));
                    mBrowserBox->addRow(_("@@erase|Erase@@"));
                    break;

                case PlayerRelation::FRIEND:
                    mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                    mBrowserBox->addRow(_("@@ignore|Ignore@@"));
                    mBrowserBox->addRow(_("@@erase|Erase@@"));
                    break;

                case PlayerRelation::DISREGARDED:
                    mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                    mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
                    mBrowserBox->addRow(_("@@erase|Erase@@"));
                    break;

                case PlayerRelation::IGNORED:
                    mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                    mBrowserBox->addRow(_("@@erase|Erase@@"));
                    break;

                case PlayerRelation::ERASED:
                    mBrowserBox->addRow(_("@@unignore|Unignore@@"));
                    mBrowserBox->addRow(_("@@disregard|Disregard@@"));
                    mBrowserBox->addRow(_("@@ignore|Completely ignore@@"));
                    break;

                default:
                    break;
            }

            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow(_("@@follow|Follow@@"));
            mBrowserBox->addRow(_("@@imitation|Imitation@@"));
            mBrowserBox->addRow(_("@@move|Move@@"));
            mBrowserBox->addRow(_("@@undress|Undress@@"));

            if (player_relations.getDefault() & PlayerRelation::TRADE)
            {
                mBrowserBox->addRow("##3---");
                mBrowserBox->addRow(_("@@buy|Buy@@"));
                mBrowserBox->addRow(_("@@sell|Sell@@"));
            }

            mBrowserBox->addRow("##3---");

            if (player_node->isInParty())
            {
                if (player_node->getParty())
                {
                    if (!player_node->getParty()->isMember(wTab->getNick()))
                    {
                        mBrowserBox->addRow(_("@@party|Invite to party@@"));
                    }
                    else
                    {
                        mBrowserBox->addRow(
                            _("@@kick party|Kick from party@@"));
                    }
                    mBrowserBox->addRow("##3---");
                }
            }
            Guild *guild1 = being->getGuild();
            Guild *guild2 = player_node->getGuild();
            if (guild2)
            {
                if (guild1)
                {
                    if (guild1->getId() == guild2->getId())
                    {
                        mBrowserBox->addRow(
                            _("@@guild-kick|Kick from guild@@"));
                        mBrowserBox->addRow(
                            _("@@guild-pos|Change pos in guild >@@"));
                    }
                }
                else
                {
                    mBrowserBox->addRow(_("@@guild|Invite to guild@@"));
                }
            }
        }
    }
    mBrowserBox->addRow(strprintf(_("@@cancel|Cancel@@")));

    showPopup(x, y);
}

void PopupMenu::showChangePos(int x, int y)
{
    mBrowserBox->clearRows();
    mBrowserBox->addRow(_("Change guild position"));

    if (!player_node)
        return;

    const Guild *guild = player_node->getGuild();
    if (guild)
    {
        PositionsMap map = guild->getPositions();
        PositionsMap::iterator itr = map.begin();
        PositionsMap::iterator itr_end = map.end();
        for (; itr != itr_end; ++itr)
        {
            mBrowserBox->addRow(strprintf(_("@@guild-pos-%d|%s@@"),
                                itr->first, itr->second.c_str()));
        }
        mBrowserBox->addRow(strprintf(_("@@cancel|Cancel@@")));

        showPopup(x, y);
    }
    else
    {
        mBeingId = 0;
        mFloorItem = 0;
        mItem = 0;
        mMapItem = 0;
        mNick = "";
        setVisible(false);
    }
}

void PopupMenu::handleLink(const std::string &link,
                           gcn::MouseEvent *event _UNUSED_)
{
    if (!actorSpriteManager)
        return;

    Being *being = actorSpriteManager->findBeing(mBeingId);

    // Talk To action
    if (link == "talk" && being && being->canTalk())
    {
        being->talkTo();
    }
    // Trade action
    else if (link == "trade" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        Net::getTradeHandler()->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow)
            tradeWindow->clear();
    }
    else if (link == "buy" && being && mBeingId != 0)
    {
        if (being->getType() == Being::NPC)
            Net::getNpcHandler()->buy(mBeingId);
        else if (being->getType() == Being::PLAYER)
            Net::getBuySellHandler()->requestSellList(being->getName());
    }
    else if (link == "buy" && !mNick.empty())
    {
        Net::getBuySellHandler()->requestSellList(mNick);
    }
    else if (link == "sell" && being && mBeingId != 0)
    {
        if (being->getType() == Being::NPC)
            Net::getNpcHandler()->sell(mBeingId);
        else if (being->getType() == Being::PLAYER)
            Net::getBuySellHandler()->requestBuyList(being->getName());
    }
    else if (link == "sell" && !mNick.empty())
    {
        Net::getBuySellHandler()->requestBuyList(mNick);
    }
    // Attack action
    else if (link == "attack" && being)
    {
        if (player_node)
            player_node->attack(being, true);
    }
    else if (link == "heal" && being && being->getType() != Being::MONSTER)
    {
        actorSpriteManager->heal(player_node, being);
    }
    else if (link == "unignore" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
                                     PlayerRelation::NEUTRAL);
    }
    else if (link == "unignore" && !mNick.empty())
    {
        player_relations.setRelation(mNick,
                                     PlayerRelation::NEUTRAL);
    }
    else if (link == "ignore" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
                                     PlayerRelation::IGNORED);
    }
    else if (link == "ignore" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::IGNORED);
    }
    else if (link == "erase" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(), PlayerRelation::ERASED);
    }
    else if (link == "erase" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::ERASED);
    }
    else if (link == "disregard" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
                                     PlayerRelation::DISREGARDED);
    }
    else if (link == "disregard" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::DISREGARDED);
    }
    else if (link == "friend" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(), PlayerRelation::FRIEND);
    }
    else if (link == "friend" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::FRIEND);
    }
    // Guild action
    else if (link == "guild" && !mNick.empty())
    {
        if (player_node)
        {
            const Guild *guild = player_node->getGuild();
            if (guild)
                Net::getGuildHandler()->invite(guild->getId(), mNick);
        }
    }
    else if (link == "nuke" && being)
    {
        actorSpriteManager->addBlock(being->getId());
        actorSpriteManager->destroy(being);
    }
    // Follow Player action
    else if (link == "follow" && !mNick.empty())
    {
        if (player_node)
            player_node->setFollow(mNick);
    }
    else if (link == "imitation" && !mNick.empty())
    {
        if (player_node)
            player_node->setImitate(mNick);
    }
    // Pick Up Floor Item action
    else if ((link == "pickup") && mFloorItem)
    {
        if (player_node)
            player_node->pickUp(mFloorItem);
    }
    // Look To action
    else if (link == "look")
    {
    }
    else if (link == "use" && mItem)
    {
        if (mItem->isEquipment())
        {
            if (mItem->isEquipped())
                Net::getInventoryHandler()->unequipItem(mItem);
            else
                Net::getInventoryHandler()->equipItem(mItem);
        }
        else
        {
            Net::getInventoryHandler()->useItem(mItem);
        }
    }
    else if (link == "use" && mItemId)
    {
        if (mItemId < SPELL_MIN_ID)
        {
            Inventory *inv = PlayerInfo::getInventory();
            if (inv)
            {
                Item *item = inv->findItem(mItemId);
                if (item)
                {
                    if (item->isEquipment())
                    {
                        if (item->isEquipped())
                            Net::getInventoryHandler()->unequipItem(item);
                        else
                            Net::getInventoryHandler()->equipItem(item);
                    }
                    else
                    {
                        Net::getInventoryHandler()->useItem(item);
                    }
                }
            }
        }
        else if (spellManager)
        {
            spellManager->useItem(mItemId);
        }
    }
    else if (link == "chat")
    {
        if (chatWindow)
        {
            if (mItem)
            {
                if (serverVersion > 0)
                {
                    chatWindow->addItemText(mItem->getInfo().getName(
                        mItem->getColor()));
                }
                else
                {
                    chatWindow->addItemText(mItem->getInfo().getName());
                }
            }
            else if (mFloorItem)
            {
                if (serverVersion > 0)
                {
                    chatWindow->addItemText(mFloorItem->getInfo().getName(
                        mFloorItem->getColor()));
                }
                else
                {
                    chatWindow->addItemText(mFloorItem->getInfo().getName());
                }
            }
        }
    }
    else if (link == "whisper" && !mNick.empty() && chatWindow)
    {
        if (chatWindow)
        {
            if (config.getBoolValue("whispertab"))
                chatWindow->localChatInput("/q " + mNick);
            else
                chatWindow->addInputText("/w \"" + mNick + "\" ");
        }
    }
    else if (link == "move" && being)
    {
        if (player_node)
            player_node->navigateTo(being->getTileX(), being->getTileY());
    }
    else if (link == "split" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit,
                             inventoryWindow, mItem);
    }
    else if (link == "drop" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemDrop,
                             inventoryWindow, mItem);
    }
    else if (link == "store" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd,
                             inventoryWindow, mItem);
    }
    else if (link == "store 10" && mItem)
    {
        int cnt = 10;
        if (cnt > mItem->getQuantity())
            cnt = mItem->getQuantity();
        Net::getInventoryHandler()->moveItem(Inventory::INVENTORY,
            mItem->getInvIndex(), cnt,
            Inventory::STORAGE);
    }
    else if (link == "store half" && mItem)
    {
        Net::getInventoryHandler()->moveItem(Inventory::INVENTORY,
            mItem->getInvIndex(), mItem->getQuantity() / 2,
            Inventory::STORAGE);
    }
    else if (link == "store all" && mItem)
    {
        Net::getInventoryHandler()->moveItem(Inventory::INVENTORY,
            mItem->getInvIndex(), mItem->getQuantity(),
            Inventory::STORAGE);
    }
    else if (link == "addtrade" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::TradeAdd,
                             tradeWindow, mItem);
    }
    else if (link == "addtrade 10" && mItem)
    {
        if (tradeWindow)
        {
            int cnt = 10;
            if (cnt > mItem->getQuantity())
                cnt = mItem->getQuantity();
            tradeWindow->tradeItem(mItem, cnt);
        }
    }
    else if (link == "addtrade half" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity() / 2);
    }
    else if (link == "addtrade all" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity());
    }
    else if (link == "retrieve" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove, mWindow,
                                     mItem);
    }
    else if (link == "retrieve 10" && mItem)
    {
        int cnt = 10;
        if (cnt > mItem->getQuantity())
            cnt = mItem->getQuantity();
        Net::getInventoryHandler()->moveItem(Inventory::STORAGE,
            mItem->getInvIndex(), cnt,
            Inventory::INVENTORY);
    }
    else if (link == "retrieve half" && mItem)
    {
        Net::getInventoryHandler()->moveItem(Inventory::STORAGE,
            mItem->getInvIndex(), mItem->getQuantity() / 2,
            Inventory::INVENTORY);
    }
    else if (link == "retrieve all" && mItem)
    {
        Net::getInventoryHandler()->moveItem(Inventory::STORAGE,
            mItem->getInvIndex(), mItem->getQuantity(),
            Inventory::INVENTORY);
    }
    else if (link == "party" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        Net::getPartyHandler()->invite(being);
    }
    else if (link == "kick party" && being
             && being->getType() == Being::PLAYER)
    {
        Net::getPartyHandler()->kick(being);
    }
    else if (link == "kick party" && !mNick.empty())
    {
        if (player_node && player_node->getParty())
        {
            PartyMember *member = player_node->getParty()->getMember(mNick);
            if (member)
                Net::getPartyHandler()->kick(mNick);
        }
    }
    else if (link == "name" && !mNick.empty())
    {
        const std::string &name = mNick;
        if (chatWindow)
            chatWindow->addInputText(name);
    }
    else if (link == "admin-kick" && being &&
             (being->getType() == ActorSprite::PLAYER ||
             being->getType() == ActorSprite::MONSTER))
    {
        Net::getAdminHandler()->kick(being->getId());
    }
    else if (link == "chat close" && mTab)
    {
        mTab->handleCommand("close", "");
    }
    else if (link == "leave party" && mTab)
    {
        Net::getPartyHandler()->leave();
    }
    else if (link == "chat clear" && mTab)
    {
        if (chatWindow)
            chatWindow->clearTab();
    }
    else if (link == "remove map" && mMapItem)
    {
        if (viewport)
        {
            Map *map = viewport->getCurrentMap();
            if (map)
            {
                SpecialLayer *specialLayer = map->getSpecialLayer();
                if (specialLayer)
                {
                    const int x = mMapItem->getX();
                    const int y = mMapItem->getY();
                    specialLayer->setTile(x, y, MapItem::EMPTY);
                    if (socialWindow)
                        socialWindow->removePortal(x, y);
                }
            }
        }
    }
    else if (link == "rename map" && mMapItem)
    {
        mRenameListener.setMapItem(mMapItem);
        mDialog = new TextDialog(_("Rename map sign          "),
                                 _("Name:                    "));
        mRenameListener.setDialog(mDialog);
        mDialog->setText(mMapItem->getComment());
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&mRenameListener);
    }
    else if (link == "load old outfits")
    {
        if (outfitWindow)
            outfitWindow->load(true);
    }
    else if (link == "load old spells")
    {
        if (spellManager)
        {
            spellManager->load(true);
            spellManager->save();
        }
    }
    else if (link == "load old item shortcuts")
    {
        if (itemShortcutWindow)
        {
            int num = itemShortcutWindow->getTabIndex();
            if (num >= 0 && num < SHORTCUT_TABS && itemShortcut[num])
            {
                itemShortcut[num]->load(true);
                itemShortcut[num]->save();
            }
        }
    }
    else if (link == "load old drop shortcuts")
    {
        if (dropShortcut)
        {
            dropShortcut->load(true);
            dropShortcut->save();
        }
    }
    else if (link == "edit spell" && mSpell)
    {
        new TextCommandEditor(mSpell);
    }
    else if (link == "undress" && being)
    {
        Net::getBeingHandler()->undress(being);
    }
    else if (link == "guild-kick" && !mNick.empty())
    {
        if (player_node)
        {
            const Guild *guild = player_node->getGuild();
            if (guild)
                Net::getGuildHandler()->kick(guild->getMember(mNick));
        }
    }
    else if (link == "enable highlight" && mTab)
    {
        mTab->setAllowHighlight(true);
    }
    else if (link == "disable highlight" && mTab)
    {
        mTab->setAllowHighlight(false);
    }
    else if (link == "dont remove name" && mTab)
    {
        mTab->setRemoveNames(false);
    }
    else if (link == "remove name" && mTab)
    {
        mTab->setRemoveNames(true);
    }
    else if (link == "disable away" && mTab)
    {
        mTab->setNoAway(true);
    }
    else if (link == "enable away" && mTab)
    {
        mTab->setNoAway(false);
    }
    else if (link == "guild-pos" && !mNick.empty())
    {
        showChangePos(getX(), getY());
        return;
    }
    else if (!link.find("guild-pos-"))
    {
        if (player_node)
        {
            int num = atoi(link.substr(10).c_str());
            const Guild *guild = player_node->getGuild();
            if (guild)
            {
                Net::getGuildHandler()->changeMemberPostion(
                    guild->getMember(mNick), num);
            }
        }
    }
    else if (!link.find("player_"))
    {
        mBeingId = atoi(link.substr(7).c_str());
        Being *being = actorSpriteManager->findBeing(mBeingId);
        if (being)
        {
            showPopup(getX(), getY(), being);
            return;
        }
    }
    else if (!link.find("hide button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), false);
    }
    else if (!link.find("show button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), true);
    }
    else if (!link.find("hide bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), false);
    }
    else if (!link.find("show bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), true);
    }
    // Unknown actions
    else if (link != "cancel")
    {
        logger->log("PopupMenu: Warning, unknown action '%s'", link.c_str());
    }

    setVisible(false);

    mBeingId = 0;
    mFloorItem = 0;
    mItem = 0;
    mItemId = 0;
    mMapItem = 0;
    mNick = "";
}

void PopupMenu::showPopup(Window *parent, int x, int y, Item *item,
                          bool isInventory)
{
    if (!item)
        return;

    mItem = item;
    mWindow = parent;
    mBrowserBox->clearRows();

    int cnt = item->getQuantity();

    if (isInventory)
    {
        if (tradeWindow && tradeWindow->isVisible())
        {
            mBrowserBox->addRow(strprintf("@@addtrade|%s@@",
                _("Add to trade")));
            if (cnt > 1)
            {
                if (cnt > 10)
                {
                    mBrowserBox->addRow(strprintf("@@addtrade 10|%s@@",
                                        _("Add to trade 10")));
                }
                mBrowserBox->addRow(strprintf("@@addtrade half|%s@@",
                                    _("Add to trade half")));
                mBrowserBox->addRow(strprintf("@@addtrade all|%s@@",
                                    _("Add to trade all")));
            }
            mBrowserBox->addRow("##3---");
        }
        if (InventoryWindow::isStorageActive())
        {
            mBrowserBox->addRow(strprintf("@@store|%s@@", _("Store")));
            if (cnt > 1)
            {
                if (cnt > 10)
                {
                    mBrowserBox->addRow(strprintf("@@store 10|%s@@",
                                        _("Store 10")));
                }
                mBrowserBox->addRow(strprintf("@@store half|%s@@",
                                    _("Store half")));
                mBrowserBox->addRow(strprintf("@@store all|%s@@",
                                    _("Store all")));
            }
            mBrowserBox->addRow("##3---");
        }

        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Unequip")));
            else
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Equip")));
        }
        else
            mBrowserBox->addRow(strprintf("@@use|%s@@", _("Use")));

        if (cnt > 1)
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
        else
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow(strprintf("@@split|%s@@", _("Split")));

    }
    // Assume in storage for now
    // TODO: make this whole system more flexible, if needed
    else
    {
        mBrowserBox->addRow(strprintf("@@retrieve|%s@@", _("Retrieve")));
        if (cnt > 1)
        {
            if (cnt > 10)
            {
                mBrowserBox->addRow(strprintf("@@retrieve 10|%s@@",
                                    _("Retrieve 10")));
            }
            mBrowserBox->addRow(strprintf("@@retrieve half|%s@@",
                                _("Retrieve half")));
            mBrowserBox->addRow(strprintf("@@retrieve all|%s@@",
                                _("Retrieve all")));
        }
    }
    mBrowserBox->addRow(strprintf("@@chat|%s@@", _("Add to chat")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showItemPopup(int x, int y, int itemId)
{
    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    Item *item = inv->findItem(itemId);
    if (item)
    {
        showItemPopup(x, y, item);
    }
    else
    {
        mItem = 0;
        mItemId = itemId;
        mBrowserBox->clearRows();

        mBrowserBox->addRow(strprintf("@@use|%s@@", _("Use")));
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow(strprintf("@@load old item shortcuts|%s@@",
                            _("Load old item shortcuts")));
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

        showPopup(x, y);
    }
}

void PopupMenu::showItemPopup(int x, int y, Item *item)
{
    mItem = item;
    if (item)
        mItemId = item->getId();
    else
        mItemId = 0;
    mBrowserBox->clearRows();

    if (item)
    {
        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Unequip")));
            else
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Equip")));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@use|%s@@", _("Use")));
        }

        if (item->getQuantity() > 1)
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
        else
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow(strprintf("@@split|%s@@", _("Split")));

        if (InventoryWindow::isStorageActive())
            mBrowserBox->addRow(strprintf("@@store|%s@@", _("Store")));
        mBrowserBox->addRow(strprintf("@@chat|%s@@", _("Add to chat")));
        mBrowserBox->addRow("##3---");
    }
    mBrowserBox->addRow(strprintf("@@load old item shortcuts|%s@@",
                        _("Load old item shortcuts")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showDropPopup(int x, int y, Item *item)
{
    mItem = item;
    mBrowserBox->clearRows();

    if (item)
    {
        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Unequip")));
            else
                mBrowserBox->addRow(strprintf("@@use|%s@@", _("Equip")));
        }
        else
            mBrowserBox->addRow(strprintf("@@use|%s@@", _("Use")));

        if (item->getQuantity() > 1)
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
        else
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow(strprintf("@@split|%s@@", _("Split")));

        if (InventoryWindow::isStorageActive())
            mBrowserBox->addRow(strprintf("@@store|%s@@", _("Store")));
        mBrowserBox->addRow(strprintf("@@chat|%s@@", _("Add to chat")));
        mBrowserBox->addRow("##3---");
    }
    mBrowserBox->addRow(strprintf("@@load old drop shortcuts|%s@@",
                        _("Load old drop shortcuts")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, Button *button)
{
    if (!button || !windowMenu)
        return;

    mButton = button;

    mBrowserBox->clearRows();
    std::list <gcn::Button*> names = windowMenu->getButtons();
    std::list <gcn::Button*>::iterator it, it_end;
    for (it = names.begin(), it_end = names.end(); it != it_end; ++it)
    {
        Button *btn = dynamic_cast<Button*>(*it);
        if (!btn || btn->getActionEventId() == "SET")
            continue;

        if (btn->isVisible())
        {
            mBrowserBox->addRow(strprintf("@@hide button_%s|%s %s@@",
                btn->getActionEventId().c_str(), _("Hide"),
                btn->getCaption().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show button_%s|%s %s@@",
                btn->getActionEventId().c_str(), _("Show"),
                btn->getCaption().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, ProgressBar *b)
{
    if (!b || !miniStatusWindow)
        return;

    mBrowserBox->clearRows();
    std::list <ProgressBar*> bars = miniStatusWindow->getBars();
    std::list <ProgressBar*>::iterator it, it_end;
    for (it = bars.begin(), it_end = bars.end(); it != it_end; ++it)
    {
        ProgressBar *bar = *it;
        if (!bar || bar->getActionEventId() == "status bar")
            continue;

        if (bar->isVisible())
        {
            mBrowserBox->addRow(strprintf("@@hide bar_%s|%s %s@@",
                bar->getActionEventId().c_str(), _("Hide"),
                bar->getId().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show bar_%s|%s %s@@",
                bar->getActionEventId().c_str(), _("Show"),
                bar->getId().c_str()));
        }
    }

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y)
{
    setContentSize(mBrowserBox->getWidth() + 8, mBrowserBox->getHeight() + 8);
    if (graphics->getWidth() < (x + getWidth() + 5))
        x = graphics->getWidth() - getWidth();
    if (graphics->getHeight() < (y + getHeight() + 5))
        y = graphics->getHeight() - getHeight();
    setPosition(x, y);
    setVisible(true);
    requestMoveToTop();
}

RenameListener::RenameListener() :
    mMapItem(0),
    mDialog(0)
{
}

void RenameListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && mMapItem && viewport && mDialog)
    {
        Map *map = viewport->getMap();
        if (!map)
            return;

        SpecialLayer *sl = map->getSpecialLayer();
        MapItem *item = 0;
        if (sl)
        {
            item = sl->getTile(mMapItem->getX(), mMapItem->getY());
            if (!item)
            {
                sl->setTile(mMapItem->getX(), mMapItem->getY(),
                            mMapItem->getType());
                item = sl->getTile(mMapItem->getX(), mMapItem->getY());
            }
            item->setComment(mDialog->getText());
        }
        item = map->findPortalXY(mMapItem->getX(), mMapItem->getY());
        if (item)
            item->setComment(mDialog->getText());

        if (socialWindow)
            socialWindow->updatePortalNames();
    }
    mDialog = 0;
}
