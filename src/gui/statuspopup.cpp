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

void StatusPopup::setLabelText(gcn::Label *label, char *text, int key)
{
    label->setCaption(strprintf("%s  %s", text,
        keyboard.getKeyValueString(key).c_str()));
}

void StatusPopup::updateLabels()
{
    if (!player_node || !viewport)
        return;

    switch (player_node->getInvertDirection())
    {
        case 0:
            setLabelText(mMoveType, _("(D) default moves"),
                keyboard.KEY_INVERT_DIRECTION);
            break;

        case 1:
            setLabelText(mMoveType, _("(I) invert moves"),
                keyboard.KEY_INVERT_DIRECTION);
            break;

        case 2:
            setLabelText(mMoveType, _("(c) moves with some crazy moves"),
                keyboard.KEY_INVERT_DIRECTION);
            break;

        case 3:
            setLabelText(mMoveType, _("(C) moves with crazy moves"),
                keyboard.KEY_INVERT_DIRECTION);
            break;

        case 4:
            setLabelText(mMoveType, _("(d) double normal + crazy"),
                keyboard.KEY_INVERT_DIRECTION);
            break;

        default:
            setLabelText(mMoveType, _("(?) move"),
                keyboard.KEY_INVERT_DIRECTION);
            break;
    }
    mMoveType->adjustSize();

    if (player_node->getCrazyMoveType() < 10)
    {
        mCrazyMoveType->setCaption(strprintf("(%d) crazy move number %d  %s",
            player_node->getCrazyMoveType(), player_node->getCrazyMoveType(),
            keyboard.getKeyValueString(
            keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE).c_str()));
    }
    else
    {
        switch (player_node->getCrazyMoveType())
        {
            case 10:
                setLabelText(mCrazyMoveType, _("(a) custom crazy move"),
                    keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE);
                break;
            default:
                setLabelText(mCrazyMoveType, _("(?) crazy move"),
                    keyboard.KEY_CHANGE_CRAZY_MOVES_TYPE);
                break;
        }
    }
    mCrazyMoveType->adjustSize();

    switch (player_node->getMoveToTargetType())
    {
        case 0:
            setLabelText(mMoveToTargetType, _("(0) default moves to target"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 1:
            setLabelText(mMoveToTargetType,
                _("(1) moves to target in distance 1"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 2:
            setLabelText(mMoveToTargetType,
                _("(2) moves to target in distance 2"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 3:
            setLabelText(mMoveToTargetType,
                _("(3) moves to target in distance 3"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 4:
            setLabelText(mMoveToTargetType,
                _("(5) moves to target in distance 5"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 5:
            setLabelText(mMoveToTargetType,
                _("(7) moves to target in distance 7"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        case 6:
            setLabelText(mMoveToTargetType,
                _("(A) moves to target in attack range"),
                keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
        default:
            setLabelText(mMoveToTargetType,
                _("(?) move to target"), keyboard.KEY_CHANGE_MOVE_TO_TARGET);
            break;
    }
    mMoveToTargetType->adjustSize();

    switch (player_node->getFollowMode())
    {
        case 0:
            setLabelText(mFollowMode, _("(D) default follow"),
                keyboard.KEY_CHANGE_FOLLOW_MODE);
            break;
        case 1:
            setLabelText(mFollowMode, _("(R) relative follow"),
                keyboard.KEY_CHANGE_FOLLOW_MODE);
            break;
        case 2:
            setLabelText(mFollowMode, _("(M) mirror follow"),
                keyboard.KEY_CHANGE_FOLLOW_MODE);
            break;
        case 3:
            setLabelText(mFollowMode, _("(P) pet follow"),
                keyboard.KEY_CHANGE_FOLLOW_MODE);
            break;
        default:
            setLabelText(mFollowMode, _("(?) unknown follow"),
                keyboard.KEY_CHANGE_FOLLOW_MODE);
            break;
    }
    mFollowMode->adjustSize();

    switch (player_node->getAttackWeaponType())
    {
        case 1:
            setLabelText(mAttackWeaponType, _("(D) default attack"),
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE);
            break;
        case 2:
            setLabelText(mAttackWeaponType,
                _("(s) switch attack without shield"),
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE);
            break;
        case 3:
            setLabelText(mAttackWeaponType,
                _("(S) switch attack with shield"),
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE);
            break;
        default:
            setLabelText(mAttackWeaponType, _("(?) attack"),
                keyboard.KEY_CHANGE_ATTACK_WEAPON_TYPE);
            break;
    }
    mAttackWeaponType->adjustSize();

    switch (player_node->getAttackType())
    {
        case 0:
            setLabelText(mAttackType, _("(D) default attack"),
                keyboard.KEY_CHANGE_ATTACK_TYPE);
            break;
        case 1:
            setLabelText(mAttackType, _("(G) go and attack"),
                keyboard.KEY_CHANGE_ATTACK_TYPE);
            break;
        case 2:
            setLabelText(mAttackType, _("(A) go, attack, pickup"),
                keyboard.KEY_CHANGE_ATTACK_TYPE);
            break;
        case 3:
            setLabelText(mAttackType, _("(d) without auto attack"),
                keyboard.KEY_CHANGE_ATTACK_TYPE);
            break;
        default:
            setLabelText(mAttackType, _("(?) attack"),
                keyboard.KEY_CHANGE_ATTACK_TYPE);
            break;
    }
    mAttackType->adjustSize();

    mDropCounter->setCaption(strprintf("(%d) drop counter %d  %s",
        player_node->getQuickDropCounter(), player_node->getQuickDropCounter(),
        keyboard.getKeyValueString(keyboard.KEY_SWITCH_QUICK_DROP).c_str()));
    mDropCounter->adjustSize();

    switch (player_node->getPickUpType())
    {
        case 0:
            setLabelText(mPickUpType, _("(S) small pick up 1x1 cells"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 1:
            setLabelText(mPickUpType, _("(D) default pick up 2x1 cells"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 2:
            setLabelText(mPickUpType, _("(F) forward pick up 2x3 cells"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 3:
            setLabelText(mPickUpType, _("(3) pick up 3x3 cells"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 4:
            setLabelText(mPickUpType, _("(g) go and pick up in distance 4"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 5:
            setLabelText(mPickUpType, _("(G) go and pick up in distance 8"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        case 6:
            setLabelText(mPickUpType, _("(A) go and pick up in max distance"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
        default:
            setLabelText(mPickUpType, _("(?) pick up"),
                keyboard.KEY_CHANGE_PICKUP_TYPE);
            break;
    }
    mPickUpType->adjustSize();

    switch (viewport->getDebugPath())
    {
        case 0:
            setLabelText(mMapType, _("(N) normal map view"),
                keyboard.KEY_PATHFIND);
            break;
        case 1:
            setLabelText(mMapType, _("(D) debug map view"),
                keyboard.KEY_PATHFIND);
            break;
        case 2:
            setLabelText(mMapType, _("(u) ultra map view"),
                keyboard.KEY_PATHFIND);
            break;
        case 3:
            setLabelText(mMapType, _("(U) ultra map view 2"),
                keyboard.KEY_PATHFIND);
            break;
        case 4:
            setLabelText(mMapType, _("(e) empty map view"),
                keyboard.KEY_PATHFIND);
            break;
        case 5:
            setLabelText(mMapType, _("(b) black & white map view"),
                keyboard.KEY_PATHFIND);
            break;
        default:
            setLabelText(mMapType, _("(?) map view"), keyboard.KEY_PATHFIND);
            break;
    }
    mMapType->adjustSize();

    switch (player_node->getMagicAttackType())
    {
        case 0:
            setLabelText(mMagicAttackType, _("(f) use #flar for magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
        case 1:
            setLabelText(mMagicAttackType,
                 _("(c) use #chiza for magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
        case 2:
            setLabelText(mMagicAttackType,
                _("(I) use #ingrav for magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
        case 3:
            setLabelText(mMagicAttackType,
                _("(F) use #frillyar for magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
        case 4:
            setLabelText(mMagicAttackType,
                _("(U) use #upmarmu for magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
        default:
            setLabelText(mMagicAttackType, _("(?) magic attack"),
                keyboard.KEY_SWITCH_MAGIC_ATTACK);
            break;
    }
    mMagicAttackType->adjustSize();

    switch (player_node->getImitationMode())
    {
        case 0:
            setLabelText(mImitationMode, _("(D) default imitation"),
                keyboard.KEY_CHANGE_IMITATION_MODE);
            break;
        case 1:
            setLabelText(mImitationMode, _("(O) outfits imitation"),
                keyboard.KEY_CHANGE_IMITATION_MODE);
            break;
        default:
            setLabelText(mImitationMode, _("(?) imitation"),
                keyboard.KEY_CHANGE_IMITATION_MODE);
            break;
    }
    mImitationMode->adjustSize();

    switch ((int)player_node->getAwayMode())
    {
        case 0:
            setLabelText(mAwayMode, _("(O) on keyboard"), keyboard.KEY_AWAY);
            break;
        case 1:
            setLabelText(mAwayMode, _("(A) away"), keyboard.KEY_AWAY);
            break;
        default:
            setLabelText(mAwayMode, _("(?) away"), keyboard.KEY_AWAY);
            break;
    }
    mAwayMode->adjustSize();

    switch (viewport->getCameraMode())
    {
        case 0:
            setLabelText(mCameraMode, _("(G) game camera mode"),
                keyboard.KEY_CAMERA);
            break;
        case 1:
            setLabelText(mCameraMode, _("(F) free camera mode"),
                keyboard.KEY_CAMERA);
            break;
        case 2:
            setLabelText(mCameraMode, _("(D) design camera mode"),
                keyboard.KEY_CAMERA);
            break;
        default:
            setLabelText(mCameraMode, _("(?) away"), keyboard.KEY_CAMERA);
            break;
    }
    mCameraMode->adjustSize();

    if (player_node->getDisableGameModifiers())
    {
        setLabelText(mDisableGameModifiers, _("Game modifiers are disabled"),
            keyboard.KEY_DISABLE_GAME_MODIFIERS);
    }
    else
    {
        setLabelText(mDisableGameModifiers, _("Game modifiers are enabled"),
            keyboard.KEY_DISABLE_GAME_MODIFIERS);
    }
    mDisableGameModifiers->adjustSize();
}
