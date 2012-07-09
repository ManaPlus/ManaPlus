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

#ifndef SLIDER_H
#define SLIDER_H

#include <guichan/widgets/slider.hpp>

class Image;
class ImageRect;

/**
 * Slider widget. Same as the Guichan slider but with custom look.
 *
 * \ingroup GUI
 */
class Slider : public gcn::Slider
{
    public:
        /**
         * Constructor with scale start equal to 0.
         */
        Slider(double scaleEnd = 1.0);

        /**
         * Constructor.
         */
        Slider(double scaleStart, double scaleEnd);

        /**
         * Destructor.
         */
        ~Slider();

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the slider.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Draws the marker.
         */
        void drawMarker(gcn::Graphics *graphics);

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(gcn::MouseEvent& event);

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(gcn::MouseEvent& event);

        void keyPressed(gcn::KeyEvent& keyEvent);

        enum SLIDER_ENUM
        {
            HSTART,
            HMID,
            HEND,
            HGRIP,
            VSTART,
            VMID,
            VEND,
            VGRIP,
            SLIDER_MAX
        };

    private:
        /**
         * Used to initialize instances.
         */
        void init();

        static ImageRect buttons[2];
        //*hStart, *hMid, *hEnd, *hGrip;
//        static Image *vStart, *vMid, *vEnd, *vGrip;
//        static Image *hStartHi, *hMidHi, *hEndHi, *hGripHi;
//        static Image *vStartHi, *vMidHi, *vEndHi, *vGripHi;
        bool mHasMouse;
        static float mAlpha;
        static int mInstances;
};

#endif
