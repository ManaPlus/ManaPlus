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

#include "gui/base/gui.hpp"

#include "gui/widgets/widget.h"

#include "gui/focushandler.h"

#include "input/mouseinput.h"

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"

#include "debug.h"

namespace gcn
{
    Gui::Gui() :
        mTop(nullptr),
        mGraphics(nullptr),
        mInput(nullptr),
        mFocusHandler(new FocusHandler),
        mTabbing(true),
        mKeyListeners(),
        mShiftPressed(false),
        mMetaPressed(false),
        mControlPressed(false),
        mAltPressed(false),
        mLastMousePressButton(0),
        mLastMousePressTimeStamp(0),
        mLastMouseX(0),
        mLastMouseY(0),
        mClickCount(1),
        mLastMouseDragButton(0),
        mWidgetWithMouseQueue()
    {
    }

    Gui::~Gui()
    {
        if (Widget::widgetExists(mTop))
            setTop(nullptr);

        delete mFocusHandler;
        mFocusHandler = nullptr;
    }

    void Gui::setTop(Widget* top)
    {
        if (mTop)
            mTop->_setFocusHandler(nullptr);
        if (top)
            top->_setFocusHandler(mFocusHandler);

        mTop = top;
    }

    Widget* Gui::getTop() const
    {
        return mTop;
    }

    void Gui::setGraphics(Graphics* graphics)
    {
        mGraphics = graphics;
    }

    Graphics* Gui::getGraphics() const
    {
        return mGraphics;
    }

    void Gui::setInput(SDLInput* input)
    {
        mInput = input;
    }

    SDLInput* Gui::getInput() const
    {
        return mInput;
    }

    void Gui::logic()
    {
    }

    void Gui::draw()
    {
    }

    void Gui::focusNone()
    {
        mFocusHandler->focusNone();
    }

    void Gui::setTabbingEnabled(bool tabbing)
    {
        mTabbing = tabbing;
    }

    bool Gui::isTabbingEnabled()
    {
        return mTabbing;
    }

