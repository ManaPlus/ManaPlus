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

#include "gui/widgets/scrollarea.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "gui/theme.h"

#include "resources/image.h"

#include "utils/dtor.h"

#include "debug.h"

int ScrollArea::instances = 0;
float ScrollArea::mAlpha = 1.0;
ImageRect ScrollArea::background;
ImageRect ScrollArea::vMarker;
ImageRect ScrollArea::vMarkerHi;
Image *ScrollArea::buttons[4][2];

static std::string const buttonFiles[2] =
{
    "scrollbuttons.xml",
    "scrollbuttons_pressed.xml"
};

ScrollArea::ScrollArea():
    gcn::ScrollArea(),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mHasMouse(false),
    mOpaque(true),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0)
{
    addWidgetListener(this);
    init();
}

ScrollArea::ScrollArea(gcn::Widget *widget):
    gcn::ScrollArea(widget),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mHasMouse(false),
    mOpaque(true),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0)
{
    init();
}

ScrollArea::~ScrollArea()
{
    // Garbage collection
    delete getContent();

    instances--;
    if (instances == 0 && Theme::instance())
    {
        Theme *theme = Theme::instance();
        theme->unloadRect(background);
        theme->unloadRect(vMarker);
        theme->unloadRect(vMarkerHi);
        for (int i = 0; i < 2; i ++)
        {
            for (int f = UP; f < BUTTONS_DIR; f ++)
            {
                if (buttons[f][i])
                    buttons[f][i]->decRef();
            }
        }
    }

    delete mVertexes;
    mVertexes = nullptr;
}

void ScrollArea::init()
{
    // Draw background by default
    setOpaque(true);

    setUpButtonScrollAmount(2);
    setDownButtonScrollAmount(2);
    setLeftButtonScrollAmount(2);
    setRightButtonScrollAmount(2);

    if (instances == 0)
    {
        for (int f = 0; f < 9; f ++)
        {
            background.grid[f] = nullptr;
            vMarker.grid[f] = nullptr;
            vMarkerHi.grid[f] = nullptr;
        }

        if (Theme::instance())
        {
            Theme::instance()->loadRect(background, "scroll_background.xml");
            Theme::instance()->loadRect(vMarker, "scroll.xml");
            Theme::instance()->loadRect(vMarkerHi, "scroll_highlighted.xml");
        }

        for (int i = 0; i < 2; i ++)
        {
            Skin *skin = Theme::instance()->load(buttonFiles[i]);
            if (skin)
            {
                const ImageRect &rect = skin->getBorder();
                for (int f = UP; f < BUTTONS_DIR; f ++)
                {
                    rect.grid[f]->incRef();
                    buttons[f][i] = rect.grid[f];
                }
            }
            else
            {
                for (int f = UP; f < BUTTONS_DIR; f ++)
                    buttons[f][i] = nullptr;
            }
            Theme::instance()->unload(skin);
        }
    }

    instances++;

    mGray = Theme::getThemeColor(Theme::SCROLLBAR_GRAY, 32);
    mBackground = Theme::getThemeColor(Theme::BACKGROUND);
}

void ScrollArea::logic()
{
    if (!isVisible())
        return;

    gcn::ScrollArea::logic();
    gcn::Widget *content = getContent();

    // When no scrollbar in a certain direction, adapt content size to match
    // the content dimension exactly.
    if (content)
    {
        if (getHorizontalScrollPolicy() == gcn::ScrollArea::SHOW_NEVER)
        {
            content->setWidth(getChildrenArea().width -
                    2 * content->getFrameSize());
        }
        if (getVerticalScrollPolicy() == gcn::ScrollArea::SHOW_NEVER)
        {
            content->setHeight(getChildrenArea().height -
                    2 * content->getFrameSize());
        }
    }

    if (mUpButtonPressed)
    {
        setVerticalScrollAmount(getVerticalScrollAmount() -
                                mUpButtonScrollAmount);
    }
    else if (mDownButtonPressed)
    {
        setVerticalScrollAmount(getVerticalScrollAmount() +
                                mDownButtonScrollAmount);
    }
    else if (mLeftButtonPressed)
    {
        setHorizontalScrollAmount(getHorizontalScrollAmount() -
                                  mLeftButtonScrollAmount);
    }
    else if (mRightButtonPressed)
    {
        setHorizontalScrollAmount(getHorizontalScrollAmount() +
                                  mRightButtonScrollAmount);
    }
}

