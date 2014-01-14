/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "defaults.h"

#include "graphicsmanager.h"

#include "being/being.h"

#include "input/keydata.h"

#include "render/graphics.h"

#include "net/netconsts.h"

#ifndef USE_SDL2
#include <SDL_keyboard.h>
#endif

#include "debug.h"

VariableData* createData(const int defData)
{
    return new IntData(defData);
}

VariableData* createData(const double defData)
{
    return new FloatData(defData);
}

VariableData* createData(const float defData)
{
    return new FloatData(defData);
}

VariableData* createData(const std::string &defData)
{
    return new StringData(defData);
}

VariableData* createData(const char *const defData)
{
    return new StringData(defData);
}

VariableData* createData(const bool defData)
{
    return new BoolData(defData);
}

#define AddDEF(key, value)  \
    configData->insert(std::pair<std::string, VariableData*> \
    (key, createData(value)));

DefaultsData* getConfigDefaults()
{
    DefaultsData *const configData = new DefaultsData;
    AddDEF("OverlayDetail", 2);
    AddDEF("speechBubbleAlpha", 1.0F);
    AddDEF("MostUsedServerName0", "server.themanaworld.org");
    AddDEF("visiblenames", true);
    AddDEF("speech", static_cast<int>(Being::NO_NAME_IN_BUBBLE));
    AddDEF("showgender", true);
    AddDEF("showlevel", false);
    AddDEF("showMonstersTakedDamage", true);
    AddDEF("highlightAttackRange", false);
    AddDEF("highlightMapPortals", true);
    AddDEF("highlightMonsterAttackRange", false);
    AddDEF("chatMaxCharLimit", 300);
    AddDEF("lowTraffic", true);
    AddDEF("invertMoveDirection", 0);
    AddDEF("crazyMoveType", 1);
    AddDEF("attackWeaponType", 1);
    AddDEF("quickDropCounter", 1);
    AddDEF("pickUpType", 3);
    AddDEF("magicAttackType", 0);
    AddDEF("attackType", 2);
    AddDEF("followMode", 0);
    AddDEF("imitationMode", 0);
    AddDEF("syncPlayerMove", false);
    AddDEF("drawPath", false);
    AddDEF("moveToTargetType", 6);
    AddDEF("crazyMoveProgram", "mumrsonmdmlon");
    AddDEF("disableGameModifiers", false);
    AddDEF("targetDeadPlayers", false);
    AddDEF("afkMessage", "I am away from keyboard.");
    AddDEF("afkFormat", 0);
    AddDEF("particleMaxCount", 3000);
    AddDEF("particleFastPhysics", 1);
    AddDEF("particleEmitterSkip", 1);
    AddDEF("particleeffects", true);
    AddDEF("logToStandardOut", false);
    AddDEF("opengl", 0);
#ifdef ANDROID
    AddDEF("screenwidth", 0);
    AddDEF("screenheight", 0);
    AddDEF("showScreenJoystick", true);
    AddDEF("showScreenButtons", true);
    AddDEF("showBeingPopup", false);
    AddDEF("mouseDirectionMove", true);
    AddDEF("showScreenKeyboard", true);
    AddDEF("fpslimit", 100);
    AddDEF("theme", "jewelry");
    AddDEF("showChatColorsList", false);
    AddDEF("customcursor", false);
    AddDEF("showDidYouKnow", false);
#else
    AddDEF("screenwidth", defaultScreenWidth);
    AddDEF("screenheight", defaultScreenHeight);
    AddDEF("showScreenJoystick", false);
    AddDEF("showScreenButtons", false);
    AddDEF("showBeingPopup", true);
    AddDEF("mouseDirectionMove", false);
    AddDEF("showScreenKeyboard", false);
    AddDEF("fpslimit", 60);
    AddDEF("theme", "");
    AddDEF("showChatColorsList", true);
#ifdef __native_client__
    AddDEF("customcursor", false);
#else
    AddDEF("customcursor", true);
#endif
    AddDEF("showDidYouKnow", true);
#endif
    AddDEF("showEmotesButton", true);
    AddDEF("screen", false);
    AddDEF("hwaccel", false);
    AddDEF("sound", false);
    AddDEF("sfxVolume", 100);
    AddDEF("musicVolume", 60);
    AddDEF("remember", false);
    AddDEF("username", "");
    AddDEF("lastCharacter", "");
    AddDEF("altfpslimit", 5);
    AddDEF("updatehost", "");
    AddDEF("screenshotDirectory2", "");
    AddDEF("useScreenshotDirectorySuffix", true);
    AddDEF("screenshotDirectorySuffix", "");
    AddDEF("EnableSync", false);
    AddDEF("joystickEnabled", false);
    AddDEF("upTolerance", 100);
    AddDEF("downTolerance", 100);
    AddDEF("leftTolerance", 100);
    AddDEF("rightTolerance", 100);
    AddDEF("logNpcInGui", true);
    AddDEF("download-music", false);
    AddDEF("guialpha", 0.8F);
    AddDEF("ChatLogLength", 0);
    AddDEF("enableChatLog", true);
    AddDEF("whispertab", true);
    AddDEF("showownname", true);
    AddDEF("showpickupparticle", true);
    AddDEF("showpickupchat", true);
    AddDEF("ReturnToggles", false);
    AddDEF("ScrollLaziness", 16);
    AddDEF("ScrollRadius", 0);
    AddDEF("ScrollCenterOffsetX", 0);
    AddDEF("ScrollCenterOffsetY", 0);
    AddDEF("onlineServerList", "");
    AddDEF("enableMumble", false);
    AddDEF("playBattleSound", true);
    AddDEF("playGuiSound", true);
    AddDEF("playMusic", true);
    AddDEF("packetcounters", true);
    AddDEF("safemode", false);
    AddDEF("font", "fonts/dejavusans.ttf");
    AddDEF("boldFont", "fonts/dejavusans-bold.ttf");
    AddDEF("particleFont", "fonts/dejavusans.ttf");
    AddDEF("helpFont", "fonts/dejavusansmono.ttf");
    AddDEF("secureFont", "fonts/dejavusansmono.ttf");
    AddDEF("japanFont", "fonts/mplus-1p-regular.ttf");
    AddDEF("chinaFont", "fonts/wqy-microhei.ttf");
    AddDEF("npcFont", "fonts/dejavusans.ttf");
    AddDEF("showBackground", true);
    AddDEF("enableTradeTab", true);
    AddDEF("cyclePlayers", true);
    AddDEF("cycleMonsters", true);
    AddDEF("cycleNPC", true);
    AddDEF("floorItemsHighlight", true);
    AddDEF("enableBotCheker", true);
    AddDEF("removeColors", true);
    AddDEF("showMagicInDebug", true);
    AddDEF("allowCommandsInChatTabs", true);
    AddDEF("serverMsgInDebug", true);
    AddDEF("hideShopMessages", true);
    AddDEF("showChatHistory", true);
    AddDEF("chatMaxLinesLimit", 40);
    AddDEF("chatColor", 0);
    AddDEF("hideShield", true);
    AddDEF("showJob", true);
    AddDEF("updateOnlineList", true);
    AddDEF("targetOnlyReachable", true);
    AddDEF("errorsInDebug", true);
    AddDEF("tradebot", true);
    AddDEF("debugLog", false);
    AddDEF("drawHotKeys", true);
    AddDEF("serverAttack", true);
    AddDEF("autofixPos", false);
    AddDEF("alphaCache", true);
    AddDEF("attackMoving", true);
    AddDEF("attackNext", false);
    AddDEF("quickStats", true);
    AddDEF("warpParticle", false);
    AddDEF("autoShop", false);
    AddDEF("enableBattleTab", false);
    AddDEF("showBattleEvents", false);
    AddDEF("showMobHP", true);
    AddDEF("showOwnHP", true);
    AddDEF("usePersistentIP", false);
    AddDEF("showJobExp", true);
    AddDEF("showExtMinimaps", false);
    AddDEF("hideChatInput", true);
    AddDEF("enableAttackFilter", true);
    AddDEF("enablePickupFilter", true);
    AddDEF("securetrades", true);
    AddDEF("unsecureChars", "IO0@#$");
    AddDEF("currentTip", 0);
    AddDEF("useLocalTime", false);
    AddDEF("enableAdvert", true);
    AddDEF("enableMapReduce", true);
    AddDEF("showPlayersStatus", true);
    AddDEF("beingopacity", false);
    AddDEF("adjustPerfomance", true);
    AddDEF("enableAlphaFix", false);
    AddDEF("disableAdvBeingCaching", false);
    AddDEF("disableBeingCaching", false);
    AddDEF("enableReorderSprites", true);
    AddDEF("showip", false);
    AddDEF("seflMouseHeal", true);
    AddDEF("enableLazyScrolling", true);
    AddDEF("extMouseTargeting", true);
    AddDEF("showMVP", false);
    AddDEF("pvpAttackType", 0);
    AddDEF("lang", "");
    AddDEF("selectedJoystick", 0);
    AddDEF("useInactiveJoystick", false);
    AddDEF("testInfo", "");
    AddDEF("enableresize", true);
    AddDEF("noframe", false);
    AddDEF("groupFriends", true);
    AddDEF("grabinput", false);
    AddDEF("usefbo", false);
    AddDEF("gamma", 1);
    AddDEF("vsync", 0);
    AddDEF("enableBuggyServers", true);
    AddDEF("soundwhisper", "newmessage");
    AddDEF("soundhighlight", "reminder");
    AddDEF("soundglobal", "email");
    AddDEF("sounderror", "error");
    AddDEF("soundtrade", "start");
    AddDEF("soundinfo", "notify");
    AddDEF("soundrequest", "attention");
    AddDEF("soundguild", "newmessage");
    AddDEF("soundshowwindow", "page");
    AddDEF("soundhidewindow", "book");
    AddDEF("autohideButtons", true);
    AddDEF("autohideChat", false);
    AddDEF("downloadProxy", "");
    AddDEF("downloadProxyType", 0);
    AddDEF("blur", false);
#if defined(WIN32) || defined(__APPLE__)
    AddDEF("centerwindow", true);
#else
    AddDEF("centerwindow", false);
#endif
    AddDEF("audioFrequency", 44100);
    AddDEF("audioChannels", 2);
#ifdef USE_SDL2
    AddDEF("repeateDelay", 500);
    AddDEF("repeateInterval", 30);
    AddDEF("repeateInterval2", 500);
#else
    AddDEF("repeateDelay", SDL_DEFAULT_REPEAT_DELAY);
    AddDEF("repeateInterval", SDL_DEFAULT_REPEAT_INTERVAL);
    AddDEF("repeateInterval2", SDL_DEFAULT_REPEAT_DELAY);
#endif
    AddDEF("compresstextures", 0);
    AddDEF("rectangulartextures", true);
    AddDEF("networksleep", 0);
    AddDEF("newtextures", true);
    AddDEF("videodetected", false);
    AddDEF("hideErased", false);
    AddDEF("enableDelayedAnimations", true);
    AddDEF("enableCompoundSpriteDelay", true);
    AddDEF("useAtlases", true);
    AddDEF("useTextureSampler", false);
    AddDEF("ministatussaved", 0);
    AddDEF("allowscreensaver", false);
    AddDEF("debugOpenGL", 0);
    AddDEF("protectChatFocus", true);
#if defined(__APPLE__)
    AddDEF("enableGamma", false);
#else
    AddDEF("enableGamma", true);
#endif
    AddDEF("logInput", false);
    AddDEF("highlightWords", "");
    AddDEF("globalsFilter", "Sagatha");
    AddDEF("selfMouseHeal", true);
    AddDEF("serverslistupdate", "");
    AddDEF("fadeoutmusic", true);
    AddDEF("screenActionKeyboard", Input::KEY_SHOW_KEYBOARD);
    AddDEF("screenActionButton0", Input::KEY_STOP_SIT);
    AddDEF("screenActionButton1", Input::KEY_TARGET_ATTACK);
    AddDEF("screenActionButton2", Input::KEY_PICKUP);
    AddDEF("screenActionButton3", Input::KEY_PATHFIND);
    AddDEF("screenActionButton4", Input::KEY_STOP_ATTACK);
    AddDEF("screenActionButton5", Input::KEY_TALK);
    AddDEF("screenActionButton6", Input::KEY_TARGET_NPC);
    AddDEF("screenActionButton7", Input::KEY_TARGET_MONSTER);
    AddDEF("screenActionButton8", Input::KEY_WINDOW_CHAT);
    AddDEF("screenActionButton9", Input::KEY_WINDOW_DEBUG);
    AddDEF("screenActionButton10", Input::KEY_TRADE);
    AddDEF("screenActionButton11", Input::KEY_DIRECT_DOWN);
    AddDEF("screenButtonsFormat", 0);
    AddDEF("autoresizeminimaps", false);
    AddDEF("showGuildOnline", false);
    AddDEF("showPartyOnline", false);
    AddDEF("enableGmTab", true);
    AddDEF("gamecount", 0);
    AddDEF("rated", false);
    AddDEF("weightMsg", true);
    AddDEF("enableLangTab", true);
    AddDEF("showAllLang", false);
    AddDEF("moveNames", false);
    AddDEF("uselonglivesprites", false);
    AddDEF("uselonglivesounds", true);
    AddDEF("screenDensity", 0);
    AddDEF("cfgver", 4);
    AddDEF("enableDebugLog", false);
    AddDEF("doubleClick", true);
    AddDEF("useDiagonalSpeed", true);
    AddDEF("protectedItems", "");
    AddDEF("inventorySortOrder", 0);
    AddDEF("storageSortOrder", 0);
    AddDEF("buySortOrder", 0);
    AddDEF("showmotd", false);
    AddDEF("playMapAnimations", true);
    AddDEF("usepets", true);
    return configData;
}

