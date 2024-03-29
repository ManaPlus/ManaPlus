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

#ifndef EVENTS_ACTIONEVENT_H
#define EVENTS_ACTIONEVENT_H

#include "events/event.h"

#include <string>

#include "localconsts.h"

class Widget;

/**
  * Represents an action event. An action event is an event
  * that can be fired by a widget whenever an action has occured.
  * What exactly an action is is up to the widget that fires
  * the action event. An example is a Button which fires an action
  * event as soon as the Button is clicked, another example is
  * TextField which fires an action event as soon as the enter
  * key is pressed.
  *
  * Any object can listen for actions from widgets by implementing
  * the ActionListener interface.
  *
  * If you have implement a widget of your own it's a good idea to
  * let the widget fire action events whenever you feel an action
  * has occured so action listeners of the widget can be informed
  * of the state of the widget.
  *
  * @see Widget::addActionListener, Widget::removeActionListener,
  *      Widget::distributeActionEvent
  * @author Olof Naessén
  */
class ActionEvent final : public Event
{
    public:
        /**
          * Constructor.
          *
          * @param source The source widget of the event.
          * @param id An identifier of the event.
          */
        ActionEvent(Widget *const source, const std::string &id) :
            Event(source),
            mId(id)
        {
        }

        A_DELETE_COPY(ActionEvent)

        /**
          * Gets the identifier of the event. An identifier can
          * be used to distinguish from two actions from the same
          * widget or to let many widgets fire the same widgets
          * that should be treated equally.
          *
          * @return The identifier of the event.
          */
        const std::string &getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

    protected:
        /**
          * Holds the identifier of the event.
          */
        std::string mId;
};

#endif  // EVENTS_ACTIONEVENT_H
