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

#include "gui/popupmenu.h"

#include "actorspritemanager.h"
#include "dropshortcut.h"
#include "game.h"
#include "guild.h"
#include "guildmanager.h"
#include "inputmanager.h"
#include "item.h"
#include "itemshortcut.h"
#include "localplayer.h"
#include "maplayer.h"
#include "party.h"
#include "playerinfo.h"
#include "playerrelations.h"
#include "spellmanager.h"

#include "gui/buydialog.h"
#include "gui/chatwindow.h"
#include "gui/equipmentwindow.h"
#include "gui/inventorywindow.h"
#include "gui/itemamountwindow.h"
#include "gui/ministatuswindow.h"
#include "gui/outfitwindow.h"
#include "gui/selldialog.h"
#include "gui/skilldialog.h"
#include "gui/socialwindow.h"
#include "gui/textcommandeditor.h"
#include "gui/textdialog.h"
#include "gui/tradewindow.h"
#include "gui/viewport.h"
#include "gui/windowmenu.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/whispertab.h"

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/buysellhandler.h"
#include "net/guildhandler.h"
#include "net/npchandler.h"
#include "net/partyhandler.h"
#include "gui/shortcutwindow.h"
#include "net/tradehandler.h"

#include "resources/iteminfo.h"

#include "utils/copynpaste.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include <guichan/listmodel.hpp>

#include "debug.h"

extern int serverVersion;

std::string tradePartnerName("");

PopupMenu::PopupMenu():
    Popup("PopupMenu", "popupmenu.xml"),
    mBrowserBox(new BrowserBox(this)),
    mBeingId(0),
    mFloorItemId(0),
    mItem(nullptr),
    mItemId(0),
    mItemColor(1),
    mMapItem(nullptr),
    mTab(nullptr),
    mSpell(nullptr),
    mWindow(nullptr),
    mDialog(nullptr),
    mButton(nullptr),
    mNick(""),
    mTextField(nullptr),
    mType(static_cast<int>(Being::UNKNOWN)),
    mX(0),
    mY(0)
{
    mBrowserBox->setHighlightMode(BrowserBox::BACKGROUND);
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mRenameListener.setMapItem(nullptr);
    mRenameListener.setDialog(nullptr);
    mPlayerListener.setNick("");
    mPlayerListener.setDialog(nullptr);
    mPlayerListener.setType(static_cast<int>(Being::UNKNOWN));
    mScrollArea = new ScrollArea(mBrowserBox, false);
    mScrollArea->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO); 

    add(mScrollArea);
}

