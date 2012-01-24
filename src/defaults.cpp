/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "defaults.h"

#include "utils/stringutils.h"

#include "being.h"
#include "graphics.h"
#include "client.h"

#include <stdlib.h>

#include "debug.h"

using namespace Mana;

VariableData* createData(int defData)
{
    return new IntData(defData);
}

VariableData* createData(double defData)
{
    return new FloatData(defData);
}

VariableData* createData(float defData)
{
    return new FloatData(defData);
}

VariableData* createData(const std::string &defData)
{
    return new StringData(defData);
}

VariableData* createData(const char* defData)
{
    return new StringData(defData);
}

VariableData* createData(bool defData)
{
    return new BoolData(defData);
}

#define AddDEF(defaultsData, key, value)  \
 defaultsData->insert(std::pair<std::string, VariableData*> \
                     (key, createData(value)));


DefaultsData* getConfigDefaults()
{
    DefaultsData* configData = new DefaultsData;
    // Init main config defaults
    AddDEF(configData, "OverlayDetail", 2);
    AddDEF(configData, "speechBubblecolor", "000000");
    AddDEF(configData, "speechBubbleAlpha", 1.0f);
    AddDEF(configData, "MostUsedServerName0", "server.themanaworld.org");
    AddDEF(configData, "visiblenames", true);
    AddDEF(configData, "speech", static_cast<int>(Being::TEXT_OVERHEAD));
    AddDEF(configData, "showgender", true);
    AddDEF(configData, "showlevel", false);
    AddDEF(configData, "showMonstersTakedDamage", true);
    AddDEF(configData, "highlightAttackRange", false);
    AddDEF(configData, "highlightMapPortals", true);
    AddDEF(configData, "highlightMonsterAttackRange", false);
    AddDEF(configData, "chatMaxCharLimit", 300);
    AddDEF(configData, "lowTraffic", true);
    AddDEF(configData, "invertMoveDirection", 0);
    AddDEF(configData, "crazyMoveType", 1);
    AddDEF(configData, "attackWeaponType", 1);
    AddDEF(configData, "quickDropCounter", 1);
    AddDEF(configData, "pickUpType", 3);
    AddDEF(configData, "magicAttackType", 0);
    AddDEF(configData, "attackType", 1);
    AddDEF(configData, "followMode", 0);
    AddDEF(configData, "imitationMode", 0);
    AddDEF(configData, "syncPlayerMove", false);
    AddDEF(configData, "drawPath", false);
    AddDEF(configData, "moveToTargetType", 6);
    AddDEF(configData, "crazyMoveProgram", "mumrsonmdmlon");
    AddDEF(configData, "disableGameModifiers", false);
    AddDEF(configData, "targetDeadPlayers", false);
    AddDEF(configData, "afkMessage", "I am away from keyboard");
    AddDEF(configData, "particleMaxCount", 3000);
    AddDEF(configData, "particleFastPhysics", 0);
    AddDEF(configData, "particleEmitterSkip", 1);
    AddDEF(configData, "particleeffects", true);
    AddDEF(configData, "logToStandardOut", false);
    AddDEF(configData, "opengl", 0);
    AddDEF(configData, "screenwidth", defaultScreenWidth);
    AddDEF(configData, "screenheight", defaultScreenHeight);
    AddDEF(configData, "screen", false);
    AddDEF(configData, "hwaccel", false);
    AddDEF(configData, "sound", false);
    AddDEF(configData, "sfxVolume", 100);
    AddDEF(configData, "musicVolume", 60);
    AddDEF(configData, "remember", false);
    AddDEF(configData, "username", "");
    AddDEF(configData, "lastCharacter", "");
    AddDEF(configData, "fpslimit", 60);
    AddDEF(configData, "altfpslimit", 5);
    AddDEF(configData, "updatehost", "");
    AddDEF(configData, "screenshotDirectory", "");
    AddDEF(configData, "useScreenshotDirectorySuffix", true);
    AddDEF(configData, "screenshotDirectorySuffix", "");
    AddDEF(configData, "EnableSync", false);
    AddDEF(configData, "joystickEnabled", false);
    AddDEF(configData, "upTolerance", 100);
    AddDEF(configData, "downTolerance", 100);
    AddDEF(configData, "leftTolerance", 100);
    AddDEF(configData, "rightTolerance", 100);
    AddDEF(configData, "logNpcInGui", true);
    AddDEF(configData, "download-music", false);
    AddDEF(configData, "guialpha", 0.8f);
    AddDEF(configData, "ChatLogLength", 0);
    AddDEF(configData, "enableChatLog", false);
    AddDEF(configData, "whispertab", true);
    AddDEF(configData, "customcursor", true);
    AddDEF(configData, "showownname", true);
    AddDEF(configData, "showpickupparticle", false);
    AddDEF(configData, "showpickupchat", true);
    AddDEF(configData, "fontSize", 11);
    AddDEF(configData, "ReturnToggles", false);
    AddDEF(configData, "ScrollLaziness", 16);
    AddDEF(configData, "ScrollRadius", 0);
    AddDEF(configData, "ScrollCenterOffsetX", 0);
    AddDEF(configData, "ScrollCenterOffsetY", 0);
    AddDEF(configData, "onlineServerList", "");
    AddDEF(configData, "theme", "");
    AddDEF(configData, "enableMumble", false);
    AddDEF(configData, "playBattleSound", true);
    AddDEF(configData, "playGuiSound", true);
    AddDEF(configData, "playMusic", true);
    AddDEF(configData, "packetcounters", true);
    AddDEF(configData, "safemode", false);
    AddDEF(configData, "font", "fonts/dejavusans.ttf");
    AddDEF(configData, "boldFont", "fonts/dejavusans-bold.ttf");
    AddDEF(configData, "particleFont", "fonts/dejavusans.ttf");
    AddDEF(configData, "helpFont", "fonts/dejavusansmono.ttf");
    AddDEF(configData, "secureFont", "fonts/dejavusansmono.ttf");
    AddDEF(configData, "japanFont", "fonts/mplus-1p-regular.ttf");
    AddDEF(configData, "showBackground", true);
    AddDEF(configData, "enableTradeTab", true);
    AddDEF(configData, "logToChat", false);
    AddDEF(configData, "cyclePlayers", true);
    AddDEF(configData, "cycleMonsters", true);
    AddDEF(configData, "floorItemsHighlight", true);
    AddDEF(configData, "enableBotCheker", true);
    AddDEF(configData, "removeColors", true);
    AddDEF(configData, "showMagicInDebug", true);
    AddDEF(configData, "allowCommandsInChatTabs", true);
    AddDEF(configData, "serverMsgInDebug", true);
    AddDEF(configData, "hideShopMessages", true);
    AddDEF(configData, "showChatHistory", true);
    AddDEF(configData, "showChatColorsList", true);
    AddDEF(configData, "chatMaxLinesLimit", 20);
    AddDEF(configData, "chatColor", 0);
    AddDEF(configData, "hideShield", true);
    AddDEF(configData, "showJob", true);
    AddDEF(configData, "updateOnlineList", true);
    AddDEF(configData, "targetOnlyReachable", true);
    AddDEF(configData, "errorsInDebug", true);
    AddDEF(configData, "tradebot", true);
    AddDEF(configData, "debugLog", false);
    AddDEF(configData, "drawHotKeys", true);
    AddDEF(configData, "serverAttack", true);
    AddDEF(configData, "autofixPos", true);
    AddDEF(configData, "alphaCache", true);
    AddDEF(configData, "attackMoving", true);
    AddDEF(configData, "quickStats", true);
    AddDEF(configData, "warpParticle", false);
    AddDEF(configData, "autoShop", false);
    AddDEF(configData, "enableBattleTab", false);
    AddDEF(configData, "showBattleEvents", false);
    AddDEF(configData, "showMobHP", true);
    AddDEF(configData, "showOwnHP", true);
    AddDEF(configData, "usePersistentIP", false);
    AddDEF(configData, "showJobExp", true);
    AddDEF(configData, "showBeingPopup", true);
    AddDEF(configData, "showExtMinimaps", false);
    AddDEF(configData, "hideChatInput", true);
    AddDEF(configData, "enableAttackFilter", true);
    AddDEF(configData, "securetrades", true);
    AddDEF(configData, "unsecureChars", "IO0@#$");
    AddDEF(configData, "currentTip", 0);
    AddDEF(configData, "showDidYouKnow", true);
    AddDEF(configData, "useLocalTime", false);
    AddDEF(configData, "enableAdvert", true);
    AddDEF(configData, "enableMapReduce", true);
    AddDEF(configData, "showPlayersStatus", true);
    AddDEF(configData, "beingopacity", false);
    AddDEF(configData, "adjustPerfomance", true);
    AddDEF(configData, "enableAlphaFix", false);
    AddDEF(configData, "disableAdvBeingCaching", false);
    AddDEF(configData, "disableBeingCaching", false);
    AddDEF(configData, "enableReorderSprites", true);
    AddDEF(configData, "showip", false);
    AddDEF(configData, "seflMouseHeal", true);
    AddDEF(configData, "enableLazyScrolling", true);
    AddDEF(configData, "extMouseTargeting", true);
    AddDEF(configData, "showMVP", false);
    AddDEF(configData, "pvpAttackType", 0);
    AddDEF(configData, "lang", "");
    AddDEF(configData, "selectedJoystick", 0);
    AddDEF(configData, "useInactiveJoystick", false);
    AddDEF(configData, "testInfo", "");
    return configData;
}

