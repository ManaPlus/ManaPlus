/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/windows/chatwindow.h"

#include "actormanager.h"
#include "game.h"
#include "guild.h"
#include "party.h"
#include "settings.h"
#include "spellmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "const/gui/chat.h"

#include "fs/virtfs/virtfstools.h"

#include "input/inputmanager.h"

#include "gui/focushandler.h"
#include "gui/gui.h"
#include "gui/skin.h"
#include "gui/viewport.h"

#include "gui/models/colorlistmodel.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/setupwindow.h"
#include "gui/windows/whoisonline.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chatinput.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/tabs/chat/battletab.h"
#include "gui/widgets/tabs/chat/channeltab.h"
#include "gui/widgets/tabs/chat/gmtab.h"
#include "gui/widgets/tabs/chat/langtab.h"
#include "gui/widgets/tabs/chat/tradetab.h"
#include "gui/widgets/tabs/chat/whispertab.h"

#include "render/opengl/opengldebug.h"

#include "net/chathandler.h"
#include "net/serverfeatures.h"

#include "utils/copynpaste.h"
#include "utils/delete2.h"

#include <sys/stat.h>

#include <sstream>

#include "debug.h"

ChatWindow *chatWindow = nullptr;

static const char *const ACTION_COLOR_PICKER = "color picker";

ChatWindow::ChatWindow() :
    // TRANSLATORS: chat window name
    Window(_("Chat"), Modal_false, nullptr, "chat.xml"),
    ActionListener(),
    KeyListener(),
    AttributeListener(),
    mItemLinkHandler(new ItemLinkHandler),
    mChatTabs(CREATEWIDGETR(TabbedArea, this)),
    mChatInput(new ChatInput(this)),
    mRainbowColor(0U),
    mWhispers(),
    mChannels(),
    mHistory(),
    mCurHist(),
    mCommands(),
    mCustomWords(),
    mTradeFilter(),
    mColorListModel(new ColorListModel),
    mColorPicker(new DropDown(this, mColorListModel)),
    mChatButton(new Button(this, ":)", "openemote", this)),
    mAwayLog(),
    mHighlights(),
    mGlobalsFilter(),
    mChatColor(config.getIntValue("chatColor")),
    mEmoteButtonSpacing(mSkin ? mSkin->getOption("emoteButtonSpacing", 2) : 2),
    mEmoteButtonY(mSkin ? mSkin->getOption("emoteButtonY", -2) : -2),
    mChatHistoryIndex(0),
    mReturnToggles(config.getBoolValue("ReturnToggles")),
    mGMLoaded(false),
    mHaveMouse(false),
    mAutoHide(config.getBoolValue("autohideChat")),
    mShowBattleEvents(config.getBoolValue("showBattleEvents")),
    mShowAllLang(serverConfig.getValue("showAllLang", 0)),
    mEnableTradeFilter(config.getBoolValue("enableTradeFilter")),
    mTmpVisible(false)
{
    setWindowName("Chat");

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    int w = 600;
#ifdef ANDROID
    if (mainGraphics->getWidth() < 710)
        w = mainGraphics->getWidth() - 110;
    if (w < 100)
        w = 100;
    if (mainGraphics->getHeight() < 480)
        setDefaultSize(w, 90, ImagePosition::UPPER_LEFT, -110, -35);
    else
        setDefaultSize(w, 123, ImagePosition::UPPER_LEFT, -110, -35);
#else  // ANDROID

    if (mainGraphics->getWidth() < 600)
        w = mainGraphics->getWidth() - 10;
    if (w < 100)
        w = 100;
    setDefaultSize(w, 123, ImagePosition::LOWER_LEFT);
#endif  // ANDROID

    setMinWidth(150);
    setMinHeight(90);

    setTitleBarHeight(getPadding() + getTitlePadding());

    if (emoteWindow)
        emoteWindow->addListeners(this);

    mChatButton->adjustSize();

    mChatTabs->enableScrollButtons(true);
    mChatTabs->setFollowDownScroll(true);
    mChatTabs->setResizeHeight(false);

    mChatInput->setActionEventId("chatinput");
    mChatInput->addActionListener(this);
    mChatInput->setAllowSpecialActions(false);

    mColorPicker->setActionEventId(ACTION_COLOR_PICKER);
    mColorPicker->addActionListener(this);
    mColorPicker->setSelected(mChatColor);

    mItemLinkHandler->setAllowCommands(false);

    loadWindowState();

    mColorPicker->setPosition(this->getWidth() - mColorPicker->getWidth()
        - 2 * mPadding - 8 - 16, mPadding);

    // Add key listener to chat input to be able to respond to up/down
    mChatInput->addKeyListener(this);
    mCurHist = mHistory.end();
    mColorPicker->setVisible(fromBool(config.getBoolValue(
        "showChatColorsList"), Visible));
    updateTabsMargin();

    fillCommands();
    if (localPlayer && localPlayer->isGM())
        loadGMCommands();
    initTradeFilter();
    loadCustomList();
    parseHighlights();
    parseGlobalsFilter();

    config.addListener("autohideChat", this);
    config.addListener("showBattleEvents", this);
    config.addListener("globalsFilter", this);
    config.addListener("enableTradeFilter", this);

    enableVisibleSound(true);
}

ChatWindow::~ChatWindow()
{
    config.removeListeners(this);
    CHECKLISTENERS
    saveState();
    config.setValue("ReturnToggles", mReturnToggles);
    removeAllWhispers();
    removeAllChannels();
    delete2(mItemLinkHandler);
    delete2(mColorPicker);
    delete2(mColorListModel);
}

void ChatWindow::postInit()
{
    Window::postInit();
    add(mChatTabs);
    add(mChatInput);
    add(mColorPicker);
    add(mChatButton);
    updateVisibility();
}

void ChatWindow::loadCommandsFile(const std::string &name)
{
    StringVect list;
    VirtFs::loadTextFile(name, list);
    StringVectCIter it = list.begin();
    const StringVectCIter it_end = list.end();

    while (it != it_end)
    {
        const std::string str = *it;
        if (!str.empty())
            mCommands.push_back(str);
        ++ it;
    }
}

void ChatWindow::fillCommands()
{
    loadCommandsFile("chatcommands.txt");
    inputManager.addChatCommands(mCommands);
}

void ChatWindow::loadGMCommands()
{
    if (mGMLoaded)
        return;

    loadCommandsFile("gmcommands.txt");
    mGMLoaded = true;
}

void ChatWindow::updateTabsMargin()
{
    if (mColorPicker->mVisible == Visible_true)
        mChatTabs->setRightMargin(mColorPicker->getWidth() + 16 + 8);
    else
        mChatTabs->setRightMargin(8);
}

