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

#include "gui/setup_other.h"
#include "gui/editdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/textfield.h"

#include "configuration.h"
#include "localplayer.h"
#include "log.h"

#include "utils/gettext.h"

#define ACTION_SHOW_TAKEDDAMAGE "taked damage"
#define ACTION_ONLY_REACHABLE "only reachable"
#define ACTION_ERRORS_IN_DEBUG "errors in debug"
#define ACTION_HIGHLIGHT_PORTALS "highlight portals"
#define ACTION_HIGHLIGHT_ATTACK_RANGE "highlight attack"
#define ACTION_HIGHLIGHT_MONSTER_ATTACK_RANGE "highlight monster attack"
#define ACTION_CYCLE_PLAYERS "cycle players"
#define ACTION_CYCLE_MONSTERS "cycle monsters"
#define ACTION_ENABLE_BOTCHECKER "bot checker"
#define ACTION_FLOORITEMS_HIGHLIGHT "floor items"
#define ACTION_MOVE_PROGRAM "move program"
#define ACTION_EDIT_PROGRAM "edit program"
#define ACTION_EDIT_PROGRAM_OK "edit program ok"
#define ACTION_AFK "move afk"
#define ACTION_EDIT_AFK "edit afk"
#define ACTION_EDIT_AFK_OK "edit afk ok"
#define ACTION_ENABLE_AFK "enable afk"
#define ACTION_TRADEBOT "trade bot"
#define ACTION_BUGGY_SERVERS "buggy servers"
#define ACTION_DEBUG_LOG "debug log"
#define ACTION_SERVER_ATTACK "server attack"
#define ACTION_FIX_POS "fix pos"
#define ACTION_ATTACK_MOVING "attack moving"
#define ACTION_QUICK_STATS "quick stats"
#define ACTION_WARP_PARTICLE "warp particle"
#define ACTION_AUTO_SHOP "auto shop"
#define ACTION_SHOW_MOB_HP "show mob hp"
#define ACTION_SHOW_OWN_HP "show own hp"
#define ACTION_SHOW_JOB_EXP "show job exp"
#define ACTION_SHOW_BEING_POPUP "show being popup"

