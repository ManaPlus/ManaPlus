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

#include "gui/focushandler.h"

#include "gui/gui.h"

#include "gui/widgets/window.h"

#include "listeners/focuslistener.h"

#include "debug.h"

FocusHandler::FocusHandler() :
    mWidgets(),
    mFocusedWidget(nullptr),
    mModalFocusedWidget(nullptr),
    mModalMouseInputFocusedWidget(nullptr),
    mDraggedWidget(nullptr),
    mLastWidgetWithMouse(nullptr),
    mLastWidgetWithModalFocus(nullptr),
    mLastWidgetWithModalMouseInputFocus(nullptr),
    mLastWidgetPressed(nullptr),
    mModalStack()
{
}

void FocusHandler::requestModalFocus(Widget *const widget)
{
    /* If there is another widget with modal focus, remove its modal focus
     * and put it on the modal widget stack.
     */
    if (mModalFocusedWidget && mModalFocusedWidget != widget)
    {
        mModalStack.push_front(mModalFocusedWidget);
        mModalFocusedWidget = nullptr;
    }

    mModalFocusedWidget = widget;
    if (mFocusedWidget && !mFocusedWidget->isModalFocused())
        focusNone();
}

void FocusHandler::releaseModalFocus(Widget *const widget)
{
    mModalStack.remove(widget);

    if (mModalFocusedWidget == widget)
    {
        if (mModalFocusedWidget == widget)
            mModalFocusedWidget = nullptr;

        /* Check if there were any previously modal widgets that'd still like
         * to regain their modal focus.
         */
        if (!mModalStack.empty())
        {
            requestModalFocus(mModalStack.front());
            mModalStack.pop_front();
        }
    }
}

void FocusHandler::remove(Widget *const widget)
{
    releaseModalFocus(widget);

    if (isFocused(widget))
        mFocusedWidget = nullptr;

    FOR_EACH (WidgetIterator, iter, mWidgets)
    {
        if ((*iter) == widget)
        {
            mWidgets.erase(iter);
            break;
        }
    }

    if (mDraggedWidget == widget)
    {
        mDraggedWidget = nullptr;
        return;
    }

    if (mLastWidgetWithMouse == widget)
    {
        mLastWidgetWithMouse = nullptr;
        return;
    }

    if (mLastWidgetWithModalFocus == widget)
    {
        mLastWidgetWithModalFocus = nullptr;
        return;
    }

    if (mLastWidgetWithModalMouseInputFocus == widget)
    {
        mLastWidgetWithModalMouseInputFocus = nullptr;
        return;
    }

    if (mLastWidgetPressed == widget)
    {
        mLastWidgetPressed = nullptr;
        return;
    }
}

void FocusHandler::tabNext()
{
    if (mFocusedWidget)
    {
        if (!mFocusedWidget->isTabOutEnabled())
            return;
    }

    if (mWidgets.empty())
    {
        mFocusedWidget = nullptr;
        return;
    }

    int i;
    int focusedWidget = -1;
    const int sz = static_cast<int>(mWidgets.size());
    for (i = 0; i < sz; ++ i)
    {
        if (mWidgets[i] == mFocusedWidget)
            focusedWidget = i;
    }
    const int focused = focusedWidget;
    bool done = false;

    // i is a counter that ensures that the following loop
    // won't get stuck in an infinite loop
    i = sz;
    do
    {
        ++ focusedWidget;

        if (i == 0)
        {
            focusedWidget = -1;
            break;
        }

        -- i;

        if (focusedWidget >= sz)
            focusedWidget = 0;

        if (focusedWidget == focused)
            return;

        const Widget *const widget = mWidgets.at(focusedWidget);
        if (widget->isFocusable() && widget->isTabInEnabled() &&
            (!mModalFocusedWidget || widget->isModalFocused()))
        {
            done = true;
        }
    }
    while (!done);

    if (focusedWidget >= 0)
    {
        mFocusedWidget = mWidgets.at(focusedWidget);
        Event focusEvent(mFocusedWidget);
        distributeFocusGainedEvent(focusEvent);
    }

    if (focused >= 0)
    {
        Event focusEvent(mWidgets.at(focused));
        distributeFocusLostEvent(focusEvent);
    }

    checkForWindow();
}

