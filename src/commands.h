/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "playerrelations.h"
#include "keydata.h"

#include <string>

class ChatTab;

extern ChatTab *localChatTab;

#define BOOLEAN_OPTIONS _("Options to /%s are \"yes\", \"no\", \"true\", "\
"\"false\", \"1\", \"0\".")

#define decHandler(name) void name(const std::string &args, ChatTab *const tab)

typedef void (*CommandFuncPtr) (const std::string &args, ChatTab *const tab);

struct CommandInfo
{
    const char *name;
    CommandFuncPtr func;
    int actionId;
};

namespace Commands
{
    decHandler(announce);
    decHandler(help);
    decHandler(where);
    decHandler(who);
    decHandler(msg);
    decHandler(query);
    decHandler(clear);
    decHandler(cleanGraphics);
    decHandler(cleanFonts);
    decHandler(createParty);
    decHandler(createGuild);
    decHandler(party);
    decHandler(me);
    decHandler(toggle);
    decHandler(present);
    decHandler(ignore);
    decHandler(unignore);
    decHandler(beFriend);
    decHandler(disregard);
    decHandler(neutral);
    decHandler(blackList);
    decHandler(enemy);
    decHandler(erase);
    decHandler(quit);
    decHandler(showAll);
    decHandler(move);
    decHandler(target);
    decHandler(attackHuman);
    decHandler(closeAll);
    decHandler(ignoreAll);
    decHandler(outfit);
    decHandler(emote);
    decHandler(away);
    decHandler(pseudoAway);
    decHandler(follow);
    decHandler(imitation);
    decHandler(heal);
    decHandler(navigate);
    decHandler(mail);
    decHandler(hack);
    decHandler(priceLoad);
    decHandler(priceSave);
    decHandler(trade);
    decHandler(disconnect);
    decHandler(undress);
    decHandler(attack);
    decHandler(dirs);
    decHandler(info);
    decHandler(wait);
    decHandler(uptime);
    decHandler(addAttack);
    decHandler(addPriorityAttack);
    decHandler(removeAttack);
    decHandler(addIgnoreAttack);
    decHandler(serverIgnoreAll);
    decHandler(serverUnIgnoreAll);
    decHandler(setDrop);
    decHandler(error);
    decHandler(url);
    decHandler(open);
    decHandler(dump);
    decHandler(dumpGraphics);
    decHandler(dumpEnvironment);
    decHandler(dumpTests);
    decHandler(dumpOGL);
    decHandler(cacheInfo);
    decHandler(execute);
    decHandler(testsdlfont);
    decHandler(enableHighlight);
    decHandler(disableHighlight);
    decHandler(dontRemoveName);
    decHandler(removeName);
    decHandler(disableAway);
    decHandler(enableAway);

    void replaceVars(std::string &str);
}  // namespace Commands

enum
{
    COMMAND_CLOSEALL,
    COMMAND_IGNOREALL,
    COMMAND_HELP,
    COMMAND_ANNOUNCE,
    COMMAND_WHERE,
    COMMAND_WHO,
    COMMAND_MSG,
    COMMAND_WHISPER,
    COMMAND_W,
    COMMAND_QUERY,
    COMMAND_Q,
    COMMAND_IGNORE,
    COMMAND_UNIGNORE,
    COMMAND_FRIEND,
    COMMAND_BEFRIEND,
    COMMAND_DISREGARD,
    COMMAND_NEUTRAL,
    COMMAND_BLACKLIST,
    COMMAND_ENEMY,
    COMMAND_ERASE,
    COMMAND_CLEAR,
    COMMAND_CLEANGRAPHICS,
    COMMAND_CLEANFONTS,
    COMMAND_CREATEPARTY,
    COMMAND_CREATEGUILD,
    COMMAND_PARTY,
    COMMAND_ME,
    COMMAND_TOGGLE,
    COMMAND_PRESENT,
    COMMAND_QUIT,
    COMMAND_ALL,
    COMMAND_MOVE,
    COMMAND_TARGET,
    COMMAND_ATKHUMAN,
    COMMAND_OUTFIT,
    COMMAND_EMOTE,
    COMMAND_AWAY,
    COMMAND_PSEUDOAWAY,
    COMMAND_FOLLOW,
    COMMAND_HEAL,
    COMMAND_NAVIGATE,
    COMMAND_IMITATION,
    COMMAND_MAIL,
    COMMAND_TRADE,
    COMMAND_PRICELOAD,
    COMMAND_PRICESAVE,
    COMMAND_CACHEINFO,
    COMMAND_DISCONNECT,
    COMMAND_UNDRESS,
    COMMAND_ATTACK,
    COMMAND_DIRS,
    COMMAND_INFO,
    COMMAND_WAIT,
    COMMAND_UPTIME,
    COMMAND_ADDPRIORITYATTACK,
    COMMAND_ADDATTACK,
    COMMAND_REMOVEATTACK,
    COMMAND_REMOVEIGNOREATTACK,
    COMMAND_ADDIGNOREATTACK,
    COMMAND_DUMP,
    COMMAND_SERVERIGNOREALL,
    COMMAND_SERVERUNIGNOREALL,
    COMMAND_SETDROP,
    COMMAND_ERROR,
    COMMAND_DUMPG,
    COMMAND_DUMPE,
    COMMAND_DUMPT,
    COMMAND_DUMPOGL,
    COMMAND_URL,
    COMMAND_OPEN,
    COMMAND_EXECUTE,
    COMMAND_TESTSDLFONT,
    COMMAND_ENABLEHIGHLIGHT,
    COMMAND_DISABLEHIGHLIGHT,
    COMMAND_DONTREMOVENAME,
    COMMAND_REMOVENAME,
    COMMAND_DISABLEAWAY,
    COMMAND_ENABLEAWAY,
    COMMAND_DROP,
    COMMAND_DROPN,
    COMMAND_MOVETOTARGET,
    COMMAND_MOVETOHOME,
    COMMAND_SETHOME,
    COMMAND_MAGICATTACK,
    COMMAND_HACK,
    END_COMMANDS,
};

