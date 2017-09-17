/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/tabs/setup_misc.h"

#include "configuration.h"

#include "const/resources/map/map.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/setupbuttonitem.h"
#include "gui/widgets/scrollarea.h"

#include "listeners/uploadlistener.h"

#include "net/net.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

static const int shortcutsListSize = 3;

static const char *const shortcutsList[] =
{
    // TRANSLATORS: show buttons at top right corner type
    N_("Always show"),
    // TRANSLATORS: show buttons at top right corner type
    N_("Auto hide in small resolution"),
    // TRANSLATORS: show buttons at top right corner type
    N_("Always auto hide")
};

static const int proxyTypeListSize = 8;

static const char *const proxyTypeList[] =
{
    // TRANSLATORS: Proxy type selection
    N_("System proxy"),
    // TRANSLATORS: Proxy type selection
    N_("Direct connection"),
    "HTTP",
    "HTTP 1.0",
    "SOCKS4",
    "SOCKS4A",
    "SOCKS5",
    // TRANSLATORS: Proxy type selection
    N_("SOCKS5 hostname")
};

static const int densityListSize = 7;

static const char *const densityList[] =
{
    // TRANSLATORS: screen density type
    N_("default"),
    // TRANSLATORS: screen density type
    N_("low"),
    // TRANSLATORS: screen density type
    N_("medium"),
    // TRANSLATORS: screen density type
    N_("tv"),
    // TRANSLATORS: screen density type
    N_("high"),
    // TRANSLATORS: screen density type
    N_("xhigh"),
    // TRANSLATORS: screen density type
    N_("xxhigh")
};

#ifdef USE_SDL2
static const int sdlLogListSize = 7;

static const char *const sdlLogList[] =
{
    // TRANSLATORS: sdl2 log level
    N_("default"),
    // TRANSLATORS: sdl2 log level
    N_("verbose"),
    // TRANSLATORS: sdl2 log level
    N_("debug"),
    // TRANSLATORS: sdl2 log level
    N_("info"),
    // TRANSLATORS: sdl2 log level
    N_("warn"),
    // TRANSLATORS: sdl2 log level
    N_("error"),
    // TRANSLATORS: sdl2 log level
    N_("critical")
};
#endif  // USE_SDL2

Setup_Misc::Setup_Misc(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mProxyTypeList(new NamesModel),
    mShortcutsList(new NamesModel),
#ifdef USE_SDL2
    mSdlLogList(new NamesModel),
#endif  // USE_SDL2
    mDensityList(new NamesModel)
{
    // TRANSLATORS: misc tab in settings
    setName(_("Misc"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Monsters"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show damage inflicted to monsters"), "",
        "showMonstersTakedDamage", this, "showMonstersTakedDamageEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto target only reachable monsters"), "",
        "targetOnlyReachable", this, "targetOnlyReachableEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Select auto target for attack skills"), "",
        "skillAutotarget", this, "skillAutotargetEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Highlight monster attack range"), "",
        "highlightMonsterAttackRange", this,
        "highlightMonsterAttackRangeEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show monster hp bar"), "",
        "showMobHP", this, "showMobHPEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Cycle monster targets"), "",
        "cycleMonsters", this, "cycleMonstersEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Map"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show warps particles"), "",
        "warpParticle", this, "warpParticleEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Highlight map portals"), "",
        "highlightMapPortals", this, "highlightMapPortalsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Highlight floor items"), "",
        "floorItemsHighlight", this, "floorItemsHighlightEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Highlight player attack range"), "",
        "highlightAttackRange", this, "highlightAttackRangeEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show extended minimaps"), "",
        "showExtMinimaps", this, "showExtMinimapsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Draw path"), "", "drawPath",
        this, "drawPathEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Draw hotkeys on map"), "", "drawHotKeys",
        this, "drawHotKeysEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable lazy scrolling"), "",
        "enableLazyScrolling", this, "enableLazyScrollingEvent");

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Scroll laziness"), "", "ScrollLaziness",
        this, "ScrollLazinessEvent", 1, 160);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Scroll radius"), "", "ScrollRadius",
        this, "ScrollRadiusEvent", 0, mapTileSize);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto resize minimaps"), "", "autoresizeminimaps",
        this, "autoresizeminimapsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Play map animations"), "", "playMapAnimations",
        this, "playMapAnimationsEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Moving"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto fix position"), "",
        "autofixPos", this, "autofixPosEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show server side position"), "",
        "showserverpos", this, "showserverposEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Attack while moving"), "",
        "attackMoving", this, "attackMovingEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Attack next target"), "",
        "attackNext", this, "attackNextEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Sync player move"), "", "syncPlayerMove",
        this, "syncPlayerMoveEvent");

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Sync player move distance"), "",
        "syncPlayerMoveDistance",
        this, "syncPlayerMoveDistanceEvent", 1, 30);

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Crazy move A program"), "",
        "crazyMoveProgram", this, "crazyMoveProgramEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Mouse relative moves "
        "(good for touch interfaces)"), "", "mouseDirectionMove",
        this, "mouseDirectionMoveEvent");

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Player"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show own hp bar"), "",
        "showOwnHP", this, "showOwnHPEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable quick stats"), "",
        "quickStats", this, "quickStatsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Cycle player targets"), "",
        "cyclePlayers", this, "cyclePlayersEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show job exp messages"), "",
        "showJobExp", this, "showJobExpEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show players popups"), "",
        "showBeingPopup", this, "showBeingPopupEvent");

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Afk message"), "",
        "afkMessage", this, "afkMessageEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show job"), "", "showJob",
        this, "showJobEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable attack filter"), "",
        "enableAttackFilter", this, "enableAttackFilterEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable pickup filter"), "",
        "enablePickupFilter", this, "enablePickupFilterEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable advert protocol"), "",
        "enableAdvert", this, "enableAdvertEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enabled pets support"),
        "", "usepets", this, "usepetsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable weight notifications"), "",
        "weightMsg", this, "weightMsgEvent");

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Shop"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Accept sell/buy requests"), "",
        "autoShop", this, "autoShopEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable shop mode"), "",
        "tradebot", this, "tradebotEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("NPC"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Cycle npc targets"), "",
        "cycleNPC", this, "cycleNPCEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Log NPC dialogue"), "", "logNpcInGui",
        this, "logNpcInGuiEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Bots support"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable guild bot support and disable native "
        "guild support"), "", "enableGuildBot", this,
        "enableGuildBotEvent",
        "0",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable manamarket bot support"),
        "", "enableManaMarketBot", this,
        "enableManaMarketBotEvent",
        "0",
        MainConfig_false);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Keyboard"), "", this);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Repeat delay"), "",
        "repeateDelay", this, "repeateDelayEvent", 0, 10000);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Repeat interval"), "",
        "repeateInterval", this, "repeateIntervalEvent", 0, 10000);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Custom repeat interval"), "",
        "repeateInterval2", this, "repeateInterval2Event", 0, 10000);

