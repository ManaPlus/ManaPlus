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

#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "gui/widgets/widget2.h"

#include <guichan/focuslistener.hpp>
#include <guichan/widgets/textfield.hpp>

#include "localconsts.h"

class ImageRect;
class PopupMenu;
class TextField;

/**
 * A text field.
 *
 * \ingroup GUI
 */
class TextField : public gcn::TextField,
                  public gcn::FocusListener,
                  public Widget2
{
    public:
        /**
         * Constructor, initializes the text field with the given string.
         */
        TextField(const Widget2 *const widget,
                  const std::string &text = "",
                  const bool loseFocusOnTab = true,
                  gcn::ActionListener *const listener = nullptr,
                  std::string eventId = "",
                  const bool sendAlwaysEvents = false);

        A_DELETE_COPY(TextField)

        ~TextField();

        /**
         * Draws the text field.
         */
        virtual void draw(gcn::Graphics *graphics) override;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the background and border.
         */
        void drawFrame(gcn::Graphics *graphics) override;

        /**
         * Determine whether the field should be numeric or not
         */
        void setNumeric(const bool numeric);

        /**
         * Set the range on the field if it is numeric
         */
        void setRange(int min, int max)
        {
            mMinimum = min;
            mMaximum = max;
        }

        /**
         * Processes one keypress.
         */
        void keyPressed(gcn::KeyEvent &keyEvent) override;

        /**
         * Set the minimum value for a range
         */
        void setMinimum(int min)
        { mMinimum = min; }

        /**
         * Set the maximum value for a range
         */
        void setMaximum(int max)
        { mMaximum = max; }

        /**
         * Return the value for a numeric field
         */
        int getValue() const A_WARN_UNUSED;

        void setSendAlwaysEvents(bool b)
        { mSendAlwaysEvents = b; }

        void adjustSize();

        void adjustHeight();

        void setCaretPosition(unsigned int position);

        void mousePressed(gcn::MouseEvent &mouseEvent) override;

        void handlePaste();

        void handleCopy() const;

        void focusGained(const gcn::Event &event);

        void focusLost(const gcn::Event &event);

    protected:
        void drawCaret(gcn::Graphics* graphics, int x) override;

        void fixScroll();

        void fontChanged();

        bool mSendAlwaysEvents;

        static Skin *mSkin;

    private:
        static int instances;
        static float mAlpha;
        static ImageRect skin;
        bool mNumeric;
        int mMinimum;
        unsigned mMaximum;
        bool mLoseFocusOnTab;
        int mLastEventPaste;
        int mPadding;
        const gcn::Color *mCaretColor;
        gcn::Color mForegroundColor2;
        PopupMenu *mPopupMenu;
};

#endif
