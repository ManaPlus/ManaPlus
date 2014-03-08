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

#ifndef GUI_WIDGETS_SLIDER_H
#define GUI_WIDGETS_SLIDER_H

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "gui/widgets/widget.h"

#include "localconsts.h"

class ImageCollection;

/**
 * Slider widget. Same as the Guichan slider but with custom look.
 *
 * \ingroup GUI
 */
class Slider final : public Widget,
                     public MouseListener,
                     public KeyListener
{
    public:
        /**
         * Draw orientations for the slider. A slider can be drawn vertically or
         * horizontally.
         */
        enum Orientation
        {
            HORIZONTAL = 0,
            VERTICAL
        };

        /**
         * Constructor with scale start equal to 0.
         */
        explicit Slider(Widget2 *const widget,
                        const double scaleEnd = 1.0);

        /**
         * Constructor.
         */
        Slider(Widget2 *const widget,
               const double scaleStart,
               const double scaleEnd);

        A_DELETE_COPY(Slider)

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
        void draw(Graphics *graphics) override final;

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(MouseEvent& event) override final;

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(MouseEvent& event) override final;

        void mousePressed(MouseEvent &mouseEvent) override final;

        void mouseDragged(MouseEvent &mouseEvent) override final;

        void mouseWheelMovedUp(MouseEvent &mouseEvent) override final;

        void mouseWheelMovedDown(MouseEvent &mouseEvent) override final;

        void keyPressed(KeyEvent& keyEvent) override final;

        /**
         * Sets the scale of the slider.
         *
         * @param scaleStart The start value of the scale.
         * @param scaleEnd tThe end of value the scale.
         * @see getScaleStart, getScaleEnd
         */
        void setScale(const double scaleStart, const double scaleEnd);

        /**
         * Gets the start value of the scale.
         *
         * @return The start value of the scale.
         * @see setScaleStart, setScale
         */
        double getScaleStart() const
        { return mScaleStart; }

        /**
         * Sets the start value of the scale.
         *
         * @param scaleStart The start value of the scale.
         * @see getScaleStart
         */
        void setScaleStart(const double scaleStart)
        { mScaleStart = scaleStart; }

        /**
         * Gets the end value of the scale.
         *
         * @return The end value of the scale.
         * @see setScaleEnd, setScale
         */
        double getScaleEnd() const
        { return mScaleEnd; }

        /**
         * Sets the end value of the scale.
         *
         * @param scaleEnd The end value of the scale.
         * @see getScaleEnd
         */
        void setScaleEnd(const double scaleEnd)
        { mScaleEnd = scaleEnd; }

        /**
         * Sets the current selected value.
         *
         * @param value The current selected value.
         * @see getValue
         */
        void setValue(const double value);

        /**
         * Gets the current selected value.
         *
         * @return The current selected value.
         * @see setValue
         */
        double getValue() const
        { return mValue; }

        /**
         * Gets the length of the marker.
         *
         * @return The length of the marker.
         * @see setMarkerLength
         */
        int getMarkerLength() const
        { return mMarkerLength; }

        /**
         * Sets the length of the marker.
         *
         * @param length The length for the marker.
         * @see getMarkerLength
         */
        void setMarkerLength(const int length)
        { mMarkerLength = length; }

        /**
         * Sets the orientation of the slider. A slider can be drawn vertically
         * or horizontally.
         *
         * @param orientation The orientation of the slider.
         * @see getOrientation
         */
        void setOrientation(const Orientation orientation)
        { mOrientation = orientation; }

        /**
         * Gets the orientation of the slider. A slider can be drawn vertically
         * or horizontally.
         *
         * @return The orientation of the slider.
         * @see setOrientation
         */
        Orientation getOrientation() const
        { return mOrientation; }

        /**
         * Sets the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         *
         * @param length The step length.
         * @see getStepLength
         */
        void setStepLength(const double length)
        { mStepLength = length; }

        /**
         * Gets the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         *
         * @return the step length.
         * @see setStepLength
         */
        double getStepLength() const
        { return mStepLength; }

        enum SLIDER_ENUM
        {
            HSTART = 0,
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

        /**
         * Converts a marker position to a value in the scale.
         *
         * @param position The position to convert.
         * @return A scale value corresponding to the position.
         * @see valueToMarkerPosition
         */
        double markerPositionToValue(const int position) const;

        /**
         * Converts a value to a marker position.
         *
         * @param value The value to convert.
         * @return A marker position corresponding to the value.
         * @see markerPositionToValue
         */
        int valueToMarkerPosition(const double value) const;

        /**
         * Gets the marker position of the current selected value.
         *
         * @return The marker position of the current selected value.
         */
        int getMarkerPosition() const
        { return valueToMarkerPosition(getValue()); }

        static ImageRect buttons[2];
        static float mAlpha;
        static int mInstances;

        /**
         * Holds the current selected value.
         */
        double mValue;

        /**
         * Holds the step length. The step length is used when the keys LEFT
         * and RIGHT are pressed to step in the scale.
         */
        double mStepLength;

        /**
         * Holds the start value of the scale.
         */
        double mScaleStart;

        /**
         * Holds the end value of the scale.
         */
        double mScaleEnd;

        /**
         * Holds the orientation of the slider. A slider can be drawn
         * vertically or horizontally.
         */
        Orientation mOrientation;

        ImageCollection *mVertexes;

        /**
         * Holds the length of the marker.
         */
        int mMarkerLength;

        bool mHasMouse;
        bool mRedraw;
};

#endif  // GUI_WIDGETS_SLIDER_H
