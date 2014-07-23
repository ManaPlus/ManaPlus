/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gamemodifiers.h"

#include "configuration.h"
#include "settings.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "gui/dialogtype.h"
#include "gui/viewport.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "listeners/awaylistener.h"
#include "listeners/updatestatuslistener.h"

#include "utils/gettext.h"

#include "debug.h"

GameModifiers *modifiers = nullptr;

GameModifiers::GameModifiers()
{
    settings.crazyMoveType = config.getIntValue("crazyMoveType");
    settings.moveToTargetType = config.getIntValue("moveToTargetType");
    settings.followMode = config.getIntValue("followMode");
    settings.attackWeaponType = config.getIntValue("attackWeaponType");
    settings.attackType = config.getIntValue("attackType");
    settings.quickDropCounter = config.getIntValue("quickDropCounter");
    settings.pickUpType = config.getIntValue("pickUpType");
    settings.magicAttackType = config.getIntValue("magicAttackType");
    settings.pvpAttackType = config.getIntValue("pvpAttackType");
    settings.imitationMode = config.getIntValue("imitationMode");
    settings.disableGameModifiers = config.getBoolValue(
        "disableGameModifiers");
}

GameModifiers::~GameModifiers()
{
}

void GameModifiers::changeMode(unsigned *restrict const var,
                               const unsigned limit,
                               const char *restrict const conf,
                               std::string (GameModifiers::*const func)(),
                               const unsigned def,
                               const bool save,
                               const bool forward)
{
    if (!var)
        return;

    if (forward)
    {
        (*var) ++;
        if (*var >= limit)
            *var = def;
    }
    else
    {
        if (!*var)
            *var = limit - 1;
        else
            (*var) --;
    }

    if (save)
        config.setValue(conf, *var);
    UpdateStatusListener::distributeEvent();
    const std::string str = (this->*func)();
    if (str.size() > 4)
        debugMsg(str.substr(4));
}

const char *GameModifiers::getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz)
{
    if (index < sz)
        return arr[index];
    return arr[sz];
}

static const unsigned mMoveTypeSize = 5;

void GameModifiers::changeMoveType(const bool forward)
{
    player_node->setMoveState(0);
    changeMode(&settings.moveType, mMoveTypeSize, "invertMoveDirection",
        &GameModifiers::getMoveTypeString, 0, false, forward);
}

static const char *const mMoveTypeStrings[] =
{
    // TRANSLATORS: move type in status bar
    N_("(D) default moves"),
    // TRANSLATORS: move type in status bar
    N_("(I) invert moves"),
    // TRANSLATORS: move type in status bar
    N_("(c) moves with some crazy moves"),
    // TRANSLATORS: move type in status bar
    N_("(C) moves with crazy moves"),
    // TRANSLATORS: move type in status bar
    N_("(d) double normal + crazy"),
    // TRANSLATORS: move type in status bar
    N_("(?) unknown move")
};

std::string GameModifiers::getMoveTypeString()
{
    return gettext(getVarItem(&mMoveTypeStrings[0],
        settings.moveType, mMoveTypeSize));
}

static const unsigned mCrazyMoveTypeSize = 11;

void GameModifiers::changeCrazyMoveType(const bool forward)
{
    player_node->setCrazyMoveState(0U);
    changeMode(&settings.crazyMoveType, mCrazyMoveTypeSize, "crazyMoveType",
        &GameModifiers::getCrazyMoveTypeString, 1, true, forward);
}

std::string GameModifiers::getCrazyMoveTypeString()
{
    const unsigned int crazyMoveType = settings.crazyMoveType;
    if (crazyMoveType < mCrazyMoveTypeSize - 1)
    {
        // TRANSLATORS: crazy move type in status bar
        return strprintf(_("(%u) crazy move number %u"),
            crazyMoveType, crazyMoveType);
    }
    else if (crazyMoveType == mCrazyMoveTypeSize - 1)
    {
        // TRANSLATORS: crazy move type in status bar
        return _("(a) custom crazy move");
    }
    else
    {
        // TRANSLATORS: crazy move type in status bar
        return _("(?) crazy move");
    }
}

static const unsigned mMoveToTargetTypeSize = 13;

void GameModifiers::changeMoveToTargetType(const bool forward)
{
    changeMode(&settings.moveToTargetType, mMoveToTargetTypeSize,
        "moveToTargetType",
        &GameModifiers::getMoveToTargetTypeString, 0, true, forward);
}

