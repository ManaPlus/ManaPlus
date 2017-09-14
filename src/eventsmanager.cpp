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

#include "eventsmanager.h"

#include "configuration.h"
#include "client.h"
#ifndef DYECMD
#include "game.h"
#endif  // DYECMD
#ifdef USE_MUMBLE
#include "mumblemanager.h"
#endif  // USE_MUMBLE
#include "sdlshared.h"
#include "settings.h"

#include "gui/windowmanager.h"

#include "being/localplayer.h"

#include "input/inputmanager.h"

#ifdef USE_SDL2
#include "render/graphics.h"
#else  // USE_SDL2
#include "logger.h"
#endif  // USE_SDL2

#include "utils/process.h"
#include "utils/sdlhelper.h"
#ifdef USE_X11
#include "utils/x11logger.h"
#endif  // USE_X11

#include "debug.h"

EventsManager eventsManager;

EventsManager::EventsManager() :
    ConfigListener(),
    mLogInput(false)
{
}

EventsManager::~EventsManager()
{
    CHECKLISTENERS
}

void EventsManager::init()
{
    mLogInput = config.getBoolValue("logInput");
    config.addListener("logInput", this);
}

void EventsManager::enableEvents()
{
    // disable unused SDL events
#ifndef USE_SDL2
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
#endif  // USE_SDL2

    SDL_EventState(SDL_SYSWMEVENT, mLogInput ? SDL_ENABLE : SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
}

void EventsManager::shutdown()
{
    config.removeListeners(this);
}

bool EventsManager::handleCommonEvents(const SDL_Event &event) const
{
    BLOCK_START("EventsManager::handleCommonEvents")
    if (mLogInput)
        logEvent(event);

    switch (event.type)
    {
        case SDL_QUIT:
            client->setState(State::EXIT);
            logger->log1("force exit");
            BLOCK_END("EventsManager::handleCommonEvents")
            return true;
#ifdef USE_SDL2
        case SDL_WINDOWEVENT:
            handleSDL2WindowEvent(event);
            BLOCK_END("EventsManager::handleCommonEvents")
            return true;
#else  // USE_SDL2
        case SDL_VIDEORESIZE:
            WindowManager::resizeVideo(event.resize.w,
                event.resize.h,
                false);
            BLOCK_END("EventsManager::handleCommonEvents")
            return true;
        case SDL_ACTIVEEVENT:
            handleActive(event);
            BLOCK_END("EventsManager::handleCommonEvents")
            return true;
#ifdef ANDROID
        case SDL_KEYBOARDSHOW:
            WindowManager::updateScreenKeyboard(event.user.code);
            BLOCK_END("EventsManager::handleCommonEvents")
            return true;
        case SDL_ACCELEROMETER:
            break;
#endif  // ANDROID
#endif  // USE_SDL2
        default:
            break;
    }

    if (inputManager.handleEvent(event))
    {
        BLOCK_END("EventsManager::handleCommonEvents")
        return true;
    }

    BLOCK_END("EventsManager::handleCommonEvents")
    return false;
}

bool EventsManager::handleEvents() const
{
    BLOCK_START("EventsManager::handleEvents")
#ifndef DYECMD
    if (Game::instance() != nullptr)
    {
        // Let the game handle the events while it is active
        Game::instance()->handleInput();
    }
    else
#endif  // DYECMD
    {
        SDL_Event event;
        // Handle SDL events
        while (SDL::PollEvent(&event))
        {
            if (!handleCommonEvents(event))
            {
                switch (event.type)
                {
#ifdef ANDROID
#ifndef USE_SDL2
                    case SDL_ACTIVEEVENT:
                        if ((event.active.state & SDL_APPACTIVE)
                            && !event.active.gain)
                        {
                            client->setState(State::EXIT);
                            logger->log1("exit on lost focus");
                        }
                        break;

#endif  // USE_SDL2
#endif  // ANDROID
                    default:
                        break;
                }
            }

#ifdef USE_MUMBLE
            if (localPlayer && mumbleManager)
            {
                mumbleManager->setPos(localPlayer->getTileX(),
                    localPlayer->getTileY(), localPlayer->getDirection());
            }
#endif  // USE_MUMBLE
        }
        if (client->getState() == State::EXIT)
        {
            BLOCK_END("EventsManager::handleEvents")
            return true;
        }
    }
    BLOCK_END("EventsManager::handleEvents")
    return false;
}

void EventsManager::handleGameEvents() const
{
    BLOCK_START("EventsManager::handleGameEvents")
#ifndef DYECMD
    Game *const game = Game::instance();

    // Events
    SDL_Event event;
    while (SDL::PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            game->updateHistory(event);
        game->checkKeys();

        if (handleCommonEvents(event))
            break;
    }  // End while
#endif  // DYECMD

    BLOCK_END("EventsManager::handleGameEvents")
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
#ifdef USE_SDL2
#define winEventLog(name, name2) \
    case name: \
        str = name2; \
        break

        case SDL_MOUSEMOTION:
            logger->log("event: SDL_MOUSEMOTION: %u, %d,%d, %d,%d",
                event.motion.state,
                event.motion.x,
                event.motion.y,
                event.motion.xrel,
                event.motion.yrel);
            break;
        case SDL_FINGERDOWN:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERDOWN: %u,%u (%f,%f) (%f,%f) %f",
                CAST_U32(touch.touchId),
                CAST_U32(touch.fingerId),
                touch.x * w, touch.y * w,
                touch.dx * w, touch.dy * h,
                touch.pressure);
            break;
        }
        case SDL_FINGERUP:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERUP: %u,%u (%f,%f) (%f,%f) %f",
                CAST_U32(touch.touchId),
                CAST_U32(touch.fingerId),
                touch.x * w, touch.y * w,
                touch.dx * w, touch.dy * h,
                touch.pressure);
            break;
        }
        case SDL_FINGERMOTION:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERMOTION: %u,%u (%f,%f) (%f,%f) %f",
                CAST_U32(touch.touchId),
                CAST_U32(touch.fingerId),
                touch.x * w, touch.y * h,
                touch.dx * w, touch.dy * h,
                touch.pressure);
            break;
        }
        case SDL_MULTIGESTURE:
        {
            const SDL_MultiGestureEvent &gesture = event.mgesture;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_MULTIGESTURE: %u %f,%f (%f,%f) %d",
                CAST_U32(gesture.touchId),
                gesture.dTheta, gesture.dDist,
                gesture.x * w, gesture.y * h,
                CAST_S32(gesture.numFingers));
            break;
        }
        case SDL_KEYDOWN:
            logger->log("event: SDL_KEYDOWN: %s(%d) %s(%d) %d,%d,%u",
                SDL_GetScancodeName(event.key.keysym.scancode),
                event.key.keysym.scancode,
                SDL_GetKeyName(event.key.keysym.sym),
                event.key.keysym.sym,
                event.key.state,
                event.key.repeat,
                CAST_U32(event.key.keysym.mod));
            break;
        case SDL_KEYUP:
            logger->log("event: SDL_KEYUP: %s(%d) %s(%d) %d,%d,%u",
                SDL_GetScancodeName(event.key.keysym.scancode),
                event.key.keysym.scancode,
                SDL_GetKeyName(event.key.keysym.sym),
                event.key.keysym.sym,
                event.key.state,
                event.key.repeat,
                CAST_U32(event.key.keysym.mod));
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
#if SDL_VERSION_ATLEAST(2, 0, 5)
                winEventLog(SDL_WINDOWEVENT_TAKE_FOCUS,
                    "SDL_WINDOWEVENT_TAKE_FOCUS");
                winEventLog(SDL_WINDOWEVENT_HIT_TEST,
                    "SDL_WINDOWEVENT_HIT_TEST");
