/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "chatwindow.h"

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "guild.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "party.h"
#include "playerinfo.h"
#include "playerrelations.h"
#include "spellshortcut.h"
#include "sound.h"

#include "gui/setup.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/battletab.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/tradetab.h"
#include "gui/widgets/whispertab.h"

#include "net/chathandler.h"
#include "net/playerhandler.h"
#include "net/net.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/focushandler.hpp>
#include <guichan/focuslistener.hpp>

#include <sstream>

#include <sys/stat.h>

/**
 * The chat input hides when it loses focus. It is also invisible by default.
 */
class ChatInput : public TextField, public gcn::FocusListener
{
    public:
        ChatInput(ChatWindow *window, TabbedArea *tabs):
            TextField("", false),
            mWindow(window),
            mChatTabs(tabs)
        {
            setVisible(false);
            addFocusListener(this);
        }

        /**
         * Called if the chat input loses focus. It will set itself to
         * invisible as result.
         */
        void focusLost(const gcn::Event &event _UNUSED_)
        {
            processVisible(false);
        }

        void processVisible(bool n)
        {
            if (!mWindow || isVisible() == n)
                return;

            setVisible(n);
            if (config.getBoolValue("hideChatInput"))
                mWindow->adjustTabSize();
        }

    private:
        ChatWindow *mWindow;
        TabbedArea *mChatTabs;
};

const char *COLOR_NAME[14] =
{
    N_("default"),
    N_("black"),
    N_("red"),
    N_("green"),
    N_("blue"),
    N_("gold"),
    N_("yellow"),
    N_("pink"),
    N_("purple"),
    N_("grey"),
    N_("brown"),
    N_("rainbow 1"),
    N_("rainbow 2"),
    N_("rainbow 3"),
};


class ColorListModel : public gcn::ListModel
{
public:
    virtual ~ColorListModel() { }

    virtual int getNumberOfElements()
    {
        return 14;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(COLOR_NAME[i]);
    }
};

#define ACTION_COLOR_PICKER "color picker"


ChatWindow::ChatWindow():
    Window(_("Chat")),
    mTmpVisible(false),
    mChatHistoryIndex(0)
{
    listen(CHANNEL_NOTICES);
    listen(Mana::CHANNEL_ATTRIBUTES);

    setWindowName("Chat");

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    // no title presented, title bar is padding so window can be moved.
    gcn::Window::setTitleBarHeight(gcn::Window::getPadding() + 4);
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(true);
    setSaveVisible(true);
    setDefaultSize(600, 123, ImageRect::LOWER_LEFT);
    setMinWidth(150);
    setMinHeight(90);

    mItemLinkHandler = new ItemLinkHandler;

    mChatTabs = new TabbedArea;
    mChatTabs->enableScrollButtons(true);
    mChatTabs->setFollowDownScroll(true);

    mChatInput = new ChatInput(this, mChatTabs);
    mChatInput->setActionEventId("chatinput");
    mChatInput->addActionListener(this);

    mChatColor = config.getIntValue("chatColor");
    mColorListModel = new ColorListModel;
    mColorPicker = new DropDown(mColorListModel);

    mColorPicker->setActionEventId(ACTION_COLOR_PICKER);
    mColorPicker->addActionListener(this);
    mColorPicker->setSelected(mChatColor);

    add(mChatTabs);
    add(mChatInput);
    add(mColorPicker);

    loadWindowState();

    mColorPicker->setPosition(this->getWidth() - mColorPicker->getWidth()
                              - 2*getPadding() - 8 - 16, getPadding());

    // Add key listener to chat input to be able to respond to up/down
    mChatInput->addKeyListener(this);
    mCurHist = mHistory.end();

    mReturnToggles = config.getBoolValue("ReturnToggles");

    mRainbowColor = 0;

    mColorPicker->setVisible(config.getBoolValue("showChatColorsList"));

    fillCommands();
    initTradeFilter();
    loadCustomList();
}

ChatWindow::~ChatWindow()
{
    config.setValue("ReturnToggles", mReturnToggles);
    removeAllWhispers();
    delete mItemLinkHandler;
    mItemLinkHandler = 0;
    delete mColorPicker;
    mColorPicker = 0;
    delete mColorListModel;
    mColorListModel = 0;
}

