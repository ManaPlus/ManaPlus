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

#include "gui/popups/popupmenu.h"

#include "actormanager.h"
#include "commandhandler.h"
#include "configuration.h"
#include "dropshortcut.h"
#include "game.h"
#include "guild.h"
#include "guildmanager.h"
#include "item.h"
#include "maplayer.h"
#include "party.h"
#include "spellmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "input/inputmanager.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/textcommandeditor.h"
#include "gui/windows/textdialog.h"
#include "gui/windows/tradewindow.h"
#include "gui/windowmenu.h"

#include "gui/viewport.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/tabs/chattab.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/tabs/whispertab.h"

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/buysellhandler.h"
#include "net/guildhandler.h"
#include "net/inventoryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/partyhandler.h"
#include "net/tradehandler.h"

#include "resources/iteminfo.h"

#include "utils/copynpaste.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include <guichan/listmodel.hpp>

#include "debug.h"

extern int serverVersion;

std::string tradePartnerName;

PopupMenu::PopupMenu() :
    Popup("PopupMenu", "popupmenu.xml"),
    mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "popupbrowserbox.xml")),
    mScrollArea(nullptr),
    mBeingId(0),
    mFloorItemId(0),
    mItem(nullptr),
    mItemId(0),
    mItemColor(1),
    mMapItem(nullptr),
    mTab(nullptr),
    mSpell(nullptr),
    mWindow(nullptr),
    mRenameListener(),
    mPlayerListener(),
    mDialog(nullptr),
    mButton(nullptr),
    mNick(),
    mTextField(nullptr),
    mType(static_cast<int>(Being::UNKNOWN)),
    mX(0),
    mY(0)
{
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mRenameListener.setMapItem(nullptr);
    mRenameListener.setDialog(nullptr);
    mPlayerListener.setNick("");
    mPlayerListener.setDialog(nullptr);
    mPlayerListener.setType(static_cast<int>(Being::UNKNOWN));
    mScrollArea = new ScrollArea(mBrowserBox, false);
    mScrollArea->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
}

void PopupMenu::postInit()
{
    add(mScrollArea);
}