void ChatWindow::adjustTabSize()
{
    const Rect area = getChildrenArea();

    const int aw = area.width;
    const int ah = area.height;
    const int frame = mChatInput->getFrameSize();
    const int inputHeight = mChatInput->getHeight();
    const int frame2 = 2 * frame;
    const int awFrame2 = aw - frame2;
    const bool showEmotes = config.getBoolValue("showEmotesButton");
    int y = ah - inputHeight - frame;
    mChatInput->setPosition(frame, y);
    mChatTabs->setWidth(awFrame2);
    const int height = ah - frame2 - (inputHeight + frame2);
    if (mChatInput->mVisible == Visible_true ||
        !config.getBoolValue("hideChatInput"))
    {
        mChatTabs->setHeight(height);
    }
    else
    {
        mChatTabs->setHeight(height + inputHeight);
    }
    updateTabsMargin();

    if (showEmotes)
    {
        const int chatButtonSize = 20;
        int w = awFrame2 - chatButtonSize;
        const int x = aw - frame - chatButtonSize;
        w -= mEmoteButtonSpacing;
        y += mEmoteButtonY;
        mChatInput->setWidth(w);
        mChatButton->setVisible(mChatInput->mVisible);
        mChatButton->setPosition(x, y);
    }
    else
    {
        mChatInput->setWidth(awFrame2);
        mChatButton->setVisible(Visible_false);
    }

    const ChatTab *const tab = getFocused();
    if (tab)
    {
        Widget *const content = tab->mScrollArea;
        if (content)
        {
            const int contentFrame2 = 2 * content->getFrameSize();
            content->setSize(mChatTabs->getWidth() - contentFrame2,
                mChatTabs->getContainerHeight() - contentFrame2);
            content->logic();
        }
    }

    mColorPicker->setPosition(this->getWidth() - mColorPicker->getWidth()
        - 2 * mPadding - 8 - 16, mPadding);

    mChatTabs->adjustSize();
}

void ChatWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    adjustTabSize();
}

ChatTab *ChatWindow::getFocused() const
{
    return static_cast<ChatTab*>(mChatTabs->getSelectedTab());
}

void ChatWindow::clearTab(ChatTab *const tab)
{
    if (tab)
        tab->clearText();
}

void ChatWindow::clearTab() const
{
    clearTab(getFocused());
}

void ChatWindow::prevTab()
{
    if (!mChatTabs)
        return;

    int tab = mChatTabs->getSelectedTabIndex();

    if (tab <= 0)
        tab = mChatTabs->getNumberOfTabs();
    tab--;

    mChatTabs->setSelectedTabByIndex(tab);
}

void ChatWindow::nextTab()
{
    if (!mChatTabs)
        return;

    int tab = mChatTabs->getSelectedTabIndex();

    tab++;
    if (tab == mChatTabs->getNumberOfTabs())
        tab = 0;

    mChatTabs->setSelectedTabByIndex(tab);
}

void ChatWindow::selectTabByType(const ChatTabTypeT &type)
{
    if (!mChatTabs)
        return;

    int sz = mChatTabs->getNumberOfTabs();
    for (int f = 0; f < sz; f ++)
    {
        ChatTab *const tab = dynamic_cast<ChatTab*>(
            mChatTabs->getTabByIndex(f));
        if (tab && tab->getType() == type)
        {
            mChatTabs->setSelectedTab(tab);
            break;
        }
    }
}

void ChatWindow::closeTab() const
{
    if (!mChatTabs)
        return;

    ChatTab *const tab = dynamic_cast<ChatTab*>(mChatTabs->getTabByIndex(
        mChatTabs->getSelectedTabIndex()));
    if (!tab)
        return;
    const ChatTabTypeT &type = tab->getType();
    if (type == ChatTabType::WHISPER || type == ChatTabType::CHANNEL)
        tab->handleCommand("close", "");
}

void ChatWindow::defaultTab()
{
    if (mChatTabs)
        mChatTabs->setSelectedTabByIndex(CAST_U32(0));
}

void ChatWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "chatinput")
    {
        std::string message = mChatInput->getText();

        if (!message.empty())
        {
            // If message different from previous, put it in the history
            if (mHistory.empty() || message != mHistory.back())
                mHistory.push_back(message);

            // Reset history iterator
            mCurHist = mHistory.end();

            // Send the message to the server
            chatInput(addColors(message));

            // Clear the text from the chat input
            mChatInput->setText("");
        }

        if (message.empty() || !mReturnToggles)
        {
            // Remove focus and hide input
            mChatInput->unprotectFocus();
            if (mFocusHandler)
                mFocusHandler->focusNone();

            // If the chatWindow is shown up because you want to send a message
            // It should hide now
            if (mTmpVisible)
                setVisible(Visible_false);
        }
    }
    else if (eventId == "emote")
    {
        if (emoteWindow)
        {
            const std::string str = emoteWindow->getSelectedEmote();
            if (!str.empty())
            {
                addInputText(str, false);
                emoteWindow->clearEmote();
            }
        }
    }
    else if (eventId == "openemote")
    {
        if (emoteWindow)
        {
            if (emoteWindow->mVisible == Visible_true)
                emoteWindow->hide();
            else
                emoteWindow->show();
        }
    }
    else if (eventId == "color")
    {
        if (emoteWindow)
        {
            const std::string str = emoteWindow->getSelectedColor();
            if (!str.empty())
            {
                addInputText(str, false);
                emoteWindow->clearColor();
            }
        }
    }
    else if (eventId == "font")
    {
        if (emoteWindow)
        {
            const std::string str = emoteWindow->getSelectedFont();
            if (!str.empty())
            {
                addInputText(str, false);
                emoteWindow->clearFont();
            }
        }
    }
    else if (eventId == ACTION_COLOR_PICKER)
    {
        if (mColorPicker)
        {
            mChatColor = mColorPicker->getSelected();
            config.setValue("chatColor", mChatColor);
        }
    }

    if (mColorPicker)
    {
        const Visible vis = fromBool(config.getBoolValue(
            "showChatColorsList"), Visible);
        if (mColorPicker->mVisible != vis)
            mColorPicker->setVisible(vis);
    }
}

bool ChatWindow::requestChatFocus()
{
    // Make sure chatWindow is visible
    if (!isWindowVisible())
    {
        setVisible(Visible_true);

        /*
         * This is used to hide chatWindow after sending the message. There is
         * a trick here, because setVisible will set mTmpVisible to false, you
         * have to put this sentence *after* setVisible, not before it
         */
        mTmpVisible = true;
    }

    // Don't do anything else if the input is already visible and has focus
    if (mChatInput->isVisible() && mChatInput->isFocused())
        return false;

    // Give focus to the chat input
    mChatInput->processVisible(Visible_true);
    unHideWindow();
    mChatInput->requestFocus();
    return true;
}

