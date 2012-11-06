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
#include "graphicsvertexes.h"

#include "utils/dtor.h"

#include "debug.h"

int ScrollArea::instances = 0;
float ScrollArea::mAlpha = 1.0;
ImageRect ScrollArea::background;
ImageRect ScrollArea::vMarker;
ImageRect ScrollArea::vMarkerHi;
ImageRect ScrollArea::vBackground;
ImageRect ScrollArea::hBackground;
Image *ScrollArea::buttons[4][2];

static std::string const buttonFiles[2] =
{
    "scrollbuttons.xml",
    "scrollbuttons_pressed.xml"
};

ScrollArea::ScrollArea(const bool opaque, const std::string &skin) :
    gcn::ScrollArea(),
    gcn::WidgetListener(),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mHasMouse(false),
    mOpaque(opaque),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0)
{
    addWidgetListener(this);
    init(skin);
}

ScrollArea::ScrollArea(gcn::Widget *const widget, const bool opaque,
                       const std::string &skin) :
    gcn::ScrollArea(widget),
    gcn::WidgetListener(),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mHasMouse(false),
    mOpaque(opaque),
    mVertexes(new GraphicsVertexes()),
    mRedraw(true),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0)
{
    init(skin);
}

ScrollArea::~ScrollArea()
{
    // Garbage collection
    delete getContent();

    instances--;
    const Theme *const theme = Theme::instance();
    if (theme)
    {
        theme->unloadRect(background);
        if (instances == 0)
        {
            theme->unloadRect(vMarker);
            theme->unloadRect(vMarkerHi);
            theme->unloadRect(vBackground);
            theme->unloadRect(hBackground);
            for (int i = 0; i < 2; i ++)
            {
                for (int f = UP; f < BUTTONS_DIR; f ++)
                {
                    if (buttons[f][i])
                        buttons[f][i]->decRef();
                }
            }
        }
    }

    delete mVertexes;
    mVertexes = nullptr;
}

void ScrollArea::init(std::string skinName)
{
    setOpaque(mOpaque);

    setUpButtonScrollAmount(2);
    setDownButtonScrollAmount(2);
    setLeftButtonScrollAmount(2);
    setRightButtonScrollAmount(2);

    if (skinName == "")
        skinName = "scroll_background.xml";
    Theme *const theme = Theme::instance();
    if (theme)
        theme->loadRect(background, skinName, "scroll_background.xml");

    if (instances == 0)
    {
        for (int f = 0; f < 9; f ++)
        {
            background.grid[f] = nullptr;
            vMarker.grid[f] = nullptr;
            vMarkerHi.grid[f] = nullptr;
            vBackground.grid[f] = nullptr;
            hBackground.grid[f] = nullptr;
        }

        if (theme)
        {
            theme->loadRect(vMarker, "scroll.xml", "");
            theme->loadRect(vMarkerHi, "scroll_highlighted.xml", "scroll.xml");
            theme->loadRect(vBackground, "scroll_vbackground.xml", "");
            theme->loadRect(hBackground, "scroll_hbackground.xml", "");
        }

        for (int i = 0; i < 2; i ++)
        {
            Skin *const skin = Theme::instance()->load(
                buttonFiles[i], "scrollbuttons.xml");
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
}

void ScrollArea::logic()
{
    BLOCK_START("ScrollArea::logic")
    if (!isVisible())
    {
        BLOCK_END("ScrollArea::logic")
        return;
    }

    gcn::ScrollArea::logic();
    gcn::Widget *const content = getContent();

    // When no scrollbar in a certain direction, adapt content size to match
    // the content dimension exactly.
    if (content)
    {
        const unsigned int frameSize = 2 * content->getFrameSize();
        if (mHPolicy == gcn::ScrollArea::SHOW_NEVER)
        {
            content->setWidth((mVBarVisible ? (mDimension.width
                - mScrollbarWidth) : mDimension.width) - frameSize);
        }
        if (mVPolicy == gcn::ScrollArea::SHOW_NEVER)
        {
            content->setHeight((mHBarVisible ? (mDimension.height
                - mScrollbarWidth) : mDimension.height) - frameSize);
        }
    }

    if (mUpButtonPressed)
        setVerticalScrollAmount(mVScroll - mUpButtonScrollAmount);
    else if (mDownButtonPressed)
        setVerticalScrollAmount(mVScroll + mDownButtonScrollAmount);
    else if (mLeftButtonPressed)
        setHorizontalScrollAmount(mHScroll - mLeftButtonScrollAmount);
    else if (mRightButtonPressed)
        setHorizontalScrollAmount(mHScroll + mRightButtonScrollAmount);
    BLOCK_END("ScrollArea::logic")
}

void ScrollArea::updateAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int a = 0; a < 9; a++)
        {
            if (background.grid[a])
                background.grid[a]->setAlpha(mAlpha);
            if (hBackground.grid[a])
                hBackground.grid[a]->setAlpha(mAlpha);
            if (vBackground.grid[a])
                vBackground.grid[a]->setAlpha(mAlpha);
            if (vMarker.grid[a])
                vMarker.grid[a]->setAlpha(mAlpha);
            if (vMarkerHi.grid[a])
                vMarkerHi.grid[a]->setAlpha(mAlpha);
        }
    }
}

