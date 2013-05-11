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

#include "gui/chatwindow.h"

#include "actorspritemanager.h"
#include "client.h"
#include "commands.h"
#include "configuration.h"
#include "game.h"
#include "guild.h"
#include "inputmanager.h"
#include "keyevent.h"
#include "localplayer.h"
#include "party.h"
#include "playerinfo.h"
#include "spellshortcut.h"
#include "soundmanager.h"

#include "gui/gui.h"
#include "gui/setup.h"
#include "gui/sdlfont.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/widgets/battletab.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/langtab.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/tradetab.h"
#include "gui/widgets/whispertab.h"

#include "net/chathandler.h"
#include "net/playerhandler.h"
#include "net/net.h"

#include "utils/copynpaste.h"
#include "utils/gettext.h"

#include "resources/resourcemanager.h"

#include <guichan/focushandler.hpp>
#include <guichan/focuslistener.hpp>

#include <sstream>

#include <sys/stat.h>

#include "debug.h"

/**
 * The chat input hides when it loses focus. It is also invisible by default.
 */
class ChatInput final : public TextField
{
    public:
        ChatInput(ChatWindow *const window, TabbedArea *const tabs):
            TextField(window, "", false),
            mWindow(window),
            mChatTabs(tabs),
            mFocusGaining(false)
        {
            setVisible(false);
            addFocusListener(this);
        }

        A_DELETE_COPY(ChatInput)

        /**
         * Called if the chat input loses focus. It will set itself to
         * invisible as result.
         */
        void focusLost(const gcn::Event &event)
        {
            TextField::focusLost(event);
            if (mFocusGaining || !config.getBoolValue("protectChatFocus"))
            {
                processVisible(false);
                if (chatWindow)
                    chatWindow->updateVisibility();
                mFocusGaining = false;
                return;
            }
            mFocusGaining = true;
            requestFocus();
            mFocusGaining = false;
        }

        void processVisible(const bool n)
        {
            if (!mWindow || isVisible() == n)
                return;

            if (!n)
                mFocusGaining = true;
            setVisible(n);
            if (config.getBoolValue("hideChatInput"))
                mWindow->adjustTabSize();
        }

        void unprotectFocus()
        { mFocusGaining = true; }

        void setVisible(bool visible)
        {
            TextField::setVisible(visible);
        }

    private:
        ChatWindow *mWindow;
        TabbedArea *mChatTabs;
        bool mFocusGaining;
};

const char *COLOR_NAME[14] =
{
    // TRANSLATORS: chat color
    N_("default"),
    // TRANSLATORS: chat color
    N_("black"),
    // TRANSLATORS: chat color
    N_("red"),
    // TRANSLATORS: chat color
    N_("green"),
    // TRANSLATORS: chat color
    N_("blue"),
    // TRANSLATORS: chat color
    N_("gold"),
    // TRANSLATORS: chat color
    N_("yellow"),
    // TRANSLATORS: chat color
    N_("pink"),
    // TRANSLATORS: chat color
    N_("purple"),
    // TRANSLATORS: chat color
    N_("grey"),
    // TRANSLATORS: chat color
    N_("brown"),
    // TRANSLATORS: chat color
    N_("rainbow 1"),
    // TRANSLATORS: chat color
    N_("rainbow 2"),
    // TRANSLATORS: chat color
    N_("rainbow 3"),
};


class ColorListModel final : public gcn::ListModel
{
public:
    virtual ~ColorListModel()
    { }

    virtual int getNumberOfElements()
    {
        return 14;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return "???";
        return gettext(COLOR_NAME[i]);
    }
};

static const char *const ACTION_COLOR_PICKER = "color picker";