bool ChatWindow::isInputFocused() const
{
    return mChatInput->isFocused();
}

void ChatWindow::removeTab(ChatTab *const tab)
{
    mChatTabs->removeTab(tab);
}

void ChatWindow::addTab(ChatTab *const tab)
{
    if (!tab)
        return;

    mChatTabs->addTab(tab, tab->mScrollArea);
    logic();
}

void ChatWindow::removeWhisper(const std::string &nick)
{
    std::string tempNick = nick;
    toLower(tempNick);
    mWhispers.erase(tempNick);
}

void ChatWindow::removeChannel(const std::string &name)
{
    std::string tempName = name;
    toLower(tempName);
    mChannels.erase(tempName);
    chatHandler->partChannel(name);
}

void ChatWindow::removeAllWhispers()
{
    std::list<ChatTab*> tabs;

    FOR_EACH (TabMap::iterator, iter, mWhispers)
        tabs.push_back(iter->second);

    for (std::list<ChatTab*>::iterator it = tabs.begin();
         it != tabs.end(); ++it)
    {
        delete *it;
    }

    mWhispers.clear();
}

void ChatWindow::removeAllChannels()
{
    std::list<ChatTab*> tabs;

    FOR_EACH (ChannelMap::iterator, iter, mChannels)
        tabs.push_back(iter->second);

    for (std::list<ChatTab*>::iterator it = tabs.begin();
         it != tabs.end(); ++it)
    {
        delete *it;
    }

    mChannels.clear();
}

void ChatWindow::ignoreAllWhispers()
{
    for (TabMap::iterator iter = mWhispers.begin();
         iter != mWhispers.end();
         ++ iter)
    {
        WhisperTab *const tab = iter->second;
        if (tab)
        {
            if (player_relations.getRelation(tab->getNick())
                != Relation::IGNORED)
            {
                player_relations.setRelation(tab->getNick(),
                    Relation::IGNORED);
            }
            tab->handleCommand("close", "");
        }
    }
}

void ChatWindow::chatInput(const std::string &message) const
{
    ChatTab *tab = nullptr;
    std::string msg = message;
    trim(msg);

    if (config.getBoolValue("allowCommandsInChatTabs")
        && msg.length() > 1
        && ((msg.at(0) == '#' && msg.at(1) != '#') || msg.at(0) == '@')
        && localChatTab)
    {
        tab = localChatTab;
    }
    else
    {
        tab = getFocused();
        if (tab == nullptr)
            tab = localChatTab;
    }
    if (tab)
        tab->chatInput(msg);
    Game::instance()->setValidSpeed();
}

void ChatWindow::localChatInput(const std::string &msg) const
{
    if (localChatTab)
        localChatTab->chatInput(msg);
    else
        chatInput(msg);
}

void ChatWindow::doPresent() const
{
    if (!actorManager)
        return;

    const ActorSprites &actors = actorManager->getAll();
    std::string response;
    int playercount = 0;

    FOR_EACH (ActorSpritesIterator, it, actors)
    {
        if ((*it)->getType() == ActorType::Player)
        {
            if (!response.empty())
                response.append(", ");
            response.append(static_cast<Being*>(*it)->getName());
            playercount ++;
        }
    }

    const std::string log = strprintf(
        // TRANSLATORS: chat message
        _("Present: %s; %d players are present."),
        response.c_str(), playercount);

    if (getFocused())
        getFocused()->chatLog(log, ChatMsgType::BY_SERVER);
}

void ChatWindow::scroll(const int amount) const
{
    if (!isWindowVisible())
        return;

    ChatTab *const tab = getFocused();
    if (tab)
        tab->scroll(amount);
}

void ChatWindow::mousePressed(MouseEvent &event)
{
    if (event.isConsumed())
        return;

    if (event.getButton() == MouseButton::RIGHT)
    {
        if (popupMenu)
        {
            ChatTab *const cTab = dynamic_cast<ChatTab*>(
                mChatTabs->getSelectedTab());
            if (cTab)
            {
                event.consume();
                if (inputManager.isActionActive(InputAction::CHAT_MOD))
                {
                    inputManager.executeChatCommand(
                        InputAction::CLOSE_CHAT_TAB,
                        std::string(), cTab);
                }
                else
                {
                    popupMenu->showChatPopup(viewport->mMouseX,
                        viewport->mMouseY,
                        cTab);
                }
            }
        }
    }

    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        const int clicks = event.getClickCount();
        if (clicks == 2)
        {
            toggleChatFocus();
            if (gui)
                gui->resetClickCount();
        }
        else if (clicks == 1)
        {
            const ChatTab *const tab = getFocused();
            if (tab)
                mMoved = !isResizeAllowed(event);
        }
    }

    mDragOffsetX = event.getX();
    mDragOffsetY = event.getY();
}

void ChatWindow::mouseDragged(MouseEvent &event)
{
    Window::mouseDragged(event);

    if (event.isConsumed())
        return;

    if (canMove() && isMovable() && mMoved)
    {
        int newX = std::max(0, getX() + event.getX() - mDragOffsetX);
        int newY = std::max(0, getY() + event.getY() - mDragOffsetY);
        newX = std::min(mainGraphics->mWidth - getWidth(), newX);
        newY = std::min(mainGraphics->mHeight - getHeight(), newY);
        setPosition(newX, newY);
    }
}

#define ifKey(key, str) \
    else if (actionId == key) \
    { \
        temp = str; \
    }