void ScrollArea::updateAlpha()
{
    float alpha = std::max(Client::getGuiAlpha(),
                           Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            if (background.grid[a])
                background.grid[a]->setAlpha(mAlpha);
            if (vMarker.grid[a])
                vMarker.grid[a]->setAlpha(mAlpha);
            if (vMarkerHi.grid[a])
                vMarkerHi.grid[a]->setAlpha(mAlpha);
        }
    }
}

void ScrollArea::draw(gcn::Graphics *graphics)
{
    if (mVBarVisible)
    {
        drawUpButton(graphics);
        drawDownButton(graphics);
        drawVBar(graphics);
        drawVMarker(graphics);
    }

    if (mHBarVisible)
    {
        drawLeftButton(graphics);
        drawRightButton(graphics);
        drawHBar(graphics);
        drawHMarker(graphics);
    }


    if (mHBarVisible && mVBarVisible)
    {
        graphics->setColor(getBaseColor());
        graphics->fillRectangle(gcn::Rectangle(getWidth() - mScrollbarWidth,
                    getHeight() - mScrollbarWidth,
                    mScrollbarWidth,
                    mScrollbarWidth));
    }

    updateAlpha();

    drawChildren(graphics);
}

//void ScrollArea::drawFrame(gcn::Graphics *graphics A_UNUSED)
void ScrollArea::drawFrame(gcn::Graphics *graphics)
{
    if (mOpaque)
    {
        const int bs = getFrameSize();
        const int w = getWidth() + bs * 2;
        const int h = getHeight() + bs * 2;

        bool recalc = false;
        if (mRedraw)
        {
            recalc = true;
        }
        else
        {
            // because we don't know where parent windows was moved,
            // need recalc vertexes
            gcn::ClipRectangle &rect = static_cast<Graphics*>(
                graphics)->getTopClip();
            if (rect.xOffset != mXOffset || rect.yOffset != mYOffset)
            {
                recalc = true;
                mXOffset = rect.xOffset;
                mYOffset = rect.yOffset;
            }
            else if (rect.width != mDrawWidth || rect.height != mDrawHeight)
            {
                recalc = true;
                mDrawWidth = rect.width;
                mDrawHeight = rect.height;
            }
            else if (static_cast<Graphics*>(graphics)->getRedraw())
            {
                recalc = true;
            }
        }

        if (recalc)
        {
            mRedraw = false;
            static_cast<Graphics*>(graphics)->calcWindow(
                mVertexes, 0, 0, w, h, background);
        }


        static_cast<Graphics*>(graphics)->
            drawImageRect2(mVertexes, background);

//        static_cast<Graphics*>(graphics)->
//            drawImageRect(0, 0, w, h, background);
    }
}

void ScrollArea::setOpaque(bool opaque)
{
    mOpaque = opaque;
    setFrameSize(mOpaque ? 2 : 0);
}

void ScrollArea::drawButton(gcn::Graphics *graphics, BUTTON_DIR dir)
{
    int state = 0;
    gcn::Rectangle dim;

    switch (dir)
    {
        case UP:
            state = mUpButtonPressed ? 1 : 0;
            dim = getUpButtonDimension();
            break;
        case DOWN:
            state = mDownButtonPressed ? 1 : 0;
            dim = getDownButtonDimension();
            break;
        case LEFT:
            state = mLeftButtonPressed ? 1 : 0;
            dim = getLeftButtonDimension();
            break;
        case RIGHT:
            state = mRightButtonPressed ? 1 : 0;
            dim = getRightButtonDimension();
            break;
        default:
            logger->log("ScrollArea::drawButton unknown dir: "
                        + toString(static_cast<unsigned>(dir)));
            break;
    }

    if (buttons[dir][state])
    {
        static_cast<Graphics*>(graphics)->
            drawImage(buttons[dir][state], dim.x, dim.y);
    }
}