static const CommandInfo commands[] =
{
    {"closeall", &Commands::closeAll, -1},
    {"ignoreall", &Commands::ignoreAll, -1},
    {"help", &Commands::help, -1},
    {"announce", &Commands::announce, -1},
    {"where", &Commands::where, -1},
    {"who", &Commands::who, -1},
    {"msg", Commands::msg, -1},
    {"whisper", Commands::msg, -1},
    {"w", &Commands::msg, -1},
    {"query", Commands::query, -1},
    {"q", &Commands::query, -1},
    {"ignore", &Commands::ignore, -1},
    {"unignore", &Commands::unignore, -1},
    {"friend", Commands::beFriend, -1},
    {"befriend", &Commands::beFriend, -1},
    {"disregard", &Commands::disregard, -1},
    {"neutral", &Commands::neutral, -1},
    {"blacklist", &Commands::blackList, -1},
    {"enemy", &Commands::enemy, -1},
    {"erase", &Commands::erase, -1},
    {"clear", &Commands::clear, -1},
    {"cleangraphics", &Commands::cleanGraphics, -1},
    {"cleanfonts", &Commands::cleanFonts, -1},
    {"createparty", &Commands::createParty, -1},
    {"createguild", &Commands::createGuild, -1},
    {"party", &Commands::party, -1},
    {"me", &Commands::me, -1},
    {"toggle", &Commands::toggle, -1},
    {"present", &Commands::present, -1},
    {"quit", &Commands::quit, -1},
    {"all", &Commands::showAll, -1},
    {"move", &Commands::move, -1},
    {"target", &Commands::target, -1},
    {"atkhuman", &Commands::attackHuman, -1},
    {"outfit", &Commands::outfit, -1},
    {"emote", &Commands::emote, -1},
    {"away", &Commands::away, -1},
    {"pseudoaway", &Commands::pseudoAway, -1},
    {"follow", &Commands::follow, -1},
    {"heal", &Commands::heal, -1},
    {"navigate", &Commands::navigate, -1},
    {"imitation", &Commands::imitation, -1},
    {"mail", &Commands::mail, -1},
    {"trade", &Commands::trade, -1},
    {"priceload", &Commands::priceLoad, -1},
    {"pricesave", &Commands::priceSave, -1},
    {"cacheinfo", &Commands::cacheInfo, -1},
    {"disconnect", &Commands::disconnect, -1},
    {"undress", &Commands::undress, -1},
    {"attack", &Commands::attack, -1},
    {"dirs", &Commands::dirs, -1},
    {"info", &Commands::info, -1},
    {"wait", &Commands::wait, -1},
    {"uptime", &Commands::uptime, -1},
    {"addpriorityattack", &Commands::addPriorityAttack, -1},
    {"addattack", &Commands::addAttack, -1},
    {"removeattack", Commands::removeAttack, -1},
    {"removeignoreattack", &Commands::removeAttack, -1},
    {"addignoreattack", &Commands::addIgnoreAttack, -1},
    {"dump", &Commands::dump, -1},
    {"serverignoreall", &Commands::serverIgnoreAll, -1},
    {"serverunignoreall", &Commands::serverUnIgnoreAll, -1},
    {"setdrop", &Commands::setDrop, -1},
    {"error", &Commands::error, -1},
    {"dumpg", &Commands::dumpGraphics, -1},
    {"dumpe", &Commands::dumpEnvironment, -1},
    {"dumpt", &Commands::dumpTests, -1},
    {"dumpogl", &Commands::dumpOGL, -1},
    {"url", &Commands::url, -1},
    {"open", &Commands::open, -1},
    {"execute", &Commands::execute, -1},
    {"testsdlfont", &Commands::testsdlfont, -1},
    {"enablehighlight", &Commands::enableHighlight, -1},
    {"disablehighlight", &Commands::disableHighlight, -1},
    {"", &Commands::dontRemoveName, -1},
    {"", &Commands::removeName, -1},
    {"disableaway", &Commands::disableAway, -1},
    {"enableaway", &Commands::enableAway, -1},
    {"drop", nullptr, Input::KEY_QUICK_DROP},
    {"dropn", nullptr, Input::KEY_QUICK_DROPN},
    {"movetotarget", nullptr, Input::KEY_MOVE_TO_TARGET},
    {"movetohome", nullptr, Input::KEY_MOVE_TO_HOME},
    {"sethome", nullptr, Input::KEY_SET_HOME},
    {"magicattack", nullptr, Input::KEY_MAGIC_ATTACK},
    {"hack", &Commands::hack, -1}
};

#undef decHandler

#endif  // COMMANDS_H
