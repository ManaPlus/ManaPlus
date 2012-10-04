/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/textfield.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "keyevent.h"
#include "logger.h"

#include "gui/palette.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "resources/image.h"

#include "utils/copynpaste.h"
#include "utils/dtor.h"

#include <guichan/font.hpp>

#undef DELETE //Win32 compatibility hack

#include "debug.h"

int TextField::instances = 0;
float TextField::mAlpha = 1.0;
ImageRect TextField::skin;

TextField::TextField(const std::string &text, const bool loseFocusOnTab,
                     gcn::ActionListener *const listener, std::string eventId,
                     const bool sendAlwaysEvents):
    gcn::TextField(text),
    mSendAlwaysEvents(sendAlwaysEvents),
    mNumeric(false),
    mMinimum(0),
    mMaximum(0),
    mLoseFocusOnTab(loseFocusOnTab),
    mLastEventPaste(false),
    mCaretColor(&Theme::getThemeColor(Theme::CARET))
{
    setFrameSize(2);
    mForegroundColor = Theme::getThemeColor(Theme::TEXTFIELD);

    if (instances == 0)
    {
        if (Theme::instance())
            Theme::instance()->loadRect(skin, "textfield_background.xml", "");
    }

    instances++;

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);
}

TextField::~TextField()
{
    instances--;
    if (instances == 0 && Theme::instance())
        Theme::instance()->unloadRect(skin);
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
    updateAlpha();

    if (isFocused())
    {
        drawCaret(graphics,
                  getFont()->getWidth(mText.substr(0, mCaretPosition)) -
                  mXScroll);
    }

    graphics->setColor(mForegroundColor);
    graphics->setFont(getFont());
    graphics->drawText(mText, 1 - mXScroll, 1);
}

void TextField::drawFrame(gcn::Graphics *graphics)
{
    //updateAlpha(); -> Not useful...

    int w, h;
    const int bs = 2 * getFrameSize();
    w = getWidth() + bs;
    h = getHeight() + bs;

    static_cast<Graphics*>(graphics)->drawImageRect(0, 0, w, h, skin);
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
        case 2: // Ctrl+b
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

        case 6: // Ctrl+f
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

        case 4: // Ctrl+d
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

        case 8: // Ctrl+h
            deleteCharLeft(mText, &mCaretPosition);
            consumed = true;
            break;

        case 5: // Ctrl+e
            mCaretPosition = static_cast<int>(mText.size());
            consumed = true;
            break;

        case 11: // Ctrl+k
            mText = mText.substr(0, mCaretPosition);
            consumed = true;
            break;

        case 21: // Ctrl+u
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

        case 22: // Control code 22, SYNCHRONOUS IDLE, sent on Ctrl+v
            // hack to prevent paste key sticking
            if (mLastEventPaste && mLastEventPaste > cur_time)
                break;
            handlePaste();
            mLastEventPaste = cur_time + 2;
            consumed = true;
            break;

        case 23: // Ctrl+w
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
    graphics->drawLine(x, clipArea.height - 2, x, 1);
}