void PopupMenu::showPopup(const int x, const int y, const Being *const being)
{
    if (!being || !player_node || !actorManager)
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
            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade with player
            mBrowserBox->addRow("trade", _("Trade"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade attack player
            mBrowserBox->addRow("attack", _("Attack"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: send whisper to player
            mBrowserBox->addRow("whisper", _("Whisper"));
            addGmCommands();
            mBrowserBox->addRow("##3---");

            // TRANSLATORS: popup menu item
            // TRANSLATORS: heal player
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
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: kick player from guild
                            _("Kick from guild"));
                        if (guild2->getServerGuild())
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@guild-pos|%s >@@",
                                // TRANSLATORS: popup menu item
                                // TRANSLATORS: change player position in guild
                                _("Change pos in guild")));
                        }
                    }
                }
                else if (guild2->getMember(mNick))
                {
                    mBrowserBox->addRow("guild-kick",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: kick player from guild
                        _("Kick from guild"));
                    if (guild2->getServerGuild())
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@guild-pos|%s >@@",
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: change player position in guild
                            _("Change pos in guild")));
                    }
                }
                else
                {
                    if (guild2->getServerGuild()
                        || (guildManager && guildManager->havePower()))
                    {
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: invite player to guild
                        mBrowserBox->addRow("guild", _("Invite to guild"));
                    }
                }
            }

            // TRANSLATORS: popup menu item
            // TRANSLATORS: set player invisible for self by id
            mBrowserBox->addRow("nuke", _("Nuke"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to player location
            mBrowserBox->addRow("move", _("Move"));
            addPlayerMisc();
            addBuySell(being);
            break;
        }

        case ActorSprite::NPC:
            // NPCs can be talked to (single option, candidate for removal
            // unless more options would be added)
            // TRANSLATORS: popup menu item
            // TRANSLATORS: talk with npc
            mBrowserBox->addRow("talk", _("Talk"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: buy from npc
            mBrowserBox->addRow("buy", _("Buy"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: sell to npc
            mBrowserBox->addRow("sell", _("Sell"));
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to npc location
            mBrowserBox->addRow("move", _("Move"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add comment to npc
            mBrowserBox->addRow("addcomment", _("Add comment"));
            break;

        case ActorSprite::MONSTER:
        {
            // Monsters can be attacked
            // TRANSLATORS: popup menu item
            // TRANSLATORS: attack monster
            mBrowserBox->addRow("attack", _("Attack"));

            if (config.getBoolValue("enableAttackFilter"))
            {
                mBrowserBox->addRow("##3---");
                if (actorManager->isInAttackList(name)
                    || actorManager->isInIgnoreAttackList(name)
                    || actorManager->isInPriorityAttackList(name))
                {
                    mBrowserBox->addRow("remove attack",
                        // TRANSLATORS: remove monster from attack list
                        // TRANSLATORS: popup menu item
                        _("Remove from attack list"));
                }
                else
                {
                    mBrowserBox->addRow("add attack priority",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to priotiry attack list
                        _("Add to priority attack list"));
                    mBrowserBox->addRow("add attack",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to attack list
                        _("Add to attack list"));
                    mBrowserBox->addRow("add attack ignore",
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: add monster to ignore list
                        _("Add to ignore list"));
                }
            }
            break;
        }

        case ActorSprite::AVATAR:
        case ActorSprite::UNKNOWN:
        case ActorSprite::FLOOR_ITEM:
        case ActorSprite::PORTAL:
        case ActorSprite::PET:
        default:
            /* Other beings aren't interesting... */
            return;
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add being name to chat
    mBrowserBox->addRow("name", _("Add name to chat"));
    mBrowserBox->addRow("##3---");

    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPopup(const int x, const int y,
                          std::vector<ActorSprite*> &beings)
{
    mX = x;
    mY = y;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Players"));
    FOR_EACH (std::vector<ActorSprite*>::const_iterator, it, beings)
    {
        const Being *const being = dynamic_cast<const Being*>(*it);
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
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));
    showPopup(x, y);
}

void PopupMenu::showPlayerPopup(const int x, const int y,
                                const std::string &nick)
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

    // TRANSLATORS: popup menu item
    // TRANSLATORS: send whisper to player
    mBrowserBox->addRow("whisper", _("Whisper"));
    addGmCommands();
    mBrowserBox->addRow("##3---");

    addPlayerRelation(name);
    mBrowserBox->addRow("##3---");

    addFollow();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add comment to player
    mBrowserBox->addRow("addcomment", _("Add comment"));

    if (player_node->isInParty())
    {
        const Party *const party = player_node->getParty();
        if (party)
        {
            const PartyMember *const member = party->getMember(mNick);
            if (member)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from party
                mBrowserBox->addRow("kick party", _("Kick from party"));
                mBrowserBox->addRow("##3---");
                const PartyMember *const o = party->getMember(
                    player_node->getName());
                if (o && member->getMap() == o->getMap())
                {
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: move to player position
                    mBrowserBox->addRow("move", _("Move"));
                }
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
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from guild
                mBrowserBox->addRow("guild-kick", _("Kick from guild"));
            }
            if (guild2->getServerGuild())
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: change player position in guild
                mBrowserBox->addRow(strprintf(
                    "@@guild-pos|%s >@@", _("Change pos in guild")));
            }
        }
        else
        {
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: invite player to guild
                mBrowserBox->addRow("guild", _("Invite to guild"));
            }
        }
    }

    addBuySellDefault();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add player name to chat
    mBrowserBox->addRow("name", _("Add name to chat"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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
    const std::string name = floorItem->getName();
    mNick = name;

    mBrowserBox->addRow(name);

    if (config.getBoolValue("enablePickupFilter"))
    {
        if (actorManager->isInPickupList(name)
            || (actorManager->isInPickupList("")
            && !actorManager->isInIgnorePickupList(name)))
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: pickup item from ground
            mBrowserBox->addRow("pickup", _("Pick up"));
            mBrowserBox->addRow("##3---");
        }
        addPickupFilter(name);
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: pickup item from ground
        mBrowserBox->addRow("pickup", _("Pick up"));
    }
    addProtection();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add item name to chat
    mBrowserBox->addRow("chat", _("Add to chat"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Map Item"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: rename map item
    mBrowserBox->addRow("rename map", _("Rename"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove map item
    mBrowserBox->addRow("remove map", _("Remove"));

    if (player_node && player_node->isGM())
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp to map item
        mBrowserBox->addRow("warp map", _("Warp"));
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showMapPopup(const int x, const int y,
                             const int x2, const int y2)
{
    mX = x2;
    mY = y2;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Map Item"));

    if (player_node && player_node->isGM())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: warp to map item
        mBrowserBox->addRow("warp map", _("Warp"));
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: move to map item
    mBrowserBox->addRow("move", _("Move"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: move camera to map item
    mBrowserBox->addRow("movecamera", _("Move camera"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showOutfitsPopup(const int x, const int y)
{
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Outfits"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: clear selected outfit
    mBrowserBox->addRow("clear outfit", _("Clear outfit"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Spells"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: edit selected spell
    mBrowserBox->addRow("edit spell", _("Edit spell"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChatPopup(const int x, const int y, ChatTab *const tab)
{
    if (!tab || !actorManager || !player_node)
        return;

    mTab = tab;
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (tab->getType() == static_cast<int>(ChatTab::TAB_WHISPER))
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close chat tab
        mBrowserBox->addRow("chat close", _("Close"));
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove all text from chat tab
    mBrowserBox->addRow("chat clear", _("Clear"));
    mBrowserBox->addRow("##3---");

    if (tab->getAllowHighlight())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: disable chat tab highlight
        mBrowserBox->addRow("disable highlight", _("Disable highlight"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable chat tab highlight
        mBrowserBox->addRow("enable highlight", _("Enable highlight"));
    }
    if (tab->getRemoveNames())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: do not remove player names from chat tab
        mBrowserBox->addRow("dont remove name", _("Don't remove name"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: remove player names from chat tab
        mBrowserBox->addRow("remove name", _("Remove name"));
    }
    if (tab->getNoAway())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable away messages in chat tab
        mBrowserBox->addRow("enable away", _("Enable away"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: disable away messages in chat tab
        mBrowserBox->addRow("disable away", _("Disable away"));
    }
    mBrowserBox->addRow("##3---");
    if (tab->getType() == static_cast<int>(ChatTab::TAB_PARTY))
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: enable away messages in chat tab
        mBrowserBox->addRow("leave party", _("Leave"));
        mBrowserBox->addRow("##3---");
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy selected text to clipboard
    mBrowserBox->addRow("chat clipboard", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");

    if (tab->getType() == static_cast<int>(ChatTab::TAB_WHISPER))
    {
        const WhisperTab *const wTab = static_cast<WhisperTab*>(tab);
        std::string name = wTab->getNick();

        const Being* const being  = actorManager->findBeingByName(
            name, Being::PLAYER);

        if (being)
        {
            mBeingId = being->getId();
            mNick = being->getName();
            mType = static_cast<int>(being->getType());

            // TRANSLATORS: popup menu item
            // TRANSLATORS: trade with player
            mBrowserBox->addRow("trade", _("Trade"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: attack player
            mBrowserBox->addRow("attack", _("Attack"));
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: heal player
            mBrowserBox->addRow("heal", _("Heal"));
            mBrowserBox->addRow("##3---");
            addPlayerRelation(name);
            mBrowserBox->addRow("##3---");
            addFollow();
            // TRANSLATORS: popup menu item
            // TRANSLATORS: move to player position
            mBrowserBox->addRow("move", _("Move"));
            addPlayerMisc();
            addBuySell(being);
            mBrowserBox->addRow("##3---");

            if (player_node->isInParty())
            {
                const Party *const party = player_node->getParty();
                if (party)
                {
                    if (!party->isMember(wTab->getNick()))
                    {
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: invite player to party
                        mBrowserBox->addRow("party", _("Invite to party"));
                    }
                    else
                    {
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: kick player from party
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
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: kick player from guild
                            mBrowserBox->addRow(strprintf(
                                "@@guild-kick|%s@@", _("Kick from guild")));
                        }
                        if (guild2->getServerGuild())
                        {
                            // TRANSLATORS: popup menu item
                            // TRANSLATORS: change player position in guild
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
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: invite player to guild
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
                    {
                        // TRANSLATORS: popup menu item
                        // TRANSLATORS: move to player location
                        mBrowserBox->addRow("move", _("Move"));
                    }
                }
            }
            addPlayerMisc();
            addBuySellDefault();
            mBrowserBox->addRow("##3---");
        }
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showChangePos(const int x, const int y)
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("Change guild position"));

    if (!player_node)
        return;

    mX = x;
    mY = y;
    const Guild *const guild = player_node->getGuild();
    if (guild)
    {
        const PositionsMap map = guild->getPositions();
        FOR_EACH (PositionsMap::const_iterator, itr, map)
        {
            mBrowserBox->addRow(strprintf("@@guild-pos-%u|%s@@",
                itr->first, itr->second.c_str()));
        }
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close menu
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
    if (actorManager)
        being = actorManager->findBeing(mBeingId);

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
    else if (link == "attack" && being)
    {
        if (player_node)
            player_node->attack(being, true);
    }
    else if (link == "heal" && being && being->getType() != Being::MONSTER)
    {
        if (actorManager)
            actorManager->heal(being);
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
        if (actorManager)
        {
            actorManager->addBlock(static_cast<uint32_t>(
                being->getId()));
            actorManager->destroy(being);
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
        if (player_node && actorManager)
        {
            FloorItem *const item = actorManager->findItem(
                mFloorItemId);
            if (item)
                player_node->pickUp(item);
        }
    }
    else if (link == "use" && mItemId)
    {
        if (mItemId < SPELL_MIN_ID)
        {
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
            {
                const Item *const item = inv->findItem(mItemId, mItemColor);
                PlayerInfo::useEquipItem(item, true);
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
            else if (mFloorItemId && actorManager)
            {
                const FloorItem *const item = actorManager->findItem(
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
    else if (link == "whisper" && !mNick.empty())
    {
        if (chatWindow)
        {
            if (config.getBoolValue("whispertab"))
            {
                chatWindow->localChatInput("/q " + mNick);
            }
            else
            {
                chatWindow->addInputText(std::string("/w \"").append(
                    mNick).append("\" "));
            }
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
            viewport->moveCameraToPosition(mX * mapTileSize, mY * mapTileSize);
    }
    else if (link == "split" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::ItemSplit,
                             inventoryWindow, mItem);
    }
    else if (link == "drop" && mItem)
    {
        if (!PlayerInfo::isItemProtected(mItem->getId()))
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::ItemDrop,
                inventoryWindow, mItem);
        }
    }
    else if (link == "drop all" && mItem)
    {
        PlayerInfo::dropItem(mItem, mItem->getQuantity(), true);
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
        if (!PlayerInfo::isItemProtected(mItem->getId()))
        {
            ItemAmountWindow::showWindow(ItemAmountWindow::TradeAdd,
                tradeWindow, mItem);
        }
    }
    else if (link == "addtrade 10" && mItem)
    {
        if (tradeWindow && !PlayerInfo::isItemProtected(mItem->getId()))
        {
            int cnt = 10;
            if (cnt > mItem->getQuantity())
                cnt = mItem->getQuantity();
            tradeWindow->tradeItem(mItem, cnt, true);
        }
    }
    else if (link == "addtrade half" && mItem)
    {
        if (tradeWindow && !PlayerInfo::isItemProtected(mItem->getId()))
            tradeWindow->tradeItem(mItem, mItem->getQuantity() / 2, true);
    }
    else if (link == "addtrade all-1" && mItem)
    {
        if (tradeWindow && !PlayerInfo::isItemProtected(mItem->getId()))
            tradeWindow->tradeItem(mItem, mItem->getQuantity() - 1, true);
    }
    else if (link == "addtrade all" && mItem)
    {
        if (tradeWindow && !PlayerInfo::isItemProtected(mItem->getId()))
            tradeWindow->tradeItem(mItem, mItem->getQuantity(), true);
    }
    else if (link == "retrieve" && mItem)
    {
        ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove,
            mWindow, mItem);
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
    else if (link == "protect item" && mItemId)
    {
        PlayerInfo::protectItem(mItemId);
    }
    else if (link == "unprotect item" && mItemId)
    {
        PlayerInfo::unprotectItem(mItemId);
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
        if (commandHandler)
            commandHandler->invokeCommand("close", "", mTab);
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
            const Map *const map = viewport->getMap();
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
        mDialog->postInit();
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
        (new TextCommandEditor(mSpell))->postInit();
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
        dialog->postInit();
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
                    Net::getGuildHandler()->kick(guild->getMember(mNick), "");
                else if (guildManager)
                    guildManager->kick(mNick);
            }
        }
    }
    else if (link == "enable highlight" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_ENABLEHIGHLIGHT, mTab);
    }
    else if (link == "disable highlight" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_DISABLEHIGHLIGHT, mTab);
    }
    else if (link == "dont remove name" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_DONTREMOVENAME, mTab);
    }
    else if (link == "remove name" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_REMOVENAME, mTab);
    }
    else if (link == "disable away" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_DISABLEAWAY, mTab);
    }
    else if (link == "enable away" && mTab)
    {
        if (commandHandler)
            commandHandler->invokeCommand(COMMAND_ENABLEAWAY, mTab);
    }
    else if (link == "chat clipboard" && mTab)
    {
        if (chatWindow)
            chatWindow->copyToClipboard(mX, mY);
    }
    else if (link == "npc clipboard" && mBeingId)
    {
        NpcDialog::copyToClipboard(mBeingId, mX, mY);
    }
    else if (link == "remove attack" && being)
    {
        if (actorManager && being->getType() == Being::MONSTER)
        {
            actorManager->removeAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack" && being)
    {
        if (actorManager && being->getType() == Being::MONSTER)
        {
            actorManager->addAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack priority" && being)
    {
        if (actorManager && being->getType() == Being::MONSTER)
        {
            actorManager->addPriorityAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "add attack ignore" && being)
    {
        if (actorManager && being->getType() == Being::MONSTER)
        {
            actorManager->addIgnoreAttackMob(being->getName());
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "remove pickup" && !mNick.empty())
    {
        if (actorManager)
        {
            actorManager->removePickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "add pickup" && !mNick.empty())
    {
        if (actorManager)
        {
            actorManager->addPickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "add pickup ignore" && !mNick.empty())
    {
        if (actorManager)
        {
            actorManager->addIgnorePickupItem(mNick);
            if (socialWindow)
                socialWindow->updatePickupFilter();
        }
    }
    else if (link == "attack moveup")
    {
        if (actorManager)
        {
            const int idx = actorManager->getAttackMobIndex(mNick);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorManager->getAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorManager->setAttackMobs(mobs);
                        actorManager->rebuildAttackMobs();
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
        if (actorManager)
        {
            const int idx = actorManager->
                getPriorityAttackMobIndex(mNick);
            if (idx > 0)
            {
                std::list<std::string> mobs
                    = actorManager->getPriorityAttackMobs();
                std::list<std::string>::iterator it = mobs.begin();
                std::list<std::string>::iterator it2 = mobs.begin();
                while (it != mobs.end())
                {
                    if (*it == mNick)
                    {
                        -- it2;
                        mobs.splice(it2, mobs, it);
                        actorManager->setPriorityAttackMobs(mobs);
                        actorManager->rebuildPriorityAttackMobs();
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
        if (actorManager)
        {
            const int idx = actorManager->getAttackMobIndex(mNick);
            const int size = actorManager->getAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorManager->getAttackMobs();
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
                        actorManager->setAttackMobs(mobs);
                        actorManager->rebuildAttackMobs();
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
            const int idx = actorManager
                ->getPriorityAttackMobIndex(mNick);
            const int size = actorManager->getPriorityAttackMobsSize();
            if (idx + 1 < size)
            {
                std::list<std::string> mobs
                    = actorManager->getPriorityAttackMobs();
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
                        actorManager->setPriorityAttackMobs(mobs);
                        actorManager->rebuildPriorityAttackMobs();
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
        if (actorManager)
        {
            if (mNick.empty())
            {
                if (actorManager->isInAttackList(mNick))
                {
                    actorManager->removeAttackMob(mNick);
                    actorManager->addIgnoreAttackMob(mNick);
                }
                else
                {
                    actorManager->removeAttackMob(mNick);
                    actorManager->addAttackMob(mNick);
                }
            }
            else
            {
                actorManager->removeAttackMob(mNick);
            }
            if (socialWindow)
                socialWindow->updateAttackFilter();
        }
    }
    else if (link == "pickup remove")
    {
        if (actorManager)
        {
            if (mNick.empty())
            {
                if (actorManager->isInPickupList(mNick))
                {
                    actorManager->removePickupItem(mNick);
                    actorManager->addIgnorePickupItem(mNick);
                }
                else
                {
                    actorManager->removePickupItem(mNick);
                    actorManager->addPickupItem(mNick);
                }
            }
            else
            {
                actorManager->removePickupItem(mNick);
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
            if (equipmentWindow && !equipmentWindow->isWindowVisible())
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
    else if (link == "goto" && !mNick.empty())
    {
        Net::getAdminHandler()->gotoName(mNick);
    }
    else if (link == "recall" && !mNick.empty())
    {
        Net::getAdminHandler()->recallName(mNick);
    }
    else if (link == "revive" && !mNick.empty())
    {
        Net::getAdminHandler()->reviveName(mNick);
    }
    else if (link == "ipcheck" && !mNick.empty())
    {
        Net::getAdminHandler()->ipcheckName(mNick);
    }
    else if (link == "gm" && !mNick.empty())
    {
        showGMPopup();
        return;
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
        if (actorManager)
        {
            mBeingId = atoi(link.substr(7).c_str());
            being = actorManager->findBeing(mBeingId);
            if (being)
            {
                showPopup(getX(), getY(), being);
                return;
            }
        }
    }
    else if (!link.compare(0, 10, "flooritem_"))
    {
        if (actorManager)
        {
            const int id = atoi(link.substr(10).c_str());
            if (id)
            {
                const FloorItem *const item = actorManager->findItem(id);
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
    const bool isProtected = PlayerInfo::isItemProtected(mItemId);

    if (isInventory)
    {
        if (tradeWindow && tradeWindow->isWindowVisible() && !isProtected)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to trade
            mBrowserBox->addRow("addtrade", _("Add to trade"));
            if (cnt > 1)
            {
                if (cnt > 10)
                {
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: add 10 item amount to trade
                    mBrowserBox->addRow("addtrade 10", _("Add to trade 10"));
                }
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add half item amount to trade
                mBrowserBox->addRow("addtrade half", _("Add to trade half"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add all amount except one item to trade
                mBrowserBox->addRow("addtrade all-1", _("Add to trade all-1"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add all amount item to trade
                mBrowserBox->addRow("addtrade all", _("Add to trade all"));
            }
            mBrowserBox->addRow("##3---");
        }
        if (InventoryWindow::isStorageActive())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to storage
            mBrowserBox->addRow("store", _("Store"));
            if (cnt > 1)
            {
                if (cnt > 10)
                {
                    // TRANSLATORS: popup menu item
                    // TRANSLATORS: add 10 item amount to storage
                    mBrowserBox->addRow("store 10", _("Store 10"));
                }
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add half item amount to storage
                mBrowserBox->addRow("store half", _("Store half"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add all except one item amount to storage
                mBrowserBox->addRow("store all-1", _("Store all-1"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: add all item amount to storage
                mBrowserBox->addRow("store all", _("Store all"));
            }
            mBrowserBox->addRow("##3---");
        }

        addUseDrop(item, isProtected);
    }
    // Assume in storage for now
    else
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: get item from storage
        mBrowserBox->addRow("retrieve", _("Retrieve"));
        if (cnt > 1)
        {
            if (cnt > 10)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: get 10 item amount from storage
                mBrowserBox->addRow("retrieve 10", _("Retrieve 10"));
            }
            // TRANSLATORS: popup menu item
            // TRANSLATORS: get half item amount from storage
            mBrowserBox->addRow("retrieve half", _("Retrieve half"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: get all except one item amount from storage
            mBrowserBox->addRow("retrieve all-1", _("Retrieve all-1"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: get all item amount from storage
            mBrowserBox->addRow("retrieve all", _("Retrieve all"));
        }
    }
    addProtection();
    if (config.getBoolValue("enablePickupFilter"))
    {
        mNick = item->getName();
        mBrowserBox->addRow("##3---");
        addPickupFilter(mNick);
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add item name to chat
    mBrowserBox->addRow("chat", _("Add to chat"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

        if (!PlayerInfo::isItemProtected(mItemId))
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: use item
            mBrowserBox->addRow("use", _("Use"));
        }
        addProtection();
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: close menu
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
        const bool isProtected = PlayerInfo::isItemProtected(mItemId);
        addUseDrop(item, isProtected);
        if (InventoryWindow::isStorageActive())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to storage
            mBrowserBox->addRow("store", _("Store"));
        }
        // TRANSLATORS: popup menu item
        // TRANSLATORS: add item name to chat
        mBrowserBox->addRow("chat", _("Add to chat"));

        if (config.getBoolValue("enablePickupFilter"))
        {
            mNick = item->getName();

            mBrowserBox->addRow("##3---");
            addPickupFilter(mNick);
        }
    }
    addProtection();
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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
        const bool isProtected = PlayerInfo::isItemProtected(mItemId);
        addUseDrop(item, isProtected);
        if (InventoryWindow::isStorageActive())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to storage
            mBrowserBox->addRow("store", _("Store"));
        }
        addProtection();
        // TRANSLATORS: popup menu item
        // TRANSLATORS: add item name to chat
        mBrowserBox->addRow("chat", _("Add to chat"));
        if (config.getBoolValue("enablePickupFilter"))
        {
            mNick = item->getName();
            mBrowserBox->addRow("##3---");
            addPickupFilter(mNick);
        }
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("clear drops", _("Clear drop window"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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
        const Button *const btn = dynamic_cast<const Button*>(*it);
        if (!btn || btn->getActionEventId() == "SET")
            continue;

        if (btn->isVisible())
        {
            mBrowserBox->addRow(strprintf("@@hide button_%s|%s %s (%s)@@",
                // TRANSLATORS: popup menu item
                btn->getActionEventId().c_str(), _("Hide"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show button_%s|%s %s (%s)@@",
                // TRANSLATORS: popup menu item
                btn->getActionEventId().c_str(), _("Show"),
                btn->getDescription().c_str(), btn->getCaption().c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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
                // TRANSLATORS: popup menu item
                bar->getActionEventId().c_str(), _("Hide"),
                bar->getId().c_str()));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@show bar_%s|%s %s@@",
                // TRANSLATORS: popup menu item
                bar->getActionEventId().c_str(), _("Show"),
                bar->getId().c_str()));
        }
    }

    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("reset yellow", _("Reset yellow bar"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy status to chat
    mBrowserBox->addRow("bar to chat", _("Copy to chat"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showAttackMonsterPopup(const int x, const int y,
                                       const std::string &name, const int type)
{
    if (!player_node || !actorManager)
        return;

    mNick = name;
    mType = static_cast<int>(Being::MONSTER);
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("(default)"));
    }
    else
    {
        mBrowserBox->addRow(name);
    }
    switch (type)
    {
        case MapItem::ATTACK:
        {
            const int idx = actorManager->getAttackMobIndex(name);
            const int size = actorManager->getAttackMobsSize();
            if (idx > 0)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target up
                mBrowserBox->addRow("attack moveup", _("Move up"));
            }
            if (idx + 1 < size)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target down
                mBrowserBox->addRow("attack movedown", _("Move down"));
            }
            break;
        }
        case MapItem::PRIORITY:
        {
            const int idx = actorManager->
                getPriorityAttackMobIndex(name);
            const int size = actorManager->getPriorityAttackMobsSize();
            if (idx > 0)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target up
                mBrowserBox->addRow("priority moveup", _("Move up"));
            }
            if (idx + 1 < size)
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: move attack target down
                mBrowserBox->addRow("priority movedown", _("Move down"));
            }
            break;
        }
        case MapItem::IGNORE_:
            break;
        default:
            break;
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove attack target
    mBrowserBox->addRow("attack remove", _("Remove"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showPickupItemPopup(const int x, const int y,
                                    const std::string &name)
{
    if (!player_node || !actorManager)
        return;

    mNick = name;
    mType = static_cast<int>(Being::FLOOR_ITEM);
    mX = x;
    mY = y;

    mBrowserBox->clearRows();

    if (name.empty())
    {
        // TRANSLATORS: popup menu header
        mBrowserBox->addRow(_("(default)"));
    }
    else
    {
        mBrowserBox->addRow(name);
    }

    // TRANSLATORS: popup menu item
    // TRANSLATORS: remove item from pickup filter
    mBrowserBox->addRow("pickup remove", _("Remove"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

    // TRANSLATORS: popup menu item
    // TRANSLATORS: undress item from player
    mBrowserBox->addRow("undress item", _("Undress"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showTextFieldPopup(int x, int y, TextField *const input)
{
    mX = x;
    mY = y;
    mTextField = input;

    mBrowserBox->clearRows();

    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy text to clipboard
    mBrowserBox->addRow("clipboard copy", _("Copy"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: paste text from clipboard
    mBrowserBox->addRow("clipboard paste", _("Paste"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

    // TRANSLATORS: popup menu item
    // TRANSLATORS: open link in browser
    mBrowserBox->addRow("open link", _("Open link"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy link to clipboard
    mBrowserBox->addRow("clipboard link", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showWindowsPopup(const int x, const int y)
{
    mX = x;
    mY = y;

    mBrowserBox->clearRows();
    const std::vector<ButtonText*> &names = windowMenu->getButtonTexts();
    // TRANSLATORS: popup menu header
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
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(x, y);
}

void PopupMenu::showNpcDialogPopup(const int npcId, const int x, const int y)
{
    mBeingId = npcId;
    mX = x;
    mY = y;
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu item
    // TRANSLATORS: copy npc text to clipboard
    mBrowserBox->addRow("npc clipboard", _("Copy to clipboard"));
    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

void PopupMenu::addPlayerRelation(const std::string &name)
{
    switch (player_relations.getRelation(name))
    {
        case PlayerRelation::NEUTRAL:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to friends list
            mBrowserBox->addRow("friend", _("Be friend"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("disregard", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("ignore", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to black list
            mBrowserBox->addRow("blacklist", _("Black list"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to enemy list
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::FRIEND:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("disregard", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("ignore", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to black list
            mBrowserBox->addRow("blacklist", _("Black list"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to enemy list
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::BLACKLISTED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("unignore", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("disregard", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("ignore", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to enemy list
            mBrowserBox->addRow("enemy", _("Set as enemy"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::DISREGARDED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("unignore", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to completle ignore list
            mBrowserBox->addRow("ignore", _("Completely ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::IGNORED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("unignore", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::ENEMY2:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("unignore", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("disregard", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("ignore", _("Ignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to black list
            mBrowserBox->addRow("blacklist", _("Black list"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to erased list
            mBrowserBox->addRow("erase", _("Erase"));
            break;

        case PlayerRelation::ERASED:
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove player from ignore list
            mBrowserBox->addRow("unignore", _("Unignore"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to disregarded list
            mBrowserBox->addRow("disregard", _("Disregard"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add player to ignore list
            mBrowserBox->addRow("ignore", _("Completely ignore"));
            break;

        default:
            break;
    }
}

void PopupMenu::addFollow()
{
    if (features.getBoolValue("allowFollow"))
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("follow", _("Follow"));
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: imitate player
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
                // TRANSLATORS: popup menu item
                // TRANSLATORS: buy item
                mBrowserBox->addRow("buy", _("Buy"));
                // TRANSLATORS: popup menu item
                // TRANSLATORS: sell item
                mBrowserBox->addRow("sell", _("Sell"));
            }
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: buy item
            mBrowserBox->addRow("buy", _("Buy (?)"));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: sell item
            mBrowserBox->addRow("sell", _("Sell (?)"));
        }
    }
}

void PopupMenu::addBuySellDefault()
{
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: buy item
        mBrowserBox->addRow("buy", _("Buy (?)"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: sell item
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
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: invite player to party
                mBrowserBox->addRow("party", _("Invite to party"));
            }
            else
            {
                // TRANSLATORS: popup menu item
                // TRANSLATORS: kick player from party
                mBrowserBox->addRow("kick party", _("Kick from party"));
            }
            mBrowserBox->addRow("##3---");
        }
    }
}

void PopupMenu::addPlayerMisc()
{
    // TRANSLATORS: popup menu item
    mBrowserBox->addRow("items", _("Show Items"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: undress player
    mBrowserBox->addRow("undress", _("Undress"));
    // TRANSLATORS: popup menu item
    // TRANSLATORS: add comment to player
    mBrowserBox->addRow("addcomment", _("Add comment"));
}

void PopupMenu::addPickupFilter(const std::string &name)
{
    if (actorManager->isInPickupList(name)
        || actorManager->isInIgnorePickupList(name))
    {
        mBrowserBox->addRow("remove pickup",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: remove item from pickup list
            _("Remove from pickup list"));
    }
    else
    {
        // TRANSLATORS: popup menu item
        mBrowserBox->addRow("add pickup", _("Add to pickup list"));
        mBrowserBox->addRow("add pickup ignore",
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add item to pickup list
            _("Add to ignore list"));
    }
}

void PopupMenu::showPopup(const int x, const int y,
                          gcn::ListModel *const model)
{
    if (!model)
        return;

    mBrowserBox->clearRows();
    for (int f = 0, sz = model->getNumberOfElements(); f < sz; f ++)
    {
        mBrowserBox->addRow(strprintf("dropdown_%d", f),
            model->getElementAt(f).c_str());
    }
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
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

void PopupMenu::addProtection()
{
    if (PlayerInfo::isItemProtected(mItemId))
    {
        mBrowserBox->addRow("##3---");
        // TRANSLATORS: popup menu item
        // TRANSLATORS: remove protection from item
        mBrowserBox->addRow("unprotect item", _("Unprotect item"));
    }
    else
    {
        if (mItemId < SPELL_MIN_ID)
        {
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: popup menu item
            // TRANSLATORS: add protection to item
            mBrowserBox->addRow("protect item", _("Protect item"));
        }
    }
}

void PopupMenu::addUseDrop(const Item *const item, const bool isProtected)
{
    if (item->isEquipment())
    {
        if (item->isEquipped())
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: unequip item
            mBrowserBox->addRow("use", _("Unequip"));
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: use item
            mBrowserBox->addRow("use", _("Equip"));
        }
    }
    else
    {
        if (!isProtected)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: use item
            mBrowserBox->addRow("use", _("Use"));
        }
    }

    if (!isProtected)
    {
        mBrowserBox->addRow("##3---");
        if (item->getQuantity() > 1)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop item
            mBrowserBox->addRow("drop", _("Drop..."));
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop all item amount
            mBrowserBox->addRow("drop all", _("Drop all"));
        }
        else
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: drop item
            mBrowserBox->addRow("drop", _("Drop"));
        }
    }

    if (Net::getInventoryHandler()->canSplit(item))
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: split items
        mBrowserBox->addRow("split", _("Split"));
    }
}

void PopupMenu::addGmCommands()
{
    if (player_node->isGM())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: gm commands
        mBrowserBox->addRow("gm", _("GM..."));
    }
}

void PopupMenu::showGMPopup()
{
    mBrowserBox->clearRows();
    // TRANSLATORS: popup menu header
    mBrowserBox->addRow(_("GM commands"));
    if (player_node->isGM())
    {
        // TRANSLATORS: popup menu item
        // TRANSLATORS: check player ip
        mBrowserBox->addRow("ipcheck", _("Check ip"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: go to player position
        mBrowserBox->addRow("goto", _("Goto"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: recall player to current position
        mBrowserBox->addRow("recall", _("Recall"));
        // TRANSLATORS: popup menu item
        // TRANSLATORS: revive player
        mBrowserBox->addRow("revive", _("Revive"));
        if (mBeingId)
        {
            // TRANSLATORS: popup menu item
            // TRANSLATORS: kick player
            mBrowserBox->addRow("admin-kick", _("Kick"));
        }
    }

    mBrowserBox->addRow("##3---");
    // TRANSLATORS: popup menu item
    // TRANSLATORS: close menu
    mBrowserBox->addRow("cancel", _("Cancel"));

    showPopup(getX(), getY());
}

RenameListener::RenameListener() :
    gcn::ActionListener(),
    mMapItemX(0),
    mMapItemY(0),
    mDialog(nullptr)
{
}

void RenameListener::setMapItem(MapItem *const mapItem)
{
    if (mapItem)
    {
        mMapItemX = mapItem->getX();
        mMapItemY = mapItem->getY();
    }
    else
    {
        mMapItemX = 0;
        mMapItemY = 0;
    }
}

void RenameListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && viewport && mDialog)
    {
        Map *const map = viewport->getMap();
        if (!map)
            return;

        SpecialLayer *const sl = map->getSpecialLayer();
        MapItem *item = nullptr;
        if (sl)
        {
            item = sl->getTile(mMapItemX, mMapItemY);
            if (item)
                item->setComment(mDialog->getText());
        }
        item = map->findPortalXY(mMapItemX, mMapItemY);
        if (item)
            item->setComment(mDialog->getText());

        if (socialWindow)
            socialWindow->updatePortalNames();
    }
    mDialog = nullptr;
}

PlayerListener::PlayerListener() :
    ActionListener(),
    mNick(),
    mDialog(nullptr),
    mType(static_cast<int>(Being::UNKNOWN))
{
}

void PlayerListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && !mNick.empty() && mDialog)
    {
        std::string comment = mDialog->getText();
        Being *const being  = actorManager->findBeingByName(
            mNick, static_cast<ActorSprite::Type>(mType));
        if (being)
            being->setComment(comment);
        Being::saveComment(mNick, comment, mType);
    }
    mDialog = nullptr;
}
