/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_CHATWINDOW_H
#define GUI_WINDOWS_CHATWINDOW_H

#include "gui/chatmsgtype.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/attributelistener.h"
#include "listeners/configlistener.h"
#include "listeners/keylistener.h"
#include "listeners/statlistener.h"

#include <list>
#include <map>
#include <set>

class Button;
class ChannelTab;
class ChatTab;
class ChatInput;
class ColorListModel;
class DropDown;
class TabbedArea;
class ItemLinkHandler;
class WhisperTab;

const int DEFAULT_CHAT_WINDOW_SCROLL = 7;

/**
 * The chat window.
 *
 * \ingroup Interface
 */
class ChatWindow final : public Window,
                         public ActionListener,
                         public KeyListener,
                         public ConfigListener,
                         public AttributeListener,
                         public StatListener
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
        static void clearTab(ChatTab *const tab);

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
        void action(const ActionEvent &event) override final;

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
        void localChatInput(const std::string &msg) const;

        /** Called when key is pressed */
        void keyPressed(KeyEvent &event) override final;

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
        void mouseDragged(MouseEvent &event) override final;

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(MouseEvent &event) override final;

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

        void addWhisper(const std::string &restrict nick,
                        const std::string &restrict mes,
                        const ChatMsgType::Type own = ChatMsgType::BY_OTHER);

        WhisperTab *addWhisperTab(const std::string &caption,
                                  const std::string &nick,
                                  const bool switchTo = false) A_WARN_UNUSED;

        WhisperTab *getWhisperTab(const std::string &nick) const A_WARN_UNUSED;

        ChannelTab *addChannelTab(const std::string &name,
                                  const bool switchTo = false);

        ChatTab *addChatTab(const std::string &name,
                            const bool switchTo,
                            const bool join) A_WARN_UNUSED;

        void removeAllWhispers();

        void removeAllChannels();

        void ignoreAllWhispers();

        bool resortChatLog(std::string line, ChatMsgType::Type own,
                           const std::string &channel,
                           const bool ignoreRecord,
                           const bool tryRemoveColors);

        static void battleChatLog(const std::string &line,
                                  ChatMsgType::Type own
                                  = ChatMsgType::BY_UNKNOWN,
                                  const bool ignoreRecord = false,
                                  const bool tryRemoveColors = true);

        void channelChatLog(const std::string &channel,
                            const std::string &line,
                            ChatMsgType::Type own,
                            const bool ignoreRecord,
                            const bool tryRemoveColors);

        void updateOnline(const std::set<std::string> &onlinePlayers) const;

        void loadState();

        void saveState() const;

        bool saveTab(const int num, ChatTab *const tab) const;

        void loadCustomList();

        void loadGMCommands();

        static std::string doReplace(const std::string &msg) A_WARN_UNUSED;

        void adjustTabSize();

        void addToAwayLog(const std::string &line);

        void displayAwayLog() const;

        void clearAwayLog()
        { mAwayLog.clear(); }

        void parseHighlights();

        void parseGlobalsFilter();

        bool findHighlight(const std::string &str) A_WARN_UNUSED;

        void copyToClipboard(const int x, const int y) const;

        void optionChanged(const std::string &name) override final;

        void mouseEntered(MouseEvent& event) override final;

        void mouseMoved(MouseEvent &event) override final;

        void mouseExited(MouseEvent& event A_UNUSED) override final;

        void draw(Graphics* graphics) override final;

        void updateVisibility();

        void unHideWindow();

        void widgetResized(const Event &event) override final;

        void addGlobalMessage(const std::string &line);

        void postInit() override final;

        bool isTabPresent(const ChatTab *const tab) const A_WARN_UNUSED;

        void attributeChanged(const int id,
                              const int oldVal,
                              const int newVal) override final;

        void statChanged(const int id,
                         const int oldVal1,
                         const int oldVal2) override final;

        static void localPetSay(const std::string &nick,
                                const std::string &text);

        static void localPetEmote(const std::string &nick,
                                  const uint8_t emoteId);

        static void localPetMove(const std::string &nick,
                                 const int x, const int y);

        void postConnection();

#ifdef USE_PROFILER
        void logicChildren();
#endif

    protected:
        friend class ChannelTab;
        friend class ChatTab;
        friend class WhisperTab;
        friend class PopupMenu;

        typedef std::list<std::string> History;

        /** Remove the given tab from the window */
        void removeTab(ChatTab *const tab);

        /** Add the tab to the window */
        void addTab(ChatTab *const tab);

        void removeWhisper(const std::string &nick);

        void removeChannel(const std::string &nick);

        void autoComplete();

        std::string addColors(std::string &msg);

        std::string autoCompleteHistory(const std::string &partName) const;

        std::string autoComplete(const std::string &partName,
                                 const History *const words) const;

        static std::string autoComplete(const StringVect &names,
                                        std::string partName);

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

        void updateTabsMargin();

        bool addCurrentToHistory();

        typedef std::map<const std::string, WhisperTab*> TabMap;
        typedef std::map<const std::string, ChannelTab*> ChannelMap;

        /** Manage whisper tabs */
        TabMap mWhispers;
        ChannelMap mChannels;

        typedef History::iterator HistoryIterator;
        History mHistory;          /**< Command history. */
        HistoryIterator mCurHist;  /**< History iterator. */

        typedef std::list<std::string> ChatCommands;
        typedef ChatCommands::iterator ChatCommandsIterator;
        History mCommands;         /**< Command list. */
        History mCustomWords;

        StringVect mTradeFilter;

        ColorListModel *mColorListModel;
        DropDown *mColorPicker;
        Button *mChatButton;
        std::list<std::string> mAwayLog;
        StringVect mHighlights;
        StringVect mGlobalsFilter;
        int mChatColor;
        unsigned int mChatHistoryIndex;
        bool mReturnToggles;  // Marks whether <Return> toggles the chat log
                              // or not
        bool mGMLoaded;
        bool mHaveMouse;
        bool mAutoHide;
        bool mShowBattleEvents;
        bool mShowAllLang;
        bool mTmpVisible;
};

extern ChatWindow *chatWindow;

#endif  // GUI_WINDOWS_CHATWINDOW_H
