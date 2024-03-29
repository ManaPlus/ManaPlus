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

#ifndef GUI_WIDGETS_TEXTFIELD_H
#define GUI_WIDGETS_TEXTFIELD_H

#include "listeners/focuslistener.h"
#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "enums/simpletypes/losefocusontab.h"

#include "gui/fonts/textchunk.h"

#include "gui/widgets/widget.h"

#include "localconsts.h"

/**
 * A text field.
 *
 * \ingroup GUI
 */
class TextField notfinal : public Widget,
                           public FocusListener,
                           public KeyListener,
                           public MouseListener,
                           public WidgetListener
{
    public:
        /**
         * Constructor, initializes the text field with the given string.
         */
        TextField(const Widget2 *restrict const widget,
                  const std::string &restrict text,
                  const LoseFocusOnTab loseFocusOnTab,
                  ActionListener *restrict const listener,
                  const std::string &restrict eventId,
                  const bool sendAlwaysEvents);

        A_DELETE_COPY(TextField)

        ~TextField() override;

        /**
         * Draws the text field.
         */
        void draw(Graphics *const graphics) override A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override A_NONNULL(2);

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the background and border.
         */
        void drawFrame(Graphics *const graphics) override final A_NONNULL(2);

        void safeDrawFrame(Graphics *const graphics) override final
                           A_NONNULL(2);

        /**
         * Determine whether the field should be numeric or not
         */
        void setNumeric(const bool numeric);

        /**
         * Set the range on the field if it is numeric
         */
        void setRange(const int min,
                      const int max)
        {
            mMinimum = min;
            mMaximum = max;
        }

        /**
         * Processes one keypress.
         */
        void keyPressed(KeyEvent &event) override;

        /**
         * Set the minimum value for a range
         */
        void setMinimum(const int min)
        { mMinimum = min; }

        /**
         * Set the maximum value for a range
         */
        void setMaximum(const int max)
        { mMaximum = max; }

        /**
         * Return the value for a numeric field
         */
        int getValue() const A_WARN_UNUSED;

        void setSendAlwaysEvents(const bool b) noexcept2
        { mSendAlwaysEvents = b; }

        void adjustSize();

        void adjustHeight();

        void setCaretPosition(unsigned int position);

        void mousePressed(MouseEvent &event) override final;

        void handlePaste();

        void handleCopy() const;

#ifdef ANDROID
        void focusGained(const Event &event) override final;
#else  // ANDROID

        void focusGained(const Event &event) override final;
#endif  // ANDROID

        void focusLost(const Event &event) override;

        void moveCaretBack();

        void moveCaretForward();

        void moveCaretWordBack();

        void moveCaretWordForward();

        void caretDelete();

        void caretDeleteToStart();

        void caretDeleteWord();

        void setAllowSpecialActions(const bool b)
        { mAllowSpecialActions = b; }

        std::string getTextBeforeCaret() const
        { return mText.substr(0, mCaretPosition); }

        /**
         * Sets the text of the text field.
         *
         * @param text The text of the text field.
         * @see getText
         */
        void setText(const std::string& text);

        /**
         * Gets the text of the text field.
         *
         * @return The text of the text field.
         * @see setText
         */
        const std::string& getText() const
        { return mText; }

        /**
         * Gets the caret position. As there is only one line of text
         * in a text field the position is the caret's x coordinate.
         *
         * @return The caret position.
         * @see setCaretPosition
         */
        unsigned int getCaretPosition() const
        { return mCaretPosition; }

        void mouseDragged(MouseEvent& event) override final;

        void widgetHidden(const Event &event) override final;

        void setParent(Widget *widget) override final;

        void setWindow(Widget *const widget) override final;

        void signalEvent();

    protected:
        void drawCaret(Graphics* graphics, int x) A_NONNULL(2);

        void fixScroll();

        void fontChanged() override;

        bool handleNormalKeys(const InputActionT action, bool &consumed);

        void handleCtrlKeys(const InputActionT action, bool &consumed);

        static Skin *mSkin;

        /**
         * Holds the text of the text box.
         */
        std::string mText;

        TextChunk mTextChunk;

        /**
         * Holds the caret position.
         */
        unsigned int mCaretPosition;

        /**
         * Holds the amount scrolled in x. If a user types more characters than
         * the text field can display, due to the text field being to small, the
         * text needs to scroll in order to show the last type character.
         */
        int mXScroll;

        const Color *mCaretColor;
        static int instances;
        static float mAlpha;
        static ImageRect skin;
        int mMinimum;
        int mMaximum;
        time_t mLastEventPaste;
        int mPadding;
        bool mNumeric;
        LoseFocusOnTab mLoseFocusOnTab;
        bool mAllowSpecialActions;
        bool mSendAlwaysEvents;
        bool mTextChanged;
};

#endif  // GUI_WIDGETS_TEXTFIELD_H