    void Gui::addGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Gui::removeGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }

    void Gui::handleMouseInput()
    {
    }

    void Gui::handleKeyInput()
    {
    }

    void Gui::handleMouseMoved(const MouseInput& mouseInput)
    {
        // Check if the mouse leaves the application window.
        if (!mWidgetWithMouseQueue.empty() && (mouseInput.getX() < 0
            || mouseInput.getY() < 0 || !mTop->getDimension().isPointInRect(
            mouseInput.getX(), mouseInput.getY())))
        {
            // Distribute an event to all widgets in the
            // "widget with mouse" queue.
            while (!mWidgetWithMouseQueue.empty())
            {
                Widget *const widget = mWidgetWithMouseQueue.front();

                if (Widget::widgetExists(widget))
                {
                    distributeMouseEvent(widget,
                                         MouseEvent::EXITED,
                                         mouseInput.getButton(),
                                         mouseInput.getX(),
                                         mouseInput.getY(),
                                         true,
                                         true);
                }

                mWidgetWithMouseQueue.pop_front();
            }

            return;
        }

        // Check if there is a need to send mouse exited events by
        // traversing the "widget with mouse" queue.
        bool widgetWithMouseQueueCheckDone = mWidgetWithMouseQueue.empty();
        while (!widgetWithMouseQueueCheckDone)
        {
            unsigned int iterations = 0;
            for (std::deque<Widget*>::iterator
                 iter = mWidgetWithMouseQueue.begin();
                 iter != mWidgetWithMouseQueue.end();
                 ++ iter)
            {
                Widget *const widget = *iter;

                // If a widget in the "widget with mouse queue" doesn't
                // exists anymore it should be removed from the queue.
                if (!Widget::widgetExists(widget))
                {
                    mWidgetWithMouseQueue.erase(iter);
                    break;
                }
                else
                {
                    int x, y;
                    widget->getAbsolutePosition(x, y);

                    if (x > mouseInput.getX()
                        || y > mouseInput.getY()
                        || x + widget->getWidth() <= mouseInput.getX()
                        || y + widget->getHeight() <= mouseInput.getY()
                        || !widget->isVisible())
                    {
                        distributeMouseEvent(widget,
                                             MouseEvent::EXITED,
                                             mouseInput.getButton(),
                                             mouseInput.getX(),
                                             mouseInput.getY(),
                                             true,
                                             true);
                        mClickCount = 1;
                        mLastMousePressTimeStamp = 0;
                        mWidgetWithMouseQueue.erase(iter);
                        break;
                    }
                }

                iterations++;
            }

            widgetWithMouseQueueCheckDone =
                (iterations == mWidgetWithMouseQueue.size());
        }

        // Check all widgets below the mouse to see if they are
        // present in the "widget with mouse" queue. If a widget
        // is not then it should be added and an entered event should
        // be sent to it.
        Widget* parent = getMouseEventSource(
            mouseInput.getX(), mouseInput.getY());
        Widget* widget = parent;

        // If a widget has modal mouse input focus then it will
        // always be returned from getMouseEventSource, but we only wan't to
        // send mouse entered events if the mouse has actually entered the
        // widget with modal mouse input focus, hence we need to check if
        // that's the case. If it's not we should simply ignore to send any
        // mouse entered events.
        if (mFocusHandler->getModalMouseInputFocused()
            && widget == mFocusHandler->getModalMouseInputFocused()
            && Widget::widgetExists(widget))
        {
            int x, y;
            widget->getAbsolutePosition(x, y);

            if (x > mouseInput.getX() || y > mouseInput.getY()
                || x + widget->getWidth() <= mouseInput.getX()
                || y + widget->getHeight() <= mouseInput.getY())
            {
                parent = nullptr;
            }
        }

        while (parent)
        {
            parent = widget->getParent();

            // Check if the widget is present in the "widget with mouse" queue.
            bool widgetIsPresentInQueue = false;
            FOR_EACH (std::deque<Widget*>::const_iterator,
                      iter, mWidgetWithMouseQueue)
            {
                if (*iter == widget)
                {
                    widgetIsPresentInQueue = true;
                    break;
                }
            }

            // Widget is not present, send an entered event and add
            // it to the "widget with mouse" queue.
            if (!widgetIsPresentInQueue
                && Widget::widgetExists(widget))
            {
                distributeMouseEvent(widget,
                                     MouseEvent::ENTERED,
                                     mouseInput.getButton(),
                                     mouseInput.getX(),
                                     mouseInput.getY(),
                                     true,
                                     true);
                mWidgetWithMouseQueue.push_front(widget);
            }

            const Widget *const swap = widget;
            widget = parent;
            parent = swap->getParent();
        }

        if (mFocusHandler->getDraggedWidget())
        {
            distributeMouseEvent(mFocusHandler->getDraggedWidget(),
                                 MouseEvent::DRAGGED,
                                 mLastMouseDragButton,
                                 mouseInput.getX(),
                                 mouseInput.getY());
        }
        else
        {
            Widget *const sourceWidget = getMouseEventSource(
                mouseInput.getX(), mouseInput.getY());

            distributeMouseEvent(sourceWidget,
                                 MouseEvent::MOVED,
                                 mouseInput.getButton(),
                                 mouseInput.getX(),
                                 mouseInput.getY());
        }
    }

    void Gui::handleMouseWheelMovedDown(const MouseInput& mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(
            mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget())
            sourceWidget = mFocusHandler->getDraggedWidget();

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(sourceWidget,
                             MouseEvent::WHEEL_MOVED_DOWN,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());
    }

    void Gui::handleMouseWheelMovedUp(const MouseInput& mouseInput)
    {
        Widget* sourceWidget = getMouseEventSource(
            mouseInput.getX(), mouseInput.getY());

        if (mFocusHandler->getDraggedWidget())
            sourceWidget = mFocusHandler->getDraggedWidget();

        int sourceWidgetX, sourceWidgetY;
        sourceWidget->getAbsolutePosition(sourceWidgetX, sourceWidgetY);

        distributeMouseEvent(sourceWidget,
                             MouseEvent::WHEEL_MOVED_UP,
                             mouseInput.getButton(),
                             mouseInput.getX(),
                             mouseInput.getY());
    }

    Widget* Gui::getWidgetAt(int x, int y)
    {
        // If the widget's parent has no child then we have found the widget..
        Widget* parent = mTop;
        Widget* child = mTop;

        while (child)
        {
            Widget *const swap = child;
            int parentX, parentY;
            parent->getAbsolutePosition(parentX, parentY);
            child = parent->getWidgetAt(x - parentX, y - parentY);
            parent = swap;
        }

        return parent;
    }

    Widget* Gui::getMouseEventSource(int x, int y)
    {
        Widget *const widget = getWidgetAt(x, y);

        // +++ possible nullpointer
        if (mFocusHandler->getModalMouseInputFocused()
            && !widget->isModalMouseInputFocused())
        {
            return mFocusHandler->getModalMouseInputFocused();
        }

        return widget;
    }

    Widget* Gui::getKeyEventSource()
    {
        Widget* widget = mFocusHandler->getFocused();

        // +++ possible nullpointer
        while (widget->_getInternalFocusHandler()
               && widget->_getInternalFocusHandler()->getFocused())
        {
            widget = widget->_getInternalFocusHandler()->getFocused();
        }

        return widget;
    }

    void Gui::distributeMouseEvent(Widget* source,
                                   int type,
                                   int button,
                                   int x,
                                   int y,
                                   bool force,
                                   bool toSourceOnly)
    {
        Widget* parent = source;
        Widget* widget = source;

        if (mFocusHandler->getModalFocused()
            && !widget->isModalFocused()
            && !force)
        {
            return;
        }

        if (mFocusHandler->getModalMouseInputFocused()
            && !widget->isModalMouseInputFocused()
            && !force)
        {
            return;
        }

        MouseEvent mouseEvent(source,
                              mShiftPressed,
                              mControlPressed,
                              mAltPressed,
                              mMetaPressed,
                              type,
                              button,
                              x,
                              y,
                              mClickCount);

        while (parent)
        {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!Widget::widgetExists(widget))
                break;

            parent = widget->getParent();

            if (widget->isEnabled() || force)
            {
                int widgetX, widgetY;
                widget->getAbsolutePosition(widgetX, widgetY);

                mouseEvent.mX = x - widgetX;
                mouseEvent.mY = y - widgetY;

                std::list<MouseListener*> mouseListeners
                    = widget->_getMouseListeners();

                // Send the event to all mouse listeners of the widget.
                for (std::list<MouseListener*>::const_iterator
                     it = mouseListeners.begin();
                     it != mouseListeners.end();
                     ++it)
                {
                    switch (mouseEvent.getType())
                    {
                        case MouseEvent::ENTERED:
                            (*it)->mouseEntered(mouseEvent);
                            break;
                        case MouseEvent::EXITED:
                            (*it)->mouseExited(mouseEvent);
                            break;
                        case MouseEvent::MOVED:
                            (*it)->mouseMoved(mouseEvent);
                            break;
                        case MouseEvent::PRESSED:
                            (*it)->mousePressed(mouseEvent);
                            break;
                        case MouseEvent::RELEASED:
                            (*it)->mouseReleased(mouseEvent);
                            break;
                        case MouseEvent::WHEEL_MOVED_UP:
                            (*it)->mouseWheelMovedUp(mouseEvent);
                            break;
                        case MouseEvent::WHEEL_MOVED_DOWN:
                            (*it)->mouseWheelMovedDown(mouseEvent);
                            break;
                        case MouseEvent::DRAGGED:
                            (*it)->mouseDragged(mouseEvent);
                            break;
                        case MouseEvent::CLICKED:
                            (*it)->mouseClicked(mouseEvent);
                            break;
                        default:
                            break;
                    }
                }

                if (toSourceOnly)
                    break;
            }

            const Widget *const swap = widget;
            widget = parent;
            parent = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused()
                && !widget->isModalFocused())
            {
                break;
            }

            // If a non modal mouse input focused widget has been reach
            // and we have modal mouse input focus cancel the distribution.
            if (mFocusHandler->getModalMouseInputFocused()
                && !widget->isModalMouseInputFocused())
            {
                break;
            }
        }
    }

    void Gui::distributeKeyEvent(KeyEvent& keyEvent)
    {
        Widget* parent = keyEvent.getSource();
        Widget* widget = keyEvent.getSource();

        if (mFocusHandler->getModalFocused()
            && !widget->isModalFocused())
        {
            return;
        }

        if (mFocusHandler->getModalMouseInputFocused()
            && !widget->isModalMouseInputFocused())
        {
            return;
        }

        while (parent)
        {
            // If the widget has been removed due to input
            // cancel the distribution.
            if (!Widget::widgetExists(widget))
                break;

            parent = widget->getParent();

            if (widget->isEnabled())
            {
                std::list<KeyListener*> keyListeners
                    = widget->_getKeyListeners();

                // Send the event to all key listeners of the source widget.
                for (std::list<KeyListener*>::const_iterator
                     it = keyListeners.begin();
                     it != keyListeners.end();
                     ++it)
                {
                    switch (keyEvent.getType())
                    {
                        case KeyEvent::PRESSED:
                            (*it)->keyPressed(keyEvent);
                            break;
                        case KeyEvent::RELEASED:
                            (*it)->keyReleased(keyEvent);
                            break;
                        default:
                            break;
                    }
                }
            }

            const Widget *const swap = widget;
            widget = parent;
            parent = swap->getParent();

            // If a non modal focused widget has been reach
            // and we have modal focus cancel the distribution.
            if (mFocusHandler->getModalFocused()
                && !widget->isModalFocused())
            {
                break;
            }
        }
    }

    void Gui::distributeKeyEventToGlobalKeyListeners(KeyEvent& keyEvent)
    {
        for (KeyListenerListIterator it = mKeyListeners.begin();
             it != mKeyListeners.end(); ++ it)
        {
            switch (keyEvent.getType())
            {
                case KeyEvent::PRESSED:
                    (*it)->keyPressed(keyEvent);
                    break;
                case KeyEvent::RELEASED:
                    (*it)->keyReleased(keyEvent);
                    break;
                default:
                    break;
            }

            if (keyEvent.isConsumed())
                break;
        }
    }

    void Gui::handleModalMouseInputFocus()
    {
        BLOCK_START("Gui::handleModalMouseInputFocus")
        // Check if modal mouse input focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalMouseInputFocus()
            != mFocusHandler->getModalMouseInputFocused())
            && (!mFocusHandler->getLastWidgetWithModalMouseInputFocus()))
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(
                mFocusHandler->getModalMouseInputFocused());
        }
        // Check if modal mouse input focus has been released.
        else if ((mFocusHandler->getLastWidgetWithModalMouseInputFocus()
                 != mFocusHandler->getModalMouseInputFocused())
                 && (mFocusHandler->getLastWidgetWithModalMouseInputFocus()))
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalMouseInputFocus(nullptr);
        }
        BLOCK_END("Gui::handleModalMouseInputFocus")
    }

    void Gui::handleModalFocus()
    {
        BLOCK_START("Gui::handleModalFocus")
        // Check if modal focus has been gained by a widget.
        if ((mFocusHandler->getLastWidgetWithModalFocus()
            != mFocusHandler->getModalFocused())
            && (!mFocusHandler->getLastWidgetWithModalFocus()))
        {
            handleModalFocusGained();
            mFocusHandler->setLastWidgetWithModalFocus(
                mFocusHandler->getModalFocused());
        }
        // Check if modal focus has been released.
        else if ((mFocusHandler->getLastWidgetWithModalFocus()
                 != mFocusHandler->getModalFocused())
                 && (mFocusHandler->getLastWidgetWithModalFocus()))
        {
            handleModalFocusReleased();
            mFocusHandler->setLastWidgetWithModalFocus(nullptr);
        }
        BLOCK_END("Gui::handleModalFocus")
    }

    void Gui::handleModalFocusGained()
    {
         // Distribute an event to all widgets in the "widget with mouse" queue.
        while (!mWidgetWithMouseQueue.empty())
        {
            Widget *const widget = mWidgetWithMouseQueue.front();

            if (Widget::widgetExists(widget))
            {
                distributeMouseEvent(widget,
                                     MouseEvent::EXITED,
                                     mLastMousePressButton,
                                     mLastMouseX,
                                     mLastMouseY,
                                     true,
                                     true);
            }

            mWidgetWithMouseQueue.pop_front();
        }

        mFocusHandler->setLastWidgetWithModalMouseInputFocus(
            mFocusHandler->getModalMouseInputFocused());
    }

    void Gui::handleModalFocusReleased()
    {
         // Check all widgets below the mouse to see if they are
        // present in the "widget with mouse" queue. If a widget
        // is not then it should be added and an entered event should
        // be sent to it.
        Widget* widget = getMouseEventSource(mLastMouseX, mLastMouseY);
        Widget* parent = widget;

        while (parent)
        {
            parent = widget->getParent();

            // Check if the widget is present in the "widget with mouse" queue.
            bool widgetIsPresentInQueue = false;
            FOR_EACH (std::deque<Widget*>::const_iterator,
                      iter, mWidgetWithMouseQueue)
            {
                if (*iter == widget)
                {
                    widgetIsPresentInQueue = true;
                    break;
                }
            }

            // Widget is not present, send an entered event and add
            // it to the "widget with mouse" queue.
            if (!widgetIsPresentInQueue && Widget::widgetExists(widget))
            {
                distributeMouseEvent(widget,
                                     MouseEvent::ENTERED,
                                     mLastMousePressButton,
                                     mLastMouseX,
                                     mLastMouseY,
                                     false,
                                     true);
                mWidgetWithMouseQueue.push_front(widget);
            }

            const Widget *const swap = widget;
            widget = parent;
            parent = swap->getParent();
        }
    }
}  // namespace gcn