ChatWindow::ChatWindow():
    // TRANSLATORS: chat window name
    Window(_("Chat"), false, nullptr, "chat.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mItemLinkHandler(new ItemLinkHandler),
    mChatTabs(new TabbedArea(this)),
    mChatInput(new ChatInput(this, mChatTabs)),
    mRainbowColor(0),
    mWhispers(),
    mHistory(),
    mCurHist(),
    mCommands(),
    mCustomWords(),
    mReturnToggles(config.getBoolValue("ReturnToggles")),
    mTradeFilter(),
    mColorListModel(new ColorListModel),
    mColorPicker(new DropDown(this, mColorListModel)),
    mChatColor(config.getIntValue("chatColor")),
    mChatHistoryIndex(0),
    mAwayLog(),
    mHighlights(),
    mGMLoaded(false),
    mHaveMouse(false),
    mAutoHide(config.getBoolValue("autohideChat")),
    mShowBattleEvents(config.getBoolValue("showBattleEvents")),
    mShowAllLang(serverConfig.getValue("showAllLang", 0)),
    mTmpVisible(false)
{
    listen(CHANNEL_ATTRIBUTES);

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
        setDefaultSize(w, 90, ImageRect::UPPER_LEFT, -110, -35);
    else
        setDefaultSize(w, 123, ImageRect::UPPER_LEFT, -110, -35);
#else
    if (mainGraphics->getWidth() < 600)
        w = mainGraphics->getWidth() - 10;
    if (w < 100)
        w = 100;
    setDefaultSize(w, 123, ImageRect::LOWER_LEFT);
#endif
    setMinWidth(150);
    setMinHeight(90);

    setTitleBarHeight(getPadding() + getTitlePadding());

    mChatTabs->enableScrollButtons(true);
    mChatTabs->setFollowDownScroll(true);

    mChatInput->setActionEventId("chatinput");
    mChatInput->addActionListener(this);

    mColorPicker->setActionEventId(ACTION_COLOR_PICKER);
    mColorPicker->addActionListener(this);
    mColorPicker->setSelected(mChatColor);

    add(mChatTabs);
    add(mChatInput);
    add(mColorPicker);

    loadWindowState();

    mColorPicker->setPosition(this->getWidth() - mColorPicker->getWidth()
        - 2 * mPadding - 8 - 16, mPadding);

    // Add key listener to chat input to be able to respond to up/down
    mChatInput->addKeyListener(this);
    mCurHist = mHistory.end();
    mColorPicker->setVisible(config.getBoolValue("showChatColorsList"));

    fillCommands();
    if (player_node && player_node->isGM())
        loadGMCommands();
    initTradeFilter();
    loadCustomList();
    parseHighlights();

    config.addListener("autohideChat", this);
    config.addListener("showBattleEvents", this);

    enableVisibleSound(true);
}

ChatWindow::~ChatWindow()
{
    config.removeListeners(this);
    saveState();
    config.setValue("ReturnToggles", mReturnToggles);
    removeAllWhispers();
    delete mItemLinkHandler;
    mItemLinkHandler = nullptr;
    delete mColorPicker;
    mColorPicker = nullptr;
    delete mColorListModel;
    mColorListModel = nullptr;
}

void ChatWindow::loadCommandsFile(const std::string &name)
{
    StringVect list;
    ResourceManager::loadTextFile(name, list);
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
}

void ChatWindow::loadGMCommands()
{
    if (mGMLoaded)
        return;

    loadCommandsFile("gmcommands.txt");
    mGMLoaded = true;
}

void ChatWindow::adjustTabSize()
{
    const gcn::Rectangle area = getChildrenArea();

    const int aw = area.width;
    const int ah = area.height;
    const int frame = mChatInput->getFrameSize();
    const int inputHeight = mChatInput->getHeight();
    const int frame2 = 2 * frame;
    const int awFrame2 = aw - frame2;
    mChatInput->setPosition(frame, ah - inputHeight - frame);
    mChatInput->setWidth(awFrame2);
    mChatTabs->setWidth(awFrame2);
    const int height = ah - frame2 - (inputHeight + frame2);
    if (mChatInput->isVisible() || !config.getBoolValue("hideChatInput"))
        mChatTabs->setHeight(height);
    else
        mChatTabs->setHeight(height + inputHeight);

    const ChatTab *const tab = getFocused();
    if (tab)
    {
        gcn::Widget *const content = tab->mScrollArea;
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
}

void ChatWindow::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    adjustTabSize();
}

ChatTab *ChatWindow::getFocused() const
{
    return static_cast<ChatTab*>(mChatTabs->getSelectedTab());
}

void ChatWindow::clearTab(ChatTab *const tab) const
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

    mChatTabs->setSelectedTabByPos(tab);
}