void ChatWindow::fillCommands()
{
    mCommands.push_back("/all");
    mCommands.push_back("/away ");
    mCommands.push_back("/closeall");
    mCommands.push_back("/clear");
    mCommands.push_back("/create ");
    mCommands.push_back("/close");
    mCommands.push_back("/cacheinfo");
    mCommands.push_back("/erase ");
    mCommands.push_back("/follow ");
    mCommands.push_back("/heal ");
    mCommands.push_back("/ignoreall");
    mCommands.push_back("/help");
    mCommands.push_back("/announce ");
    mCommands.push_back("/where");
    mCommands.push_back("/who");
    mCommands.push_back("/msg ");
    mCommands.push_back("/mail ");
    mCommands.push_back("/whisper ");
    mCommands.push_back("/w ");
    mCommands.push_back("/query ");
    mCommands.push_back("/ignore ");
    mCommands.push_back("/unignore ");
    mCommands.push_back("/join ");
    mCommands.push_back("/list");
    mCommands.push_back("/party");
    mCommands.push_back("/createparty ");
    mCommands.push_back("/createguild ");
    mCommands.push_back("/me ");
    mCommands.push_back("/toggle");
    mCommands.push_back("/present");
    mCommands.push_back("/quit");
    mCommands.push_back("/move ");
    mCommands.push_back("/target ");
    mCommands.push_back("/invite ");
    mCommands.push_back("/leave");
    mCommands.push_back("/kick ");
    mCommands.push_back("/item");
    mCommands.push_back("/imitation");
    mCommands.push_back("/exp");
    mCommands.push_back("/ping");
    mCommands.push_back("/outfit ");
    mCommands.push_back("/emote ");
    mCommands.push_back("/navigate ");
    mCommands.push_back("/priceload");
    mCommands.push_back("/pricesave");
    mCommands.push_back("/trade ");
    mCommands.push_back("/friend ");
    mCommands.push_back("/befriend ");
    mCommands.push_back("/disregard ");
    mCommands.push_back("/neutral ");
    mCommands.push_back("/raw ");
    mCommands.push_back("/disconnect");
    mCommands.push_back("/undress ");
    mCommands.push_back("/attack");
    mCommands.push_back("/dirs");
    mCommands.push_back("/info");
    mCommands.push_back("/wait");
    mCommands.push_back("/uptime");
    mCommands.push_back("/addattack ");
    mCommands.push_back("/addpriorityattack ");
    mCommands.push_back("/removeattack ");
    mCommands.push_back("/addignoreattack ");
}

void ChatWindow::resetToDefaultSize()
{
    Window::resetToDefaultSize();
}

void ChatWindow::adjustTabSize()
{
    const gcn::Rectangle area = getChildrenArea();

    mChatInput->setPosition(mChatInput->getFrameSize(),
                            area.height - mChatInput->getHeight() -
                            mChatInput->getFrameSize());
    mChatInput->setWidth(area.width - 2 * mChatInput->getFrameSize());

    mChatTabs->setWidth(area.width - 2 * mChatTabs->getFrameSize());
    int height = area.height - 2 * mChatTabs->getFrameSize() -
        (mChatInput->getHeight() + mChatInput->getFrameSize() * 2);
    if (mChatInput->isVisible() || !config.getBoolValue("hideChatInput"))
        mChatTabs->setHeight(height);
    else
        mChatTabs->setHeight(height + mChatInput->getHeight());

    ChatTab *tab = getFocused();
    if (tab)
    {
        gcn::Widget *content = tab->mScrollArea;
        if (content)
        {
            content->setSize(mChatTabs->getWidth()
                - 2 * content->getFrameSize(),
                mChatTabs->getContainerHeight()
                - 2 * content->getFrameSize());
            content->logic();
        }
    }

    mColorPicker->setPosition(this->getWidth() - mColorPicker->getWidth()
        - 2*getPadding() - 8 - 16, getPadding());

//    if (mColorPicker->isVisible())
//        mChatTabs->setRightMargin(mColorPicker->getWidth() - 8);
}

