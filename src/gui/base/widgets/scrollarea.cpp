/*
 *  The ManaPlus Client
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

/*
 * For comments regarding functions please see the header file.
 */

#include "gui/base/widgets/scrollarea.hpp"

#include "debug.h"

namespace gcn
{
    ScrollArea::ScrollArea(const Widget2 *const widget) :
        gcn::BasicContainer(widget),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(SHOW_AUTO),
        mVPolicy(SHOW_AUTO),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(const Widget2 *const widget,
                           Widget *const content) :
        gcn::BasicContainer(widget),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(SHOW_AUTO),
        mVPolicy(SHOW_AUTO),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::ScrollArea(const Widget2 *const widget,
                           Widget *content,
                           ScrollPolicy hPolicy,
                           ScrollPolicy vPolicy) :
        gcn::BasicContainer(widget),
        MouseListener(),
        mVScroll(0),
        mHScroll(0),
        mScrollbarWidth(12),
        mHPolicy(hPolicy),
        mVPolicy(vPolicy),
        mVBarVisible(false),
        mHBarVisible(false),
        mUpButtonPressed(false),
        mDownButtonPressed(false),
        mLeftButtonPressed(false),
        mRightButtonPressed(false),
        mUpButtonScrollAmount(10),
        mDownButtonScrollAmount(10),
        mLeftButtonScrollAmount(10),
        mRightButtonScrollAmount(10),
        mIsVerticalMarkerDragged(false),
        mIsHorizontalMarkerDragged(false),
        mHorizontalMarkerDragOffset(0),
        mVerticalMarkerDragOffset(0),
        mOpaque(true)
    {
        setContent(content);
        addMouseListener(this);
    }

    ScrollArea::~ScrollArea()
    {
        setContent(nullptr);
    }

    void ScrollArea::setContent(Widget* widget)
    {
        if (widget)
        {
            clear();
            add(widget);
            widget->setPosition(0, 0);
        }
        else
        {
            clear();
        }

        checkPolicies();
    }

    Widget* ScrollArea::getContent()
    {
        if (!mWidgets.empty())
            return *mWidgets.begin();

        return nullptr;
    }

