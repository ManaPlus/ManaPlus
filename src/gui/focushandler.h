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

#ifndef GUI_FOCUSHANDLER_H
#define GUI_FOCUSHANDLER_H

#include "gui/focushandler.h"

#include <list>
#include <vector>

#include "localconsts.h"

class Event;
class Widget;

/**
 * The focus handler. This focus handler does exactly the same as the Guichan
 * focus handler, but keeps a stack of modal widgets to be able to handle
 * multiple modal focus requests.
 */
class FocusHandler final
{
    public:
        FocusHandler();

        A_DELETE_COPY(FocusHandler)

        /**
          * Requests focus for a widget. Focus will only be granted to a widget
          * if it's focusable and if no other widget has modal focus.
          * If a widget receives focus a focus event will be sent to the
          * focus listeners of the widget.
          *
          * @param widget The widget to request focus for.
          * @see isFocused, Widget::requestFocus
          */
        void requestFocus(Widget *const widget);

        /**
          * Requests modal focus for a widget. Focus will only be granted
          * to a widget if it's focusable and if no other widget has modal
          * focus.
          *
          * @param widget The widget to request modal focus for.
          * @throws Exception when another widget already has modal focus.
          * @see releaseModalFocus, Widget::requestModalFocus
          */
        void requestModalFocus(Widget *const widget);

        /**
          * Requests modal mouse input focus for a widget. Focus will only
          * be granted to a widget if it's focusable and if no other widget
          * has modal mouse input focus.
          *
          * Modal mouse input focus means no other widget then the widget with
          * modal mouse input focus will receive mouse input. The widget with
          * modal mouse input focus will also receive mouse input no matter what
          * the mouse input is or where the mouse input occurs.
          *
          * @param widget The widget to focus for modal mouse input focus.
          * @throws Exception when another widget already has modal mouse input
          *         focus.
          * @see releaseModalMouseInputFocus, Widget::requestModalMouseInputFocus
          */
        void requestModalMouseInputFocus(Widget *const widget);

        /**
          * Releases modal focus if the widget has modal focus.
          * If the widget doesn't have modal focus no relase will occur.
          *
          * @param widget The widget to release modal focus for.
          * @see reuqestModalFocus, Widget::releaseModalFocus
          */
        void releaseModalFocus(Widget *const widget);

        /**
          * Releases modal mouse input focus if the widget has modal mouse input
          * focus. If the widget doesn't have modal mouse input focus no relase
          * will occur.
          *
          * @param widget the widget to release modal mouse input focus for.
          * @see requestModalMouseInputFocus, Widget::releaseModalMouseInputFocus
          */
        void releaseModalMouseInputFocus(Widget *const widget);

        /**
          * Checks if a widget is focused.
          *
          * @param widget The widget to check.
          * @return True if the widget is focused, false otherwise.
          * @see Widget::isFocused
          */
        bool isFocused(const Widget *const widget) const;

        /**
          * Gets the widget with focus.
          *
          * @return The widget with focus. NULL if no widget has focus.
          */
        Widget* getFocused() const A_WARN_UNUSED;

        /**
          * Gets the widget with modal focus.
          *
          * @return The widget with modal focus. NULL if no widget has
          *         modal focus.
          */
        Widget* getModalFocused() const A_WARN_UNUSED;

        /**
          * Gets the widget with modal mouse input focus.
          *
          * @return The widget with modal mouse input focus. NULL if
          *         no widget has modal mouse input focus.
          */
        Widget* getModalMouseInputFocused() const A_WARN_UNUSED;

        /**
          * Focuses the next widget added to a conainer.
          * If no widget has focus the first widget gets focus. The order
          * in which the widgets are focused is determined by the order
          * they were added to a container.
          *
          * @see focusPrevious
          */
        void focusNext();

        /**
          * Focuses the previous widget added to a contaienr.
          * If no widget has focus the first widget gets focus. The order
          * in which the widgets are focused is determined by the order
          * they were added to a container.
          *
          * @see focusNext
          */
        void focusPrevious();

        /**
          * Adds a widget to by handles by the focus handler.
          *
          * @param widget The widget to add.
          * @see remove
          */
        void add(Widget *const widget);

        /**
          * Removes a widget from the focus handler.
          *
          * @param widget The widget to remove.
          * @see add
          */
        void remove(Widget *const widget);

        /**
          * Focuses nothing. A focus event will also be sent to the
          * focused widget's focus listeners if a widget has focus.
          */
        void focusNone();

        /**
          * Focuses the next widget which allows tabbing in unless
          * the current focused Widget disallows tabbing out.
          *
          * @see tabPrevious
          */
        void tabNext();

        /**
          * Focuses the previous widget which allows tabbing in unless
          * current focused widget disallows tabbing out.
          *
          * @see tabNext
          */
        void tabPrevious();