DefaultsData* getBrandingDefaults()
{
    DefaultsData* brandingData = new DefaultsData;
    // Init config defaults
    AddDEF(brandingData, "wallpapersPath", "");
    AddDEF(brandingData, "wallpapersFile", "");
    AddDEF(brandingData, "appName", "ManaPlus");
    AddDEF(brandingData, "appIcon", "icons/manaplus.png");
    AddDEF(brandingData, "loginMusic", "Magick - Real.ogg");
    AddDEF(brandingData, "defaultServer", "");
    AddDEF(brandingData, "defaultPort", DEFAULT_PORT);
    AddDEF(brandingData, "defaultServerType", "tmwathena");
    AddDEF(brandingData, "onlineServerList",
           "http://manaplus.evolonline.org/serverlist.xml");
    AddDEF(brandingData, "appShort", "mana");
    AddDEF(brandingData, "defaultUpdateHost", "");
    AddDEF(brandingData, "helpPath", "");
//    AddDEF(brandingData, "onlineServerList", "");
    AddDEF(brandingData, "theme", "");
    AddDEF(brandingData, "font", "fonts/dejavusans.ttf");
    AddDEF(brandingData, "boldFont", "fonts/dejavusans-bold.ttf");
    AddDEF(brandingData, "particleFont", "fonts/dejavusans.ttf");
    AddDEF(brandingData, "helpFont", "fonts/dejavusansmono.ttf");
    AddDEF(brandingData, "secureFont", "fonts/dejavusansmono.ttf");
    AddDEF(brandingData, "japanFont", "fonts/mplus-1p-regular.ttf");

    AddDEF(brandingData, "guiPath", "graphics/gui/");
    AddDEF(brandingData, "guiThemePath", "themes/");
    AddDEF(brandingData, "fontsPath", "fonts/");

    AddDEF(brandingData, "wallpaperFile", "");
    AddDEF(brandingData, "dataPath", "");
    return brandingData;
}

