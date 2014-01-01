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
 * Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/sdlinput.h"

#include "sdlshared.h"

#include "input/inputmanager.h"

#ifdef USE_SDL2
#include "gui/gui.h"
#endif

#include <SDL_keyboard.h>
#include <SDL_timer.h>

#include <guichan/exception.hpp>

SDLInput::SDLInput() :
    mKeyInputQueue(),
    mMouseInputQueue(),
    mMouseDown(false),
    mMouseInWindow(true)
{
}

bool SDLInput::isKeyQueueEmpty()
{
    return mKeyInputQueue.empty();
}

KeyInput SDLInput::dequeueKeyInput2()
{
    if (mKeyInputQueue.empty())
    {
        throw GCN_EXCEPTION("The queue is empty.");
    }

    KeyInput keyInput = mKeyInputQueue.front();
    mKeyInputQueue.pop();

    return keyInput;
}

bool SDLInput::isMouseQueueEmpty()
{
    return mMouseInputQueue.empty();
}

gcn::MouseInput SDLInput::dequeueMouseInput()
{
    gcn::MouseInput mouseInput;

    if (mMouseInputQueue.empty())
        throw GCN_EXCEPTION("The queue is empty.");

    mouseInput = mMouseInputQueue.front();
    mMouseInputQueue.pop();

    return mouseInput;
}

MouseInput SDLInput::dequeueMouseInput2()
{
    MouseInput mouseInput;

    if (mMouseInputQueue.empty())
        throw GCN_EXCEPTION("The queue is empty.");

    mouseInput = mMouseInputQueue.front();
    mMouseInputQueue.pop();

    return mouseInput;
}

