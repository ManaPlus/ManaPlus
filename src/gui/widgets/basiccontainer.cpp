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

#include "gui/widgets/basiccontainer.h"

#include <algorithm>

#include "debug.h"

BasicContainer::~BasicContainer()
{
    clear();
}

void BasicContainer::moveToTop(Widget* widget)
{
    FOR_EACH (WidgetListIterator, iter, mWidgets)
    {
        if (*iter == widget)
        {
            mWidgets.erase(iter);
            mWidgets.push_back(widget);
            break;
        }
    }
    FOR_EACH (WidgetListIterator, iter, mLogicWidgets)
    {
        if (*iter == widget)
        {
            mLogicWidgets.erase(iter);
            mLogicWidgets.push_back(widget);
            return;
        }
    }
}

void BasicContainer::moveToBottom(Widget* widget)
{
    WidgetListIterator iter = std::find(mWidgets.begin(),
        mWidgets.end(), widget);
    if (iter != mWidgets.end())
    {
        mWidgets.erase(iter);
        mWidgets.insert(mWidgets.begin(), widget);
    }

    WidgetListIterator iter2 = std::find(mLogicWidgets.begin(),
        mLogicWidgets.end(), widget);
    if (iter2 != mLogicWidgets.end())
    {
        mLogicWidgets.erase(iter2);
        mLogicWidgets.insert(mLogicWidgets.begin(), widget);
    }
}

void BasicContainer::death(const Event &event)
{
    WidgetListIterator iter = std::find(mWidgets.begin(),
        mWidgets.end(), event.getSource());
    if (iter != mWidgets.end())
        mWidgets.erase(iter);

    WidgetListIterator iter2 = std::find(mLogicWidgets.begin(),
        mLogicWidgets.end(), event.getSource());
    if (iter2 != mLogicWidgets.end())
        mLogicWidgets.erase(iter2);
}

Rect BasicContainer::getChildrenArea()
{
    return Rect(0, 0, mDimension.width, mDimension.height);
}

void BasicContainer::focusNext()
{
    WidgetListConstIterator it;

    for (it = mWidgets.begin(); it != mWidgets.end(); ++ it)
    {
        if ((*it)->isFocused())
            break;
    }

    WidgetListConstIterator end = it;

    if (it == mWidgets.end())
        it = mWidgets.begin();

    ++ it;

    for ( ; it != end; ++ it)
    {
        if (it == mWidgets.end())
            it = mWidgets.begin();

        if ((*it)->isFocusable())
        {
            (*it)->requestFocus();
            return;
        }
    }
}

void BasicContainer::focusPrevious()
{
    WidgetListReverseIterator it;

    for (it = mWidgets.rbegin(); it != mWidgets.rend(); ++ it)
    {
        if ((*it)->isFocused())
            break;
    }

    const WidgetListReverseIterator end = it;

    ++ it;

    if (it == mWidgets.rend())
        it = mWidgets.rbegin();

    for ( ; it != end; ++ it)
    {
        if (it == mWidgets.rend())
            it = mWidgets.rbegin();

        if ((*it)->isFocusable())
        {
            (*it)->requestFocus();
            return;
        }
    }
}

Widget *BasicContainer::getWidgetAt(int x, int y)
{
    const Rect r = getChildrenArea();

    if (!r.isPointInRect(x, y))
        return nullptr;

    x -= r.x;
    y -= r.y;

    for (WidgetListReverseIterator it = mWidgets.rbegin();
         it != mWidgets.rend(); ++ it)
    {
        const Widget *const widget = *it;
        if (widget->isVisible() && widget->getDimension()
            .isPointInRect(x, y))
        {
            return *it;
        }
    }

    return nullptr;
}

void BasicContainer::logic()
{
    BLOCK_START("BasicContainer::logic")
    if (!mVisible)
    {
        BLOCK_END("BasicContainer::logic")
        return;
    }
    logicChildren();
    BLOCK_END("BasicContainer::logic")
}

