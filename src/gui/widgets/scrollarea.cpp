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

#include "gui/widgets/scrollarea.h"

#include "client.h"
#include "graphicsvertexes.h"

#include "gui/gui.h"

#include "resources/image.h"

#include "debug.h"

int ScrollArea::instances = 0;
float ScrollArea::mAlpha = 1.0;
bool ScrollArea::mShowButtons = true;
int ScrollArea::mMarkerSize = 0;
int ScrollArea::mScrollbarSize = 12;
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

ScrollArea::ScrollArea(Widget2 *const widget2,
                       Widget *const widget,
                       const bool opaque,
                       const std::string &skin) :
    gcn::ScrollArea(widget2, widget),
    WidgetListener(),
    mX(0),
    mY(0),
    mClickX(0),
    mClickY(0),
    mVertexes(new ImageCollection),
    mVertexes2(new ImageCollection),
    mXOffset(0),
    mYOffset(0),
    mDrawWidth(0),
    mDrawHeight(0),
    mHasMouse(false),
    mRedraw(true)
{
    mOpaque = opaque;
    init(skin);
}

ScrollArea::~ScrollArea()
{
    if (gui)
        gui->removeDragged(this);

    // Garbage collection
    delete getContent();

    instances--;
    if (instances == 0)
    {
        Theme::unloadRect(background);
        Theme::unloadRect(vMarker);
        Theme::unloadRect(vMarkerHi);
        Theme::unloadRect(vBackground);
        Theme::unloadRect(hBackground);
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
    delete mVertexes2;
    mVertexes2 = nullptr;
}

void ScrollArea::init(std::string skinName)
{
    setOpaque(mOpaque);

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
            vBackground.grid[f] = nullptr;
            hBackground.grid[f] = nullptr;
        }

        // +++ here probably need move background from static
        if (skinName == "")
            skinName = "scroll_background.xml";
        Theme *const theme = Theme::instance();
        if (theme)
        {
            theme->loadRect(background, skinName, "scroll_background.xml");
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
                    if (rect.grid[f])
                        rect.grid[f]->incRef();
                    buttons[f][i] = rect.grid[f];
                }
                if (i == 0)
                {
                    mShowButtons = (skin->getOption("showbuttons", 1) == 1);
                    mMarkerSize = skin->getOption("markersize", 0);
                    mScrollbarSize = skin->getOption("scrollbarsize", 12);
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
    mScrollbarWidth = mScrollbarSize;
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
    Widget *const content = getContent();

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
    const float alpha = std::max(client->getGuiAlpha(),
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

void ScrollArea::draw(Graphics *graphics)
{
    BLOCK_START("ScrollArea::draw")
    if (mVBarVisible || mHBarVisible)
    {
        if (isBatchDrawRenders(openGLMode))
        {
            if (!mOpaque)
                updateCalcFlag(graphics);
//            if (mRedraw)
            {
                mVertexes->clear();
                if (mVBarVisible)
                {
                    if (mShowButtons)
                    {
                        calcButton(graphics, UP);
                        calcButton(graphics, DOWN);
                    }
                    calcVBar(graphics);
                    calcVMarker(graphics);
                }

                if (mHBarVisible)
                {
                    if (mShowButtons)
                    {
                        calcButton(graphics, LEFT);
                        calcButton(graphics, RIGHT);
                    }
                    calcHBar(graphics);
                    calcHMarker(graphics);
                }
            }
            graphics->drawTileCollection(mVertexes);
        }
        else
        {
            if (mVBarVisible)
            {
                if (mShowButtons)
                {
                    drawButton(graphics, UP);
                    drawButton(graphics, DOWN);
                }
                drawVBar(graphics);
                drawVMarker(graphics);
            }

            if (mHBarVisible)
            {
                if (mShowButtons)
                {
                    drawButton(graphics, LEFT);
                    drawButton(graphics, RIGHT);
                }
                drawHBar(graphics);
                drawHMarker(graphics);
            }
        }
    }

    updateAlpha();

    if (mRedraw)
    {
        const bool redraw = graphics->getRedraw();
        graphics->setRedraw(true);
        drawChildren(graphics);
        graphics->setRedraw(redraw);
    }
    else
    {
        drawChildren(graphics);
    }
    mRedraw = false;
    BLOCK_END("ScrollArea::draw")
}

void ScrollArea::updateCalcFlag(Graphics *const graphics)
{
    if (!mRedraw)
    {
        // because we don't know where parent windows was moved,
        // need recalc vertexes
        const ClipRect &rect = graphics->getTopClip();
        if (rect.xOffset != mXOffset || rect.yOffset != mYOffset)
        {
            mRedraw = true;
            mXOffset = rect.xOffset;
            mYOffset = rect.yOffset;
        }
        else if (rect.width != mDrawWidth || rect.height != mDrawHeight)
        {
            mRedraw = true;
            mDrawWidth = rect.width;
            mDrawHeight = rect.height;
        }
        else if (graphics->getRedraw())
        {
            mRedraw = true;
        }
    }
}

void ScrollArea::drawFrame(Graphics *graphics)
{
    BLOCK_START("ScrollArea::drawFrame")
    if (mOpaque)
    {
        const int bs = mFrameSize * 2;
        const int w = mDimension.width + bs;
        const int h = mDimension.height + bs;

        updateCalcFlag(graphics);

        if (isBatchDrawRenders(openGLMode))
        {
            if (mRedraw)
            {
                mVertexes2->clear();
                graphics->calcWindow(mVertexes2,
                    0, 0,
                    w, h,
                    background);
            }
            graphics->drawTileCollection(mVertexes2);
        }
        else
        {
            graphics->drawImageRect(0, 0,
                w, h,
                background);
        }
    }
    BLOCK_END("ScrollArea::drawFrame")
}

void ScrollArea::setOpaque(bool opaque)
{
    mOpaque = opaque;
    setFrameSize(mOpaque ? 2 : 0);
}

void ScrollArea::drawButton(Graphics *const graphics,
                            const BUTTON_DIR dir)
{
    int state = 0;
    Rect dim;

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
            return;
    }

    if (buttons[dir][state])
        graphics->drawImage(buttons[dir][state], dim.x, dim.y);
}

void ScrollArea::calcButton(Graphics *const graphics,
                            const BUTTON_DIR dir)
{
    int state = 0;
    Rect dim;

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
            return;
    }

    if (buttons[dir][state])
    {
        static_cast<Graphics*>(graphics)->calcTileCollection(
            mVertexes, buttons[dir][state], dim.x, dim.y);
    }
}

void ScrollArea::drawVBar(Graphics *const graphics)
{
    const Rect &dim = getVerticalBarDimension();

    if (vBackground.grid[4])
    {
        graphics->drawPattern(vBackground.grid[4],
            dim.x, dim.y, dim.width, dim.height);
    }
    if (vBackground.grid[1])
    {
        graphics->drawPattern(vBackground.grid[1],
            dim.x, dim.y,
            dim.width, vBackground.grid[1]->getHeight());
    }
    if (vBackground.grid[7])
    {
        graphics->drawPattern(vBackground.grid[7],
            dim.x, dim.height - vBackground.grid[7]->getHeight() + dim.y,
            dim.width, vBackground.grid[7]->getHeight());
    }
}

void ScrollArea::calcVBar(Graphics *const graphics)
{
    const Rect &dim = getVerticalBarDimension();

    if (vBackground.grid[4])
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }
    if (vBackground.grid[1])
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[1],
            dim.x, dim.y,
            dim.width, vBackground.grid[1]->getHeight());
    }
    if (vBackground.grid[7])
    {
        graphics->calcPattern(mVertexes,
            vBackground.grid[7],
            dim.x, dim.height - vBackground.grid[7]->getHeight() + dim.y,
            dim.width, vBackground.grid[7]->getHeight());
    }
}