Setup_Other::Setup_Other():
    mShowMonstersTakedDamage(config.getBoolValue("showMonstersTakedDamage")),
    mTargetOnlyReachable(config.getBoolValue("targetOnlyReachable")),
    mHighlightPortals(config.getBoolValue("highlightMapPortals")),
    mHighlightAttackRange(config.getBoolValue("highlightAttackRange")),
    mHighlightMonsterAttackRange(
        config.getBoolValue("highlightMonsterAttackRange")),
    mCyclePlayers(config.getBoolValue("cyclePlayers")),
    mCycleMonsters(config.getBoolValue("cycleMonsters")),
    mEnableBotChecker(config.getBoolValue("enableBotCheker")),
    mFloorItemsHighlight(config.getBoolValue("floorItemsHighlight")),
    mMoveProgram(config.getStringValue("crazyMoveProgram")),
    mAfk(config.getStringValue("afkMessage")),
    mTradeBot(config.getBoolValue("tradebot")),
    mBuggyServers(serverConfig.getValueBool("enableBuggyServers", true)),
    mDebugLog(config.getBoolValue("debugLog")),
    mServerAttack(config.getBoolValue("serverAttack")),
    mAutofixPos(config.getBoolValue("autofixPos")),
    mAttackMoving(config.getBoolValue("attackMoving")),
    mQuickStats(config.getBoolValue("quickStats")),
    mWarpParticle(config.getBoolValue("warpParticle")),
    mAutoShop(config.getBoolValue("autoShop")),
    mShowMobHP(config.getBoolValue("showMobHP")),
    mShowOwnHP(config.getBoolValue("showOwnHP")),
    mShowJobExp(config.getBoolValue("showJobExp")),
    mShowBeingPopup(config.getBoolValue("showBeingPopup")),
    mEditDialog(0)
{
    setName(_("Misc"));

    mShowMonstersTakedDamageCheckBox = new CheckBox(
        _("Show damage inflicted to monsters"),
        mShowMonstersTakedDamage,
        this, ACTION_SHOW_TAKEDDAMAGE);

    mTargetOnlyReachableCheckBox = new CheckBox(
        _("Auto target only reachable monsters"),
        mTargetOnlyReachable,
        this, ACTION_ONLY_REACHABLE);

    mHighlightPortalsCheckBox = new CheckBox(_("Highlight map portals"),
        mHighlightPortals,
        this, ACTION_HIGHLIGHT_PORTALS);

    mHighlightAttackRangeCheckBox = new CheckBox(
        _("Highlight player attack range"),
        mHighlightAttackRange,
        this, ACTION_HIGHLIGHT_ATTACK_RANGE);

    mHighlightMonsterAttackRangeCheckBox = new CheckBox(
        _("Highlight monster attack range"),
        mHighlightMonsterAttackRange,
        this, ACTION_HIGHLIGHT_MONSTER_ATTACK_RANGE);

    mCyclePlayersCheckBox = new CheckBox(_("Cycle player targets"),
        mCyclePlayers, this, ACTION_CYCLE_PLAYERS);

    mCycleMonstersCheckBox = new CheckBox(_("Cycle monster targets"),
        mCycleMonsters, this, ACTION_CYCLE_MONSTERS);

    mEnableBotCheckerCheckBox = new CheckBox(_("Enable bot checker"),
        mEnableBotChecker, this, ACTION_ENABLE_BOTCHECKER);

    mFloorItemsHighlightCheckBox = new CheckBox(_("Highlight floor items"),
        mFloorItemsHighlight, this, ACTION_FLOORITEMS_HIGHLIGHT);

    mMoveProgramLabel = new Label(_("Crazy move A program"));

    mMoveProgramField = new TextField(mMoveProgram, true,
                                      this, ACTION_MOVE_PROGRAM);

    mMoveProgramButton = new Button(_("Edit"), ACTION_EDIT_PROGRAM, this);

    mAfkField = new TextField(mAfk, true, this, ACTION_AFK);

    mAfkButton = new Button(_("Edit"), ACTION_EDIT_AFK, this);

    mTradeBotCheckBox = new CheckBox(_("Enable shop mode"),
                                                mTradeBot,
                                                this, ACTION_TRADEBOT);

    mBuggyServersCheckBox = new CheckBox(_("Enable buggy servers protection"),
                                                mBuggyServers,
                                                this, ACTION_BUGGY_SERVERS);

    mDebugLogCheckBox = new CheckBox(_("Enable debug log"),
                                     mDebugLog,
                                     this, ACTION_DEBUG_LOG);

    mServerAttackCheckBox = new CheckBox(_("Enable server side attack"),
                                         mServerAttack,
                                         this, ACTION_SERVER_ATTACK);

    mAutofixPosCheckBox = new CheckBox(_("Auto fix position"),
                                       mAutofixPos,
                                       this, ACTION_FIX_POS);

    mAttackMovingCheckBox = new CheckBox(_("Attack while moving"),
                                         mAttackMoving,
                                         this, ACTION_ATTACK_MOVING);

    mQuickStatsCheckBox = new CheckBox(_("Enable quick stats"),
                                       mQuickStats,
                                       this, ACTION_QUICK_STATS);

    mWarpParticleCheckBox = new CheckBox(_("Show warps particles"),
                                       mWarpParticle,
                                       this, ACTION_WARP_PARTICLE);

    mAutoShopCheckBox = new CheckBox(_("Accept sell/buy requests"),
                                       mAutoShop,
                                       this, ACTION_AUTO_SHOP);

    mShowMobHPCheckBox = new CheckBox(_("Show monster hp bar"),
                                       mShowMobHP,
                                       this, ACTION_SHOW_MOB_HP);

    mShowOwnHPCheckBox = new CheckBox(_("Show own hp bar"),
                                       mShowOwnHP,
                                       this, ACTION_SHOW_OWN_HP);

    mShowJobExpCheckBox = new CheckBox(_("Show job exp messages"),
                                       mShowJobExp,
                                       this, ACTION_SHOW_JOB_EXP);

    mShowBeingPopupCheckBox = new CheckBox(_("Show players popups"),
                                       mShowBeingPopup,
                                       this, ACTION_SHOW_BEING_POPUP);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mShowMonstersTakedDamageCheckBox, 12);
    place(12, 0, mServerAttackCheckBox, 10);
    place(0, 1, mTargetOnlyReachableCheckBox, 12);
    place(12, 1, mAutofixPosCheckBox, 10);
    place(0, 2, mHighlightPortalsCheckBox, 12);
    place(12, 2, mAttackMovingCheckBox, 10);
    place(12, 3, mQuickStatsCheckBox, 10);
    place(12, 4, mWarpParticleCheckBox, 10);
    place(12, 5, mAutoShopCheckBox, 10);
    place(12, 6, mShowMobHPCheckBox, 10);
    place(12, 7, mShowOwnHPCheckBox, 10);
    place(12, 8, mShowJobExpCheckBox, 10);
    place(12, 9, mShowBeingPopupCheckBox, 10);
    place(0, 3, mFloorItemsHighlightCheckBox, 12);
    place(0, 4, mHighlightAttackRangeCheckBox, 12);
    place(0, 5, mHighlightMonsterAttackRangeCheckBox, 12);
    place(0, 6, mCyclePlayersCheckBox, 12);
    place(0, 7, mCycleMonstersCheckBox, 12);
    place(0, 8, mEnableBotCheckerCheckBox, 12);
    place(0, 9, mMoveProgramLabel, 12);
    place(0, 10, mMoveProgramField, 9);
    place(9, 10, mMoveProgramButton, 2);
    place(0, 11, mAfkField, 9);
    place(9, 11, mAfkButton, 2);
    place(0, 12, mTradeBotCheckBox, 12);
    place(0, 13, mBuggyServersCheckBox, 12);
    place(0, 14, mDebugLogCheckBox, 12);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);

    setDimension(gcn::Rectangle(0, 0, 550, 500));
}

