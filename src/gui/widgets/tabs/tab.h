/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_TABS_TAB_H
#define GUI_WIDGETS_TABS_TAB_H

#include "gui/widgets/basiccontainer.h"

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

class ImageCollection;
class Label;
class Skin;
class TabbedArea;

/**
 * A tab, the same as the Guichan tab in 0.8, but extended
 */
class Tab : public BasicContainer,
            public MouseListener,
            public WidgetListener
{
    public:
        explicit Tab(const Widget2 *const widget);

        A_DELETE_COPY(Tab)

        virtual ~Tab();

        enum
        {
            TAB_STANDARD    = 0,
            TAB_HIGHLIGHTED = 1,
            TAB_SELECTED    = 2,
            TAB_UNUSED      = 3,
            TAB_COUNT       = 4  // Must be last
        };

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draw the tabbed area.
         */
        void draw(Graphics *graphics) override final;

        /**
         * Set the normal color for the tab's text.
         */
        void setTabColor(const Color *const color1,
                         const Color *const color2)
        {
            mTabColor = color1;
            mTabOutlineColor = color2;
        }

        /**
         * Set the highlighted color for the tab's text.
         */
        void setHighlightedTabColor(const Color *const color1,
                                    const Color *const color2)
        {
            mTabHighlightedColor = color1;
            mTabHighlightedOutlineColor = color2;
        }

        /**
         * Set the selected color for the tab's text.
         */
        void setSelectedTabColor(const Color *const color1,
                                 const Color *const color2)
        {
            mTabSelectedColor = color1;
            mTabSelectedOutlineColor = color2;
        }

        /**
         * Set the flash color for the tab's text.
         */
        void setFlashTabColor(const Color *const color1,
                              const Color *const color2)
        {
            mFlashColor = color1;
            mFlashOutlineColor = color2;
        }

        /**
         * Set the player flash color for the tab's text.
         */
        void setPlayerFlashTabColor(const Color *const color1,
                                    const Color *const color2)
        {
            mPlayerFlashColor = color1;
            mPlayerFlashOutlineColor = color2;
        }

        /**
         * Set tab flashing state
         */
        void setFlash(const int flash)
        { mFlash = flash; }

        int getFlash() const A_WARN_UNUSED
        { return mFlash; }

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

        void setLabelFont(Font *const font);

        Label *getLabel() const A_WARN_UNUSED
        { return mLabel; }

        void adjustSize();

        void setTabbedArea(TabbedArea* tabbedArea);

        TabbedArea* getTabbedArea() const A_WARN_UNUSED;

        void setCaption(const std::string& caption);

        const std::string &getCaption() const A_WARN_UNUSED;

        void mouseEntered(MouseEvent &mouseEvent) override final;

        void mouseExited(MouseEvent &mouseEvent) override final;

        void setImage(Image *const image);

    protected:
        friend class TabbedArea;

        virtual void setCurrent()
        { }

        Label* mLabel;


        TabbedArea* mTabbedArea;

    private:
        /** Load images if no other instances exist yet */
        void init();

        static Skin *tabImg[TAB_COUNT];  /**< Tab state graphics */
        static int mInstances;           /**< Number of tab instances */
        static float mAlpha;

        const Color *mTabColor;
        const Color *mTabOutlineColor;
        const Color *mTabHighlightedColor;
        const Color *mTabHighlightedOutlineColor;
        const Color *mTabSelectedColor;
        const Color *mTabSelectedOutlineColor;
        const Color *mFlashColor;
        const Color *mFlashOutlineColor;
        const Color *mPlayerFlashColor;
        const Color *mPlayerFlashOutlineColor;
        int mFlash;
        ImageCollection *mVertexes;
        Image *mImage;
        int mMode;
        bool mRedraw;

    protected:
        bool mHasMouse;
};

#endif  // GUI_WIDGETS_TABS_TAB_H