void FocusHandler::tabPrevious()
{
    if (mFocusedWidget)
    {
        if (!mFocusedWidget->isTabOutEnabled())
            return;
    }

    if (mWidgets.empty())
    {
        mFocusedWidget = nullptr;
        return;
    }

    int i;
    int focusedWidget = -1;
    const int sz = static_cast<int>(mWidgets.size());
    for (i = 0; i < sz; ++ i)
    {
        if (mWidgets[i] == mFocusedWidget)
            focusedWidget = i;
    }
    const int focused = focusedWidget;
    bool done = false;

    // i is a counter that ensures that the following loop
    // won't get stuck in an infinite loop
    i = sz;
    do
    {
        -- focusedWidget;

        if (i == 0)
        {
            focusedWidget = -1;
            break;
        }

        -- i;

        if (focusedWidget <= 0)
            focusedWidget = sz - 1;

        if (focusedWidget == focused)
            return;

        const Widget *const widget = mWidgets.at(focusedWidget);
        if (widget->isFocusable() && widget->isTabInEnabled() &&
            (!mModalFocusedWidget || widget->isModalFocused()))
        {
            done = true;
        }
    }
    while (!done);

    if (focusedWidget >= 0)
    {
        mFocusedWidget = mWidgets.at(focusedWidget);
        Event focusEvent(mFocusedWidget);
        distributeFocusGainedEvent(focusEvent);
    }

    if (focused >= 0)
    {
        Event focusEvent(mWidgets.at(focused));
        distributeFocusLostEvent(focusEvent);
    }

    checkForWindow();
}

void FocusHandler::checkForWindow() const
{
    if (mFocusedWidget)
    {
        Widget *widget = mFocusedWidget->getParent();

        while (widget)
        {
            Window *const window = dynamic_cast<Window*>(widget);

            if (window)
            {
                window->requestMoveToTop();
                break;
            }

            widget = widget->getParent();
        }
    }
}

void FocusHandler::distributeFocusGainedEvent(const Event &focusEvent)
{
    if (gui)
        gui->distributeGlobalFocusGainedEvent(focusEvent);

    Widget *const sourceWidget = focusEvent.getSource();

    std::list<FocusListener*> focusListeners
        = sourceWidget->getFocusListeners();

    // Send the event to all focus listeners of the widget.
    for (std::list<FocusListener*>::const_iterator
          it = focusListeners.begin();
          it != focusListeners.end();
          ++ it)
    {
        (*it)->focusGained(focusEvent);
    }
}

void FocusHandler::requestFocus(Widget *const widget)
{
    if (!widget || widget == mFocusedWidget)
        return;

    int toBeFocusedIndex = -1;
    for (unsigned int i = 0, sz = static_cast<unsigned int>(
         mWidgets.size()); i < sz; ++i)
    {
        if (mWidgets[i] == widget)
        {
            toBeFocusedIndex = i;
            break;
        }
    }

    if (toBeFocusedIndex < 0)
        return;

    Widget *const oldFocused = mFocusedWidget;

    if (oldFocused != widget)
    {
        mFocusedWidget = mWidgets.at(toBeFocusedIndex);

        if (oldFocused)
        {
            Event focusEvent(oldFocused);
            distributeFocusLostEvent(focusEvent);
        }

        Event focusEvent(mWidgets.at(toBeFocusedIndex));
        distributeFocusGainedEvent(focusEvent);
    }
}

void FocusHandler::requestModalMouseInputFocus(Widget *const widget)
{
    if (mModalMouseInputFocusedWidget
        && mModalMouseInputFocusedWidget != widget)
    {
        return;
    }

    mModalMouseInputFocusedWidget = widget;
}

void FocusHandler::releaseModalMouseInputFocus(Widget *const widget)
{
    if (mModalMouseInputFocusedWidget == widget)
        mModalMouseInputFocusedWidget = nullptr;
}

Widget* FocusHandler::getFocused() const
{
    return mFocusedWidget;
}

Widget* FocusHandler::getModalFocused() const
{
    return mModalFocusedWidget;
}

Widget* FocusHandler::getModalMouseInputFocused() const
{
    return mModalMouseInputFocusedWidget;
}