#endif  // SDL_VERSION_ATLEAST(2, 0, 5)
                default:
                    str = strprintf("unknown: %d",
                        event.window.event);
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
            const size_t sz = strlen(event.text.text);
            for (size_t f = 0; f < sz; f ++)
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
        case SDL_MOUSEWHEEL:
#if SDL_VERSION_ATLEAST(2, 0, 4)
            logger->log("event: SDL_MOUSEWHEEL: %u,%u, %d,%d, %u",
                event.wheel.windowID,
                event.wheel.which,
                event.wheel.x,
                event.wheel.y,
                event.wheel.direction);
#else  // SDL_VERSION_ATLEAST(2, 0, 4)

            logger->log("event: SDL_MOUSEWHEEL: %u,%u, %d,%d",
                event.wheel.windowID,
                event.wheel.which,
                event.wheel.x,
                event.wheel.y);
#endif  // SDL_VERSION_ATLEAST(2, 0, 4)
            break;
#if SDL_VERSION_ATLEAST(2, 0, 4)
        case SDL_AUDIODEVICEADDED:
            logger->log("event: SDL_AUDIODEVICEADDED: %u,%u",
                event.adevice.which,
                event.adevice.iscapture);
            break;
        case SDL_KEYMAPCHANGED:
            logger->log("event: SDL_KEYMAPCHANGED");
            break;
