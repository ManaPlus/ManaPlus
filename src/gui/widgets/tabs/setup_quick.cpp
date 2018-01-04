/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_quick.h"

#include "gamemodifiers.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/setupquickitem.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

Setup_Quick::Setup_Quick(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mMoveTypeList(new NamesModel),
    mCrazyMoveTypeList(new NamesModel)
{
    // TRANSLATORS: quick tab in settings
    setName(_("Quick"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupQuickItem("", this, "moveTypeEvent",
        &GameModifiers::getMoveTypeString,
        &GameModifiers::changeMoveType);

    new SetupQuickItem("", this, "crazyMoveTypeEvent",
        &GameModifiers::getCrazyMoveTypeString,
        &GameModifiers::changeCrazyMoveType);

    new SetupQuickItem("", this, "moveToTargetTypeEvent",
        &GameModifiers::getMoveToTargetTypeString,
        &GameModifiers::changeMoveToTargetType);

    new SetupQuickItem("", this, "followModeEvent",
        &GameModifiers::getFollowModeString,
        &GameModifiers::changeFollowMode);

    new SetupQuickItem("", this, "attackWeaponTypeEvent",
        &GameModifiers::getAttackWeaponTypeString,
        &GameModifiers::changeAttackWeaponType);

    new SetupQuickItem("", this, "attackTypeEvent",
        &GameModifiers::getAttackTypeString,
        &GameModifiers::changeAttackType);

    new SetupQuickItem("", this, "magicAttackTypeEvent",
        &GameModifiers::getMagicAttackTypeString,
        &GameModifiers::changeMagicAttackType);

    new SetupQuickItem("", this, "pvpAttackTypeEvent",
        &GameModifiers::getPvpAttackTypeString,
        &GameModifiers::changePvpAttackType);

    new SetupQuickItem("", this, "quickDropCounterEvent",
        &GameModifiers::getQuickDropCounterString,
        &GameModifiers::changeQuickDropCounter);

    new SetupQuickItem("", this, "pickUpTypeEvent",
        &GameModifiers::getPickUpTypeString,
        &GameModifiers::changePickUpType);

    new SetupQuickItem("", this, "mapDrawTypeEvent",
        &GameModifiers::getMapDrawTypeString,
        &GameModifiers::changeMapDrawType);

    new SetupQuickItem("", this, "imitationModeEvent",
        &GameModifiers::getImitationModeString,
        &GameModifiers::changeImitationMode);

    new SetupQuickItem("", this, "CameraModeEvent",
        &GameModifiers::getCameraModeString,
        &GameModifiers::changeCameraMode);

    new SetupQuickItem("", this, "awayModeEvent",
        &GameModifiers::getAwayModeString,
        &GameModifiers::changeAwayMode);

    new SetupQuickItem("", this, "targetingTypeEvent",
        &GameModifiers::getTargetingTypeString,
        &GameModifiers::changeTargetingType);

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Quick::~Setup_Quick()
{
    delete2(mMoveTypeList);
    delete2(mCrazyMoveTypeList);
}
