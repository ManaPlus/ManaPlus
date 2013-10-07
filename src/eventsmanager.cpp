/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "eventsmanager.h"

#include "configuration.h"
#include "logger.h"

#include "debug.h"

EventsManager eventsManager;

EventsManager::EventsManager() :
    mLogInput(false)
{
}

void EventsManager::init()
{
    mLogInput = config.getBoolValue("logInput");
    config.addListener("logInput", this);
}

bool EventsManager::handleEvent(const SDL_Event &event)
{
    if (mLogInput)
        logEvent(event);
    switch (event.type)
    {
    }

    return false;
}

void EventsManager::optionChanged(const std::string &name)
{
    if (name == "logInput")
        mLogInput = config.getBoolValue("logInput");
}

void EventsManager::logEvent(const SDL_Event &event)
{
    switch (event.type)
    {
        case SDL_MOUSEMOTION:
            logger->log("event: SDL_MOUSEMOTION: %d,%d,%d",
                event.motion.state, event.motion.x, event.motion.y);
            break;
#ifdef USE_SDL2
#define winEventLog(name, name2) \
    case name: \
        str = name2; \
        break

        case SDL_FINGERDOWN:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERDOWN: %lld,%lld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId, touch.x * w, touch.y * w,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_FINGERUP:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERUP: %lld,%lld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId, touch.x * w, touch.y * h,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_FINGERMOTION:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERMOTION: %lld,%lld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId,
                touch.x * w, touch.y * h,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_MULTIGESTURE:
        {
            const SDL_MultiGestureEvent &gesture = event.mgesture;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_MULTIGESTURE: %lld %f,%f (%f,%f) %d,%d",
                gesture.touchId, gesture.dTheta, gesture.dDist,
                gesture.x * w, gesture.y * h, (int)gesture.numFingers,
                (int)gesture.padding);
            break;
        }
        case SDL_KEYDOWN:
            logger->log("event: SDL_KEYDOWN: %d,%d", event.key.state,
                event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            logger->log("event: SDL_KEYUP: %d,%d", event.key.state,
                event.key.keysym.scancode);
            break;
        case SDL_WINDOWEVENT:
        {
            const int data1 = event.window.data1;
            const int data2 = event.window.data2;
            std::string str;
            switch (event.window.event)
            {
                winEventLog(SDL_WINDOWEVENT_NONE, "SDL_WINDOWEVENT_NONE");
                winEventLog(SDL_WINDOWEVENT_SHOWN, "SDL_WINDOWEVENT_SHOWN");
                winEventLog(SDL_WINDOWEVENT_HIDDEN, "SDL_WINDOWEVENT_HIDDEN");
                winEventLog(SDL_WINDOWEVENT_EXPOSED,
                    "SDL_WINDOWEVENT_EXPOSED");
                winEventLog(SDL_WINDOWEVENT_MOVED, "SDL_WINDOWEVENT_MOVED");
                winEventLog(SDL_WINDOWEVENT_RESIZED,
                    "SDL_WINDOWEVENT_RESIZED");
                winEventLog(SDL_WINDOWEVENT_SIZE_CHANGED,
                    "SDL_WINDOWEVENT_SIZE_CHANGED");
                winEventLog(SDL_WINDOWEVENT_MINIMIZED,
                    "SDL_WINDOWEVENT_MINIMIZED");
                winEventLog(SDL_WINDOWEVENT_MAXIMIZED,
                    "SDL_WINDOWEVENT_MAXIMIZED");
                winEventLog(SDL_WINDOWEVENT_RESTORED,
                    "SDL_WINDOWEVENT_RESTORED");
                winEventLog(SDL_WINDOWEVENT_ENTER, "SDL_WINDOWEVENT_ENTER");
                winEventLog(SDL_WINDOWEVENT_LEAVE, "SDL_WINDOWEVENT_LEAVE");
                winEventLog(SDL_WINDOWEVENT_FOCUS_GAINED,
                    "SDL_WINDOWEVENT_FOCUS_GAINED");
                winEventLog(SDL_WINDOWEVENT_FOCUS_LOST,
                    "SDL_WINDOWEVENT_FOCUS_LOST");
                winEventLog(SDL_WINDOWEVENT_CLOSE, "SDL_WINDOWEVENT_CLOSE");
                default:
                    str = "unknown";
                    break;
            }
            logger->log("event: SDL_WINDOWEVENT: %s: %d,%d",
                str.c_str(), data1, data2);
            break;
        }
        case SDL_TEXTINPUT:
        {
            const char *const text = event.text.text;
            logger->log("event: SDL_TEXTINPUT: %s", text);
            const int sz = strlen(event.text.text);
            for (int f = 0; f < sz; f ++)
                logger->log("dec: %d", text[f]);
            break;
        }
        case SDL_APP_TERMINATING:
            logger->log("SDL_APP_TERMINATING");
            break;
        case SDL_APP_LOWMEMORY:
            logger->log("SDL_APP_LOWMEMORY");
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            logger->log("SDL_APP_WILLENTERBACKGROUND");
            break;
        case SDL_APP_WILLENTERFOREGROUND:
            logger->log("SDL_APP_WILLENTERFOREGROUND");
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            logger->log("SDL_APP_DIDENTERFOREGROUND");
            break;
        case SDL_APP_DIDENTERBACKGROUND:
            logger->log("SDL_APP_DIDENTERBACKGROUND");
            break;
#else
        case SDL_KEYDOWN:
            logger->log("event: SDL_KEYDOWN: %d,%d,%d", event.key.state,
                event.key.keysym.scancode, event.key.keysym.unicode);
            break;
        case SDL_KEYUP:
            logger->log("event: SDL_KEYUP: %d,%d,%d", event.key.state,
                event.key.keysym.scancode, event.key.keysym.unicode);
            break;
        case SDL_VIDEORESIZE:
            logger->log("event: SDL_VIDEORESIZE");
            break;
        case SDL_VIDEOEXPOSE:
            logger->log("event: SDL_VIDEOEXPOSE");
            break;
        case SDL_ACTIVEEVENT:
            logger->log("event: SDL_ACTIVEEVENT: %d %d",
                event.active.state, event.active.gain);
            break;
#endif
        case SDL_MOUSEBUTTONDOWN:
            logger->log("event: SDL_MOUSEBUTTONDOWN: %d,%d,%d,%d",
                event.button.button, event.button.state,
            event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            logger->log("event: SDL_MOUSEBUTTONUP: %d,%d,%d,%d",
                event.button.button, event.button.state,
            event.button.x, event.button.y);
            break;
        case SDL_JOYAXISMOTION:
            logger->log("event: SDL_JOYAXISMOTION: %d,%d,%d",
                event.jaxis.which, event.jaxis.axis, event.jaxis.value);
            break;
        case SDL_JOYBALLMOTION:
            logger->log("event: SDL_JOYBALLMOTION: %d,%d,%d,%d",
                event.jball.which, event.jball.ball,
                event.jball.xrel, event.jball.yrel);
            break;
        case SDL_JOYHATMOTION:
            logger->log("event: SDL_JOYHATMOTION: %d,%d,%d", event.jhat.which,
                event.jhat.hat, event.jhat.value);
            break;
        case SDL_JOYBUTTONDOWN:
            logger->log("event: SDL_JOYBUTTONDOWN: %d,%d,%d",
                event.jbutton.which, event.jbutton.button,
                event.jbutton.state);
            break;
        case SDL_JOYBUTTONUP:
            logger->log("event: SDL_JOYBUTTONUP: %d,%d,%d",
                event.jbutton.which, event.jbutton.button,
                event.jbutton.state);
            break;
        case SDL_QUIT:
            logger->log("event: SDL_QUIT");
            break;
        case SDL_SYSWMEVENT:
            logger->log("event: SDL_SYSWMEVENT");
            break;
        case SDL_USEREVENT:
            logger->log("event: SDL_USEREVENT");
            break;
#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
            logger->log("event: SDL_ACCELEROMETER");
            break;
#endif
#endif
        default:
            logger->log("event: other: %d", event.type);
            break;
    };
}
