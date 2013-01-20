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

#include "commandhandler.h"

#include "gui/widgets/chattab.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

CommandHandler::CommandHandler()
{
    const int sz = sizeof(commands) / sizeof(CommandInfo);
    for (int f = 0; f < sz; f ++)
        mCommands[commands[f].name] = commands[f].func;
}

void CommandHandler::handleCommands(const std::string &command,
                                    ChatTab *const tab)
{
    // here need add splitting commands
    handleCommand(command, tab);
}

void CommandHandler::handleCommand(const std::string &command,
                                   ChatTab *const tab)
{
    const size_t pos = command.find(' ');
    const std::string type(command, 0, pos);
    std::string args(command, pos == std::string::npos
                     ? command.size() : pos + 1);

    args = trim(args);
    const CommandsMapIter it = mCommands.find(type);
    if (it != mCommands.end())
        ((*it).second)(args, tab);
    else if (!tab->handleCommand(type, args))
        tab->chatLog(_("Unknown command."));
}
