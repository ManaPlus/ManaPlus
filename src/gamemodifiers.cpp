/*
 *  The ManaPlus Client
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

#include "gamemodifiers.h"

#include "configuration.h"
#include "game.h"
#include "settings.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "gui/viewport.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"

#include "gui/widgets/createwidget.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "listeners/gamemodifierlistener.h"

#include "resources/map/map.h"

#include "listeners/awaylistener.h"
#include "listeners/updatestatuslistener.h"

#include "utils/gettext.h"

#include "debug.h"

#define addModifier(name1, name2, sz, ...) \
    const unsigned GameModifiers::m##name1##Size = sz; \
    const char *const GameModifiers::m##name1##Strings[] = \
    __VA_ARGS__; \
    std::string GameModifiers::get##name1##String() \
    { \
        return gettext(getVarItem(&m##name1##Strings[0], \
        CAST_U32(settings.name2), m##name1##Size)); \
    }

#define addModifier2(name1, name2, str, sz, ...) \
    const unsigned GameModifiers::m##name1##Size = sz; \
    const char *const GameModifiers::m##name1##Strings[] = \
    __VA_ARGS__; \
    void GameModifiers::change##name1(const bool forward) \
    { \
        changeMode(&settings.name2, m##name1##Size, str, \
            &GameModifiers::get##name1##String, 0, true, forward); \
    } \
    std::string GameModifiers::get##name1##String() \
    { \
        return gettext(getVarItem(&m##name1##Strings[0], \
        settings.name2, m##name1##Size)); \
    }

#define changeMethod(name1, name2, str) \
    void GameModifiers::change##name1(const bool forward) \
    { \
        changeMode(&settings.name2, m##name1##Size, str, \
            &GameModifiers::get##name1##String, 0, true, forward); \
    }

void GameModifiers::init()
{
    settings.crazyMoveType = config.getIntValue("crazyMoveType");
    settings.moveToTargetType = config.getIntValue("moveToTargetType");
    settings.followMode = config.getIntValue("followMode");
    settings.attackWeaponType = config.getIntValue("attackWeaponType");
    settings.attackType = config.getIntValue("attackType");
    settings.targetingType = config.getIntValue("targetingType");
    settings.quickDropCounter = config.getIntValue("quickDropCounter");
    settings.pickUpType = config.getIntValue("pickUpType");
    settings.magicAttackType = config.getIntValue("magicAttackType");
    settings.pvpAttackType = config.getIntValue("pvpAttackType");
    settings.imitationMode = config.getIntValue("imitationMode");
    settings.disableGameModifiers = config.getBoolValue(
        "disableGameModifiers");
    settings.awayMode = false;
    settings.mapDrawType = MapType::NORMAL;
//    UpdateStatusListener::distributeEvent();
}

void GameModifiers::changeMode(unsigned *restrict const var,
                               const unsigned limit,
                               const char *restrict const conf,
                               std::string (*const func)(),
                               const unsigned def,
                               const bool save,
                               const bool forward)
{
    if (var == nullptr)
        return;

    if (forward)
    {
        (*var) ++;
        if (*var >= limit)
            *var = def;
    }
    else
    {
        if (*var == 0U)
            *var = limit - 1;
        else
            (*var) --;
    }

    if (save)
        config.setValue(conf, *var);
    UpdateStatusListener::distributeEvent();
    GameModifierListener::distributeEvent();
    const std::string str = (*func)();
    if (str.size() > 4)
        debugMsg(str.substr(4))
}

const char *GameModifiers::getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz)
{
    if (index < sz)
        return arr[index];
    return arr[sz];
}

addModifier(MoveType, moveType, 5,
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
})

void GameModifiers::changeMoveType(const bool forward)
{
    localPlayer->setMoveState(0);
    changeMode(&settings.moveType, mMoveTypeSize, "invertMoveDirection",
        &GameModifiers::getMoveTypeString, 0, false, forward);
}

const unsigned GameModifiers::mCrazyMoveTypeSize = 11;

void GameModifiers::changeCrazyMoveType(const bool forward)
{
    settings.crazyMoveState = 0U;
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

addModifier2(MoveToTargetType, moveToTargetType, "moveToTargetType", 13,
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
})

addModifier2(FollowMode, followMode, "followMode", 4,
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
})

addModifier2(AttackWeaponType, attackWeaponType, "attackWeaponType", 4,
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
})

addModifier2(AttackType, attackType, "attackType", 4,
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
})

addModifier2(TargetingType, targetingType, "targetingType", 2,
{
    // TRANSLATORS: targeting type in status bar
    N_("(D) don't switch target"),
    // TRANSLATORS: targeting type in status bar
    N_("(C) always attack closest"),
    // TRANSLATORS: targeting type in status bar
    N_("(?) targeting")
})

const unsigned GameModifiers::mQuickDropCounterSize = 31;

changeMethod(QuickDropCounter, quickDropCounter, "quickDropCounter")

std::string GameModifiers::getQuickDropCounterString()
{
    const unsigned int cnt = settings.quickDropCounter;
    if (cnt > 9)
    {
        return strprintf("(%c) drop counter %u", CAST_S8(
            'a' + cnt - 10), cnt);
    }
    return strprintf("(%u) drop counter %u", cnt, cnt);
}

void GameModifiers::setQuickDropCounter(const int n)
{
    if (n < 1 || n >= CAST_S32(mQuickDropCounterSize))
        return;
    settings.quickDropCounter = n;
    config.setValue("quickDropCounter", n);
    UpdateStatusListener::distributeEvent();
    GameModifierListener::distributeEvent();
}

addModifier2(PickUpType, pickUpType, "pickUpType", 7,
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
})

addModifier2(MagicAttackType, magicAttackType, "magicAttackType", 5,
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
})

addModifier2(PvpAttackType, pvpAttackType, "pvpAttackType", 4,
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
})

addModifier2(ImitationMode, imitationMode, "imitationMode", 2,
{
    // TRANSLATORS: imitation type in status bar
    N_("(D) default imitation"),
    // TRANSLATORS: imitation type in status bar
    N_("(O) outfits imitation"),
    // TRANSLATORS: imitation type in status bar
    N_("(?) imitation")
})

addModifier(GameModifiers, disableGameModifiers, 2,
{
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are enabled"),
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are disabled"),
    // TRANSLATORS: game modifiers state in status bar
    N_("Game modifiers are unknown")
})

void GameModifiers::changeGameModifiers(const bool forward A_UNUSED)
{
    settings.disableGameModifiers = !settings.disableGameModifiers;
    config.setValue("disableGameModifiers", settings.disableGameModifiers);
    UpdateStatusListener::distributeEvent();
    GameModifierListener::distributeEvent();
}

addModifier(MapDrawType, mapDrawType, 7,
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
})

void GameModifiers::changeMapDrawType(const bool forward A_UNUSED)
{
    if (viewport != nullptr)
        viewport->toggleMapDrawType();
}

addModifier(AwayMode, awayMode, 2,
{
    // TRANSLATORS: away type in status bar
    N_("(O) on keyboard"),
    // TRANSLATORS: away type in status bar
    N_("(A) away"),
    // TRANSLATORS: away type in status bar
    N_("(?) away")
})

void GameModifiers::changeAwayMode(const bool forward A_UNUSED)
{
    if (localPlayer == nullptr)
        return;

    settings.awayMode = !settings.awayMode;
    localPlayer->setAfkTime(0);
    localPlayer->setHalfAway(false);
    localPlayer->updateName();
    Game::instance()->updateFrameRate(0);
    UpdateStatusListener::distributeEvent();
    GameModifierListener::distributeEvent();
    if (settings.awayMode)
    {
        if (chatWindow != nullptr)
            chatWindow->clearAwayLog();

        localPlayer->cancelFollow();
        localPlayer->navigateClean();
        if (outfitWindow != nullptr)
            outfitWindow->wearAwayOutfit();
        OkDialog *const dialog = CREATEWIDGETR(OkDialog,
            // TRANSLATORS: away message box header
            _("Away"),
            serverConfig.getValue("afkMessage", "I am away from keyboard."),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::SILENCE,
            Modal_true,
            ShowCenter_false,
            nullptr,
            260);
        localPlayer->setAwayDialog(dialog);
        dialog->addActionListener(localPlayer->getAwayListener());
        soundManager.volumeOff();
        localPlayer->addAfkEffect();
    }
    else
    {
        localPlayer->setAwayDialog(nullptr);
        soundManager.volumeRestore();
        if (chatWindow != nullptr)
        {
            chatWindow->displayAwayLog();
            chatWindow->clearAwayLog();
        }
        localPlayer->removeAfkEffect();
    }
}

addModifier(CameraMode, cameraMode, 2,
{
    // TRANSLATORS: camera mode in status bar
    N_("(G) game camera mode"),
    // TRANSLATORS: camera mode in status bar
    N_("(F) free camera mode"),
    // TRANSLATORS: camera mode in status bar
    N_("(?) away")
})


void GameModifiers::changeCameraMode(const bool forward A_UNUSED)
{
    if (viewport != nullptr)
        viewport->toggleCameraMode();
}

void GameModifiers::resetModifiers()
{
    settings.moveType = 0;
    settings.crazyMoveType = config.resetIntValue("crazyMoveType");
    settings.moveToTargetType = config.resetIntValue("moveToTargetType");
    settings.followMode = config.resetIntValue("followMode");
    settings.attackWeaponType = config.resetIntValue("attackWeaponType");
    settings.attackType = config.resetIntValue("attackType");
    settings.magicAttackType = config.resetIntValue("magicAttackType");
    settings.pvpAttackType = config.resetIntValue("pvpAttackType");
    settings.quickDropCounter = config.resetIntValue("quickDropCounter");
    settings.pickUpType = config.resetIntValue("pickUpType");
    settings.targetingType = config.resetIntValue("targetingType");
    settings.mapDrawType = MapType::NORMAL;
    if (viewport != nullptr)
    {
        if (settings.cameraMode != 0U)
            viewport->toggleCameraMode();
        Map *const map = viewport->getMap();
        if (map != nullptr)
            map->setDrawLayersFlags(MapType::NORMAL);
    }
    settings.imitationMode = config.resetIntValue("imitationMode");
    settings.disableGameModifiers = config.resetBoolValue(
        "disableGameModifiers");

    UpdateStatusListener::distributeEvent();
    GameModifierListener::distributeEvent();
}
