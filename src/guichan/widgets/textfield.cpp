/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 * Copyright (C) 2011-2012  The ManaPlus Developers
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

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widgets/textfield.hpp"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"

#include "debug.h"

namespace gcn
{
    TextField::TextField() :
        Widget(),
        MouseListener(),
        KeyListener(),
        mCaretPosition(0),
        mXScroll(0)
    {
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    TextField::TextField(const std::string& text) :
        Widget(),
        MouseListener(),
        KeyListener(),
        mText(text),
        mCaretPosition(0),
        mXScroll(0)
    {
        adjustSize();

        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
    }

    void TextField::setText(const std::string& text)
    {
        if (text.size() < mCaretPosition)
            mCaretPosition = static_cast<int>(text.size());

        mText = text;
    }

    void TextField::drawCaret(Graphics* graphics, int x)
    {
    }

    void TextField::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mCaretPosition = getFont()->getStringIndexAt(
                mText, mouseEvent.getX() + mXScroll);
            fixScroll();
        }
    }

    void TextField::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextField::adjustSize()
    {
        setWidth(getFont()->getWidth(mText) + 7);
        adjustHeight();

        fixScroll();
    }

    void TextField::adjustHeight()
    {
        setHeight(getFont()->getHeight() + 4);
    }

    void TextField::fixScroll()
    {
        if (isFocused())
        {
            const int caretX = getFont()->getWidth(
                mText.substr(0, mCaretPosition));

            if (caretX - mXScroll >= getWidth() - 4)
            {
                mXScroll = caretX - getWidth() + 4;
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

    unsigned int TextField::getCaretPosition() const
    {
        return mCaretPosition;
    }

    const std::string& TextField::getText() const
    {
        return mText;
    }

    void TextField::fontChanged()
    {
        fixScroll();
    }
}