void BasicContainer::setFocusHandler(FocusHandler *const focusHandler)
{
    Widget::setFocusHandler(focusHandler);

    if (mInternalFocusHandler)
        return;

    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
        (*iter)->setFocusHandler(focusHandler);
}

void BasicContainer::add(Widget *const widget)
{
    mWidgets.push_back(widget);
    if (widget->isAllowLogic())
        mLogicWidgets.push_back(widget);

    if (!mInternalFocusHandler)
        widget->setFocusHandler(getFocusHandler());
    else
        widget->setFocusHandler(mInternalFocusHandler);

    widget->setParent(this);
    widget->addDeathListener(this);
}

void BasicContainer::remove(Widget* widget)
{
    FOR_EACH (WidgetListIterator, iter, mWidgets)
    {
        if (*iter == widget)
        {
            mWidgets.erase(iter);
            widget->setFocusHandler(nullptr);
            widget->setParent(nullptr);
            widget->removeDeathListener(this);
            break;
        }
    }
    FOR_EACH (WidgetListIterator, iter, mLogicWidgets)
    {
        if (*iter == widget)
        {
            mLogicWidgets.erase(iter);
            return;
        }
    }
}

void BasicContainer::clear()
{
    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
    {
        Widget *const widget = *iter;
        widget->setFocusHandler(nullptr);
        widget->setParent(nullptr);
        widget->removeDeathListener(this);
    }

    mWidgets.clear();
    mLogicWidgets.clear();
}

void BasicContainer::drawChildren(Graphics* graphics)
{
    BLOCK_START("BasicContainer::drawChildren")
    graphics->pushClipArea(getChildrenArea());

    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
    {
        Widget *const widget = *iter;
        if (widget->isVisible())
        {
            // If the widget has a frame,
            // draw it before drawing the widget
            if (widget->mFrameSize > 0)
            {
                Rect rec = widget->mDimension;
                const int frame = widget->mFrameSize;
                const int frame2 = frame * 2;
                rec.x -= frame;
                rec.y -= frame;
                rec.width += frame2;
                rec.height += frame2;
                graphics->pushClipArea(rec);
                BLOCK_START("BasicContainer::drawChildren 1")
                widget->drawFrame(graphics);
                BLOCK_END("BasicContainer::drawChildren 1")
                graphics->popClipArea();
            }

            graphics->pushClipArea(widget->mDimension);
            BLOCK_START("BasicContainer::drawChildren 2")
            widget->draw(graphics);
            BLOCK_END("BasicContainer::drawChildren 2")
            graphics->popClipArea();
        }
    }

    graphics->popClipArea();
    BLOCK_END("BasicContainer::drawChildren")
}

void BasicContainer::logicChildren()
{
    BLOCK_START("BasicContainer::logicChildren")
    FOR_EACH (WidgetListConstIterator, iter, mLogicWidgets)
        (*iter)->logic();
    BLOCK_END("BasicContainer::logicChildren")
}

void BasicContainer::showWidgetPart(Widget *const widget, Rect area)
{
    const Rect widgetArea = getChildrenArea();

    const int x = widget->mDimension.x;
    const int y = widget->mDimension.y;
    area.x += x;
    area.y += y;

    if (area.x + area.width > widgetArea.width)
        widget->setX(x - area.x - area.width + widgetArea.width);

    if (area.y + area.height > widgetArea.height)
        widget->setY(y - area.y - area.height + widgetArea.height);

    if (area.x < 0)
        widget->setX(x - area.x);

    if (area.y < 0)
        widget->setY(y - area.y);
}

void BasicContainer::setInternalFocusHandler(FocusHandler* focusHandler)
{
    Widget::setInternalFocusHandler(focusHandler);

    FOR_EACH (WidgetListConstIterator, iter, mWidgets)
    {
        if (!mInternalFocusHandler)
            (*iter)->setFocusHandler(getFocusHandler());
        else
            (*iter)->setFocusHandler(mInternalFocusHandler);
    }
}
