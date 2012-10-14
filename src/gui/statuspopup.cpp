/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
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

#include "gui/statuspopup.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/textbox.h"

#include "inputmanager.h"
#include "localplayer.h"
#include "units.h"
#include "keyboardconfig.h"

#include <guichan/font.hpp>
#include <guichan/widgets/label.hpp>

#include "debug.h"

StatusPopup::StatusPopup() :
    Popup("StatusPopup", "statuspopup.xml"),
    mMoveType(new Label),
    mCrazyMoveType(new Label),
    mMoveToTargetType(new Label),
    mFollowMode(new Label),
    mAttackType(new Label),
    mAttackWeaponType(new Label),
    mDropCounter(new Label),
    mPickUpType(new Label),
    mMapType(new Label),
    mMagicAttackType(new Label),
    mPvpAttackType(new Label),
    mDisableGameModifiers(new Label),
    mImitationMode(new Label),
    mAwayMode(new Label),
    mCameraMode(new Label)
{
    const int fontHeight = getFont()->getHeight();

    mMoveType->setPosition(0, 0);
    mCrazyMoveType->setPosition(0, fontHeight);
    mMoveToTargetType->setPosition(0, 2 * fontHeight);
    mFollowMode->setPosition(0,  3 * fontHeight);
    mAttackWeaponType->setPosition(0, 4 + 4 * fontHeight);
    mAttackType->setPosition(0, 4 + 5 * fontHeight);
    mMagicAttackType->setPosition(0, 4 + 6 * fontHeight);
    mPvpAttackType->setPosition(0, 4 + 7 * fontHeight);
    mDropCounter->setPosition(0, 8 + 8 * fontHeight);
    mPickUpType->setPosition(0, 8 + 9 * fontHeight);
    mMapType->setPosition(0, 12 + 10 * fontHeight);
    mImitationMode->setPosition(0, 16 + 11 * fontHeight);
    mAwayMode->setPosition(0, 16 + 12 * fontHeight);
    mCameraMode->setPosition(0, 16 + 13 * fontHeight);
    mDisableGameModifiers->setPosition(0, 20 + 14 * fontHeight);

    mMoveType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mCrazyMoveType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mMoveToTargetType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mFollowMode->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mAttackWeaponType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mAttackType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mMagicAttackType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mPvpAttackType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mDropCounter->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mPickUpType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mMapType->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mImitationMode->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mAwayMode->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mCameraMode->setForegroundColor(Theme::getThemeColor(Theme::POPUP));
    mDisableGameModifiers->setForegroundColor(
        Theme::getThemeColor(Theme::POPUP));

    add(mMoveType);
    add(mCrazyMoveType);
    add(mMoveToTargetType);
    add(mFollowMode);
    add(mAttackWeaponType);
    add(mAttackType);
    add(mDropCounter);
    add(mPickUpType);
    add(mMapType);
    add(mMagicAttackType);
    add(mPvpAttackType);
    add(mDisableGameModifiers);
    add(mImitationMode);
    add(mAwayMode);
    add(mCameraMode);

//    addMouseListener(this);
}

StatusPopup::~StatusPopup()
{
}

