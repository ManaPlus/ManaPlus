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

#include "commandhandler.h"

#include "input/inputmanager.h"

#include "gui/widgets/tabs/chattab.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

CommandHandler::CommandHandler() :
    mCommands()
{
    const int sz = sizeof(commands) / sizeof(CommandInfo);
    for (int f = 0; f < sz; f ++)
    {
        const std::string name = commands[f].name;
        if (!name.empty())
            mCommands[name] = &commands[f];
    }
}

void CommandHandler::addChatCommands(std::list<std::string> &arr)
{
    const int sz = sizeof(commands) / sizeof(CommandInfo);
    for (int f = 0; f < sz; f ++)
    {
        const CommandInfo &info = commands[f];
        const std::string name = info.name;
        if (!name.empty())
        {
            std::string cmd = std::string("/").append(name);
            if (info.useArgs)
                cmd.append(" ");
            arr.push_back(cmd);
        }
    }
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
    invokeCommand(type, args, tab, true);
}

void CommandHandler::invokeCommand(const std::string &type,
                                   const std::string &args,
                                   ChatTab *const tab,
                                   const bool warn) const
{
    const CommandsMapIter it = mCommands.find(type);
    if (it != mCommands.end())
    {
        callFunc(*(*it).second, args, tab);
    }
    else if (!tab->handleCommand(type, args))
    {
        if (warn)
        {
            // TRANSLATORS: chat commands handling message
            tab->chatLog(_("Unknown command."));
        }
    }
}

void CommandHandler::callFunc(const CommandInfo &info,
                              const std::string &args,
                              ChatTab *const tab)
{
    const CommandFuncPtr func = info.func;
    if (func)
        func(args, tab);
    else
        inputManager.executeAction(info.actionId);
}

void CommandHandler::invokeCommand(const int type)
{
    if (type < 0 || type >= END_COMMANDS)
        return;
    callFunc(commands[type], "", nullptr);
}

void CommandHandler::invokeCommand(const int type,
                                   ChatTab *const tab)
{
    if (type < 0 || type >= END_COMMANDS)
        return;
    callFunc(commands[type], "", tab);
}

void CommandHandler::invokeCommand(const int type,
                                   const std::string &args)
{
    if (type < 0 || type >= END_COMMANDS)
        return;
    callFunc(commands[type], args, nullptr);
}

void CommandHandler::invokeCommand(const int type,
                                   const std::string &args,
                                   ChatTab *const tab)
{
    if (type < 0 || type >= END_COMMANDS)
        return;
    callFunc(commands[type], args, tab);
}