void ChatWindow::keyPressed(KeyEvent &event)
{
    const InputActionT actionId = event.getActionId();
    std::string temp;
    if (actionId == InputAction::GUI_DOWN)
    {
        if (mCurHist != mHistory.end())
        {
            // Move forward through the history
            const HistoryIterator prevHist = mCurHist++;
            addCurrentToHistory();

            if (mCurHist != mHistory.end())
            {
                mChatInput->setText(*mCurHist);
                mChatInput->setCaretPosition(CAST_U32(
                        mChatInput->getText().length()));
            }
            else
            {
                mChatInput->setText("");
                mCurHist = prevHist;
            }
        }
        else if (!mChatInput->getText().empty())
        {
            if (addCurrentToHistory())
                mCurHist = mHistory.end();
            mChatInput->setText("");
        }
    }
    else if (actionId == InputAction::GUI_UP &&
             mCurHist != mHistory.begin() &&
             !mHistory.empty())
    {
        // Move backward through the history
        --mCurHist;
        addCurrentToHistory();
        mChatInput->setText(*mCurHist);
        mChatInput->setCaretPosition(CAST_U32(
                mChatInput->getText().length()));
    }
    else if (actionId == InputAction::GUI_INSERT &&
             !mChatInput->getText().empty())
    {
        const std::string str = mChatInput->getText();
        // Add the current message to the history and clear the text
        if (mHistory.empty() || str != mHistory.back())
            mHistory.push_back(str);
        mCurHist = mHistory.end();
        mChatInput->setText(std::string());
    }
    else if (actionId == InputAction::GUI_TAB &&
             !mChatInput->getText().empty())
    {
        autoComplete();
        return;
    }
    else if (actionId == InputAction::GUI_CANCEL &&
             mChatInput->mVisible == Visible_true)
    {
        mChatInput->processVisible(Visible_false);
    }
    else if (actionId == InputAction::CHAT_PREV_HISTORY &&
             mChatInput->mVisible == Visible_true)
    {
        const ChatTab *const tab = getFocused();
        if (tab && tab->hasRows())
        {
            if (!mChatHistoryIndex)
            {
                mChatHistoryIndex = CAST_U32(
                    tab->getRows().size());

                mChatInput->setText("");
                mChatInput->setCaretPosition(0);
                return;
            }
            else
            {
                mChatHistoryIndex --;
            }

            unsigned int f = 0;
            const std::list<std::string> &rows = tab->getRows();
            for (std::list<std::string>::const_iterator it = rows.begin(),
                 it_end = rows.end(); it != it_end; ++ it, f ++)
            {
                if (f == mChatHistoryIndex)
                    mChatInput->setText(*it);
            }
            mChatInput->setCaretPosition(CAST_U32(
                mChatInput->getText().length()));
        }
    }
    else if (actionId == InputAction::CHAT_NEXT_HISTORY &&
             mChatInput->mVisible == Visible_true)
    {
        const ChatTab *const tab = getFocused();
        if (tab && tab->hasRows())
        {
            const std::list<std::string> &rows = tab->getRows();
            const size_t &tabSize = rows.size();
            if (CAST_SIZE(mChatHistoryIndex) + 1 < tabSize)
            {
                mChatHistoryIndex ++;
            }
            else if (CAST_SIZE(mChatHistoryIndex) < tabSize)
            {
                mChatHistoryIndex ++;
                mChatInput->setText("");
                mChatInput->setCaretPosition(0);
                return;
            }
            else
            {
                mChatHistoryIndex = 0;
            }

            unsigned int f = 0;
            for (std::list<std::string>::const_iterator
                 it = rows.begin(), it_end = rows.end();
                 it != it_end; ++it, f++)
            {
                if (f == mChatHistoryIndex)
                    mChatInput->setText(*it);
            }
            mChatInput->setCaretPosition(CAST_U32(
                    mChatInput->getText().length()));
        }
    }
    else if (actionId == InputAction::GUI_F1)
    {
        if (emoteWindow)
        {
            if (emoteWindow->mVisible == Visible_true)
                emoteWindow->hide();
            else
                emoteWindow->show();
        }
    }
    ifKey(InputAction::GUI_F2, "\u2318")
    ifKey(InputAction::GUI_F3, "\u263A")
    ifKey(InputAction::GUI_F4, "\u2665")
    ifKey(InputAction::GUI_F5, "\u266A")
    ifKey(InputAction::GUI_F6, "\u266B")
    ifKey(InputAction::GUI_F7, "\u26A0")
    ifKey(InputAction::GUI_F8, "\u2622")
    ifKey(InputAction::GUI_F9, "\u262E")
    ifKey(InputAction::GUI_F10, "\u2605")
    ifKey(InputAction::GUI_F11, "\u2618")
    ifKey(InputAction::GUI_F12, "\u2592")

    if (inputManager.isActionActive(InputAction::GUI_CTRL))
    {
        if (actionId == InputAction::GUI_B)
        {
            std::string inputText = mChatInput->getTextBeforeCaret();
            toLower(inputText);
            const size_t idx = inputText.rfind("##b");
            if (idx == std::string::npos
                || mChatInput->getTextBeforeCaret().substr(idx, 3) == "##b")
            {
                temp = "##B";
            }
            else
            {
                temp = "##b";
            }
        }
    }

    if (!temp.empty())
        addInputText(temp, false);
}

#undef ifKey

bool ChatWindow::addCurrentToHistory()
{
    const std::string &str = mChatInput->getText();
    if (str.empty())
        return false;
    FOR_EACH (HistoryIterator, it, mHistory)
    {
        if (*it == str)
            return false;
    }
    mHistory.push_back(str);
    return true;
}

void ChatWindow::statChanged(const AttributesT id,
                             const int oldVal1,
                             const int oldVal2)
{
    if (!mShowBattleEvents || !config.getBoolValue("showJobExp"))
        return;

    if (id != Attributes::PLAYER_JOB)
        return;

    const std::pair<int, int> exp = PlayerInfo::getStatExperience(id);
    if (oldVal1 > exp.first || !oldVal2)
        return;

    const int change = exp.first - oldVal1;
    if (change != 0)
    {
        battleChatLog(std::string("+").append(toString(
            change)).append(" job"),
            ChatMsgType::BY_SERVER);
    }
}

void ChatWindow::attributeChanged(const AttributesT id,
                                  const int oldVal,
                                  const int newVal)
{
    if (!mShowBattleEvents)
        return;
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (id)
    {
        case Attributes::PLAYER_EXP:
        {
            if (oldVal > newVal)
                break;
            const int change = newVal - oldVal;
            if (change != 0)
            {
                battleChatLog(std::string("+").append(toString(
                    change)).append(" xp"),
                    ChatMsgType::BY_SERVER);
            }
            break;
        }
        case Attributes::PLAYER_LEVEL:
            battleChatLog(std::string("Level: ").append(toString(newVal)),
                ChatMsgType::BY_SERVER);
            break;
        default:
            break;
    };
    PRAGMA45(GCC diagnostic pop)
}

void ChatWindow::addInputText(const std::string &text, const bool space)
{
    const int caretPos = mChatInput->getCaretPosition();
    const std::string &inputText = mChatInput->getText();

    std::ostringstream ss;
    ss << inputText.substr(0, caretPos) << text;
    if (space)
        ss << " ";

    ss << inputText.substr(caretPos);
    mChatInput->setText(ss.str());
    mChatInput->setCaretPosition(caretPos + CAST_S32(
        text.length()) + CAST_S32(space));
    requestChatFocus();
}

void ChatWindow::addItemText(const std::string &item)
{
    std::ostringstream text;
    text << "[" << item << "]";
    addInputText(text.str());
}

void ChatWindow::setVisible(Visible visible)
{
    Window::setVisible(visible);

    /*
     * For whatever reason, if setVisible is called, the mTmpVisible effect
     * should be disabled.
     */
    mTmpVisible = false;
}

