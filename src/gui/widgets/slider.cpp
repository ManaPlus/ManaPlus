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

#include "gui/widgets/slider.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/gui.h"

#include "resources/image.h"

#include "debug.h"

ImageRect Slider::buttons[2];
float Slider::mAlpha = 1.0;
int Slider::mInstances = 0;

static std::string const data[2] =
{
    "slider.xml",
    "slider_highlighted.xml"
};

Slider::Slider(Widget2 *const widget,
               const double scaleEnd) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mValue(0),
    mStepLength(scaleEnd / 10),
    mScaleStart(0),
    mScaleEnd(scaleEnd),
    mOrientation(HORIZONTAL),
    mVertexes(new ImageCollection),
    mMarkerLength(10),
    mHasMouse(false),
    mRedraw(true)
{
    init();
}

Slider::Slider(Widget2 *const widget,
               const double scaleStart,
               const double scaleEnd) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mValue(scaleStart),
    mStepLength((scaleEnd - scaleStart) / 10),
    mScaleStart(scaleStart),
    mScaleEnd(scaleEnd),
    mOrientation(HORIZONTAL),
    mVertexes(new ImageCollection),
    mMarkerLength(10),
    mHasMouse(false),
    mRedraw(true)
{
    init();
}

Slider::~Slider()
{
    if (gui)
        gui->removeDragged(this);

    delete mVertexes;
    mVertexes = nullptr;
    mInstances--;
    if (mInstances == 0 && Theme::instance())
    {
        for (int mode = 0; mode < 2; mode ++)
            Theme::unloadRect(buttons[mode]);
    }
}

void Slider::init()
{
    setFocusable(true);
    setFrameSize(1);

    addMouseListener(this);
    addKeyListener(this);

    setFrameSize(0);

    // Load resources
    if (mInstances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
        {
            for (int mode = 0; mode < 2; mode ++)
                theme->loadRect(buttons[mode], data[mode], "slider.xml", 0, 8);
        }
        updateAlpha();
    }

    mInstances++;

    if (buttons[0].grid[HGRIP])
        setMarkerLength(buttons[0].grid[HGRIP]->getWidth());
}

void Slider::updateAlpha()
{
    const float alpha = std::max(client->getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int f = 0; f < 2; f ++)
        {
            for (int d = 0; d < SLIDER_MAX; d ++)
            {
                if (buttons[f].grid[d])
                    buttons[f].grid[d]->setAlpha(mAlpha);
            }
        }
    }
}

