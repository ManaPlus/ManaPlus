/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "input/keydata.h"

#include <string>

class ChatTab;

extern ChatTab *localChatTab;

// TRANSLATORS: chat option changed message
#define BOOLEAN_OPTIONS _("Options to /%s are \"yes\", \"no\", \"true\", "\
"\"false\", \"1\", \"0\".")

#define decHandler(name) void name(const std::string &args, ChatTab *const tab)

typedef void (*CommandFuncPtr) (const std::string &args, ChatTab *const tab);

struct CommandInfo
{
    const char *name;
    CommandFuncPtr func;
    int actionId;
    bool useArgs;
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
    decHandler(emotePet);
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
    decHandler(dumpGL);
    decHandler(dumpMods);
    decHandler(cacheInfo);
    decHandler(execute);
    decHandler(testsdlfont);
    decHandler(enableHighlight);
    decHandler(disableHighlight);
    decHandler(dontRemoveName);
    decHandler(removeName);
    decHandler(disableAway);
    decHandler(enableAway);
    decHandler(testParticle);
    decHandler(createItems);
    decHandler(talkRaw);
    decHandler(talkPet);
    decHandler(uploadConfig);
    decHandler(uploadServerConfig);
    decHandler(uploadLog);

    void replaceVars(std::string &str);
}  // namespace Commands

enum
{
    COMMAND_CLOSEALL = 0,
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
    COMMAND_EMOTEPET,
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
    COMMAND_DUMPGL,
    COMMAND_DUMPMODS,
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
    COMMAND_TEST_PARTICLE,
    COMMAND_CREATEITEMS,
    COMMAND_TALKRAW,
    COMMAND_TALKPET,
    COMMAND_UPLOADCONFIG,
    COMMAND_UPLOADSERVERCONFIG,
    COMMAND_UPLOADLOG,
    COMMAND_HACK,
    END_COMMANDS
};

static const CommandInfo commands[] =
{
    {"closeall", &Commands::closeAll, -1, false},
    {"ignoreall", &Commands::ignoreAll, -1, false},
    {"help", &Commands::help, -1, false},
    {"announce", &Commands::announce, -1, true},
    {"where", &Commands::where, -1, false},
    {"who", &Commands::who, -1, false},
    {"msg", Commands::msg, -1, true},
    {"whisper", Commands::msg, -1, true},
    {"w", &Commands::msg, -1, true},
    {"query", Commands::query, -1, true},
    {"q", &Commands::query, -1, true},
    {"ignore", &Commands::ignore, -1, true},
    {"unignore", &Commands::unignore, -1, true},
    {"friend", Commands::beFriend, -1, true},
    {"befriend", &Commands::beFriend, -1, true},
    {"disregard", &Commands::disregard, -1, true},
    {"neutral", &Commands::neutral, -1, true},
    {"blacklist", &Commands::blackList, -1, true},
    {"enemy", &Commands::enemy, -1, true},
    {"erase", &Commands::erase, -1, true},
    {"clear", &Commands::clear, -1, true},
    {"cleangraphics", &Commands::cleanGraphics, -1, false},
    {"cleanfonts", &Commands::cleanFonts, -1, false},
    {"createparty", &Commands::createParty, -1, true},
    {"createguild", &Commands::createGuild, -1, true},
    {"party", &Commands::party, -1, true},
    {"me", &Commands::me, -1, true},
    {"toggle", &Commands::toggle, -1, false},
    {"present", &Commands::present, -1, false},
    {"quit", &Commands::quit, -1, false},
    {"all", &Commands::showAll, -1, false},
    {"move", &Commands::move, -1, true},
    {"target", &Commands::target, -1, true},
    {"atkhuman", &Commands::attackHuman, -1, true},
    {"outfit", &Commands::outfit, -1, true},
    {"emote", &Commands::emote, -1, true},
    {"emotepet", &Commands::emotePet, -1, true},
    {"away", &Commands::away, -1, true},
    {"pseudoaway", &Commands::pseudoAway, -1, true},
    {"follow", &Commands::follow, -1, true},
    {"heal", &Commands::heal, -1, true},
    {"navigate", &Commands::navigate, -1, true},
    {"imitation", &Commands::imitation, -1, true},
    {"mail", &Commands::mail, -1, true},
    {"trade", &Commands::trade, -1, true},
    {"priceload", &Commands::priceLoad, -1, false},
    {"pricesave", &Commands::priceSave, -1, false},
    {"cacheinfo", &Commands::cacheInfo, -1, false},
    {"disconnect", &Commands::disconnect, -1, false},
    {"undress", &Commands::undress, -1, true},
    {"attack", &Commands::attack, -1, true},
    {"dirs", &Commands::dirs, -1, false},
    {"info", &Commands::info, -1, false},
    {"wait", &Commands::wait, -1, true},
    {"uptime", &Commands::uptime, -1, false},
    {"addpriorityattack", &Commands::addPriorityAttack, -1, true},
    {"addattack", &Commands::addAttack, -1, true},
    {"removeattack", Commands::removeAttack, -1, true},
    {"removeignoreattack", &Commands::removeAttack, -1, true},
    {"addignoreattack", &Commands::addIgnoreAttack, -1, true},
    {"dump", &Commands::dump, -1, false},
    {"serverignoreall", &Commands::serverIgnoreAll, -1, false},
    {"serverunignoreall", &Commands::serverUnIgnoreAll, -1, false},
    {"setdrop", &Commands::setDrop, -1, true},
    {"error", &Commands::error, -1, false},
    {"dumpg", &Commands::dumpGraphics, -1, false},
    {"dumpe", &Commands::dumpEnvironment, -1, false},
    {"dumpt", &Commands::dumpTests, -1, false},
    {"dumpogl", &Commands::dumpOGL, -1, false},
    {"dumpgl", &Commands::dumpGL, -1, false},
    {"dumpmods", &Commands::dumpMods, -1, false},
    {"url", &Commands::url, -1, true},
    {"open", &Commands::open, -1, true},
    {"execute", &Commands::execute, -1, true},
    {"testsdlfont", &Commands::testsdlfont, -1, false},
    {"enablehighlight", &Commands::enableHighlight, -1, false},
    {"disablehighlight", &Commands::disableHighlight, -1, false},
    {"", &Commands::dontRemoveName, -1, false},
    {"", &Commands::removeName, -1, false},
    {"disableaway", &Commands::disableAway, -1, false},
    {"enableaway", &Commands::enableAway, -1, false},
    {"drop", nullptr, Input::KEY_QUICK_DROP, false},
    {"dropn", nullptr, Input::KEY_QUICK_DROPN, false},
    {"movetotarget", nullptr, Input::KEY_MOVE_TO_TARGET, false},
    {"movetohome", nullptr, Input::KEY_MOVE_TO_HOME, false},
    {"sethome", nullptr, Input::KEY_SET_HOME, false},
    {"magicattack", nullptr, Input::KEY_MAGIC_ATTACK, false},
    {"testparticle", &Commands::testParticle, -1, true},
    {"createitems", &Commands::createItems, -1, false},
    {"talkraw", &Commands::talkRaw, -1, true},
    {"talkpet", &Commands::talkPet, -1, true},
    {"uploadconfig", &Commands::uploadConfig, -1, false},
    {"uploadserverconfig", &Commands::uploadServerConfig, -1, false},
    {"uploadlog", &Commands::uploadLog, -1, false},
    {"hack", &Commands::hack, -1, true}
};

#undef decHandler

#endif  // COMMANDS_H
