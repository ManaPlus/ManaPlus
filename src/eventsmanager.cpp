/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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
#include "game.h"
#ifdef USE_SDL2
#include "render/graphics.h"
#endif
#include "logger.h"
#include "mumblemanager.h"

#include "being/localplayer.h"

#include "input/inputmanager.h"

#include "utils/process.h"

#include "debug.h"

EventsManager eventsManager;

EventsManager::EventsManager() :
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

void EventsManager::shutdown()
{
   config.removeListeners(this);
}

bool EventsManager::handleCommonEvents(const SDL_Event &event) const
{
    if (mLogInput)
        logEvent(event);

    switch (event.type)
    {
        case SDL_QUIT:
            client->setState(STATE_EXIT);
            logger->log1("force exit");
            return true;
#ifdef USE_SDL2
        case SDL_WINDOWEVENT:
            handleSDL2WindowEvent(event);
            return true;
#else
        case SDL_VIDEORESIZE:
            client->resizeVideo(event.resize.w, event.resize.h, false);
            return true;
        case SDL_ACTIVEEVENT:
            handleActive(event);
            return true;
#ifdef ANDROID
        case SDL_KEYBOARDSHOW:
            client->updateScreenKeyboard(event.user.code);
            return true;
        case SDL_ACCELEROMETER:
            break;
#endif
#endif
        default:
            break;
    }

    if (inputManager.handleEvent(event))
        return true;

    return false;
}

bool EventsManager::handleEvents() const
{
    if (Game::instance())
    {
        // Let the game handle the events while it is active
        Game::instance()->handleInput();
    }
    else
    {
        SDL_Event event;

        // Handle SDL events
#ifdef USE_SDL2
        while (SDL_WaitEventTimeout(&event, 0))
#else
        while (SDL_PollEvent(&event))
#endif
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
                            client->setState(STATE_EXIT);
                            logger->log1("exit on lost focus");
                        }
                        break;

#endif
#else
#ifndef USE_SDL2
#endif
#endif
                    default:
                        break;
                }
            }

#ifdef USE_MUMBLE
            if (player_node && mumbleManager)
            {
                mumbleManager->setPos(player_node->getTileX(),
                    player_node->getTileY(), player_node->getDirection());
            }
#endif
        }
        if (client->getState() == STATE_EXIT)
            return true;
    }
    return false;
}

void EventsManager::handleGameEvents() const
{
    Game *const game = Game::instance();

    // Events
    SDL_Event event;
#ifdef USE_SDL2
    while (SDL_WaitEventTimeout(&event, 0))
#else
    while (SDL_PollEvent(&event))
#endif
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            game->updateHistory(event);
        game->checkKeys();

        if (handleCommonEvents(event))
            break;
    }  // End while
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
            logger->log("event: SDL_FINGERDOWN: %u,%u (%f,%f) (%f,%f)",
                static_cast<unsigned int>(touch.touchId),
                static_cast<unsigned int>(touch.fingerId),
                touch.x * w, touch.y * w,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_FINGERUP:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERUP: %u,%u (%f,%f) (%f,%f)",
                static_cast<unsigned int>(touch.touchId),
                static_cast<unsigned int>(touch.fingerId),
                touch.x * w, touch.y * w,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_FINGERMOTION:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_FINGERMOTION: %u,%u (%f,%f) (%f,%f)",
                static_cast<unsigned int>(touch.touchId),
                static_cast<unsigned int>(touch.fingerId),
                touch.x * w, touch.y * h,
                touch.dx * w, touch.dy * h);
            break;
        }
        case SDL_MULTIGESTURE:
        {
            const SDL_MultiGestureEvent &gesture = event.mgesture;
            const int w = mainGraphics->mWidth;
            const int h = mainGraphics->mHeight;
            logger->log("event: SDL_MULTIGESTURE: %u %f,%f (%f,%f) %d,%d",
                static_cast<unsigned int>(gesture.touchId),
                gesture.dTheta, gesture.dDist,
                gesture.x * w, gesture.y * h,
                static_cast<int>(gesture.numFingers),
                static_cast<int>(gesture.padding));
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

#ifdef USE_SDL2
void EventsManager::handleSDL2WindowEvent(const SDL_Event &event)
{
    int fpsLimit = 0;
    const int eventType = event.window.event;
    const bool inGame = (client->getState() == STATE_GAME);
    switch (eventType)
    {
        case SDL_WINDOWEVENT_RESIZED:
            client->resizeVideo(event.window.data1, event.window.data2, false);
            break;
        case SDL_WINDOWEVENT_ENTER:
            client->setMouseFocused(true);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            client->setMouseFocused(false);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            client->setInputFocused(true);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            client->setInputFocused(false);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            client->setIsMinimized(true);
            if (inGame)
            {
                if (player_node && !player_node->getAway())
                {
                    fpsLimit = config.getIntValue("altfpslimit");
                    player_node->setHalfAway(true);
                }
            }
            setPriority(false);
            break;
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_MAXIMIZED:
            client->setIsMinimized(false);
            if (inGame)
            {
                if (player_node)
                {
                    if (!player_node->getAway())
                        fpsLimit = config.getIntValue("fpslimit");
                    player_node->setHalfAway(false);
                }
            }
            setPriority(true);
            break;
        default:
            break;
    }

    if (!inGame)
        return;

    if (eventType == SDL_WINDOWEVENT_MINIMIZED
        || eventType == SDL_WINDOWEVENT_RESTORED
        || eventType == SDL_WINDOWEVENT_MAXIMIZED)
    {
        if (player_node)
        {
            player_node->updateStatus();
            player_node->updateName();
        }
        Game::instance()->updateFrameRate(fpsLimit);
    }
}
#else
void EventsManager::handleActive(const SDL_Event &event)
{
    int fpsLimit = 0;
    const bool inGame = (client->getState() == STATE_GAME);
    if (event.active.state & SDL_APPACTIVE)
    {
        if (event.active.gain)
        {   // window restore
            client->setIsMinimized(false);
            if (inGame && player_node)
            {
                if (!player_node->getAway())
                    fpsLimit = config.getIntValue("fpslimit");
                player_node->setHalfAway(false);
            }
            setPriority(true);
        }
        else
        {   // window minimization
#ifdef ANDROID
            client->setState(STATE_EXIT);
#else
            client->setIsMinimized(true);
            if (inGame && player_node && !player_node->getAway())
            {
                fpsLimit = config.getIntValue("altfpslimit");
                player_node->setHalfAway(true);
            }
            setPriority(false);
#endif
        }
        if (inGame && player_node)
            player_node->updateStatus();
    }
    if (inGame && player_node)
        player_node->updateName();

    if (event.active.state & SDL_APPINPUTFOCUS)
        client->setInputFocused(event.active.gain);
    if (event.active.state & SDL_APPMOUSEFOCUS)
        client->setMouseFocused(event.active.gain);
    if (inGame)
        Game::instance()->updateFrameRate(fpsLimit);
}
#endif
