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

#include "gui/widgets/textfield.h"

#include "client.h"
#include "configuration.h"
#include "inputmanager.h"
#include "keyevent.h"

#include "gui/popupmenu.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "resources/image.h"

#include "utils/copynpaste.h"
#include "utils/dtor.h"

#include <guichan/font.hpp>

#undef DELETE  // Win32 compatibility hack

#include "debug.h"

Skin *TextField::mSkin;
int TextField::instances = 0;
float TextField::mAlpha = 1.0;
ImageRect TextField::skin;

TextField::TextField(const Widget2 *const widget,
                     const std::string &text, const bool loseFocusOnTab,
                     gcn::ActionListener *const listener, std::string eventId,
                     const bool sendAlwaysEvents):
    gcn::TextField(text),
    gcn::FocusListener(),
    Widget2(widget),
    mSendAlwaysEvents(sendAlwaysEvents),
    mNumeric(false),
    mMinimum(0),
    mMaximum(0),
    mLoseFocusOnTab(loseFocusOnTab),
    mLastEventPaste(false),
    mPadding(1),
    mCaretColor(&getThemeColor(Theme::CARET)),
    mForegroundColor2(getThemeColor(Theme::TEXTFIELD_OUTLINE)),
    mPopupMenu(nullptr)
{
    setFrameSize(2);
    mForegroundColor = getThemeColor(Theme::TEXTFIELD);

    addFocusListener(this);

    if (instances == 0)
    {
        if (Theme::instance())
        {
            mSkin = Theme::instance()->loadSkinRect(skin, "textfield.xml",
                "textfield_background.xml");
        }
    }

    instances++;

    if (mSkin)
    {
        mPadding = mSkin->getPadding();
        mFrameSize = mSkin->getOption("frameSize", 2);
    }

    adjustSize();
    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);
}

TextField::~TextField()
{
    if (gui)
        gui->removeDragged(this);

    delete mPopupMenu;
    mPopupMenu = nullptr;

    instances--;
    if (instances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
        {
            theme->unload(mSkin);
            theme->unloadRect(skin);
        }
    }
}

void TextField::updateAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            if (skin.grid[a])
                skin.grid[a]->setAlpha(mAlpha);
        }
    }
}

void TextField::draw(gcn::Graphics *graphics)
{
    BLOCK_START("TextField::draw")
    updateAlpha();

    if (isFocused())
    {
        drawCaret(graphics,
                  getFont()->getWidth(mText.substr(0, mCaretPosition)) -
                  mXScroll);
    }

    static_cast<Graphics*>(graphics)->setColorAll(
        mForegroundColor, mForegroundColor2);
    gcn::Font *const font = getFont();
    font->drawString(graphics, mText, mPadding - mXScroll, mPadding);
    BLOCK_END("TextField::draw")
}

void TextField::drawFrame(gcn::Graphics *graphics)
{
    BLOCK_START("TextField::drawFrame")
    const int bs = 2 * getFrameSize();
    static_cast<Graphics*>(graphics)->drawImageRect(0, 0,
        getWidth() + bs, getHeight() + bs, skin);
    BLOCK_END("TextField::drawFrame")
}

void TextField::setNumeric(const bool numeric)
{
    mNumeric = numeric;
    if (!numeric)
        return;

    const char *const text = mText.c_str();
    for (const char *textPtr = text; *textPtr; ++textPtr)
    {
        if (*textPtr < '0' || *textPtr > '9')
        {
            setText(mText.substr(0, textPtr - text));
            return;
        }
    }
}

int TextField::getValue() const
{
    if (!mNumeric)
        return 0;

    const int value = atoi(mText.c_str());
    if (value < mMinimum)
        return mMinimum;

    if (value > static_cast<signed>(mMaximum))
        return mMaximum;

    return value;
}