void ChatWindow::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

    adjustTabSize();
}

/*
void ChatWindow::logic()
{
    Window::logic();

    Tab *tab = getFocused();
    if (tab != mCurrentTab)
        mCurrentTab = tab;
}
*/

ChatTab *ChatWindow::getFocused() const
{
    return static_cast<ChatTab*>(mChatTabs->getSelectedTab());
}

void ChatWindow::clearTab(ChatTab *tab)
{
    if (tab)
        tab->clearText();
}

void ChatWindow::clearTab()
{
    clearTab(getFocused());
}

void ChatWindow::prevTab()
{
    if (!mChatTabs)
        return;

    int tab = mChatTabs->getSelectedTabIndex();

    if (tab == 0)
        tab = mChatTabs->getNumberOfTabs();
    tab--;

    mChatTabs->setSelectedTab(tab);
}

void ChatWindow::nextTab()
{
    if (!mChatTabs)
        return;

    int tab = mChatTabs->getSelectedTabIndex();

    tab++;
    if (tab == mChatTabs->getNumberOfTabs())
        tab = 0;

    mChatTabs->setSelectedTab(tab);
}

void ChatWindow::defaultTab()
{
    if (mChatTabs)
        mChatTabs->setSelectedTab(static_cast<unsigned>(0));
}

void ChatWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "chatinput")
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
            if (mFocusHandler)
                mFocusHandler->focusNone();

            // If the chatWindow is shown up because you want to send a message
            // It should hide now
            if (mTmpVisible)
                setVisible(false);
        }
    }
    else if (event.getId() == ACTION_COLOR_PICKER)
    {
        mChatColor = mColorPicker->getSelected();
        config.setValue("chatColor", mChatColor);
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
    if (!isVisible())
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
    mChatInput->requestFocus();
    return true;
}

bool ChatWindow::isInputFocused() const
{
    return mChatInput->isFocused();
}

void ChatWindow::removeTab(ChatTab *tab)
{
    mChatTabs->removeTab(tab);
}

void ChatWindow::addTab(ChatTab *tab)
{
    mChatTabs->addTab(tab, tab->mScrollArea);

    // Update UI
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
    TabMap::iterator iter;
    std::list<ChatTab*> tabs;

    for (iter = mWhispers.begin(); iter != mWhispers.end(); ++iter)
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
    TabMap::iterator iter;
    for (iter = mWhispers.begin(); iter != mWhispers.end(); ++iter)
    {
        WhisperTab *tab = dynamic_cast<WhisperTab*>(iter->second);
        if (tab && player_relations.getRelation(tab->getNick())
            != PlayerRelation::IGNORED)
        {
            player_relations.setRelation(tab->getNick(),
                                         PlayerRelation::IGNORED);
        }

        delete(iter->second);
        iter->second = 0;
    }
}

void ChatWindow::chatInput(const std::string &message)
{
    ChatTab *tab = NULL;
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

void ChatWindow::doPresent()
{
    if (!actorSpriteManager)
        return;

    const ActorSprites &actors = actorSpriteManager->getAll();
    std::string response = "";
    int playercount = 0;

    for (ActorSpritesConstIterator it = actors.begin(), it_end = actors.end();
         it != it_end; ++it)
    {
        if ((*it)->getType() == ActorSprite::PLAYER)
        {
            if (!response.empty())
                response += ", ";
            response += static_cast<Being*>(*it)->getName();
            ++playercount;
        }
    }

    std::string log = strprintf(_("Present: %s; %d players are present."),
                                response.c_str(), playercount);

    if (getFocused())
        getFocused()->chatLog(log, BY_SERVER);
}

void ChatWindow::scroll(int amount)
{
    if (!isVisible())
        return;

    ChatTab *tab = getFocused();
    if (tab)
        tab->scroll(amount);
}

void ChatWindow::mousePressed(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        if (viewport)
        {
            gcn::Tab *tab = mChatTabs->getSelectedTab();
            if (tab)
            {
                ChatTab *cTab = dynamic_cast<ChatTab*>(tab);
                if (cTab)
                    viewport->showChatPopup(cTab);
            }
        }
    }

    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        ChatTab *tab = getFocused();
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

    if (isMovable() && mMoved)
    {
        int newX = std::max(0, getX() + event.getX() - mDragOffsetX);
        int newY = std::max(0, getY() + event.getY() - mDragOffsetY);
        newX = std::min(graphics->getWidth() - getWidth(), newX);
        newY = std::min(graphics->getHeight() - getHeight(), newY);
        setPosition(newX, newY);
    }
}

