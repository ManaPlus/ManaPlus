/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef GAME_H
#define GAME_H

#include <string>
#include "gui/sdlinput.h"

#define MAX_LASTKEYS 10

extern volatile int cur_time;
//extern std::string map_path;  // TODO: Get rid of this global

class Map;
class WindowMenu;

struct LastKey
{
    int key;
    int time;
    int cnt;
};

/**
 * The main class responsible for running the game. The game starts after you
 * have selected your character.
 */
class Game
{
    public:
        /**
         * Constructs the game, creating all the managers, handlers, engines
         * and GUI windows that make up the game.
         */
        Game();

        /**
         * Destructor, cleans up the game.
         */
        ~Game();

        /**
         * Provides access to the game instance.
         */
        static Game *instance()
        { return mInstance; }

        static void clearInstance()
        { mInstance = 0; }

        /**
         * This method takes the game a small step further. It is called 100
         * times per second.
         */
        void logic();

        void handleInput();

        void changeMap(const std::string &mapName);

        /**
         * Returns the currently active map.
         */
        Map *getCurrentMap()
        { return mCurrentMap; }

        const std::string &getCurrentMapName() const
        { return mMapName; }

        void setValidSpeed();

        void adjustPerfomance();

        void resetAdjustLevel();

        void setAdjustLevel(int n)
        { mAdjustLevel = n; }

    private:

        void updateHistory(SDL_Event &event);

        void checkKeys();

        void clearKeysArray();


        int mLastTarget;

//        WindowMenu *mWindowMenu;

        Map *mCurrentMap;
        std::string mMapName;
        bool mValidSpeed;
        int mLastAction;
        LastKey mLastKeys[MAX_LASTKEYS];
        unsigned mNextAdjustTime;
        int mAdjustLevel;
        bool mAdjustPerfomance;
        int mLowerCounter;

        static Game *mInstance;
};

#endif