static const char *const mMoveToTargetTypeStrings[] =
{
    // TRANSLATORS: move to target type in status bar
    N_("(0) default moves to target"),
    // TRANSLATORS: move to target type in status bar
    N_("(1) moves to target in distance 1"),
    // TRANSLATORS: move to target type in status bar
    N_("(2) moves to target in distance 2"),
    // TRANSLATORS: move to target type in status bar
    N_("(3) moves to target in distance 3"),
    // TRANSLATORS: move to target type in status bar
    N_("(4) moves to target in distance 4"),
    // TRANSLATORS: move to target type in status bar
    N_("(5) moves to target in distance 5"),
    // TRANSLATORS: move to target type in status bar
    N_("(6) moves to target in distance 6"),
    // TRANSLATORS: move to target type in status bar
    N_("(7) moves to target in distance 7"),
    // TRANSLATORS: move to target type in status bar
    N_("(8) moves to target in distance 8"),
    // TRANSLATORS: move to target type in status bar
    N_("(9) moves to target in distance 9"),
    // TRANSLATORS: move to target type in status bar
    N_("(A) moves to target in attack range"),
    // TRANSLATORS: move to target type in status bar
    N_("(a) archer attack range"),
    // TRANSLATORS: move to target type in status bar
    N_("(B) moves to target in attack range - 1"),
    // TRANSLATORS: move to target type in status bar
    N_("(?) move to target")
};

std::string GameModifiers::getMoveToTargetTypeString()
{
    return gettext(getVarItem(&mMoveToTargetTypeStrings[0],
        settings.moveToTargetType, mMoveToTargetTypeSize));
}

static const unsigned mFollowModeSize = 4;

void GameModifiers::changeFollowMode(const bool forward)
{
    changeMode(&settings.followMode, mFollowModeSize, "followMode",
        &GameModifiers::getFollowModeString, 0, true, forward);
}

static const char *const mFollowModeStrings[] =
{
    // TRANSLATORS: folow mode in status bar
    N_("(D) default follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(R) relative follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(M) mirror follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(P) pet follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(?) unknown follow")
};

std::string GameModifiers::getFollowModeString()
{
    return gettext(getVarItem(&mFollowModeStrings[0],
        settings.followMode, mFollowModeSize));
}

const unsigned mAttackWeaponTypeSize = 4;

void GameModifiers::changeAttackWeaponType(const bool forward)
{
    changeMode(&settings.attackWeaponType, mAttackWeaponTypeSize,
        "attackWeaponType",
        &GameModifiers::getAttackWeaponTypeString, 1, true, forward);
}

static const char *const mAttackWeaponTypeStrings[] =
{
    // TRANSLATORS: switch attack type in status bar
    N_("(?) attack"),
    // TRANSLATORS: switch attack type in status bar
    N_("(D) default attack"),
    // TRANSLATORS: switch attack type in status bar
    N_("(s) switch attack without shield"),
    // TRANSLATORS: switch attack type in status bar
    N_("(S) switch attack with shield"),
    // TRANSLATORS: switch attack type in status bar
    N_("(?) attack")
};

std::string GameModifiers::getAttackWeaponTypeString()
{
    return gettext(getVarItem(&mAttackWeaponTypeStrings[0],
        settings.attackWeaponType, mAttackWeaponTypeSize));
}

const unsigned mAttackTypeSize = 4;

void GameModifiers::changeAttackType(const bool forward)
{
    changeMode(&settings.attackType, mAttackTypeSize, "attackType",
        &GameModifiers::getAttackTypeString, 0, true, forward);
}

static const char *const mAttackTypeStrings[] =
{
    // TRANSLATORS: attack type in status bar
    N_("(D) default attack"),
    // TRANSLATORS: attack type in status bar
    N_("(G) go and attack"),
    // TRANSLATORS: attack type in status bar
    N_("(A) go, attack, pickup"),
    // TRANSLATORS: attack type in status bar
    N_("(d) without auto attack"),
    // TRANSLATORS: attack type in status bar
    N_("(?) attack")
};

std::string GameModifiers::getAttackTypeString()
{
    return gettext(getVarItem(&mAttackTypeStrings[0],
        settings.attackType, mAttackTypeSize));
}

const unsigned mQuickDropCounterSize = 31;

