/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef INPUT_MOUSEINPUT_H
#define INPUT_MOUSEINPUT_H

#include "enums/events/mousebutton.h"
#include "enums/events/mouseeventtype.h"

#include "localconsts.h"

class MouseInput final
{
    public:
        MouseInput() noexcept2 :
            mType(MouseEventType::MOVED),
            mButton(MouseButton::EMPTY),
            mTimeStamp(0),
            mX(0),
            mY(0),
            mRealX(0),
            mRealY(0)
        { }

        MouseInput(const MouseInput &m) noexcept2 :
            mType(m.mType),
            mButton(m.mButton),
            mTimeStamp(m.mTimeStamp),
            mX(m.mX),
            mY(m.mY),
            mRealX(m.mRealX),
            mRealY(m.mRealY)
        {
        }

        A_DEFAULT_COPY(MouseInput)

        MouseInput &operator=(const MouseInput &m) noexcept2
        {
            mType = m.mType;
            mButton = m.mButton;
            mTimeStamp = m.mTimeStamp;
            mX = m.mX;
            mY = m.mY;
            mRealX = m.mRealX;
            mRealY = m.mRealY;
            return *this;
        }

        ~MouseInput()
        { }

        void setType(MouseEventTypeT type) noexcept2
        {
            mType = type;
        }

        MouseEventTypeT getType() const noexcept2
        {
            return mType;
        }

        void setButton(MouseButtonT button) noexcept2
        {
            mButton = button;
        }

        MouseButtonT getButton() const noexcept2
        {
            return mButton;
        }

        int getTimeStamp() const noexcept2
        {
            return mTimeStamp;
        }

        void setTimeStamp(int timeStamp) noexcept2
        {
            mTimeStamp = timeStamp;
        }

        void setX(int x) noexcept2
        {
            mX = x;
        }

        int getX() const noexcept2
        {
            return mX;
        }

        void setY(int y) noexcept2
        {
            mY = y;
        }

        int getY() const noexcept2
        {
            return mY;
        }

        void setReal(const int x, const int y) noexcept2
        { mRealX = x; mRealY = y; }

        int getRealX() const noexcept2 A_WARN_UNUSED
        { return mRealX; }

        int getRealY() const noexcept2 A_WARN_UNUSED
        { return mRealY; }

#ifdef ANDROID
        int getTouchX() const noexcept2 A_WARN_UNUSED
        { return mRealX; }

        int getTouchY() const noexcept2 A_WARN_UNUSED
        { return mRealY; }
#else  // ANDROID

        int getTouchX() const noexcept2 A_WARN_UNUSED
        { return mX; }

        int getTouchY() const noexcept2 A_WARN_UNUSED
        { return mY; }
#endif  // ANDROID

    protected:
        /**
         * Holds the type of the mouse input.
         */
        MouseEventTypeT mType;

        /**
         * Holds the button of the mouse input.
         */
        MouseButtonT mButton;

        /**
         * Holds the timestamp of the mouse input. Used to
         * check for double clicks.
         */
        int mTimeStamp;

        /**
         * Holds the x coordinate of the mouse input.
         */
        int mX;

        /**
         * Holds the y coordinate of the mouse input.
         */
        int mY;

        int mRealX;

        int mRealY;
};

#endif  // INPUT_MOUSEINPUT_H
