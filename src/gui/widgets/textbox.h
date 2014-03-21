/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_TEXTBOX_H
#define GUI_WIDGETS_TEXTBOX_H

#include "gui/widgets/widget.h"

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "localconsts.h"

/**
 * A text box, meant to be used inside a scroll area. Same as the Guichan text
 * box except this one doesn't have a background or border, instead completely
 * relying on the scroll area.
 *
 * \ingroup GUI
 */
class TextBox final : public Widget,
                      public MouseListener,
                      public KeyListener
{
    public:
        /**
         * Constructor.
         */
        explicit TextBox(const Widget2 *const widget);

        A_DELETE_COPY(TextBox)

        ~TextBox();

        /**
         * Sets the text after wrapping it to the current width of the widget.
         */
        void setTextWrapped(const std::string &text, const int minDimension);

        /**
         * Get the minimum text width for the text box.
         */
        int getMinWidth() const A_WARN_UNUSED
        { return mMinWidth; }

        void keyPressed(KeyEvent& event) override final;

        void draw(Graphics* graphics) override final;

        void setForegroundColor(const Color &color);

        void setForegroundColorAll(const Color &color1,
                                   const Color &color2);

        /**
         * Sets the text of the text box.
         *
         * @param text The text of the text box.
         * @see getText
         */
        void setText(const std::string& text);

        /**
         * Gets the text of the text box.
         *
         * @return The text of the text box.
         * @see setText
         */
        std::string getText() const;

        /**
         * Gets a certain row from the text.
         *
         * @param row The number of the row to get from the text.
         * @return A row from the text of the text box.
         * @see setTextRow
         */
        const std::string& getTextRow(const int row) const
        { return mTextRows[row]; }

        /**
         * Sets the text of a certain row of the text.
         *
         * @param row The number of the row to set in the text.
         * @param text The text to set in the given row number.
         * @see getTextRow
         */
        void setTextRow(const int row, const std::string& text);

        /**
         * Gets the number of rows in the text.
         *
         * @return The number of rows in the text.
         */
        unsigned int getNumberOfRows() const
        { return static_cast<int>(mTextRows.size()); }

        /**
         * Gets the caret position in the text.
         *
         * @return The caret position in the text.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const;

        /**
         * Sets the position of the caret in the text.
         *
         * @param position the positon of the caret.
         * @see getCaretPosition
         */
        void setCaretPosition(unsigned int position);

        /**
         * Gets the row number where the caret is currently located.
         *
         * @return The row number where the caret is currently located.
         * @see setCaretRow
         */
        unsigned int getCaretRow() const
        { return mCaretRow; }

        /**
         * Sets the row where the caret should be currently located.
         *
         * @param The row where the caret should be currently located.
         * @see getCaretRow
         */
        void setCaretRow(const int row);

        /**
         * Gets the column where the caret is currently located.
         *
         * @return The column where the caret is currently located.
         * @see setCaretColumn
         */
        unsigned int getCaretColumn() const;

        /**
         * Sets the column where the caret should be currently located.
         *
         * @param The column where the caret should be currently located.
         * @see getCaretColumn
         */
        void setCaretColumn(const int column);

        /**
         * Sets the row and the column where the caret should be curretly
         * located.
         *
         * @param row The row where the caret should be currently located.
         * @param column The column where the caret should be currently located.
         * @see getCaretRow, getCaretColumn
         */
        void setCaretRowColumn(const int row, const int column);

        /**
         * Scrolls the text to the caret if the text box is in a scroll area.
         * 
         * @see ScrollArea
         */
        void scrollToCaret();

        /**
         * Checks if the text box is editable.
         *
         * @return True it the text box is editable, false otherwise.
         * @see setEditable
         */
        bool isEditable() const
        { return mEditable; }

        /**
         * Sets the text box to be editable or not.
         *
         * @param editable True if the text box should be editable, false otherwise.
         */
        void setEditable(const bool editable)
        { mEditable = editable; }

        /**
         * Adds a row of text to the end of the text.
         *
         * @param row The row to add.
         */
        void addRow(const std::string &row);

        /**
         * Checks if the text box is opaque. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @return True if the text box is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const
        { return mOpaque; }

        /**
         * Sets the text box to be opaque or not. An opaque text box will draw
         * it's background and it's text. A non opaque text box only draw it's
         * text making it transparent.
         *
         * @param opaque True if the text box should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(const bool opaque)
        { mOpaque = opaque; }

        void fontChanged() override final
        { adjustSize(); }

        void mousePressed(MouseEvent& event) override final;

        void mouseDragged(MouseEvent& event) override final;

    private:
        /**
         * Draws the caret. Overloaded this method if you want to
         * change the style of the caret.
         *
         * @param graphics a Graphics object to draw with.
         * @param x the x position.
         * @param y the y position.
         */
        void drawCaret(Graphics *const graphics, const int x, const int y);

        /**
         * Adjusts the text box's size to fit the text.
         */
        void adjustSize();

        /**
         * Holds all the rows of the text.
         */
        std::vector<std::string> mTextRows;

        /**
         * Holds the current column of the caret.
         */
        int mCaretColumn;

        /**
         * Holds the current row of the caret.
         */
        int mCaretRow;

        int mMinWidth;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mEditable;

        /**
         * True if the text box is editable, false otherwise.
         */
        bool mOpaque;
};

#endif  // GUI_WIDGETS_TEXTBOX_H
