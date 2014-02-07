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

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "commands.h"

#include <list>
#include <string>

#include "localconsts.h"

class ChatTab;

extern ChatTab *localChatTab;

typedef std::map<std::string, const CommandInfo*> CommandsMap;
typedef CommandsMap::const_iterator CommandsMapIter;

/**
 * A class to parse and handle user commands
 */
class CommandHandler final
{
    public:
        /**
         * Constructor
         */
        CommandHandler();

        A_DELETE_COPY(CommandHandler)

        /**
         * Destructor
         */
        ~CommandHandler()
        { }

        static void addChatCommands(std::list<std::string> &arr);

        /**
         * Parse and handle the given command.
         */
        void handleCommand(const std::string &command,
                           ChatTab *const tab = localChatTab) const;

        void handleCommands(const std::string &command,
                            ChatTab *const tab = localChatTab);

        void invokeCommand(const std::string &type,
                           const std::string &args,
                           ChatTab *const tab,
                           const bool warn = false) const;

        static void invokeCommand(const int type);

        static void invokeCommand(const int type,
                                  const std::string &args,
                                  ChatTab *const tab);

        static void invokeCommand(const int type,
                                  const std::string &args);

        static void invokeCommand(const int type,
                                  ChatTab *const tab);

    protected:
        friend class ChatTab;
        friend class WhisperTab;
        CommandsMap mCommands;

    private:
        static void callFunc(const CommandInfo &info,
                             const std::string &args,
                             ChatTab *const tab);
};

extern CommandHandler *commandHandler;

#endif  // COMMANDHANDLER_H