void ScrollArea::drawHBar(Graphics *const graphics)
{
    const Rect &dim = getHorizontalBarDimension();

    if (hBackground.grid[4])
    {
        graphics->drawPattern(hBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }

    if (hBackground.grid[3])
    {
        graphics->drawPattern(hBackground.grid[3],
            dim.x, dim.y,
            hBackground.grid[3]->getWidth(), dim.height);
    }

    if (hBackground.grid[5])
    {
        graphics->drawPattern(hBackground.grid[5],
            dim.x + dim.width - hBackground.grid[5]->getWidth(),
            dim.y,
            hBackground.grid[5]->getWidth(),
            dim.height);
    }
}

void ScrollArea::calcHBar(Graphics *const graphics)
{
    const Rect &dim = getHorizontalBarDimension();

    if (hBackground.grid[4])
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[4],
            dim.x, dim.y,
            dim.width, dim.height);
    }

    if (hBackground.grid[3])
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[3],
            dim.x, dim.y,
            hBackground.grid[3]->getWidth(), dim.height);
    }

    if (hBackground.grid[5])
    {
        graphics->calcPattern(mVertexes,
            hBackground.grid[5],
            dim.x + dim.width - hBackground.grid[5]->getWidth(),
            dim.y,
            hBackground.grid[5]->getWidth(),
            dim.height);
    }
}