void ScrollArea::drawUpButton(gcn::Graphics *graphics)
{
    drawButton(graphics, UP);
}

void ScrollArea::drawDownButton(gcn::Graphics *graphics)
{
    drawButton(graphics, DOWN);
}

void ScrollArea::drawLeftButton(gcn::Graphics *graphics)
{
    drawButton(graphics, LEFT);
}

void ScrollArea::drawRightButton(gcn::Graphics *graphics)
{
    drawButton(graphics, RIGHT);
}

void ScrollArea::drawVBar(gcn::Graphics *graphics)
{
    const gcn::Rectangle dim = getVerticalBarDimension();
    graphics->setColor(mGray);
    graphics->fillRectangle(dim);
    graphics->setColor(mBackground);
}

void ScrollArea::drawHBar(gcn::Graphics *graphics)
{
    const gcn::Rectangle dim = getHorizontalBarDimension();
    graphics->setColor(mGray);
    graphics->fillRectangle(dim);
    graphics->setColor(mBackground);
}

void ScrollArea::drawVMarker(gcn::Graphics *graphics)
{
    gcn::Rectangle dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (getWidth() - getScrollbarWidth())))
    {
        static_cast<Graphics*>(graphics)->
            drawImageRect(dim.x, dim.y, dim.width, dim.height, vMarkerHi);
    }
    else
    {
        static_cast<Graphics*>(graphics)->
            drawImageRect(dim.x, dim.y, dim.width, dim.height, vMarker);
    }
}

void ScrollArea::drawHMarker(gcn::Graphics *graphics)
{
    gcn::Rectangle dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (getHeight() - getScrollbarWidth())))
    {
        static_cast<Graphics*>(graphics)->
            drawImageRect(dim.x, dim.y, dim.width, dim.height, vMarkerHi);
    }
    else
    {
        static_cast<Graphics*>(graphics)->
            drawImageRect(dim.x, dim.y, dim.width, dim.height, vMarker);
    }
}

void ScrollArea::mouseMoved(gcn::MouseEvent& event)
{
    mX = event.getX();
    mY = event.getY();
}

void ScrollArea::mouseEntered(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void ScrollArea::mouseExited(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void ScrollArea::widgetResized(const gcn::Event &event A_UNUSED)
{
    mRedraw = true;
    getContent()->setSize(getWidth() - 2 * getFrameSize(),
                          getHeight() - 2 * getFrameSize());
}

void ScrollArea::widgetMoved(const gcn::Event& event A_UNUSED)
{
    mRedraw = true;
}

void ScrollArea::mousePressed(gcn::MouseEvent& event)
{
    gcn::ScrollArea::mousePressed(event);
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        mClickX = event.getX();
        mClickY = event.getY();
    }
}

void ScrollArea::mouseReleased(gcn::MouseEvent& event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT && mClickX && mClickY)
    {
        if (!event.isConsumed())
        {
            int dx = event.getX() - mClickX;
            int dy = event.getY() - mClickY;

            if ((dx < 10 && dx > 0) || (dx > -10 && dx < 0))
                dx = 0;

            if ((dy < 10 && dy > 0) || (dy > -10 && dy < 0))
                dy = 0;

            if (dx)
            {
                int s = getHorizontalScrollAmount() + dx;
                if (s < 0)
                    s = 0;
                else if (s > getHorizontalMaxScroll())
                    s = getHorizontalMaxScroll();

                setHorizontalScrollAmount(s);
            }
            if (dy)
            {
                int s = getVerticalScrollAmount() + dy;
                if (s < 0)
                    s = 0;
                else if (s > getVerticalMaxScroll())
                    s = getVerticalMaxScroll();

                setVerticalScrollAmount(s);
            }
            mClickX = 0;
            mClickY = 0;
            event.consume();
        }
    }
    gcn::ScrollArea::mouseReleased(event);
}
