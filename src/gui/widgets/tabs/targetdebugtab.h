/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_TARGETDEBUGTAB_H
#define GUI_WIDGETS_TABS_TARGETDEBUGTAB_H

#include "gui/widgets/tabs/debugtab.h"

class Label;

class TargetDebugTab final : public DebugTab
{
    friend class DebugWindow;

    public:
        explicit TargetDebugTab(const Widget2 *const widget);

        A_DELETE_COPY(TargetDebugTab)

        void logic() override final;

    private:
        Label *mTargetLabel A_NONNULLPOINTER;
        Label *mTargetIdLabel A_NONNULLPOINTER;
        Label *mTargetTypeLabel A_NONNULLPOINTER;
        Label *mTargetLevelLabel A_NONNULLPOINTER;
        Label *mTargetRaceLabel A_NONNULLPOINTER;
        Label *mTargetPartyLabel A_NONNULLPOINTER;
        Label *mTargetGuildLabel A_NONNULLPOINTER;
        Label *mAttackDelayLabel A_NONNULLPOINTER;
        Label *mMinHitLabel A_NONNULLPOINTER;
        Label *mMaxHitLabel A_NONNULLPOINTER;
        Label *mCriticalHitLabel A_NONNULLPOINTER;
        Label *mKarmaLabel A_NONNULLPOINTER;
        Label *mMannerLabel A_NONNULLPOINTER;
        Label *mEffectsLabel A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_TABS_TARGETDEBUGTAB_H