void Setup_Other::action(const gcn::ActionEvent &event)
{
    if (event.getId() == ACTION_SHOW_TAKEDDAMAGE)
    {
        mShowMonstersTakedDamage = mShowMonstersTakedDamageCheckBox
            ->isSelected();
    }
    else if (event.getId() == ACTION_ONLY_REACHABLE)
    {
        mTargetOnlyReachable = mTargetOnlyReachableCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_HIGHLIGHT_PORTALS)
    {
        mHighlightPortals = mHighlightPortalsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_HIGHLIGHT_ATTACK_RANGE)
    {
        mHighlightAttackRange = mHighlightAttackRangeCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_HIGHLIGHT_MONSTER_ATTACK_RANGE)
    {
        mHighlightMonsterAttackRange = mHighlightMonsterAttackRangeCheckBox
            ->isSelected();
    }
    else if (event.getId() == ACTION_CYCLE_PLAYERS)
    {
        mCyclePlayers = mCyclePlayersCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_CYCLE_MONSTERS)
    {
        mCycleMonsters = mCycleMonstersCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_ENABLE_BOTCHECKER)
    {
        mEnableBotChecker = mEnableBotCheckerCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_FLOORITEMS_HIGHLIGHT)
    {
        mFloorItemsHighlight = mFloorItemsHighlightCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_MOVE_PROGRAM)
    {
        mMoveProgram = mMoveProgramField->getText();
    }
    else if (event.getId() == ACTION_EDIT_PROGRAM)
    {
        mEditDialog =  new EditDialog("Crazy Move A",
            mMoveProgramField->getText(), ACTION_EDIT_PROGRAM_OK);
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == ACTION_EDIT_PROGRAM_OK)
    {
        mMoveProgramField->setText(mEditDialog->getMsg());
    }

    else if (event.getId() == ACTION_AFK)
        mAfk = mAfkField->getText();
    else if (event.getId() == ACTION_EDIT_AFK)
    {
        mEditDialog =  new EditDialog("Afk message", mAfkField->getText(), 
                                      ACTION_EDIT_AFK_OK);
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == ACTION_EDIT_AFK_OK)
    {
        mAfkField->setText(mEditDialog->getMsg());
    }
    else if (event.getId() == ACTION_TRADEBOT)
    {
        mTradeBot = mTradeBotCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_BUGGY_SERVERS)
    {
        mBuggyServers = mBuggyServersCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_DEBUG_LOG)
    {
        mDebugLog = mDebugLogCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SERVER_ATTACK)
    {
        mServerAttack = mServerAttackCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_FIX_POS)
    {
        mAutofixPos = mAutofixPosCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_ATTACK_MOVING)
    {
        mAttackMoving = mAttackMovingCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_QUICK_STATS)
    {
        mQuickStats = mQuickStatsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_WARP_PARTICLE)
    {
        mWarpParticle = mWarpParticleCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_AUTO_SHOP)
    {
        mAutoShop = mAutoShopCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_MOB_HP)
    {
        mShowMobHP = mShowMobHPCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_OWN_HP)
    {
        mShowOwnHP = mShowOwnHPCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_JOB_EXP)
    {
        mShowJobExp = mShowJobExpCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_BEING_POPUP)
    {
        mShowBeingPopup = mShowBeingPopupCheckBox->isSelected();
    }
}

