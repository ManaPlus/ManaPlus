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

#ifndef GUI_WIDGETS_LISTBOX_H
#define GUI_WIDGETS_LISTBOX_H

#include "gui/widgets/widget2.h"

#include <guichan/widgets/listbox.hpp>

#include "localconsts.h"

class Skin;

/**
 * A list box, meant to be used inside a scroll area. Same as the Guichan list
 * box except this one doesn't have a background, instead completely relying
 * on the scroll area. It also adds selection listener functionality.
 *
 * \ingroup GUI
 */
class ListBox : public gcn::ListBox,
                public Widget2
{
    public:
        /**
         * Constructor.
         */
        ListBox(const Widget2 *const widget,
                gcn::ListModel *const listModel,
                const std::string &skin);

        A_DELETE_COPY(ListBox)

        virtual ~ListBox();

        void postInit();

        /**
         * Draws the list box.
         */
        void draw(gcn::Graphics *graphics) override;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        // Inherited from KeyListener

        void keyPressed(gcn::KeyEvent& keyEvent) override final;

        // Inherited from MouseListener

        void mouseWheelMovedUp(gcn::MouseEvent& mouseEvent) override final;

        void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent) override final;

        void mousePressed(gcn::MouseEvent &event) override;

        void mouseReleased(gcn::MouseEvent &event) override;

        void mouseReleased1(const gcn::MouseEvent &event);

        void mouseDragged(gcn::MouseEvent &event) override;

        void refocus();

        void setDistributeMousePressed(const bool b)
        { mDistributeMousePressed = b; }

        virtual void adjustSize();

        void logic() override final;

        virtual int getSelectionByMouse(const int y) const;

        void setCenter(const bool b)
        { mCenterText = b; }

        int getPressedIndex() const
        { return mPressedIndex; }

        unsigned int getRowHeight() const override A_WARN_UNUSED
        { return mRowHeight; }

        void setRowHeight(unsigned int n)
        { mRowHeight = n; }

    protected:
        gcn::Color mHighlightColor;
        gcn::Color mForegroundSelectedColor;
        gcn::Color mForegroundSelectedColor2;
        int mOldSelected;
        int mPadding;
        int mPressedIndex;
        unsigned int mRowHeight;
        int mItemPadding;
        Skin *mSkin;
        static float mAlpha;
        bool mDistributeMousePressed;
        bool mCenterText;
};

#endif  // GUI_WIDGETS_LISTBOX_H
