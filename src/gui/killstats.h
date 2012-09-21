/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#ifndef KILLSTATS_H
#define KILLSTATS_H

#include <guichan/actionlistener.hpp>

#include "listener.h"

#include "gui/widgets/window.h"

class Label;
class Button;

namespace gcn
{
    class Label;
}

class KillStats final : public Window,
                        private gcn::ActionListener,
                        public Listener
{
    public:
        /**
         * Constructor.
         */
        KillStats();

        /**
         * Destructor.
         */
        ~KillStats();

        /**
         * Stuff.
         */
        void action(const gcn::ActionEvent &event);

        void gainXp(int Xp);

        /**
         * Recalc stats if needed
         */
        void recalcStats();

        /**
         * Draw this window
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Updates this dialog
         */
        void update();

        /**
         * Updates jacko info
         */
        void updateJackoLabel();

        void jackoDead(const int id);

        void jackoAlive(const int id);

        void processEvent(Channels channel A_UNUSED,
                          const DepricatedEvent &event);

        void resetTimes();

    private:
        void validateJacko();

        int mKillCounter;   /**< Session Kill counter. */
        int mExpCounter;    /**< Session Exp counter. */
        int mKillTCounter;   /**< Timer Kill counter. */
        int mExpTCounter;    /**< Timer Exp counter. */
        time_t mKillTimer;  /**< Timer for kill stats. */
        Button *mResetButton;
        Button *mTimerButton;
        Label *mLine1;
        Label *mLine2;
        Label *mLine3;
        Label *mLine4;
        Label *mLine5;
        Label *mLine6;
        Label *mLine7;

        gcn::Label *mExpSpeed1Label;
        gcn::Label *mExpTime1Label;
        gcn::Label *mExpSpeed5Label;
        gcn::Label *mExpTime5Label;
        gcn::Label *mExpSpeed15Label;
        gcn::Label *mExpTime15Label;

        gcn::Label *mLastKillExpLabel;
        gcn::Label *mTimeBeforeJackoLabel;

        int m1minExpTime;
        int m1minExpNum;
        int m1minSpeed;

        int m5minExpTime;
        int m5minExpNum;
        int m5minSpeed;

        int m15minExpTime;
        int m15minExpNum;
        int m15minSpeed;

        int mJackoSpawnTime;
        int mValidateJackoTime;
        int mJackoId;
        bool mIsJackoAlive;
        bool mIsJackoMustSpawn;
        bool mIsJackoSpawnTimeUnknown;
};

extern KillStats *killStats;

#endif