DefaultsData* getPathsDefaults()
{
    DefaultsData *pathsData = new DefaultsData;
    // Init paths.xml defaults
    AddDEF(pathsData, "itemIcons", "graphics/items/");
    AddDEF(pathsData, "unknownItemFile", "unknown-item.png");
    AddDEF(pathsData, "sprites", "graphics/sprites/");
    AddDEF(pathsData, "spriteErrorFile", "error.xml");

    AddDEF(pathsData, "particles", "graphics/particles/");
    AddDEF(pathsData, "levelUpEffectFile", "levelup.particle.xml");
    AddDEF(pathsData, "portalEffectFile", "warparea.particle.xml");
    AddDEF(pathsData, "hitEffectId", 26);
    AddDEF(pathsData, "criticalHitEffectId", 28);

    AddDEF(pathsData, "minimaps", "graphics/minimaps/");
    AddDEF(pathsData, "maps", "maps/");

    AddDEF(pathsData, "sfx", "sfx/");
    AddDEF(pathsData, "attackSfxFile", "fist-swish.ogg");
    AddDEF(pathsData, "music", "music/");

    AddDEF(pathsData, "wallpapers", "graphics/images/");
    AddDEF(pathsData, "wallpaperFile", "login_wallpaper.png");

    AddDEF(pathsData, "help", "help/");

    return pathsData;
}

#undef AddDEF
