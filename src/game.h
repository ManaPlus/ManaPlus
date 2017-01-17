/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "enums/input/inputaction.h"

#include <string>

#include "localconsts.h"

#include <SDL_events.h>

static const int MAX_LASTKEYS = 10;

extern volatile time_t cur_time;

class Map;

struct LastKey final
{
    A_DELETE_COPY(LastKey)

    LastKey() :
        time(0),
        key(InputAction::NO_VALUE),
        cnt(0)
    { }

    time_t time;
    InputActionT key;
    int cnt;
};

/**
 * The main class responsible for running the game. The game starts after you
 * have selected your character.
 */
class Game final
{
    public:
        /**
         * Constructs the game, creating all the managers, handlers, engines
         * and GUI windows that make up the game.
         */
        Game();

        A_DELETE_COPY(Game)

        /**
         * Destructor, cleans up the game.
         */
        ~Game();

        /**
         * Provides access to the game instance.
         */
        static Game *instance() A_WARN_UNUSED
        { return mInstance; }

        constexpr2 static void clearInstance() noexcept2
        { mInstance = nullptr; }

        /**
         * This method takes the game a small step further. It is called 100
         * times per second.
         */
        void logic();

        void slowLogic();

        void handleInput();

        void handleMove();

        void changeMap(const std::string &mapName);

        void updateFrameRate(int fpsLimit);

        /**
         * Returns the currently active map.
         */
        Map *getCurrentMap() const noexcept2 A_WARN_UNUSED
        { return mCurrentMap; }

        const std::string &getCurrentMapName() const noexcept2 A_WARN_UNUSED
        { return mMapName; }

        void setValidSpeed();

        void adjustPerfomance();

        void resetAdjustLevel();

        void setAdjustLevel(const int n)
        { mAdjustLevel = n; }

        static void videoResized(const int width, const int height);

        bool getValidSpeed() const noexcept2 A_WARN_UNUSED
        { return mValidSpeed; }

        static void moveInDirection(const unsigned char direction);

        static bool createScreenshot();

        static void addWatermark();

        static bool saveScreenshot(SDL_Surface *const screenshot);

        void updateHistory(const SDL_Event &event);

        void checkKeys();

    private:
        void clearKeysArray();

        Map *mCurrentMap;
        std::string mMapName;
        bool mValidSpeed;
        LastKey mLastKeys[MAX_LASTKEYS];
        time_t mNextAdjustTime;
        int mAdjustLevel;
        bool mAdjustPerfomance;
        int mLowerCounter;
        int mPing;
        time_t mTime;
        time_t mTime2;

        static Game *mInstance;
};

extern bool mStatsReUpdated;

#endif  // GAME_H