    void ScrollArea::setHorizontalScrollPolicy(ScrollPolicy hPolicy)
    {
        mHPolicy = hPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getHorizontalScrollPolicy() const
    {
        return mHPolicy;
    }

    void ScrollArea::setVerticalScrollPolicy(ScrollPolicy vPolicy)
    {
        mVPolicy = vPolicy;
        checkPolicies();
    }

    ScrollArea::ScrollPolicy ScrollArea::getVerticalScrollPolicy() const
    {
        return mVPolicy;
    }

    void ScrollArea::setScrollPolicy(ScrollPolicy hPolicy,
                                     ScrollPolicy vPolicy)
    {
        mHPolicy = hPolicy;
        mVPolicy = vPolicy;
        checkPolicies();
    }

    void ScrollArea::setVerticalScrollAmount(int vScroll)
    {
        const int max = getVerticalMaxScroll();

        mVScroll = vScroll;

        if (vScroll > max)
            mVScroll = max;

        if (vScroll < 0)
            mVScroll = 0;
    }

    int ScrollArea::getVerticalScrollAmount() const
    {
        return mVScroll;
    }

    void ScrollArea::setHorizontalScrollAmount(int hScroll)
    {
        const int max = getHorizontalMaxScroll();

        mHScroll = hScroll;

        if (hScroll > max)
            mHScroll = max;
        else if (hScroll < 0)
            mHScroll = 0;
    }

    int ScrollArea::getHorizontalScrollAmount() const
    {
        return mHScroll;
    }

    void ScrollArea::setScrollAmount(int hScroll, int vScroll)
    {
        setHorizontalScrollAmount(hScroll);
        setVerticalScrollAmount(vScroll);
    }

    int ScrollArea::getHorizontalMaxScroll()
    {
        checkPolicies();

        const Widget *const content = getContent();
        if (!content)
            return 0;

        const int value = content->getWidth() - getChildrenArea().width +
            2 * content->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    int ScrollArea::getVerticalMaxScroll()
    {
        checkPolicies();

        const Widget *const content = getContent();
        if (!content)
            return 0;

        int value;

        value = content->getHeight() - getChildrenArea().height +
            2 * content->getFrameSize();

        if (value < 0)
            return 0;

        return value;
    }

    void ScrollArea::setScrollbarWidth(int width)
    {
        if (width > 0)
            mScrollbarWidth = width;
    }

    int ScrollArea::getScrollbarWidth() const
    {
        return mScrollbarWidth;
    }

    void ScrollArea::mouseReleased(MouseEvent& mouseEvent)
    {
        mUpButtonPressed = false;
        mDownButtonPressed = false;
        mLeftButtonPressed = false;
        mRightButtonPressed = false;
        mIsHorizontalMarkerDragged = false;
        mIsVerticalMarkerDragged = false;

        mouseEvent.consume();
    }

    void ScrollArea::draw(Graphics *graphics A_UNUSED)
    {
    }

    void ScrollArea::drawHBar(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawVBar(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawBackground(Graphics *graphics A_UNUSED)
    {
    }

    void ScrollArea::drawUpButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawDownButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawLeftButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawRightButton(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawVMarker(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::drawHMarker(Graphics* graphics A_UNUSED)
    {
    }

    void ScrollArea::logic()
    {
        BLOCK_START("ScrollArea::logic")
        checkPolicies();

        setVerticalScrollAmount(getVerticalScrollAmount());
        setHorizontalScrollAmount(getHorizontalScrollAmount());

        Widget *const content = getContent();
        if (content)
        {
            const int frameSize = content->getFrameSize();
            content->setPosition(-mHScroll + frameSize, -mVScroll + frameSize);
            content->logic();
        }
        BLOCK_END("ScrollArea::logic")
    }

    void ScrollArea::checkPolicies()
    {
        const int w = getWidth();
        const int h = getHeight();

        mHBarVisible = false;
        mVBarVisible = false;

        const Widget *const content = getContent();
        if (!content)
        {
            mHBarVisible = (mHPolicy == SHOW_ALWAYS);
            mVBarVisible = (mVPolicy == SHOW_ALWAYS);
            return;
        }

        if (mHPolicy == SHOW_AUTO &&
            mVPolicy == SHOW_AUTO)
        {
            if (content->getWidth() <= w
                && content->getHeight() <= h)
            {
                mHBarVisible = false;
                mVBarVisible = false;
            }

            if (content->getWidth() > w)
            {
                mHBarVisible = true;
            }

            if ((content->getHeight() > h)
                || (mHBarVisible && content->getHeight()
                > h - mScrollbarWidth))
            {
                mVBarVisible = true;
            }

            if (mVBarVisible && content->getWidth() > w - mScrollbarWidth)
                mHBarVisible = true;

            return;
        }

        switch (mHPolicy)
        {
            case SHOW_NEVER:
                mHBarVisible = false;
                break;

            case SHOW_ALWAYS:
                mHBarVisible = true;
                break;

            case SHOW_AUTO:
                if (mVPolicy == SHOW_NEVER)
                {
                    mHBarVisible = (content->getWidth() > w);
                }
                else  // (mVPolicy == SHOW_ALWAYS)
                {
                    mHBarVisible = (content->getWidth()
                        > w - mScrollbarWidth);
                }
                break;

            default:
                break;
        }

        switch (mVPolicy)
        {
            case SHOW_NEVER:
                mVBarVisible = false;
                break;

            case SHOW_ALWAYS:
                mVBarVisible = true;
                break;

            case SHOW_AUTO:
                if (mHPolicy == SHOW_NEVER)
                {
                    mVBarVisible = (content->getHeight() > h);
                }
                else  // (mHPolicy == SHOW_ALWAYS)
                {
                    mVBarVisible = (content->getHeight()
                        > h - mScrollbarWidth);
                }
                break;
            default:
                break;
        }
    }

    Rect ScrollArea::getChildrenArea()
    {
        const Rect area = Rect(0, 0,
            mVBarVisible ? (getWidth() - mScrollbarWidth) : getWidth(),
            mHBarVisible ? (getHeight() - mScrollbarWidth) : getHeight());

        if (area.width < 0 || area.height < 0)
            return Rect();

        return area;
    }

    void ScrollArea::showWidgetPart(Widget* widget, Rect area)
    {
        const Widget *const content = getContent();
        if (widget != content)
            return;

        BasicContainer::showWidgetPart(widget, area);

        setHorizontalScrollAmount(content->getFrameSize()
            - content->getX());
        setVerticalScrollAmount(content->getFrameSize()
            - content->getY());
    }

    Widget *ScrollArea::getWidgetAt(int x, int y)
    {
        if (getChildrenArea().isPointInRect(x, y))
            return getContent();

        return nullptr;
    }

    void ScrollArea::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount()
            - getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ScrollArea::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (mouseEvent.isConsumed())
            return;

        setVerticalScrollAmount(getVerticalScrollAmount()
            + getChildrenArea().height / 8);

        mouseEvent.consume();
    }

    void ScrollArea::setWidth(int width)
    {
        Widget::setWidth(width);
        checkPolicies();
    }

    void ScrollArea::setHeight(int height)
    {
        Widget::setHeight(height);
        checkPolicies();
    }

    void ScrollArea::setDimension(const Rect& dimension)
    {
        Widget::setDimension(dimension);
        checkPolicies();
    }

    void ScrollArea::setLeftButtonScrollAmount(int amount)
    {
        mLeftButtonScrollAmount = amount;
    }

    void ScrollArea::setRightButtonScrollAmount(int amount)
    {
        mRightButtonScrollAmount = amount;
    }

    void ScrollArea::setUpButtonScrollAmount(int amount)
    {
        mUpButtonScrollAmount = amount;
    }

    void ScrollArea::setDownButtonScrollAmount(int amount)
    {
        mDownButtonScrollAmount = amount;
    }

    int ScrollArea::getLeftButtonScrollAmount() const
    {
        return mLeftButtonScrollAmount;
    }

    int ScrollArea::getRightButtonScrollAmount() const
    {
        return mRightButtonScrollAmount;
    }

    int ScrollArea::getUpButtonScrollAmount() const
    {
        return mUpButtonScrollAmount;
    }

    int ScrollArea::getDownButtonScrollAmount() const
    {
        return mDownButtonScrollAmount;
    }

    void ScrollArea::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool ScrollArea::isOpaque() const
    {
        return mOpaque;
    }
}  // namespace gcn
