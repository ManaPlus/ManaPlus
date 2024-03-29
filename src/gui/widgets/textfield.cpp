/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gui/widgets/textfield.h"

#include "settings.h"

#ifdef USE_SDL2
#include "enums/input/keyvalue.h"
#endif  // USE_SDL2

#include "gui/gui.h"
#include "gui/skin.h"
#if defined(ANDROID) || defined(__SWITCH__)
#include "gui/windowmanager.h"
#endif  // ANDROID

#include "gui/fonts/font.h"

#include "gui/popups/popupmenu.h"

#include "input/inputmanager.h"

#include "utils/copynpaste.h"
#include "utils/stringutils.h"

#ifndef USE_SDL2
#include "utils/timer.h"
#endif  // USE_SDL2

#include "render/graphics.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#undef DELETE  // Win32 compatibility hack

#include "debug.h"

Skin *TextField::mSkin;
int TextField::instances = 0;
float TextField::mAlpha = 1.0;
ImageRect TextField::skin;

TextField::TextField(const Widget2 *restrict const widget,
                     const std::string &restrict text,
                     const LoseFocusOnTab loseFocusOnTab,
                     ActionListener *restrict const listener,
                     const std::string &restrict eventId,
                     const bool sendAlwaysEvents) :
    Widget(widget),
    FocusListener(),
    KeyListener(),
    MouseListener(),
    WidgetListener(),
    mText(text),
    mTextChunk(),
    mCaretPosition(0),
    mXScroll(0),
    mCaretColor(&getThemeColor(ThemeColorId::CARET, 255U)),
    mMinimum(0),
    mMaximum(0),
    mLastEventPaste(0),
    mPadding(1),
    mNumeric(false),
    mLoseFocusOnTab(loseFocusOnTab),
    mAllowSpecialActions(true),
    mSendAlwaysEvents(sendAlwaysEvents),
    mTextChanged(true)
{
    mAllowLogic = false;
    setFocusable(true);
    addMouseListener(this);
    addKeyListener(this);

    setFrameSize(2);
    mForegroundColor = getThemeColor(ThemeColorId::TEXTFIELD, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::TEXTFIELD_OUTLINE, 255U);

    addFocusListener(this);

    if (instances == 0)
    {
        if (theme != nullptr)
        {
            mSkin = theme->loadSkinRect(skin,
                "textfield.xml",
                "textfield_background.xml",
                0,
                8);
        }
    }

    instances++;

    if (mSkin != nullptr)
    {
        mPadding = mSkin->getPadding();
        mFrameSize = mSkin->getOption("frameSize", 2);
    }

    adjustSize();
    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener != nullptr)
        addActionListener(listener);
}

TextField::~TextField()
{
    if (mWindow != nullptr)
        mWindow->removeWidgetListener(this);

    if (gui != nullptr)
        gui->removeDragged(this);

    instances--;
    if (instances == 0)
    {
        if (theme != nullptr)
        {
            theme->unload(mSkin);
            Theme::unloadRect(skin, 0, 8);
        }
    }
    mTextChunk.deleteImage();
}

void TextField::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            if (skin.grid[a] != nullptr)
                skin.grid[a]->setAlpha(mAlpha);
        }
    }
}

void TextField::draw(Graphics *const graphics)
{
    BLOCK_START("TextField::draw")
    updateAlpha();

    Font *const font = getFont();
    if (isFocused())
    {
        drawCaret(graphics,
            font->getWidth(mText.substr(0, mCaretPosition)) - mXScroll);
    }

    if (mTextChanged)
    {
        mTextChunk.textFont = font;
        mTextChunk.deleteImage();
        mTextChunk.text = mText;
        mTextChunk.color = mForegroundColor;
        mTextChunk.color2 = mForegroundColor2;
        font->generate(mTextChunk);
        mTextChanged = false;
    }

    const Image *const image = mTextChunk.img;
    if (image != nullptr)
        graphics->drawImage(image, mPadding - mXScroll, mPadding);

    BLOCK_END("TextField::draw")
}