void TextField::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int val = keyEvent.getKey().getValue();

    if (val >= 32)
    {
        if (mNumeric)
        {
            if ((val >= '0' && val <= '9') || (val == '-' && !mCaretPosition))
            {
                char buf[2];
                buf[0] = static_cast<char>(val);
                buf[1] = 0;
                mText.insert(mCaretPosition, std::string(buf));
                mCaretPosition += 1;
                keyEvent.consume();
                fixScroll();
                if (mSendAlwaysEvents)
                    distributeActionEvent();
                return;
            }
        }
        else if (!mMaximum || mText.size() < mMaximum)
        {
            int len;
            if (val < 128)
                len = 1;               // 0xxxxxxx
            else if (val < 0x800)
                len = 2;               // 110xxxxx 10xxxxxx
            else if (val < 0x10000)
                len = 3;               // 1110xxxx 10xxxxxx 10xxxxxx
            else
                len = 4;               // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

            char buf[4];
            for (int i = 0; i < len; ++ i)
            {
                buf[i] = static_cast<char>(val >> (6 * (len - i - 1)));
                if (i > 0)
                    buf[i] = static_cast<char>((buf[i] & 63) | 128);
            }

            if (len > 1)
                buf[0] |= static_cast<char>(255 << (8 - len));

            mText.insert(mCaretPosition, std::string(buf, buf + len));
            mCaretPosition += len;
            keyEvent.consume();
            fixScroll();
            if (mSendAlwaysEvents)
                distributeActionEvent();
            return;
        }
    }

    /* In UTF-8, 10xxxxxx is only used for inner parts of characters. So skip
       them when processing key presses. */

    // unblock past key
    if (val != 22)
        mLastEventPaste = 0;

    bool consumed(false);
    switch (val)
    {
        case 2:  // Ctrl+b
        {
            while (mCaretPosition > 0)
            {
                --mCaretPosition;
                if ((mText[mCaretPosition] & 192) != 128)
                    break;
            }
            consumed = true;
            break;
        }

        case 6:  // Ctrl+f
        {
            const unsigned sz = static_cast<unsigned>(mText.size());
            while (mCaretPosition < sz)
            {
                ++mCaretPosition;
                if (mCaretPosition == sz ||
                    (mText[mCaretPosition] & 192) != 128)
                {
                    break;
                }
            }
            consumed = true;
            break;
        }

        case 4:  // Ctrl+d
        {
            unsigned sz = static_cast<unsigned>(mText.size());
            while (mCaretPosition < sz)
            {
                --sz;
                mText.erase(mCaretPosition, 1);
                if (mCaretPosition == sz ||
                    (mText[mCaretPosition] & 192) != 128)
                {
                    break;
                }
            }
            consumed = true;
            break;
        }

        case 8:  // Ctrl+h
            deleteCharLeft(mText, &mCaretPosition);
            consumed = true;
            break;

        case 5:  // Ctrl+e
            mCaretPosition = static_cast<int>(mText.size());
            consumed = true;
            break;

        case 11:  // Ctrl+k
            mText = mText.substr(0, mCaretPosition);
            consumed = true;
            break;

        case 21:  // Ctrl+u
            if (mCaretPosition > 0)
            {
                mText = mText.substr(mCaretPosition);
                mCaretPosition = 0;
            }
            consumed = true;
            break;

        case 3:
            handleCopy();
            consumed = true;
            break;

        case 22:  // Control code 22, SYNCHRONOUS IDLE, sent on Ctrl+v
            // hack to prevent paste key sticking
            if (mLastEventPaste && mLastEventPaste > cur_time)
                break;
            handlePaste();
            mLastEventPaste = cur_time + 2;
            consumed = true;
            break;

        case 23:  // Ctrl+w
            while (mCaretPosition > 0)
            {
                deleteCharLeft(mText, &mCaretPosition);
                if (mCaretPosition > 0 && isWordSeparator(
                    mText[mCaretPosition - 1]))
                {
                    break;
                }
            }
            consumed = true;
            break;

        default:
            break;
    }

    if (consumed)
    {
        if (mSendAlwaysEvents)
            distributeActionEvent();

        keyEvent.consume();
        fixScroll();
        return;
    }

    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    switch (action)
    {
        case Input::KEY_GUI_LEFT:
        {
            consumed = true;
            while (mCaretPosition > 0)
            {
                --mCaretPosition;
                if ((mText[mCaretPosition] & 192) != 128)
                    break;
            }
            break;
        }

        case Input::KEY_GUI_RIGHT:
        {
            consumed = true;
            const unsigned sz = static_cast<unsigned>(mText.size());
            while (mCaretPosition < sz)
            {
                ++mCaretPosition;
                if (mCaretPosition == sz ||
                    (mText[mCaretPosition] & 192) != 128)
                {
                    break;
                }
            }
            break;
        }

        case Input::KEY_GUI_DELETE:
        {
            consumed = true;
            unsigned sz = static_cast<unsigned>(mText.size());
            while (mCaretPosition < sz)
            {
                --sz;
                mText.erase(mCaretPosition, 1);
                if (mCaretPosition == sz ||
                    (mText[mCaretPosition] & 192) != 128)
                {
                    break;
                }
            }
            break;
        }

        case Input::KEY_GUI_BACKSPACE:
            consumed = true;
            deleteCharLeft(mText, &mCaretPosition);
            break;

        case Input::KEY_GUI_SELECT2:
            distributeActionEvent();
            keyEvent.consume();
            fixScroll();
            return;

        case Input::KEY_GUI_HOME:
            mCaretPosition = 0;
            consumed = true;
            break;

        case Input::KEY_GUI_END:
            mCaretPosition = static_cast<unsigned>(mText.size());
            consumed = true;
            break;

        case Input::KEY_GUI_TAB:
            if (mLoseFocusOnTab)
                return;
            consumed = true;
            break;

        default:
            break;
    }

    if (mSendAlwaysEvents)
        distributeActionEvent();

    if (consumed)
        keyEvent.consume();
    fixScroll();
}

