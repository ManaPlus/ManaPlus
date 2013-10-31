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

#ifndef EVENTSMANAGER_H
#define EVENTSMANAGER_H

#include "configlistener.h"

#include <string>

#include <SDL_events.h>

#include "localconsts.h"

class EventsManager final : public ConfigListener
{
    public:
        EventsManager();

        A_DELETE_COPY(EventsManager)

        virtual ~EventsManager();

        void init();

        void shutdown();

        bool handleEvents();

        bool handleCommonEvents(const SDL_Event &event);

        void handleGameEvents();

#ifdef USE_SDL2
        static void handleSDL2WindowEvent(const SDL_Event &event);
#else
        static void handleActive(const SDL_Event &event);
#endif

        void optionChanged(const std::string &name);

        static void logEvent(const SDL_Event &event);

    protected:
        bool mLogInput;
};

extern EventsManager eventsManager;

#endif  // EVENTSMANAGER_H