#ifdef USE_SDL2
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable alt-tab workaround"), "",
        "blockAltTab", this, "blockAltTabEvent");
#endif  // USE_SDL2


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Windows"), "", this);

    mShortcutsList->fillFromArray(&shortcutsList[0], shortcutsListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Shortcut buttons"), "", "autohideButtons", this,
        "autohideButtonsEvent", mShortcutsList, 200);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Proxy server"), "", this);

    mProxyTypeList->fillFromArray(&proxyTypeList[0], proxyTypeListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Proxy type"), "", "downloadProxyType", this,
        "downloadProxyTypeEvent", mProxyTypeList, 200);

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Proxy address:port"), "",
        "downloadProxy", this, "downloadProxyEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Tunnel through HTTP proxy"), "",
        "downloadProxyTunnel", this, "downloadProxyTunnelEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Logging"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable OpenGL version check "
        "(do not disable)"), "", "checkOpenGLVersion", this,
        "checkOpenGLVersionEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable debug log"), "",
        "debugLog", this, "debugLogEvent");

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Ignore logging packets"), "",
        "ignorelogpackets", this, "ignorelogpacketsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Log unimplemented packets"), "",
        "unimplimentedLog", this, "unimplimentedLogEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable OpenGL log"), "",
        "debugOpenGL", this, "debugOpenGLEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable input log"), "",
        "logInput", this, "logInputEvent");

#ifdef USE_SDL2
    mSdlLogList->fillFromArray(&sdlLogList[0], sdlLogListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("SDL logging level"), "",
        "sdlLogLevel", this, "sdlLogLevelEvent", mSdlLogList, 100);
#endif  // USE_SDL2

    // TRANSLATORS: settings option
    new SetupButtonItem(_("Upload log file"), "", "upload",
        this, "uploadLog", &uploadListener);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable server side attack"), "",
        "serverAttack", this, "serverAttackEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Hide support page link on error"), "",
        "hidesupport", this, "hidesupportEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable double clicks"), "",
        "doubleClick", this, "doubleClickEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable bot checker"), "",
        "enableBotCheker", this, "enableBotChekerEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable buggy servers protection "
        "(do not disable)"), "", "enableBuggyServers", this,
        "enableBuggyServersEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Low traffic mode"), "", "lowTraffic",
        this, "lowTrafficEvent");

#ifndef ANDROID
    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use FBO for screenshots (only for opengl)"),
        "", "usefbo", this, "usefboEvent");
#endif  // ANDROID

#ifndef WIN32
    // TRANSLATORS: settings option
    new SetupItemTextField(_("Screenshot directory"), "",
        "screenshotDirectory3", this, "screenshotDirectory3Event",
        MainConfig_true,
        UseBase64_true);
#endif  // WIN32

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Network delay between sub servers"),
        "", "networksleep", this, "networksleepEvent", 0, 10000);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show background"), "", "showBackground",
        this, "showBackgroundEvent");

    mDensityList->fillFromArray(&densityList[0], densityListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Screen density override"), "",
        "screenDensity", this, "screenDensityEvent", mDensityList, 100);

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Misc::~Setup_Misc()
{
    delete2(mProxyTypeList);
    delete2(mShortcutsList);
    delete2(mDensityList);
#ifdef USE_SDL2
    delete2(mSdlLogList);
#endif  // USE_SDL2
}

void Setup_Misc::apply()
{
    SetupTabScroll::apply();

    logger->setDebugLog(config.getBoolValue("debugLog"));
    logger->setReportUnimplemented(config.getBoolValue("unimplimentedLog"));
    Net::loadIgnorePackets();
}

void Setup_Misc::externalUpdated()
{
    reread("showJob");
    reread("enableGuildBot");
    reread("enableManaMarketBot");
    reread("enableBuggyServers");
    reread("afkMessage");
}