/*
void ChatWindow::mouseReleased(gcn::MouseEvent &event _UNUSED_)
{

}
*/

void ChatWindow::keyPressed(gcn::KeyEvent &event)
{
    if (event.getKey().getValue() == Key::DOWN)
    {
        if (mCurHist != mHistory.end())
        {
            // Move forward through the history
            HistoryIterator prevHist = mCurHist++;

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
        else if (mChatInput->getText() != "")
        {
            mChatInput->setText("");
        }
    }
    else if (event.getKey().getValue() == Key::UP &&
             mCurHist != mHistory.begin() && !mHistory.empty())
    {
        // Move backward through the history
        --mCurHist;
        mChatInput->setText(*mCurHist);
        mChatInput->setCaretPosition(static_cast<unsigned>(
                mChatInput->getText().length()));
    }
    else if (event.getKey().getValue() == Key::INSERT &&
             mChatInput->getText() != "")
    {
        // Add the current message to the history and clear the text
        if (mHistory.empty() || mChatInput->getText() != mHistory.back())
            mHistory.push_back(mChatInput->getText());
        mCurHist = mHistory.end();
        mChatInput->setText("");
    }
    else if (keyboard.isKeyActive(keyboard.KEY_AUTOCOMPLETE_CHAT) &&
             mChatInput->getText() != "")
    {
        autoComplete();
        return;
    }
    else if (keyboard.isKeyActive(keyboard.KEY_DEACTIVATE_CHAT) &&
             mChatInput->isVisible())
    {
        mChatInput->processVisible(false);
    }
    else if (keyboard.isKeyActive(keyboard.KEY_CHAT_PREV_HISTORY) &&
             mChatInput->isVisible())
    {
        ChatTab *tab = getFocused();
        if (tab && (unsigned)tab->getRows().size() > 0)
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

            std::list<std::string>::iterator it;
            unsigned int f = 0;
            for (it = tab->getRows().begin();
                 it != tab->getRows().end(); ++it, f++)
            {
                if (f == mChatHistoryIndex)
                    mChatInput->setText(*it);
            }
            mChatInput->setCaretPosition(static_cast<unsigned>(
                    mChatInput->getText().length()));
        }
    }
    else if (keyboard.isKeyActive(keyboard.KEY_CHAT_NEXT_HISTORY) &&
             mChatInput->isVisible())
    {
        ChatTab *tab = getFocused();
        if (tab && !tab->getRows().empty())
        {
            if (mChatHistoryIndex + 1 < tab->getRows().size())
            {
                mChatHistoryIndex ++;
            }
            else if (mChatHistoryIndex < tab->getRows().size())
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

            std::list<std::string>::iterator it;
            unsigned int f = 0;
            for (it = tab->getRows().begin();
                 it != tab->getRows().end(); ++it, f++)
            {
                if (f == mChatHistoryIndex)
                    mChatInput->setText(*it);
            }
            mChatInput->setCaretPosition(static_cast<unsigned>(
                    mChatInput->getText().length()));
        }
    }

    std::string Temp;
    switch (event.getKey().getValue())
    {
        case Key::F2: Temp = "\u2318"; break;
        case Key::F3: Temp = "\u263A"; break;
        case Key::F4: Temp = "\u2665"; break;
        case Key::F5: Temp = "\u266A"; break;
        case Key::F6: Temp = "\u266B"; break;
        case Key::F7: Temp = "\u26A0"; break;
        case Key::F8: Temp = "\u2622"; break;
        case Key::F9: Temp = "\u262E"; break;
        case Key::F10: Temp = "\u2605"; break;
        case Key::F11: Temp = "\u2618"; break;
        case Key::F12: Temp = "\u2592"; break;
        default: break;
    }

    if (Temp != "")
        addInputText(Temp, false);
}

