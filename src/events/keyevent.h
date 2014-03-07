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

#ifndef EVENTS_KEYEVENT_H
#define EVENTS_KEYEVENT_H

#include "events/inputguievent.h"
#include "input/key.h"

#include <string>

class Widget;

/**
  * Represents a key event.
  */
class KeyEvent: public InputGuiEvent
{
    public:
        /**
          * Key event types.
          */
        enum
        {
            PRESSED = 0,
            RELEASED
        };

        /**
          * Constructor.
          *
          * @param source The source widget of the event.
          * @param type The type of the event. A value from KeyEventType.
          *                     false otherwise.
          * @param key The key of the event.
          */
        KeyEvent(Widget *const source,
                 const unsigned int type,
                 const int actionId,
                 const Key &key) :
            InputGuiEvent(source),
            mKey(key),
#ifdef USE_SDL2
            mText(),
#endif
            mType(type),
            mActionId(actionId)
        { }

        /**
          * Destructor.
          */
        virtual ~KeyEvent()
        { }

        /**
          * Gets the type of the event.
          *
          * @return The type of the event.
          */
        unsigned int getType() const A_WARN_UNUSED
        { return mType; }

        /**
          * Gets the key of the event.
          *
          * @return The key of the event.
          */
        const Key &getKey() const A_WARN_UNUSED
        { return mKey; }

        int getActionId() const A_WARN_UNUSED
        { return mActionId; }

#ifdef USE_SDL2
        void setText(const std::string &text)
        { mText = text; }

        std::string getText() const
        { return mText; }
#endif

    protected:
        /** 
          * Holds the key of the key event.
          */
        Key mKey;

#ifdef USE_SDL2
        std::string mText;
#endif

        /**
          * Holds the type of the key event.
          */
        unsigned int mType;

        int mActionId;
};

#endif  // EVENTS_KEYEVENT_H
