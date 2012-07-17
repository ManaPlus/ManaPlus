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

#include "gui/widgets/textbox.h"

#include "keydata.h"
#include "keyevent.h"

#include "gui/theme.h"

#include <guichan/font.hpp>

#include <sstream>

#include "debug.h"

TextBox::TextBox() :
    mTextColor(&Theme::getThemeColor(Theme::TEXT))
{
    setOpaque(false);
    setFrameSize(0);
    mMinWidth = getWidth();
}

void TextBox::setTextWrapped(const std::string &text, int minDimension)
{
    // Make sure parent scroll area sets width of this widget
    if (getParent())
        getParent()->logic();

    // Take the supplied minimum dimension as a starting point and try to beat it
    mMinWidth = minDimension;

    std::stringstream wrappedStream;
    size_t spacePos;
    size_t newlinePos;
    size_t lastNewlinePos = 0;
    int minWidth = 0;
    int xpos;

    spacePos = text.rfind(" ", text.size());

    if (spacePos != std::string::npos)
    {
        const std::string word = text.substr(spacePos + 1);
        const int length = getFont()->getWidth(word);

        if (length > mMinWidth)
            mMinWidth = length;
    }

    do
    {
        // Determine next piece of string to wrap
        newlinePos = text.find("\n", lastNewlinePos);

        if (newlinePos == std::string::npos)
            newlinePos = text.size();

        std::string line =
            text.substr(lastNewlinePos, newlinePos - lastNewlinePos);
        size_t lastSpacePos = 0;
        xpos = 0;

        do
        {
            spacePos = line.find(" ", lastSpacePos);

            if (spacePos == std::string::npos)
                spacePos = line.size();

            std::string word =
                line.substr(lastSpacePos, spacePos - lastSpacePos);

            int width = getFont()->getWidth(word);

            if (xpos == 0 && width > mMinWidth)
            {
                mMinWidth = width;
                xpos = width;
                wrappedStream << word;
            }
            else if (xpos != 0 && xpos + getFont()->getWidth(" ") + width <= 
                     mMinWidth)
            {
                xpos += getFont()->getWidth(" ") + width;
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
//                    spacePos = 0;
                    lastNewlinePos = 0;
                    newlinePos = text.find("\n", lastNewlinePos);
                    if (newlinePos == std::string::npos)
                        newlinePos = text.size();
                    line = text.substr(lastNewlinePos, newlinePos -
                                       lastNewlinePos);
//                    width = 0;
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
        while (spacePos != line.size());

        if (text.find("\n", lastNewlinePos) != std::string::npos)
            wrappedStream << "\n";

        lastNewlinePos = newlinePos + 1;
    }
    while (newlinePos != text.size());

    if (xpos > minWidth)
        minWidth = xpos;

    mMinWidth = minWidth;

    gcn::TextBox::setText(wrappedStream.str());
}

void TextBox::keyPressed(gcn::KeyEvent& keyEvent)
{
    gcn::Key key = keyEvent.getKey();
    int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    switch (action)
    {
        case Input::KEY_GUI_LEFT:
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
                    mCaretColumn = static_cast<int>(
                        mTextRows[mCaretRow].size());
                }
            }
            break;
        }

        case Input::KEY_GUI_RIGHT:
        {
            ++mCaretColumn;
            if (mCaretColumn > static_cast<int>(mTextRows[mCaretRow].size()))
            {
                ++ mCaretRow;

                if (mCaretRow >= static_cast<int>(mTextRows.size()))
                {
                    mCaretRow = static_cast<int>(mTextRows.size()) - 1;
                    if (mCaretRow < 0)
                        mCaretRow = 0;

                    mCaretColumn = static_cast<int>(
                        mTextRows[mCaretRow].size());
                }
                else
                {
                    mCaretColumn = 0;
                }
            }
            break;
        }

        case Input::KEY_GUI_DOWN:
        {
            setCaretRow(mCaretRow + 1);
            break;
        }
        case Input::KEY_GUI_UP:
        {
            setCaretRow(mCaretRow - 1);
            break;
        }
        case Input::KEY_GUI_HOME:
        {
            mCaretColumn = 0;
            break;
        }
        case Input::KEY_GUI_END:
        {
            mCaretColumn = static_cast<int>(mTextRows[mCaretRow].size());
            break;
        }

        case Input::KEY_GUI_SELECT2:
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

        case Input::KEY_GUI_BACKSPACE:
        {
            if (mCaretColumn != 0 && mEditable)
            {
                mTextRows[mCaretRow].erase(mCaretColumn - 1, 1);
                --mCaretColumn;
            }
            else if (mCaretColumn == 0 && mCaretRow != 0 && mEditable)
            {
                mCaretColumn = static_cast<int>(
                    mTextRows[mCaretRow - 1].size());
                mTextRows[mCaretRow - 1] += mTextRows[mCaretRow];
                mTextRows.erase(mTextRows.begin() + mCaretRow);
                --mCaretRow;
            }
            break;
        }

        case Input::KEY_GUI_DELETE:
        {
            if (mCaretColumn < static_cast<int>(
                mTextRows[mCaretRow].size()) && mEditable)
            {
                mTextRows[mCaretRow].erase(mCaretColumn, 1);
            }
            else if (mCaretColumn == static_cast<int>(
                     mTextRows[mCaretRow].size()) &&
                     mCaretRow < (static_cast<int>(mTextRows.size()) - 1) &&
                     mEditable)
            {
                mTextRows[mCaretRow] += mTextRows[mCaretRow + 1];
                mTextRows.erase(mTextRows.begin() + mCaretRow + 1);
            }
            break;
        }

        case Input::KEY_GUI_PAGE_UP:
        {
            gcn::Widget* par = getParent();

            if (par)
            {
                int rowsPerPage = par->getChildrenArea().height
                    / getFont()->getHeight();
                mCaretRow -= rowsPerPage;

                if (mCaretRow < 0)
                    mCaretRow = 0;
            }
            break;
        }

        case Input::KEY_GUI_PAGE_DOWN:
        {
            gcn::Widget* par = getParent();

            if (par)
            {
                int rowsPerPage = par->getChildrenArea().height
                    / getFont()->getHeight();
                mCaretRow += rowsPerPage;

                if (mCaretRow >= static_cast<int>(mTextRows.size()))
                    mCaretRow = static_cast<int>(mTextRows.size()) - 1;
            }
            break;
        }

        case Input::KEY_GUI_TAB:
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
                    std::string(1, static_cast<char>(key.getValue())));
                ++ mCaretColumn;
            }
            break;
        }
    }

    adjustSize();
    scrollToCaret();

    keyEvent.consume();
}
