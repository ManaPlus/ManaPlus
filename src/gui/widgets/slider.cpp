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

#include "gui/widgets/slider.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "input/keydata.h"
#include "input/keyevent.h"

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

Slider::Slider(const double scaleEnd) :
    gcn::Slider(scaleEnd),
    Widget2(),
    mVertexes(new ImageCollection),
    mHasMouse(false),
    mRedraw(true)
{
    init();
}

Slider::Slider(const double scaleStart, const double scaleEnd) :
    gcn::Slider(scaleStart, scaleEnd),
    Widget2(),
    mVertexes(new ImageCollection),
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

void Slider::draw(gcn::Graphics *graphics)
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
    Graphics *const g = static_cast<Graphics*>(graphics);

    updateAlpha();

    if (isBatchDrawRenders(openGLMode))
    {
        if (mRedraw || g->getRedraw())
        {
            mRedraw = false;
            mVertexes->clear();
            if (!mHasMouse)
            {
                g->calcTileCollection(mVertexes,
                    buttons[0].grid[HSTART], x, y);

                const int width = buttons[0].grid[HSTART]->getWidth();
                w -= width + buttons[0].grid[HEND]->getWidth();
                x += width;

                if (buttons[0].grid[HMID])
                {
                    const Image *const hMid = buttons[0].grid[HMID];
                    g->calcPattern(mVertexes, hMid, x, y,
                        w, hMid->getHeight());
                }

                x += w;
                g->calcTileCollection(mVertexes, buttons[0].grid[HEND], x, y);

                const Image *const img = buttons[0].grid[HGRIP];
                if (img)
                {
                    g->calcTileCollection(mVertexes, img, getMarkerPosition(),
                        (mDimension.height - img->getHeight()) / 2);
                }
            }
            else
            {
                g->calcTileCollection(mVertexes,
                    buttons[1].grid[HSTART], x, y);

                const int width = buttons[1].grid[HSTART]->getWidth();
                w -= width;
                if (buttons[1].grid[HEND])
                    w -= buttons[1].grid[HEND]->getWidth();
                x += width;

                if (buttons[1].grid[HMID])
                {
                    const Image *const hMid = buttons[1].grid[HMID];
                    g->calcPattern(mVertexes, hMid, x, y,
                        w, hMid->getHeight());
                }

                x += w;
                if (buttons[1].grid[HEND])
                {
                    g->calcTileCollection(mVertexes,
                        buttons[1].grid[HEND], x, y);
                }

                const Image *const img = buttons[1].grid[HGRIP];
                if (img)
                {
                    g->calcTileCollection(mVertexes, img, getMarkerPosition(),
                        (mDimension.height - img->getHeight()) / 2);
                }
            }
        }
        g->drawTileCollection(mVertexes);
    }
    else
    {
        if (!mHasMouse)
        {
            g->drawImage2(buttons[0].grid[HSTART], x, y);
            const int width = buttons[0].grid[HSTART]->getWidth();
            w -= width + buttons[0].grid[HEND]->getWidth();
            x += width;

            if (buttons[0].grid[HMID])
            {
                const Image *const hMid = buttons[0].grid[HMID];
                g->drawPattern(hMid, x, y, w, hMid->getHeight());
            }

            x += w;
            g->drawImage2(buttons[0].grid[HEND], x, y);

            const Image *const img = buttons[0].grid[HGRIP];
            if (img)
            {
                g->drawImage2(img, getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
        else
        {
            g->drawImage2(buttons[1].grid[HSTART], x, y);

            const int width = buttons[1].grid[HSTART]->getWidth();
            w -= width;
            if (buttons[1].grid[HEND])
                w -= buttons[1].grid[HEND]->getWidth();
            x += width;

            if (buttons[1].grid[HMID])
            {
                const Image *const hMid = buttons[1].grid[HMID];
                g->drawPattern(hMid, x, y, w, hMid->getHeight());
            }

            x += w;
            if (buttons[1].grid[HEND])
                g->drawImage2(buttons[1].grid[HEND], x, y);

            const Image *const img = buttons[1].grid[HGRIP];
            if (img)
            {
                g->drawImage2(img, getMarkerPosition(),
                    (mDimension.height - img->getHeight()) / 2);
            }
        }
    }

    BLOCK_END("Slider::draw")
}

void Slider::mouseEntered(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
    mRedraw = true;
}

void Slider::mouseExited(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
    mRedraw = true;
}

void Slider::mousePressed(gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.getButton() == gcn::MouseEvent::LEFT
        && mouseEvent.getX() >= 0
        && mouseEvent.getX() <= getWidth()
        && mouseEvent.getY() >= 0
        && mouseEvent.getY() <= getHeight())
    {
        if (getOrientation() == HORIZONTAL)
        {
            setValue2(markerPositionToValue(
                mouseEvent.getX() - getMarkerLength() / 2));
        }
        else
        {
            setValue2(markerPositionToValue(getHeight()
                - mouseEvent.getY() - getMarkerLength() / 2));
        }

        distributeActionEvent();
    }
}

void Slider::mouseDragged(gcn::MouseEvent &mouseEvent)
{
    if (getOrientation() == HORIZONTAL)
    {
        setValue2(markerPositionToValue(mouseEvent.getX()
            - getMarkerLength() / 2));
    }
    else
    {
        setValue2(markerPositionToValue(getHeight()
            - mouseEvent.getY() - getMarkerLength() / 2));
    }

    distributeActionEvent();

    mouseEvent.consume();
}

void Slider::mouseWheelMovedUp(gcn::MouseEvent &mouseEvent)
{
    setValue2(getValue() + getStepLength());
    distributeActionEvent();

    mouseEvent.consume();
}

void Slider::mouseWheelMovedDown(gcn::MouseEvent &mouseEvent)
{
    setValue2(getValue() - getStepLength());
    distributeActionEvent();

    mouseEvent.consume();
}

void Slider::keyPressed(gcn::KeyEvent& keyEvent)
{
    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    if (getOrientation() == HORIZONTAL)
    {
        if (action == Input::KEY_GUI_RIGHT)
        {
            setValue2(getValue() + getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_LEFT)
        {
            setValue2(getValue() - getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
    }
    else
    {
        if (action == Input::KEY_GUI_UP)
        {
            setValue2(getValue() + getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_DOWN)
        {
            setValue2(getValue() - getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
    }
}

void Slider::setValue2(const double value)
{
    setValue(value);
    mRedraw = true;
}
