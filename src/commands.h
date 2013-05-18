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
    COMMAND_HACK,
    END_COMMANDS,
};

static const CommandInfo commands[] =
{
    {"closeall", &Commands::closeAll},
    {"ignoreall", &Commands::ignoreAll},
    {"help", &Commands::help},
    {"announce", &Commands::announce},
    {"where", &Commands::where},
    {"who", &Commands::who},
    {"msg", Commands::msg},
    {"whisper", Commands::msg},
    {"w", &Commands::msg},
    {"query", Commands::query},
    {"q", &Commands::query},
    {"ignore", &Commands::ignore},
    {"unignore", &Commands::unignore},
    {"friend", Commands::beFriend},
    {"befriend", &Commands::beFriend},
    {"disregard", &Commands::disregard},
    {"neutral", &Commands::neutral},
    {"blacklist", &Commands::blackList},
    {"enemy", &Commands::enemy},
    {"erase", &Commands::erase},
    {"clear", &Commands::clear},
    {"cleangraphics", &Commands::cleanGraphics},
    {"cleanfonts", &Commands::cleanFonts},
    {"createparty", &Commands::createParty},
    {"createguild", &Commands::createGuild},
    {"party", &Commands::party},
    {"me", &Commands::me},
    {"toggle", &Commands::toggle},
    {"present", &Commands::present},
    {"quit", &Commands::quit},
    {"all", &Commands::showAll},
    {"move", &Commands::move},
    {"target", &Commands::target},
    {"atkhuman", &Commands::attackHuman},
    {"outfit", &Commands::outfit},
    {"emote", &Commands::emote},
    {"away", &Commands::away},
    {"pseudoaway", &Commands::pseudoAway},
    {"follow", &Commands::follow},
    {"heal", &Commands::heal},
    {"navigate", &Commands::navigate},
    {"imitation", &Commands::imitation},
    {"mail", &Commands::mail},
    {"trade", &Commands::trade},
    {"priceload", &Commands::priceLoad},
    {"pricesave", &Commands::priceSave},
    {"cacheinfo", &Commands::cacheInfo},
    {"disconnect", &Commands::disconnect},
    {"undress", &Commands::undress},
    {"attack", &Commands::attack},
    {"dirs", &Commands::dirs},
    {"info", &Commands::info},
    {"wait", &Commands::wait},
    {"uptime", &Commands::uptime},
    {"addpriorityattack", &Commands::addPriorityAttack},
    {"addattack", &Commands::addAttack},
    {"removeattack", Commands::removeAttack},
    {"removeignoreattack", &Commands::removeAttack},
    {"addignoreattack", &Commands::addIgnoreAttack},
    {"dump", &Commands::dump},
    {"serverignoreall", &Commands::serverIgnoreAll},
    {"serverunignoreall", &Commands::serverUnIgnoreAll},
    {"setdrop", &Commands::setDrop},
    {"error", &Commands::error},
    {"dumpg", &Commands::dumpGraphics},
    {"dumpe", &Commands::dumpEnvironment},
    {"dumpt", &Commands::dumpTests},
    {"dumpogl", &Commands::dumpOGL},
    {"url", &Commands::url},
    {"open", &Commands::open},
    {"execute", &Commands::execute},
    {"testsdlfont", &Commands::testsdlfont},
    {"enablehighlight", &Commands::enableHighlight},
    {"disablehighlight", &Commands::disableHighlight},
    {"hack", &Commands::hack}
};

#undef decHandler

#endif  // COMMANDS_H