void Slider::draw(Graphics *graphics)
{
    BLOCK_START("Slider::draw")
    if (!buttons[0].grid[HSTART] || !buttons[1].grid[HSTART]
        || !buttons[0].grid[HEND])
    {
        BLOCK_END("Slider::draw")
        return;
    }

    int w = getWidth();
    const int h = getHeight();
    int x = 0;
    const int y = mHasMouse ? (h - buttons[1].grid[HSTART]->getHeight()) / 2 :
        (h - buttons[0].grid[HSTART]->getHeight()) / 2;

    updateAlpha();

    if (isBatchDrawRenders(openGLMode))
    {
        if (mRedraw || graphics->getRedraw())
        {
            mRedraw = false;
            mVertexes->clear();
            if (!mHasMouse)
            {
                graphics->calcTileCollection(mVertexes,
                    buttons[0].grid[HSTART], x, y);

                const int width = buttons[0].grid[HSTART]->getWidth();
                w -= width + buttons[0].grid[HEND]->getWidth();
                x += width;

                if (buttons[0].grid[HMID])
                {
                    const Image *const hMid = buttons[0].grid[HMID];
                    graphics->calcPattern(mVertexes,
                        hMid,
                        x, y,
                        w, hMid->getHeight());
                }

                x += w;
                graphics->calcTileCollection(mVertexes,
                    buttons[0].grid[HEND],
                    x, y);

                const Image *const img = buttons[0].grid[HGRIP];
                if (img)
                {
                    graphics->calcTileCollection(mVertexes,
                        img,
                        getMarkerPosition(),
                        (mDimension.height - img->getHeight()) / 2);
                }
            }
            else
            {
                graphics->calcTileCollection(mVertexes,
                    buttons[1].grid[HSTART],
                    x, y);

                const int width = buttons[1].grid[HSTART]->getWidth();
                w -= width;
                if (buttons[1].grid[HEND])
                    w -= buttons[1].grid[HEND]->getWidth();
                x += width;

                if (buttons[1].grid[HMID])
                {
                    const Image *const hMid = buttons[1].grid[HMID];
                    graphics->calcPattern(mVertexes,
                        hMid,
                        x, y,
                        w, hMid->getHeight());
                }

                x += w;
                if (buttons[1].grid[HEND])
                {
                    graphics->calcTileCollection(mVertexes,
                        buttons[1].grid[HEND], x, y);
                }

                const Image *const img = buttons[1].grid[HGRIP];
                if (img)
                {
                    graphics->calcTileCollection(mVertexes,
                        img,
                        getMarkerPosition(),
                        (mDimension.height - img->getHeight()) / 2);
                }
            }
        }
        graphics->drawTileCollection(mVertexes);
    }
    else
    {
        if (!mHasMouse)
        {
            graphics->drawImage(buttons[0].grid[HSTART], x, y);
            const int width = buttons[0].grid[HSTART]->getWidth();
            w -= width + buttons[0].grid[HEND]->getWidth();
            x += width;

            if (buttons[0].grid[HMID])
            {
                const Image *const hMid = buttons[0].grid[HMID];
                graphics->drawPattern(hMid, x, y, w, hMid->getHeight());
            }

            x += w;
            graphics->drawImage(buttons[0].grid[HEND], x, y);

            const Image *const img = buttons[0].grid[HGRIP];
            if (img)
            {
                graphics->drawImage(img, getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
        else
        {
            graphics->drawImage(buttons[1].grid[HSTART], x, y);

            const int width = buttons[1].grid[HSTART]->getWidth();
            w -= width;
            if (buttons[1].grid[HEND])
                w -= buttons[1].grid[HEND]->getWidth();
            x += width;

            if (buttons[1].grid[HMID])
            {
                const Image *const hMid = buttons[1].grid[HMID];
                graphics->drawPattern(hMid, x, y, w, hMid->getHeight());
            }

            x += w;
            if (buttons[1].grid[HEND])
                graphics->drawImage(buttons[1].grid[HEND], x, y);

            const Image *const img = buttons[1].grid[HGRIP];
            if (img)
            {
                graphics->drawImage(img, getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
    }

    BLOCK_END("Slider::draw")
}

void Slider::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
    mRedraw = true;
}

void Slider::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
    mRedraw = true;
}

void Slider::mousePressed(MouseEvent &mouseEvent)
{
    const int x = mouseEvent.getX();
    const int y = mouseEvent.getY();
    const int width = mDimension.width;
    const int height = mDimension.height;

    if (mouseEvent.getButton() == MouseEvent::LEFT
        && x >= 0 && x <= width && y >= 0 && y <= height)
    {
        if (mOrientation == HORIZONTAL)
            setValue(markerPositionToValue(x - mMarkerLength / 2));
        else
            setValue(markerPositionToValue(height - y - mMarkerLength / 2));

        distributeActionEvent();
    }
}

void Slider::mouseDragged(MouseEvent &mouseEvent)
{
    if (mOrientation == HORIZONTAL)
    {
        setValue(markerPositionToValue(mouseEvent.getX() - mMarkerLength / 2));
    }
    else
    {
        setValue(markerPositionToValue(
            mDimension.height - mouseEvent.getY() - mMarkerLength / 2));
    }

    distributeActionEvent();

    mouseEvent.consume();
}

void Slider::mouseWheelMovedUp(MouseEvent &mouseEvent)
{
    setValue(mValue + mStepLength);
    distributeActionEvent();
    mouseEvent.consume();
}

void Slider::mouseWheelMovedDown(MouseEvent &mouseEvent)
{
    setValue(mValue - mStepLength);
    distributeActionEvent();
    mouseEvent.consume();
}

void Slider::keyPressed(KeyEvent& keyEvent)
{
    const int action = keyEvent.getActionId();

    if (mOrientation == HORIZONTAL)
    {
        if (action == Input::KEY_GUI_RIGHT)
        {
            setValue(mValue + mStepLength);
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_LEFT)
        {
            setValue(mValue - mStepLength);
            distributeActionEvent();
            keyEvent.consume();
        }
    }
    else
    {
        if (action == Input::KEY_GUI_UP)
        {
            setValue(mValue + mStepLength);
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_DOWN)
        {
            setValue(mValue - mStepLength);
            distributeActionEvent();
            keyEvent.consume();
        }
    }
}

void Slider::setScale(const double scaleStart, const double scaleEnd)
{
    mScaleStart = scaleStart;
    mScaleEnd = scaleEnd;
}

void Slider::setValue(const double value)
{
    mRedraw = true;
    if (value > mScaleEnd)
    {
        mValue = mScaleEnd;
        return;
    }

    if (value < mScaleStart)
    {
        mValue = mScaleStart;
        return;
    }

    mValue = value;
}

double Slider::markerPositionToValue(const int v) const
{
    int w;
    if (mOrientation == HORIZONTAL)
        w = mDimension.width;
    else
        w = mDimension.height;

    const double pos = v / (static_cast<double>(w) - mMarkerLength);
    return (1.0 - pos) * mScaleStart + pos * mScaleEnd;
}

int Slider::valueToMarkerPosition(const double value) const
{
    int v;
    if (mOrientation == HORIZONTAL)
        v = mDimension.width;
    else
        v = mDimension.height;

    const int w = static_cast<int>((v - mMarkerLength)
            * (value  - mScaleStart)
            / (mScaleEnd - mScaleStart));

    if (w < 0)
        return 0;

    if (w > v - mMarkerLength)
        return v - mMarkerLength;

    return w;
}
