/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "game.h"

#include "actormanager.h"
#include "client.h"
#include "configuration.h"
#include "effectmanager.h"
#include "eventsmanager.h"
#include "gamemodifiers.h"
#include "soundmanager.h"
#include "settings.h"

#include "being/crazymoves.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/itemshortcut.h"
#include "const/spells.h"

#include "const/gui/chat.h"

#include "enums/being/beingdirection.h"

#include "fs/mkdir.h"

#include "fs/virtfs/fs.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/viewport.h"
#include "gui/windowmanager.h"
#include "gui/windowmenu.h"

#include "gui/fonts/font.h"

#include "gui/shortcut/dropshortcut.h"
#include "gui/shortcut/emoteshortcut.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/bankwindow.h"
#include "gui/windows/cutinwindow.h"
#include "gui/windows/mailwindow.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/emotewindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/whoisonline.h"

#include "gui/widgets/tabs/chat/battletab.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/emoteshortcutcontainer.h"
#include "gui/widgets/itemshortcutcontainer.h"
#include "gui/widgets/spellshortcutcontainer.h"
#include "gui/widgets/virtshortcutcontainer.h"

#include "gui/widgets/tabs/chat/gmtab.h"
#include "gui/widgets/tabs/chat/langtab.h"
#include "gui/widgets/tabs/chat/tradetab.h"

#include "input/inputmanager.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"

#include "input/touch/touchmanager.h"

#include "net/generalhandler.h"
#include "net/gamehandler.h"
#include "net/net.h"
#include "net/packetcounters.h"

#include "particle/particleengine.h"

#include "resources/delayedmanager.h"
#include "resources/mapreader.h"
#include "resources/screenshothelper.h"

#include "resources/db/mapdb.h"

#include "resources/map/map.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sprite/animatedsprite.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/pnglib.h"
#include "utils/sdlcheckutils.h"
#include "utils/timer.h"

#ifdef __native_client__
#include "utils/naclmessages.h"
#endif  // __native_client__

#include "listeners/assertlistener.h"
#include "listeners/errorlistener.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/guildmanager.h"
#endif  // TMWA_SUPPORT

#ifdef USE_MUMBLE
#include "mumblemanager.h"
#endif  // USE_MUMBLE

#ifdef WIN32
#include <sys/time.h>
#undef ERROR
#endif  // WIN32

#include "debug.h"

QuitDialog *quitDialog = nullptr;
Window *disconnectedDialog = nullptr;

bool mStatsReUpdated = false;
const time_t adjustDelay = 10;

/**
 * Initialize every game sub-engines in the right order
 */
static void initEngines()
{
    actorManager = new ActorManager;
    effectManager = new EffectManager;
#ifdef TMWA_SUPPORT
    GuildManager::init();
#endif  // TMWA_SUPPORT

    crazyMoves = new CrazyMoves;

    particleEngine = new ParticleEngine;
    particleEngine->setMap(nullptr);
    particleEngine->setupEngine();
    BeingInfo::init();

    if (gameHandler != nullptr)
        gameHandler->initEngines();

    keyboard.update();
    if (joystick != nullptr)
        joystick->update();

    UpdateStatusListener::distributeEvent();
}

/**
 * Create all the various globally accessible gui windows
 */