void ChatWindow::addWhisper(const std::string &restrict nick,
                            const std::string &restrict mes,
                            const ChatMsgTypeT own)
{
    if (mes.empty() || !localPlayer)
        return;

    std::string playerName = localPlayer->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    if (tempNick.compare(playerName) == 0)
        return;

    WhisperTab *tab = nullptr;
    const TabMap::const_iterator i = mWhispers.find(tempNick);

    if (i != mWhispers.end())
    {
        tab = i->second;
    }
    else if (config.getBoolValue("whispertab"))
    {
        tab = addWhisperTab(nick, nick);
        if (tab)
            saveState();
    }

    if (tab)
    {
        if (own == ChatMsgType::BY_PLAYER)
        {
            tab->chatInput(mes);
        }
        else if (own == ChatMsgType::BY_SERVER)
        {
            tab->chatLog(mes, ChatMsgType::BY_SERVER);
        }
        else
        {
            if (tab->getRemoveNames())
            {
                std::string msg = mes;
                const size_t idx = mes.find(':');
                if (idx != std::string::npos && idx > 0)
                {
                    std::string nick2 = msg.substr(0, idx);
                    msg = msg.substr(idx + 1);
                    nick2 = removeColors(nick2);
                    nick2 = trim(nick2);
                    if (config.getBoolValue("removeColors"))
                        msg = removeColors(msg);
                    msg = trim(msg);
                    tab->chatLog(nick2, msg);
                }
                else
                {
                    if (config.getBoolValue("removeColors"))
                        msg = removeColors(msg);
                    tab->chatLog(msg, ChatMsgType::BY_SERVER);
                }
            }
            else
            {
                tab->chatLog(nick, mes);
            }
            localPlayer->afkRespond(tab, nick);
        }
    }
    else if (localChatTab)
    {
        if (own == ChatMsgType::BY_PLAYER)
        {
            chatHandler->privateMessage(nick, mes);

            // TRANSLATORS: chat message
            localChatTab->chatLog(strprintf(_("Whispering to %s: %s"),
                nick.c_str(), mes.c_str()),
                ChatMsgType::BY_PLAYER);
        }
        else
        {
            localChatTab->chatLog(std::string(nick).append(
                " : ").append(mes),
                ChatMsgType::ACT_WHISPER,
                IgnoreRecord_false);
            if (localPlayer)
                localPlayer->afkRespond(nullptr, nick);
        }
    }
}

WhisperTab *ChatWindow::addWhisperTab(const std::string &caption,
                                      const std::string &nick,
                                      const bool switchTo)
{
    if (!localPlayer)
        return nullptr;

    std::string playerName = localPlayer->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    const TabMap::const_iterator i = mWhispers.find(tempNick);
    WhisperTab *ret = nullptr;

    if (tempNick.compare(playerName) == 0)
        return nullptr;

    if (i != mWhispers.end())
    {
        ret = i->second;
    }
    else
    {
        ret = new WhisperTab(this, caption, nick);
        if (gui && !player_relations.isGoodName(nick))
            ret->setLabelFont(gui->getSecureFont());
        mWhispers[tempNick] = ret;
        if (config.getBoolValue("showChatHistory"))
            ret->loadFromLogFile(nick);
    }

    if (switchTo)
        mChatTabs->setSelectedTab(ret);

    return ret;
}

WhisperTab *ChatWindow::getWhisperTab(const std::string &nick) const
{
    if (!localPlayer)
        return nullptr;

    std::string playerName = localPlayer->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    const TabMap::const_iterator i = mWhispers.find(tempNick);
    WhisperTab *ret = nullptr;

    if (tempNick.compare(playerName) == 0)
        return nullptr;

    if (i != mWhispers.end())
        ret = i->second;

    return ret;
}

ChatTab *ChatWindow::addSpecialChannelTab(const std::string &name,
                                          const bool switchTo)
{
    ChatTab *ret = nullptr;
    if (name == TRADE_CHANNEL)
    {
        if (!tradeChatTab)
        {
            tradeChatTab = new TradeTab(chatWindow);
            tradeChatTab->setAllowHighlight(false);
            chatHandler->joinChannel(tradeChatTab->getChannelName());
        }
        ret = tradeChatTab;
    }
    else if (name == GM_CHANNEL)
    {
        if (!gmChatTab)
        {
            gmChatTab = new GmTab(chatWindow);
            chatHandler->joinChannel(gmChatTab->getChannelName());
        }
        ret = gmChatTab;
    }
    if (switchTo)
        mChatTabs->setSelectedTab(ret);

    return ret;
}

ChatTab *ChatWindow::addChannelTab(const std::string &name,
                                   const bool switchTo)
{
    std::string tempName = name;
    toLower(tempName);

    ChatTab *const tab = addSpecialChannelTab(name, switchTo);
    if (tab)
        return tab;

    const ChannelMap::const_iterator i = mChannels.find(tempName);
    ChannelTab *ret = nullptr;

    if (i != mChannels.end())
    {
        ret = i->second;
    }
    else
    {
        ret = new ChannelTab(this, name);
        mChannels[tempName] = ret;
        ret->setAllowHighlight(false);
        if (config.getBoolValue("showChatHistory"))
            ret->loadFromLogFile(name);
    }

    if (switchTo)
        mChatTabs->setSelectedTab(ret);

    return ret;
}

ChatTab *ChatWindow::addChatTab(const std::string &name,
                                const bool switchTo,
                                const bool join)
{
    if (serverFeatures->haveChatChannels() && name.size() > 1
        && name[0] == '#')
    {
        ChatTab *const tab = addChannelTab(name, switchTo);
        if (tab && join)
            chatHandler->joinChannel(name);
        return tab;
    }
    else
    {
        return addWhisperTab(name, name, switchTo);
    }
}

void ChatWindow::postConnection()
{
    FOR_EACH (ChannelMap::const_iterator, iter, mChannels)
    {
        ChatTab *const tab = iter->second;
        if (!tab)
            return;
        chatHandler->joinChannel(tab->getChannelName());
    }
    if (langChatTab)
        chatHandler->joinChannel(langChatTab->getChannelName());
}

#define changeColor(fun) \
    { \
        msg = removeColors(msg); \
        int skip = 0; \
        const size_t sz = msg.length(); \
        for (size_t f = 0; f < sz; f ++) \
        { \
            if (skip > 0) \
            { \
                newMsg += msg.at(f); \
                skip --; \
                continue; \
            } \
            const unsigned char ch = CAST_U8(msg.at(f)); \
            if (f + 2 < sz && msg.substr(f, 2) == "%%") \
            { \
                newMsg += msg.at(f); \
                skip = 2; \
            } \
            else if (ch > 0xc0 || ch < 0x80) \
            { \
                newMsg += "##" + toString(fun) + msg.at(f); \
                if (mRainbowColor > 9U) \
                    mRainbowColor = 0U; \
            } \
            else \
            { \
                newMsg += msg.at(f); \
            } \
        } \
    }

