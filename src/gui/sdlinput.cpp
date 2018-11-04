/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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
 * Copyright (c) 2004, 2005, 2006, 2007 Olof Naessén and Per Larsson
 * Copyright (C) 2007-2010  The Mana World Development Team
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naessén a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdlshared.h"

#include "input/inputmanager.h"

#include "gui/gui.h"
#include "gui/sdlinput.h"

#include "render/graphics.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_timer.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

extern volatile time_t cur_time;

SDLInput *guiInput = nullptr;

SDLInput::SDLInput() :
    mKeyInputQueue(),
    mMouseInputQueue(),
    mMouseMoveTime(0),
    mMouseDown(false),
    mMouseInWindow(true)
{
}

KeyInput SDLInput::dequeueKeyInput()
{
    if (mKeyInputQueue.empty())
        return KeyInput();

    KeyInput keyInput = mKeyInputQueue.front();
    mKeyInputQueue.pop();

    return keyInput;
}

MouseInput SDLInput::dequeueMouseInput()
{
    MouseInput mouseInput;

    if (mMouseInputQueue.empty())
        return MouseInput();

    mouseInput = mMouseInputQueue.front();
    mMouseInputQueue.pop();

    return mouseInput;
}

void SDLInput::pushInput(const SDL_Event &event)
{
    BLOCK_START("SDLInput::pushInput")
    KeyInput keyInput;
    MouseInput mouseInput;

    switch (event.type)
    {
        case SDL_KEYDOWN:
        {
            keyInput.setType(KeyEventType::PRESSED);
            convertKeyEventToKey(event, keyInput);
            mKeyInputQueue.push(keyInput);
            break;
        }

        case SDL_KEYUP:
        {
            keyInput.setType(KeyEventType::RELEASED);
            convertKeyEventToKey(event, keyInput);
            mKeyInputQueue.push(keyInput);
            break;
        }

#ifdef USE_SDL2
        case SDL_TEXTINPUT:
            keyInput.setType(KeyEventType::PRESSED);
            keyInput.setKey(Key(KeyValue::TEXTINPUT));
            keyInput.setText(event.text.text);
            mKeyInputQueue.push(keyInput);
            break;

        case SDL_MOUSEWHEEL:
        {
            const int y = event.wheel.y;
            if (y)
            {
                mouseInput.setX(gui->getLastMouseX());
                mouseInput.setY(gui->getLastMouseY());
#ifdef ANDROID
                mouseInput.setReal(0, 0);
#endif  // ANDROID

                mouseInput.setButton(MouseButton::WHEEL);
                if (y > 0)
                    mouseInput.setType(MouseEventType::WHEEL_MOVED_UP);
                else
                    mouseInput.setType(MouseEventType::WHEEL_MOVED_DOWN);
                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
            }

            break;
        }
#endif  // USE_SDL2

#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
            break;
#endif  // USE_SDL2
#endif  // ANDROID

        case SDL_MOUSEBUTTONDOWN:
        {
            mMouseDown = true;
            const int scale = mainGraphics->getScale();
            const int x = event.button.x / scale;
            const int y = event.button.y / scale;
            mouseInput.setX(x);
            mouseInput.setY(y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(x, y);
#else  // USE_SDL2

            mouseInput.setReal(event.button.realx / scale,
                event.button.realy / scale);
#endif  // USE_SDL2
#endif  // ANDROID

            mouseInput.setButton(convertMouseButton(event.button.button));

#ifndef USE_SDL2
            if (event.button.button == SDL_BUTTON_WHEELDOWN)
                mouseInput.setType(MouseEventType::WHEEL_MOVED_DOWN);
            else if (event.button.button == SDL_BUTTON_WHEELUP)
                mouseInput.setType(MouseEventType::WHEEL_MOVED_UP);
            else
#endif  // USE_SDL2
                mouseInput.setType(MouseEventType::PRESSED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            mMouseDown = false;
            const int scale = mainGraphics->getScale();
            const int x = event.button.x / scale;
            const int y = event.button.y / scale;
            mouseInput.setX(x);
            mouseInput.setY(y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(x, y);
#else  // USE_SDL2

            mouseInput.setReal(event.button.realx / scale,
                event.button.realy / scale);
#endif  // USE_SDL2
#endif  // ANDROID

            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(MouseEventType::RELEASED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            const int scale = mainGraphics->getScale();
            const int x = event.motion.x / scale;
            const int y = event.motion.y / scale;
            mouseInput.setX(x);
            mouseInput.setY(y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(x, y);
#else  // USE_SDL2

            mouseInput.setReal(event.motion.realx / scale,
                event.motion.realy / scale);
#endif  // USE_SDL2
#endif  // ANDROID

            mouseInput.setButton(MouseButton::EMPTY);
            mouseInput.setType(MouseEventType::MOVED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            mMouseMoveTime = cur_time;
            break;
        }
#ifndef USE_SDL2
        case SDL_ACTIVEEVENT:
            /*
             * This occurs when the mouse leaves the window and the Gui-chan
             * application loses its mousefocus.
             */
            if ((event.active.state & SDL_APPMOUSEFOCUS) != 0 &&
                event.active.gain == 0U)
            {
                mMouseInWindow = false;

                if (!mMouseDown)
                {
                    mouseInput.setX(-1);
                    mouseInput.setY(-1);
                    mouseInput.setButton(MouseButton::EMPTY);
                    mouseInput.setType(MouseEventType::MOVED);
                    mMouseInputQueue.push(mouseInput);
                    mMouseMoveTime = cur_time;
                }
            }

            if ((event.active.state & SDL_APPMOUSEFOCUS) != 0 &&
                event.active.gain != 0U)
            {
                mMouseInWindow = true;
            }
            break;
#endif  // USE_SDL2

        default:
            break;
    }  // end switch
    BLOCK_END("SDLInput::pushInput")
}

void SDLInput::convertKeyEventToKey(const SDL_Event &event, KeyInput &keyInput)
{
    keyInput.setKey(Key(convertKeyCharacter(event)));
    const InputActionT actionId = inputManager.getActionByKey(event);
    if (actionId > InputAction::NO_VALUE)
        keyInput.setActionId(actionId);
}

MouseButtonT SDLInput::convertMouseButton(const int button)
{
    switch (button)
    {
        case SDL_BUTTON_LEFT:
            return MouseButton::LEFT;
        case SDL_BUTTON_RIGHT:
            return MouseButton::RIGHT;
        case SDL_BUTTON_MIDDLE:
            return MouseButton::MIDDLE;
#ifndef USE_SDL2
        case SDL_BUTTON_WHEELUP:
        case SDL_BUTTON_WHEELDOWN:
            return MouseButton::EMPTY;
#endif  // USE_SDL2

        default:
            // We have an unknown mouse type which is ignored.
            logger->log("unknown button type: %d", button);
            return MouseButton::EMPTY;
    }
}

void SDLInput::simulateMouseClick(const int x, const int y,
                                  const MouseButtonT button)
{
    MouseInput mouseInput;
    mouseInput.setX(x);
    mouseInput.setY(y);
    mouseInput.setReal(x, y);
    mouseInput.setButton(MouseButton::EMPTY);
    mouseInput.setType(MouseEventType::MOVED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
    mouseInput.setButton(button);
    mouseInput.setType(MouseEventType::PRESSED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
    mouseInput.setType(MouseEventType::RELEASED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
}

void SDLInput::simulateMouseMove()
{
    if (gui == nullptr)
        return;

    if (mMouseMoveTime == cur_time)
        return;

    mMouseMoveTime = cur_time;
    int x;
    int y;
    Gui::getMouseState(x, y);

    MouseInput mouseInput;
    mouseInput.setX(x);
    mouseInput.setY(y);
    mouseInput.setReal(x, y);
    mouseInput.setButton(MouseButton::EMPTY);
    mouseInput.setType(MouseEventType::MOVED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
}

void SDLInput::simulateKey(const int guiKey,
                           const InputActionT actionId)
{
    KeyInput keyInput;
    keyInput.setType(KeyEventType::PRESSED);
#ifdef USE_SDL2
    char str[2];
    str[0] = CAST_S8(guiKey);
    str[1] = 0;

    keyInput.setKey(Key(KeyValue::TEXTINPUT));
    keyInput.setText(str);
    if (guiKey >= 32)
        mKeyInputQueue.push(keyInput);
#endif  // USE_SDL2

    keyInput.setKey(Key(guiKey));
    if (actionId > InputAction::NO_VALUE)
        keyInput.setActionId(actionId);
    mKeyInputQueue.push(keyInput);
    keyInput.setType(KeyEventType::RELEASED);
    mKeyInputQueue.push(keyInput);
}