void PopupMenu::showPopup(const int x, const int y, const Being *const being)
{
    if (!being || !player_node || !actorSpriteManager)
        return;

    mBeingId = being->getId();
    mNick = being->getName();
    mType = static_cast<int>(being->getType());
    mBrowserBox->clearRows();
    mX = x;
    mY = y;

    const std::string &name = mNick;

    mBrowserBox->addRow(name + being->getGenderSignWithSpace());

    switch (being->getType())
    {
        case ActorSprite::PLAYER:
            {
                mBrowserBox->addRow("trade", _("Trade"));
                mBrowserBox->addRow("attack", _("Attack"));
                mBrowserBox->addRow("whisper", _("Whisper"));

                mBrowserBox->addRow("##3---");

                mBrowserBox->addRow("heal", _("Heal"));
                mBrowserBox->addRow("##3---");

                addPlayerRelation(name);

                mBrowserBox->addRow("##3---");
                addFollow();

                addParty(being->getPartyName());

                const Guild *const guild1 = being->getGuild();
                const Guild *const guild2 = player_node->getGuild();
                if (guild2)
                {
                    if (guild1)
                    {
                        if (guild1->getId() == guild2->getId())
                        {
                            mBrowserBox->addRow("guild-kick",
                                _("Kick from guild"));
                            if (guild2->getServerGuild())
                            {
                                mBrowserBox->addRow(strprintf(
                                    "@@guild-pos|%s >@@",
                                    _("Change pos in guild")));
                            }
                        }
                    }
                    else if (guild2->getMember(mNick))
                    {
                        mBrowserBox->addRow("guild-kick",
                            _("Kick from guild"));
                        if (guild2->getServerGuild())
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@guild-pos|%s >@@",
                                _("Change pos in guild")));
                        }
                    }
                    else
                    {
                        if (guild2->getServerGuild()
                            || (guildManager && guildManager->havePower()))
                        {
                            mBrowserBox->addRow("guild", _("Invite to guild"));
                        }
                    }
                }

                if (player_node->isGM())
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow("admin-kick", _("Kick player"));
                }
                mBrowserBox->addRow("nuke", _("Nuke"));
                mBrowserBox->addRow("move", _("Move"));
                addPlayerMisc();
                addBuySell(being);
            }
            break;

        case ActorSprite::NPC:
            // NPCs can be talked to (single option, candidate for removal
            // unless more options would be added)
            mBrowserBox->addRow("talk", _("Talk"));
            mBrowserBox->addRow("buy", _("Buy"));
            mBrowserBox->addRow("sell", _("Sell"));
            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow("move", _("Move"));
            mBrowserBox->addRow("addcomment", _("Add comment"));
            break;

        case ActorSprite::MONSTER:
            {
                // Monsters can be attacked
                mBrowserBox->addRow("attack", _("Attack"));

                if (player_node->isGM())
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow("admin-kick", _("Kick"));
                }

                if (config.getBoolValue("enableAttackFilter"))
                {
                    mBrowserBox->addRow("##3---");
                    if (actorSpriteManager->isInAttackList(name)
                        || actorSpriteManager->isInIgnoreAttackList(name)
                        || actorSpriteManager->isInPriorityAttackList(name))
                    {
                        mBrowserBox->addRow("remove attack",
                            _("Remove from attack list"));
                    }
                    else
                    {
                        mBrowserBox->addRow("add attack priority",
                            _("Add to priority attack list"));
                        mBrowserBox->addRow("add attack",
                            _("Add to attack list"));
                        mBrowserBox->addRow("add attack ignore",
                            _("Add to ignore list"));
                    }
                }
            }
            break;

        case ActorSprite::UNKNOWN:
        case ActorSprite::FLOOR_ITEM:
        case ActorSprite::PORTAL:
        default:
            /* Other beings aren't interesting... */
            return;
    }
    mBrowserBox->addRow("name", _("Add name to chat"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y,
                          std::vector<ActorSprite*> &beings)
{
    mX = x;
    mY = y;
    mBrowserBox->clearRows();
    mBrowserBox->addRow(_("Players"));
    FOR_EACH (std::vector<ActorSprite*>::const_iterator, it, beings)
    {
        const Being *const being = dynamic_cast<Being*>(*it);
        const ActorSprite *const actor = *it;
        if (being && !being->getName().empty())
        {
            mBrowserBox->addRow(strprintf("@@player_%u|%s >@@",
                static_cast<unsigned>(being->getId()), (being->getName()
                + being->getGenderSignWithSpace()).c_str()));
        }
        else if (actor->getType() == ActorSprite::FLOOR_ITEM)
        {
            const FloorItem *const floorItem
                = static_cast<const FloorItem*>(actor);
            mBrowserBox->addRow(strprintf("@@flooritem_%u|%s >@@",
                static_cast<unsigned>(actor->getId()),
                floorItem->getName().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(x, y);
}

void PopupMenu::showPlayerPopup(const int x, const int y, std::string nick)
{
    if (nick.empty() || !player_node)
        return;

    mNick = nick;
    mBeingId = 0;
    mType = static_cast<int>(Being::PLAYER);
    mX = x;
    mY = y;
    mBrowserBox->clearRows();

    const std::string &name = mNick;

    mBrowserBox->addRow(name);

    mBrowserBox->addRow("whisper", _("Whisper"));
    mBrowserBox->addRow("##3---");

    addPlayerRelation(name);

    mBrowserBox->addRow("##3---");
    addFollow();
    mBrowserBox->addRow("addcomment", _("Add comment"));

    if (player_node->isInParty())
    {
        const Party *const party = player_node->getParty();
        if (party)
        {
            const PartyMember *const member = party->getMember(mNick);
            if (member)
            {
                mBrowserBox->addRow("kick party", _("Kick from party"));
                mBrowserBox->addRow("##3---");
                const PartyMember *const o = party->getMember(
                    player_node->getName());
                if (o && member->getMap() == o->getMap())
                    mBrowserBox->addRow("move", _("Move"));
            }
        }
    }

    const Guild *const guild2 = player_node->getGuild();
    if (guild2)
    {
        if (guild2->getMember(mNick))
        {
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
            {
                mBrowserBox->addRow("guild-kick", _("Kick from guild"));
            }
            if (guild2->getServerGuild())
            {
                mBrowserBox->addRow(strprintf(
                    "@@guild-pos|%s >@@", _("Change pos in guild")));
            }
        }
        else
        {
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
            {
                mBrowserBox->addRow("guild", _("Invite to guild"));
            }
        }
    }

    addBuySellDefault();
    mBrowserBox->addRow("name", _("Add name to chat"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y,
                          const FloorItem *const floorItem)
{
    if (!floorItem)
        return;

    mFloorItemId = floorItem->getId();
    mX = x;
    mY = y;
    mType = static_cast<int>(Being::FLOOR_ITEM);
    mBrowserBox->clearRows();
    std::string name = floorItem->getName();
    mNick = name;

    mBrowserBox->addRow(name);

    if (config.getBoolValue("enablePickupFilter"))
    {
        if (actorSpriteManager->isInPickupList(name)
            || (actorSpriteManager->isInPickupList("")
            && !actorSpriteManager->isInIgnorePickupList(name)))
        {
            mBrowserBox->addRow("pickup", _("Pick up"));
            mBrowserBox->addRow("##3---");
        }
        addPickupFilter(name);
    }
    else
    {
        mBrowserBox->addRow("pickup", _("Pick up"));
    }
    mBrowserBox->addRow("chat", _("Add to chat"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y, MapItem *const mapItem)
{
    if (!mapItem)
        return;

    mMapItem = mapItem;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Map Item"));
    mBrowserBox->addRow("rename map", _("Rename"));
    mBrowserBox->addRow("remove map", _("Remove"));

    if (player_node && player_node->isGM())
    {
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow("warp map", _("Warp"));
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showMapPopup(const int x, const int y,
                             const int x2, const int y2)
{
    mX = x2;
    mY = y2;

    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Map Item"));

    if (player_node && player_node->isGM())
        mBrowserBox->addRow("warp map", _("Warp"));
    mBrowserBox->addRow("move", _("Move"));
    mBrowserBox->addRow("movecamera", _("Move camera"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showOutfitsPopup(const int x, const int y)
{
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Outfits"));
    mBrowserBox->addRow("clear outfit", _("Clear outfit"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showSpellPopup(const int x, const int y,
                               TextCommand *const cmd)
{
    if (!cmd)
        return;

    mBrowserBox->clearRows();

    mSpell = cmd;
    mX = x;
    mY = y;

    mBrowserBox->addRow(_("Spells"));
    mBrowserBox->addRow("edit spell", _("Edit spell"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChatPopup(const int x, const int y, ChatTab *const tab)
{
    if (!tab || !actorSpriteManager || !player_node)
        return;

    mTab = tab;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (tab->getType() == static_cast<int>(ChatTab::TAB_WHISPER))
        mBrowserBox->addRow("chat close", _("Close"));

    mBrowserBox->addRow("chat clear", _("Clear"));
    mBrowserBox->addRow("##3---");

    if (tab->getAllowHighlight())
        mBrowserBox->addRow("disable highlight", _("Disable highlight"));
    else
        mBrowserBox->addRow("enable highlight", _("Enable highlight"));
    if (tab->getRemoveNames())
        mBrowserBox->addRow("dont remove name", _("Don't remove name"));
    else
        mBrowserBox->addRow("remove name", _("Remove name"));
    if (tab->getNoAway())
        mBrowserBox->addRow("enable away", _("Enable away"));
    else
        mBrowserBox->addRow("disable away", _("Disable away"));
    mBrowserBox->addRow("##3---");

    if (tab->getType() == static_cast<int>(ChatTab::TAB_PARTY))
    {
        mBrowserBox->addRow("leave party", _("Leave"));
        mBrowserBox->addRow("##3---");
    }
    mBrowserBox->addRow("chat clipboard", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");

    if (tab->getType() == static_cast<int>(ChatTab::TAB_WHISPER))
    {
        const WhisperTab *const wTab = static_cast<WhisperTab*>(tab);
        std::string name = wTab->getNick();

        const Being* const being  = actorSpriteManager->findBeingByName(
            name, Being::PLAYER);

        if (being)
        {
            mBeingId = being->getId();
            mNick = being->getName();
            mType = static_cast<int>(being->getType());

            mBrowserBox->addRow("trade", _("Trade"));
            mBrowserBox->addRow("attack", _("Attack"));

            mBrowserBox->addRow("##3---");

            mBrowserBox->addRow("heal", _("Heal"));
            mBrowserBox->addRow("##3---");

            addPlayerRelation(name);

            mBrowserBox->addRow("##3---");
            addFollow();
            mBrowserBox->addRow("move", _("Move"));
            addPlayerMisc();
            addBuySell(being);
            mBrowserBox->addRow("##3---");

            if (player_node->isInParty())
            {
                if (player_node->getParty())
                {
                    if (!player_node->getParty()->isMember(wTab->getNick()))
                    {
                        mBrowserBox->addRow("party", _("Invite to party"));
                    }
                    else
                    {
                        mBrowserBox->addRow("kick party",
                            _("Kick from party"));
                    }
                    mBrowserBox->addRow("##3---");
                }
            }
            const Guild *const guild1 = being->getGuild();
            const Guild *const guild2 = player_node->getGuild();
            if (guild2)
            {
                if (guild1)
                {
                    if (guild1->getId() == guild2->getId())
                    {
                        if (guild2->getServerGuild() || (guildManager
                            && guildManager->havePower()))
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@guild-kick|%s@@", _("Kick from guild")));
                        }
                        if (guild2->getServerGuild())
                        {
                            mBrowserBox->addRow(strprintf("@@guild-pos|%s >@@",
                                 _("Change pos in guild")));
                        }
                    }
                }
                else
                {
                    if (guild2->getServerGuild() || (guildManager
                        && guildManager->havePower()))
                    {
                        mBrowserBox->addRow("guild", _("Invite to guild"));
                    }
                }
            }
        }
        else
        {
            mNick = name;
            mType = static_cast<int>(Being::PLAYER);
            addPlayerRelation(name);
            mBrowserBox->addRow("##3---");
            addFollow();

            if (player_node->isInParty())
            {
                const Party *const party = player_node->getParty();
                if (party)
                {
                    const PartyMember *const m = party->getMember(mNick);
                    if (m)
                        mBrowserBox->addRow("move", _("Move"));
                }
            }
            addPlayerMisc();
            addBuySellDefault();
            mBrowserBox->addRow("##3---");
        }
    }
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChangePos(const int x, const int y)
{
    mBrowserBox->clearRows();
    mBrowserBox->addRow(_("Change guild position"));

    if (!player_node)
        return;

    mX = x;
    mY = y;
    const Guild *const guild = player_node->getGuild();
    if (guild)
    {
        PositionsMap map = guild->getPositions();
        FOR_EACH (PositionsMap::const_iterator, itr, map)
        {
            mBrowserBox->addRow(strprintf("@@guild-pos-%u|%s@@",
                itr->first, itr->second.c_str()));
        }
        mBrowserBox->addRow("cancel", _("Cancel"));

        showPopup(x, y);
    }
    else
    {
        mBeingId = 0;
        mFloorItemId = 0;
        mItem = nullptr;
        mMapItem = nullptr;
        mNick.clear();
        mType = static_cast<int>(Being::UNKNOWN);
        mX = 0;
        mY = 0;
        setVisible(false);
    }
}

void PopupMenu::handleLink(const std::string &link,
                           gcn::MouseEvent *event A_UNUSED)
{
    Being *being = nullptr;
    if (actorSpriteManager)
        being = actorSpriteManager->findBeing(mBeingId);

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
        if (actorSpriteManager)
            actorSpriteManager->heal(being);
    }
    else if (link == "unignore" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
                                     PlayerRelation::NEUTRAL);
    }
    else if (link == "unignore" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::NEUTRAL);
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

    else if (link == "blacklist" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
            PlayerRelation::BLACKLISTED);
    }
    else if (link == "blacklist" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::BLACKLISTED);
    }
    else if (link == "enemy" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(),
            PlayerRelation::ENEMY2);
    }
    else if (link == "enemy" && !mNick.empty())
    {
        player_relations.setRelation(mNick, PlayerRelation::ENEMY2);
    }
    else if (link == "erase" && being &&
             being->getType() == ActorSprite::PLAYER)
    {
        player_relations.setRelation(being->getName(), PlayerRelation::ERASED);
        being->updateName();
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
            const Guild *const guild = player_node->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                    Net::getGuildHandler()->invite(guild->getId(), mNick);
                else if (guildManager)
                    guildManager->invite(mNick);
            }
        }
    }
    else if (link == "nuke" && being)
    {
        if (actorSpriteManager)
        {
            actorSpriteManager->addBlock(static_cast<uint32_t>(
                being->getId()));
            actorSpriteManager->destroy(being);
        }
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
    else if ((link == "pickup") && mFloorItemId)
    {
        if (player_node && actorSpriteManager)
        {
            FloorItem *const item = actorSpriteManager->findItem(
                mFloorItemId);
            if (item)
                player_node->pickUp(item);
        }
    }
    // Look To action
    else if (link == "look")
    {
    }
    else if (link == "use" && mItemId)
    {
        if (mItemId < SPELL_MIN_ID)
        {
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
            {
                const Item *const item = inv->findItem(mItemId, mItemColor);
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
        else if (mItemId < SKILL_MIN_ID && spellManager)
        {
            spellManager->useItem(mItemId);
        }
        else if (skillDialog)
        {
            skillDialog->useItem(mItemId);
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
            else if (mFloorItemId && actorSpriteManager)
            {
                const FloorItem *const item = actorSpriteManager->findItem(
                    mFloorItemId);

                if (item)
                {
                    if (serverVersion > 0)
                    {
                        chatWindow->addItemText(item->getInfo().getName(
                            item->getColor()));
                    }
                    else
                    {
                        chatWindow->addItemText(item->getInfo().getName());
                    }
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
    else if (link == "move" && !mNick.empty())
    {
        if (player_node)
        {
            if (being)
            {
                player_node->navigateTo(being->getTileX(), being->getTileY());
            }
            else if (player_node->isInParty())
            {
                const Party *const party = player_node->getParty();
                if (party)
                {
                    const PartyMember *const m = party->getMember(mNick);
                    const PartyMember *const o = party->getMember(
                        player_node->getName());
                    if (m && o && m->getMap() == o->getMap())
                        player_node->navigateTo(m->getX(), m->getY());
                }
            }
        }
    }
    else if (link == "move" && (mX || mY))
    {
        if (player_node)
            player_node->navigateTo(mX, mY);
    }
    else if (link == "movecamera" && (mX || mY))
    {
        if (viewport)
            viewport->moveCameraToPosition(mX * 32, mY * 32);
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
    else if (link == "drop all" && mItem)
    {
        Net::getInventoryHandler()->dropItem(mItem, mItem->getQuantity());
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
        Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
            mItem->getInvIndex(), cnt,
            Inventory::STORAGE);
    }
    else if (link == "store half" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
            mItem->getInvIndex(), mItem->getQuantity() / 2,
            Inventory::STORAGE);
    }
    else if (link == "store all-1" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
            mItem->getInvIndex(), mItem->getQuantity() - 1,
            Inventory::STORAGE);
    }
    else if (link == "store all" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::INVENTORY,
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
            tradeWindow->tradeItem(mItem, cnt, true);
        }
    }
    else if (link == "addtrade half" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity() / 2, true);
    }
    else if (link == "addtrade all-1" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity() - 1, true);
    }
    else if (link == "addtrade all" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity(), true);
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
        Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
            mItem->getInvIndex(), cnt,
            Inventory::INVENTORY);
    }
    else if (link == "retrieve half" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
            mItem->getInvIndex(), mItem->getQuantity() / 2,
            Inventory::INVENTORY);
    }
    else if (link == "retrieve all-1" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
            mItem->getInvIndex(), mItem->getQuantity() - 1,
            Inventory::INVENTORY);
    }
    else if (link == "retrieve all" && mItem)
    {
        Net::getInventoryHandler()->moveItem2(Inventory::STORAGE,
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
            const PartyMember *const member = player_node->
                getParty()->getMember(mNick);
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
    else if (link == "warp map" && mMapItem)
    {
        if (Game::instance())
        {
            Net::getAdminHandler()->warp(Game::instance()->getCurrentMapName(),
                mMapItem->getX(), mMapItem->getY());
        }
    }
    else if (link == "warp map" && (mX || mY))
    {
        if (Game::instance())
        {
            Net::getAdminHandler()->warp(Game::instance()->getCurrentMapName(),
                mX, mY);
        }
    }
    else if (link == "remove map" && mMapItem)
    {
        if (viewport)
        {
            const Map *const map = viewport->getCurrentMap();
            if (map)
            {
                SpecialLayer *const specialLayer = map->getSpecialLayer();
                if (specialLayer)
                {
                    const bool isHome = (mMapItem->getType()
                        == static_cast<int>(MapItem::HOME));
                    const int x = static_cast<const int>(mMapItem->getX());
                    const int y = static_cast<const int>(mMapItem->getY());
                    specialLayer->setTile(x, y,
                        static_cast<int>(MapItem::EMPTY));
                    if (socialWindow)
                        socialWindow->removePortal(x, y);
                    if (isHome && player_node)
                    {
                        player_node->removeHome();
                        player_node->saveHomes();
                    }
                }
            }
        }
    }
    else if (link == "rename map" && mMapItem)
    {
        mRenameListener.setMapItem(mMapItem);
        // TRANSLATORS: number of chars in string should be near original
        mDialog = new TextDialog(_("Rename map sign          "),
        // TRANSLATORS: number of chars in string should be near original
                                 _("Name:                    "));
        mRenameListener.setDialog(mDialog);
        mDialog->setText(mMapItem->getComment());
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&mRenameListener);
    }
    else if (link == "clear drops")
    {
        if (dropShortcut)
            dropShortcut->clear();
    }
    else if (link == "edit spell" && mSpell)
    {
        new TextCommandEditor(mSpell);
    }
    else if (link == "undress" && being)
    {
        Net::getBeingHandler()->undress(being);
    }
    else if (link == "addcomment" && !mNick.empty())
    {
        // TRANSLATORS: number of chars in string should be near original
        TextDialog *const dialog = new TextDialog(
            _("Player comment            "),
        // TRANSLATORS: number of chars in string should be near original
            _("Comment:                      "));
        mPlayerListener.setDialog(dialog);
        mPlayerListener.setNick(mNick);
        mPlayerListener.setType(mType);

        if (being)
        {
            being->updateComment();
            dialog->setText(being->getComment());
        }
        else
        {
            dialog->setText(Being::loadComment(mNick, mType));
        }
        dialog->setActionEventId("ok");
        dialog->addActionListener(&mPlayerListener);
    }
    else if (link == "guild-kick" && !mNick.empty())
    {
        if (player_node)
        {
            const Guild *const guild = player_node->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                    Net::getGuildHandler()->kick(guild->getMember(mNick));
                else if (guildManager)
                    guildManager->kick(mNick);
            }
        }
    }
    else if (link == "enable highlight" && mTab)
    {
        mTab->setAllowHighlight(true);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "disable highlight" && mTab)
    {
        mTab->setAllowHighlight(false);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "dont remove name" && mTab)
    {
        mTab->setRemoveNames(false);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "remove name" && mTab)
    {
        mTab->setRemoveNames(true);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "disable away" && mTab)
    {
        mTab->setNoAway(true);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "enable away" && mTab)
    {
        mTab->setNoAway(false);
        if (chatWindow)
            chatWindow->saveState();
    }
    else if (link == "chat clipboard" && mTab)
    {
        if (chatWindow)
            chatWindow->copyToClipboard(mX, mY);
    }
    else if (link == "remove attack" && being)
    {
        if (actorSpriteManager && being->getType() == Being::MONSTER)
        {
            actorSpriteManager->removeAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack" && being)
    {
        if (actorSpriteManager && being->getType() == Being::MONSTER)
        {
            actorSpriteManager->addAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack priority" && being)
    {
        if (actorSpriteManager && being->getType() == Being::MONSTER)
        {
            actorSpriteManager->addPriorityAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack ignore" && being)
    {
        if (actorSpriteManager && being->getType() == Being::MONSTER)
        {
            actorSpriteManager->addIgnoreAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "remove pickup" && !mNick.empty())
    {
        if (actorSpriteManager)
        {
            actorSpriteManager->removePickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "add pickup" && !mNick.empty())
    {
        if (actorSpriteManager)
        {
            actorSpriteManager->addPickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "add pickup ignore" && !mNick.empty())
    {
        if (actorSpriteManager)
        {
            actorSpriteManager->addIgnorePickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "attack moveup")
    {
        if (actorSpriteManager)
        {
            const int idx = actorSpriteManager->getAttackMobIndex(mNick);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorSpriteManager->getAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorSpriteManager->setAttackMobs(mobs);
                        actorSpriteManager->rebuildAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "priority moveup")
    {
        if (actorSpriteManager)
        {
            const int idx = actorSpriteManager->
                getPriorityAttackMobIndex(mNick);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorSpriteManager->getPriorityAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorSpriteManager->setPriorityAttackMobs(mobs);
                        actorSpriteManager->rebuildPriorityAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "attack movedown")
    {
        if (actorSpriteManager)
        {
            const int idx = actorSpriteManager->getAttackMobIndex(mNick);
            const int size = actorSpriteManager->getAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorSpriteManager->getAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        ++ it2;
                        if (it2 == mobs.end())
                            break;

                        mobs.splice(it, mobs, it2);
                        actorSpriteManager->setAttackMobs(mobs);
                        actorSpriteManager->rebuildAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "priority movedown")
    {
        if (player_node)
        {
            const int idx = actorSpriteManager
                ->getPriorityAttackMobIndex(mNick);
            const int size = actorSpriteManager->getPriorityAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorSpriteManager->getPriorityAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        ++ it2;
                        if (it2 == mobs.end())
                            break;

                        mobs.splice(it, mobs, it2);
                        actorSpriteManager->setPriorityAttackMobs(mobs);
                        actorSpriteManager->rebuildPriorityAttackMobs();
                        break;
                    }
                    ++ it;
                    ++ it2;
                }

                if (socialWindow)
                    socialWindow->updateAttackFilter();
            }
        }
    }
    else if (link == "attack remove")
    {
        if (actorSpriteManager)
        {
            if (mNick.empty())
            {
                if (actorSpriteManager->isInAttackList(mNick))
                {
                    actorSpriteManager->removeAttackMob(mNick);
                    actorSpriteManager->addIgnoreAttackMob(mNick);
                }
                else
                {
                    actorSpriteManager->removeAttackMob(mNick);
                    actorSpriteManager->addAttackMob(mNick);
                }
            }
            else
            {
                actorSpriteManager->removeAttackMob(mNick);
            }
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "pickup remove")
    {
        if (actorSpriteManager)
        {
            if (mNick.empty())
            {
                if (actorSpriteManager->isInPickupList(mNick))
                {
                    actorSpriteManager->removePickupItem(mNick);
                    actorSpriteManager->addIgnorePickupItem(mNick);
                }
                else
                {
                    actorSpriteManager->removePickupItem(mNick);
                    actorSpriteManager->addPickupItem(mNick);
                }
            }
            else
            {
                actorSpriteManager->removePickupItem(mNick);
            }
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "reset yellow")
    {
        if (player_node)
            player_node->resetYellowBar();
    }
    else if (link == "bar to chat" && !mNick.empty())
    {
        if (chatWindow)
            chatWindow->addInputText(mNick);
    }
    else if (link == "items" && being)
    {
        if (being == player_node)
        {
            if (equipmentWindow && !equipmentWindow->isVisible())
                equipmentWindow->setVisible(true);
        }
        else
        {
            if (beingEquipmentWindow)
            {
                beingEquipmentWindow->setBeing(being);
                beingEquipmentWindow->setVisible(true);
            }
        }
    }
    else if (link == "undress item" && being && mItemId)
    {
        being->undressItemById(mItemId);
    }
    else if (link == "guild-pos" && !mNick.empty())
    {
        showChangePos(getX(), getY());
        return;
    }
    else if (link == "clear outfit")
    {
        if (outfitWindow)
            outfitWindow->clearCurrentOutfit();
    }
    else if (link == "clipboard copy")
    {
        if (mTextField)
            mTextField->handleCopy();
    }
    else if (link == "clipboard paste")
    {
        if (mTextField)
            mTextField->handlePaste();
    }
    else if (link == "open link" && !mNick.empty())
    {
        openBrowser(mNick);
    }
    else if (link == "clipboard link" && !mNick.empty())
    {
        sendBuffer(mNick);
    }
    else if (!link.compare(0, 10, "guild-pos-"))
    {
        if (player_node)
        {
            const int num = atoi(link.substr(10).c_str());
            const Guild *const guild = player_node->getGuild();
            if (guild)
            {
                Net::getGuildHandler()->changeMemberPostion(
                    guild->getMember(mNick), num);
            }
        }
    }
    else if (!link.compare(0, 7, "player_"))
    {
        if (actorSpriteManager)
        {
            mBeingId = atoi(link.substr(7).c_str());
            being = actorSpriteManager->findBeing(mBeingId);
            if (being)
            {
                showPopup(getX(), getY(), being);
                return;
            }
        }
    }
    else if (!link.compare(0, 10, "flooritem_"))
    {
        if (actorSpriteManager)
        {
            const int id = atoi(link.substr(10).c_str());
            if (id)
            {
                const FloorItem *const item = actorSpriteManager->findItem(id);
                if (item)
                {
                    mFloorItemId = item->getId();
                    showPopup(getX(), getY(), item);
                    return;
                }
            }
        }
    }
    else if (!link.compare(0, 12, "hide button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), false);
    }
    else if (!link.compare(0, 12, "show button_"))
    {
        if (windowMenu)
            windowMenu->showButton(link.substr(12), true);
    }
    else if (!link.compare(0, 9, "hide bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), false);
    }
    else if (!link.compare(0, 9, "show bar_"))
    {
        if (miniStatusWindow)
            miniStatusWindow->showBar(link.substr(9), true);
    }
    else if (!link.compare(0, 12, "show window_"))
    {
        const int id = atoi(link.substr(12).c_str());
        if (id >= 0)
            inputManager.executeAction(id);
    }
    // Unknown actions
    else if (link != "cancel")
    {
        logger->log("PopupMenu: Warning, unknown action '%s'", link.c_str());
    }

    setVisible(false);

    mBeingId = 0;
    mFloorItemId = 0;
    mItem = nullptr;
    mItemId = 0;
    mItemColor = 1;
    mMapItem = nullptr;
    mTab = nullptr;
    mSpell = nullptr;
    mWindow = nullptr;
    mDialog = nullptr;
    mButton = nullptr;
    mNick.clear();
    mTextField = nullptr;
    mType = static_cast<int>(Being::UNKNOWN);
    mX = 0;
    mY = 0;
}

void PopupMenu::showPopup(Window *const parent, const int x, const int y,
                          Item *const item, const bool isInventory)
{
    if (!item)
        return;

    mItem = item;
    mItemId = item->getId();
    mItemColor = item->getColor();
    mWindow = parent;
    mX = x;
    mY = y;
    mNick.clear();
    mBrowserBox->clearRows();

    const int cnt = item->getQuantity();

    if (isInventory)
    {
        if (tradeWindow && tradeWindow->isVisible())
        {
            mBrowserBox->addRow("addtrade", _("Add to trade"));
            if (cnt > 1)
            {
                if (cnt > 10)
                    mBrowserBox->addRow("addtrade 10", _("Add to trade 10"));
                mBrowserBox->addRow("addtrade half", _("Add to trade half"));
                mBrowserBox->addRow("addtrade all-1", _("Add to trade all-1"));
                mBrowserBox->addRow("addtrade all", _("Add to trade all"));
            }
            mBrowserBox->addRow("##3---");
        }
        if (InventoryWindow::isStorageActive())
        {
            mBrowserBox->addRow("store", _("Store"));
            if (cnt > 1)
            {
                if (cnt > 10)
                    mBrowserBox->addRow("store 10", _("Store 10"));
                mBrowserBox->addRow("store half", _("Store half"));
                mBrowserBox->addRow("store all-1", _("Store all-1"));
                mBrowserBox->addRow("store all", _("Store all"));
            }
            mBrowserBox->addRow("##3---");
        }

        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow("use", _("Unequip"));
            else
                mBrowserBox->addRow("use", _("Equip"));
        }
        else
        {
            mBrowserBox->addRow("use", _("Use"));
        }

        if (cnt > 1)
        {
            mBrowserBox->addRow("drop", _("Drop..."));
            mBrowserBox->addRow("drop all", _("Drop all"));
        }
        else
        {
            mBrowserBox->addRow("drop", _("Drop"));
        }

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow("split", _("Split"));

    }
    // Assume in storage for now
    // TODO: make this whole system more flexible, if needed
    else
    {
        mBrowserBox->addRow("retrieve", _("Retrieve"));
        if (cnt > 1)
        {
            if (cnt > 10)
                mBrowserBox->addRow("retrieve 10", _("Retrieve 10"));
            mBrowserBox->addRow("retrieve half", _("Retrieve half"));
            mBrowserBox->addRow("retrieve all-1", _("Retrieve all-1"));
            mBrowserBox->addRow("retrieve all", _("Retrieve all"));
        }
    }
    if (config.getBoolValue("enablePickupFilter"))
    {
        mNick = item->getName();
        mBrowserBox->addRow("##3---");
        addPickupFilter(mNick);
    }
    mBrowserBox->addRow("chat", _("Add to chat"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showItemPopup(const int x, const int y, const int itemId,
                              const unsigned char color)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    Item *const item = inv->findItem(itemId, color);
    if (item)
    {
        showItemPopup(x, y, item);
    }
    else
    {
        mItem = nullptr;
        mItemId = itemId;
        mItemColor = color;
        mX = x;
        mY = y;
        mBrowserBox->clearRows();

        mBrowserBox->addRow("use", _("Use"));
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow("cancel", _("Cancel"));

        showPopup(x, y);
    }
}

void PopupMenu::showItemPopup(const int x, const int y, Item *const item)
{
    mItem = item;
    mX = x;
    mY = y;
    if (item)
    {
        mItemId = item->getId();
        mItemColor = item->getColor();
    }
    else
    {
        mItemId = 0;
        mItemColor = 1;
    }
    mNick.clear();
    mBrowserBox->clearRows();

    if (item)
    {
        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow("use", _("Unequip"));
            else
                mBrowserBox->addRow("use", _("Equip"));
        }
        else
        {
            mBrowserBox->addRow("use", _("Use"));
        }

        if (item->getQuantity() > 1)
        {
            mBrowserBox->addRow("drop", _("Drop..."));
            mBrowserBox->addRow("drop all", _("Drop all"));
        }
        else
        {
            mBrowserBox->addRow("drop", _("Drop"));
        }

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow("split", _("Split"));

        if (InventoryWindow::isStorageActive())
            mBrowserBox->addRow("store", _("Store"));
        mBrowserBox->addRow("chat", _("Add to chat"));

        if (config.getBoolValue("enablePickupFilter"))
        {
            mNick = item->getName();

            mBrowserBox->addRow("##3---");
            addPickupFilter(mNick);
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showDropPopup(const int x, const int y, Item *const item)
{
    mItem = item;
    mX = x;
    mY = y;
    mNick.clear();
    mBrowserBox->clearRows();

    if (item)
    {
        mItemId = item->getId();
        mItemColor = item->getColor();

        if (item->isEquipment())
        {
            if (item->isEquipped())
                mBrowserBox->addRow("use", _("Unequip"));
            else
                mBrowserBox->addRow("use", _("Equip"));
        }
        else
        {
            mBrowserBox->addRow("use", _("Use"));
        }

        if (item->getQuantity() > 1)
        {
            mBrowserBox->addRow("drop", _("Drop..."));
            mBrowserBox->addRow("drop all", _("Drop all"));
        }
        else
        {
            mBrowserBox->addRow("drop", _("Drop"));
        }

        if (Net::getInventoryHandler()->canSplit(item))
            mBrowserBox->addRow("split", _("Split"));

        if (InventoryWindow::isStorageActive())
            mBrowserBox->addRow("store", _("Store"));
        mBrowserBox->addRow("chat", _("Add to chat"));
        if (config.getBoolValue("enablePickupFilter"))
        {
            mNick = item->getName();
            mBrowserBox->addRow("##3---");
            addPickupFilter(mNick);
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("clear drops", _("Clear drop window"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y, Button *const button)
{
    if (!button || !windowMenu)
        return;

    mButton = button;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    std::vector<Button *> names = windowMenu->getButtons();
    for (std::vector<Button *>::const_iterator it = names.begin(),
         it_end = names.end(); it != it_end; ++ it)
    {
        const Button *const btn = dynamic_cast<Button*>(*it);
        if (!btn || btn->getActionEventId() == "SET")
            continue;

        if (btn->isVisible())
        {
            mBrowserBox->addRow(strprintf("@@hide button_%s|%s %s (%s)@@",
                btn->getActionEventId().c_str(), _("Hide"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show button_%s|%s %s (%s)@@",
                btn->getActionEventId().c_str(), _("Show"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y, const ProgressBar *const b)
{
    if (!b || !miniStatusWindow)
        return;

    mNick = b->text();
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    std::vector <ProgressBar*> bars = miniStatusWindow->getBars();
    ProgressBar *onlyBar = nullptr;
    int cnt = 0;

    // search for alone visible bar
    for (std::vector <ProgressBar*>::const_iterator it = bars.begin(),
         it_end = bars.end(); it != it_end; ++it)
    {
        ProgressBar *const bar = *it;
        if (!bar)
            continue;

        if (bar->isVisible())
        {
            cnt ++;
            onlyBar = bar;
        }
    }
    if (cnt > 1)
        onlyBar = nullptr;

    for (std::vector <ProgressBar*>::const_iterator it = bars.begin(),
         it_end = bars.end(); it != it_end; ++it)
    {
        ProgressBar *const bar = *it;
        if (!bar || bar == onlyBar)
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
    mBrowserBox->addRow("reset yellow", _("Reset yellow bar"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("bar to chat", _("Copy to chat"));
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showAttackMonsterPopup(const int x, const int y,
                                       std::string name, const int type)
{
    if (!player_node || !actorSpriteManager)
        return;

    mNick = name;
    mType = static_cast<int>(Being::MONSTER);
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
        mBrowserBox->addRow(_("(default)"));
    else
        mBrowserBox->addRow(name);
    switch (type)
    {
        case MapItem::ATTACK:
        {
            const int idx = actorSpriteManager->getAttackMobIndex(name);
            const int size = actorSpriteManager->getAttackMobsSize();
            if (idx > 0)
                mBrowserBox->addRow("attack moveup", _("Move up"));
            if (idx + 1 < size)
                mBrowserBox->addRow("attack movedown", _("Move down"));
            break;
        }
        case MapItem::PRIORITY:
        {
            const int idx = actorSpriteManager->
                getPriorityAttackMobIndex(name);
            const int size = actorSpriteManager->getPriorityAttackMobsSize();
            if (idx > 0)
                mBrowserBox->addRow("priority moveup", _("Move up"));
            if (idx + 1 < size)
                mBrowserBox->addRow("priority movedown", _("Move down"));
            break;
        }
        case MapItem::IGNORE_:
            break;
        default:
            break;
    }

    mBrowserBox->addRow("attack remove", _("Remove"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPickupItemPopup(const int x, const int y, std::string name)
{
    if (!player_node || !actorSpriteManager)
        return;

    mNick = name;
    mType = static_cast<int>(Being::FLOOR_ITEM);
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
        mBrowserBox->addRow(_("(default)"));
    else
        mBrowserBox->addRow(name);

    mBrowserBox->addRow("pickup remove", _("Remove"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showUndressPopup(const int x, const int y,
                                 const Being *const being, Item *const item)
{
    if (!being || !item)
        return;

    mBeingId = being->getId();
    mItem = item;
    mItemId = item->getId();
    mItemColor = item->getColor();
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    mBrowserBox->addRow("undress item", _("Undress"));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showTextFieldPopup(int x, int y, TextField *input)
{
    mX = x;
    mY = y;
    mTextField = input;

    mBrowserBox->clearRows();

    mBrowserBox->addRow("clipboard copy", _("Copy"));
    mBrowserBox->addRow("clipboard paste", _("Paste"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showLinkPopup(const int x, const int y,
                              const std::string &link)
{
    mX = x;
    mY = y;
    mNick = link;

    mBrowserBox->clearRows();

    mBrowserBox->addRow("open link", _("Open link"));
    mBrowserBox->addRow("clipboard link", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showWindowsPopup(const int x, const int y)
{
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    const std::vector<ButtonText*> &names = windowMenu->getButtonTexts();
    mBrowserBox->addRow(_("Show window"));

    FOR_EACH (std::vector<ButtonText*>::const_iterator, it, names)
    {
        const ButtonText *const btn = *it;
        if (!btn)
            continue;

        mBrowserBox->addRow(strprintf("show window_%d", btn->key),
            btn->text.c_str());
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y)
{
    const int pad2 = 2 * mPadding;
    const int bPad2 = 2 * mBrowserBox->getPadding();
    mBrowserBox->setPosition(mPadding, mPadding);
    mScrollArea->setPosition(mPadding, mPadding);
    // add padding to initial size before draw browserbox
    int height = mBrowserBox->getHeight();
    if (height + pad2 >= mainGraphics->getHeight())
    {
        height = mainGraphics->getHeight() - bPad2 - pad2;
        mBrowserBox->setWidth(mBrowserBox->getWidth() + bPad2 + 5);
        mScrollArea->setWidth(mBrowserBox->getWidth() + pad2 + 10);
        setContentSize(mBrowserBox->getWidth() + pad2 + 20,
            height + pad2);
    }
    else
    {
        mBrowserBox->setWidth(mBrowserBox->getWidth() + bPad2);
        mScrollArea->setWidth(mBrowserBox->getWidth() + pad2);
        setContentSize(mBrowserBox->getWidth() + pad2,
            height + pad2);
    }
    if (mainGraphics->mWidth < (x + getWidth() + 5))
        x = mainGraphics->mWidth - getWidth();
    if (mainGraphics->mHeight < (y + getHeight() + 5))
        y = mainGraphics->mHeight - getHeight();
    mScrollArea->setHeight(height);
    setPosition(x, y);
    setVisible(true);
    requestMoveToTop();
}

void PopupMenu::addPlayerRelation(std::string name)
{
    switch (player_relations.getRelation(name))
    {
        case PlayerRelation::NEUTRAL:
            mBrowserBox->addRow("friend", _("Be friend"));
            mBrowserBox->addRow("disregard", _("Disregard"));
            mBrowserBox->addRow("ignore", _("Ignore"));
            mBrowserBox->addRow("blacklist", _("Black list"));
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::FRIEND:
            mBrowserBox->addRow("disregard", _("Disregard"));
            mBrowserBox->addRow("ignore", _("Ignore"));
            mBrowserBox->addRow("blacklist", _("Black list"));
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::BLACKLISTED:
            mBrowserBox->addRow("unignore", _("Unignore"));
            mBrowserBox->addRow("disregard", _("Disregard"));
            mBrowserBox->addRow("ignore", _("Ignore"));
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::DISREGARDED:
            mBrowserBox->addRow("unignore", _("Unignore"));
            mBrowserBox->addRow("ignore", _("Completely ignore"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::IGNORED:
            mBrowserBox->addRow("unignore", _("Unignore"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::ENEMY2:
            mBrowserBox->addRow("unignore", _("Unignore"));
            mBrowserBox->addRow("disregard", _("Disregard"));
            mBrowserBox->addRow("ignore", _("Ignore"));
            mBrowserBox->addRow("blacklist", _("Black list"));
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::ERASED:
            mBrowserBox->addRow("unignore", _("Unignore"));
            mBrowserBox->addRow("disregard", _("Disregard"));
            mBrowserBox->addRow("ignore", _("Completely ignore"));
            break;

        default:
            break;
    }
}

void PopupMenu::addFollow()
{
    mBrowserBox->addRow("follow", _("Follow"));
    mBrowserBox->addRow("imitation", _("Imitation"));
}

void PopupMenu::addBuySell(const Being *const being)
{
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow("##3---");
        if (being->isAdvanced())
        {
            if (being->isShopEnabled())
            {
                mBrowserBox->addRow("buy", _("Buy"));
                mBrowserBox->addRow("sell", _("Sell"));
            }
        }
        else
        {
            mBrowserBox->addRow("buy", _("Buy (?)"));
            mBrowserBox->addRow("sell", _("Sell (?)"));
        }
    }
}

void PopupMenu::addBuySellDefault()
{
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow("buy", _("Buy (?)"));
        mBrowserBox->addRow("sell", _("Sell (?)"));
    }
}

void PopupMenu::addParty(const std::string &partyName)
{
    if (player_node->isInParty())
    {
        if (player_node->getParty())
        {
            if (player_node->getParty()->getName() != partyName)
                mBrowserBox->addRow("party", _("Invite to party"));
            else
                mBrowserBox->addRow("kick party", _("Kick from party"));
            mBrowserBox->addRow("##3---");
        }
    }
}

void PopupMenu::addPlayerMisc()
{
    mBrowserBox->addRow("items", _("Show Items"));
    mBrowserBox->addRow("undress", _("Undress"));
    mBrowserBox->addRow("addcomment", _("Add comment"));
}

void PopupMenu::addPickupFilter(const std::string &name)
{
    if (actorSpriteManager->isInPickupList(name)
        || actorSpriteManager->isInIgnorePickupList(name))
    {
        mBrowserBox->addRow("remove pickup",
            _("Remove from pickup list"));
    }
    else
    {
        mBrowserBox->addRow("add pickup", _("Add to pickup list"));
        mBrowserBox->addRow("add pickup ignore",
            _("Add to ignore list"));
    }
}

void PopupMenu::showPopup(const int x, const int y, gcn::ListModel *model)
{
    if (!model)
        return;

    mBrowserBox->clearRows();
    for (int f = 0, sz = model->getNumberOfElements(); f < sz; f ++)
    {
        mBrowserBox->addRow(strprintf("dropdown_%d", f),
            model->getElementAt(f).c_str());
    }
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(x, y);
}

void PopupMenu::clear()
{
    if (mDialog)
    {
        mDialog->close();
        mDialog = nullptr;
    }
    mItem = nullptr;
    mMapItem = nullptr;
    mTab = nullptr;
    mSpell = nullptr;
    mWindow = nullptr;
    mButton = nullptr;
    mTextField = nullptr;
}

RenameListener::RenameListener() :
    gcn::ActionListener(),
    mMapItem(nullptr),
    mDialog(nullptr)
{
}

void RenameListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && mMapItem && viewport && mDialog)
    {
        Map *const map = viewport->getMap();
        if (!map)
            return;

        SpecialLayer *const sl = map->getSpecialLayer();
        MapItem *item = nullptr;
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
    mDialog = nullptr;
}

PlayerListener::PlayerListener() :
    ActionListener(),
    mNick(""),
    mDialog(nullptr),
    mType(static_cast<int>(Being::UNKNOWN))
{
}

void PlayerListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && !mNick.empty() && mDialog)
    {
        std::string comment = mDialog->getText();
        Being *const being  = actorSpriteManager->findBeingByName(
            mNick, static_cast<ActorSprite::Type>(mType));
        if (being)
            being->setComment(comment);
        Being::saveComment(mNick, comment, mType);
    }
    mDialog = nullptr;
}