std::string ChatWindow::addColors(std::string &msg)
{
    // default color or chat command
    if (mChatColor == 0 || msg.length() == 0 || msg.at(0) == '#'
        || msg.at(0) == '/' || msg.at(0) == '@' || msg.at(0) == '!')
    {
        return msg;
    }

    std::string newMsg("");
    const int cMap[] = {1, 4, 5, 2, 3, 6, 7, 9, 0, 8};

    // rainbow
    switch (mChatColor)
    {
        case 11:
            changeColor(mRainbowColor++)
            return newMsg;
        case 12:
            changeColor(cMap[mRainbowColor++])
            return newMsg;
        case 13:
            changeColor(cMap[9-mRainbowColor++])
            return newMsg;
        default:
            break;
    }

    // simple colors
    return std::string("##").append(toString(mChatColor - 1)).append(msg);
}

#undef changeColor

void ChatWindow::autoComplete()
{
    const int caretPos = mChatInput->getCaretPosition();
    int startName = 0;
    const std::string &inputText = mChatInput->getText();
    std::string name = inputText.substr(0, caretPos);

    for (int f = caretPos - 1; f > -1; f --)
    {
        if (isWordSeparator(inputText[f]))
        {
            startName = f + 1;
            name = inputText.substr(f + 1, caretPos - f);
            break;
        }
    }

    if (caretPos - 1 + 1 == startName)
        return;

    const ChatTab *const cTab = static_cast<ChatTab*>(
        mChatTabs->getSelectedTab());
    StringVect nameList;

    if (cTab)
        cTab->getAutoCompleteList(nameList);
    std::string newName = autoComplete(nameList, name);
    if (!newName.empty() && !startName)
        secureChatCommand(newName);

    if (cTab && newName.empty())
    {
        cTab->getAutoCompleteCommands(nameList);
        newName = autoComplete(nameList, name);
    }

    if (newName.empty() && actorManager)
    {
        actorManager->getPlayerNames(nameList, NpcNames_true);
        newName = autoComplete(nameList, name);
        if (!newName.empty() && !startName)
            secureChatCommand(newName);
    }
    if (newName.empty())
        newName = autoCompleteHistory(name);
    if (newName.empty() && spellManager)
        newName = spellManager->autoComplete(name);
    if (newName.empty())
        newName = autoComplete(name, &mCommands);
    if (newName.empty() && actorManager)
    {
        actorManager->getMobNames(nameList);
        newName = autoComplete(nameList, name);
    }
    if (newName.empty())
        newName = autoComplete(name, &mCustomWords);
    if (newName.empty())
    {
        whoIsOnline->getPlayerNames(nameList);
        newName = autoComplete(nameList, name);
    }

    if (!newName.empty())
    {
        mChatInput->setText(inputText.substr(0, startName).append(newName)
            .append(inputText.substr(caretPos,
            inputText.length() - caretPos)));

        const int len = caretPos - CAST_S32(name.length())
            + CAST_S32(newName.length());

        if (startName > 0)
            mChatInput->setCaretPosition(len + 1);
        else
            mChatInput->setCaretPosition(len);
    }
}

std::string ChatWindow::autoComplete(const StringVect &names,
                                     std::string partName)
{
    StringVectCIter i = names.begin();
    const StringVectCIter i_end = names.end();
    toLower(partName);
    std::string newName;

    while (i != i_end)
    {
        if (!i->empty())
        {
            std::string name = *i;
            toLower(name);

            const size_t pos = name.find(partName, 0);
            if (pos == 0)
            {
                if (!newName.empty())
                    newName = findSameSubstringI(*i, newName);
                else
                    newName = *i;
            }
        }
        ++i;
    }

    return newName;
}

std::string ChatWindow::autoComplete(const std::string &partName,
                                     const History *const words) const
{
    if (!words)
        return "";

    ChatCommands::const_iterator i = words->begin();
    const ChatCommands::const_iterator i_end = words->end();
    StringVect nameList;

    while (i != i_end)
    {
        const std::string line = *i;
        if (line.find(partName, 0) == 0)
            nameList.push_back(line);
        ++i;
    }
    return autoComplete(nameList, partName);
}

/*
void ChatWindow::moveTabLeft(ChatTab *tab)
{
    mChatTabs->moveLeft(tab);
}

void ChatWindow::moveTabRight(ChatTab *tab)
{
    mChatTabs->moveRight(tab);
}
*/

std::string ChatWindow::autoCompleteHistory(const std::string &partName) const
{
    History::const_iterator i = mHistory.begin();
    const History::const_iterator i_end = mHistory.end();
    StringVect nameList;

    while (i != i_end)
    {
        std::string line = *i;
        unsigned int f = 0;
        while (f < line.length() && !isWordSeparator(line.at(f)))
            f++;

        line = line.substr(0, f);
        if (!line.empty())
            nameList.push_back(line);

        ++i;
    }
    return autoComplete(nameList, partName);
}