void getConfigDefaults2(DefaultsData *const configData)
{
    if (!configData)
        return;
    const int density = graphicsManager.getDensity();
    int size = 12;
    int buttonSize = 1;
    switch (density)
    {
        case DENSITY_LOW:
        case DENSITY_MEDIUM:
            buttonSize = 1;
            break;
        case DENSITY_TV:
            size = 14;
            buttonSize = 1;
            break;
        case DENSITY_HIGH:
            size = 16;
            buttonSize = 2;
            break;
        case DENSITY_XHIGH:
            size = 18;
            buttonSize = 3;
            break;
        case DENSITY_XXHIGH:
            size = 20;
            buttonSize = 3;
            break;
        default:
            break;
    }
    AddDEF("fontSize", size);
    AddDEF("npcfontSize", size + 1);
    if (mainGraphics->getHeight() < 480)
    {
        AddDEF("screenButtonsSize", 0);
        AddDEF("screenJoystickSize", 0);
    }
    else
    {
        AddDEF("screenButtonsSize", buttonSize);
        AddDEF("screenJoystickSize", buttonSize);
    }
}

DefaultsData* getBrandingDefaults()
{
    DefaultsData *const configData = new DefaultsData;
    AddDEF("wallpapersPath", "");
    AddDEF("wallpapersFile", "");
    AddDEF("appName", "ManaPlus");
    AddDEF("appIcon", "icons/manaplus");
    AddDEF("loginMusic", "Magick - Real.ogg");
    AddDEF("defaultServer", "");
    AddDEF("defaultPort", DEFAULT_PORT);
    AddDEF("defaultServerType", "tmwathena");
    AddDEF("onlineServerList",
           "http://manaplus.org/serverlist.xml");
    AddDEF("onlineServerFile", "serverlistplus.xml");
    AddDEF("appShort", "mana");
    AddDEF("screenshots", "ManaPlus");
    AddDEF("defaultUpdateHost", "");
    AddDEF("helpPath", "");
    AddDEF("theme", "");
    AddDEF("font", "fonts/dejavusans.ttf");
    AddDEF("boldFont", "fonts/dejavusans-bold.ttf");
    AddDEF("particleFont", "fonts/dejavusans.ttf");
    AddDEF("helpFont", "fonts/dejavusansmono.ttf");
    AddDEF("secureFont", "fonts/dejavusansmono.ttf");
    AddDEF("npcFont", "fonts/dejavusans.ttf");
    AddDEF("japanFont", "fonts/mplus-1p-regular.ttf");
    AddDEF("chinaFont", "fonts/wqy-microhei.ttf");

    AddDEF("guiPath", "graphics/gui/");
    AddDEF("guiThemePath", "themes/");
    AddDEF("fontsPath", "fonts/");

    AddDEF("systemsounds", "sfx/system/");

    AddDEF("wallpaperFile", "");
    AddDEF("dataPath", "");
    return configData;
}