void ChatWindow::nextTab()
{
    if (!mChatTabs)
        return;

    int tab = mChatTabs->getSelectedTabIndex();

    tab++;
    if (tab == mChatTabs->getNumberOfTabs())
        tab = 0;

    mChatTabs->setSelectedTabByPos(tab);
}

void ChatWindow::closeTab() const
{
    if (!mChatTabs)
        return;

    Tab *const tab = mChatTabs->getTabByIndex(
        mChatTabs->getSelectedTabIndex());
    if (!tab)
        return;
    WhisperTab *const whisper = dynamic_cast<WhisperTab* const>(tab);
    if (!whisper)
        return;

    whisper->handleCommand("close", "");
}

void ChatWindow::defaultTab()
{
    if (mChatTabs)
        mChatTabs->setSelectedTabByPos(static_cast<unsigned>(0));
}

void ChatWindow::action(const gcn::ActionEvent &event)
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
                setVisible(false);
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

    if (mColorPicker && mColorPicker->isVisible()
        != config.getBoolValue("showChatColorsList"))
    {
        mColorPicker->setVisible(config.getBoolValue(
            "showChatColorsList"));
    }
}

bool ChatWindow::requestChatFocus()
{
    // Make sure chatWindow is visible
    if (!isWindowVisible())
    {
        setVisible(true);

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
    mChatInput->processVisible(true);
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

void ChatWindow::ignoreAllWhispers()
{
    for (TabMap::iterator iter = mWhispers.begin();
         iter != mWhispers.end();
         ++ iter)
    {
        const WhisperTab *const tab = dynamic_cast<const WhisperTab* const>(
            iter->second);
        if (tab && player_relations.getRelation(tab->getNick())
            != PlayerRelation::IGNORED)
        {
            player_relations.setRelation(tab->getNick(),
                                         PlayerRelation::IGNORED);
        }

        delete (iter->second);
        iter->second = nullptr;
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
    }
    if (tab)
        tab->chatInput(msg);
    Game::instance()->setValidSpeed();
}

void ChatWindow::localChatInput(const std::string &msg)
{
    if (localChatTab)
        localChatTab->chatInput(msg);
    else
        chatInput(msg);
}

void ChatWindow::doPresent() const
{
    if (!actorSpriteManager)
        return;

    const ActorSprites &actors = actorSpriteManager->getAll();
    std::string response;
    int playercount = 0;

    FOR_EACH (ActorSpritesConstIterator, it, actors)
    {
        if ((*it)->getType() == ActorSprite::PLAYER)
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
        getFocused()->chatLog(log, BY_SERVER);
}

void ChatWindow::scroll(const int amount) const
{
    if (!isWindowVisible())
        return;

    ChatTab *const tab = getFocused();
    if (tab)
        tab->scroll(amount);
}

void ChatWindow::mousePressed(gcn::MouseEvent &event)
{
    if (event.isConsumed())
        return;

    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        if (viewport)
        {
            Tab *const tab = mChatTabs->getSelectedTab();
            if (tab)
            {
                if (inputManager.isActionActive(static_cast<int>(
                    Input::KEY_CHAT_MOD)))
                {
                    ChatTab *const wTab = dynamic_cast<WhisperTab*>(tab);
                    if (wTab)
                        wTab->handleCommand("close", "");
                }
                else
                {
                    ChatTab *const cTab = dynamic_cast<ChatTab*>(tab);
                    if (cTab)
                        viewport->showChatPopup(cTab);
                }
            }
        }
    }

    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const ChatTab *const tab = getFocused();
        if (tab)
            mMoved = !isResizeAllowed(event);
    }

    mDragOffsetX = event.getX();
    mDragOffsetY = event.getY();
}

void ChatWindow::mouseDragged(gcn::MouseEvent &event)
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

#define caseKey(key, str) case key:\
    temp = str; \
    break

void ChatWindow::keyPressed(gcn::KeyEvent &event)
{
    const int key = event.getKey().getValue();
    const int actionId = static_cast<KeyEvent*>(&event)->getActionId();
    if (actionId == static_cast<int>(Input::KEY_GUI_DOWN))
    {
        if (mCurHist != mHistory.end())
        {
            // Move forward through the history
            const HistoryIterator prevHist = mCurHist++;

            if (mCurHist != mHistory.end())
            {
                mChatInput->setText(*mCurHist);
                mChatInput->setCaretPosition(static_cast<unsigned>(
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
            mChatInput->setText("");
        }
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_UP) &&
             mCurHist != mHistory.begin() && !mHistory.empty())
    {
        // Move backward through the history
        --mCurHist;
        mChatInput->setText(*mCurHist);
        mChatInput->setCaretPosition(static_cast<unsigned>(
                mChatInput->getText().length()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_INSERT) &&
             mChatInput->getText() != "")
    {
        // Add the current message to the history and clear the text
        if (mHistory.empty() || mChatInput->getText() != mHistory.back())
            mHistory.push_back(mChatInput->getText());
        mCurHist = mHistory.end();
        mChatInput->setText("");
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_TAB) &&
             !mChatInput->getText().empty())
    {
        autoComplete();
        return;
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL) &&
             mChatInput->isVisible())
    {
        mChatInput->processVisible(false);
    }
    else if (actionId == static_cast<int>(Input::KEY_CHAT_PREV_HISTORY) &&
             mChatInput->isVisible())
    {
        const ChatTab *const tab = getFocused();
        if (tab && tab->hasRows())
        {
            if (!mChatHistoryIndex)
            {
                mChatHistoryIndex = static_cast<unsigned>(
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
            mChatInput->setCaretPosition(static_cast<unsigned>(
                mChatInput->getText().length()));
        }
    }
    else if (actionId == static_cast<int>(Input::KEY_CHAT_NEXT_HISTORY) &&
             mChatInput->isVisible())
    {
        ChatTab *const tab = getFocused();
        if (tab && tab->hasRows())
        {
            const size_t &tabSize = tab->getRows().size();
            if (mChatHistoryIndex + 1 < tabSize)
            {
                mChatHistoryIndex ++;
            }
            else if (mChatHistoryIndex < tabSize)
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
                 it = tab->getRows().begin(), it_end = tab->getRows().end();
                 it != it_end; ++it, f++)
            {
                if (f == mChatHistoryIndex)
                    mChatInput->setText(*it);
            }
            mChatInput->setCaretPosition(static_cast<unsigned>(
                    mChatInput->getText().length()));
        }
    }

    std::string temp;
    switch (key)
    {
        caseKey(Key::F2, "\u2318");
        caseKey(Key::F3, "\u263A");
        caseKey(Key::F4, "\u2665");
        caseKey(Key::F5, "\u266A");
        caseKey(Key::F6, "\u266B");
        caseKey(Key::F7, "\u26A0");
        caseKey(Key::F8, "\u2622");
        caseKey(Key::F9, "\u262E");
        caseKey(Key::F10, "\u2605");
        caseKey(Key::F11, "\u2618");
        caseKey(Key::F12, "\u2592");
        default:
            break;
    }

    if (!temp.empty())
        addInputText(temp, false);
}

void ChatWindow::processEvent(Channels channel, const DepricatedEvent &event)
{
    if (channel == CHANNEL_ATTRIBUTES)
    {
        if (!mShowBattleEvents)
            return;

        if (event.getName() == EVENT_UPDATEATTRIBUTE)
        {
            switch (event.getInt("id"))
            {
                case PlayerInfo::EXP:
                {
                    if (event.getInt("oldValue") > event.getInt("newValue"))
                        break;

                    const int change = event.getInt("newValue")
                        - event.getInt("oldValue");

                    if (change != 0)
                    {
                        battleChatLog(std::string("+").append(toString(
                            change)).append(" xp"));
                    }
                    break;
                }
                case PlayerInfo::LEVEL:
                    battleChatLog(std::string("Level: ").append(toString(
                        event.getInt("newValue"))));
                    break;
                default:
                    break;
            };
        }
        else if (event.getName() == EVENT_UPDATESTAT)
        {
            if (!config.getBoolValue("showJobExp"))
                return;

            const int id = event.getInt("id");
            if (id == Net::getPlayerHandler()->getJobLocation())
            {
                const std::pair<int, int> exp
                    = PlayerInfo::getStatExperience(id);
                if (event.getInt("oldValue1") > exp.first
                    || !event.getInt("oldValue2"))
                {
                    return;
                }

                const int change = exp.first - event.getInt("oldValue1");
                if (change != 0)
                {
                    battleChatLog(std::string("+").append(
                        toString(change)).append(" job"));
                }
            }
        }
    }
}

void ChatWindow::addInputText(const std::string &text, const bool space)
{
    const int caretPos = mChatInput->getCaretPosition();
    const std::string inputText = mChatInput->getText();

    std::ostringstream ss;
    ss << inputText.substr(0, caretPos) << text;
    if (space)
        ss << " ";

    ss << inputText.substr(caretPos);
    mChatInput->setText(ss.str());
    mChatInput->setCaretPosition(caretPos + static_cast<int>(
        text.length()) + static_cast<int>(space));
    requestChatFocus();
}

void ChatWindow::addItemText(const std::string &item)
{
    std::ostringstream text;
    text << "[" << item << "]";
    addInputText(text.str());
}

void ChatWindow::setVisible(bool visible)
{
    Window::setVisible(visible);

    /*
     * For whatever reason, if setVisible is called, the mTmpVisible effect
     * should be disabled.
     */
    mTmpVisible = false;
}

void ChatWindow::addWhisper(const std::string &nick,
                            const std::string &mes, const Own own)
{
    if (mes.empty() || !player_node)
        return;

    std::string playerName = player_node->getName();
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
        tab = addWhisperTab(nick);
        if (tab)
            saveState();
    }

    if (tab)
    {
        if (own == BY_PLAYER)
        {
            tab->chatInput(mes);
        }
        else if (own == BY_SERVER)
        {
            tab->chatLog(mes);
        }
        else
        {
            if (tab->getRemoveNames())
            {
                std::string msg = mes;
                const size_t idx = mes.find(":");
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
                    tab->chatLog(msg, BY_SERVER);
                }
            }
            else
            {
                tab->chatLog(nick, mes);
            }
            player_node->afkRespond(tab, nick);
        }
    }
    else if (localChatTab)
    {
        if (own == BY_PLAYER)
        {
            Net::getChatHandler()->privateMessage(nick, mes);

            // TRANSLATORS: chat message
            localChatTab->chatLog(strprintf(_("Whispering to %s: %s"),
                nick.c_str(), mes.c_str()), BY_PLAYER);
        }
        else
        {
            localChatTab->chatLog(std::string(nick).append(
                " : ").append(mes), ACT_WHISPER, false);
            if (player_node)
                player_node->afkRespond(nullptr, nick);
        }
    }
}

WhisperTab *ChatWindow::addWhisperTab(const std::string &nick,
                                      const bool switchTo)
{
    if (!player_node)
        return nullptr;

    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    const TabMap::const_iterator i = mWhispers.find(tempNick);
    WhisperTab *ret;

    if (tempNick.compare(playerName) == 0)
        return nullptr;

    if (i != mWhispers.end())
    {
        ret = i->second;
    }
    else
    {
        ret = new WhisperTab(this, nick);
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
    if (!player_node)
        return nullptr;

    std::string playerName = player_node->getName();
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
            msg = removeColors(msg);
            for (unsigned int f = 0; f < msg.length(); f ++)
            {
                newMsg += "##" + toString(mRainbowColor++) + msg.at(f);
                if (mRainbowColor > 9)
                    mRainbowColor = 0;
            }
            return newMsg;
        case 12:
            msg = removeColors(msg);
            for (unsigned int f = 0; f < msg.length(); f ++)
            {
                newMsg += "##" + toString(cMap[mRainbowColor++]) + msg.at(f);
                if (mRainbowColor > 9)
                    mRainbowColor = 0;
            }
            return newMsg;
        case 13:
            msg = removeColors(msg);
            for (unsigned int f = 0; f < msg.length(); f ++)
            {
                newMsg += "##" + toString(cMap[9-mRainbowColor++]) + msg.at(f);
                if (mRainbowColor > 9)
                    mRainbowColor = 0;
            }
            return newMsg;
        default:
            break;
    }

    // simple colors
    return std::string("##").append(toString(mChatColor - 1)).append(msg);
}

void ChatWindow::autoComplete()
{
    const int caretPos = mChatInput->getCaretPosition();
    int startName = 0;
    const std::string inputText = mChatInput->getText();
    bool needSecure(false);
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
    if (!newName.empty())
        needSecure = true;

    if (newName.empty() && actorSpriteManager)
    {
        actorSpriteManager->getPlayerNames(nameList, true);
        newName = autoComplete(nameList, name);
        if (!newName.empty())
            needSecure = true;
    }
    if (newName.empty())
        newName = autoCompleteHistory(name);
    if (newName.empty() && spellManager)
        newName = spellManager->autoComplete(name);
    if (newName.empty())
        newName = autoComplete(name, &mCommands);
    if (newName.empty() && actorSpriteManager)
    {
        actorSpriteManager->getMobNames(nameList);
        newName = autoComplete(nameList, name);
    }
    if (newName.empty())
        newName = autoComplete(name, &mCustomWords);

    if (!newName.empty())
    {
        if (!startName && needSecure && (newName[0] == '/'
            || newName[0] == '@' || newName[0] == '#'))
        {
            newName = "_" + newName;
        }
        mChatInput->setText(inputText.substr(0, startName).append(newName)
            .append(inputText.substr(caretPos,
            inputText.length() - caretPos)));

        const int len = caretPos - static_cast<int>(name.length())
            + static_cast<int>(newName.length());

        if (startName > 0)
            mChatInput->setCaretPosition(len + 1);
        else
            mChatInput->setCaretPosition(len);
    }
}

std::string ChatWindow::autoComplete(StringVect &names,
                                     std::string partName) const
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
                                     History *const words) const
{
    if (!words)
        return "";

    Commands::const_iterator i = words->begin();
    const Commands::const_iterator i_end = words->end();
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
        if (line != "")
            nameList.push_back(line);

        ++i;
    }
    return autoComplete(nameList, partName);
}