bool ChatWindow::resortChatLog(std::string line,
                               ChatMsgTypeT own,
                               const std::string &channel,
                               const IgnoreRecord ignoreRecord,
                               const TryRemoveColors tryRemoveColors)
{
    if (own == ChatMsgType::BY_UNKNOWN)
    {
        const size_t pos = line.find(" : ");
        if (pos != std::string::npos)
        {
            if (line.length() <= pos + 3)
                own = ChatMsgType::BY_SERVER;
            else if (line.substr(0, pos) == localPlayer->getName())
                own = ChatMsgType::BY_PLAYER;
            else
                own = ChatMsgType::BY_OTHER;
        }
        else
        {
            own = ChatMsgType::BY_SERVER;
        }
    }

    std::string prefix;
    if (!channel.empty())
    {
        prefix = std::string("##3").append(channel).append("##0");
    }
    else if (mEnableTradeFilter &&
             tradeChatTab &&
             findI(line, mTradeFilter) != std::string::npos)
    {
        // TRANSLATORS: prefix for moved message to trade tab.
        tradeChatTab->chatLog(std::string("##S") +  _("Moved: ") + line,
            own,
            ignoreRecord,
            tryRemoveColors);
        if (own == ChatMsgType::BY_PLAYER)
        {
            own = ChatMsgType::BY_SERVER;
            // TRANSLATORS: moved message to trade tab warning.
            line = _("Your message was moved to trade tab");
        }
        else
        {
            return false;
        }
    }

    size_t idx2 = line.find(": ");
    if (idx2 != std::string::npos)
    {
        std::string tmpNick = line.substr(0, idx2);
        if (tmpNick.find('#') != std::string::npos ||
            tmpNick.find(':') != std::string::npos ||
            tmpNick.find('%') != std::string::npos ||
            tmpNick.find('@') != std::string::npos ||
            tmpNick.size() < 5 ||
            tmpNick[0] == '@' ||
            tmpNick[0] == '/' ||
            tmpNick[0] == '!'
            )
        {
            replaceAll(tmpNick, "#", "_");
            replaceAll(tmpNick, "%", "_");
            // TRANSLATORS: error message
            line = _("Broken nick detected: ") + line;
            own = ChatMsgType::BY_SERVER;
        }
        const size_t idx = line.find(": \302\202");
        if (idx == idx2)
        {
            if (line.find(": \302\202\302") != std::string::npos)
            {
                if (line.find(": \302\202\302e") != std::string::npos)
                {
                    // Do nothing. Before was local pet emote
                }
                else if (line.find(": \302\202\302m") != std::string::npos)
                {
                    // Do nothing. Before was local pet move
                }
                else if (line.find(": \302\202\302d") != std::string::npos)
                {
                    // Do nothing. Before was local pet direction
                }
                else if (line.find(": \302\202\302a") != std::string::npos)
                {
                    // Do nothing. Before was local pet ai enable/disable
                }
                // ignore other special message formats.
                return false;
            }

            // pet talk message detected
            if (line.find(": \302\202\303 ") != std::string::npos)
            {
                // Do nothing. Before was local pet talk
                return false;
            }

            if (tradeChatTab)
            {
                line = line.erase(idx + 2, 2);
                tradeChatTab->chatLog(prefix + line,
                    own,
                    ignoreRecord,
                    tryRemoveColors);
            }
            else if (localChatTab)
            {
                line = line.erase(idx + 2, 2);
                localChatTab->chatLog(prefix + line,
                    own,
                    ignoreRecord,
                    tryRemoveColors);
            }
            return false;
        }
    }

    if (!channel.empty())
    {
        if (langChatTab)
        {
            if (langChatTab->getChannelName() == channel)
            {
                langChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
            }
            else if (mShowAllLang)
            {
                langChatTab->chatLog(prefix + line,
                    own,
                    ignoreRecord,
                    tryRemoveColors);
            }
        }
        else if (serverFeatures->haveChatChannels())
        {
            channelChatLog(channel, line, own, ignoreRecord, tryRemoveColors);
            return false;
        }
        else if (mShowAllLang)
        {
            localChatTab->chatLog(prefix + line,
                own,
                ignoreRecord,
                tryRemoveColors);
        }
    }
    else if (localChatTab && channel.empty())
    {
        localChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
    }
    return true;
}

void ChatWindow::battleChatLog(const std::string &line, ChatMsgTypeT own,
                               const IgnoreRecord ignoreRecord,
                               const TryRemoveColors tryRemoveColors)
{
    if (own == ChatMsgType::BY_UNKNOWN)
        own = ChatMsgType::BY_SERVER;
    if (battleChatTab)
        battleChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
    else if (debugChatTab)
        debugChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
}

void ChatWindow::channelChatLog(const std::string &channel,
                                const std::string &line,
                                ChatMsgTypeT own,
                                const IgnoreRecord ignoreRecord,
                                const TryRemoveColors tryRemoveColors)
{
    std::string tempChannel = channel;
    toLower(tempChannel);

    ChatTab *tab = nullptr;
    const ChannelMap::const_iterator i = mChannels.find(tempChannel);

    if (i != mChannels.end())
    {
        tab = i->second;
    }
    else
    {
        tab = addChannelTab(channel, false);
        if (tab)
            saveState();
    }

    if (tab)
        tab->chatLog(line, own, ignoreRecord, tryRemoveColors);
}

