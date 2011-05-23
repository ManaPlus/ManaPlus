/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/widgets/slider.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"

#include "gui/theme.h"

#include "resources/image.h"

#include "debug.h"

Image *Slider::hStart, *Slider::hMid, *Slider::hEnd, *Slider::hGrip;
Image *Slider::vStart, *Slider::vMid, *Slider::vEnd, *Slider::vGrip;
Image *Slider::hStartHi, *Slider::hMidHi, *Slider::hEndHi, *Slider::hGripHi;
Image *Slider::vStartHi, *Slider::vMidHi, *Slider::vEndHi, *Slider::vGripHi;
float Slider::mAlpha = 1.0;
int Slider::mInstances = 0;

Slider::Slider(double scaleEnd):
    gcn::Slider(scaleEnd),
    mHasMouse(false)
{
    init();
}

Slider::Slider(double scaleStart, double scaleEnd):
    gcn::Slider(scaleStart, scaleEnd),
    mHasMouse(false)
{
    init();
}

Slider::~Slider()
{
    mInstances--;

    if (mInstances == 0)
    {
        delete hStart;
        delete hMid;
        delete hEnd;
        delete hGrip;
        delete vStart;
        delete vMid;
        delete vEnd;
        delete vGrip;
        delete hStartHi;
        delete hMidHi;
        delete hEndHi;
        delete hGripHi;
        delete vStartHi;
        delete vMidHi;
        delete vEndHi;
        delete vGripHi;
    }
}

void Slider::init()
{
    setFrameSize(0);

    // Load resources
    if (mInstances == 0)
    {
        int x, y, w, h, o1, o2;

        Image *slider = Theme::getImageFromTheme("slider.png");
        Image *sliderHi = Theme::getImageFromTheme("slider_hilight.png");

        x = 0; y = 0;
        w = 15; h = 6;
        o1 = 4; o2 = 11;
        if (slider)
        {
            hStart = slider->getSubImage(x, y, o1 - x, h);
            hMid = slider->getSubImage(o1, y, o2 - o1, h);
            hEnd = slider->getSubImage(o2, y, w - o2 + x, h);
        }
        else
        {
            hStart = 0;
            hMid = 0;
            hEnd = 0;
        }
        if (sliderHi)
        {
            hStartHi = sliderHi->getSubImage(x, y, o1 - x, h);
            hMidHi = sliderHi->getSubImage(o1, y, o2 - o1, h);
            hEndHi = sliderHi->getSubImage(o2, y, w - o2 + x, h);
        }
        else
        {
            hStartHi = 0;
            hMidHi = 0;
            hEndHi = 0;
        }

        x = 6; y = 8;
        w = 9; h = 10;
        if (slider)
            hGrip = slider->getSubImage(x, y, w, h);
        else
            hGrip = 0;
        if (sliderHi)
            hGripHi = sliderHi->getSubImage(x, y, w, h);
        else
            hGripHi = 0;

        x = 0; y = 6;
        w = 6; h = 21;
        o1 = 10; o2 = 18;
        if (slider)
        {
            vStart = slider->getSubImage(x, y, w, o1 - y);
            vMid = slider->getSubImage(x, o1, w, o2 - o1);
            vEnd = slider->getSubImage(x, o2, w, h - o2 + y);
        }
        else
        {
            vStart = 0;
            vMid = 0;
            vEnd = 0;
        }
        if (sliderHi)
        {
            vStartHi = sliderHi->getSubImage(x, y, w, o1 - y);
            vMidHi = sliderHi->getSubImage(x, o1, w, o2 - o1);
            vEndHi = sliderHi->getSubImage(x, o2, w, h - o2 + y);
        }
        else
        {
            vStartHi = 0;
            vMidHi = 0;
            vEndHi = 0;
        }

        x = 6; y = 8;
        w = 9; h = 10;
        if (slider)
            vGrip = slider->getSubImage(x, y, w, h);
        else
            vGrip = 0;

        if (sliderHi)
            vGripHi = sliderHi->getSubImage(x, y, w, h);
        else
            vGripHi = 0;

        if (slider)
            slider->decRef();
        if (sliderHi)
            sliderHi->decRef();
    }

    mInstances++;

    if (hGrip)
        setMarkerLength(hGrip->getWidth());
}

void Slider::updateAlpha()
{
    float alpha = std::max(Client::getGuiAlpha(),
                           Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        if (hStart)
            hStart->setAlpha(mAlpha);
        if (hMid)
            hMid->setAlpha(mAlpha);
        if (hEnd)
            hEnd->setAlpha(mAlpha);
        if (hGrip)
            hGrip->setAlpha(mAlpha);
        if (hStartHi)
            hStartHi->setAlpha(mAlpha);
        if (hMidHi)
            hMidHi->setAlpha(mAlpha);
        if (hEndHi)
            hEndHi->setAlpha(mAlpha);
        if (hGripHi)
            hGripHi->setAlpha(mAlpha);

        if (vStart)
            vStart->setAlpha(mAlpha);
        if (vMid)
            vMid->setAlpha(mAlpha);
        if (vEnd)
            vEnd->setAlpha(mAlpha);
        if (vGrip)
            vGrip->setAlpha(mAlpha);
        if (vStartHi)
            vStartHi->setAlpha(mAlpha);
        if (vMidHi)
            vMidHi->setAlpha(mAlpha);
        if (vEndHi)
            vEndHi->setAlpha(mAlpha);
        if (vGripHi)
            vGripHi->setAlpha(mAlpha);
    }

}

void Slider::draw(gcn::Graphics *graphics)
{
    if (!hStart || !hStartHi)
        return;

    int w = getWidth();
    int h = getHeight();
    int x = 0;
    int y = mHasMouse ? (h - hStartHi->getHeight()) / 2 :
            (h - hStart->getHeight()) / 2;

    updateAlpha();

    if (!mHasMouse)
    {
        static_cast<Graphics*>(graphics)->drawImage(hStart, x, y);

        w -= hStart->getWidth() + hEnd->getWidth();
        x += hStart->getWidth();

        if (hMid)
        {
            static_cast<Graphics*>(graphics)->
                drawImagePattern(hMid, x, y, w, hMid->getHeight());
        }

        x += w;
        if (hEnd)
            static_cast<Graphics*>(graphics)->drawImage(hEnd, x, y);
    }
    else
    {
        static_cast<Graphics*>(graphics)->drawImage(hStartHi, x, y);

        w -= hStartHi->getWidth();
        if (hEndHi)
            w -= hEndHi->getWidth();
        x += hStartHi->getWidth();

        if (hMidHi)
        {
            static_cast<Graphics*>(graphics)->
                drawImagePattern(hMidHi, x, y, w, hMidHi->getHeight());
        }

        x += w;
        if (hEndHi)
            static_cast<Graphics*>(graphics)->drawImage(hEndHi, x, y);
    }

    drawMarker(graphics);
}

void Slider::drawMarker(gcn::Graphics *graphics)
{
    if (!(mHasMouse?hGripHi:hGrip))
        return;

    static_cast<Graphics*>(graphics)->
       drawImage(mHasMouse?hGripHi:hGrip, getMarkerPosition(),
       (getHeight() - (mHasMouse?hGripHi:hGrip)->getHeight()) / 2);
}

void Slider::mouseEntered(gcn::MouseEvent& event _UNUSED_)
{
    mHasMouse = true;
}

void Slider::mouseExited(gcn::MouseEvent& event _UNUSED_)
{
    mHasMouse = false;
}