void StatusPopup::update()
{
    updateLabels();

    int maxWidth = mMoveType->getWidth();

    if (mCrazyMoveType->getWidth() > maxWidth)
        maxWidth = mCrazyMoveType->getWidth();
    if (mMoveToTargetType->getWidth() > maxWidth)
        maxWidth = mMoveToTargetType->getWidth();
    if (mFollowMode->getWidth() > maxWidth)
        maxWidth = mFollowMode->getWidth();
    if (mAttackWeaponType->getWidth() > maxWidth)
        maxWidth = mAttackWeaponType->getWidth();
    if (mAttackType->getWidth() > maxWidth)
        maxWidth = mAttackType->getWidth();
    if (mDropCounter->getWidth() > maxWidth)
        maxWidth = mDropCounter->getWidth();
    if (mPickUpType->getWidth() > maxWidth)
        maxWidth = mPickUpType->getWidth();
    if (mMapType->getWidth() > maxWidth)
        maxWidth = mMapType->getWidth();
    if (mMagicAttackType->getWidth() > maxWidth)
        maxWidth = mMagicAttackType->getWidth();
    if (mPvpAttackType->getWidth() > maxWidth)
        maxWidth = mPvpAttackType->getWidth();
    if (mDisableGameModifiers->getWidth() > maxWidth)
        maxWidth = mDisableGameModifiers->getWidth();
    if (mImitationMode->getWidth() > maxWidth)
        maxWidth = mImitationMode->getWidth();
    if (mAwayMode->getWidth() > maxWidth)
        maxWidth = mAwayMode->getWidth();
    if (mCameraMode->getWidth() > maxWidth)
        maxWidth = mCameraMode->getWidth();

    maxWidth += 2 * getPadding();
    setWidth(maxWidth);

    setHeight(mDisableGameModifiers->getY()
        + mDisableGameModifiers->getHeight() + 2 * getPadding());
}

void StatusPopup::view(const int x, const int y)
{
    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > mainGraphics->mWidth)
        posX = mainGraphics->mWidth - getWidth();
    if (posY + getHeight() > mainGraphics->mHeight)
        posY = y - getHeight() - distance;

    update();

    setPosition(posX, posY);
    setVisible(true);
    requestMoveToTop();
}

void StatusPopup::setLabelText(Label *const label, const char *const text,
                               int const key) const
{
    label->setCaption(strprintf("%s  %s", text,
        inputManager.getKeyValueString(key).c_str()));
}

void StatusPopup::setLabelText2(Label *const label,
                                const std::string &text,
                                const Input::KeyAction key) const
{
    label->setCaption(strprintf("%s  %s", text.c_str(),
        inputManager.getKeyValueString(static_cast<int>(key)).c_str()));
    label->adjustSize();
}

void StatusPopup::updateLabels()
{
    if (!player_node || !viewport)
        return;

    setLabelText2(mMoveType, player_node->getInvertDirectionString(),
        Input::KEY_INVERT_DIRECTION);
    setLabelText2(mCrazyMoveType, player_node->getCrazyMoveTypeString(),
        Input::KEY_CHANGE_CRAZY_MOVES_TYPE);
    setLabelText2(mMoveToTargetType, player_node->getMoveToTargetTypeString(),
        Input::KEY_CHANGE_MOVE_TO_TARGET);
    setLabelText2(mFollowMode, player_node->getFollowModeString(),
        Input::KEY_CHANGE_FOLLOW_MODE);
    setLabelText2(mAttackWeaponType, player_node->getAttackWeaponTypeString(),
        Input::KEY_CHANGE_ATTACK_WEAPON_TYPE);
    setLabelText2(mAttackType, player_node->getAttackTypeString(),
        Input::KEY_CHANGE_ATTACK_TYPE);
    setLabelText2(mDropCounter, player_node->getQuickDropCounterString(),
        Input::KEY_SWITCH_QUICK_DROP);
    setLabelText2(mPickUpType, player_node->getPickUpTypeString(),
        Input::KEY_CHANGE_PICKUP_TYPE);
    setLabelText2(mMapType, player_node->getDebugPathString(),
        Input::KEY_PATHFIND);
    setLabelText2(mMagicAttackType, player_node->getMagicAttackString(),
        Input::KEY_SWITCH_MAGIC_ATTACK);
    setLabelText2(mPvpAttackType, player_node->getPvpAttackString(),
        Input::KEY_SWITCH_PVP_ATTACK);
    setLabelText2(mImitationMode, player_node->getImitationModeString(),
        Input::KEY_CHANGE_IMITATION_MODE);
    setLabelText2(mAwayMode, player_node->getAwayModeString(),
        Input::KEY_AWAY);
    setLabelText2(mCameraMode, player_node->getCameraModeString(),
        Input::KEY_CAMERA);
    setLabelText2(mDisableGameModifiers, player_node->getGameModifiersString(),
        Input::KEY_DISABLE_GAME_MODIFIERS);
}
