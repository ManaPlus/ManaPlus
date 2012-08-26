/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 * Copyright (C) 2011-2012  The ManaPlus Developers
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

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/widgets/slider.hpp"

#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"

#include "debug.h"

namespace gcn
{
    Slider::Slider(double scaleEnd) :
        mDragged(false),
        mScaleStart(0),
        mScaleEnd(scaleEnd)
    {
        setFocusable(true);
        setFrameSize(1);
        setOrientation(HORIZONTAL);
        setValue(0);
        setStepLength(scaleEnd / 10);
        setMarkerLength(10);

        addMouseListener(this);
        addKeyListener(this);
    }

    Slider::Slider(double scaleStart, double scaleEnd) :
        mDragged(false),
        mScaleStart(scaleStart),
        mScaleEnd(scaleEnd)
    {
        setFocusable(true);
        setFrameSize(1);
        setOrientation(HORIZONTAL);
        setValue(scaleStart);
        setStepLength((scaleEnd  - scaleStart) / 10);
        setMarkerLength(10);

        addMouseListener(this);
        addKeyListener(this);
    }

    void Slider::setScale(double scaleStart, double scaleEnd)
    {
        mScaleStart = scaleStart;
        mScaleEnd = scaleEnd;
    }

    double Slider::getScaleStart() const
    {
        return mScaleStart;
    }

    void Slider::setScaleStart(double scaleStart)
    {
        mScaleStart = scaleStart;
    }

    double Slider::getScaleEnd() const
    {
        return mScaleEnd;
    }

    void Slider::setScaleEnd(double scaleEnd)
    {
        mScaleEnd = scaleEnd;
    }

    void Slider::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == gcn::MouseEvent::LEFT
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() <= getWidth()
            && mouseEvent.getY() >= 0
            && mouseEvent.getY() <= getHeight())
        {
            if (getOrientation() == HORIZONTAL)
            {
                setValue(markerPositionToValue(
                    mouseEvent.getX() - getMarkerLength() / 2));
            }
            else
            {
                setValue(markerPositionToValue(getHeight()
                    - mouseEvent.getY() - getMarkerLength() / 2));
            }

            distributeActionEvent();
        }
    }

    void Slider::mouseDragged(MouseEvent& mouseEvent)
    {
        if (getOrientation() == HORIZONTAL)
        {
            setValue(markerPositionToValue(mouseEvent.getX()
                - getMarkerLength() / 2));
        }
        else
        {
            setValue(markerPositionToValue(getHeight()
                - mouseEvent.getY() - getMarkerLength() / 2));
        }

        distributeActionEvent();

        mouseEvent.consume();
    }

    void Slider::setValue(double value)
    {
        if (value > getScaleEnd())
        {
            mValue = getScaleEnd();
            return;
        }

        if (value < getScaleStart())
        {
            mValue = getScaleStart();
            return;
        }

        mValue = value;
    }

    double Slider::getValue() const
    {
        return mValue;
    }

    int Slider::getMarkerLength() const
    {
        return mMarkerLength;
    }

    void Slider::setMarkerLength(int length)
    {
        mMarkerLength = length;
    }

    void Slider::keyPressed(KeyEvent& keyEvent A_UNUSED)
    {
    }

    void Slider::setOrientation(Slider::Orientation orientation)
    {
        mOrientation = orientation;
    }

    Slider::Orientation Slider::getOrientation() const
    {
        return mOrientation;
    }

    double Slider::markerPositionToValue(int v) const
    {
        int w;
        if (getOrientation() == HORIZONTAL)
            w = getWidth();
        else
            w = getHeight();

        const double pos = v / (static_cast<double>(w) - getMarkerLength());
        return (1.0 - pos) * getScaleStart() + pos * getScaleEnd();
    }

    int Slider::valueToMarkerPosition(double value) const
    {
        int v;
        if (getOrientation() == HORIZONTAL)
            v = getWidth();
        else
            v = getHeight();

        const int w = static_cast<int>((v - getMarkerLength())
                * (value  - getScaleStart())
                / (getScaleEnd() - getScaleStart()));

        if (w < 0)
            return 0;

        if (w > v - getMarkerLength())
            return v - getMarkerLength();

        return w;
    }

    void Slider::setStepLength(double length)
    {
        mStepLength = length;
    }

    double Slider::getStepLength() const
    {
        return mStepLength;
    }

    int Slider::getMarkerPosition() const
    {
        return valueToMarkerPosition(getValue());
    }

    void Slider::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        setValue(getValue() + getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }

    void Slider::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        setValue(getValue() - getStepLength());
        distributeActionEvent();

        mouseEvent.consume();
    }
}
