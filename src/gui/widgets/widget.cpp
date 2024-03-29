/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/widget.h"

#include "gui/focushandler.h"

#include "listeners/actionlistener.h"
#include "listeners/widgetdeathlistener.h"
#include "listeners/widgetlistener.h"

#include "utils/foreach.h"

#include "debug.h"

Font* Widget::mGlobalFont = nullptr;
std::list<Widget*> Widget::mAllWidgets;
std::set<Widget*> Widget::mAllWidgetsSet;

Widget::Widget(const Widget2 *const widget) :
    Widget2(widget),
    mVisible(Visible_true),
    mMouseListeners(),
    mKeyListeners(),
    mActionListeners(),
    mDeathListeners(),
    mFocusListeners(),
    mWidgetListeners(),
    mForegroundColor(0x000000),
    mBackgroundColor(0xffffff),
    mBaseColor(0x808090),
    mDimension(),
    mActionEventId(),
    mId(),
    mFocusHandler(nullptr),
    mInternalFocusHandler(nullptr),
    mParent(nullptr),
    mCurrentFont(nullptr),
    mFrameSize(0),
    mFocusable(false),
    mTabIn(true),
    mTabOut(true),
    mEnabled(true),
    mAllowLogic(true),
    mMouseConsume(true),
    mRedraw(true),
    mSelectable(true)
{
    mAllWidgets.push_back(this);
    mAllWidgetsSet.insert(this);
}

Widget::~Widget()
{
    FOR_EACH (WidgetDeathListenerIterator, iter, mDeathListeners)
    {
        Event event(this);
        (*iter)->death(event);
    }

    // +++ call to virtual member
    setFocusHandler(nullptr);

    mAllWidgets.remove(this);
    mAllWidgetsSet.erase(this);
}

void Widget::setWidth(const int width)
{
    Rect newDimension = mDimension;
    newDimension.width = width;
    setDimension(newDimension);
}

void Widget::setHeight(const int height)
{
    Rect newDimension = mDimension;
    newDimension.height = height;
    setDimension(newDimension);
}

void Widget::setX(const int x)
{
    Rect newDimension = mDimension;
    newDimension.x = x;
    setDimension(newDimension);
}

void Widget::setY(const int y)
{
    Rect newDimension = mDimension;
    newDimension.y = y;
    setDimension(newDimension);
}

void Widget::setPosition(const int x, const int y)
{
    Rect newDimension = mDimension;
    newDimension.x = x;
    newDimension.y = y;
    setDimension(newDimension);
}

void Widget::setDimension(const Rect& dimension)
{
    const Rect oldDimension = mDimension;
    mDimension = dimension;

    if (mDimension.width != oldDimension.width
        || mDimension.height != oldDimension.height)
    {
        distributeResizedEvent();
    }

    if (mDimension.x != oldDimension.x || mDimension.y != oldDimension.y)
        distributeMovedEvent();
}

bool Widget::isFocused() const
{
    if (mFocusHandler == nullptr)
        return false;

    return mFocusHandler->isFocused(this);
}

void Widget::setFocusable(const bool focusable)
{
    if (!focusable && isFocused() && (mFocusHandler != nullptr))
        mFocusHandler->focusNone();
    mFocusable = focusable;
}

bool Widget::isFocusable() const
{
    return mFocusable && isVisible() && isEnabled();
}

void Widget::requestFocus()
{
    if (mFocusHandler == nullptr)
        return;

    if (isFocusable())
        mFocusHandler->requestFocus(this);
}

void Widget::requestMoveToTop()
{
    if (mParent != nullptr)
        mParent->moveToTop(this);
}

void Widget::requestMoveToBottom()
{
    if (mParent != nullptr)
        mParent->moveToBottom(this);
}

void Widget::setVisible(Visible visible)
{
    if (visible == Visible_false && isFocused() && (mFocusHandler != nullptr))
        mFocusHandler->focusNone();

    if (visible == Visible_true)
        distributeShownEvent();
    else
        distributeHiddenEvent();

    mVisible = visible;
}

void Widget::setFocusHandler(FocusHandler *const focusHandler)
{
    if (mFocusHandler != nullptr)
    {
        releaseModalFocus();
        mFocusHandler->remove(this);
    }

    if (focusHandler != nullptr)
        focusHandler->add(this);

    mFocusHandler = focusHandler;
}

void Widget::addActionListener(ActionListener *const actionListener)
{
    mActionListeners.push_back(actionListener);
}

void Widget::removeActionListener(ActionListener *const actionListener)
{
    mActionListeners.remove(actionListener);
}

void Widget::addDeathListener(WidgetDeathListener *const deathListener)
{
    mDeathListeners.push_back(deathListener);
}

void Widget::removeDeathListener(WidgetDeathListener *const deathListener)
{
    mDeathListeners.remove(deathListener);
}

void Widget::addKeyListener(KeyListener *const keyListener)
{
    mKeyListeners.push_back(keyListener);
}

void Widget::removeKeyListener(KeyListener *const keyListener)
{
    mKeyListeners.remove(keyListener);
}

void Widget::addFocusListener(FocusListener *const focusListener)
{
    mFocusListeners.push_back(focusListener);
}

void Widget::removeFocusListener(FocusListener *const focusListener)
{
    mFocusListeners.remove(focusListener);
}

void Widget::addMouseListener(MouseListener *const mouseListener)
{
    mMouseListeners.push_back(mouseListener);
}

