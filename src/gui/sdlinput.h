/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_SDLINPUT_H
#define GUI_SDLINPUT_H

#include "enums/input/keyvalue.h"

#include "input/keyinput.h"

#include <SDL_events.h>

#ifdef USE_SDL2
#include "sdlshared.h"
#endif  // USE_SDL2

#include "input/mouseinput.h"

#include <queue>

/**
 * SDL implementation of SDLInput.
 */
class SDLInput final
{
    public:
        /**
         * Constructor.
         */
        SDLInput();

        A_DELETE_COPY(SDLInput)

        /**
         * Pushes an SDL event. It should be called at least once per frame to
         * update input with user input.
         *
         * @param event an event from SDL.
         */
        void pushInput(const SDL_Event &event);

        KeyInput dequeueKeyInput() A_WARN_UNUSED;

        bool isKeyQueueEmpty() const noexcept2 A_WARN_UNUSED
        { return mKeyInputQueue.empty(); }

        bool isMouseQueueEmpty() const noexcept2 A_WARN_UNUSED
        { return mMouseInputQueue.empty(); }

        MouseInput dequeueMouseInput() A_WARN_UNUSED;

        void simulateMouseClick(const int x, const int y,
                                const MouseButtonT button);

        void simulateMouseMove();

    protected:
        /**
         * Converts a mouse button from SDL to a Guichan mouse button
         * representation.
         *
         * @param button an SDL mouse button.
         * @return a Guichan mouse button.
         */
        static MouseButtonT convertMouseButton(const int button) A_WARN_UNUSED;

