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
    decHandler(join);
    decHandler(listChannels);
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

    void replaceVars(std::string &str);
}

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
    {"join", &Commands::join},
    {"list", &Commands::listChannels},
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
    {"hack", &Commands::hack}
};

#undef decHandler

#endif // COMMANDS_H
