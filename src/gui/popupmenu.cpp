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

#include "actorsprite.h"
#include "actorspritemanager.h"
#include "being.h"
#include "dropshortcut.h"
#include "guild.h"
#include "guildmanager.h"
#include "flooritem.h"
#include "graphics.h"
#include "item.h"
#include "itemshortcut.h"
#include "localplayer.h"
#include "logger.h"
#include "map.h"
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
#include "gui/socialwindow.h"
#include "gui/textcommandeditor.h"
#include "gui/textdialog.h"
#include "gui/tradewindow.h"
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

#include "debug.h"

std::string tradePartnerName("");

PopupMenu::PopupMenu():
    Popup("PopupMenu", "popupmenu.xml"),
    mBeingId(0),
    mFloorItem(0),
    mItem(0),
    mItemId(0),
    mItemColor(1),
    mMapItem(0),
    mTab(0),
    mSpell(0),
    mDialog(0),
    mButton(0),
    mNick(""),
    mType(Being::UNKNOWN)
{
    mBrowserBox = new BrowserBox;
    mBrowserBox->setPosition(4, 4);
    mBrowserBox->setHighlightMode(BrowserBox::BACKGROUND);
    mBrowserBox->setOpaque(false);
    mBrowserBox->setLinkHandler(this);
    mRenameListener.setMapItem(0);
    mRenameListener.setDialog(0);
    mPlayerListener.setNick("");
    mPlayerListener.setDialog(0);
    mPlayerListener.setType(Being::UNKNOWN);

    add(mBrowserBox);
}