        /**
         * Converts an SDL event key to a key value.
         *
         * @param event an SDL event with a key to convert.
         * @return a key value.
         * @see Key
         */
        constexpr2 static int convertKeyCharacter(const SDL_Event &event)
                                                 A_WARN_UNUSED
        {
            const SDL_keysym keysym = event.key.keysym;
#ifdef USE_SDL2
            int value = keysym.scancode;
#else  // USE_SDL2

            int value = keysym.unicode;
#endif  // USE_SDL2

            PRAGMA45(GCC diagnostic push)
            PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
            switch (keysym.sym)
            {
              case SDLK_TAB:
                  value = KeyValue::TAB;
                  break;
              case SDLK_LALT:
                  value = KeyValue::LEFT_ALT;
                  break;
              case SDLK_RALT:
                  value = KeyValue::RIGHT_ALT;
                  break;
              case SDLK_LSHIFT:
                  value = KeyValue::LEFT_SHIFT;
                  break;
              case SDLK_RSHIFT:
                  value = KeyValue::RIGHT_SHIFT;
                  break;
              case SDLK_LCTRL:
                  value = KeyValue::LEFT_CONTROL;
                  break;
              case SDLK_RCTRL:
                  value = KeyValue::RIGHT_CONTROL;
                  break;
              case SDLK_BACKSPACE:
                  value = KeyValue::BACKSPACE;
                  break;
              case SDLK_PAUSE:
                  value = KeyValue::PAUSE;
                  break;
              case SDLK_SPACE:
                  // Special characters like ~ (tilde) ends up
                  // with the keysym.sym SDLK_SPACE which
                  // without this check would be lost. The check
                  // is only valid on key down events in SDL.
#ifndef USE_SDL2
                  if (event.type == SDL_KEYUP ||
                      keysym.unicode == ' ')
#endif  // USE_SDL2
                  {
                      value = KeyValue::SPACE;
                  }
                  break;
              case SDLK_ESCAPE:
#ifdef USE_SDL2
              case SDLK_AC_BACK:
#endif  // USE_SDL2
                  value = KeyValue::ESCAPE;
                  break;
              case SDLK_DELETE:
                  value = KeyValue::DELETE_;
                  break;
              case SDLK_INSERT:
                  value = KeyValue::INSERT;
                  break;
              case SDLK_HOME:
                  value = KeyValue::HOME;
                  break;
              case SDLK_END:
                  value = KeyValue::END;
                  break;
              case SDLK_PAGEUP:
                  value = KeyValue::PAGE_UP;
                  break;
              case SDLK_PRINT:
                  value = KeyValue::PRINT_SCREEN;
                  break;
              case SDLK_PAGEDOWN:
                  value = KeyValue::PAGE_DOWN;
                  break;
              case SDLK_F1:
                  value = KeyValue::F1;
                  break;
              case SDLK_F2:
                  value = KeyValue::F2;
                  break;
              case SDLK_F3:
                  value = KeyValue::F3;
                  break;
              case SDLK_F4:
                  value = KeyValue::F4;
                  break;
              case SDLK_F5:
                  value = KeyValue::F5;
                  break;
              case SDLK_F6:
                  value = KeyValue::F6;
                  break;
              case SDLK_F7:
                  value = KeyValue::F7;
                  break;
              case SDLK_F8:
                  value = KeyValue::F8;
                  break;
              case SDLK_F9:
                  value = KeyValue::F9;
                  break;
              case SDLK_F10:
                  value = KeyValue::F10;
                  break;
              case SDLK_F11:
                  value = KeyValue::F11;
                  break;
              case SDLK_F12:
                  value = KeyValue::F12;
                  break;
              case SDLK_F13:
                  value = KeyValue::F13;
                  break;
              case SDLK_F14:
                  value = KeyValue::F14;
                  break;
              case SDLK_F15:
                  value = KeyValue::F15;
                  break;
              case SDLK_NUMLOCK:
                  value = KeyValue::NUM_LOCK;
                  break;
              case SDLK_CAPSLOCK:
                  value = KeyValue::CAPS_LOCK;
                  break;
              case SDLK_SCROLLOCK:
                  value = KeyValue::SCROLL_LOCK;
                  break;
              case SDLK_RMETA:
                  value = KeyValue::RIGHT_META;
                  break;
              case SDLK_LMETA:
                  value = KeyValue::LEFT_META;
                  break;
#ifndef USE_SDL2
              case SDLK_LSUPER:
                  value = KeyValue::LEFT_SUPER;
                  break;
              case SDLK_RSUPER:
                  value = KeyValue::RIGHT_SUPER;
                  break;
#endif  // USE_SDL2

              case SDLK_MODE:
                  value = KeyValue::ALT_GR;
                  break;
              case SDLK_UP:
                  value = KeyValue::UP;
                  break;
              case SDLK_DOWN:
                  value = KeyValue::DOWN;
                  break;
              case SDLK_LEFT:
                  value = KeyValue::LEFT;
                  break;
              case SDLK_RIGHT:
                  value = KeyValue::RIGHT;
                  break;
              case SDLK_RETURN:
                  value = KeyValue::ENTER;
                  break;
              case SDLK_KP_ENTER:
                  value = KeyValue::ENTER;
                  break;

              default:
                  break;
            }
            PRAGMA45(GCC diagnostic pop)

            if (!(CAST_U32(keysym.mod) & KMOD_NUM))
            {
                PRAGMA45(GCC diagnostic push)
                PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
                switch (keysym.sym)
                {
                  case SDLK_KP0:
                      value = KeyValue::INSERT;
                      break;
                  case SDLK_KP1:
                      value = KeyValue::END;
                      break;
                  case SDLK_KP2:
                      value = KeyValue::DOWN;
                      break;
                  case SDLK_KP3:
                      value = KeyValue::PAGE_DOWN;
                      break;
                  case SDLK_KP4:
                      value = KeyValue::LEFT;
                      break;
                  case SDLK_KP5:
                      value = 0;
                      break;
                  case SDLK_KP6:
                      value = KeyValue::RIGHT;
                      break;
                  case SDLK_KP7:
                      value = KeyValue::HOME;
                      break;
                  case SDLK_KP8:
                      value = KeyValue::UP;
                      break;
                  case SDLK_KP9:
                      value = KeyValue::PAGE_UP;
                      break;
                  default:
                      break;
                }
                PRAGMA45(GCC diagnostic pop)
            }
            return value;
        }

        static void convertKeyEventToKey(const SDL_Event &event,
                                         KeyInput &keyInput);

        std::queue<KeyInput> mKeyInputQueue;
        std::queue<MouseInput> mMouseInputQueue;

        bool mMouseDown;
        bool mMouseInWindow;
};

extern SDLInput *guiInput;                  /**< GUI input */

#endif  // GUI_SDLINPUT_H
