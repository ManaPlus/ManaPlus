/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/setup_other.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "configuration.h"
#include "logger.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Other::Setup_Other() :
    SetupTabScroll(),
    mProxyTypeList(new SetupItemNames()),
    mShortcutsList(new SetupItemNames())
{
    setName(_("Misc"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemLabel(_("Monsters"), "", this);

    new SetupItemCheckBox(_("Show damage inflicted to monsters"), "",
        "showMonstersTakedDamage", this, "showMonstersTakedDamageEvent");

    new SetupItemCheckBox(_("Auto target only reachable monsters"), "",
        "targetOnlyReachable", this, "targetOnlyReachableEvent");

    new SetupItemCheckBox(_("Highlight monster attack range"), "",
        "highlightMonsterAttackRange", this,
        "highlightMonsterAttackRangeEvent");

    new SetupItemCheckBox(_("Show monster hp bar"), "",
        "showMobHP", this, "showMobHPEvent");

    new SetupItemCheckBox(_("Cycle monster targets"), "",
        "cycleMonsters", this, "cycleMonstersEvent");


    new SetupItemLabel(_("Map"), "", this);

    new SetupItemCheckBox(_("Show warps particles"), "",
        "warpParticle", this, "warpParticleEvent");

    new SetupItemCheckBox(_("Highlight map portals"), "",
        "highlightMapPortals", this, "highlightMapPortalsEvent");

    new SetupItemCheckBox(_("Highlight floor items"), "",
        "floorItemsHighlight", this, "floorItemsHighlightEvent");

    new SetupItemCheckBox(_("Highlight player attack range"), "",
        "highlightAttackRange", this, "highlightAttackRangeEvent");

    new SetupItemCheckBox(_("Show extended minimaps"), "",
        "showExtMinimaps", this, "showExtMinimapsEvent");

    new SetupItemCheckBox(_("Draw path"), "", "drawPath",
        this, "drawPathEvent");

    new SetupItemCheckBox(_("Draw hotkeys on map"), "", "drawHotKeys",
        this, "drawHotKeysEvent");

    new SetupItemCheckBox(_("Enable lazy scrolling"), "",
        "enableLazyScrolling", this, "enableLazyScrollingEvent");

    new SetupItemIntTextField(_("Scroll laziness"), "", "ScrollLaziness",
        this, "ScrollLazinessEvent", 1, 160);

    new SetupItemIntTextField(_("Scroll radius"), "", "ScrollRadius",
        this, "ScrollRadiusEvent", 0, 32);


    new SetupItemLabel(_("Moving"), "", this);

    new SetupItemCheckBox(_("Auto fix position"), "",
        "autofixPos", this, "autofixPosEvent");

    new SetupItemCheckBox(_("Attack while moving"), "",
        "attackMoving", this, "attackMovingEvent");

    new SetupItemCheckBox(_("Attack next target"), "",
        "attackNext", this, "attackNextEvent");

    new SetupItemCheckBox(_("Sync player move"), "", "syncPlayerMove",
        this, "syncPlayerMoveEvent");

    new SetupItemTextField(_("Crazy move A program"), "",
        "crazyMoveProgram", this, "crazyMoveProgramEvent");


    new SetupItemLabel(_("Player"), "", this);

    new SetupItemCheckBox(_("Show own hp bar"), "",
        "showOwnHP", this, "showOwnHPEvent");

    new SetupItemCheckBox(_("Enable quick stats"), "",
        "quickStats", this, "quickStatsEvent");

    new SetupItemCheckBox(_("Cycle player targets"), "",
        "cyclePlayers", this, "cyclePlayersEvent");

    new SetupItemCheckBox(_("Show job exp messages"), "",
        "showJobExp", this, "showJobExpEvent");

    new SetupItemCheckBox(_("Show players popups"), "",
        "showBeingPopup", this, "showBeingPopupEvent");

    new SetupItemTextField(_("Afk message"), "",
        "afkMessage", this, "afkMessageEvent");

    new SetupItemCheckBox(_("Show job"), "", "showJob",
        this, "showJobEvent", false);

    new SetupItemCheckBox(_("Enable attack filter"), "",
        "enableAttackFilter", this, "enableAttackFilterEvent");

    new SetupItemCheckBox(_("Enable pickup filter"), "",
        "enablePickupFilter", this, "enablePickupFilterEvent");

    new SetupItemCheckBox(_("Enable advert protocol"), "",
        "enableAdvert", this, "enableAdvertEvent");


    new SetupItemLabel(_("Shop"), "", this);

    new SetupItemCheckBox(_("Accept sell/buy requests"), "",
        "autoShop", this, "autoShopEvent");

    new SetupItemCheckBox(_("Enable shop mode"), "",
        "tradebot", this, "tradebotEvent");


    new SetupItemLabel(_("NPC"), "", this);

    new SetupItemCheckBox(_("Cycle npc targets"), "",
        "cycleNPC", this, "cycleNPCEvent");

    new SetupItemCheckBox(_("Log NPC dialogue"), "", "logNpcInGui",
        this, "logNpcInGuiEvent");


    new SetupItemLabel(_("Bots support"), "", this);

    new SetupItemCheckBox(_("Enable auction bot support"), "",
        "enableAuctionBot", this, "enableAuctionBotEvent", false);

    new SetupItemCheckBox(_("Enable guild bot support and disable native "
        "guild support"), "", "enableGuildBot", this,
        "enableGuildBotEvent", false);


    new SetupItemLabel(_("Keyboard"), "", this);

    new SetupItemIntTextField(_("Repeat delay"), "",
        "repeateDelay", this, "repeateDelayEvent", 0, 10000);

    new SetupItemIntTextField(_("Repeat interval"), "",
        "repeateInterval", this, "repeateIntervalEvent", 0, 10000);


    new SetupItemLabel(_("Windows"), "", this);

    mShortcutsList->push_back(_("Always show"));
    mShortcutsList->push_back(_("Auto hide in small resolution"));
    mShortcutsList->push_back(_("Always auto hide"));
    new SetupItemSlider2(_("Shortcut buttons"), "", "autohideButtons", this,
        "autohideButtonsEvent", 0, 2, mShortcutsList, false, true, true);


    new SetupItemLabel(_("Proxy server"), "", this);

    mProxyTypeList->push_back(_("System proxy"));
    mProxyTypeList->push_back(_("Direct connection"));
    mProxyTypeList->push_back("HTTP");
    mProxyTypeList->push_back("HTTP 1.0");
    mProxyTypeList->push_back("SOCKS4");
    mProxyTypeList->push_back("SOCKS4A");
    mProxyTypeList->push_back("SOCKS5");
    mProxyTypeList->push_back(_("SOCKS5 hostname"));
    new SetupItemSlider2(_("Proxy type"), "", "downloadProxyType", this,
        "downloadProxyTypeEvent", 0, 7, mProxyTypeList);

    new SetupItemTextField(_("Proxy address:port"), "",
        "downloadProxy", this, "downloadProxyEvent");


    new SetupItemLabel(_("Other"), "", this);

    new SetupItemCheckBox(_("Enable server side attack"), "",
        "serverAttack", this, "serverAttackEvent");

    new SetupItemCheckBox(_("Enable bot checker"), "",
        "enableBotCheker", this, "enableBotChekerEvent");

    new SetupItemCheckBox(_("Enable buggy servers protection "
        "(do not disable)"), "", "enableBuggyServers", this,
        "enableBuggyServersEvent", false);

    new SetupItemCheckBox(_("Enable debug log"), "",
        "debugLog", this, "debugLogEvent");

    new SetupItemCheckBox(_("Low traffic mode"), "", "lowTraffic",
        this, "lowTrafficEvent");

    new SetupItemCheckBox(_("Hide shield sprite"), "", "hideShield",
        this, "hideShieldEvent");

    new SetupItemCheckBox(_("Use FBO for screenshots (only for opengl)"),
        "", "usefbo", this, "usefboEvent");

    new SetupItemIntTextField(_("Network delay between sub servers"),
        "", "networksleep", this, "networksleepEvent", 0, 10000);

    new SetupItemCheckBox(_("Show background"), "", "showBackground",
        this, "showBackgroundEvent");

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

Setup_Other::~Setup_Other()
{
    delete mProxyTypeList;
    mProxyTypeList = nullptr;
    delete mShortcutsList;
    mShortcutsList = nullptr;
}

void Setup_Other::apply()
{
    SetupTabScroll::apply();

    logger->setDebugLog(config.getBoolValue("debugLog"));
}