void Widget::removeMouseListener(MouseListener *const mouseListener)
{
    mMouseListeners.remove(mouseListener);
}

void Widget::addWidgetListener(WidgetListener *const widgetListener)
{
    mWidgetListeners.push_back(widgetListener);
}

void Widget::removeWidgetListener(WidgetListener *const widgetListener)
{
    mWidgetListeners.remove(widgetListener);
}

void Widget::getAbsolutePosition(int& x, int& y) const
{
    if (mParent == nullptr)
    {
        x = mDimension.x;
        y = mDimension.y;
        return;
    }

    int parentX;
    int parentY;

    mParent->getAbsolutePosition(parentX, parentY);

    const Rect &rect = mParent->getChildrenArea();
    x = parentX + mDimension.x + rect.x;
    y = parentY + mDimension.y + rect.y;
}

Font* Widget::getFont() const
{
    if (mCurrentFont == nullptr)
        return mGlobalFont;
    return mCurrentFont;
}

void Widget::setGlobalFont(Font *const font)
{
    mGlobalFont = font;

    FOR_EACH (std::list<Widget*>::const_iterator, iter, mAllWidgets)
    {
        if ((*iter)->mCurrentFont == nullptr)
            (*iter)->fontChanged();
    }
}

void Widget::setFont(Font *const font)
{
    mCurrentFont = font;
    fontChanged();
}

void Widget::distributeWindowResizeEvent()
{
    FOR_EACH (std::list<Widget*>::const_iterator, iter, mAllWidgets)
        (*iter)->windowResized();
}

bool Widget::widgetExists(const Widget *const widget)
{
    return mAllWidgetsSet.find(const_cast<Widget*>(widget))
        != mAllWidgetsSet.end();
}

void Widget::setSize(const int width, const int height)
{
    Rect newDimension = mDimension;
    newDimension.width = width;
    newDimension.height = height;
    setDimension(newDimension);
}

bool Widget::isEnabled() const
{
    return mEnabled && isVisible();
}

void Widget::requestModalFocus()
{
    if (mFocusHandler == nullptr)
        return;
    mFocusHandler->requestModalFocus(this);
}

void Widget::requestModalMouseInputFocus()
{
    if (mFocusHandler == nullptr)
        return;
    mFocusHandler->requestModalMouseInputFocus(this);
}

void Widget::releaseModalFocus()
{
    if (mFocusHandler == nullptr)
        return;
    mFocusHandler->releaseModalFocus(this);
}

void Widget::releaseModalMouseInputFocus()
{
    if (mFocusHandler == nullptr)
        return;
    mFocusHandler->releaseModalMouseInputFocus(this);
}

bool Widget::isModalFocused() const
{
    if (mFocusHandler == nullptr)
        return false;

    if (mParent != nullptr)
    {
        return (mFocusHandler->getModalFocused() == this)
            || mParent->isModalFocused();
    }

    return mFocusHandler->getModalFocused() == this;
}

bool Widget::isModalMouseInputFocused() const
{
    if (mFocusHandler == nullptr)
        return false;

    if (mParent != nullptr)
    {
        return (mFocusHandler->getModalMouseInputFocused() == this)
            || mParent->isModalMouseInputFocused();
    }

    return mFocusHandler->getModalMouseInputFocused() == this;
}

const std::list<MouseListener*> &Widget::getMouseListeners() const
{
    return mMouseListeners;
}

const std::list<KeyListener*> &Widget::getKeyListeners() const
{
    return mKeyListeners;
}

const std::list<FocusListener*> &Widget::getFocusListeners() const
{
    return mFocusListeners;
}

Rect Widget::getChildrenArea()
{
    return Rect(0, 0, 0, 0);
}

FocusHandler* Widget::getInternalFocusHandler()
{
    return mInternalFocusHandler;
}

void Widget::setInternalFocusHandler(FocusHandler *const focusHandler)
{
    mInternalFocusHandler = focusHandler;
}

void Widget::distributeResizedEvent()
{
    FOR_EACH (WidgetListenerIterator, iter, mWidgetListeners)
    {
        Event event(this);
        (*iter)->widgetResized(event);
    }
}

void Widget::distributeMovedEvent()
{
    FOR_EACH (WidgetListenerIterator, iter, mWidgetListeners)
    {
        Event event(this);
        (*iter)->widgetMoved(event);
    }
}

void Widget::distributeHiddenEvent()
{
    FOR_EACH (WidgetListenerIterator, iter, mWidgetListeners)
    {
        Event event(this);
        (*iter)->widgetHidden(event);
    }
}

void Widget::distributeActionEvent()
{
    FOR_EACH (ActionListenerIterator, iter, mActionListeners)
    {
        ActionEvent actionEvent(this, mActionEventId);
        (*iter)->action(actionEvent);
    }
}

void Widget::distributeShownEvent()
{
    FOR_EACH (WidgetListenerIterator, iter, mWidgetListeners)
    {
        Event event(this);
        (*iter)->widgetShown(event);
    }
}

void Widget::showPart(const Rect &rectangle)
{
    if (mParent != nullptr)
        mParent->showWidgetPart(this, rectangle);
}

void Widget::windowResized()
{
    mRedraw = true;
}

Widget *Widget::callPostInit(Widget *const widget)
{
    if (widget != nullptr)
        widget->postInit();
    return widget;
}
