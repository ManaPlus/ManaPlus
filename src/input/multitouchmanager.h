/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef INPUT_MULTITOUCHMANAGER_H
#define INPUT_MULTITOUCHMANAGER_H

#include <map>

#include "localconsts.h"

#ifdef USE_SDL2
union SDL_Event;
#endif

struct MultiTouchEvent
{
    bool active;
    float x;
    float y;
};

typedef std::map<int, MultiTouchEvent> MultiTouchEventsMap;
typedef std::map<int, MultiTouchEventsMap> MultiTouchDevicesMap;

class MultiTouchManager final
{
    public:
        MultiTouchManager();

        ~MultiTouchManager();

        A_DELETE_COPY(MultiTouchManager)

        void init();

#ifdef USE_SDL2
        void updateFinger(const SDL_Event &event, const bool active);

        void handleFingerDown(const SDL_Event &event);

        void handleFingerUp(const SDL_Event &event);

        void checkDevice(const int touchId,
                         const int fingerId);
#endif

    private:
        MultiTouchDevicesMap mEvents;
};

extern MultiTouchManager multiTouchManager;

#endif  // INPUT_MULTITOUCHMANAGER_H