void TextField::handlePaste()
{
    std::string text = getText();
    size_t caretPos = getCaretPosition();

    if (retrieveBuffer(text, caretPos))
    {
        setText(text);
        setCaretPosition(static_cast<unsigned>(caretPos));
    }
}

void TextField::handleCopy() const
{
    std::string text = getText();
    sendBuffer(text);
}

void TextField::drawCaret(gcn::Graphics* graphics, int x)
{
    const gcn::Rectangle clipArea = graphics->getCurrentClipArea();

    graphics->setColor(*mCaretColor);
    graphics->drawLine(x + mPadding, clipArea.height - mPadding,
        x + mPadding, mPadding);
}

void TextField::adjustSize()
{
    setWidth(getFont()->getWidth(mText) + 2 * mPadding + 1);
    adjustHeight();

    fixScroll();
}

void TextField::adjustHeight()
{
    setHeight(getFont()->getHeight() + 2 * mPadding);
}

void TextField::fixScroll()
{
    if (isFocused())
    {
        const int caretX = getFont()->getWidth(
            mText.substr(0, mCaretPosition));

        const int pad = 2 * mPadding;
        if (caretX - mXScroll >= getWidth() - pad)
        {
            mXScroll = caretX - getWidth() + pad;
        }
        else if (caretX - mXScroll <= 0)
        {
            mXScroll = caretX - getWidth() / 2;

            if (mXScroll < 0)
                mXScroll = 0;
        }
    }
}

void TextField::setCaretPosition(unsigned int position)
{
    if (position > mText.size())
        mCaretPosition = static_cast<int>(mText.size());
    else
        mCaretPosition = position;

    fixScroll();
}

void TextField::fontChanged()
{
    fixScroll();
}

void TextField::mousePressed(gcn::MouseEvent &mouseEvent)
{
#ifdef ANDROID
    if (!Client::isKeyboardVisible())
        inputManager.executeAction(Input::KEY_SHOW_KEYBOARD);
#endif
    if (mouseEvent.getButton() == gcn::MouseEvent::RIGHT)
    {
        if (viewport)
        {
            viewport->showTextFieldPopup(this);
        }
        else
        {
            if (!mPopupMenu)
                mPopupMenu = new PopupMenu();
            int x = 0;
            int y = 0;
            SDL_GetMouseState(&x, &y);
            mPopupMenu->showTextFieldPopup(x, y, this);
        }
    }
    else
    {
        gcn::TextField::mousePressed(mouseEvent);
    }
}

void TextField::focusGained(const gcn::Event &event A_UNUSED)
{
#ifdef ANDROID
    if (!Client::isKeyboardVisible())
        inputManager.executeAction(Input::KEY_SHOW_KEYBOARD);
#endif
}

void TextField::focusLost(const gcn::Event &event A_UNUSED)
{
}