void ChatWindow::initTradeFilter()
{
    const std::string tradeListName = settings.serverConfigDir
        + "/tradefilter.txt";

    std::ifstream tradeFile;
    struct stat statbuf;

    if (!stat(tradeListName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        tradeFile.open(tradeListName.c_str(), std::ios::in);
        if (tradeFile.is_open())
        {
            char line[100];
            while (tradeFile.getline(line, 100))
            {
                const std::string str = line;
                if (!str.empty())
                    mTradeFilter.push_back(str);
            }
        }
        tradeFile.close();
    }
}

void ChatWindow::updateOnline(const std::set<std::string> &onlinePlayers) const
{
    const Party *party = nullptr;
    const Guild *guild = nullptr;
    if (localPlayer)
    {
        party = localPlayer->getParty();
        guild = localPlayer->getGuild();
    }
    FOR_EACH (TabMap::const_iterator, iter, mWhispers)
    {
        if (!iter->second)
            return;

        WhisperTab *const tab = static_cast<WhisperTab*>(iter->second);
        if (!tab)
            continue;

        if (onlinePlayers.find(tab->getNick()) != onlinePlayers.end())
        {
            tab->setWhisperTabColors();
        }
        else
        {
            const std::string &nick = tab->getNick();
            if (actorManager)
            {
                const Being *const being = actorManager->findBeingByName(
                    nick, ActorType::Player);
                if (being)
                {
                    tab->setWhisperTabColors();
                    continue;
                }
            }
            if (party)
            {
                const PartyMember *const pm = party->getMember(nick);
                if (pm && pm->getOnline())
                {
                    tab->setWhisperTabColors();
                    continue;
                }
            }
            if (guild)
            {
                const GuildMember *const gm = guild->getMember(nick);
                if (gm && gm->getOnline())
                {
                    tab->setWhisperTabColors();
                    continue;
                }
            }
            tab->setWhisperTabOfflineColors();
        }
    }
}

void ChatWindow::loadState()
{
    int num = 0;
    while (num < 50)
    {
        const std::string nick = serverConfig.getValue(
            "chatWhisper" + toString(num), "");

        if (nick.empty())
            break;

        const int flags = serverConfig.getValue(
            "chatWhisperFlags" + toString(num), 1);

        ChatTab *const tab = addChatTab(nick, false, false);
        if (tab)
        {
            tab->setAllowHighlight(flags & 1);
            tab->setRemoveNames((flags & 2) / 2);
            tab->setNoAway((flags & 4) / 4);
        }
        num ++;
    }
}

void ChatWindow::saveState() const
{
    int num = 0;
    for (ChannelMap::const_iterator iter = mChannels.begin(),
         iter_end = mChannels.end(); iter != iter_end && num < 50; ++iter)
    {
        if (!iter->second)
            return;
        if (!saveTab(num, iter->second))
            continue;
        num ++;
    }

    for (TabMap::const_iterator iter = mWhispers.begin(),
         iter_end = mWhispers.end(); iter != iter_end && num < 50; ++iter)
    {
        if (!iter->second)
            return;
        if (!saveTab(num, iter->second))
            continue;
        num ++;
    }

    while (num < 50)
    {
        serverConfig.deleteKey("chatWhisper" + toString(num));
        serverConfig.deleteKey("chatWhisperFlags" + toString(num));
        num ++;
    }
}

bool ChatWindow::saveTab(const int num,
                         const ChatTab *const tab) const
{
    if (!tab)
        return false;

    serverConfig.setValue("chatWhisper" + toString(num),
        tab->getChannelName());

    serverConfig.setValue("chatWhisperFlags" + toString(num),
        CAST_S32(tab->getAllowHighlight())
        + (2 * CAST_S32(tab->getRemoveNames()))
        + (4 * CAST_S32(tab->getNoAway())));

    return true;
}

std::string ChatWindow::doReplace(const std::string &msg)
{
    std::string str = msg;
    replaceSpecialChars(str);
    return str;
}

void ChatWindow::loadCustomList()
{
    std::ifstream listFile;
    struct stat statbuf;

    std::string listName = settings.serverConfigDir
        + "/customwords.txt";

    if (!stat(listName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        listFile.open(listName.c_str(), std::ios::in);
        if (listFile.is_open())
        {
            char line[101];
            while (listFile.getline(line, 100))
            {
                std::string str = line;
                if (!str.empty())
                    mCustomWords.push_back(str);
            }
        }
        listFile.close();
    }
}

void ChatWindow::addToAwayLog(const std::string &line)
{
    if (!settings.awayMode)
        return;

    if (mAwayLog.size() > 20)
        mAwayLog.pop_front();

    if (findI(line, mHighlights) != std::string::npos)
        mAwayLog.push_back("##aaway:" + line);
}

void ChatWindow::displayAwayLog() const
{
    if (!localChatTab)
        return;

    std::list<std::string>::const_iterator i = mAwayLog.begin();
    const std::list<std::string>::const_iterator i_end = mAwayLog.end();

    while (i != i_end)
    {
        std::string str = *i;
        localChatTab->addNewRow(str);
        ++i;
    }
}

void ChatWindow::parseHighlights()
{
    mHighlights.clear();
    if (!localPlayer)
        return;

    splitToStringVector(mHighlights, config.getStringValue(
        "highlightWords"), ',');

    mHighlights.push_back(localPlayer->getName());
}

void ChatWindow::parseGlobalsFilter()
{
    mGlobalsFilter.clear();
    if (!localPlayer)
        return;

    splitToStringVector(mGlobalsFilter, config.getStringValue(
        "globalsFilter"), ',');

    mHighlights.push_back(localPlayer->getName());
}

bool ChatWindow::findHighlight(const std::string &str)
{
    return findI(str, mHighlights) != std::string::npos;
}

void ChatWindow::copyToClipboard(const int x, const int y) const
{
    const ChatTab *const tab = getFocused();
    if (!tab)
        return;

    const BrowserBox *const text = tab->mTextOutput;
    if (!text)
        return;

    std::string str = text->getTextAtPos(x, y);
    sendBuffer(str);
}

void ChatWindow::optionChanged(const std::string &name)
{
    if (name == "autohideChat")
        mAutoHide = config.getBoolValue("autohideChat");
    else if (name == "showBattleEvents")
        mShowBattleEvents = config.getBoolValue("showBattleEvents");
    else if (name == "globalsFilter")
        parseGlobalsFilter();
    else if (name == "enableTradeFilter")
        mEnableTradeFilter = config.getBoolValue("enableTradeFilter");
}

void ChatWindow::mouseMoved(MouseEvent &event)
{
    mHaveMouse = true;
    Window::mouseMoved(event);
}

void ChatWindow::mouseEntered(MouseEvent& event)
{
    mHaveMouse = true;
    Window::mouseEntered(event);
}

void ChatWindow::mouseExited(MouseEvent& event)
{
    updateVisibility();
    Window::mouseExited(event);
}

void ChatWindow::draw(Graphics *const graphics)
{
    BLOCK_START("ChatWindow::draw")
    if (!mAutoHide || mHaveMouse)
    {
        GLDEBUG_START("ChatWindow::draw");
        Window::draw(graphics);
        GLDEBUG_END();
    }
    BLOCK_END("ChatWindow::draw")
}

void ChatWindow::safeDraw(Graphics *const graphics)
{
    BLOCK_START("ChatWindow::draw")
    if (!mAutoHide || mHaveMouse)
    {
        GLDEBUG_START("ChatWindow::draw");
        Window::safeDraw(graphics);
        GLDEBUG_END();
    }
    BLOCK_END("ChatWindow::draw")
}

void ChatWindow::updateVisibility()
{
    if (!gui)
        return;

    int mouseX = 0;
    int mouseY = 0;
    int x = 0;
    int y = 0;
    Gui::getMouseState(mouseX, mouseY);
    getAbsolutePosition(x, y);
    if (mChatInput->isVisible())
    {
        mHaveMouse = true;
    }
    else
    {
        mHaveMouse = mouseX >= x && mouseX <= x + getWidth()
            && mouseY >= y && mouseY <= y + getHeight();
    }
}

void ChatWindow::unHideWindow()
{
    mHaveMouse = true;
}

#ifdef USE_PROFILER
void ChatWindow::logicChildren()
{
    BLOCK_START("ChatWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("ChatWindow::logicChildren")
}
#endif  // USE_PROFILER

void ChatWindow::addGlobalMessage(const std::string &line)
{
    if (debugChatTab && findI(line, mGlobalsFilter) != std::string::npos)
        debugChatTab->chatLog(line, ChatMsgType::BY_OTHER);
    else
        localChatTab->chatLog(line, ChatMsgType::BY_GM);
}

bool ChatWindow::isTabPresent(const ChatTab *const tab) const
{
    return mChatTabs->isTabPresent(tab);
}

void ChatWindow::debugMessage(const std::string &msg)
{
    if (debugChatTab)
        debugChatTab->chatLog(msg, ChatMsgType::BY_SERVER);
}

void ChatWindow::showGMTab()
{
    if (!gmChatTab &&
        config.getBoolValue("enableGmTab") &&
        localPlayer->getGMLevel() >= paths.getIntValue("gmTabMinimalLevel"))
    {
        addSpecialChannelTab(GM_CHANNEL, false);
    }
}

void ChatWindow::toggleChatFocus()
{
    if (mChatInput->isVisible() && mChatInput->isFocused())
        mChatInput->processVisible(Visible_false);
    else
        requestChatFocus();
}

void ChatWindow::joinRoom(const bool isJoin)
{
    Tab *const tab = mChatTabs->getTabByIndex(0);
    if (tab)
    {
        std::string name;
        if (isJoin)
        {
            name = PlayerInfo::getRoomName();
        }
        else
        {
            // TRANSLATORS: chat tab name
            name = _("General");
        }
        tab->setCaption(name);
    }
}
