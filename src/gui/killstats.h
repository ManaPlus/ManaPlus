/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

class KillStats final : public Window,
                        private gcn::ActionListener,
                        public Listener
{
    public:
        /**
         * Constructor.
         */
        KillStats();

        A_DELETE_COPY(KillStats)

        /**
         * Destructor.
         */
        ~KillStats();

        /**
         * Stuff.
         */
        void action(const gcn::ActionEvent &event) override;

        void gainXp(int Xp);

        /**
         * Recalc stats if needed
         */
        void recalcStats();

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
                          const DepricatedEvent &event) override;

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

        Label *mExpSpeed1Label;
        Label *mExpTime1Label;
        Label *mExpSpeed5Label;
        Label *mExpTime5Label;
        Label *mExpSpeed15Label;
        Label *mExpTime15Label;

        Label *mLastKillExpLabel;
        Label *mTimeBeforeJackoLabel;

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