void SDLInput::pushInput(const SDL_Event &event)
{
    KeyInput keyInput;
    MouseInput mouseInput;

    switch (event.type)
    {
        case SDL_KEYDOWN:
        {
            keyInput.setType(gcn::KeyInput::PRESSED);
            convertKeyEventToKey(event, keyInput);
            mKeyInputQueue.push(keyInput);
            break;
        }

        case SDL_KEYUP:
        {
            keyInput.setType(gcn::KeyInput::RELEASED);
            convertKeyEventToKey(event, keyInput);
            mKeyInputQueue.push(keyInput);
            break;
        }

#ifdef USE_SDL2
        case SDL_TEXTINPUT:
            keyInput.setType(gcn::KeyInput::PRESSED);
            keyInput.setKey(gcn::Key(Key::TEXTINPUT));
            keyInput.setText(event.text.text);
            mKeyInputQueue.push(keyInput);
            break;

        case SDL_MOUSEWHEEL:
        {
//            const int x = event.wheel.x;
            const int y = event.wheel.y;
            if (y)
            {
                mouseInput.setX(gui->getLastMouseX());
                mouseInput.setY(gui->getLastMouseY());
#ifdef ANDROID
                mouseInput.setReal(0, 0);
#endif
                mouseInput.setButton(-1);
                if (y > 0)
                    mouseInput.setType(gcn::MouseInput::WHEEL_MOVED_UP);
                else
                    mouseInput.setType(gcn::MouseInput::WHEEL_MOVED_DOWN);
                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
            }

            break;
        }
#endif

#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
            break;
#endif
#endif

        case SDL_MOUSEBUTTONDOWN:
            mMouseDown = true;
            mouseInput.setX(event.button.x);
            mouseInput.setY(event.button.y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(event.button.x, event.button.y);
#else
            mouseInput.setReal(event.button.realx, event.button.realy);
#endif
#endif
            mouseInput.setButton(convertMouseButton(event.button.button));

#ifndef USE_SDL2
            if (event.button.button == SDL_BUTTON_WHEELDOWN)
                mouseInput.setType(gcn::MouseInput::WHEEL_MOVED_DOWN);
            else if (event.button.button == SDL_BUTTON_WHEELUP)
                mouseInput.setType(gcn::MouseInput::WHEEL_MOVED_UP);
            else
#endif
                mouseInput.setType(gcn::MouseInput::PRESSED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;

        case SDL_MOUSEBUTTONUP:
            mMouseDown = false;
            mouseInput.setX(event.button.x);
            mouseInput.setY(event.button.y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(event.button.x, event.button.y);
#else
            mouseInput.setReal(event.button.realx, event.button.realy);
#endif
#endif
            mouseInput.setButton(convertMouseButton(event.button.button));
            mouseInput.setType(gcn::MouseInput::RELEASED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;

        case SDL_MOUSEMOTION:
            mouseInput.setX(event.motion.x);
            mouseInput.setY(event.motion.y);
#ifdef ANDROID
#ifdef USE_SDL2
            mouseInput.setReal(event.motion.x, event.motion.y);
#else
            mouseInput.setReal(event.motion.realx, event.motion.realy);
#endif
#endif
            mouseInput.setButton(gcn::MouseInput::EMPTY);
            mouseInput.setType(gcn::MouseInput::MOVED);
            mouseInput.setTimeStamp(SDL_GetTicks());
            mMouseInputQueue.push(mouseInput);
            break;

#ifndef USE_SDL2
        case SDL_ACTIVEEVENT:
            /*
             * This occurs when the mouse leaves the window and the Gui-chan
             * application loses its mousefocus.
             */
            if ((event.active.state & SDL_APPMOUSEFOCUS)
                && !event.active.gain)
            {
                mMouseInWindow = false;

                if (!mMouseDown)
                {
                    mouseInput.setX(-1);
                    mouseInput.setY(-1);
                    mouseInput.setButton(gcn::MouseInput::EMPTY);
                    mouseInput.setType(gcn::MouseInput::MOVED);
                    mMouseInputQueue.push(mouseInput);
                }
            }

            if ((event.active.state & SDL_APPMOUSEFOCUS)
                && event.active.gain)
            {
                mMouseInWindow = true;
            }
            break;
#endif
        default:
            break;
    }  // end switch
}

void SDLInput::convertKeyEventToKey(const SDL_Event &event, KeyInput &keyInput)
{
    keyInput.setKey(gcn::Key(convertKeyCharacter(event)));
    keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
    keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
    keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
    keyInput.setMetaPressed(event.key.keysym.mod & KMOD_META);
#ifdef USE_SDL2
    const int code = event.key.keysym.scancode;
    keyInput.setNumericPad((code >= SDL_SCANCODE_KP_DIVIDE
        && code <= SDL_SCANCODE_KP_PERIOD) || code == SDL_SCANCODE_KP_EQUALS);
#else
    const int code = event.key.keysym.sym;
    keyInput.setNumericPad(code >= SDLK_KP0 && code <= SDLK_KP_EQUALS);
#endif
    const int actionId = inputManager.getActionByKey(event);
    if (actionId >= 0)
        keyInput.setActionId(actionId);
}

int SDLInput::convertMouseButton(const int button)
{
    switch (button)
    {
      case SDL_BUTTON_LEFT:
          return gcn::MouseInput::LEFT;
      case SDL_BUTTON_RIGHT:
          return gcn::MouseInput::RIGHT;
      case SDL_BUTTON_MIDDLE:
          return gcn::MouseInput::MIDDLE;
      default:
          // We have an unknown mouse type which is ignored.
          return button;
    }
}

int SDLInput::convertKeyCharacter(const SDL_Event &event)
{
    const SDL_keysym keysym = event.key.keysym;
#ifdef USE_SDL2
    int value = keysym.scancode;
#else
    int value = keysym.unicode;
#endif

    switch (keysym.sym)
    {
      case SDLK_TAB:
          value = Key::TAB;
          break;
      case SDLK_LALT:
          value = Key::LEFT_ALT;
          break;
      case SDLK_RALT:
          value = Key::RIGHT_ALT;
          break;
      case SDLK_LSHIFT:
          value = Key::LEFT_SHIFT;
          break;
      case SDLK_RSHIFT:
          value = Key::RIGHT_SHIFT;
          break;
      case SDLK_LCTRL:
          value = Key::LEFT_CONTROL;
          break;
      case SDLK_RCTRL:
          value = Key::RIGHT_CONTROL;
          break;
      case SDLK_BACKSPACE:
          value = Key::BACKSPACE;
          break;
      case SDLK_PAUSE:
          value = Key::PAUSE;
          break;
      case SDLK_SPACE:
          // Special characters like ~ (tilde) ends up
          // with the keysym.sym SDLK_SPACE which
          // without this check would be lost. The check
          // is only valid on key down events in SDL.
#ifndef USE_SDL2
          if (event.type == SDL_KEYUP || keysym.unicode == ' ')
#endif
          {
              value = Key::SPACE;
          }
          break;
      case SDLK_ESCAPE:
#ifdef USE_SDL2
      case SDLK_AC_BACK:
#endif
          value = Key::ESCAPE;
          break;
      case SDLK_DELETE:
          value = Key::DELETE_;
          break;
      case SDLK_INSERT:
          value = Key::INSERT;
          break;
      case SDLK_HOME:
          value = Key::HOME;
          break;
      case SDLK_END:
          value = Key::END;
          break;
      case SDLK_PAGEUP:
          value = Key::PAGE_UP;
          break;
      case SDLK_PRINT:
          value = Key::PRINT_SCREEN;
          break;
      case SDLK_PAGEDOWN:
          value = Key::PAGE_DOWN;
          break;
      case SDLK_F1:
          value = Key::F1;
          break;
      case SDLK_F2:
          value = Key::F2;
          break;
      case SDLK_F3:
          value = Key::F3;
          break;
      case SDLK_F4:
          value = Key::F4;
          break;
      case SDLK_F5:
          value = Key::F5;
          break;
      case SDLK_F6:
          value = Key::F6;
          break;
      case SDLK_F7:
          value = Key::F7;
          break;
      case SDLK_F8:
          value = Key::F8;
          break;
      case SDLK_F9:
          value = Key::F9;
          break;
      case SDLK_F10:
          value = Key::F10;
          break;
      case SDLK_F11:
          value = Key::F11;
          break;
      case SDLK_F12:
          value = Key::F12;
          break;
      case SDLK_F13:
          value = Key::F13;
          break;
      case SDLK_F14:
          value = Key::F14;
          break;
      case SDLK_F15:
          value = Key::F15;
          break;
      case SDLK_NUMLOCK:
          value = Key::NUM_LOCK;
          break;
      case SDLK_CAPSLOCK:
          value = Key::CAPS_LOCK;
          break;
      case SDLK_SCROLLOCK:
          value = Key::SCROLL_LOCK;
          break;
      case SDLK_RMETA:
          value = Key::RIGHT_META;
          break;
      case SDLK_LMETA:
          value = Key::LEFT_META;
          break;
#ifndef USE_SDL2
      case SDLK_LSUPER:
          value = Key::LEFT_SUPER;
          break;
      case SDLK_RSUPER:
          value = Key::RIGHT_SUPER;
          break;
#endif
      case SDLK_MODE:
          value = Key::ALT_GR;
          break;
      case SDLK_UP:
          value = Key::UP;
          break;
      case SDLK_DOWN:
          value = Key::DOWN;
          break;
      case SDLK_LEFT:
          value = Key::LEFT;
          break;
      case SDLK_RIGHT:
          value = Key::RIGHT;
          break;
      case SDLK_RETURN:
          value = Key::ENTER;
          break;
      case SDLK_KP_ENTER:
          value = Key::ENTER;
          break;

      default:
          break;
    }

    if (!(keysym.mod & KMOD_NUM))
    {
        switch (keysym.sym)
        {
          case SDLK_KP0:
              value = Key::INSERT;
              break;
          case SDLK_KP1:
              value = Key::END;
              break;
          case SDLK_KP2:
              value = Key::DOWN;
              break;
          case SDLK_KP3:
              value = Key::PAGE_DOWN;
              break;
          case SDLK_KP4:
              value = Key::LEFT;
              break;
          case SDLK_KP5:
              value = 0;
              break;
          case SDLK_KP6:
              value = Key::RIGHT;
              break;
          case SDLK_KP7:
              value = Key::HOME;
              break;
          case SDLK_KP8:
              value = Key::UP;
              break;
          case SDLK_KP9:
              value = Key::PAGE_UP;
              break;
          default:
              break;
        }
    }
    return value;
}

void SDLInput::simulateMouseClick(const int x, const int y,
                                  const unsigned int button)
{
    MouseInput mouseInput;
    mouseInput.setX(x);
    mouseInput.setY(y);
    mouseInput.setReal(x, y);
    mouseInput.setButton(button);
    mouseInput.setType(gcn::MouseInput::PRESSED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
    mouseInput.setType(gcn::MouseInput::RELEASED);
    mouseInput.setTimeStamp(SDL_GetTicks());
    mMouseInputQueue.push(mouseInput);
}
