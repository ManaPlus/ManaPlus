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

#ifndef GUI_WIDGETS_LISTBOX_H
#define GUI_WIDGETS_LISTBOX_H

#include "gui/color.h"

#include "gui/widgets/widget.h"

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "localconsts.h"

class Skin;
class KeyEvent;
class ListModel;
class MouseEvent;
class SelectionListener;
class Widget2;

/**
 * A list box, meant to be used inside a scroll area. Same as the Guichan list
 * box except this one doesn't have a background, instead completely relying
 * on the scroll area. It also adds selection listener functionality.
 *
 * \ingroup GUI
 */
class ListBox : public Widget,
                public MouseListener,
                public KeyListener
{
    public:
        /**
         * Constructor.
         */
        ListBox(const Widget2 *const widget,
                ListModel *const listModel,
                const std::string &skin);

        A_DELETE_COPY(ListBox)

        virtual ~ListBox();

        void postInit();

        /**
         * Draws the list box.
         */
        void draw(Graphics *graphics) override;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        // Inherited from KeyListener

        void keyPressed(KeyEvent& event) override final;

        // Inherited from MouseListener

        void mouseWheelMovedUp(MouseEvent& event) override final;

        void mouseWheelMovedDown(MouseEvent& event) override final;

        void mousePressed(MouseEvent &event) override;

        void mouseReleased(MouseEvent &event) override;

        void mouseReleased1(const MouseEvent &event);

        void mouseDragged(MouseEvent &event) override;

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

        virtual unsigned int getRowHeight() const A_WARN_UNUSED
        { return mRowHeight; }

        void setRowHeight(unsigned int n)
        { mRowHeight = n; }

        /**
         * Gets the selected item as an index in the list model.
         *
         * @return the selected item as an index in the list model.
         * @see setSelected
         */
        int getSelected() const
        { return mSelected; }

         /**
         * Sets the selected item. The selected item is represented by
         * an index from the list model.
         *
         * @param selected the selected item as an index from the list model.
         * @see getSelected
         */
        void setSelected(const int selected);

        /**
         * Sets the list model to use.
         *
         * @param listModel the list model to use.
         * @see getListModel
         */
        void setListModel(ListModel *listModel);

        /**
         * Gets the list model used.
         *
         * @return the list model used.
         * @see setListModel
         */
        ListModel *getListModel() const
        { return mListModel; }

        /**
         * Checks whether the list box wraps when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @return true if wrapping is enabled, fasle otherwise.
         * @see setWrappingEnabled
         */
        bool isWrappingEnabled() const
        { return mWrappingEnabled; }

        /**
         * Sets the list box to wrap or not when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @see isWrappingEnabled
         */
        void setWrappingEnabled(const bool wrappingEnabled)
        { mWrappingEnabled = wrappingEnabled; }

        /**
         * Adds a selection listener to the list box. When the selection
         * changes an event will be sent to all selection listeners of the
         * list box.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener The selection listener to add.
         * @since 0.8.0
         */
        void addSelectionListener(SelectionListener *const selectionListener);

        /**
         * Removes a selection listener from the list box.
         *
         * @param selectionListener The selection listener to remove.
         * @since 0.8.0
         */
        void removeSelectionListener(SelectionListener *const
                                     selectionListener);

        /**
         * Distributes a value changed event to all selection listeners
         * of the list box.
         *
         * @since 0.8.0
         */
        void distributeValueChangedEvent();

    protected:
        /**
         * The selected item as an index in the list model.
         */
        int mSelected;

        /**
         * The list model to use.
         */
        ListModel *mListModel;

        /**
         * True if wrapping is enabled, false otherwise.
         */
        bool mWrappingEnabled;

        /**
         * Typdef.
         */
        typedef std::list<SelectionListener*> SelectionListenerList;

        /**
         * The selection listeners of the list box.
         */
        SelectionListenerList mSelectionListeners;

        /**
         * Typedef.
         */
        typedef SelectionListenerList::iterator SelectionListenerIterator;

        Color mHighlightColor;
        Color mForegroundSelectedColor;
        Color mForegroundSelectedColor2;
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