static void createGuiWindows()
{
    if (setupWindow != nullptr)
        setupWindow->clearWindowsForReset();

    if (emoteShortcut != nullptr)
        emoteShortcut->load();

    GameModifiers::init();

    // Create dialogs
    CREATEWIDGETV0(emoteWindow, EmoteWindow);
    delete2(debugChatTab)
    if (chatWindow != nullptr)
    {
        chatWindow->scheduleDelete();
        chatWindow = nullptr;
    }
    CREATEWIDGETV(chatWindow, ChatWindow,
        "Chat");
    CREATEWIDGETV0(tradeWindow, TradeWindow);
    CREATEWIDGETV(equipmentWindow, EquipmentWindow,
        PlayerInfo::getEquipment(),
        localPlayer);
    CREATEWIDGETV(beingEquipmentWindow, EquipmentWindow,
        nullptr, nullptr, true);
    beingEquipmentWindow->setVisible(Visible_false);
    CREATEWIDGETV0(statusWindow, StatusWindow);
    CREATEWIDGETV0(miniStatusWindow, MiniStatusWindow);
    CREATEWIDGETV(inventoryWindow, InventoryWindow,
        PlayerInfo::getInventory());
    if (Net::getNetworkType() != ServerType::TMWATHENA)
    {
        CREATEWIDGETV(cartWindow, InventoryWindow,
            PlayerInfo::getCartInventory());
    }
    CREATEWIDGETV0(cutInWindow, CutInWindow);
    CREATEWIDGETV0(shopWindow, ShopWindow);
    CREATEWIDGETV0(skillDialog, SkillDialog);
    CREATEWIDGETV0(minimap, Minimap);
    if (debugWindow != nullptr)
    {
        debugWindow->scheduleDelete();
        debugWindow = nullptr;
    }
    CREATEWIDGETV(debugWindow, DebugWindow,
        "Debug");
    CREATEWIDGETV(itemShortcutWindow, ShortcutWindow,
        "ItemShortcut", "items.xml", 83, 460);

    for (unsigned f = 0; f < SHORTCUT_TABS - 1; f ++)
    {
        itemShortcutWindow->addTab(toString(f + 1),
            new ItemShortcutContainer(nullptr, f));
    }
    if (Net::getNetworkType() != ServerType::TMWATHENA)
    {
        itemShortcutWindow->addTab("A",
            new ItemShortcutContainer(nullptr, SHORTCUT_TABS - 1));
    }
    if (config.getBoolValue("showDidYouKnow"))
    {
        didYouKnowWindow->setVisible(Visible_true);
        didYouKnowWindow->loadData(0);
    }

    CREATEWIDGETV(emoteShortcutWindow, ShortcutWindow,
        "EmoteShortcut",
        new EmoteShortcutContainer(nullptr),
        "emotes.xml",
        130, 480);
    CREATEWIDGETV0(outfitWindow, OutfitWindow);
    CREATEWIDGETV(dropShortcutWindow, ShortcutWindow,
        "DropShortcut",
        new VirtShortcutContainer(nullptr, dropShortcut),
        "drops.xml",
        0, 0);
    CREATEWIDGETV(spellShortcutWindow, ShortcutWindow,
        "SpellShortcut",
        "spells.xml",
        265, 328);
    for (unsigned f = 0; f < SPELL_SHORTCUT_TABS; f ++)
    {
        spellShortcutWindow->addTab(toString(f + 1),
            new SpellShortcutContainer(nullptr, f));
    }

    CREATEWIDGETV0(bankWindow, BankWindow);
    CREATEWIDGETV0(mailWindow, MailWindow);
    CREATEWIDGETV0(whoIsOnline, WhoIsOnline);
    CREATEWIDGETV0(killStats, KillStats);
    CREATEWIDGETV0(socialWindow, SocialWindow);
    CREATEWIDGETV0(questsWindow, QuestsWindow);

    // TRANSLATORS: chat tab header
    localChatTab = new ChatTab(chatWindow, _("General"),
        GENERAL_CHANNEL, "#General", ChatTabType::INPUT);
    localChatTab->setAllowHighlight(false);
    if (config.getBoolValue("showChatHistory"))
        localChatTab->loadFromLogFile("#General");

    // TRANSLATORS: chat tab header
    debugChatTab = new ChatTab(chatWindow, _("Debug"), "",
        "#Debug", ChatTabType::DEBUG);
    debugChatTab->setAllowHighlight(false);

    if (assertListener != nullptr)
    {
        const StringVect &messages = assertListener->getMessages();
        FOR_EACH (StringVectCIter, it, messages)
        {
            debugChatTab->chatLog(*it,
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        delete2(assertListener);
    }
    if (config.getBoolValue("enableTradeTab"))
        chatWindow->addSpecialChannelTab(TRADE_CHANNEL, false);
    else
        tradeChatTab = nullptr;

    if (config.getBoolValue("enableBattleTab"))
    {
        battleChatTab = new BattleTab(chatWindow);
        battleChatTab->setAllowHighlight(false);
    }
    else
    {
        battleChatTab = nullptr;
    }

    chatWindow->showGMTab();
    if (!isSafeMode)
        chatWindow->loadState();

    if (setupWindow != nullptr)
        setupWindow->externalUpdate();

    if (localPlayer != nullptr)
        localPlayer->updateStatus();

    if (generalHandler != nullptr)
        generalHandler->gameStarted();
}

/**
 * Destroy all the globally accessible gui windows
 */
static void destroyGuiWindows()
{
    if (generalHandler != nullptr)
        generalHandler->gameEnded();

    if (whoIsOnline != nullptr)
        whoIsOnline->setAllowUpdate(false);

#ifdef TMWA_SUPPORT
    GuildManager::clear();
#endif  // TMWA_SUPPORT

    delete2(windowMenu);
    delete2(localChatTab)  // Need to do this first, so it can remove itself
    delete2(debugChatTab)
    delete2(tradeChatTab)
    delete2(battleChatTab)
    delete2(langChatTab)
    delete2(gmChatTab);
#ifdef TMWA_SUPPORT
    if (guildManager != nullptr && GuildManager::getEnableGuildBot())
        guildManager->reload();
#endif  // TMWA_SUPPORT

    logger->log("start deleting");
    delete2(emoteWindow);
    delete2(chatWindow)
    logger->log("end deleting");
    delete2(statusWindow)
    delete2(miniStatusWindow)
    delete2(inventoryWindow)
    delete2(cartWindow)
    delete2(shopWindow)
    delete2(skillDialog)
    delete2(minimap)
    delete2(equipmentWindow)
    delete2(beingEquipmentWindow)
    delete2(tradeWindow)
    delete2(debugWindow)
    delete2(itemShortcutWindow)
    delete2(emoteShortcutWindow)
    delete2(outfitWindow)
    delete2(socialWindow)
    delete2(dropShortcutWindow);
    delete2(spellShortcutWindow);
    delete2(bankWindow);
    delete2(cutInWindow);
    delete2(mailWindow);
    delete2(questsWindow);
    delete2(whoIsOnline);
    delete2(killStats);
}

Game *Game::mInstance = nullptr;

Game::Game() :
    mCurrentMap(nullptr),
    mMapName(""),
    mValidSpeed(true),
    mNextAdjustTime(cur_time + adjustDelay),
    mAdjustLevel(0),
    mAdjustPerfomance(config.getBoolValue("adjustPerfomance")),
    mLowerCounter(0),
    mPing(0),
    mTime(cur_time + 1),
    mTime2(cur_time + 10)
{
    touchManager.setInGame(true);

//    assert(!mInstance);
    if (mInstance != nullptr)
        logger->log("error: double game creation");
    mInstance = this;

    config.incValue("gamecount");

    disconnectedDialog = nullptr;

    // Create the viewport
    viewport = new Viewport;
    viewport->setSize(mainGraphics->mWidth, mainGraphics->mHeight);
    PlayerInfo::clear();

    emptyBeingSlot = new BeingSlot;

    BasicContainer2 *const top = static_cast<BasicContainer2*>(gui->getTop());
    if (top != nullptr)
        top->add(viewport);
    viewport->requestMoveToBottom();

    AnimatedSprite::setEnableCache(
        mainGraphics->getOpenGL() != RENDER_SOFTWARE &&
        config.getBoolValue("enableDelayedAnimations"));

    CompoundSprite::setEnableDelay(
        config.getBoolValue("enableCompoundSpriteDelay"));

    createGuiWindows();
    windowMenu = new WindowMenu(nullptr);

    if (windowContainer != nullptr)
        windowContainer->add(windowMenu);

#ifdef USE_OPENGL
    MapReader::loadEmptyAtlas();
#endif  // USE_OPENGL

    initEngines();

    chatWindow->postConnection();
    mailWindow->postConnection();

    // Initialize beings
    if (actorManager != nullptr)
        actorManager->setPlayer(localPlayer);

    gameHandler->ping(tick_time);

    if (setupWindow != nullptr)
        setupWindow->setInGame(true);
    clearKeysArray();

#ifdef TMWA_SUPPORT
    if (guildManager != nullptr && GuildManager::getEnableGuildBot())
        guildManager->requestGuildInfo();
#endif  // TMWA_SUPPORT

    settings.disableLoggingInGame = config.getBoolValue(
        "disableLoggingInGame");
}

Game::~Game()
{
#ifdef USE_OPENGL
    MapReader::unloadEmptyAtlas();
#endif  // USE_OPENGL

    settings.disableLoggingInGame = false;
    touchManager.setInGame(false);
    config.write();
    serverConfig.write();
    resetAdjustLevel();
    destroyGuiWindows();

    AnimatedSprite::setEnableCache(false);

    delete2(actorManager)
    if (client->getState() != State::CHANGE_MAP)
        delete2(localPlayer)
    if (effectManager != nullptr)
        effectManager->clear();
    delete2(effectManager)
    delete2(particleEngine)
    delete2(viewport)
    delete2(mCurrentMap)
#ifdef TMWA_SUPPORT
    delete2(guildManager)
#endif  // TMWA_SUPPORT
#ifdef USE_MUMBLE
    delete2(mumbleManager)
#endif  // USE_MUMBLE

    delete2(crazyMoves);
    delete2(emptyBeingSlot);

    Being::clearCache();
    mInstance = nullptr;
    PlayerInfo::gameDestroyed();
}

void Game::addWatermark()
{
    if ((boldFont == nullptr) || !config.getBoolValue("addwatermark"))
        return;

    const Color &color1 = theme->getColor(ThemeColorId::TEXT, 255);
    const Color &color2 = theme->getColor(ThemeColorId::TEXT_OUTLINE, 255);

    boldFont->drawString(mainGraphics,
        color1,
        color2,
        settings.serverName,
        100, 50);
}

bool Game::createScreenshot(const std::string &prefix)
{
    if ((mainGraphics == nullptr) || (screenshortHelper == nullptr))
        return false;

    SDL_Surface *screenshot = nullptr;

    if (!config.getBoolValue("showip") && (gui != nullptr))
    {
        mainGraphics->setSecure(true);
        screenshortHelper->prepare();
        gui->draw();
        addWatermark();
        screenshot = screenshortHelper->getScreenshot();
        mainGraphics->setSecure(false);
    }
    else
    {
        addWatermark();
        screenshot = screenshortHelper->getScreenshot();
    }

    if (screenshot == nullptr)
        return false;

    return saveScreenshot(screenshot, prefix);
}

bool Game::saveScreenshot(SDL_Surface *const screenshot,
                          const std::string &prefix)
{
    std::string screenshotDirectory = settings.screenshotDir;
    if (mkdir_r(screenshotDirectory.c_str()) != 0)
    {
        logger->log("Directory %s doesn't exist and can't be created! "
                    "Setting screenshot directory to home.",
                    screenshotDirectory.c_str());
        screenshotDirectory = std::string(VirtFs::getUserDir());
    }

    // Search for an unused screenshot name
    std::stringstream filename;
    std::fstream testExists;

    time_t rawtime;
    char buffer [100];
    time(&rawtime);
    struct tm *const timeinfo = localtime(&rawtime);
    strftime(buffer, 99, "%Y-%m-%d_%H-%M-%S", timeinfo);

    const std::string serverName = settings.serverName;
    std::string screenShortStr;
    if (prefix.empty())
    {
        if (serverName.empty())
        {
            screenShortStr = strprintf("%s_Screenshot_%s_",
                branding.getValue("appName", "ManaPlus").c_str(),
                buffer);
        }
        else
        {
            screenShortStr = strprintf("%s_Screenshot_%s_%s_",
                branding.getValue("appName", "ManaPlus").c_str(),
                serverName.c_str(), buffer);
        }

        bool found = false;
        static unsigned int screenshotCount = 0;
        do
        {
            screenshotCount++;
            filename.str("");
            filename << screenshotDirectory << "/";
            filename << screenShortStr << screenshotCount << ".png";
            testExists.open(filename.str().c_str(), std::ios::in);
            found = !testExists.is_open();
            testExists.close();
        }
        while (!found);
    }
    else
    {
        screenShortStr = prefix;
        filename.str("");
        filename << screenshotDirectory << "/";
        filename << screenShortStr;
    }

    const std::string fileNameStr = filename.str();
    const bool success = PngLib::writePNG(screenshot, fileNameStr);
#ifdef __native_client__
    std::string nacScreenshotlDir = fileNameStr;
    cutFirst(nacScreenshotlDir, "/persistent");
    naclPostMessage("copy-from-persistent", nacScreenshotlDir);
    logger->log("nacl screenshot path: " + nacScreenshotlDir);
#endif  // __native_client__

    if (success)
    {
        if (localChatTab != nullptr)
        {
            // TRANSLATORS: save file message
            std::string str = strprintf(_("Screenshot saved as %s"),
                fileNameStr.c_str());
            localChatTab->chatLog(str,
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    else
    {
        if (localChatTab != nullptr)
        {
            // TRANSLATORS: save file message
            localChatTab->chatLog(_("Saving screenshot failed!"),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        logger->log1("Error: could not save screenshot.");
    }

    MSDL_FreeSurface(screenshot);
    return success;
}

void Game::logic()
{
    BLOCK_START("Game::logic")
    handleInput();

    // Handle all necessary game logic
    if (actorManager != nullptr)
        actorManager->logic();
    if (particleEngine != nullptr)
        particleEngine->update();
    if (mCurrentMap != nullptr)
        mCurrentMap->update(1);

    BLOCK_END("Game::logic")
}

void Game::slowLogic()
{
    BLOCK_START("Game::slowLogic")
    if (localPlayer != nullptr)
        localPlayer->slowLogic();
    const time_t time = cur_time;
    if (mTime != time)
    {
        if (valTest(Updated))
            mValidSpeed = false;

        mTime = time + 1;
        if (debugWindow != nullptr)
            debugWindow->slowLogic();
        if (killStats != nullptr)
            killStats->update();
        if (socialWindow != nullptr)
            socialWindow->slowLogic();
        if (whoIsOnline != nullptr)
            whoIsOnline->slowLogic();
        Being::reReadConfig();
        if (killStats != nullptr)
            cilk_spawn killStats->recalcStats();

        if (time > mTime2 || mTime2 - time > 10)
        {
            mTime2 = time + 10;
            config.writeUpdated();
            serverConfig.writeUpdated();
        }
        if (effectManager != nullptr)
            effectManager->logic();
    }

    if (mainGraphics->getOpenGL() != RENDER_SOFTWARE)
        DelayedManager::delayedLoad();

#ifdef TMWA_SUPPORT
    if (shopWindow != nullptr)
        cilk_spawn shopWindow->updateTimes();
    if (guildManager != nullptr)
        guildManager->slowLogic();
#endif  // TMWA_SUPPORT

    if (skillDialog != nullptr)
        cilk_spawn skillDialog->slowLogic();

    cilk_spawn PacketCounters::update();

    // Handle network stuff
    if (!gameHandler->isConnected())
    {
        if (client->getState() == State::CHANGE_MAP)
            return;  // Not a problem here

        if (client->getState() != State::ERROR)
        {
            if (disconnectedDialog == nullptr)
            {
                // TRANSLATORS: error message text
                errorMessage = _("The connection to the server was lost.");
                disconnectedDialog = DialogsManager::openErrorDialog(
                    // TRANSLATORS: error message header
                    _("Network Error"),
                    errorMessage,
                    Modal_false);
                disconnectedDialog->addActionListener(&errorListener);
                disconnectedDialog->requestMoveToTop();
            }
        }

        if ((viewport != nullptr) && !errorMessage.empty())
        {
            const Map *const map = viewport->getMap();
            if (map != nullptr)
                map->saveExtraLayer();
        }
        DialogsManager::closeDialogs();
        WindowManager::setFramerate(config.getIntValue("fpslimit"));
        mNextAdjustTime = cur_time + adjustDelay;
        if (client->getState() != State::ERROR)
            errorMessage.clear();
    }
    else
    {
        if (gameHandler->mustPing()
            && get_elapsed_time1(mPing) > 3000)
        {
            mPing = tick_time;
            gameHandler->ping(tick_time);
        }

        if (mAdjustPerfomance)
            adjustPerfomance();
        if (disconnectedDialog != nullptr)
        {
            disconnectedDialog->scheduleDelete();
            disconnectedDialog = nullptr;
        }
    }
    BLOCK_END("Game::slowLogic")
}

void Game::adjustPerfomance()
{
    FUNC_BLOCK("Game::adjustPerfomance", 1)
    const time_t time = cur_time;
    if (mNextAdjustTime <= adjustDelay)
    {
        mNextAdjustTime = time + adjustDelay;
    }
    else if (mNextAdjustTime < time)
    {
        mNextAdjustTime = time + adjustDelay;

        if (mAdjustLevel > 3 ||
            localPlayer == nullptr ||
            localPlayer->getHalfAway() ||
            settings.awayMode)
        {
            return;
        }

        int maxFps = WindowManager::getFramerate();
        if (maxFps != config.getIntValue("fpslimit"))
            return;

        if (maxFps == 0)
            maxFps = 30;
        else if (maxFps < 10)
            return;

        if (fps < maxFps - 10)
        {
            if (mLowerCounter < 2)
            {
                mLowerCounter ++;
                mNextAdjustTime = cur_time + 1;
                return;
            }
            mLowerCounter = 0;
            mAdjustLevel ++;
            switch (mAdjustLevel)
            {
                case 1:
                {
                    if (config.getBoolValue("beingopacity"))
                    {
                        config.setValue("beingopacity", false);
                        config.setSilent("beingopacity", true);
                        if (localChatTab != nullptr)
                        {
                            localChatTab->chatLog(
                                // TRANSLATORS: auto adjust settings message
                                _("Auto disable Show beings transparency"),
                                ChatMsgType::BY_SERVER,
                                IgnoreRecord_false,
                                TryRemoveColors_true);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                }
                case 2:
                    if (ParticleEngine::emitterSkip < 4)
                    {
                        ParticleEngine::emitterSkip = 4;
                        if (localChatTab != nullptr)
                        {
                            localChatTab->chatLog(
                                // TRANSLATORS: auto adjust settings message
                                _("Auto lower Particle effects"),
                                ChatMsgType::BY_SERVER,
                                IgnoreRecord_false,
                                TryRemoveColors_true);
                        }
                    }
                    else
                    {
                        mNextAdjustTime = time + 1;
                        mLowerCounter = 2;
                    }
                    break;
                case 3:
                    if (!config.getBoolValue("alphaCache"))
                    {
                        config.setValue("alphaCache", true);
                        config.setSilent("alphaCache", false);
                        if (localChatTab != nullptr)
                        {
                            localChatTab->chatLog(
                                // TRANSLATORS: auto adjust settings message
                                _("Auto enable opacity cache"),
                                ChatMsgType::BY_SERVER,
                                IgnoreRecord_false,
                                TryRemoveColors_true);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::resetAdjustLevel()
{
    if (!mAdjustPerfomance)
        return;

    mNextAdjustTime = cur_time + adjustDelay;
    switch (mAdjustLevel)
    {
        case 1:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            break;
        case 2:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            ParticleEngine::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            break;
        default:
        case 3:
            config.setValue("beingopacity",
                config.getBoolValue("beingopacity"));
            ParticleEngine::emitterSkip = config.getIntValue(
                "particleEmitterSkip") + 1;
            config.setValue("alphaCache",
                config.getBoolValue("alphaCache"));
            break;
    }
    mAdjustLevel = 0;
}

void Game::handleMove()
{
    BLOCK_START("Game::handleMove")
    if (localPlayer == nullptr)
    {
        BLOCK_END("Game::handleMove")
        return;
    }

    // Moving player around
    if ((chatWindow != nullptr) &&
        (quitDialog == nullptr) &&
        localPlayer->canMove() &&
        !chatWindow->isInputFocused() &&
        !InventoryWindow::isAnyInputFocused() &&
        !popupMenu->isPopupVisible())
    {
        NpcDialog *const dialog = NpcDialog::getActive();
        if (dialog != nullptr)
        {
            BLOCK_END("Game::handleMove")
            return;
        }

        // Ignore input if either "ignore" key is pressed
        // Stops the character moving about if the user's window manager
        // uses "ignore+arrow key" to switch virtual desktops.
        if (inputManager.isActionActive(InputAction::IGNORE_INPUT_1) ||
            inputManager.isActionActive(InputAction::IGNORE_INPUT_2))
        {
            BLOCK_END("Game::handleMove")
            return;
        }

        unsigned char direction = 0;

        // Translate pressed keys to movement and direction
        if (inputManager.isActionActive(InputAction::MOVE_UP) ||
            ((joystick != nullptr) && joystick->isUp()))
        {
            direction |= BeingDirection::UP;
            setValidSpeed();
            localPlayer->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_DOWN) ||
                 ((joystick != nullptr) && joystick->isDown()))
        {
            direction |= BeingDirection::DOWN;
            setValidSpeed();
            localPlayer->cancelFollow();
        }

        if (inputManager.isActionActive(InputAction::MOVE_LEFT) ||
            ((joystick != nullptr) && joystick->isLeft()))
        {
            direction |= BeingDirection::LEFT;
            setValidSpeed();
            localPlayer->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_RIGHT) ||
                 ((joystick != nullptr) && joystick->isRight()))
        {
            direction |= BeingDirection::RIGHT;
            setValidSpeed();
            localPlayer->cancelFollow();
        }
        else if (inputManager.isActionActive(InputAction::MOVE_FORWARD))
        {
            direction = localPlayer->getDirection();
            setValidSpeed();
            localPlayer->cancelFollow();
        }

        if ((!inputManager.isActionActive(InputAction::EMOTE)
            && !inputManager.isActionActive(InputAction::PET_EMOTE)
            && !inputManager.isActionActive(InputAction::HOMUN_EMOTE)
            && !inputManager.isActionActive(InputAction::STOP_ATTACK))
            || direction == 0)
        {
            moveInDirection(direction);
        }
    }
    BLOCK_END("Game::handleMove")
}

void Game::moveInDirection(const unsigned char direction)
{
    if (viewport == nullptr)
        return;

    if (settings.cameraMode == 0u)
    {
        if (localPlayer != nullptr)
            localPlayer->specialMove(direction);
    }
    else
    {
        int dx = 0;
        int dy = 0;
        if ((direction & BeingDirection::LEFT) != 0)
            dx = -5;
        else if ((direction & BeingDirection::RIGHT) != 0)
            dx = 5;

        if ((direction & BeingDirection::UP) != 0)
            dy = -5;
        else if ((direction & BeingDirection::DOWN) != 0)
            dy = 5;
        viewport->moveCamera(dx, dy);
    }
}

void Game::updateFrameRate(int fpsLimit)
{
    if (fpsLimit == 0)
    {
        if (settings.awayMode)
        {
            if (settings.inputFocused != KeyboardFocus::Unfocused ||
                settings.mouseFocused)
            {
                fpsLimit = config.getIntValue("fpslimit");
            }
            else
            {
                fpsLimit = config.getIntValue("altfpslimit");
            }
        }
        else
        {
            fpsLimit = config.getIntValue("fpslimit");
        }
    }
    WindowManager::setFramerate(fpsLimit);
    mNextAdjustTime = cur_time + adjustDelay;
}

/**
 * The huge input handling method.
 */
void Game::handleInput()
{
    BLOCK_START("Game::handleInput 1")
    if (joystick != nullptr)
        joystick->logic();

    eventsManager.handleGameEvents();

    // If the user is configuring the keys then don't respond.
    if (!keyboard.isEnabled() || settings.awayMode)
    {
        BLOCK_END("Game::handleInput 1")
        return;
    }

    // If pressed outfits keys, stop processing keys.
    if (inputManager.isActionActive(InputAction::WEAR_OUTFIT)
        || inputManager.isActionActive(InputAction::COPY_OUTFIT)
        || ((setupWindow != nullptr) && setupWindow->isWindowVisible()))
    {
        BLOCK_END("Game::handleInput 1")
        return;
    }

    handleMove();
    InputManager::handleRepeat();
    BLOCK_END("Game::handleInput 1")
}

/**
 * Changes the currently active map. Should only be called while the game is
 * running.
 */
void Game::changeMap(const std::string &mapPath)
{
    BLOCK_START("Game::changeMap")

    resetAdjustLevel();
    ResourceManager::cleanProtected();

    PopupManager::clearPopup();
    PopupManager::closePopupMenu();

    // Clean up floor items, beings and particles
    if (actorManager != nullptr)
        actorManager->clear();

    // Close the popup menu on map change so that invalid options can't be
    // executed.
    if (viewport != nullptr)
        viewport->cleanHoverItems();

    // Unset the map of the player so that its particles are cleared before
    // being deleted in the next step
    if (localPlayer != nullptr)
        localPlayer->setMap(nullptr);

    if (particleEngine != nullptr)
        particleEngine->clear();

    mMapName = mapPath;

    std::string fullMap = pathJoin(paths.getValue("maps", "maps/"),
        mMapName).append(".tmx");
    std::string realFullMap = pathJoin(paths.getValue("maps", "maps/"),
        MapDB::getMapName(mMapName)).append(".tmx");

    if (!VirtFs::exists(realFullMap))
        realFullMap.append(".gz");

    // Attempt to load the new map
    Map *const newMap = MapReader::readMap(fullMap, realFullMap);

    if (mCurrentMap != nullptr)
        mCurrentMap->saveExtraLayer();

    if (newMap != nullptr)
        newMap->addExtraLayer();

    if (socialWindow != nullptr)
        socialWindow->setMap(newMap);

    // Notify the minimap and actorManager about the map change
    if (minimap != nullptr)
        minimap->setMap(newMap);
    if (actorManager != nullptr)
        actorManager->setMap(newMap);
    if (particleEngine != nullptr)
        particleEngine->setMap(newMap);
    if (viewport != nullptr)
        viewport->setMap(newMap);

    // Initialize map-based particle effects
    if (newMap != nullptr)
        newMap->initializeParticleEffects();

    // Start playing new music file when necessary
    const std::string oldMusic = mCurrentMap != nullptr
        ? mCurrentMap->getMusicFile() : "";
    const std::string newMusic = newMap != nullptr ?
        newMap->getMusicFile() : "";
    if (newMusic != oldMusic)
    {
        if (newMusic.empty())
            soundManager.fadeOutMusic(1000);
        else
            soundManager.fadeOutAndPlayMusic(newMusic, 1000);
    }

    if (mCurrentMap != nullptr)
        mCurrentMap->saveExtraLayer();

    delete mCurrentMap;
    mCurrentMap = newMap;

    if (questsWindow != nullptr)
        questsWindow->setMap(mCurrentMap);

#ifdef USE_MUMBLE
    if (mumbleManager)
        mumbleManager->setMap(mapPath);
#endif  // USE_MUMBLE

    if (localPlayer != nullptr)
        localPlayer->recreateItemParticles();

    gameHandler->mapLoadedEvent();
    BLOCK_END("Game::changeMap")
}

void Game::updateHistory(const SDL_Event &event)
{
    if ((localPlayer == nullptr) || (settings.attackType == 0u))
        return;

    if (CAST_S32(event.key.keysym.sym) != -1)
    {
        bool old = false;

        const InputActionT key = KeyboardConfig::getKeyIndex(event);
        const time_t time = cur_time;
        int idx = -1;
        for (int f = 0; f < MAX_LASTKEYS; f ++)
        {
            LastKey &lastKey = mLastKeys[f];
            if (lastKey.key == key)
            {
                idx = f;
                old = true;
                break;
            }
            else if (idx >= 0 && lastKey.time < mLastKeys[idx].time)
            {
                idx = f;
            }
        }
        if (idx < 0)
        {
            idx = 0;
            for (int f = 0; f < MAX_LASTKEYS; f ++)
            {
                LastKey &lastKey = mLastKeys[f];
                if (lastKey.key == InputAction::NO_VALUE ||
                    lastKey.time < mLastKeys[idx].time)
                {
                    idx = f;
                }
            }
        }

        if (idx < 0)
            idx = 0;

        LastKey &keyIdx = mLastKeys[idx];
        if (!old)
        {
            keyIdx.time = time;
            keyIdx.key = key;
            keyIdx.cnt = 0;
        }
        else
        {
            keyIdx.cnt++;
        }
    }
}

void Game::checkKeys()
{
    const int timeRange = 120;
    const int cntInTime = 130;

    if ((localPlayer == nullptr) || (settings.attackType == 0u))
        return;

    const time_t time = cur_time;
    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        LastKey &lastKey = mLastKeys[f];
        if (lastKey.key != InputAction::NO_VALUE)
        {
            if (lastKey.time + timeRange < time)
            {
                if (lastKey.cnt > cntInTime)
                    mValidSpeed = false;
                lastKey.key = InputAction::NO_VALUE;
            }
        }
    }
}

void Game::setValidSpeed()
{
    clearKeysArray();
    mValidSpeed = true;
}

void Game::clearKeysArray()
{
    for (int f = 0; f < MAX_LASTKEYS; f ++)
    {
        mLastKeys[f].time = 0;
        mLastKeys[f].key = InputAction::NO_VALUE;
        mLastKeys[f].cnt = 0;
    }
}

void Game::videoResized(const int width, const int height)
{
    if (viewport != nullptr)
        viewport->setSize(width, height);
    if (windowMenu != nullptr)
        windowMenu->setPosition(width - windowMenu->getWidth(), 0);
}