void FocusHandler::focusNext()
{
    int i;
    int focusedWidget = -1;
    const int sz = static_cast<int>(mWidgets.size());
    for (i = 0; i < sz; ++i)
    {
        if (mWidgets[i] == mFocusedWidget)
            focusedWidget = i;
    }
    const int focused = focusedWidget;

    // i is a counter that ensures that the following loop
    // won't get stuck in an infinite loop
    i = sz;
    do
    {
        ++ focusedWidget;

        if (i == 0)
        {
            focusedWidget = -1;
            break;
        }

        -- i;

        if (focusedWidget >= sz)
            focusedWidget = 0;

        if (focusedWidget == focused)
            return;
    }
    while (!mWidgets.at(focusedWidget)->isFocusable());

    if (focusedWidget >= 0)
    {
        mFocusedWidget = mWidgets.at(focusedWidget);

        Event focusEvent(mFocusedWidget);
        distributeFocusGainedEvent(focusEvent);
    }

    if (focused >= 0)
    {
        Event focusEvent(mWidgets.at(focused));
        distributeFocusLostEvent(focusEvent);
    }
}

void FocusHandler::focusPrevious()
{
    if (mWidgets.empty())
    {
        mFocusedWidget = nullptr;
        return;
    }

    int i;
    int focusedWidget = -1;
    const int sz = static_cast<int>(mWidgets.size());
    for (i = 0; i < sz; ++ i)
    {
        if (mWidgets[i] == mFocusedWidget)
            focusedWidget = i;
    }
    const int focused = focusedWidget;

    // i is a counter that ensures that the following loop
    // won't get stuck in an infinite loop
    i = sz;
    do
    {
        -- focusedWidget;

        if (i == 0)
        {
            focusedWidget = -1;
            break;
        }

        -- i;

        if (focusedWidget <= 0)
            focusedWidget = sz - 1;

        if (focusedWidget == focused)
            return;
    }
    while (!mWidgets.at(focusedWidget)->isFocusable());

    if (focusedWidget >= 0)
    {
        mFocusedWidget = mWidgets.at(focusedWidget);
        Event focusEvent(mFocusedWidget);
        distributeFocusGainedEvent(focusEvent);
    }

    if (focused >= 0)
    {
        Event focusEvent(mWidgets.at(focused));
        distributeFocusLostEvent(focusEvent);
    }
}

bool FocusHandler::isFocused(const Widget *const widget) const
{
    return mFocusedWidget == widget;
}

void FocusHandler::add(Widget *const widget)
{
    mWidgets.push_back(widget);
}

void FocusHandler::focusNone()
{
    if (mFocusedWidget)
    {
        Widget *const focused = mFocusedWidget;
        mFocusedWidget = nullptr;

        Event focusEvent(focused);
        distributeFocusLostEvent(focusEvent);
    }
}

void FocusHandler::distributeFocusLostEvent(const Event& focusEvent)
{
    Widget *const sourceWidget = focusEvent.getSource();

    std::list<FocusListener*> focusListeners
        = sourceWidget->getFocusListeners();

    // Send the event to all focus listeners of the widget.
    for (std::list<FocusListener*>::const_iterator
          it = focusListeners.begin();
          it != focusListeners.end();
          ++ it)
    {
        (*it)->focusLost(focusEvent);
    }
}

Widget* FocusHandler::getDraggedWidget() const
{
    return mDraggedWidget;
}

void FocusHandler::setDraggedWidget(Widget *const draggedWidget)
{
    mDraggedWidget = draggedWidget;
}

Widget* FocusHandler::getLastWidgetWithMouse() const
{
    return mLastWidgetWithMouse;
}

void FocusHandler::setLastWidgetWithMouse(Widget *const lastWidgetWithMouse)
{
    mLastWidgetWithMouse = lastWidgetWithMouse;
}

Widget* FocusHandler::getLastWidgetWithModalFocus() const
{
    return mLastWidgetWithModalFocus;
}

void FocusHandler::setLastWidgetWithModalFocus(Widget *const widget)
{
    mLastWidgetWithModalFocus = widget;
}

Widget* FocusHandler::getLastWidgetWithModalMouseInputFocus() const
{
    return mLastWidgetWithModalMouseInputFocus;
}

void FocusHandler::setLastWidgetWithModalMouseInputFocus(Widget *const widget)
{
    mLastWidgetWithModalMouseInputFocus = widget;
}

Widget* FocusHandler::getLastWidgetPressed() const
{
    return mLastWidgetPressed;
}

void FocusHandler::setLastWidgetPressed(Widget *const lastWidgetPressed)
{
    mLastWidgetPressed = lastWidgetPressed;
}
