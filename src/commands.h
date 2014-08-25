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

#include "actions/actionfuncptr.h"

#include "input/inputaction.h"

#include <string>

#include "localconsts.h"

class ChatTab;

extern ChatTab *localChatTab;

// TRANSLATORS: chat option changed message
#define BOOLEAN_OPTIONS _("Options to /%s are \"yes\", \"no\", \"true\", "\
"\"false\", \"1\", \"0\".")

#define decHandler(name) bool name(InputEvent &event)

struct CommandInfo final
{
    const char *name;
    ActionFuncPtr func;
    int actionId;
    bool useArgs;
};

namespace Commands
{
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
    decHandler(error) __attribute__ ((noreturn));
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
    decHandler(gm);
    decHandler(debugSpawn);

    void replaceVars(std::string &str);
}  // namespace Commands

enum
{
    COMMAND_AWAY = 0,
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
    COMMAND_TEST_PARTICLE,
    COMMAND_CREATEITEMS,
    COMMAND_TALKRAW,
    COMMAND_TALKPET,
    COMMAND_UPLOADCONFIG,
    COMMAND_UPLOADSERVERCONFIG,
    COMMAND_UPLOADLOG,
    COMMAND_GM,
    COMMAND_HACK,
    COMMAND_DEBUGSPAWN,
    END_COMMANDS
};

static const CommandInfo commands[] =
{
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
    {"testparticle", &Commands::testParticle, -1, true},
    {"createitems", &Commands::createItems, -1, false},
    {"talkraw", &Commands::talkRaw, -1, true},
    {"talkpet", &Commands::talkPet, -1, true},
    {"uploadconfig", &Commands::uploadConfig, -1, false},
    {"uploadserverconfig", &Commands::uploadServerConfig, -1, false},
    {"uploadlog", &Commands::uploadLog, -1, false},
    {"gm", &Commands::gm, -1, true},
    {"hack", &Commands::hack, -1, true},
    {"debugSpawn", &Commands::debugSpawn, -1, true}
};

#undef decHandler

#endif  // COMMANDS_H
