/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef INPUT_KEYINPUT_H
#define INPUT_KEYINPUT_H

#include "enums/events/keyeventtype.h"

#include "enums/input/inputaction.h"

#include "input/key.h"

#ifdef USE_SDL2
#include <string>
#endif  // USE_SDL2

#include "localconsts.h"

class KeyInput final
{
    public:
        KeyInput() :
            mKey(0),
            mType(KeyEventType::PRESSED),
#ifdef USE_SDL2
            mText(),
#endif  // USE_SDL2
            mActionId(InputAction::UNDEFINED_VALUE)
        { }

        KeyInput(const KeyInput &k) :
            mKey(k.mKey),
            mType(k.mType),
#ifdef USE_SDL2
            mText(k.mText),
#endif  // USE_SDL2
            mActionId(k.mActionId)
        {
        }

        A_DEFAULT_COPY(KeyInput)

        KeyInput &operator=(const KeyInput &k) noexcept2
        {
            mKey = k.mKey;
            mType = k.mType;
#ifdef USE_SDL2
            mText = k.mText;
#endif  // USE_SDL2
            mActionId = k.mActionId;
            return *this;
        }

        ~KeyInput()
        { }

        void setType(KeyEventTypeT type) noexcept2
        {
            mType = type;
        }

        KeyEventTypeT getType() const noexcept2 A_WARN_UNUSED
        {
            return mType;
        }

        void setKey(const Key& key) noexcept2
        {
            mKey = key;
        }

        const Key& getKey() const noexcept2
        {
            return mKey;
        }

        void setActionId(const InputActionT n) noexcept2
        {
            mActionId = n;
        }

        InputActionT getActionId() const noexcept2 A_WARN_UNUSED
        {
            return mActionId;
        }

#ifdef USE_SDL2
        void setText(const std::string &text) noexcept2
        {
            mText = text;
        }

        std::string getText() const noexcept2
        {
            return mText;
        }
#endif  // USE_SDL2

    protected:
        /**
         * Holds the key of the key input.
         */
        Key mKey;

        /**
         * Holds the type of the key input.
         */
        KeyEventTypeT mType;

#ifdef USE_SDL2
        std::string mText;
#endif  // USE_SDL2

        InputActionT mActionId;
};

#endif  // INPUT_KEYINPUT_H