void TextField::safeDraw(Graphics *const graphics)
{
    TextField::draw(graphics);
}

void TextField::drawFrame(Graphics *const graphics)
{
    BLOCK_START("TextField::drawFrame")
    const int bs = 2 * mFrameSize;
    graphics->drawImageRect(0,
        0,
        mDimension.width + bs,
        mDimension.height + bs,
        skin);
    BLOCK_END("TextField::drawFrame")
}

void TextField::safeDrawFrame(Graphics *const graphics)
{
    BLOCK_START("TextField::drawFrame")
    const int bs = 2 * mFrameSize;
    graphics->drawImageRect(0,
        0,
        mDimension.width + bs,
        mDimension.height + bs,
        skin);
    BLOCK_END("TextField::drawFrame")
}

void TextField::setNumeric(const bool numeric)
{
    mNumeric = numeric;
    if (!numeric)
        return;

    const char *const text = mText.c_str();
    for (const char *textPtr = text; *textPtr != 0; ++textPtr)
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

    if (value > mMaximum)
        return mMaximum;

    return value;
}

void TextField::keyPressed(KeyEvent &event)
{
    const int val = event.getKey().getValue();

#ifdef USE_SDL2
    if (val == KeyValue::TEXTINPUT)
    {
        std::string str = event.getText();
        mText.insert(mCaretPosition, str);
        mTextChanged = true;
        mCaretPosition += CAST_U32(str.size());
        event.consume();
        fixScroll();
        if (mSendAlwaysEvents)
            distributeActionEvent();
        return;
    }
    bool consumed(false);
#else  // USE_SDL2

    if (val >= 32)
    {
        if (mNumeric)
        {
            if ((val >= '0' && val <= '9') ||
                (val == '-' && mCaretPosition == 0U))
            {
                char buf[2];
                buf[0] = CAST_8(val);
                buf[1] = 0;
                mText.insert(mCaretPosition, std::string(buf));
                mTextChanged = true;
                mCaretPosition += 1;
                event.consume();
                fixScroll();
                if (mSendAlwaysEvents)
                    distributeActionEvent();
                return;
            }
        }
        else if ((mMaximum == 0) ||
                 CAST_S32(mText.size()) < mMaximum)
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
                buf[i] = CAST_8(val >> (6 * (len - i - 1)));
                if (i > 0)
                    buf[i] = CAST_8((buf[i] & 63) | 128);
            }

            if (len > 1)
                buf[0] |= CAST_8(255U << (8 - len));

            mText.insert(mCaretPosition, std::string(buf, buf + len));
            mCaretPosition += len;
            mTextChanged = true;
            event.consume();
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
#endif  // USE_SDL2

    const InputActionT action = event.getActionId();
    if (!inputManager.isActionActive(InputAction::GUI_CTRL))
    {
        if (!handleNormalKeys(action, consumed))
        {
            if (consumed)
                event.consume();
            return;
        }
    }
    else
    {
        handleCtrlKeys(action, consumed);
    }

    if (mSendAlwaysEvents)
        distributeActionEvent();

    if (consumed)
        event.consume();
    fixScroll();
}