void ScrollArea::draw(gcn::Graphics *graphics)
{
    BLOCK_START("ScrollArea::draw")
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

    updateAlpha();
    drawChildren(graphics);
    BLOCK_END("ScrollArea::draw")
}

void ScrollArea::drawFrame(gcn::Graphics *graphics)
{
    BLOCK_START("ScrollArea::drawFrame")
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
            const gcn::ClipRectangle &rect = static_cast<Graphics*>(
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
    BLOCK_END("ScrollArea::drawFrame")
}

void ScrollArea::setOpaque(bool opaque)
{
    mOpaque = opaque;
    setFrameSize(mOpaque ? 2 : 0);
}

void ScrollArea::drawButton(gcn::Graphics *const graphics,
                            const BUTTON_DIR dir)
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
        case BUTTONS_DIR:
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

void ScrollArea::drawUpButton(gcn::Graphics *const graphics)
{
    drawButton(graphics, UP);
}

void ScrollArea::drawDownButton(gcn::Graphics *const graphics)
{
    drawButton(graphics, DOWN);
}

void ScrollArea::drawLeftButton(gcn::Graphics *const graphics)
{
    drawButton(graphics, LEFT);
}

void ScrollArea::drawRightButton(gcn::Graphics *const graphics)
{
    drawButton(graphics, RIGHT);
}

void ScrollArea::drawVBar(gcn::Graphics *const graphics)
{
    const gcn::Rectangle &dim = getVerticalBarDimension();
    Graphics *const g = static_cast<Graphics*>(graphics);

    if (vBackground.grid[4])
    {
        g->drawImagePattern(vBackground.grid[4],
            dim.x, dim.y, dim.width, dim.height);
    }
    if (vBackground.grid[1])
    {
        g->drawImagePattern(vBackground.grid[1],
            dim.x, dim.y, dim.width, vBackground.grid[1]->getHeight());
    }
    if (vBackground.grid[7])
    {
        g->drawImagePattern(vBackground.grid[7],
            dim.x, dim.height - vBackground.grid[7]->getHeight() + dim.y,
            dim.width, vBackground.grid[7]->getHeight());
    }
}

void ScrollArea::drawHBar(gcn::Graphics *const graphics)
{
    const gcn::Rectangle &dim = getHorizontalBarDimension();
    Graphics *const g = static_cast<Graphics*>(graphics);

    if (hBackground.grid[4])
    {
        g->drawImagePattern(hBackground.grid[4],
            dim.x, dim.y, dim.width, dim.height);
    }

    if (hBackground.grid[3])
    {
        g->drawImagePattern(hBackground.grid[3],
            dim.x, dim.y, hBackground.grid[3]->getWidth(), dim.height);
    }

    if (hBackground.grid[5])
    {
        g->drawImagePattern(hBackground.grid[5],
            dim.x + dim.width - hBackground.grid[5]->getWidth(), dim.y,
            hBackground.grid[5]->getWidth(), dim.height);
    }
}

void ScrollArea::drawVMarker(gcn::Graphics *const graphics)
{
    const gcn::Rectangle &dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (getWidth() - mScrollbarWidth)))
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

void ScrollArea::drawHMarker(gcn::Graphics *const graphics)
{
    const gcn::Rectangle dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (getHeight() - mScrollbarWidth)))
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
    const unsigned int frameSize = 2 * getFrameSize();
    getContent()->setSize(getWidth() - frameSize, getHeight() - frameSize);
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
                int s = mHScroll + dx;
                if (s < 0)
                {
                    s = 0;
                }
                else
                {
                    const int maxH = getHorizontalMaxScroll();
                    if (s > maxH)
                        s = maxH;
                }

                setHorizontalScrollAmount(s);
            }
            if (dy)
            {
                int s = mVScroll + dy;
                if (s < 0)
                {
                    s = 0;
                }
                else
                {
                    const int maxV = getVerticalMaxScroll();
                    if (s > maxV)
                        s = maxV;
                }

                setVerticalScrollAmount(s);
            }
            mClickX = 0;
            mClickY = 0;
            event.consume();
        }
    }
    gcn::ScrollArea::mouseReleased(event);
}