        /**
          * Gets the widget being dragged. Used by the Gui class to
          * keep track of the dragged widget.
          *
          * @return the widget being dragged.
          * @see setDraggedWidget
          */
        Widget* getDraggedWidget() const A_WARN_UNUSED;

        /**
          * Sets the widget being dragged. Used by the Gui class to
          * keep track of the dragged widget.
          *
          * @param draggedWidget The widget being dragged.
          * @see getDraggedWidget
          */
        void setDraggedWidget(Widget *const draggedWidget);

        /**
          * Gets the last widget with the mouse. Used by the Gui class
          * to keep track the last widget with the mouse.
          *
          * @return The last widget with the mouse.
          * @see setLastWidgetWithMouse
          */
        Widget* getLastWidgetWithMouse() const A_WARN_UNUSED;

        /**
          * Sets the last widget with the mouse. Used by the Gui class
          * to keep track the last widget with the mouse.
          *
          * @param lastWidgetWithMouse The last widget with the mouse.
          * @see getLastWidgetWithMouse
          */
        void setLastWidgetWithMouse(Widget *const lastWidgetWithMouse);

        /**
          * Gets the last widget with modal focus.
          *
          * @return The last widget with modal focus.
          * @see setLastWidgetWithModalFocus
          */
        Widget* getLastWidgetWithModalFocus() const A_WARN_UNUSED;

        /**
          * Sets the last widget with modal focus.
          *
          * @param widget The last widget with modal focus.
          * @see getLastWidgetWithModalFocus
          */
        void setLastWidgetWithModalFocus(Widget *const widget);

        /**
          * Gets the last widget with modal mouse input focus.
          *
          * @return The last widget with modal mouse input focus.
          * @see setLastWidgetWithModalMouseInputFocus
          */
        Widget* getLastWidgetWithModalMouseInputFocus() const A_WARN_UNUSED;

        /**
          * Sets the last widget with modal mouse input focus.
          *
          * @param widget The last widget with modal mouse input focus.
          * @see getLastWidgetWithModalMouseInputFocus
          */
        void setLastWidgetWithModalMouseInputFocus(Widget *const widget);

        /**
          * Gets the last widget pressed. Used by the Gui class to keep track
          * of pressed widgets.
          *
          * @return The last widget pressed.
          * @see setLastWidgetPressed
          */
        Widget* getLastWidgetPressed() const A_WARN_UNUSED;

        /**
          * Sets the last widget pressed. Used by the Gui class to keep track
          * of pressed widgets.
          *
          * @param lastWidgetPressed The last widget pressed.
          * @see getLastWidgetPressed
          */
        void setLastWidgetPressed(Widget *const lastWidgetPressed);

    private:
        /**
         * Checks to see if the widget tabbed to is in a window, and if it is,
         * it requests the window be moved to the top.
         */
        void checkForWindow() const;

        /**
         * Distributes a focus lost event.
         *
         * @param focusEvent the event to distribute.
         * @since 0.7.0
         */
        static void distributeFocusLostEvent(const Event& focusEvent);

        /**
         * Distributes a focus gained event.
         *
         * @param focusEvent the event to distribute.
         * @since 0.7.0
         */
        static void distributeFocusGainedEvent(const Event& focusEvent);

        /**
         * Typedef.
         */
        typedef std::vector<Widget*> WidgetVector;

        /**
         * Typedef.
         */
        typedef WidgetVector::iterator WidgetIterator;

        /**
         * Holds the widgets currently being handled by the
         * focus handler.
         */
        WidgetVector mWidgets;

        /**
         * Holds the focused widget. NULL if no widget has focus.
         */
        Widget* mFocusedWidget;

        /**
         * Holds the modal focused widget. NULL if no widget has
         * modal focused.
         */
        Widget* mModalFocusedWidget;

        /**
         * Holds the modal mouse input focused widget. NULL if no widget
         * is being dragged.
         */
        Widget* mModalMouseInputFocusedWidget;

        /**
         * Holds the dragged widget. NULL if no widget is
         * being dragged.
         */
        Widget* mDraggedWidget;

        /**
         * Holds the last widget with the mouse.
         */
        Widget* mLastWidgetWithMouse;

        /**
         * Holds the last widget with modal focus.
         */
        Widget* mLastWidgetWithModalFocus;

        /**
         * Holds the last widget with modal mouse input focus.
         */
        Widget* mLastWidgetWithModalMouseInputFocus;

        /**
         * Holds the last widget pressed.
         */
        Widget* mLastWidgetPressed;

        /**
         * Stack of widgets that have requested modal forcus.
         */
        std::list<Widget*> mModalStack;
};

#endif  // GUI_FOCUSHANDLER_H
