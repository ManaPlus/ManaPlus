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

#ifndef GUI_WINDOWS_STATUSWINDOW_H
#define GUI_WINDOWS_STATUSWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/attributelistener.h"
#include "listeners/statlistener.h"

class Button;
class Label;
class ProgressBar;
class StatsPage;
class StatsPageBasic;
class TabbedArea;

/**
 * The player status dialog.
 *
 * \ingroup Interface
 */
class StatusWindow final : public Window,
                           public ActionListener,
                           public AttributeListener,
                           public StatListener
{
    public:
        /**
         * Constructor.
         */
        StatusWindow();

        A_DELETE_COPY(StatusWindow)

        ~StatusWindow();

        void setPointsNeeded(const AttributesT id,
                             const int needed);

        static void updateHPBar(ProgressBar *const bar,
                                const bool showMax = false);
        void updateMPBar(ProgressBar *bar, const bool showMax = false) const;
        static void updateJobBar(ProgressBar *const bar,
                                 const bool percent = true);
        static void updateXPBar(ProgressBar *const bar,
                                const bool percent = true);
        static void updateWeightBar(ProgressBar *const bar);
        static void updateInvSlotsBar(ProgressBar *const bar);
        static void updateMoneyBar(ProgressBar *const bar);
        static void updateArrowsBar(ProgressBar *const bar);
        void updateStatusBar(ProgressBar *const bar,
                             const bool percent = true) const;
        static void updateProgressBar(ProgressBar *const bar,
                                      const int64_t value,
                                      const int64_t max,
                                      const bool percent);
        static void updateProgressBar(ProgressBar *const bar,
                                      const AttributesT id,
                                      const bool percent = true);

        void action(const ActionEvent &event) override;

        void attributeChanged(const AttributesT id,
                              const int64_t oldVal,
                              const int64_t newVal) override final;

        void statChanged(const AttributesT id,
                         const int oldVal1,
                         const int oldVal2) override final;

        void updateLevelLabel();

    private:
        void addTabs();
        void addTab(const std::string &name);
        void addTabBasic(const std::string &name);

        static std::string translateLetter(const char *const letters);
        static std::string translateLetter2(const std::string &letters);

        STD_VECTOR<StatsPage*> mPages;

        TabbedArea *mTabs A_NONNULLPOINTER;
        Label *mLvlLabel A_NONNULLPOINTER;
        Label *mMoneyLabel A_NONNULLPOINTER;
        Label *mHpLabel A_NONNULLPOINTER;
        Label *mMpLabel A_NONNULLPOINTER;
        Label *mXpLabel A_NONNULLPOINTER;
        ProgressBar *mHpBar A_NONNULLPOINTER;
        ProgressBar *mMpBar A_NONNULLPOINTER;
        ProgressBar *mXpBar A_NONNULLPOINTER;

        Label *mJobLvlLabel;
        Label *mJobLabel;
        ProgressBar *mJobBar;

        StatsPageBasic *mBasicStatsPage;
        Button *mCopyButton;
};

extern StatusWindow *statusWindow;

#endif  // GUI_WINDOWS_STATUSWINDOW_H
