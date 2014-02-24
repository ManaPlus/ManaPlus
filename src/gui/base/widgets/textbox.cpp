/*
 *  The ManaPlus Client
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

/*
 * For comments regarding functions please see the header file.
 */

#include "gui/base/widgets/textbox.hpp"

#include "gui/font.h"

#include "render/graphics.h"

#include "debug.h"

namespace gcn
{
    TextBox::TextBox(const Widget2 *const widget) :
        Widget(widget),
        MouseListener(),
        KeyListener(),
        mTextRows(),
        mCaretColumn(0),
        mCaretRow(0),
        mEditable(true),
        mOpaque(true)
    {
        setText("");
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSize();
    }

    TextBox::TextBox(const Widget2 *const widget,
                     const std::string& text) :
        Widget(widget),
        MouseListener(),
        KeyListener(),
        mTextRows(),
        mCaretColumn(0),
        mCaretRow(0),
        mEditable(true),
        mOpaque(true)
    {
        setText(text);
        setFocusable(true);

        addMouseListener(this);
        addKeyListener(this);
        adjustSize();
    }

    void TextBox::setText(const std::string& text)
    {
        mCaretColumn = 0;
        mCaretRow = 0;

        mTextRows.clear();

        size_t pos, lastPos = 0;
        int length;
        do
        {
            pos = text.find("\n", lastPos);

            if (pos != std::string::npos)
                length = static_cast<int>(pos - lastPos);
            else
                length = static_cast<int>(text.size() - lastPos);
            std::string sub = text.substr(lastPos, length);
            mTextRows.push_back(sub);
            lastPos = pos + 1;
        } while (pos != std::string::npos);

        adjustSize();
    }

/*
    void TextBox::draw(Graphics* graphics)
    {
    }
*/

    void TextBox::drawCaret(Graphics* graphics, int x, int y)
    {
        graphics->setColor(mForegroundColor);
        graphics->drawLine(x, getFont()->getHeight() + y, x, y);
    }

    void TextBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mCaretRow = mouseEvent.getY() / getFont()->getHeight();

            const int sz = static_cast<int>(mTextRows.size());
            if (mCaretRow >= sz)
                mCaretRow = sz - 1;

            mCaretColumn = getFont()->getStringIndexAt(
                mTextRows[mCaretRow], mouseEvent.getX());
        }
    }

    void TextBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void TextBox::keyPressed(KeyEvent& keyEvent A_UNUSED)
    {
    }

    void TextBox::adjustSize()
    {
        int width = 0;
        for (size_t i = 0, sz = mTextRows.size(); i < sz; ++i)
        {
            const int w = getFont()->getWidth(mTextRows[i]);
            if (width < w)
                width = w;
        }

        setWidth(width + 1);
        setHeight(static_cast<int>(getFont()->getHeight() * mTextRows.size()));
    }

    void TextBox::setCaretPosition(unsigned int position)
    {
        for (int row = 0, sz = static_cast<int>(mTextRows.size());
             row < sz; row ++)
        {
            if (position <= mTextRows[row].size())
            {
                mCaretRow = row;
                mCaretColumn = position;
                return;  // we are done
            }
            else
            {
                position--;
            }
        }

        // position beyond end of text
        mCaretRow = static_cast<int>(mTextRows.size() - 1);
        mCaretColumn = static_cast<int>(mTextRows[mCaretRow].size());
    }

    unsigned int TextBox::getCaretPosition() const
    {
        int pos = 0, row;

        for (row = 0; row < mCaretRow; row++)
            pos += static_cast<int>(mTextRows[row].size());

        return pos + mCaretColumn;
    }

    void TextBox::setCaretRowColumn(int row, int column)
    {
        setCaretRow(row);
        setCaretColumn(column);
    }

    void TextBox::setCaretRow(int row)
    {
        mCaretRow = row;

        const int sz = static_cast<int>(mTextRows.size());
        if (mCaretRow >= sz)
            mCaretRow = sz - 1;

        if (mCaretRow < 0)
            mCaretRow = 0;

        setCaretColumn(mCaretColumn);
    }

    unsigned int TextBox::getCaretRow() const
    {
        return mCaretRow;
    }

    void TextBox::setCaretColumn(int column)
    {
        mCaretColumn = column;

        const int sz = static_cast<int>(mTextRows[mCaretRow].size());
        if (mCaretColumn > sz)
            mCaretColumn = sz;

        if (mCaretColumn < 0)
            mCaretColumn = 0;
    }

    unsigned int TextBox::getCaretColumn() const
    {
        return mCaretColumn;
    }

    const std::string& TextBox::getTextRow(int row) const
    {
        return mTextRows[row];
    }

    void TextBox::setTextRow(int row, const std::string& text)
    {
        mTextRows[row] = text;

        if (mCaretRow == row)
            setCaretColumn(mCaretColumn);

        adjustSize();
    }

    unsigned int TextBox::getNumberOfRows() const
    {
        return static_cast<int>(mTextRows.size());
    }

    std::string TextBox::getText() const
    {
        if (mTextRows.empty())
            return std::string("");

        int i;
        std::string text;

        const int sz = static_cast<int>(mTextRows.size());
        for (i = 0; i < sz - 1; ++ i)
            text.append(mTextRows[i]).append("\n");
        text.append(mTextRows[i]);

        return text;
    }

    void TextBox::fontChanged()
    {
        adjustSize();
    }

    void TextBox::scrollToCaret()
    {
        Rect scroll;
        scroll.x = getFont()->getWidth(
            mTextRows[mCaretRow].substr(0, mCaretColumn));
        scroll.y = getFont()->getHeight() * mCaretRow;
        scroll.width = getFont()->getWidth(" ");

        // add 2 for some extra space
        scroll.height = getFont()->getHeight() + 2;

        showPart(scroll);
    }

    void TextBox::setEditable(bool editable)
    {
        mEditable = editable;
    }

    bool TextBox::isEditable() const
    {
        return mEditable;
    }

    void TextBox::addRow(const std::string &row)
    {
        mTextRows.push_back(row);
        adjustSize();
    }

    bool TextBox::isOpaque()
    {
        return mOpaque;
    }

    void TextBox::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }
}  // namespace gcn