void PopupMenu::showPopup(int x, int y, Being *being)
{
    if (!being || !player_node || !actorSpriteManager)
        return;

    mBeingId = being->getId();
    mNick = being->getName();
    mType = being->getType();
    mBrowserBox->clearRows();

    const std::string &name = mNick;

    mBrowserBox->addRow(name + being->getGenderSignWithSpace());

    switch (being->getType())
    {
        case ActorSprite::PLAYER:
            {
                mBrowserBox->addRow(strprintf("@@trade|%s@@", _("Trade")));
                mBrowserBox->addRow(strprintf("@@attack|%s@@", _("Attack")));
                mBrowserBox->addRow(strprintf("@@whisper|%s@@", _("Whisper")));

                mBrowserBox->addRow("##3---");

                mBrowserBox->addRow(strprintf("@@heal|%s@@", _("Heal")));
                mBrowserBox->addRow("##3---");

                switch (player_relations.getRelation(name))
                {
                    case PlayerRelation::NEUTRAL:
                        mBrowserBox->addRow(strprintf(
                            "@@friend|%s@@", _("Be friend")));
                        mBrowserBox->addRow(strprintf(
                            "@@disregard|%s@@", _("Disregard")));
                        mBrowserBox->addRow(strprintf(
                            "@@ignore|%s@@", _("Ignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@erase|%s@@", _("Erase")));
                        break;

                    case PlayerRelation::FRIEND:
                        mBrowserBox->addRow(strprintf(
                            "@@disregard|%s@@", _("Disregard")));
                        mBrowserBox->addRow(strprintf(
                            "@@ignore|%s@@", _("Ignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@erase|%s@@", _("Erase")));
                        break;

                    case PlayerRelation::DISREGARDED:
                        mBrowserBox->addRow(strprintf(
                            "@@unignore|%s@@", _("Unignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@ignore|%s@@", _("Completely ignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@erase|%s@@", _("Erase")));
                        break;

                    case PlayerRelation::IGNORED:
                        mBrowserBox->addRow(strprintf(
                            "@@unignore|%s@@", _("Unignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@erase|%s@@", _("Erase")));
                        break;

                    case PlayerRelation::ERASED:
                        mBrowserBox->addRow(strprintf(
                            "@@unignore|%s@@", _("Unignore")));
                        mBrowserBox->addRow(strprintf(
                            "@@disregard|%s@@", _("Disregard")));
                        mBrowserBox->addRow(strprintf(
                            "@@ignore|%s@@", _("Completely ignore")));
                        break;

                    default:
                        break;
                }

                mBrowserBox->addRow("##3---");
                mBrowserBox->addRow(strprintf("@@follow|%s@@", _("Follow")));
                mBrowserBox->addRow(strprintf(
                    "@@imitation|%s@@", _("Imitation")));

                if (player_node->isInParty())
                {
                    if (player_node->getParty())
                    {
                        if (player_node->getParty()->getName()
                            != being->getPartyName())
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@party|%s@@", _("Invite to party")));
                        }
                        else
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@kick party|%s@@", _("Kick from party")));
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
                            mBrowserBox->addRow(strprintf(
                                "@@guild-kick|%s@@", _("Kick from guild")));
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
                        mBrowserBox->addRow(strprintf(
                            "@@guild-kick|%s@@", _("Kick from guild")));
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
                            mBrowserBox->addRow(strprintf(
                                "@@guild|%s@@", _("Invite to guild")));
                        }
                    }
                }

                if (player_node->isGM())
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow(strprintf(
                        "@@admin-kick|%s@@", _("Kick player")));
                }
                mBrowserBox->addRow(strprintf("@@nuke|%s@@", _("Nuke")));
                mBrowserBox->addRow(strprintf("@@move|%s@@", _("Move")));
                mBrowserBox->addRow(strprintf("@@items|%s@@",
                    _("Show Items")));
                mBrowserBox->addRow(strprintf("@@undress|%s@@", _("Undress")));
                mBrowserBox->addRow(strprintf(
                    "@@addcomment|%s@@", _("Add comment")));

                if (player_relations.getDefault() & PlayerRelation::TRADE)
                {
                    mBrowserBox->addRow("##3---");
                    if (being->isAdvanced())
                    {
                        if (being->isShopEnabled())
                        {
                            mBrowserBox->addRow(strprintf(
                                "@@buy|%s@@", _("Buy")));
                            mBrowserBox->addRow(strprintf(
                                "@@sell|%s@@", _("Sell")));
                        }
                    }
                    else
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@buy|%s@@", _("Buy (?)")));
                        mBrowserBox->addRow(strprintf(
                            "@@sell|%s@@", _("Sell (?)")));
                    }
                }
            }
            break;

        case ActorSprite::NPC:
            // NPCs can be talked to (single option, candidate for removal
            // unless more options would be added)
            mBrowserBox->addRow(strprintf("@@talk|%s@@", _("Talk")));

            mBrowserBox->addRow(strprintf("@@buy|%s@@", _("Buy")));
            mBrowserBox->addRow(strprintf("@@sell|%s@@", _("Sell")));
            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow(strprintf("@@move|%s@@", _("Move")));
            mBrowserBox->addRow(strprintf("@@addcomment|%s@@",
                _("Add comment")));
            break;

        case ActorSprite::MONSTER:
            {
                // Monsters can be attacked
                mBrowserBox->addRow(strprintf("@@attack|%s@@", _("Attack")));

                if (player_node->isGM())
                {
                    mBrowserBox->addRow("##3---");
                    mBrowserBox->addRow(strprintf(
                        "@@admin-kick|%s@@", _("Kick")));
                }

                if (config.getBoolValue("enableAttackFilter"))
                {
                    mBrowserBox->addRow("##3---");
                    if (actorSpriteManager->isInAttackList(name)
                        || actorSpriteManager->isInIgnoreAttackList(name)
                        || actorSpriteManager->isInPriorityAttackList(name))
                    {
                        mBrowserBox->addRow(strprintf("@@remove attack|%s@@",
                            _("Remove from attack list")));
                    }
                    else
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@add attack priority|%s@@",
                            _("Add to priority attack list")));
                        mBrowserBox->addRow(strprintf(
                            "@@add attack|%s@@", _("Add to attack list")));
                        mBrowserBox->addRow(strprintf(
                            "@@add attack ignore|%s@@",
                            _("Add to ignore list")));
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
    mBrowserBox->addRow(strprintf("@@name|%s@@", _("Add name to chat")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, std::vector<Being*> &beings)
{
    mBrowserBox->clearRows();
    mBrowserBox->addRow("Players");
    std::vector<Being*>::const_iterator it, it_end;
    for (it = beings.begin(), it_end = beings.end(); it != it_end; ++it)
    {
        Being *being = *it;
        if (!being->getName().empty())
        {
            mBrowserBox->addRow(strprintf("@@player_%u|%s >@@",
                being->getId(), (being->getName()
                + being->getGenderSignWithSpace()).c_str()));
        }
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));
    showPopup(x, y);
}

void PopupMenu::showPlayerPopup(int x, int y, std::string nick)
{
    if (nick.empty() || !player_node)
        return;

    mNick = nick;
    mBeingId = 0;
    mType = Being::PLAYER;
    mBrowserBox->clearRows();

    const std::string &name = mNick;

    mBrowserBox->addRow(name);

    mBrowserBox->addRow(strprintf("@@whisper|%s@@", _("Whisper")));
    mBrowserBox->addRow("##3---");

    switch (player_relations.getRelation(name))
    {
        case PlayerRelation::NEUTRAL:
            mBrowserBox->addRow(strprintf("@@friend|%s@@", _("Be friend")));
            mBrowserBox->addRow(strprintf(
                "@@disregard|%s@@", _("Disregard")));
            mBrowserBox->addRow(strprintf("@@ignore|%s@@", _("Ignore")));
            mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
            break;

        case PlayerRelation::FRIEND:
            mBrowserBox->addRow(strprintf("@@disregard|%s@@", _("Disregard")));
            mBrowserBox->addRow(strprintf("@@ignore|%s@@", _("Ignore")));
            mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
            break;

        case PlayerRelation::DISREGARDED:
            mBrowserBox->addRow(strprintf("@@unignore|%s@@", _("Unignore")));
            mBrowserBox->addRow(strprintf(
                "@@ignore|%s@@", _("Completely ignore")));
            mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
            break;

        case PlayerRelation::IGNORED:
            mBrowserBox->addRow(strprintf("@@unignore|%s@@", _("Unignore")));
            mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
            break;

        case PlayerRelation::ERASED:
            mBrowserBox->addRow(strprintf("@@unignore|%s@@", _("Unignore")));
            mBrowserBox->addRow(strprintf("@@disregard|%s@@", _("Disregard")));
            mBrowserBox->addRow(strprintf(
                "@@ignore|%s@@", _("Completely ignore")));
            break;

        default:
            break;
    }

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@follow|%s@@", _("Follow")));
    mBrowserBox->addRow(strprintf("@@imitation|%s@@", _("Imitation")));
    mBrowserBox->addRow(strprintf("@@addcomment|%s@@", _("Add comment")));

    if (player_node->isInParty() && player_node->getParty())
    {
        PartyMember *member = player_node->getParty()->getMember(mNick);
        if (member)
        {
            mBrowserBox->addRow(strprintf(
                "@@kick party|%s@@", _("Kick from party")));
            mBrowserBox->addRow("##3---");
        }
    }

    Guild *guild2 = player_node->getGuild();
    if (guild2)
    {
        if (guild2->getMember(mNick))
        {
            if (guild2->getServerGuild() || (guildManager
                && guildManager->havePower()))
            {
                mBrowserBox->addRow(strprintf(
                    "@@guild-kick|%s@@", _("Kick from guild")));
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
                mBrowserBox->addRow(strprintf(
                    "@@guild|%s@@", _("Invite to guild")));
            }
        }
    }

    mBrowserBox->addRow("##3---");
    if (player_relations.getDefault() & PlayerRelation::TRADE)
    {
        mBrowserBox->addRow(strprintf("@@buy|%s@@", _("Buy (?)")));
        mBrowserBox->addRow(strprintf("@@sell|%s@@", _("Sell (?)")));
    }

    mBrowserBox->addRow(strprintf("@@name|%s@@", _("Add name to chat")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);

}

void PopupMenu::showPopup(int x, int y, FloorItem *floorItem)
{
    if (!floorItem)
        return;

    mFloorItem = floorItem;
    const ItemInfo &info = floorItem->getInfo();
    mBrowserBox->clearRows();
    std::string name;

    // Floor item can be picked up (single option, candidate for removal)
    if (serverVersion > 0)
        name = info.getName(floorItem->getColor());
    else
        name = info.getName();

    mBrowserBox->addRow(name);
    mBrowserBox->addRow(strprintf("@@pickup|%s@@", _("Pick up")));
    mBrowserBox->addRow(strprintf("@@chat|%s@@", _("Add to chat")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showPopup(int x, int y, MapItem *mapItem)
{
    if (!mapItem)
        return;

    mMapItem = mapItem;

    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Map Item"));
    mBrowserBox->addRow(strprintf("@@rename map|%s@@", _("Rename")));
    mBrowserBox->addRow(strprintf("@@remove map|%s@@", _("Remove")));

    if (player_node && player_node->isGM())
    {
        mBrowserBox->addRow("##3---");
        mBrowserBox->addRow(strprintf("@@warp map|%s@@", _("Warp")));
    }
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showOutfitsPopup(int x, int y)
{
    mBrowserBox->clearRows();

    mBrowserBox->addRow(_("Outfits"));
    mBrowserBox->addRow(strprintf(
        "@@load old outfits|%s@@", _("Load old outfits")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showSpellPopup(int x, int y, TextCommand *cmd)
{
    if (!cmd)
        return;

    mBrowserBox->clearRows();

    mSpell = cmd;
    mBrowserBox->addRow(_("Spells"));
    mBrowserBox->addRow(strprintf(
        "@@load old spells|%s@@", _("Load old spells")));
    mBrowserBox->addRow(strprintf("@@edit spell|%s@@", _("Edit spell")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showChatPopup(int x, int y, ChatTab *tab)
{
    if (!tab || !actorSpriteManager || !player_node)
        return;

    mTab = tab;

    mBrowserBox->clearRows();

    if (tab->getType() == ChatTab::TAB_WHISPER)
        mBrowserBox->addRow(strprintf("@@chat close|%s@@", _("Close")));

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
                            _("Don't remove name")));
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
        mBrowserBox->addRow(strprintf("@@leave party|%s@@", _("Leave")));
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
            mType = being->getType();

            mBrowserBox->addRow(strprintf("@@trade|%s@@", _("Trade")));
            mBrowserBox->addRow(strprintf("@@attack|%s@@", _("Attack")));

            mBrowserBox->addRow("##3---");

            mBrowserBox->addRow(strprintf("@@heal|%s@@", _("Heal")));
            mBrowserBox->addRow("##3---");

            switch (player_relations.getRelation(name))
            {
                case PlayerRelation::NEUTRAL:
                    mBrowserBox->addRow(strprintf(
                        "@@friend|%s@@", _("Be friend")));
                    mBrowserBox->addRow(strprintf(
                        "@@disregard|%s@@", _("Disregard")));
                    mBrowserBox->addRow(strprintf(
                        "@@ignore|%s@@", _("Ignore")));
                    mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
                    break;

                case PlayerRelation::FRIEND:
                    mBrowserBox->addRow(strprintf(
                        "@@disregard|%s@@", _("Disregard")));
                    mBrowserBox->addRow(strprintf(
                        "@@ignore|%s@@", _("Ignore")));
                    mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
                    break;

                case PlayerRelation::DISREGARDED:
                    mBrowserBox->addRow(strprintf(
                        "@@unignore|%s@@", _("Unignore")));
                    mBrowserBox->addRow(strprintf(
                        "@@ignore|%s@@", _("Completely ignore")));
                    mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
                    break;

                case PlayerRelation::IGNORED:
                    mBrowserBox->addRow(strprintf(
                        "@@unignore|%s@@", _("Unignore")));
                    mBrowserBox->addRow(strprintf("@@erase|%s@@", _("Erase")));
                    break;

                case PlayerRelation::ERASED:
                    mBrowserBox->addRow(strprintf(
                        "@@unignore|%s@@", _("Unignore")));
                    mBrowserBox->addRow(strprintf(
                        "@@disregard|%s@@", _("Disregard")));
                    mBrowserBox->addRow(strprintf(
                        "@@ignore|%s@@", _("Completely ignore")));
                    break;

                default:
                    break;
            }

            mBrowserBox->addRow("##3---");
            mBrowserBox->addRow(strprintf("@@follow|%s@@", _("Follow")));
            mBrowserBox->addRow(strprintf("@@imitation|%s@@", _("Imitation")));
            mBrowserBox->addRow(strprintf("@@move|%s@@", _("Move")));
            mBrowserBox->addRow(strprintf("@@items|%s@@", _("Show Items")));
            mBrowserBox->addRow(strprintf("@@undress|%s@@", _("Undress")));
            mBrowserBox->addRow(strprintf(
                "@@addcomment|%s@@", _("Add comment")));

            if (player_relations.getDefault() & PlayerRelation::TRADE)
            {
                mBrowserBox->addRow("##3---");
                if (being->isAdvanced())
                {
                    if (being->isShopEnabled())
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@buy|%s@@", _("Buy")));
                        mBrowserBox->addRow(strprintf(
                            "@@sell|%s@@", _("Sell")));
                    }
                }
                else
                {
                    mBrowserBox->addRow(strprintf("@@buy|%s@@", _("Buy (?)")));
                    mBrowserBox->addRow(strprintf(
                        "@@sell|%s@@", _("Sell (?)")));
                }
            }

            mBrowserBox->addRow("##3---");

            if (player_node->isInParty())
            {
                if (player_node->getParty())
                {
                    if (!player_node->getParty()->isMember(wTab->getNick()))
                    {
                        mBrowserBox->addRow(
                            strprintf("@@party|%s@@", _("Invite to party")));
                    }
                    else
                    {
                        mBrowserBox->addRow(strprintf(
                            "@@kick party|%s@@", _("Kick from party")));
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
                        mBrowserBox->addRow(strprintf(
                            "@@guild|%s@@", _("Invite to guild")));
                    }
                }
            }
        }
        else
        {
            mNick = name;
            mType = Being::PLAYER;
            mBrowserBox->addRow(strprintf(
                "@@addcomment|%s@@", _("Add comment")));
            mBrowserBox->addRow("##3---");
        }
    }
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

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
        PositionsMap::const_iterator itr = map.begin();
        PositionsMap::const_iterator itr_end = map.end();
        for (; itr != itr_end; ++itr)
        {
            mBrowserBox->addRow(strprintf("@@guild-pos-%d|%s@@",
                itr->first, itr->second.c_str()));
        }
        mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

        showPopup(x, y);
    }
    else
    {
        mBeingId = 0;
        mFloorItem = 0;
        mItem = 0;
        mMapItem = 0;
        mNick = "";
        mType = Being::UNKNOWN;
        setVisible(false);
    }
}

void PopupMenu::handleLink(const std::string &link,
                           gcn::MouseEvent *event A_UNUSED)
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
                Item *item = inv->findItem(mItemId, mItemColor);
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
            tradeWindow->tradeItem(mItem, cnt, true);
        }
    }
    else if (link == "addtrade half" && mItem)
    {
        if (tradeWindow)
            tradeWindow->tradeItem(mItem, mItem->getQuantity() / 2, true);
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
    else if (link == "warp map" && mMapItem)
    {
        if (Game::instance())
        {
            Net::getAdminHandler()->warp(Game::instance()->getCurrentMapName(),
                mMapItem->getX(), mMapItem->getY());
        }
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
                    bool isHome = (mMapItem->getType() == MapItem::HOME);
                    const int x = mMapItem->getX();
                    const int y = mMapItem->getY();
                    specialLayer->setTile(x, y, MapItem::EMPTY);
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
    else if (link == "addcomment" && !mNick.empty())
    {
        // TRANSLATORS: number of chars in string should be near original
        TextDialog *dialog = new TextDialog(_("Player comment            "),
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
            const Guild *guild = player_node->getGuild();
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
    else if (link == "attack moveup")
    {
        if (actorSpriteManager)
        {
            int idx = actorSpriteManager->getAttackMobIndex(mNick);
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
            int idx = actorSpriteManager->getPriorityAttackMobIndex(mNick);
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
            int idx = actorSpriteManager->getAttackMobIndex(mNick);
            int size = actorSpriteManager->getAttackMobsSize();
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
            int idx = actorSpriteManager->getPriorityAttackMobIndex(mNick);
            int size = actorSpriteManager->getPriorityAttackMobsSize();
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
    else if (!link.compare(0, 10, "guild-pos-"))
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
    mItemColor = 1;
    mMapItem = 0;
    mNick = "";
    mType = Being::UNKNOWN;
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
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
            mBrowserBox->addRow(strprintf("@@drop all|%s@@", _("Drop all")));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));
        }

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

void PopupMenu::showItemPopup(int x, int y, int itemId, unsigned char color)
{
    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    Item *item = inv->findItem(itemId, color);
    if (item)
    {
        showItemPopup(x, y, item);
    }
    else
    {
        mItem = 0;
        mItemId = itemId;
        mItemColor = color;
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
    {
        mItemId = item->getId();
        mItemColor = item->getColor();
    }
    else
    {
        mItemId = 0;
        mItemColor = 1;
    }
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
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
            mBrowserBox->addRow(strprintf("@@drop all|%s@@", _("Drop all")));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));
        }

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
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop...")));
            mBrowserBox->addRow(strprintf("@@drop all|%s@@", _("Drop all")));
        }
        else
        {
            mBrowserBox->addRow(strprintf("@@drop|%s@@", _("Drop")));
        }

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
    std::vector <gcn::Button*> names = windowMenu->getButtons();
    std::vector <gcn::Button*>::const_iterator it, it_end;
    for (it = names.begin(), it_end = names.end(); it != it_end; ++ it)
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

    mNick = b->text();

    mBrowserBox->clearRows();
    std::vector <ProgressBar*> bars = miniStatusWindow->getBars();
    std::vector <ProgressBar*>::const_iterator it, it_end;
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
    mBrowserBox->addRow(strprintf("@@reset yellow|%s@@",
        _("Reset yellow bar")));
    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@bar to chat|%s@@", _("Copy to chat")));
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showAttackMonsterPopup(int x, int y, std::string name,
                                       int type)
{
    if (!player_node)
        return;

    mNick = name;
    mType = Being::MONSTER;

    mBrowserBox->clearRows();

    if (name.empty())
        mBrowserBox->addRow(_("(default)"));
    else
        mBrowserBox->addRow(name);
    switch (type)
    {
        case MapItem::ATTACK:
        {
            int idx = actorSpriteManager->getAttackMobIndex(name);
            int size = actorSpriteManager->getAttackMobsSize();
            if (idx > 0)
            {
                mBrowserBox->addRow(strprintf(
                    "@@attack moveup|%s@@", _("Move up")));
            }
            if (idx + 1 < size)
            {
                mBrowserBox->addRow(strprintf(
                    "@@attack movedown|%s@@", _("Move down")));
            }
            mBrowserBox->addRow(strprintf(
                "@@attack remove|%s@@", _("Remove")));
            break;
        }
        case MapItem::PRIORITY:
        {
            int idx = actorSpriteManager->getPriorityAttackMobIndex(name);
            int size = actorSpriteManager->getPriorityAttackMobsSize();
            if (idx > 0)
            {
                mBrowserBox->addRow(strprintf(
                    "@@priority moveup|%s@@", _("Move up")));
            }
            if (idx + 1 < size)
            {
                mBrowserBox->addRow(strprintf(
                    "@@priority movedown|%s@@", _("Move down")));
            }
            mBrowserBox->addRow(strprintf(
                "@@attack remove|%s@@", _("Remove")));
            break;
        }
        case MapItem::IGNORE_:
            mBrowserBox->addRow(strprintf(
                "@@attack remove|%s@@", _("Remove")));
            break;
        default:
            break;
    }

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);
}

void PopupMenu::showUndressPopup(int x, int y, Being *being, Item *item)
{
    if (!being || !item)
        return;

    mBeingId = being->getId();
    mItem = item;
    mItemId = item->getId();
    mItemColor = item->getColor();

    mBrowserBox->clearRows();

    mBrowserBox->addRow(strprintf("@@undress item|%s@@", _("Undress")));

    mBrowserBox->addRow("##3---");
    mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));

    showPopup(x, y);

}

void PopupMenu::showPopup(int x, int y)
{
    setContentSize(mBrowserBox->getWidth() + 8, mBrowserBox->getHeight() + 8);
    if (mainGraphics->mWidth < (x + getWidth() + 5))
        x = mainGraphics->mWidth - getWidth();
    if (mainGraphics->mHeight < (y + getHeight() + 5))
        y = mainGraphics->mHeight - getHeight();
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

PlayerListener::PlayerListener() :
    mNick(""),
    mDialog(0),
    mType(Being::UNKNOWN)
{
}

void PlayerListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && !mNick.empty() && mDialog)
    {
        std::string comment = mDialog->getText();
        Being* being  = actorSpriteManager->findBeingByName(
            mNick, (ActorSprite::Type)mType);
        if (being)
            being->setComment(comment);
        Being::saveComment(mNick, comment, mType);
    }
    mDialog = 0;
}