void ChatWindow::resortChatLog(std::string line, Own own,
                               const std::string &channel,
                               const bool ignoreRecord,
                               const bool tryRemoveColors)
{
    if (own == -1)
        own = BY_SERVER;

    std::string prefix;
    if (!channel.empty())
        prefix = std::string("##3").append(channel).append("##0");

    if (tradeChatTab)
    {
        if (findI(line, mTradeFilter) != std::string::npos)
        {
            tradeChatTab->chatLog(prefix + line, own,
                ignoreRecord, tryRemoveColors);
            return;
        }

        size_t idx2 = line.find(": ");
        if (idx2 != std::string::npos)
        {
            const size_t idx = line.find(": \302\202");
            if (idx == idx2)
            {
                // ignore special message formats.
                if (line.find(": \302\202\302") != std::string::npos)
                    return;
                line = line.erase(idx + 2, 2);
                tradeChatTab->chatLog(prefix + line, own, ignoreRecord,
                    tryRemoveColors);
                return;
            }
        }

        const size_t idx1 = line.find("@@");
        if (idx1 != std::string::npos)
        {
            idx2 = line.find("|", idx1);
            if (idx2 != std::string::npos)
            {
                const size_t idx3 = line.find("@@", idx2);
                if (idx3 != std::string::npos)
                {
                    if (line.find("http", idx1) != idx1 + 2)
                    {
                        tradeChatTab->chatLog(prefix + line, own,
                            ignoreRecord, tryRemoveColors);
                        return;
                    }
                }
            }
        }
    }

    if (langChatTab && !channel.empty())
    {
        if (langChatTab->getChannelName() == channel)
        {
            langChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
        }
        else if (mShowAllLang)
        {
            if (langChatTab)
            {
                langChatTab->chatLog(prefix + line, own,
                    ignoreRecord, tryRemoveColors);
            }
            else if (localChatTab)
            {
                localChatTab->chatLog(prefix + line, own,
                    ignoreRecord, tryRemoveColors);
            }
        }
    }
    else if (localChatTab && channel.empty())
    {
        localChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
    }
}

