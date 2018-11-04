/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/textbox.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "render/graphics.h"

#include <sstream>

#include "debug.h"

TextBox::TextBox(const Widget2 *const widget) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mTextRows(),
    mCaretColumn(0),
    mCaretRow(0),
    mMinWidth(getWidth()),
    mEditable(true),
    mOpaque(Opaque_true)
{
    mAllowLogic = false;
    setText("");
    setFocusable(true);

    addMouseListener(this);
    addKeyListener(this);
    adjustSize();

    mForegroundColor = getThemeColor(ThemeColorId::TEXTBOX, 255U);
    setOpaque(Opaque_false);
    setFrameSize(0);
}

TextBox::~TextBox()
{
    if (gui != nullptr)
        gui->removeDragged(this);
}

void TextBox::setTextWrapped(const std::string &text, const int minDimension)
{
    // Make sure parent scroll area sets width of this widget
    if (getParent() != nullptr)
        getParent()->logic();

    // Take the supplied minimum dimension as a starting
    // point and try to beat it
    mMinWidth = minDimension;

    const size_t textSize = text.size();
    size_t spacePos = text.rfind(' ', textSize);

    if (spacePos != std::string::npos)
    {
        const std::string word = text.substr(spacePos + 1);
        const int length = getFont()->getWidth(word);

        if (length > mMinWidth)
            mMinWidth = length;
    }

    std::stringstream wrappedStream;
    size_t newlinePos;
    size_t lastNewlinePos = 0;
    int minWidth = 0;
    int xpos;

    do
    {
        // Determine next piece of string to wrap
        newlinePos = text.find('\n', lastNewlinePos);

        if (newlinePos == std::string::npos)
            newlinePos = textSize;

        std::string line =
            text.substr(lastNewlinePos, newlinePos - lastNewlinePos);
        size_t lastSpacePos = 0;
        xpos = 0;
        const Font *const font = getFont();
        const int spaceWidth = font->getWidth(" ");
        size_t sz = line.size();

        do
        {
            spacePos = line.find(' ', lastSpacePos);

            if (spacePos == std::string::npos)
                spacePos = sz;

            const std::string word =
                line.substr(lastSpacePos, spacePos - lastSpacePos);

            const int width = font->getWidth(word);

            if (xpos == 0 && width > mMinWidth)
            {
                mMinWidth = width;
                xpos = width;
                wrappedStream << word;
            }
            else if (xpos != 0 && xpos + spaceWidth + width <=
                     mMinWidth)
            {
                xpos += spaceWidth + width;
                wrappedStream << " " << word;
            }
            else if (lastSpacePos == 0)
            {
                xpos += width;
                wrappedStream << word;
            }
            else
            {
                if (xpos > minWidth)
                    minWidth = xpos;

                // The window wasn't big enough. Resize it and try again.
                if (minWidth > mMinWidth)
                {
                    mMinWidth = minWidth;
                    wrappedStream.clear();
                    wrappedStream.str("");
                    lastNewlinePos = 0;
                    newlinePos = text.find('\n', lastNewlinePos);
                    if (newlinePos == std::string::npos)
                        newlinePos = textSize;
                    line = text.substr(lastNewlinePos, newlinePos -
                                       lastNewlinePos);
                    sz = line.size();
                    break;
                }
                else
                {
                    wrappedStream << "\n" << word;
                }
                xpos = width;
            }
            lastSpacePos = spacePos + 1;
        }
        while (spacePos != sz);

        if (text.find('\n', lastNewlinePos) != std::string::npos)
            wrappedStream << "\n";

        lastNewlinePos = newlinePos + 1;
    }
    while (newlinePos != textSize);

    if (xpos > minWidth)
        minWidth = xpos;

    mMinWidth = minWidth;

    setText(wrappedStream.str());
}

void TextBox::setText(const std::string& text)
{
    mCaretColumn = 0;
    mCaretRow = 0;

    mTextRows.clear();
    if (text.empty())
    {
        adjustSize();
        return;
    }

    size_t pos;
    size_t lastPos = 0;
    int length;
    do
    {
        pos = text.find('\n', lastPos);

        if (pos != std::string::npos)
            length = CAST_S32(pos - lastPos);
        else
            length = CAST_S32(text.size() - lastPos);
        std::string sub = text.substr(lastPos, length);
        mTextRows.push_back(sub);
        lastPos = pos + 1;
    } while (pos != std::string::npos);

    adjustSize();
}

