/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GUI_Setup_Other_H
#define GUI_Setup_Other_H

#include "guichanfwd.h"

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class EditDialog;

class Setup_Other : public SetupTab, public gcn::ActionListener
{
    public:
        Setup_Other();

        void apply();
        void cancel();

        void action(const gcn::ActionEvent &event);

        virtual void externalUpdated();

    private:
        gcn::CheckBox *mShowMonstersTakedDamageCheckBox;
        bool mShowMonstersTakedDamage;

        gcn::CheckBox *mTargetOnlyReachableCheckBox;
        bool mTargetOnlyReachable;

        gcn::CheckBox *mHighlightPortalsCheckBox;
        bool mHighlightPortals;

        gcn::CheckBox *mHighlightAttackRangeCheckBox;
        bool mHighlightAttackRange;

        gcn::CheckBox *mHighlightMonsterAttackRangeCheckBox;
        bool mHighlightMonsterAttackRange;

        gcn::CheckBox *mCyclePlayersCheckBox;
        bool mCyclePlayers;

        gcn::CheckBox *mCycleMonstersCheckBox;
        bool mCycleMonsters;

        gcn::CheckBox *mEnableBotCheckerCheckBox;
        bool mEnableBotChecker;

        gcn::CheckBox *mFloorItemsHighlightCheckBox;
        bool mFloorItemsHighlight;

        gcn::Label *mMoveProgramLabel;
        gcn::TextField *mMoveProgramField;
        gcn::Button *mMoveProgramButton;
        std::string mMoveProgram;

        gcn::TextField *mAfkField;
        gcn::Button *mAfkButton;
        std::string mAfk;

        gcn::CheckBox *mTradeBotCheckBox;
        bool mTradeBot;

        gcn::CheckBox *mBuggyServersCheckBox;
        bool mBuggyServers;

        gcn::CheckBox *mDebugLogCheckBox;
        bool mDebugLog;

        gcn::CheckBox *mServerAttackCheckBox;
        bool mServerAttack;

        gcn::CheckBox *mAutofixPosCheckBox;
        bool mAutofixPos;

        gcn::CheckBox *mAttackMovingCheckBox;
        bool mAttackMoving;

        gcn::CheckBox *mQuickStatsCheckBox;
        bool mQuickStats;

        gcn::CheckBox *mWarpParticleCheckBox;
        bool mWarpParticle;

        gcn::CheckBox *mAutoShopCheckBox;
        bool mAutoShop;

        gcn::CheckBox *mShowMobHPCheckBox;
        bool mShowMobHP;

        gcn::CheckBox *mShowOwnHPCheckBox;
        bool mShowOwnHP;

        EditDialog *mEditDialog;
};

#endif