void ChatWindow::event(Mana::Channels channel, const Mana::Event &event)
{
    if (channel == Mana::CHANNEL_NOTICES)
    {
        if (event.getName() == Mana::EVENT_SERVERNOTICE && localChatTab)
            localChatTab->chatLog(event.getString("message"), BY_SERVER);
    }
    else if (channel == Mana::CHANNEL_ATTRIBUTES)
    {
        if (!config.getBoolValue("showBattleEvents"))
            return;

        if (event.getName() == Mana::EVENT_UPDATEATTRIBUTE)
        {
            switch (event.getInt("id"))
            {
                case EXP:
                {
                    if (event.getInt("oldValue") > event.getInt("newValue"))
                        break;

                    int change = event.getInt("newValue")
                                 - event.getInt("oldValue");

                    if (change != 0)
                        battleChatLog("+" + toString(change) + " xp");
                    break;
                }
                case LEVEL:
                    battleChatLog("Level: " + toString(
                        event.getInt("newValue")));
                    break;
                default:
                    break;
            };
        }
        else if (event.getName() == Mana::EVENT_UPDATESTAT)
        {
            if (!config.getBoolValue("showJobExp"))
                return;

            int id = event.getInt("id");
            if (id == Net::getPlayerHandler()->getJobLocation())
            {
                std::pair<int, int> exp
                    = PlayerInfo::getStatExperience(id);
                if (event.getInt("oldValue1") > exp.first
                    || !event.getInt("oldValue2"))
                {
                    return;
                }

                int change = exp.first - event.getInt("oldValue1");
                if (change != 0)
                    battleChatLog("+" + toString(change) + " job");
            }
        }
    }
}

void ChatWindow::addInputText(const std::string &text, bool space)
{
    int caretPos = mChatInput->getCaretPosition();
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

void ChatWindow::setVisible(bool isVisible)
{
    Window::setVisible(isVisible);

    /*
     * For whatever reason, if setVisible is called, the mTmpVisible effect
     * should be disabled.
     */
    mTmpVisible = false;
}

void ChatWindow::whisper(const std::string &nick,
                         const std::string &mes, Own own)
{
    if (mes.empty() || !player_node)
        return;

    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    if (tempNick.compare(playerName) == 0)
        return;

    ChatTab *tab = 0;
    TabMap::const_iterator i = mWhispers.find(tempNick);

    if (i != mWhispers.end())
        tab = i->second;
    else if (config.getBoolValue("whispertab"))
        tab = addWhisperTab(nick);

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
                std::string nick2;
                int idx = mes.find(":");
                if (idx > 0)
                {
                    nick2 = msg.substr(0, idx);
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

            localChatTab->chatLog(strprintf(_("Whispering to %s: %s"),
                nick.c_str(), mes.c_str()), BY_PLAYER);
        }
        else
        {
            localChatTab->chatLog(nick + " : " + mes, ACT_WHISPER, false);
            if (player_node)
                player_node->afkRespond(0, nick);
        }
    }
}

ChatTab *ChatWindow::addWhisperTab(const std::string &nick, bool switchTo)
{
    if (!player_node)
        return NULL;

    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    TabMap::const_iterator i = mWhispers.find(tempNick);
    ChatTab *ret;

    if (tempNick.compare(playerName) == 0)
        return NULL;

    if (i != mWhispers.end())
    {
        ret = i->second;
    }
    else
    {
        ret = new WhisperTab(nick);
        mWhispers[tempNick] = ret;
        if (config.getBoolValue("showChatHistory"))
            ret->loadFromLogFile(nick);
    }

    if (switchTo)
        mChatTabs->setSelectedTab(ret);

    return ret;
}