void TextBox::keyPressed(KeyEvent& event)
{
    const Key &key = event.getKey();
    const InputActionT action = event.getActionId();

    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (action)
    {
        case InputAction::GUI_LEFT:
        {
            --mCaretColumn;
            if (mCaretColumn < 0)
            {
                --mCaretRow;

                if (mCaretRow < 0)
                {
                    mCaretRow = 0;
                    mCaretColumn = 0;
                }
                else
                {
                    mCaretColumn = CAST_S32(
                        mTextRows[mCaretRow].size());
                }
            }
            break;
        }

        case InputAction::GUI_RIGHT:
        {
            ++mCaretColumn;
            if (mCaretColumn > CAST_S32(mTextRows[mCaretRow].size()))
            {
                ++ mCaretRow;

                const int sz = CAST_S32(mTextRows.size());
                if (mCaretRow >= sz)
                {
                    mCaretRow = sz - 1;
                    if (mCaretRow < 0)
                        mCaretRow = 0;

                    mCaretColumn = CAST_S32(
                        mTextRows[mCaretRow].size());
                }
                else
                {
                    mCaretColumn = 0;
                }
            }
            break;
        }

        case InputAction::GUI_DOWN:
        {
            setCaretRow(mCaretRow + 1);
            break;
        }
        case InputAction::GUI_UP:
        {
            setCaretRow(mCaretRow - 1);
            break;
        }
        case InputAction::GUI_HOME:
        {
            mCaretColumn = 0;
            break;
        }
        case InputAction::GUI_END:
        {
            mCaretColumn = CAST_S32(mTextRows[mCaretRow].size());
            break;
        }

        case InputAction::GUI_SELECT2:
        {
            if (mEditable)
            {
                mTextRows.insert(mTextRows.begin() + mCaretRow + 1,
                    mTextRows[mCaretRow].substr(mCaretColumn,
                    mTextRows[mCaretRow].size() - mCaretColumn));
                mTextRows[mCaretRow].resize(mCaretColumn);
                ++mCaretRow;
                mCaretColumn = 0;
            }
            break;
        }

        case InputAction::GUI_BACKSPACE:
        {
            if (mCaretColumn != 0 && mEditable)
            {
                mTextRows[mCaretRow].erase(mCaretColumn - 1, 1);
                --mCaretColumn;
            }
            else if (mCaretColumn == 0 && mCaretRow != 0 && mEditable)
            {
                mCaretColumn = CAST_S32(
                    mTextRows[mCaretRow - 1].size());
                mTextRows[mCaretRow - 1] += mTextRows[mCaretRow];
                mTextRows.erase(mTextRows.begin() + mCaretRow);
                --mCaretRow;
            }
            break;
        }

        case InputAction::GUI_DELETE:
        {
            if (mCaretColumn < CAST_S32(
                mTextRows[mCaretRow].size()) && mEditable)
            {
                mTextRows[mCaretRow].erase(mCaretColumn, 1);
            }
            else if (mCaretColumn == CAST_S32(
                     mTextRows[mCaretRow].size()) &&
                     mCaretRow < (CAST_S32(mTextRows.size()) - 1) &&
                     mEditable)
            {
                mTextRows[mCaretRow] += mTextRows[mCaretRow + 1];
                mTextRows.erase(mTextRows.begin() + mCaretRow + 1);
            }
            break;
        }

        case InputAction::GUI_PAGE_UP:
        {
            Widget *const par = getParent();

            if (par != nullptr)
            {
                const int rowsPerPage = par->getChildrenArea().height
                    / getFont()->getHeight();
                mCaretRow -= rowsPerPage;

                if (mCaretRow < 0)
                    mCaretRow = 0;
            }
            break;
        }

        case InputAction::GUI_PAGE_DOWN:
        {
            Widget *const par = getParent();

            if (par != nullptr)
            {
                const int rowsPerPage = par->getChildrenArea().height
                    / getFont()->getHeight();
                mCaretRow += rowsPerPage;

                const int sz = CAST_S32(mTextRows.size());
                if (mCaretRow >= sz)
                    mCaretRow = sz - 1;
            }
            break;
        }

        case InputAction::GUI_TAB:
        {
            if (mEditable)
            {
                mTextRows[mCaretRow].insert(mCaretColumn, std::string("    "));
                mCaretColumn += 4;
            }
            break;
        }

        default:
        {
            if (key.isCharacter() && mEditable)
            {
                mTextRows[mCaretRow].insert(mCaretColumn,
                    std::string(1, CAST_S8(key.getValue())));
                ++ mCaretColumn;
            }
            break;
        }
    }
    PRAGMA45(GCC diagnostic pop)

    adjustSize();
    scrollToCaret();

    event.consume();
}