DefaultsData* getPathsDefaults()
{
    DefaultsData *const configData = new DefaultsData;
    AddDEF("itemIcons", "graphics/items/");
    AddDEF("unknownItemFile", "unknown-item.png");
    AddDEF("sprites", "graphics/sprites/");
    AddDEF("spriteErrorFile", "error.xml");
    AddDEF("guiIcons", "graphics/guiicons/");

    AddDEF("particles", "graphics/particles/");
    AddDEF("portalEffectFile", "warparea.particle.xml");
    AddDEF("effectId", -1);
    AddDEF("hitEffectId", 26);
    AddDEF("missEffectId", -1);
    AddDEF("criticalHitEffectId", 28);
    AddDEF("afkEffectId", -1);
    AddDEF("newQuestEffectId", -1);
    AddDEF("completeQuestEffectId", -1);
    AddDEF("skillLevelUpEffectId", -1);

    AddDEF("minimaps", "graphics/minimaps/");
    AddDEF("maps", "maps/");
    AddDEF("sfx", "sfx/");
    AddDEF("music", "music/");

    AddDEF("wallpapers", "graphics/images/");
    AddDEF("wallpaperFile", "login_wallpaper.png");

    AddDEF("help", "help/");

    AddDEF("statusEffectsFile", "status-effects.xml");
    AddDEF("statusEffectsPatchFile", "status-effects_patch.xml");
    AddDEF("statusEffectsPatchDir", "status-effects.d");
    AddDEF("effectsFile", "effects.xml");
    AddDEF("effectsPatchFile", "effects_patch.xml");
    AddDEF("effectsPatchDir", "effects.d");
    AddDEF("unitsFile", "units.xml");
    AddDEF("unitsPatchFile", "units_patch.xml");
    AddDEF("unitsPatchDir", "units.d");
    AddDEF("featuresFile", "features.xml");
    AddDEF("questsFile", "quests.xml");
    AddDEF("questsPatchFile", "quests_patch.xml");
    AddDEF("questsPatchDir", "quests.d");
    AddDEF("skillsFile", "skills.xml");
    AddDEF("skillsPatchFile", "skills_patch.xml");
    AddDEF("skillsPatchDir", "skills.d");
    AddDEF("skillsFile2", "ea-skills.xml");
    AddDEF("equipmentWindowFile", "equipmentwindow.xml");
    AddDEF("emotesFile", "emotes.xml");
    AddDEF("emotesPatchFile", "emotes_patch.xml");
    AddDEF("emotesPatchDir", "emotes.d");
    AddDEF("hairColorFile", "hair.xml");
    AddDEF("hairColorPatchFile", "hair_patch.xml");
    AddDEF("hairColorPatchDir", "hair.d");
    AddDEF("itemColorsFile", "itemcolors.xml");
    AddDEF("itemColorsPatchFile", "itemcolors_patch.xml");
    AddDEF("itemColorsPatchDir", "itemcolors.d");
    AddDEF("charCreationFile", "charcreation.xml");
    AddDEF("soundsFile", "sounds.xml");
    AddDEF("soundsPatchFile", "sounds_patch.xml");
    AddDEF("soundsPatchDir", "sounds.d");
    AddDEF("itemsFile", "items.xml");
    AddDEF("itemsPatchFile", "items_patch.xml");
    AddDEF("itemsPatchDir", "items.d");
    AddDEF("avatarsFile", "avatars.xml");
    AddDEF("avatarsPatchFile", "avatars_patch.xml");
    AddDEF("avatarsPatchDir", "avatars.d");
    AddDEF("modsFile", "mods.xml");
    AddDEF("modsPatchFile", "mods_patch.xml");
    AddDEF("modsPatchDir", "mods.d");
    AddDEF("npcsFile", "npcs.xml");
    AddDEF("npcsPatchFile", "npcs_patch.xml");
    AddDEF("npcsPatchDir", "npcs.d");
    AddDEF("petsFile", "pets.xml");
    AddDEF("petsPatchFile", "pets_patch.xml");
    AddDEF("petsPatchDir", "pets.d");
    AddDEF("monstersFile", "monsters.xml");
    AddDEF("monstersPatchFile", "monsters_patch.xml");
    AddDEF("monstersPatchDir", "monsters.d");
    AddDEF("mapsRemapFile", "maps/remap.xml");
    AddDEF("mapsFile", "maps.xml");
    AddDEF("mapsPatchFile", "maps_patch.xml");
    AddDEF("mapsPatchDir", "maps.d");
    AddDEF("deadMessagesFile", "deadmessages.xml");
    AddDEF("deadMessagesPatchFile", "deadmessages_patch.xml");
    AddDEF("deadMessagesPatchDir", "deadmessages.d");

    return configData;
}

DefaultsData* getFeaturesDefaults()
{
    DefaultsData *const configData = new DefaultsData;
    AddDEF("languageTab", false);
    AddDEF("allowFollow", true);
    return configData;
}

#undef AddDEF