void ChatWindow::battleChatLog(const std::string &line, Own own,
                               const bool ignoreRecord,
                               const bool tryRemoveColors)
{
    if (own == -1)
        own = BY_SERVER;
    if (battleChatTab)
        battleChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
    else if (debugChatTab)
        debugChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
}

void ChatWindow::initTradeFilter()
{
    const std::string tradeListName = Client::getServerConfigDirectory()
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

void ChatWindow::updateOnline(std::set<std::string> &onlinePlayers) const
{
    const Party *party = nullptr;
    const Guild *guild = nullptr;
    if (player_node)
    {
        party = player_node->getParty();
        guild = player_node->getGuild();
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
            const std::string nick = tab->getNick();
            if (actorSpriteManager)
            {
                const Being *const being = actorSpriteManager->findBeingByName(
                    nick, ActorSprite::PLAYER);
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

        ChatTab *const tab = addWhisperTab(nick);
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
    for (TabMap::const_iterator iter = mWhispers.begin(),
         iter_end = mWhispers.end(); iter != iter_end && num < 50; ++iter)
    {
        if (!iter->second)
            return;

        const WhisperTab *const tab = static_cast<WhisperTab*>(iter->second);

        if (!tab)
            continue;

        serverConfig.setValue("chatWhisper" + toString(num),
            tab->getNick());

        serverConfig.setValue("chatWhisperFlags" + toString(num),
            static_cast<int>(tab->getAllowHighlight())
            + (2 * static_cast<int>(tab->getRemoveNames()))
            + (4 * static_cast<int>(tab->getNoAway())));

        num ++;
    }

    while (num < 50)
    {
        serverConfig.deleteKey("chatWhisper" + toString(num));
        serverConfig.deleteKey("chatWhisperFlags" + toString(num));
        num ++;
    }
}

std::string ChatWindow::doReplace(const std::string &msg) const
{
    std::string str = msg;
    replaceSpecialChars(str);
    return str;
}

void ChatWindow::loadCustomList()
{
    std::ifstream listFile;
    struct stat statbuf;

    std::string listName = Client::getServerConfigDirectory()
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
    if (!player_node || !player_node->getAway())
        return;

    if (mAwayLog.size() > 20)
        mAwayLog.pop_front();

    if (findI(line, mHighlights) != std::string::npos)
        mAwayLog.push_back("##9away:" + line);
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
    if (!player_node)
        return;

    splitToStringVector(mHighlights, config.getStringValue(
        "highlightWords"), ',');

    mHighlights.push_back(player_node->getName());
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
}

void ChatWindow::mouseMoved(gcn::MouseEvent &event)
{
    mHaveMouse = true;
    Window::mouseMoved(event);
}

void ChatWindow::mouseEntered(gcn::MouseEvent& mouseEvent)
{
    mHaveMouse = true;
    Window::mouseEntered(mouseEvent);
}

void ChatWindow::mouseExited(gcn::MouseEvent& mouseEvent)
{
    updateVisibility();
    Window::mouseExited(mouseEvent);
}

void ChatWindow::draw(gcn::Graphics* graphics)
{
    BLOCK_START("ChatWindow::draw")
    if (!mAutoHide || mHaveMouse)
        Window::draw(graphics);
    BLOCK_END("ChatWindow::draw")
}

void ChatWindow::updateVisibility()
{
    int mouseX = 0;
    int mouseY = 0;
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
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
