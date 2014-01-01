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

#ifndef GUI_WIDGETS_TABS_TAB_H
#define GUI_WIDGETS_TABS_TAB_H

#include "gui/widgets/widget2.h"

#include <guichan/basiccontainer.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/widgetlistener.hpp>

#include "localconsts.h"

class ImageCollection;
class Label;
class Skin;
class TabbedArea;

/**
 * A tab, the same as the Guichan tab in 0.8, but extended
 */
class Tab : public gcn::BasicContainer,
            public Widget2,
            public gcn::MouseListener,
            public gcn::WidgetListener
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
        void draw(gcn::Graphics *graphics) override final;

        /**
         * Set the normal color for the tab's text.
         */
        void setTabColor(const gcn::Color *const color1,
                         const gcn::Color *const color2)
        {
            mTabColor = color1;
            mTabOutlineColor = color2;
        }

        /**
         * Set the highlighted color for the tab's text.
         */
        void setHighlightedTabColor(const gcn::Color *const color1,
                                    const gcn::Color *const color2)
        {
            mTabHighlightedColor = color1;
            mTabHighlightedOutlineColor = color2;
        }

        /**
         * Set the selected color for the tab's text.
         */
        void setSelectedTabColor(const gcn::Color *const color1,
                                 const gcn::Color *const color2)
        {
            mTabSelectedColor = color1;
            mTabSelectedOutlineColor = color2;
        }

        /**
         * Set the flash color for the tab's text.
         */
        void setFlashTabColor(const gcn::Color *const color1,
                              const gcn::Color *const color2)
        {
            mFlashColor = color1;
            mFlashOutlineColor = color2;
        }

        /**
         * Set the player flash color for the tab's text.
         */
        void setPlayerFlashTabColor(const gcn::Color *const color1,
                                    const gcn::Color *const color2)
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

        void widgetResized(const gcn::Event &event) override final;

        void widgetMoved(const gcn::Event &event) override final;

        void setLabelFont(gcn::Font *const font);

        Label *getLabel() const A_WARN_UNUSED
        { return mLabel; }

        void adjustSize();

        void setTabbedArea(TabbedArea* tabbedArea);

        TabbedArea* getTabbedArea() const A_WARN_UNUSED;

        void setCaption(const std::string& caption);

        const std::string &getCaption() const A_WARN_UNUSED;

        void mouseEntered(gcn::MouseEvent &mouseEvent) override final;

        void mouseExited(gcn::MouseEvent &mouseEvent) override final;

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

        const gcn::Color *mTabColor;
        const gcn::Color *mTabOutlineColor;
        const gcn::Color *mTabHighlightedColor;
        const gcn::Color *mTabHighlightedOutlineColor;
        const gcn::Color *mTabSelectedColor;
        const gcn::Color *mTabSelectedOutlineColor;
        const gcn::Color *mFlashColor;
        const gcn::Color *mFlashOutlineColor;
        const gcn::Color *mPlayerFlashColor;
        const gcn::Color *mPlayerFlashOutlineColor;
        int mFlash;
        ImageCollection *mVertexes;
        Image *mImage;
        int mMode;
        bool mRedraw;

    protected:
        bool mHasMouse;
};

#endif  // GUI_WIDGETS_TABS_TAB_H
