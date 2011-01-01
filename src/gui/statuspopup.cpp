/*
 *  The Mana World
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
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

#include "gui/statuspopup.h"

#include "gui/gui.h"
#include "gui/palette.h"

#include "gui/widgets/layout.h"
#include "gui/widgets/textbox.h"

#include "graphics.h"
#include "localplayer.h"
#include "units.h"
#include "viewport.h"
#include "keyboardconfig.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>
#include <guichan/widgets/label.hpp>

StatusPopup::StatusPopup():
    Popup("StatusPopup")
{

    const int fontHeight = getFont()->getHeight();

    mMoveType = new gcn::Label;
    mMoveType->setPosition(getPadding(), getPadding());

    mCrazyMoveType = new gcn::Label;
    mCrazyMoveType->setPosition(getPadding(), fontHeight + getPadding());

    mMoveToTargetType = new gcn::Label;
    mMoveToTargetType->setPosition(getPadding(),
        2 * fontHeight + getPadding());

    mFollowMode = new gcn::Label;
    mFollowMode->setPosition(getPadding(),  3 * fontHeight + getPadding());

    mAttackWeaponType = new gcn::Label;
    mAttackWeaponType->setPosition(getPadding(),
        4 + 4 * fontHeight + getPadding());

    mAttackType = new gcn::Label;
    mAttackType->setPosition(getPadding(), 4 + 5 * fontHeight + getPadding());

    mMagicAttackType = new gcn::Label;
    mMagicAttackType->setPosition(getPadding(),
        4 + 6 * fontHeight + getPadding());

    mDropCounter = new gcn::Label;
    mDropCounter->setPosition(getPadding(), 8 + 7 * fontHeight + getPadding());

    mPickUpType = new gcn::Label;
    mPickUpType->setPosition(getPadding(), 8 + 8 * fontHeight + getPadding());

    mMapType = new gcn::Label;
    mMapType->setPosition(getPadding(), 12 + 9 * fontHeight + getPadding());

    mImitationMode = new gcn::Label;
    mImitationMode->setPosition(getPadding(),
        16 + 10 * fontHeight + getPadding());

    mAwayMode = new gcn::Label;
    mAwayMode->setPosition(getPadding(), 16 + 11 * fontHeight + getPadding());

    mCameraMode = new gcn::Label;
    mCameraMode->setPosition(getPadding(),
        16 + 12 * fontHeight + getPadding());

    mDisableGameModifiers = new gcn::Label;
    mDisableGameModifiers->setPosition(getPadding(),
        20 + 13 * fontHeight + getPadding());

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

    int minWidth = mMoveType->getWidth();

    if (mMoveToTargetType->getWidth() > minWidth)
        minWidth = mMoveToTargetType->getWidth();
    if (mFollowMode->getWidth() > minWidth)
        minWidth = mFollowMode->getWidth();
    if (mCrazyMoveType->getWidth() > minWidth)
        minWidth = mCrazyMoveType->getWidth();
    if (mAttackWeaponType->getWidth() > minWidth)
        minWidth = mAttackWeaponType->getWidth();
    if (mAttackType->getWidth() > minWidth)
        minWidth = mAttackType->getWidth();
    if (mDropCounter->getWidth() > minWidth)
        minWidth = mDropCounter->getWidth();
    if (mPickUpType->getWidth() > minWidth)
        minWidth = mPickUpType->getWidth();
    if (mMapType->getWidth() > minWidth)
        minWidth = mMapType->getWidth();
    if (mMagicAttackType->getWidth() > minWidth)
        minWidth = mMagicAttackType->getWidth();
    if (mDisableGameModifiers->getWidth() > minWidth)
        minWidth = mDisableGameModifiers->getWidth();
    if (mAwayMode->getWidth() > minWidth)
        minWidth = mAwayMode->getWidth();
    if (mCameraMode->getWidth() > minWidth)
        minWidth = mCameraMode->getWidth();
    if (mImitationMode->getWidth() > minWidth)
        minWidth = mImitationMode->getWidth();

    minWidth += 16 + 2 * getPadding();
    setWidth(minWidth);

    const int fontHeight = getFont()->getHeight();

    setHeight(24 + 8 + 14 * fontHeight + getPadding());
}

void StatusPopup::view(int x, int y)
{
    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > graphics->getWidth())
        posX = graphics->getWidth() - getWidth();
    if (posY + getHeight() > graphics->getHeight())
        posY = y - getHeight() - distance;

    update();

    setPosition(posX, posY);
    setVisible(true);
    requestMoveToTop();
}

void StatusPopup::updateLabels()
{
    if (!player_node || !viewport)
        return;

    switch (player_node->getInvertDirection())
    {
        case 0:
            mMoveType->setCaption("(D) default moves  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));
            break;

        case 1:
            mMoveType->setCaption("(I) invert moves  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));
            break;

        case 2:
            mMoveType->setCaption("(c) moves with some crazy moves  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));

        case 3:
            mMoveType->setCaption("(C) moves with crazy moves  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));
            break;

        case 4:
            mMoveType->setCaption("(d) double normal + crazy  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));
            break;

        default:
            mMoveType->setCaption("(?) move  "
                + keyboard.getKeyValueString(keyboard.KEY_INVERT_DIRECTION));
            break;
    }
    mMoveType->adjustSize();

    if (player_node->getCrazyMoveType() < 10)
    {
        mCrazyMoveType->setCaption(strprintf("(%d) crazy move number %d  ",
            player_node->getCrazyMoveType(), player_node->getCrazyMoveType())
            + keyboard.getKeyValueString(
            keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE));
    }
    else
    {
        switch (player_node->getCrazyMoveType())
        {
            case 10:
                mCrazyMoveType->setCaption("(a) custom crazy move  "
                    + keyboard.getKeyValueString(
                    keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE));
                break;
            default:
                mCrazyMoveType->setCaption("(?) crazy move  "
                    + keyboard.getKeyValueString(
                    keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE));
                break;
        }
    }
    mCrazyMoveType->adjustSize();

    switch (player_node->getMoveToTargetType())
    {
        case 0:
            mMoveToTargetType->setCaption("(0) default moves to target  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 1:
            mMoveToTargetType->setCaption("(1) moves to target in distance 1  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 2:
            mMoveToTargetType->setCaption("(2) moves to target in distance 3  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 3:
            mMoveToTargetType->setCaption("(3) moves to target in distance 3  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 4:
            mMoveToTargetType->setCaption("(5) moves to target in distance 5  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 5:
            mMoveToTargetType->setCaption("(7) moves to target in distance 7  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        case 6:
            mMoveToTargetType->setCaption(
                "(A) moves to target in attack range  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
        default:
            mMoveToTargetType->setCaption("(?) move to target  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_MOVE_TO_TARGET));
            break;
    }
    mMoveToTargetType->adjustSize();

    switch (player_node->getFollowMode())
    {
        case 0:
            mFollowMode->setCaption("(D) default follow  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_FOLLOW_MODE));
            break;
        case 1:
            mFollowMode->setCaption("(R) relative follow  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_FOLLOW_MODE));
            break;
        case 2:
            mFollowMode->setCaption("(M) mirror follow  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_FOLLOW_MODE));
            break;
        case 3:
            mFollowMode->setCaption("(P) pet follow  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_FOLLOW_MODE));
            break;
        default:
            mFollowMode->setCaption("(?) unknown follow  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_FOLLOW_MODE));
            break;
    }
    mFollowMode->adjustSize();

    switch (player_node->getAttackWeaponType())
    {
        case 1:
            mAttackWeaponType->setCaption("(D) default attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE));
            break;
        case 2:
            mAttackWeaponType->setCaption("(s) switch attack without shield  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE));
            break;
        case 3:
            mAttackWeaponType->setCaption("(S) switch attack with shield  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE));
            break;
        default:
            mAttackWeaponType->setCaption("(?) attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE));
            break;
    }
    mAttackWeaponType->adjustSize();

    switch (player_node->getAttackType())
    {
        case 0:
            mAttackType->setCaption("(D) default attack  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_ATTACK_TYPE));
            break;
        case 1:
            mAttackType->setCaption("(G) go and attack  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_ATTACK_TYPE));
            break;
        case 2:
            mAttackType->setCaption("(A) go, attack, pickup  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_ATTACK_TYPE));
            break;
        case 3:
            mAttackType->setCaption("(d) without auto attack  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_ATTACK_TYPE));
            break;
        default:
            mAttackType->setCaption("(?) attack  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_ATTACK_TYPE));
            break;
    }
    mAttackType->adjustSize();

    mDropCounter->setCaption(strprintf("(%d) drop counter %d  ",
        player_node->getQuickDropCounter(), player_node->getQuickDropCounter())
        + keyboard.getKeyValueString(keyboard.KEY_SWITCH_QUICK_DROP));
    mDropCounter->adjustSize();

    switch (player_node->getPickUpType())
    {
        case 0:
            mPickUpType->setCaption("(S) small pick up 1x1 cells  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 1:
            mPickUpType->setCaption("(D) default pick up 2x1 cells  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 2:
            mPickUpType->setCaption("(F) forward pick up 2x3 cells  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 3:
            mPickUpType->setCaption("(3) pick up 3x3 cells  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 4:
            mPickUpType->setCaption("(g) go and pick up in distance 4  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 5:
            mPickUpType->setCaption("(G) go and pick up in distance 8  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        case 6:
            mPickUpType->setCaption("(A) go and pick up in max distance  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
        default:
            mPickUpType->setCaption("(?) pick up  "
                + keyboard.getKeyValueString(keyboard.KEY_CHANGE_PICKUP_TYPE));
            break;
    }
    mPickUpType->adjustSize();

    switch (viewport->getDebugPath())
    {
        case 0:
            mMapType->setCaption("(N) normal map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        case 1:
            mMapType->setCaption("(D) debug map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        case 2:
            mMapType->setCaption("(u) ultra map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        case 3:
            mMapType->setCaption("(U) ultra map view 2  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        case 4:
            mMapType->setCaption("(e) empty map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        case 5:
            mMapType->setCaption("(b) black & white map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
        default:
            mMapType->setCaption("(?) map view  "
                    + keyboard.getKeyValueString(keyboard.KEY_PATHFIND));
            break;
    }
    mMapType->adjustSize();

    switch (player_node->getMagicAttackType())
    {
        case 0:
            mMagicAttackType->setCaption("(f) use #flar for magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
        case 1:
            mMagicAttackType->setCaption("(c) use #chiza for magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
        case 2:
            mMagicAttackType->setCaption("(I) use #ingrav for magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
        case 3:
            mMagicAttackType->setCaption("(F) use #frillyar for magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
        case 4:
            mMagicAttackType->setCaption("(U) use #upmarmu for magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
        default:
            mMagicAttackType->setCaption("(?) magic attack  "
                + keyboard.getKeyValueString(
                keyboard.KEY_SWITCH_MAGIC_ATTACK));
            break;
    }
    mMagicAttackType->adjustSize();

    switch (player_node->getImitationMode())
    {
        case 0:
            mImitationMode->setCaption("(D) default imitation  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_IMITATION_MODE));
            break;
        case 1:
            mImitationMode->setCaption("(O) outfits imitation  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_IMITATION_MODE));
            break;
        default:
            mImitationMode->setCaption("(?) imitation  "
                + keyboard.getKeyValueString(
                keyboard.KEY_CHANGE_IMITATION_MODE));
            break;
    }
    mImitationMode->adjustSize();

    switch (player_node->getAwayMode())
    {
        case 0:
            mAwayMode->setCaption("(O) on keyboard  "
                + keyboard.getKeyValueString(keyboard.KEY_AWAY));
            break;
        case 1:
            mAwayMode->setCaption("(A) away  "
                + keyboard.getKeyValueString(keyboard.KEY_AWAY));
            break;
        default:
            mAwayMode->setCaption("(?) away  "
                + keyboard.getKeyValueString(keyboard.KEY_AWAY));
            break;
    }
    mAwayMode->adjustSize();

    switch (viewport->getCameraMode())
    {
        case 0:
            mCameraMode->setCaption("(G) game camera mode  "
                + keyboard.getKeyValueString(keyboard.KEY_CAMERA));
            break;
        case 1:
            mCameraMode->setCaption("(F) free camera mode  "
                + keyboard.getKeyValueString(keyboard.KEY_CAMERA));
            break;
        case 2:
            mCameraMode->setCaption("(D) design camera mode  "
                + keyboard.getKeyValueString(keyboard.KEY_CAMERA));
            break;
        default:
            mCameraMode->setCaption("(?) away  "
                + keyboard.getKeyValueString(keyboard.KEY_CAMERA));
            break;
    }
    mCameraMode->adjustSize();

    if (player_node->getDisableGameModifiers())
    {
        mDisableGameModifiers->setCaption("Game modifiers are disabled  "
            + keyboard.getKeyValueString(keyboard.KEY_DISABLE_GAME_MODIFIERS));
    }
    else
    {
        mDisableGameModifiers->setCaption("Game modifiers are enabled  "
            + keyboard.getKeyValueString(keyboard.KEY_DISABLE_GAME_MODIFIERS));
    }
    mDisableGameModifiers->adjustSize();
}
