/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "playerrelations.h"

#include <string>

class ChatTab;

extern ChatTab *localChatTab;

#define BOOLEAN_OPTIONS _("Options to /%s are \"yes\", \"no\", \"true\", "\
"\"false\", \"1\", \"0\".")

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

        /**
         * Parse and handle the given command.
         */
        static void handleCommand(const std::string &command,
                                  ChatTab *const tab = localChatTab);

        static void handleCommands(const std::string &command,
                                   ChatTab *const tab = localChatTab);

        static void replaceVars(std::string &str);

        static signed char parseBoolean(const std::string &value);

    protected:
        friend class ChatTab;
        friend class WhisperTab;

        /**
         * Handle an announce command.
         */
        static void handleAnnounce(const std::string &args,
                                   ChatTab *const tab);

        /**
         * Handle a help command.
         */
        static void handleHelp(const std::string &args, ChatTab *const tab);

        /**
         * Handle a where command.
         */
       static void handleWhere(const std::string &args, ChatTab *const tab);

        /**
         * Handle a who command.
         */
        static void handleWho(const std::string &args, ChatTab *const tab);

        /**
         * Handle a msg command.
         */
        static void handleMsg(const std::string &args, ChatTab *const tab);

        /**
         * Handle a msg tab request.
         */
        static void handleQuery(const std::string &args, ChatTab *const tab);

        /**
         * Handle a join command.
         */
        static void handleJoin(const std::string &args, ChatTab *const tab);

        /**
         * Handle a listchannels command.
         */
        static void handleListChannels(const std::string &args,
                                       ChatTab *const tab);

        /**
         * Handle a clear command.
         */
        static void handleClear(const std::string &args, ChatTab *const tab);

        /**
         * Handle a createparty command.
         */
        static void handleCreateParty(const std::string &args,
                                      ChatTab *const tab);

        /**
         * Handle a createguild command.
         */
        static void handleCreateGuild(const std::string &args,
                                      ChatTab *const tab);

        /**
         * Handle a party command.
         */
        static void handleParty(const std::string &args, ChatTab *const tab);

        /**
         * Handle a me command.
         */
        static void handleMe(const std::string &args, ChatTab *const tab);

        /**
         * Handle a toggle command.
         */
        static void handleToggle(const std::string &args, ChatTab *const tab);

        /**
         * Handle a present command.
         */
        static void handlePresent(const std::string &args, ChatTab *const tab);

        /**
         * Handle an ignore command.
         */
        static void handleIgnore(const std::string &args, ChatTab *const tab);

        /**
         * Handle an unignore command.
         */
        static void handleUnignore(const std::string &args,
                                   ChatTab *const tab);

        /**
         * Handle an friend command.
         */
        static void handleFriend(const std::string &args, ChatTab *const tab);

        /**
         * Handle an disregard command.
         */
        static void handleDisregard(const std::string &args,
                                    ChatTab *const tab);

        /**
         * Handle an neutral command.
         */
        static void handleNeutral(const std::string &args, ChatTab *const tab);

        /**
         * Handle an blacklist command.
         */
        static void handleBlackList(const std::string &args,
                                    ChatTab *const tab);

        /**
         * Handle an enemy command.
         */
        static void handleEnemy(const std::string &args, ChatTab *const tab);

        /**
         * Handle an erase command.
         */
        static void handleErase(const std::string &args, ChatTab *const tab);

        /**
         * Change relation.
         */
        static void changeRelation(const std::string &args,
                                   const PlayerRelation::Relation relation,
                                   const std::string &relationText,
                                   ChatTab *const tab);

        /**
         * Handle a quit command.
         */
        static void handleQuit(const std::string &args, ChatTab *const tab);

        /**
         * Handle show all command.
         */
        static void handleShowAll(const std::string &args, ChatTab *const tab);

        /**
         * Handle move command.
         */
        static void handleMove(const std::string &args, ChatTab *const tab);

        /**
         * Handle target command.
         */
        static void handleTarget(const std::string &args, ChatTab *const tab);
        /**
         * Handle atkhuman command.
         */
        static void handleAttackHuman(const std::string &args, ChatTab *const tab);

        /**
         * Handle closeall command.
         */
        static void handleCloseAll(const std::string &args,
                                   ChatTab *const tab);

        /**
         * Handle ignoreall command.
         */
        static void handleIgnoreAll(const std::string &args,
                                    ChatTab *const tab);

        /**
         * Handle outfit command.
         */
        static void handleOutfit(const std::string &args, ChatTab *const tab);

        /**
         * Handle emote command.
         */
        static void handleEmote(const std::string &args, ChatTab *const tab);

        /**
         * Handle away command.
         */
        static void handleAway(const std::string &args, ChatTab *const tab);

        /**
         * Handle pseudo away command.
         */
        static void handlePseudoAway(const std::string &args,
                                     ChatTab *const tab);

        /**
         * Handle follow command.
         */
        static void handleFollow(const std::string &args, ChatTab *const tab);

        /**
         * Handle imitation command.
         */
        static void handleImitation(const std::string &args,
                                    ChatTab *const tab);

        /**
         * Handle heal command.
         */
        static void handleHeal(const std::string &args, ChatTab *const tab);

        /**
         * Handle navigate command.
         */
        static void handleNavigate(const std::string &args,
                                   ChatTab *const tab);

        static void handleMail(const std::string &args, ChatTab *const tab);

        static void handleHack(const std::string &args, ChatTab *const tab);

        static void handlePriceLoad(const std::string &args,
                                    ChatTab *const tab);

        static void handlePriceSave(const std::string &args,
                                    ChatTab *const tab);

        static void handleTrade(const std::string &args, ChatTab *const tab);

        static void handleDisconnect(const std::string &args,
                                     ChatTab *const tab);

        static void handleUndress(const std::string &args, ChatTab *const tab);

        static void handleAttack(const std::string &args, ChatTab *const tab);

        static void handleDirs(const std::string &args, ChatTab *const tab);

        static void handleInfo(const std::string &args, ChatTab *const tab);

        static void handleWait(const std::string &args, ChatTab *const tab);

        static void handleUptime(const std::string &args, ChatTab *const tab);

        static void handleAddAttack(const std::string &args,
                                    ChatTab *const tab);

        static void handleAddPriorityAttack(const std::string &args,
                                            ChatTab *const tab);

        static void handleRemoveAttack(const std::string &args,
                                       ChatTab *const tab);

        static void handleAddIgnoreAttack(const std::string &args,
                                          ChatTab *const tab);

        static void handleServerIgnoreAll(const std::string &args,
                                          ChatTab *const tab);

        static void handleServerUnIgnoreAll(const std::string &args,
                                            ChatTab *const tab);

        static void handleSetDrop(const std::string &args, ChatTab *const tab);

        static void handleError(const std::string &args, ChatTab *const tab);

        static void handleUrl(const std::string &args, ChatTab *const tab);

        static void handleOpen(const std::string &args, ChatTab *const tab);

        static void handleDump(const std::string &args, ChatTab *const tab);

        static void handleDumpGraphics(const std::string &args,
                                       ChatTab *const tab);

        static void handleDumpTests(const std::string &args,
                                    ChatTab *const tab);

        static void handleDumpOGL(const std::string &args, ChatTab *const tab);

        static void outString(ChatTab *const tab, const std::string &str,
                              const std::string &def);

        static void outStringNormal(ChatTab *const tab, const std::string &str,
                                    const std::string &def);

        static void handleCacheInfo(const std::string &args,
                                    ChatTab *const tab);

        static bool parse2Int(const std::string &args, int &x, int &y);
};

extern CommandHandler *commandHandler;

#endif // COMMANDHANDLER_H