void ScrollArea::drawVMarker(Graphics *const graphics)
{
    const Rect &dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (mDimension.width - mScrollbarWidth)))
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::calcVMarker(Graphics *const graphics)
{
    const Rect &dim = getVerticalMarkerDimension();

    if ((mHasMouse) && (mX > (mDimension.width - mScrollbarWidth)))
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::drawHMarker(Graphics *const graphics)
{
    const Rect dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (mDimension.height - mScrollbarWidth)))
    {
        graphics->drawImageRect(dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->drawImageRect(
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::calcHMarker(Graphics *const graphics)
{
    const Rect dim = getHorizontalMarkerDimension();

    if ((mHasMouse) && (mY > (mDimension.height - mScrollbarWidth)))
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarkerHi);
    }
    else
    {
        graphics->calcWindow(mVertexes,
            dim.x, dim.y,
            dim.width, dim.height,
            vMarker);
    }
}

void ScrollArea::mouseMoved(MouseEvent& event)
{
    mX = event.getX();
    mY = event.getY();
}

void ScrollArea::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void ScrollArea::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void ScrollArea::widgetResized(const Event &event A_UNUSED)
{
    mRedraw = true;
    const unsigned int frameSize = 2 * mFrameSize;
    Widget *const content = getContent();
    if (content)
    {
        content->setSize(mDimension.width - frameSize,
            mDimension.height - frameSize);
    }
}

void ScrollArea::widgetMoved(const Event& event A_UNUSED)
{
    mRedraw = true;
}

void ScrollArea::mousePressed(MouseEvent& event)
{
    const int x = event.getX();
    const int y = event.getY();

    if (getUpButtonDimension().isPointInRect(x, y))
    {
        setVerticalScrollAmount(mVScroll
                                - mUpButtonScrollAmount);
        mUpButtonPressed = true;
    }
    else if (getDownButtonDimension().isPointInRect(x, y))
    {
        setVerticalScrollAmount(mVScroll
                                + mDownButtonScrollAmount);
        mDownButtonPressed = true;
    }
    else if (getLeftButtonDimension().isPointInRect(x, y))
    {
        setHorizontalScrollAmount(mHScroll
                                  - mLeftButtonScrollAmount);
        mLeftButtonPressed = true;
    }
    else if (getRightButtonDimension().isPointInRect(x, y))
    {
        setHorizontalScrollAmount(mHScroll
                                  + mRightButtonScrollAmount);
        mRightButtonPressed = true;
    }
    else if (getVerticalMarkerDimension().isPointInRect(x, y))
    {
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged = true;

        mVerticalMarkerDragOffset = y - getVerticalMarkerDimension().y;
    }
    else if (getVerticalBarDimension().isPointInRect(x, y))
    {
        if (y < getVerticalMarkerDimension().y)
        {
            setVerticalScrollAmount(mVScroll
                - static_cast<int>(getChildrenArea().height * 0.95));
        }
        else
        {
            setVerticalScrollAmount(mVScroll
                + static_cast<int>(getChildrenArea().height * 0.95));
        }
    }
    else if (getHorizontalMarkerDimension().isPointInRect(x, y))
    {
        mIsHorizontalMarkerDragged = true;
        mIsVerticalMarkerDragged = false;

        mHorizontalMarkerDragOffset = x - getHorizontalMarkerDimension().x;
    }
    else if (getHorizontalBarDimension().isPointInRect(x, y))
    {
        if (x < getHorizontalMarkerDimension().x)
        {
            setHorizontalScrollAmount(mHScroll
                - static_cast<int>(getChildrenArea().width * 0.95));
        }
        else
        {
            setHorizontalScrollAmount(mHScroll
                + static_cast<int>(getChildrenArea().width * 0.95));
        }
    }

    if (event.getButton() == MouseEvent::LEFT)
    {
        mClickX = event.getX();
        mClickY = event.getY();
    }
}

void ScrollArea::mouseReleased(MouseEvent& event)
{
    if (event.getButton() == MouseEvent::LEFT && mClickX && mClickY)
    {
        if (!event.isConsumed())
        {
#ifdef ANDROID
            int dx = mClickX - event.getX();
            int dy = mClickY - event.getY();
#else
            int dx = event.getX() - mClickX;
            int dy = event.getY() - mClickY;
#endif
            if ((dx < 20 && dx > 0) || (dx > -20 && dx < 0))
                dx = 0;

            if ((dy < 20 && dy > 0) || (dy > -20 && dy < 0))
                dy = 0;

            if (abs(dx) > abs(dy))
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
            else if (dy)
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
            if (dx || dy)
                event.consume();
        }
    }
    gcn::ScrollArea::mouseReleased(event);
    mRedraw = true;
}

void ScrollArea::mouseDragged(MouseEvent &event)
{
    if (mIsVerticalMarkerDragged)
    {
        const Rect barDim = getVerticalBarDimension();

        const int pos = event.getY() - barDim.y
              - mVerticalMarkerDragOffset;
        const int length = getVerticalMarkerDimension().height;

        if ((barDim.height - length) > 0)
        {
            setVerticalScrollAmount((getVerticalMaxScroll() * pos)
                                      / (barDim.height - length));
        }
        else
        {
            setVerticalScrollAmount(0);
        }
    }

    if (mIsHorizontalMarkerDragged)
    {
        const Rect barDim = getHorizontalBarDimension();

        const int pos = event.getX() - barDim.x
            - mHorizontalMarkerDragOffset;
        const int length = getHorizontalMarkerDimension().width;

        if ((barDim.width - length) > 0)
        {
            setHorizontalScrollAmount((getHorizontalMaxScroll() * pos)
                                      / (barDim.width - length));
        }
        else
        {
            setHorizontalScrollAmount(0);
        }
    }

    event.consume();
    mRedraw = true;
}

Rect ScrollArea::getVerticalBarDimension() const
{
    if (!mVBarVisible)
        return Rect(0, 0, 0, 0);

    const int height = (mVBarVisible && mShowButtons) ? mScrollbarWidth : 0;
    if (mHBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth,
            height,
            mScrollbarWidth,
            mDimension.height - 2 * height - mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        height,
        mScrollbarWidth,
        mDimension.height - 2 * height);
}

Rect ScrollArea::getHorizontalBarDimension() const
{
    if (!mHBarVisible)
        return Rect(0, 0, 0, 0);

    const int width = mShowButtons ? mScrollbarWidth : 0;
    if (mVBarVisible)
    {
        return Rect(width,
            mDimension.height - mScrollbarWidth,
            mDimension.width - 2 * width - mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(width,
                      mDimension.height - mScrollbarWidth,
                      mDimension.width - 2 * width,
                      mScrollbarWidth);
}

Rect ScrollArea::getVerticalMarkerDimension()
{
    if (!mVBarVisible)
        return Rect(0, 0, 0, 0);

    int length, pos;
    int height;
    const int h2 = mShowButtons
        ? mScrollbarWidth : mMarkerSize / 2;
    const Widget *content;
    if (!mWidgets.empty())
        content = *mWidgets.begin();
    else
        content = nullptr;

    if (mHBarVisible)
        height = mDimension.height - 2 * h2 - mScrollbarWidth;
    else
        height = mDimension.height - 2 * h2;

    const int maxV = getVerticalMaxScroll();
    if (mMarkerSize && maxV)
    {
        pos = (mVScroll * height / maxV - mMarkerSize / 2);
        length = mMarkerSize;
    }
    else
    {
        if (content)
        {
            const int h3 = content->getHeight();
            if (h3)
                length = (height * getChildrenArea().height) / h3;
            else
                length = height;
        }
        else
        {
            length = height;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > height)
            length = height;

        const int maxScroll = getVerticalMaxScroll();
        if (maxScroll != 0)
            pos = ((height - length) * mVScroll) / maxScroll;
        else
            pos = 0;
    }

    return Rect(mDimension.width - mScrollbarWidth, h2 + pos,
        mScrollbarWidth, length);
}

Rect ScrollArea::getHorizontalMarkerDimension()
{
    if (!mHBarVisible)
        return Rect(0, 0, 0, 0);

    int length, pos;
    int width;
    const int w2 = mShowButtons
        ? mScrollbarWidth : mMarkerSize / 2;
    const Widget *content;
    if (!mWidgets.empty())
        content = *mWidgets.begin();
    else
        content = nullptr;

    if (mVBarVisible)
        width = mDimension.width - 2 * w2 - mScrollbarWidth;
    else
        width = mDimension.width - w2 - mScrollbarWidth;

    const int maxH = getHorizontalMaxScroll();
    if (mMarkerSize && maxH)
    {
        pos = (mHScroll * width / maxH - mMarkerSize / 2);
        length = mMarkerSize;
    }
    else
    {
        if (content)
        {
            const int w3 = content->getWidth();
            if (w3)
                length = (width * getChildrenArea().width) / w3;
            else
                length = width;
        }
        else
        {
            length = width;
        }

        if (length < mScrollbarWidth)
            length = mScrollbarWidth;

        if (length > width)
            length = width;

        if (getHorizontalMaxScroll() != 0)
        {
            pos = ((width - length) * mHScroll)
                / getHorizontalMaxScroll();
        }
        else
        {
            pos = 0;
        }
    }

    return Rect(w2 + pos, mDimension.height - mScrollbarWidth,
        length, mScrollbarWidth);
}

Rect ScrollArea::getUpButtonDimension() const
{
    if (!mVBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    return Rect(mDimension.width - mScrollbarWidth, 0,
        mScrollbarWidth, mScrollbarWidth);
}

Rect ScrollArea::getDownButtonDimension() const
{
    if (!mVBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    if (mVBarVisible && mHBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth,
            mDimension.height - mScrollbarWidth*2,
            mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        mDimension.height - mScrollbarWidth,
        mScrollbarWidth,
        mScrollbarWidth);
}

Rect ScrollArea::getLeftButtonDimension() const
{
    if (!mHBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    return Rect(0, mDimension.height - mScrollbarWidth,
        mScrollbarWidth, mScrollbarWidth);
}

Rect ScrollArea::getRightButtonDimension() const
{
    if (!mHBarVisible || !mShowButtons)
        return Rect(0, 0, 0, 0);

    if (mVBarVisible && mHBarVisible)
    {
        return Rect(mDimension.width - mScrollbarWidth*2,
            mDimension.height - mScrollbarWidth,
            mScrollbarWidth,
            mScrollbarWidth);
    }

    return Rect(mDimension.width - mScrollbarWidth,
        mDimension.height - mScrollbarWidth,
        mScrollbarWidth,
        mScrollbarWidth);
}
