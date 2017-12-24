/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_CHAT_CHATTAB_H
#define GUI_WIDGETS_TABS_CHAT_CHATTAB_H

#include "enums/gui/chatmsgtype.h"
#include "enums/gui/chattabtype.h"

#include "enums/simpletypes/ignorerecord.h"
#include "enums/simpletypes/online.h"
#include "enums/simpletypes/tryremovecolors.h"

#include "gui/widgets/browserbox.h"

#include "gui/widgets/tabs/tab.h"

#include "localconsts.h"

class ScrollArea;

#define debugMsg(str) \
    if (debugChatTab) \
    { \
        debugChatTab->chatLog(str, \
            ChatMsgType::BY_SERVER, \
            IgnoreRecord_false, \
            TryRemoveColors_true); \
    }

#define setTabColors(name) \
    setTabColor(&getThemeColor(name, 255U), \
        &getThemeColor(name##_OUTLINE, 255U)); \
    setHighlightedTabColor(&getThemeColor(name##_HIGHLIGHTED, 255U), \
        &getThemeColor(name##_HIGHLIGHTED_OUTLINE, 255U)); \
    setSelectedTabColor(&getThemeColor(name##_SELECTED, 255U), \
        &getThemeColor(name##_SELECTED_OUTLINE, 255U))

/**
 * A tab for the chat window. This is special to ease chat handling.
 */
class ChatTab notfinal : public Tab
{
    public:
        /**
         * Constructor.
         */
        ChatTab(const Widget2 *const widget,
                const std::string &name,
                const std::string &channel,
                const std::string &logName,
                const ChatTabTypeT &type);

        A_DELETE_COPY(ChatTab)

        virtual ~ChatTab();

        /**
         * Adds a line of text to our message list. Parameters:
         *
         * @param line Text message.
         * @param own  Type of message (usually the owner-type).
         * @param ignoreRecord should this not be recorded?
         * @param tryRemoveColors try remove color if configured
         */
        void chatLog(std::string line,
                     ChatMsgTypeT own,
                     const IgnoreRecord ignoreRecord,
                     const TryRemoveColors tryRemoveColors);

        /**
         * Adds the text to the message list
         *
         * @param msg  The message text which is to be sent.
         */
        void chatLog(const std::string &nick, std::string msg);

        /**
         * Determines whether the message is a command or message, then
         * sends the given message to the game server to be said, or to the
         * command handler
         *
         * @param msg  The message text which is to be sent.
         */
        void chatInput(const std::string &msg);

        /**
         * Scrolls the chat window
         *
         * @param amount direction and amount to scroll.  Negative numbers scroll
         * up, positive numbers scroll down.  The absolute amount indicates the
         * amount of 1/8ths of chat window real estate that should be scrolled.
         */
        void scroll(const int amount);

        /**
         * Clears the text from the tab
         */
        void clearText();

        /**
         * Handle special commands. Allows a tab to handle commands it
         * defines itself.
         *
         * @returns true  if the command was handled
         *          false if the command was not handled
         */
        virtual bool handleCommand(const std::string &restrict type A_UNUSED,
                                   const std::string &restrict args A_UNUSED)
        { return false; }

        /**
         * Handle special commands. Allows a tab to handle commands it
         * defines itself.
         *
         * @returns true  if the command was handled
         *          false if the command was not handled
         */
        virtual bool handleCommands(const std::string &type,
                                    const std::string &args);

        /**
         * Returns type of the being.
         */
        ChatTabTypeT getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void saveToLogFile(const std::string &msg) const;

        const std::list<std::string> &getRows() const A_WARN_UNUSED
        { return mTextOutput->getRows(); }

        bool hasRows() const A_WARN_UNUSED
        { return mTextOutput->hasRows(); }

        void loadFromLogFile(const std::string &name);

        bool getAllowHighlight() const noexcept2 A_WARN_UNUSED
        { return mAllowHightlight; }

        void setAllowHighlight(const bool n) noexcept2
        { mAllowHightlight = n; }

        bool getRemoveNames() const noexcept2 A_WARN_UNUSED
        { return mRemoveNames; }

        void setRemoveNames(const bool n) noexcept2
        { mRemoveNames = n; }

        bool getNoAway() const noexcept2 A_WARN_UNUSED
        { return mNoAway; }

        void setNoAway(const bool n) noexcept2
        { mNoAway = n; }

        void addNewRow(std::string &line);

        void showOnline(const std::string &nick,
                        const Online online);

        virtual void playNewMessageSound() const;

        const std::string &getChannelName() const
        { return mChannelName; }

    protected:
        friend class ChatWindow;
        friend class WhisperWindow;

        void setCurrent() override final
        { setFlash(0); }

        virtual void handleInput(const std::string &msg);

        virtual void handleCommandStr(const std::string &msg);

        virtual void handleHelp(const std::string &msg);

        virtual void getAutoCompleteList(StringVect &names A_UNUSED) const
        {}

        virtual void getAutoCompleteCommands(StringVect &names A_UNUSED) const
        {}

        void addRow(std::string &line);

        BrowserBox *mTextOutput A_NONNULLPOINTER;
        ScrollArea *mScrollArea;
        std::string mChannelName;
        std::string mLogName;
        ChatTabTypeT mType;
        bool mAllowHightlight;
        bool mRemoveNames;
        bool mNoAway;
        bool mShowOnline;
};

extern ChatTab *localChatTab;
extern ChatTab *debugChatTab;

#endif  // GUI_WIDGETS_TABS_CHAT_CHATTAB_H