ChatTab *ChatWindow::getWhisperTab(const std::string &nick) const
{
    if (!player_node)
        return NULL;

    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    TabMap::const_iterator i = mWhispers.find(tempNick);
    ChatTab *ret = 0;

    if (tempNick.compare(playerName) == 0)
        return NULL;

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

    std::string newMsg = "";
    int cMap[] = {1, 4, 5, 2, 3, 6, 7, 9, 0, 8};

    // rainbow
    switch(mChatColor)
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
    return "##" + toString(mChatColor - 1) + msg;
}

void ChatWindow::autoComplete()
{
    int caretPos = mChatInput->getCaretPosition();
    int startName = 0;
    const std::string inputText = mChatInput->getText();
    std::string name = inputText.substr(0, caretPos);
    std::string newName("");

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

    ChatTab *cTab = static_cast<ChatTab*>(mChatTabs->getSelectedTab());
    std::vector<std::string> nameList;

    cTab->getAutoCompleteList(nameList);
    newName = autoComplete(nameList, name);

    if (newName == "" && actorSpriteManager)
    {
        actorSpriteManager->getPlayerNames(nameList, true);
        newName = autoComplete(nameList, name);
    }
    if (newName == "")
        newName = autoCompleteHistory(name);
    if (newName == "" && spellManager)
        newName = spellManager->autoComplete(name);
    if (newName == "")
        newName = autoComplete(name, &mCommands);
    if (newName == "" && actorSpriteManager)
    {
        actorSpriteManager->getMobNames(nameList);
        newName = autoComplete(nameList, name);
    }
    if (newName == "")
        newName = autoComplete(name, &mCustomWords);

    if (newName != "")
    {
        mChatInput->setText(inputText.substr(0, startName) + newName
            + inputText.substr(caretPos, inputText.length() - caretPos));

        int len = caretPos - static_cast<int>(name.length())
                + static_cast<int>(newName.length());

        if (startName > 0)
            mChatInput->setCaretPosition(len + 1);
        else
            mChatInput->setCaretPosition(len);
    }
}

std::string ChatWindow::autoComplete(std::vector<std::string> &names,
                                     std::string partName) const
{
    std::vector<std::string>::iterator i = names.begin();
    toLower(partName);
    std::string newName("");

    while (i != names.end())
    {
        if (!i->empty())
        {
            std::string name = *i;
            toLower(name);

            std::string::size_type pos = name.find(partName, 0);
            if (pos == 0)
            {
                if (newName != "")
                {
                    toLower(newName);
                    newName = findSameSubstring(name, newName);
                }
                else
                {
                    newName = *i;
                }
            }
        }
        ++i;
    }

    return newName;
}

