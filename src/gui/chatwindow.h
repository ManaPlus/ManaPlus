/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "listener.h"

#include "configlistener.h"

#include "utils/stringvector.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/widget.hpp>
#include <guichan/widgetlistener.hpp>

#include <list>
#include <map>
#include <set>

class BrowserBox;
class ChatTab;
class Channel;
class ChatInput;
class ColorListModel;
class DropDown;
class ScrollArea;
class TabbedArea;
class ItemLinkHandler;
class Tab;
class WhisperTab;

const int DEFAULT_CHAT_WINDOW_SCROLL = 7;

enum Own
{
    BY_GM = 0,
    BY_PLAYER,
    BY_OTHER,
    BY_SERVER,
    BY_CHANNEL,
    ACT_WHISPER,      // getting whispered at
    ACT_IS,           // equivalent to "/me" on IRC
    BY_LOGGER,
    BY_UNKNOWN = -1
};

/** One item in the chat log */
struct CHATLOG final
{
    CHATLOG() :
        nick(),
        text(),
        own(BY_UNKNOWN)
    {
    }

    A_DELETE_COPY(CHATLOG)

    std::string nick;
    std::string text;
    Own own;
};

/**
 * The chat window.
 *
 * \ingroup Interface
 */
class ChatWindow final : public Window,
                         public gcn::ActionListener,
                         public gcn::KeyListener,
                         public Listener,
                         public ConfigListener
{
    public:
        /**
         * Constructor.
         */
        ChatWindow();

        A_DELETE_COPY(ChatWindow)

        /**
         * Destructor: used to write back values to the config file
         */
        ~ChatWindow();

        /**
         * Gets the focused tab.
         */
        ChatTab *getFocused() const A_WARN_UNUSED;

        /**
         * Clear the given tab.
         */
        void clearTab(ChatTab *const tab) const;

        /**
         * Clear the current tab.
         */
        void clearTab() const;

        /**
         * Switch to the previous tab in order
         */
        void prevTab();

        /**
         * Switch to the next tab in order
         */
        void nextTab();

        /**
         * Close current chat tab
         */
        void closeTab() const;

        /**
         * Switch to the default tab
         */
        void defaultTab();

        /**
         * Performs action.
         */
        void action(const gcn::ActionEvent &event) override;

        /**
         * Request focus for typing chat message.
         *
         * \returns true if the input was shown
         *          false otherwise
         */
        bool requestChatFocus();

        /**
         * Checks whether ChatWindow is Focused or not.
         */
        bool isInputFocused() const A_WARN_UNUSED;

        /**
         * Passes the text to the current tab as input
         *
         * @param msg  The message text which is to be sent.
         */
        void chatInput(const std::string &msg) const;

        /**
         * Passes the text to the local chat tab as input
         *
         * @param msg  The message text which is to be sent.
         */
        void localChatInput(const std::string &msg);

        /** Called when key is pressed */
        void keyPressed(gcn::KeyEvent &event) override;

        /** Set the chat input as the given text. */
        void setInputText(const std::string &text);

        /** Add the given text to the chat input. */
        void addInputText(const std::string &text, const bool space = true);

        /** Called to add item to chat */
        void addItemText(const std::string &item);

        /** Override to reset mTmpVisible */
        void setVisible(bool visible);

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(gcn::MouseEvent &event) override;

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(gcn::MouseEvent &event) override;

        void processEvent(Channels channel,
                          const DepricatedEvent &event) override;

        /**
         * Scrolls the chat window
         *
         * @param amount direction and amount to scroll.  Negative numbers scroll
         * up, positive numbers scroll down.  The absolute amount indicates the
         * amount of 1/8ths of chat window real estate that should be scrolled.
         */
        void scroll(const int amount) const;

        /**
         * Sets the file being recorded to
         *
         * @param msg The file to write out to. If null, then stop recording.
         */
        void setRecordingFile(const std::string &msg);

        bool getReturnTogglesChat() const A_WARN_UNUSED
        { return mReturnToggles; }

        void setReturnTogglesChat(const bool toggles)
        { mReturnToggles = toggles; }

        void doPresent() const;

        void addWhisper(const std::string &nick, const std::string &mes,
                        const Own own = BY_OTHER);

        WhisperTab *addWhisperTab(const std::string &nick,
                                  const bool switchTo = false) A_WARN_UNUSED;

        WhisperTab *getWhisperTab(const std::string &nick) const A_WARN_UNUSED;

        void removeAllWhispers();

        void ignoreAllWhispers();

        void resortChatLog(std::string line, Own own,
                           const std::string &channel,
                           const bool ignoreRecord,
                           const bool tryRemoveColors);

        static void battleChatLog(const std::string &line,
                                  Own own = BY_UNKNOWN,
                                  const bool ignoreRecord = false,
                                  const bool tryRemoveColors = true);

        void updateOnline(std::set<std::string> &onlinePlayers) const;

        void loadState();

        void saveState() const;

        void loadCustomList();

        void loadGMCommands();

        std::string doReplace(const std::string &msg) const A_WARN_UNUSED;

        void adjustTabSize();

        void addToAwayLog(const std::string &line);

        void displayAwayLog() const;

        void clearAwayLog()
        { mAwayLog.clear(); }

        void parseHighlights();

        bool findHighlight(const std::string &str) A_WARN_UNUSED;

        void copyToClipboard(const int x, const int y) const;

        void optionChanged(const std::string &name) override;

        void mouseEntered(gcn::MouseEvent& mouseEvent) override;

        void mouseMoved(gcn::MouseEvent &event) override;

        void mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED) override;

        void draw(gcn::Graphics* graphics) override;

        void updateVisibility();

        void unHideWindow();

        void widgetResized(const gcn::Event &event) override;

    protected:
        friend class ChatTab;
        friend class WhisperTab;
        friend class PopupMenu;

        typedef std::list<std::string> History;

        /** Remove the given tab from the window */
        void removeTab(ChatTab *const tab);

        /** Add the tab to the window */
        void addTab(ChatTab *const tab);

        void removeWhisper(const std::string &nick);

        void autoComplete();

        std::string addColors(std::string &msg);

        std::string autoCompleteHistory(const std::string &partName) const;

        std::string autoComplete(const std::string &partName,
                                 History *const words) const;

        std::string autoComplete(StringVect &names,
                                 std::string partName) const;

        /** Used for showing item popup on clicking links **/
        ItemLinkHandler *mItemLinkHandler;

        /** Tabbed area for holding each channel. */
        TabbedArea *mChatTabs;

        /** Input box for typing chat messages. */
        ChatInput *mChatInput;

        void initTradeFilter();

        int mRainbowColor;

    private:
        void fillCommands();

        void loadCommandsFile(const std::string &name);


        typedef std::map<const std::string, WhisperTab*> TabMap;
        /** Manage whisper tabs */
        TabMap mWhispers;

        typedef History::iterator HistoryIterator;
        History mHistory;          /**< Command history. */
        HistoryIterator mCurHist;  /**< History iterator. */

        typedef std::list<std::string> Commands;
        typedef Commands::iterator CommandsIterator;
        History mCommands;         /**< Command list. */
        History mCustomWords;

        bool mReturnToggles;  // Marks whether <Return> toggles the chat log
                              // or not

        StringVect mTradeFilter;

        ColorListModel *mColorListModel;
        DropDown *mColorPicker;
        int mChatColor;
        unsigned int mChatHistoryIndex;
        std::list<std::string> mAwayLog;
        StringVect mHighlights;
        bool mGMLoaded;
        bool mHaveMouse;
        bool mAutoHide;
        bool mShowBattleEvents;
        bool mShowAllLang;
        bool mTmpVisible;
};

extern ChatWindow *chatWindow;

#endif