void GameModifiers::changeQuickDropCounter(const bool forward)
{
    changeMode(&settings.quickDropCounter, mQuickDropCounterSize,
        "quickDropCounter",
        &GameModifiers::getQuickDropCounterString, 1, true, forward);
}

std::string GameModifiers::getQuickDropCounterString()
{
    const unsigned int cnt = settings.quickDropCounter;
    if (cnt > 9)
    {
        return strprintf("(%c) drop counter %u", static_cast<signed char>(
            'a' + cnt - 10), cnt);
    }
    else
    {
        return strprintf("(%u) drop counter %u", cnt, cnt);
    }
}

void GameModifiers::setQuickDropCounter(const int n)
{
    if (n < 1 || n >= static_cast<signed>(mQuickDropCounterSize))
        return;
    settings.quickDropCounter = n;
    config.setValue("quickDropCounter", n);
    UpdateStatusListener::distributeEvent();
}

const unsigned mPickUpTypeSize = 7;

void GameModifiers::changePickUpType(const bool forward)
{
    changeMode(&settings.pickUpType, mPickUpTypeSize, "pickUpType",
        &GameModifiers::getPickUpTypeString, 0, true, forward);
}

static const char *const mPickUpTypeStrings[] =
{
    // TRANSLATORS: pickup size in status bar
    N_("(S) small pick up 1x1 cells"),
    // TRANSLATORS: pickup size in status bar
    N_("(D) default pick up 2x1 cells"),
    // TRANSLATORS: pickup size in status bar
    N_("(F) forward pick up 2x3 cells"),
    // TRANSLATORS: pickup size in status bar
    N_("(3) pick up 3x3 cells"),
    // TRANSLATORS: pickup size in status bar
    N_("(g) go and pick up in distance 4"),
    // TRANSLATORS: pickup size in status bar
    N_("(G) go and pick up in distance 8"),
    // TRANSLATORS: pickup size in status bar
    N_("(A) go and pick up in max distance"),
    // TRANSLATORS: pickup size in status bar
    N_("(?) pick up")
};

std::string GameModifiers::getPickUpTypeString()
{
    return gettext(getVarItem(&mPickUpTypeStrings[0],
        settings.pickUpType, mPickUpTypeSize));
}

const unsigned mMagicAttackSize = 5;

void GameModifiers::changeMagicAttackType(const bool forward)
{
    changeMode(&settings.magicAttackType, mMagicAttackSize, "magicAttackType",
        &GameModifiers::getMagicAttackString, 0, true, forward);
}

static const char *const mMagicAttackStrings[] =
{
    // TRANSLATORS: magic attack in status bar
    N_("(f) use #flar for magic attack"),
    // TRANSLATORS: magic attack in status bar
    N_("(c) use #chiza for magic attack"),
    // TRANSLATORS: magic attack in status bar
    N_("(I) use #ingrav for magic attack"),
    // TRANSLATORS: magic attack in status bar
    N_("(F) use #frillyar for magic attack"),
    // TRANSLATORS: magic attack in status bar
    N_("(U) use #upmarmu for magic attack"),
    // TRANSLATORS: magic attack in status bar
    N_("(?) magic attack")
};

std::string GameModifiers::getMagicAttackString()
{
    return gettext(getVarItem(&mMagicAttackStrings[0],
        settings.magicAttackType, mMagicAttackSize));
}

const unsigned mPvpAttackTypeSize = 4;

void GameModifiers::changePvpAttackType(const bool forward)
{
    changeMode(&settings.pvpAttackType, mPvpAttackTypeSize, "pvpAttackType",
        &GameModifiers::getPvpAttackTypeString, 0, true, forward);
}

static const char *const mPvpAttackTypeStrings[] =
{
    // TRANSLATORS: player attack type in status bar
    N_("(a) attack all players"),
    // TRANSLATORS: player attack type in status bar
    N_("(f) attack all except friends"),
    // TRANSLATORS: player attack type in status bar
    N_("(b) attack bad relations"),
    // TRANSLATORS: player attack type in status bar
    N_("(d) don't attack players"),
    // TRANSLATORS: player attack type in status bar
    N_("(?) pvp attack")
};

std::string GameModifiers::getPvpAttackTypeString()
{
    return gettext(getVarItem(&mPvpAttackTypeStrings[0],
        settings.pvpAttackType, mPvpAttackTypeSize));
}

const unsigned mImitationModeSize = 2;

