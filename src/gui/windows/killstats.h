/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_WINDOWS_KILLSTATS_H
#define GUI_WINDOWS_KILLSTATS_H

#include "listeners/actionlistener.h"
#include "listeners/attributelistener.h"

#include "gui/widgets/window.h"

class Label;
class Button;

class KillStats final : public Window,
                        public ActionListener,
                        public AttributeListener
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
        ~KillStats()
        { }

        /**
         * Stuff.
         */
        void action(const ActionEvent &event) override final;

        void gainXp(int64_t Xp);

        /**
         * Recalc stats if needed
         */
        void recalcStats();

        /**
         * Updates this dialog
         */
        void update();

        void resetTimes();

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;

    private:
        time_t mKillTimer;  /**< Timer for kill stats. */
        Button *mResetButton A_NONNULLPOINTER;
        Button *mTimerButton A_NONNULLPOINTER;
        Label *mLine1;
        Label *mLine2;
        Label *mLine3;
        Label *mLine4 A_NONNULLPOINTER;
        Label *mLine5 A_NONNULLPOINTER;
        Label *mLine6 A_NONNULLPOINTER;
        Label *mLine7 A_NONNULLPOINTER;

        Label *mExpSpeed1Label A_NONNULLPOINTER;
        Label *mExpTime1Label A_NONNULLPOINTER;
        Label *mExpSpeed5Label A_NONNULLPOINTER;
        Label *mExpTime5Label A_NONNULLPOINTER;
        Label *mExpSpeed15Label A_NONNULLPOINTER;
        Label *mExpTime15Label A_NONNULLPOINTER;

        Label *mLastKillExpLabel A_NONNULLPOINTER;

        int mKillCounter;   /**< Session Kill counter. */
        int64_t mExpCounter;  /**< Session Exp counter. */
        int mKillTCounter;   /**< Timer Kill counter. */
        int64_t mExpTCounter;    /**< Timer Exp counter. */

        time_t m1minExpTime;
        int64_t m1minExpNum;
        int m1minSpeed;

        time_t m5minExpTime;
        int64_t m5minExpNum;
        int m5minSpeed;

        time_t m15minExpTime;
        int64_t m15minExpNum;
        int m15minSpeed;
};

extern KillStats *killStats;

#endif  // GUI_WINDOWS_KILLSTATS_H
