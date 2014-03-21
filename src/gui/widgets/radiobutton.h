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

#ifndef GUI_WIDGETS_RADIOBUTTON_H
#define GUI_WIDGETS_RADIOBUTTON_H

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "gui/widgets/widget.h"

#include "localconsts.h"

class Skin;

/**
 * Guichan based RadioButton with custom look
 */
class RadioButton final : public Widget,
                          public MouseListener,
                          public KeyListener

{
    public:
        /**
         * Constructor.
         */
        RadioButton(const Widget2 *const widget,
                    const std::string &restrict caption,
                    const std::string &restrict group,
                    const bool marked = false);

        A_DELETE_COPY(RadioButton)

        /**
         * Destructor.
         */
        ~RadioButton();

        /**
         * Draws the radiobutton, not the caption.
         */
        void drawBox(Graphics* graphics);

        /**
         * Implementation of the draw methods.
         * Thus, avoiding the rhomb around the radio button.
         */
        void draw(Graphics* graphics) override final;

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(MouseEvent& event) override final;

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(MouseEvent& event) override final;

        void keyPressed(KeyEvent& event) override final;

        void updateAlpha();

        void adjustSize();

        /**
         * Checks if the radio button is selected.
         *
         * @return True if the radio button is selecte, false otherwise.
         * @see setSelected
         */
        bool isSelected() const
        { return mSelected; }

        /**
         * Sets the radio button to selected or not.
         *
         * @param selected True if the radio button should be selected,
         *                 false otherwise.
         * @see isSelected
         */
        void setSelected(const bool selected);

        /**
         * Gets the caption of the radio button.
         *
         * @return The caption of the radio button.
         * @see setCaption
         */
        const std::string &getCaption() const
        { return mCaption; }

        /**
         * Sets the caption of the radio button. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * radio button's size to fit the caption.
         *
         * @param caption The caption of the radio button.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string &caption)
        { mCaption = caption; }

        void mouseClicked(MouseEvent& event) override final;

        void mouseDragged(MouseEvent& event) override final;

        /**
         * Sets the group the radio button should belong to. Note that
         * a radio button group is unique per application, not per Gui object
         * as the group is stored in a static map.
         *
         * @param group The name of the group.
         * @see getGroup
         */
        void setGroup(const std::string &group);

        /**
         * Gets the group the radio button belongs to.
         *
         * @return The group the radio button belongs to.
         * @see setGroup
         */
        const std::string &getGroup() const
        { return mGroup; }

    private:
        static int instances;
        static Skin *mSkin;
        static float mAlpha;

        /**
         * True if the radio button is selected, false otherwise.
         */
        bool mSelected;

        /**
         * Holds the caption of the radio button.
         */
        std::string mCaption;

        /**
         * Holds the group of the radio button.
         */
        std::string mGroup;

        /**
         * Typdef.
         */
        typedef std::multimap<std::string, RadioButton *> GroupMap;

        /**
         * Typdef.
         */
        typedef GroupMap::iterator GroupIterator;

        /**
         * Holds all available radio button groups.
         */
        static GroupMap mGroupMap;

        int mPadding;
        int mImagePadding;
        int mImageSize;
        int mSpacing;
        bool mHasMouse;
};

#endif  // GUI_WIDGETS_RADIOBUTTON_H