void TextBox::draw(Graphics *const graphics)
{
    BLOCK_START("TextBox::draw")
    if (mOpaque == Opaque_true)
    {
        graphics->setColor(mBackgroundColor);
        graphics->fillRectangle(Rect(0, 0, getWidth(), getHeight()));
    }

    Font *const font = getFont();
    if (isFocused() && isEditable())
    {
        drawCaret(graphics, font->getWidth(
            mTextRows[mCaretRow].substr(0, mCaretColumn)),
            mCaretRow * font->getHeight());
    }

    const int fontHeight = font->getHeight();

    for (size_t i = 0, sz = mTextRows.size(); i < sz; i++)
    {
        // Move the text one pixel so we can have a caret before a letter.
        font->drawString(graphics,
            mForegroundColor,
            mForegroundColor2,
            mTextRows[i], 1,
            CAST_S32(i * CAST_SIZE(fontHeight)));
    }
    BLOCK_END("TextBox::draw")
}

void TextBox::safeDraw(Graphics *const graphics)
{
    TextBox::draw(graphics);
}

void TextBox::setForegroundColor(const Color &color)
{
    mForegroundColor = color;
    mForegroundColor2 = color;
}

void TextBox::setForegroundColorAll(const Color &color1,
                                    const Color &color2)
{
    mForegroundColor = color1;
    mForegroundColor2 = color2;
}

std::string TextBox::getText() const
{
    if (mTextRows.empty())
        return std::string();

    int i;
    std::string text;

    const int sz = CAST_S32(mTextRows.size());
    for (i = 0; i < sz - 1; ++ i)
        text.append(mTextRows[i]).append("\n");
    text.append(mTextRows[i]);

    return text;
}

void TextBox::setTextRow(const int row, const std::string& text)
{
    mTextRows[row] = text;

    if (mCaretRow == row)
        setCaretColumn(mCaretColumn);

    adjustSize();
}

void TextBox::setCaretPosition(unsigned int position)
{
    for (int row = 0, fsz = CAST_S32(mTextRows.size());
         row < fsz;
         row ++)
    {
        if (position <= mTextRows[row].size())
        {
            mCaretRow = row;
            mCaretColumn = position;
            return;  // we are done
        }

        position--;
    }

    // position beyond end of text
    mCaretRow = CAST_S32(mTextRows.size() - 1);
    mCaretColumn = CAST_S32(mTextRows[mCaretRow].size());
}

void TextBox::setCaretRow(const int row)
{
    mCaretRow = row;

    const int sz = CAST_S32(mTextRows.size());
    if (mCaretRow >= sz)
        mCaretRow = sz - 1;

    if (mCaretRow < 0)
        mCaretRow = 0;

    setCaretColumn(mCaretColumn);
}

unsigned int TextBox::getCaretPosition() const
{
    int pos = 0;

    for (int row = 0; row < mCaretRow; row++)
        pos += CAST_S32(mTextRows[row].size());

    return pos + mCaretColumn;
}

void TextBox::setCaretColumn(const int column)
{
    mCaretColumn = column;

    const int sz = CAST_S32(mTextRows[mCaretRow].size());
    if (mCaretColumn > sz)
        mCaretColumn = sz;

    if (mCaretColumn < 0)
        mCaretColumn = 0;
}

void TextBox::setCaretRowColumn(const int row, const int column)
{
    setCaretRow(row);
    setCaretColumn(column);
}

void TextBox::scrollToCaret()
{
    const Font *const font = getFont();
    Rect scroll;
    scroll.x = font->getWidth(mTextRows[mCaretRow].substr(0, mCaretColumn));
    scroll.y = font->getHeight() * mCaretRow;
    scroll.width = font->getWidth(" ");
    // add 2 for some extra space
    scroll.height = font->getHeight() + 2;
    showPart(scroll);
}

void TextBox::addRow(const std::string &row)
{
    mTextRows.push_back(row);
    adjustSize();
}

void TextBox::mousePressed(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        const int height = getFont()->getHeight();
        if (height == 0)
            return;

        event.consume();
        mCaretRow = event.getY() / height;

        const int sz = CAST_S32(mTextRows.size());
        if (mCaretRow >= sz)
            mCaretRow = sz - 1;

        mCaretColumn = getFont()->getStringIndexAt(
            mTextRows[mCaretRow], event.getX());
    }
}

void TextBox::mouseDragged(MouseEvent& event)
{
    event.consume();
}

void TextBox::drawCaret(Graphics *const graphics,
                        const int x,
                        const int y) const
{
    graphics->setColor(mForegroundColor);
    graphics->drawLine(x, getFont()->getHeight() + y, x, y);
}

void TextBox::adjustSize()
{
    int width = 0;
    const Font *const font = getFont();
    for (size_t i = 0, sz = mTextRows.size(); i < sz; ++i)
    {
        const int w = font->getWidth(mTextRows[i]);
        if (width < w)
            width = w;
    }

    setWidth(width + 1);
    setHeight(font->getHeight() * CAST_S32(mTextRows.size()));
}

void TextBox::setEditable(const bool editable)
{
    mEditable = editable;
    mSelectable = editable;
}