std::string ChatWindow::autoComplete(std::string partName, History *words)
{
    if (!words)
        return "";

    Commands::iterator i = words->begin();
    std::vector<std::string> nameList;

    while (i != words->end())
    {
        std::string line = *i;
        std::string::size_type pos = line.find(partName, 0);
        if (pos == 0)
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

std::string ChatWindow::autoCompleteHistory(std::string partName)
{
    History::iterator i = mHistory.begin();
    std::vector<std::string> nameList;

    while (i != mHistory.end())
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
                               bool ignoreRecord, bool tryRemoveColors)
{
    if (own == -1)
        own = BY_SERVER;

    if (tradeChatTab)
    {
        if (findI(line, mTradeFilter) != std::string::npos)
        {
//            logger->log("trade: " + line);
            tradeChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
            return;
        }

        unsigned long idx = line.find(": \302\202");
        if (idx != std::string::npos)
        {
            line = line.erase(idx + 2, 2);
            tradeChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
            return;
        }

        unsigned long idx1 = line.find("@@");
        if (idx1 != std::string::npos)
        {
            unsigned long idx2 = line.find("|", idx1);
            if (idx2 != std::string::npos)
            {
                unsigned long idx3 = line.find("@@", idx2);
                if (idx3 != std::string::npos)
                {
                    tradeChatTab->chatLog(line, own, ignoreRecord,
                                          tryRemoveColors);
                    return;
                }
            }
        }
    }

    if (localChatTab)
        localChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
}

void ChatWindow::battleChatLog(std::string line, Own own,
                               bool ignoreRecord, bool tryRemoveColors)
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
    std::string tradeListName = Client::getServerConfigDirectory()
                                + "/tradefilter.txt";

    std::ifstream tradeFile;
    struct stat statbuf;

    if (!stat(tradeListName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        tradeFile.open(tradeListName.c_str(), std::ios::in);
        char line[100];
        while (tradeFile.getline(line, 100))
        {
            std::string str = line;
            if (!str.empty())
                mTradeFilter.push_back(str);
        }
        tradeFile.close();
    }
}

void ChatWindow::updateOnline(std::set<std::string> &onlinePlayers)
{
    TabMap::iterator iter;
    const Party *party = 0;
    const Guild *guild = 0;
    if (player_node)
    {
        party = player_node->getParty();
        guild = player_node->getGuild();
    }
    for (iter = mWhispers.begin(); iter != mWhispers.end(); ++iter)
    {
        if (!iter->second)
            return;

        WhisperTab *tab = static_cast<WhisperTab*>(iter->second);

        if (!tab)
            continue;

        if (onlinePlayers.find(tab->getNick()) != onlinePlayers.end())
        {
            tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER));
        }
        else
        {
            const std::string nick = tab->getNick();
            if (actorSpriteManager)
            {
                const Being *being = actorSpriteManager->findBeingByName(
                    nick, ActorSprite::PLAYER);
                if (being)
                {
                    tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER));
                    continue;
                }
            }
            if (party)
            {
                const PartyMember *pm = party->getMember(nick);
                if (pm && pm->getOnline())
                {
                    tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER));
                    continue;
                }
            }
            if (guild)
            {
                const GuildMember *gm = guild->getMember(nick);
                if (gm && gm->getOnline())
                {
                    tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER));
                    continue;
                }
            }
            tab->setTabColor(&Theme::getThemeColor(Theme::WHISPER_OFFLINE));
        }
    }
}

void ChatWindow::loadState()
{
    int num = 0;
    while (num < 50)
    {
        std::string nick = serverConfig.getValue(
            "chatWhisper" + toString(num), "");

        if (nick.empty())
            break;
        int flags = serverConfig.getValue(
            "chatWhisperFlags" + toString(num), 1);

        ChatTab *tab = addWhisperTab(nick);
        if (tab)
        {
            tab->setAllowHighlight(flags & 1);
            tab->setRemoveNames((flags & 2) / 2);
            tab->setNoAway((flags & 4) / 4);
        }
        serverConfig.deleteKey("chatWhisper" + toString(num));
        serverConfig.deleteKey("chatWhisperFlags" + toString(num));
        num ++;
    }

    while (num < 50)
    {
        serverConfig.deleteKey("chatWhisper" + toString(num));
        serverConfig.deleteKey("chatWhisperFlags" + toString(num));
        num ++;
    }
}

void ChatWindow::saveState()
{
    int num = 0;
    TabMap::iterator iter;
    for (iter = mWhispers.begin(); iter != mWhispers.end() && num < 50; ++iter)
    {
        if (!iter->second)
            return;

        WhisperTab *tab = static_cast<WhisperTab*>(iter->second);

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

std::string ChatWindow::doReplace(const std::string &msg)
{
    if (Client::getServerName() == "server.themanaworld.org"
        || Client::getServerName() == "themanaworld.org"
        || Client::getServerName() == "81.161.192.4")
    {
        return msg;
    }

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
        char line[101];
        while (listFile.getline(line, 100))
        {
            std::string str = line;
            if (!str.empty())
                mCustomWords.push_back(str);
        }
        listFile.close();
    }
}

void ChatWindow::addToAwayLog(std::string line)
{
    if (!player_node || !player_node->getAwayMode())
        return;

    if (mAwayLog.size() > 20)
        mAwayLog.pop_front();

    if (line.find(player_node->getName()) != std::string::npos)
        mAwayLog.push_back("##9away:" + line);
}

void ChatWindow::displayAwayLog()
{
    if (!localChatTab)
        return;

    std::list<std::string>::iterator i = mAwayLog.begin();

    while (i != mAwayLog.end())
    {
        localChatTab->addNewRow(*i);
        ++i;
    }
}
