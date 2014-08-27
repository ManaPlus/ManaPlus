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
    decHandler(hack);
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
    COMMAND_REMOVENAME = 0,
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