void Setup_Other::cancel()
{
    mShowMonstersTakedDamage = config.getBoolValue(
        "showMonstersTakedDamage");
    mShowMonstersTakedDamageCheckBox->setSelected(mShowMonstersTakedDamage);

    mTargetOnlyReachable = config.getBoolValue("targetOnlyReachable");
    mTargetOnlyReachableCheckBox->setSelected(mTargetOnlyReachable);

    mHighlightPortals = config.getBoolValue("highlightMapPortals");
    mHighlightPortalsCheckBox->setSelected(mHighlightPortals);

    mHighlightAttackRange = config.getBoolValue("highlightAttackRange");
    mHighlightAttackRangeCheckBox->setSelected(mHighlightAttackRange);

    mHighlightMonsterAttackRange = config.getBoolValue(
        "highlightMonsterAttackRange");
    mHighlightMonsterAttackRangeCheckBox->setSelected(
        mHighlightMonsterAttackRange);

    mCyclePlayers = config.getBoolValue("cyclePlayers");
    mCyclePlayersCheckBox->setSelected(mCyclePlayers);

    mCycleMonsters = config.getBoolValue("cycleMonsters");
    mCycleMonstersCheckBox->setSelected(mCycleMonsters);

    mEnableBotChecker = config.getBoolValue("enableBotCheker");
    mEnableBotCheckerCheckBox->setSelected(mEnableBotChecker);

    mFloorItemsHighlight = config.getBoolValue("floorItemsHighlight");
    mFloorItemsHighlightCheckBox->setSelected(mFloorItemsHighlight);

    mMoveProgram = config.getStringValue("crazyMoveProgram");
    mMoveProgramField->setText(mMoveProgram);

    mAfk = config.getStringValue("afkMessage");
    mAfkField->setText(mAfk);

    mTradeBot = config.getBoolValue("tradebot");
    mTradeBotCheckBox->setSelected(mTradeBot);

    mBuggyServers = serverConfig.getValueBool("enableBuggyServers", true);
    mBuggyServersCheckBox->setSelected(mBuggyServers);

    mDebugLog = config.getBoolValue("debugLog");
    mDebugLogCheckBox->setSelected(mDebugLog);

    mServerAttack = config.getBoolValue("serverAttack");
    mServerAttackCheckBox->setSelected(mServerAttack);

    mAutofixPos = config.getBoolValue("autofixPos");
    mAutofixPosCheckBox->setSelected(mAutofixPos);

    mAttackMoving = config.getBoolValue("attackMoving");
    mAttackMovingCheckBox->setSelected(mAttackMoving);

    mQuickStats = config.getBoolValue("quickStats");
    mQuickStatsCheckBox->setSelected(mQuickStats);

    mWarpParticle = config.getBoolValue("warpParticle");
    mWarpParticleCheckBox->setSelected(mWarpParticle);

    mAutoShop = config.getBoolValue("autoShop");
    mAutoShopCheckBox->setSelected(mAutoShop);

    mShowMobHP = config.getBoolValue("showMobHP");
    mShowMobHPCheckBox->setSelected(mShowMobHP);

    mShowOwnHP = config.getBoolValue("showOwnHP");
    mShowOwnHPCheckBox->setSelected(mShowOwnHP);

    mShowJobExp = config.getBoolValue("showJobExp");
    mShowJobExpCheckBox->setSelected(mShowJobExp);

    mShowBeingPopup = config.getBoolValue("showBeingPopup");
    mShowBeingPopupCheckBox->setSelected(mShowBeingPopup);
}

void Setup_Other::apply()
{
    config.setValue("showMonstersTakedDamage", mShowMonstersTakedDamage);
    config.setValue("targetOnlyReachable", mTargetOnlyReachable);
    config.setValue("highlightMapPortals", mHighlightPortals);
    config.setValue("highlightAttackRange", mHighlightAttackRange);
    config.setValue("highlightMonsterAttackRange",
                    mHighlightMonsterAttackRange);
    config.setValue("cyclePlayers", mCyclePlayers);
    config.setValue("cycleMonsters", mCycleMonsters);
    config.setValue("enableBotCheker", mEnableBotChecker);
    config.setValue("floorItemsHighlight", mFloorItemsHighlight);
    config.setValue("crazyMoveProgram", mMoveProgramField->getText());
    config.setValue("afkMessage", mAfkField->getText());
    config.setValue("tradebot", mTradeBot);
    serverConfig.setValue("enableBuggyServers", mBuggyServers);
    config.setValue("debugLog", mDebugLog);
    config.setValue("serverAttack", mServerAttack);
    config.setValue("autofixPos", mAutofixPos);
    config.setValue("attackMoving", mAttackMoving);
    config.setValue("quickStats", mQuickStats);
    config.setValue("warpParticle", mWarpParticle);
    config.setValue("autoShop", mAutoShop);
    config.setValue("showMobHP", mShowMobHP);
    config.setValue("showOwnHP", mShowOwnHP);
    config.setValue("showJobExp", mShowJobExp);
    config.setValue("showBeingPopup", mShowBeingPopup);
    logger->setDebugLog(mDebugLog);
}

void Setup_Other::externalUpdated()
{
    mBuggyServers = serverConfig.getValueBool("enableBuggyServers", true);
    mBuggyServersCheckBox->setSelected(mBuggyServers);
}