void GameModifiers::changeImitationMode(const bool forward)
{
    changeMode(&settings.imitationMode, mImitationModeSize, "imitationMode",
        &GameModifiers::getImitationModeString, 0, true, forward);
}

static const char *const mImitationModeStrings[] =
{
    // TRANSLATORS: imitation type in status bar
    N_("(D) default imitation"),
    // TRANSLATORS: imitation type in status bar
    N_("(O) outfits imitation"),
    // TRANSLATORS: imitation type in status bar
    N_("(?) imitation")
};

std::string GameModifiers::getImitationModeString()
{
    return gettext(getVarItem(&mImitationModeStrings[0],
        settings.imitationMode, mImitationModeSize));
}

const unsigned mGameModifiersSize = 2;

void GameModifiers::changeGameModifiers()
{
    settings.disableGameModifiers = !settings.disableGameModifiers;
    config.setValue("disableGameModifiers", settings.disableGameModifiers);
    UpdateStatusListener::distributeEvent();
}

static const char *const mGameModifiersStrings[] =
{
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are enabled"),
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are disabled"),
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are unknown")
};

std::string GameModifiers::getGameModifiersString()
{
    return gettext(getVarItem(&mGameModifiersStrings[0],
        settings.disableGameModifiers, mGameModifiersSize));
}

const unsigned mMapDrawTypeSize = 7;

static const char *const mMapDrawTypeStrings[] =
{
    // TRANSLATORS: map view type in status bar
    N_("(N) normal map view"),
    // TRANSLATORS: map view type in status bar
    N_("(D) debug map view"),
    // TRANSLATORS: map view type in status bar
    N_("(u) ultra map view"),
    // TRANSLATORS: map view type in status bar
    N_("(U) ultra map view 2"),
    // TRANSLATORS: map view type in status bar
    N_("(e) empty map view with collision"),
    // TRANSLATORS: map view type in status bar
    N_("(E) empty map view"),
    // TRANSLATORS: map view type in status bar
    N_("(b) black & white map view"),
    // TRANSLATORS: pickup size in status bar
    N_("(?) map view")
};

std::string GameModifiers::getMapDrawTypeString()
{
    return gettext(getVarItem(&mMapDrawTypeStrings[0],
        settings.mapDrawType, mMapDrawTypeSize));
}

const unsigned mAwayModeSize = 2;

void GameModifiers::changeAwayMode()
{
    if (!player_node)
        return;

    settings.awayMode = !settings.awayMode;
    player_node->setAfkTime(0);
    player_node->setHalfAway(false);
    player_node->updateName();
    UpdateStatusListener::distributeEvent();
    if (settings.awayMode)
    {
        if (chatWindow)
            chatWindow->clearAwayLog();

        player_node->cancelFollow();
        player_node->navigateClean();
        if (outfitWindow)
            outfitWindow->wearAwayOutfit();
        // TRANSLATORS: away message box header
        OkDialog *const dialog = new OkDialog(_("Away"),
            config.getStringValue("afkMessage"),
            DialogType::SILENCE, true, false);
        player_node->setAwayDialog(dialog);
        dialog->addActionListener(player_node->getAwayListener());
        soundManager.volumeOff();
        player_node->addAfkEffect();
    }
    else
    {
        player_node->setAwayDialog(nullptr);
        soundManager.volumeRestore();
        if (chatWindow)
        {
            chatWindow->displayAwayLog();
            chatWindow->clearAwayLog();
        }
        player_node->removeAfkEffect();
    }
}

static const char *const mAwayModeStrings[] =
{
    // TRANSLATORS: away type in status bar
    N_("(O) on keyboard"),
    // TRANSLATORS: away type in status bar
    N_("(A) away"),
    // TRANSLATORS: away type in status bar
    N_("(?) away")
};

std::string GameModifiers::getAwayModeString()
{
    return gettext(getVarItem(&mAwayModeStrings[0],
        settings.awayMode, mAwayModeSize));
}

const unsigned mCameraModeSize = 2;

static const char *const mCameraModeStrings[] =
{
    // TRANSLATORS: camera mode in status bar
    N_("(G) game camera mode"),
    // TRANSLATORS: camera mode in status bar
    N_("(F) free camera mode"),
    // TRANSLATORS: camera mode in status bar
    N_("(?) away")
};

std::string GameModifiers::getCameraModeString()
{
    return gettext(getVarItem(&mCameraModeStrings[0],
        settings.cameraMode, mCameraModeSize));
}
