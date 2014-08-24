/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "actions/commands.h"

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/whispertab.h"

#include "net/adminhandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Actions
{

static void changeRelation(std::string args,
                           const PlayerRelation::Relation relation,
                           const std::string &relationText,
                           ChatTab *const tab)
{
    if (!tab)
        return;

    if (args.empty())
    {
        WhisperTab *const whisper = dynamic_cast<WhisperTab* const>(tab);
        if (!whisper || whisper->getNick().empty())
        {
            // TRANSLATORS: change relation
            tab->chatLog(_("Please specify a name."), ChatMsgType::BY_SERVER);
            return;
        }
        args = whisper->getNick();
    }

    if (player_relations.getRelation(args) == relation)
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player already %s!"),
                     relationText.c_str()), ChatMsgType::BY_SERVER);
        return;
    }
    else
    {
        player_relations.setRelation(args, relation);
    }

    if (player_relations.getRelation(args) == relation)
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player successfully %s!"),
                     relationText.c_str()), ChatMsgType::BY_SERVER);
    }
    else
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player could not be %s!"),
                     relationText.c_str()), ChatMsgType::BY_SERVER);
    }
}

impHandler(chatAnnounce)
{
    Net::getAdminHandler()->announce(event.args);
    return true;
}

impHandler(chatIgnore)
{
    changeRelation(event.args, PlayerRelation::IGNORED, "ignored", event.tab);
    return true;
}

impHandler(chatUnignore)
{
    std::string args = event.args;
    if (args.empty())
    {
        WhisperTab *const whisper = dynamic_cast<WhisperTab* const>(event.tab);
        if (!whisper || whisper->getNick().empty())
        {
            // TRANSLATORS: change relation
            event.tab->chatLog(_("Please specify a name."), ChatMsgType::BY_SERVER);
            return false;
        }
        args = whisper->getNick();
    }

    const PlayerRelation::Relation rel = player_relations.getRelation(args);
    if (rel != PlayerRelation::NEUTRAL && rel != PlayerRelation::FRIEND)
    {
        player_relations.setRelation(args, PlayerRelation::NEUTRAL);
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(_("Player wasn't ignored!"), ChatMsgType::BY_SERVER);
        }
        return true;
    }

    if (event.tab)
    {
        if (player_relations.getRelation(args) == PlayerRelation::NEUTRAL)
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(_("Player no longer ignored!"),
                ChatMsgType::BY_SERVER);
        }
        else
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(_("Player could not be unignored!"),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(chatFriend)
{
    // TRANSLATORS: adding friend command
    changeRelation(event.args, PlayerRelation::FRIEND, _("friend"), event.tab);
    return true;
}

impHandler(chatDisregard)
{
    // TRANSLATORS: disregard command
    changeRelation(event.args, PlayerRelation::DISREGARDED, _("disregarded"), event.tab);
    return true;
}

impHandler(chatNeutral)
{
    // TRANSLATORS: neutral command
    changeRelation(event.args, PlayerRelation::NEUTRAL, _("neutral"), event.tab);
    return true;
}

impHandler(chatBlackList)
{
    // TRANSLATORS: blacklist command
    changeRelation(event.args, PlayerRelation::BLACKLISTED, _("blacklisted"), event.tab);
    return true;
}

impHandler(chatEnemy)
{
    // TRANSLATORS: enemy command
    changeRelation(event.args, PlayerRelation::ENEMY2, _("enemy"), event.tab);
    return true;
}

}  // namespace Actions