#endif  // SDL_VERSION_ATLEAST(2, 0, 4)
        case SDL_MOUSEBUTTONDOWN:
            logger->log("event: SDL_MOUSEBUTTONDOWN: %u,%u,%u, %d,%d",
                event.button.which,
                CAST_U32(event.button.button),
                CAST_U32(event.button.state),
                event.button.x,
                event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            logger->log("event: SDL_MOUSEBUTTONUP: %u,%u,%u, %d,%d",
                event.button.which,
                CAST_U32(event.button.button),
                CAST_U32(event.button.state),
                event.button.x,
                event.button.y);
            break;
        case SDL_JOYDEVICEADDED:
            logger->log("event: SDL_JOYDEVICEADDED: %d",
                event.jdevice.which);
            break;
        case SDL_JOYDEVICEREMOVED:
            logger->log("event: SDL_JOYDEVICEREMOVED: %d",
                event.jdevice.which);
            break;
#else  // USE_SDL2

        case SDL_MOUSEMOTION:
            logger->log("event: SDL_MOUSEMOTION: %u,%d,%d",
                event.motion.state, event.motion.x, event.motion.y);
            break;
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
        case SDL_MOUSEBUTTONDOWN:
            logger->log("event: SDL_MOUSEBUTTONDOWN: %d,%d, %d,%d",
                event.button.button,
                event.button.state,
                event.button.x,
                event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            logger->log("event: SDL_MOUSEBUTTONUP: %d,%d,%d,%d",
                event.button.button,
                event.button.state,
                event.button.x,
                event.button.y);
            break;
#endif  // USE_SDL2

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
        {
            bool res = false;
#ifdef USE_X11
            res = X11Logger::logEvent(event);
#endif  // USE_X11

            if (res == false)
                logger->assertLog("event: SDL_SYSWMEVENT: not supported:");
            break;
        }
        case SDL_USEREVENT:
            logger->log("event: SDL_USEREVENT: %d",
                event.user.code);
            break;
#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
            logger->log("event: SDL_ACCELEROMETER");
            break;
#endif  // USE_SDL2
#endif  // ANDROID

        default:
            logger->assertLog("event: other: %u", event.type);
            break;
    };
}

#ifdef USE_SDL2
void EventsManager::handleSDL2WindowEvent(const SDL_Event &event)
{
#ifndef DYECMD
    int fpsLimit = 0;
#endif  // DYECMD

    const int eventType = event.window.event;
    const bool inGame = (client->getState() == State::GAME);
    switch (eventType)
    {
        case SDL_WINDOWEVENT_RESIZED:
            WindowManager::resizeVideo(event.window.data1,
                event.window.data2,
                false);
            break;
        case SDL_WINDOWEVENT_ENTER:
            settings.mouseFocused = true;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            settings.mouseFocused = false;
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            settings.inputFocused = KeyboardFocus::Focused;
            break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            settings.inputFocused = KeyboardFocus::Focused2;
            break;
#endif  // SDL_VERSION_ATLEAST(2, 0, 5)

        case SDL_WINDOWEVENT_FOCUS_LOST:
            settings.inputFocused = KeyboardFocus::Unfocused;
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            WindowManager::setIsMinimized(true);
#ifndef DYECMD
            fpsLimit = config.getIntValue("altfpslimit");
            if (localPlayer)
                localPlayer->setHalfAway(true);
#endif  // DYECMD

            setPriority(false);
            break;
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_MAXIMIZED:
            WindowManager::setIsMinimized(false);
#ifndef DYECMD
            fpsLimit = config.getIntValue("fpslimit");
            if (localPlayer)
                localPlayer->setHalfAway(false);
#endif  // DYECMD

            setPriority(true);
            break;
        default:
            break;
    }

    if (!inGame)
        return;

#ifndef DYECMD
    if (eventType == SDL_WINDOWEVENT_MINIMIZED
        || eventType == SDL_WINDOWEVENT_RESTORED
        || eventType == SDL_WINDOWEVENT_MAXIMIZED)
    {
        if (localPlayer)
        {
            localPlayer->updateStatus();
            localPlayer->updateName();
        }
        Game::instance()->updateFrameRate(fpsLimit);
    }
#endif  // DYECMD
}
#else  // USE_SDL2

void EventsManager::handleActive(const SDL_Event &event)
{
#ifndef DYECMD
    int fpsLimit = 0;
    const bool inGame = (client->getState() == State::GAME);
#endif  // DYECMD

    if ((event.active.state & SDL_APPACTIVE) != 0)
    {
        if (event.active.gain != 0u)
        {   // window restore
            WindowManager::setIsMinimized(false);
#ifndef DYECMD
            if (localPlayer != nullptr)
            {
                if (!settings.awayMode)
                    fpsLimit = config.getIntValue("fpslimit");
                localPlayer->setHalfAway(false);
            }
#endif  // DYECMD

            setPriority(true);
        }
        else
        {   // window minimization
#ifdef ANDROID
            client->setState(State::EXIT);
#else  // ANDROID
            WindowManager::setIsMinimized(true);
#ifndef DYECMD
            if (localPlayer != nullptr && !settings.awayMode)
            {
                fpsLimit = config.getIntValue("altfpslimit");
                localPlayer->setHalfAway(true);
            }
#endif  // DYECMD

            setPriority(false);
#endif  // ANDROID
        }
#ifndef DYECMD
        if (inGame && (localPlayer != nullptr))
            localPlayer->updateStatus();
#endif  // DYECMD
    }
#ifndef DYECMD
    if (inGame && (localPlayer != nullptr))
        localPlayer->updateName();
#endif  // DYECMD

    if ((event.active.state & SDL_APPINPUTFOCUS) != 0)
    {
        settings.inputFocused = (event.active.gain != 0u) ?
            KeyboardFocus::Focused : KeyboardFocus::Unfocused;
    }
    if ((event.active.state & SDL_APPMOUSEFOCUS) != 0)
        settings.mouseFocused = (event.active.gain != 0u);
#ifndef DYECMD
    if (inGame)
        Game::instance()->updateFrameRate(fpsLimit);
#endif  // DYECMD
}
#endif  // USE_SDL2
