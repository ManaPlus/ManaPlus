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

#ifndef EVENTS_MOUSEEVENT_H
#define EVENTS_MOUSEEVENT_H

#include "events/inputguievent.h"

#include "enums/events/mousebutton.h"
#include "enums/events/mouseeventtype.h"

class Widget;

/**
  * Represents a mouse event.
  *
  * @author Olof Naessén
  */
class MouseEvent final : public InputGuiEvent
{
    public:
        /**
          * Constructor.
          *
          * @param source The source widget of the mouse event.
          * @param type The type of the mouse event.
          * @param button The button of the mouse event.
          * @param x The x coordinate of the event relative to the source widget.
          * @param y The y coordinate of the event relative the source widget.
          * @param clickCount The number of clicks generated with the same button.
          *                   It's set to zero if another button is used.
          */
        MouseEvent(Widget *const source,
                   const MouseEventTypeT type,
                   const MouseButtonT button,
                   const int x,
                   const int y,
                   const int clickCount) :
            InputGuiEvent(source),
            mType(type),
            mButton(button),
            mX(x),
            mY(y),
            mClickCount(clickCount)
        {
        }

        A_DELETE_COPY(MouseEvent)

        /**
          * Gets the button of the mouse event.
          *
          * @return The button of the mouse event.
          */
        MouseButtonT getButton() const noexcept2 A_WARN_UNUSED
        { return mButton; }

        /**
          * Gets the x coordinate of the mouse event.
          * The coordinate relative to widget the mouse listener
          * receiving the events have registered to.
          *
          * @return The x coordinate of the mouse event.
          * @see Widget::addMouseListener, Widget::removeMouseListener
          */
        int getX() const noexcept2 A_WARN_UNUSED
        { return mX; }

        /**
          * Gets the y coordinate of the mouse event.
          * The coordinate relative to widget the mouse listener
          * receiving the events have registered to.
          *
          * @return The y coordinate of the mouse event.
          * @see Widget::addMouseListener, Widget::removeMouseListener
          */
        int getY() const noexcept2 A_WARN_UNUSED
        { return mY; }

        /**
          * Gets the number of clicks generated with the same button.
          * It's set to zero if another button is used.
          *
          * @return The number of clicks generated with the same button.
          */
        int getClickCount() const noexcept2 A_WARN_UNUSED
        { return mClickCount; }

        /**
          * Gets the type of the event.
          *
          * @return The type of the event.
          */
        MouseEventTypeT getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void setX(int n)
        { mX = n; }

        void setY(int n)
        { mY = n; }

    protected:
        /**
          * Holds the type of the mouse event.
          */
        MouseEventTypeT mType;

        /**
          * Holds the button of the mouse event.
          */
        MouseButtonT mButton;

        /**
          * Holds the x-coordinate of the mouse event.
          */
        int mX;

        /**
          * Holds the y-coordinate of the mouse event.
          */
        int mY;

        /**
          * The number of clicks generated with the same button.
          * It's set to zero if another button is used.
          */
        int mClickCount;

        /**
          * Gui is a friend of this class in order to be able to manipulate
          * the protected member variables of this class and at the same time
          * keep the MouseEvent class as const as possible. Gui needs to
          * update the x och y coordinates for the coordinates to be relative
          * to widget the mouse listener receiving the events have registered
          * to.
          */
        friend class Gui;
};

#endif  // EVENTS_MOUSEEVENT_H