bool TextField::handleNormalKeys(const InputActionT action, bool &consumed)
{
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (action)
    {
        case InputAction::GUI_LEFT:
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

        case InputAction::GUI_RIGHT:
        {
            consumed = true;
            const unsigned sz = CAST_U32(mText.size());
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

        case InputAction::GUI_DELETE:
        {
            consumed = true;
            unsigned sz = CAST_U32(mText.size());
            while (mCaretPosition < sz)
            {
                --sz;
                mText.erase(mCaretPosition, 1);
                mTextChanged = true;
                if (mCaretPosition == sz ||
                    (mText[mCaretPosition] & 192) != 128)
                {
                    break;
                }
            }
            break;
        }

        case InputAction::GUI_BACKSPACE:
            consumed = true;
            deleteCharLeft(mText, &mCaretPosition);
            mTextChanged = true;
            break;

        case InputAction::GUI_SELECT2:
            distributeActionEvent();
            consumed = true;
            fixScroll();
            return false;

        case InputAction::GUI_HOME:
            mCaretPosition = 0;
            consumed = true;
            break;

        case InputAction::GUI_END:
            mCaretPosition = CAST_U32(mText.size());
            consumed = true;
            break;

        case InputAction::GUI_TAB:
            if (mLoseFocusOnTab == LoseFocusOnTab_true)
                return false;
            consumed = true;
            break;

        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
    return true;
}

void TextField::handleCtrlKeys(const InputActionT action, bool &consumed)
{
    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (action)
    {
        case InputAction::GUI_LEFT:
        {
            moveCaretWordBack();
            consumed = true;
            break;
        }
        case InputAction::GUI_RIGHT:
        {
            moveCaretWordForward();
            consumed = true;
            break;
        }
        case InputAction::GUI_B:
        {
            if (mAllowSpecialActions)
            {
                moveCaretBack();
                consumed = true;
            }
            break;
        }
        case InputAction::GUI_F:
        {
            moveCaretForward();
            consumed = true;
            break;
        }
        case InputAction::GUI_D:
        {
            caretDelete();
            consumed = true;
            break;
        }
        case InputAction::GUI_E:
        {
            mCaretPosition = CAST_S32(mText.size());
            consumed = true;
            break;
        }
        case InputAction::GUI_H:
        {
            deleteCharLeft(mText, &mCaretPosition);
            mTextChanged = true;
            consumed = true;
            break;
        }
        case InputAction::GUI_K:
        {
            mText = mText.substr(0, mCaretPosition);
            mTextChanged = true;
            consumed = true;
            break;
        }
        case InputAction::GUI_U:
        {
            caretDeleteToStart();
            consumed = true;
            break;
        }
        case InputAction::GUI_C:
        {
            handleCopy();
            consumed = true;
            break;
        }
        case InputAction::GUI_V:
        {
#ifdef USE_SDL2
            handlePaste();
#else  // USE_SDL2

            // hack to prevent paste key sticking
            if ((mLastEventPaste != 0) && mLastEventPaste > cur_time)
                break;
            handlePaste();
            mLastEventPaste = cur_time + 2;
#endif  // USE_SDL2

            consumed = true;
            break;
        }
        case InputAction::GUI_W:
        {
            caretDeleteWord();
            consumed = true;
            break;
        }
        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)
}

void TextField::moveCaretBack()
{
    while (mCaretPosition > 0)
    {
        --mCaretPosition;
        if ((mText[mCaretPosition] & 192) != 128)
        break;
    }
}

void TextField::moveCaretForward()
{
    const unsigned sz = CAST_U32(mText.size());
    while (mCaretPosition < sz)
    {
        ++mCaretPosition;
        if (mCaretPosition == sz || (mText[mCaretPosition] & 192) != 128)
            break;
    }
}

void TextField::caretDelete()
{
    unsigned sz = CAST_U32(mText.size());
    while (mCaretPosition < sz)
    {
        --sz;
        mText.erase(mCaretPosition, 1);
        if (mCaretPosition == sz || (mText[mCaretPosition] & 192) != 128)
            break;
    }
    mTextChanged = true;
}

void TextField::handlePaste()
{
    std::string text = getText();
    size_t caretPos = CAST_SIZE(getCaretPosition());

    if (retrieveBuffer(text, caretPos))
    {
        setText(text);
        setCaretPosition(CAST_U32(caretPos));
    }
}

void TextField::caretDeleteToStart()
{
    if (mCaretPosition > 0)
    {
        mText = mText.substr(mCaretPosition);
        mCaretPosition = 0;
    }
    mTextChanged = true;
}

void TextField::moveCaretWordBack()
{
    const unsigned int oldCaret = mCaretPosition;
    while (mCaretPosition > 0)
    {
        if (!isWordSeparator(mText[mCaretPosition - 1]))
            break;
        mCaretPosition --;
    }
    if (oldCaret != mCaretPosition)
        return;
    while (mCaretPosition > 0)
    {
        if (isWordSeparator(mText[mCaretPosition - 1]))
            break;
        mCaretPosition --;
    }
}

void TextField::moveCaretWordForward()
{
    const unsigned sz = CAST_U32(mText.size());
    const unsigned int oldCaret = mCaretPosition;
    while (mCaretPosition < sz)
    {
        if (!isWordSeparator(mText[mCaretPosition]))
            break;
        mCaretPosition ++;
    }
    if (oldCaret != mCaretPosition)
        return;
    while (mCaretPosition < sz)
    {
        if (isWordSeparator(mText[mCaretPosition]))
            break;
        mCaretPosition ++;
    }
}

void TextField::caretDeleteWord()
{
    while (mCaretPosition > 0)
    {
        deleteCharLeft(mText, &mCaretPosition);
        if (mCaretPosition > 0 && isWordSeparator(mText[mCaretPosition - 1]))
            break;
    }
    mTextChanged = true;
}

void TextField::handleCopy() const
{
    std::string text = getText();
    sendBuffer(text);
}

void TextField::drawCaret(Graphics* graphics, int x)
{
    const ClipRect &clipArea = graphics->getTopClip();

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

        const int width = mDimension.width;
        const int pad = 2 * mPadding;
        if (caretX - mXScroll >= width - pad)
        {
            mXScroll = caretX - width + pad;
        }
        else if (caretX - mXScroll <= 0)
        {
            mXScroll = caretX - width / 2;

            if (mXScroll < 0)
                mXScroll = 0;
        }
    }
}

void TextField::setCaretPosition(unsigned int position)
{
    const unsigned int sz = CAST_U32(mText.size());
    if (position > sz)
        mCaretPosition = CAST_S32(sz);
    else
        mCaretPosition = position;

    fixScroll();
}

void TextField::fontChanged()
{
    fixScroll();
}

void TextField::mousePressed(MouseEvent &event)
{
#if defined(ANDROID) || defined(__SWITCH__)
    if (!WindowManager::isKeyboardVisible())
        inputManager.executeAction(InputAction::SHOW_KEYBOARD);
#endif  // ANDROID

    event.consume();
    if (event.getButton() == MouseButton::RIGHT)
    {
#ifndef DYECMD
        if (popupMenu != nullptr)
            popupMenu->showTextFieldPopup(this);
#endif  // DYECMD
    }
    else if (event.getButton() == MouseButton::LEFT)
    {
        mCaretPosition = getFont()->getStringIndexAt(
            mText, event.getX() + mXScroll);
        fixScroll();
    }
}

void TextField::focusGained(const Event &event A_UNUSED)
{
#if defined(ANDROID) || defined(__SWITCH__)
    if (!WindowManager::isKeyboardVisible())
        inputManager.executeAction(InputAction::SHOW_KEYBOARD);
#endif  // ANDROID
}

void TextField::focusLost(const Event &event A_UNUSED)
{
}

void TextField::setText(const std::string& text)
{
    const unsigned int sz = CAST_U32(text.size());
    if (sz < mCaretPosition)
        mCaretPosition = sz;
    mText = text;
    mTextChanged = true;
}

void TextField::mouseDragged(MouseEvent& event)
{
    event.consume();
}

void TextField::widgetHidden(const Event &event A_UNUSED)
{
    mTextChanged = true;
    mTextChunk.deleteImage();
}

void TextField::setParent(Widget *widget)
{
    if (mWindow != nullptr)
        mWindow->addWidgetListener(this);
    Widget::setParent(widget);
}

void TextField::setWindow(Widget *const widget)
{
    if ((widget == nullptr) && (mWindow != nullptr))
    {
        mWindow->removeWidgetListener(this);
        mWindow = nullptr;
    }
    else
    {
        Widget2::setWindow(widget);
    }
}

void TextField::signalEvent()
{
    distributeActionEvent();
}
