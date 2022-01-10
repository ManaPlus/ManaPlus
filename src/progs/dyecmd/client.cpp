/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "progs/dyecmd/client.h"

#include "configmanager.h"
#include "dirs.h"
#include "eventsmanager.h"
#include "graphicsmanager.h"
#include "main.h"
#include "settings.h"
#include "soundmanager.h"

#include "fs/virtfs/fs.h"
#include "fs/virtfs/tools.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/skin.h"
#include "gui/popupmanager.h"
#include "gui/windowmanager.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/desktop.h"
#include "gui/widgets/window.h"
#include "gui/widgets/windowcontainer.h"

#include "input/inputmanager.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"

#include "input/touch/touchmanager.h"

#include "resources/imagehelper.h"

#include "resources/dye/dyepalette.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/cpu.h"
#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/fuzzer.h"
#include "utils/gettext.h"
#include "utils/gettexthelper.h"
#include "utils/mrand.h"
#ifdef ANDROID
#include "fs/paths.h"
#endif  // ANDROID
#include "utils/sdlcheckutils.h"
#include "utils/sdlhelper.h"
#include "utils/timer.h"

#include "utils/translation/translationmanager.h"

#include "configuration.h"

#include "net/ipc.h"

#ifdef WIN32
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)
#include <sys/time.h>
#include "fs/specialfolder.h"
#undef ERROR
#endif  // WIN32

#ifdef __clang__
#include <ctime>
#endif  // __clang__

#ifdef ANDROID
#ifndef USE_SDL2
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_screenkeyboard.h>
PRAGMA48(GCC diagnostic pop)
#include <fstream>
#endif  // USE_SDL2
#endif  // ANDROID

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef USE_SDL2
#include <SDL2_framerate.h>
#else  // USE_SDL2
#include <SDL_framerate.h>
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

std::string errorMessage;

Client *client = nullptr;

extern FPSmanager fpsManager;

volatile bool runCounters;
bool isSafeMode = false;
int serverVersion = 0;
int packetVersion = 0;
int packetVersionMain = 0;
int packetVersionRe = 0;
int packetVersionZero = 0;
int packetsType = 0;
int itemIdLen = 2;
bool packets_main = true;
bool packets_re = false;
bool packets_zero = false;
unsigned int tmwServerVersion = 0;
time_t start_time;
unsigned int mLastHost = 0;
unsigned long mSearchHash = 0;
int textures_count = 0;

Client::Client() :
    ActionListener(),
    mCurrentDialog(nullptr),
    mSetupButton(nullptr),
    mVideoButton(nullptr),
    mHelpButton(nullptr),
    mAboutButton(nullptr),
    mThemesButton(nullptr),
    mPerfomanceButton(nullptr),
#ifdef ANDROID
    mCloseButton(nullptr),
#endif  // ANDROID
    mState(State::GAME),
    mOldState(State::START),
    mSkin(nullptr),
    mButtonPadding(1),
    mButtonSpacing(3)
{
    WindowManager::init();
}

void Client::testsInit()
{
}

void Client::testsClear()
{
}

// +++ need remove duplicate from here
void Client::gameInit()
{
    logger = new Logger;
    SDL::initLogger();

    initRand();

    // Load branding information
    if (!settings.options.brandingPath.empty())
    {
        branding.init(settings.options.brandingPath,
            UseVirtFs_false,
            SkipError_false);
    }
    setBrandingDefaults(branding);

    Dirs::initRootDir();
    Dirs::initHomeDir();

    // Configure logger
    if (!settings.options.logFileName.empty())
    {
        settings.logFileName = settings.options.logFileName;
    }
    else
    {
        settings.logFileName = pathJoin(settings.localDataDir,
            "manaplus.log");
    }
    logger->setLogFile(settings.logFileName);

#ifdef USE_FUZZER
    Fuzzer::init();
#endif  // USE_FUZZER

    if (settings.options.test.empty())
        ConfigManager::backupConfig("config.xml");
    ConfigManager::initConfiguration();
    SDL::setLogLevel(config.getIntValue("sdlLogLevel"));
    setPathsDefaults(paths);
    initPaths();
    logger->log("init 4");
    logger->setDebugLog(config.getBoolValue("debugLog"));
    logger->setReportUnimplemented(config.getBoolValue("unimplimentedLog"));

    config.incValue("runcount");

#ifndef ANDROID
    if (settings.options.test.empty())
        ConfigManager::storeSafeParameters();
#endif  // ANDROID

    if (!VirtFs::setWriteDir(settings.localDataDir))
    {
        logger->error(strprintf("%s couldn't be set as home directory! "
            "Exiting.", settings.localDataDir.c_str()));
    }

    GettextHelper::initLang();

/*
    chatLogger = new ChatLogger;
    if (settings.options.chatLogDir.empty())
    {
        chatLogger->setBaseLogDir(settings.localDataDir
            + std::string("/logs/"));
    }
    else
    {
        chatLogger->setBaseLogDir(settings.options.chatLogDir);
    }
*/
    // Log the client version
    logger->log1(FULL_VERSION);
    logger->log("Start configPath: " + config.getConfigPath());

    Dirs::initScreenshotDir();

    // Initialize SDL
    logger->log1("Initializing SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        logger->safeError(strprintf("Could not initialize SDL: %s",
            SDL_GetError()));
    }
    atexit(SDL_Quit);

#ifndef USE_SDL2
    SDL_EnableUNICODE(1);
#endif  // USE_SDL2

    WindowManager::applyKeyRepeat();
    eventsManager.init();
    eventsManager.enableEvents();

#ifdef WIN32
    Dirs::mountDataDir();
#endif  // WIN32
#ifndef USE_SDL2
    WindowManager::setIcon();
#endif  // USE_SDL2

    ConfigManager::checkConfigVersion();
    logVars();
    Cpu::detect();
    DyePalette::initFunctions();
#if defined(USE_OPENGL)
#if !defined(ANDROID) && !defined(__APPLE__) && !defined(__native_client__)
    if (!settings.options.safeMode && settings.options.test.empty()
        && !config.getBoolValue("videodetected"))
    {
        graphicsManager.detectVideoSettings();
    }
#endif  // !defined(ANDROID) && !defined(__APPLE__) &&
        // !defined(__native_client__)
#endif  // defined(USE_OPENGL)

    updateEnv();
    SDL::allowScreenSaver(config.getBoolValue("allowscreensaver"));
    initGraphics();

#ifndef WIN32
    Dirs::extractDataDir();
    Dirs::mountDataDir();
#endif  // WIN32

    Dirs::updateDataPath();

    // Add the main data directories to our VirtFs search path
    if (!settings.options.dataPath.empty())
    {
        VirtFs::mountDir(settings.options.dataPath,
            Append_false);
    }

    // Add the local data directory to VirtFs search path
    VirtFs::mountDir(settings.localDataDir,
        Append_false);
    TranslationManager::loadCurrentLang();
    TranslationManager::loadDictionaryLang();
#ifdef ENABLE_CUSTOMNLS
    TranslationManager::loadGettextLang();
#endif  // ENABLE_CUSTOMNLS

#ifdef USE_SDL2
    WindowManager::setIcon();
#endif  // USE_SDL2
    WindowManager::initTitle();

    mainGraphics->postInit();

    theme = new Theme;
    Theme::selectSkin();
    touchManager.init();

    // Initialize the item and emote shortcuts.
//    for (size_t f = 0; f < SHORTCUT_TABS; f ++)
//        itemShortcut[f] = new ItemShortcut(f);
//    emoteShortcut = new EmoteShortcut;
//    dropShortcut = new DropShortcut;

    gui = new Gui;
    gui->postInit(mainGraphics);
    dialogsManager = new DialogsManager;
    popupManager = new PopupManager;

    initSoundManager();

    // Initialize keyboard
    keyboard.init();
    inputManager.init();

    // Initialise player relations
    Joystick::init();
    WindowManager::createWindows();

    keyboard.update();
    if (joystick != nullptr)
        joystick->update();

//    if (mState != State::ERROR)
//        mState = State::CHOOSE_SERVER;

    startTimers();

    const int fpsLimit = config.getIntValue("fpslimit");
    settings.limitFps = fpsLimit > 0;

    SDL_initFramerate(&fpsManager);
    WindowManager::setFramerate(fpsLimit);

    start_time = time(nullptr);

//    PlayerInfo::init();

#ifdef ANDROID
#ifndef USE_SDL2
    WindowManager::updateScreenKeyboard(SDL_GetScreenKeyboardHeight(nullptr));
#endif  // USE_SDL2
#endif  // ANDROID

    mSkin = theme->load("windowmenu.xml",
        "",
        true,
        Theme::getThemePath());
    if (mSkin != nullptr)
    {
        mButtonPadding = mSkin->getPadding();
        mButtonSpacing = mSkin->getOption("spacing", 3);
    }
}

Client::~Client()
{
    if (!settings.options.testMode)
        gameClear();
    else
        testsClear();
}

void Client::initSoundManager()
{
    // Initialize sound engine
    try
    {
        if (config.getBoolValue("sound"))
            soundManager.init();

        soundManager.setSfxVolume(config.getIntValue("sfxVolume"));
        soundManager.setMusicVolume(config.getIntValue("musicVolume"));
    }
    catch (const char *const err)
    {
        mState = State::ERROR;
        errorMessage = err;
        logger->log("Warning: %s", err);
    }
    soundManager.playMusic(branding.getValue(
        "loginMusic",
        "Magick - Real.ogg"),
        SkipError_true);
}

void Client::initGraphics()
{
#ifndef USE_SDL2
    WindowManager::applyVSync();
#endif  // USE_SDL2

    runCounters = config.getBoolValue("packetcounters");

    graphicsManager.initGraphics();

#ifdef USE_SDL2
    WindowManager::applyVSync();
#endif  // USE_SDL2

    imageHelper->postInit();
    setConfigDefaults2(config);
    WindowManager::applyGrabMode();
    WindowManager::applyGamma();

    mainGraphics->beginDraw();
}

void Client::gameClear()
{
    if (logger != nullptr)
        logger->log1("Quitting1");

    eventsManager.shutdown();
    WindowManager::deleteWindows();
    if (windowContainer != nullptr)
        windowContainer->slowLogic();

    stopTimers();

    delete2(ipc)

    if (logger != nullptr)
        logger->log1("Quitting2");

    delete2(mCurrentDialog)
    delete2(popupManager)
    delete2(dialogsManager)
    delete2(gui)

    if (logger != nullptr)
        logger->log1("Quitting3");

    GraphicsManager::deleteRenderers();

    if (logger != nullptr)
        logger->log1("Quitting4");

    XML::cleanupXML();

    if (logger != nullptr)
        logger->log1("Quitting5");

    soundManager.close();

    if (logger != nullptr)
        logger->log1("Quitting6");

    touchManager.clear();
    ResourceManager::deleteInstance();

    soundManager.shutdown();

    if (logger != nullptr)
        logger->log1("Quitting8");

    WindowManager::deleteIcon();

    if (logger != nullptr)
        logger->log1("Quitting9");

    delete2(joystick)

    keyboard.deinit();

    if (logger != nullptr)
        logger->log1("Quitting10");

    touchManager.shutdown();

#ifdef DEBUG_CONFIG
    config.enableKeyLogging();
#endif  // DEBUG_CONFIG

    config.removeOldKeys();
    config.write();
    serverConfig.write();

    config.clear();
    serverConfig.clear();

    if (logger != nullptr)
        logger->log1("Quitting11");

#ifdef USE_PROFILER
    Perfomance::clear();
#endif  // USE_PROFILER

#ifdef DEBUG_OPENGL_LEAKS
    if (logger)
        logger->log("textures left: %d", textures_count);
#endif  // DEBUG_OPENGL_LEAKS

    Graphics::cleanUp();

    if (logger != nullptr)
        logger->log1("Quitting12");

//    delete2(chatLogger)
    TranslationManager::close();
    VirtFs::deinit();
}

#define ADDBUTTON(var, object) var = object; \
    x -= var->getWidth() + mButtonSpacing; \
    var->setPosition(x, mButtonPadding); \
    top->add(var);

void Client::stateGame()
{
    if (gui == nullptr)
        return;

    BasicContainer2 *const top = static_cast<BasicContainer2*>(
        gui->getTop());

    if (top == nullptr)
        return;

    CREATEWIDGETV(desktop, Desktop, nullptr);
    top->add(desktop);
    int x = top->getWidth() - mButtonPadding;
    ADDBUTTON(mSetupButton, new Button(desktop,
        // TRANSLATORS: setup tab quick button
        _("Setup"), "Setup", BUTTON_SKIN, this))
    ADDBUTTON(mPerfomanceButton, new Button(desktop,
        // TRANSLATORS: perfoamance tab quick button
        _("Performance"), "Perfomance", BUTTON_SKIN, this))
    ADDBUTTON(mVideoButton, new Button(desktop,
        // TRANSLATORS: video tab quick button
        _("Video"), "Video", BUTTON_SKIN, this))
    ADDBUTTON(mThemesButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("Theme"), "Themes", BUTTON_SKIN, this))
    ADDBUTTON(mAboutButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("About"), "about", BUTTON_SKIN, this))
    ADDBUTTON(mHelpButton, new Button(desktop,
        // TRANSLATORS: theme tab quick button
        _("Help"), "help", BUTTON_SKIN, this))
#ifdef ANDROID
    ADDBUTTON(mCloseButton, new Button(desktop,
        // TRANSLATORS: close quick button
        _("Close"), "close", BUTTON_SKIN, this))
#endif  // ANDROID

    desktop->setSize(mainGraphics->getWidth(),
        mainGraphics->getHeight());
}

int Client::gameExec()
{
    int lastTickTime = tick_time;

    while (mState != State::EXIT)
    {
        PROFILER_START();
        if (eventsManager.handleEvents())
            continue;

        BLOCK_START("Client::gameExec 4")
        if (gui != nullptr)
            gui->logic();
        cur_time = time(nullptr);
        int k = 0;
        while (lastTickTime != tick_time && k < 40)
        {
//            if (mGame)
//                mGame->logic();
//            else
            if (gui != nullptr)
                gui->handleInput();

            ++lastTickTime;
            k ++;
        }
        soundManager.logic();

        logic_count = logic_count + k;
        if (gui != nullptr)
            gui->slowLogic();
//        if (mGame)
//            mGame->slowLogic();
        slowLogic();
        BLOCK_END("Client::gameExec 4")

        // This is done because at some point tick_time will wrap.
        lastTickTime = tick_time;

        // Update the screen when application is visible, delay otherwise.
        if (!WindowManager::getIsMinimized())
        {
            frame_count = frame_count + 1;
            if (gui != nullptr)
                gui->draw();
            mainGraphics->updateScreen();
        }
        else
        {
            SDL_Delay(100);
        }

        BLOCK_START("~Client::SDL_framerateDelay")
        if (settings.limitFps)
            SDL_framerateDelay(&fpsManager);
        BLOCK_END("~Client::SDL_framerateDelay")

        BLOCK_START("Client::gameExec 6")
/*
        if (mState == State::CONNECT_GAME)
        {
            stateConnectGame1();
        }
*/
        BLOCK_END("Client::gameExec 6")

        if (mState != mOldState)
        {
            BLOCK_START("Client::gameExec 7")

            mOldState = mState;

            // Get rid of the dialog of the previous state
            delete2(mCurrentDialog)

            // State has changed, while the quitDialog was active, it might
            // not be correct anymore
/*
            if (mQuitDialog)
            {
                mQuitDialog->scheduleDelete();
                mQuitDialog = nullptr;
            }
*/
            BLOCK_END("Client::gameExec 7")

            BLOCK_START("Client::gameExec 8")
            PRAGMA45(GCC diagnostic push)
            PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
            switch (mState)
            {
                case State::GAME:
                    stateGame();
                    break;

                case State::LOAD_DATA:
                {
                    BLOCK_START("Client::gameExec State::LOAD_DATA")
                    logger->log1("State: LOAD DATA");

                    // If another data path has been set,
                    // we don't load any other files...
                    if (settings.options.dataPath.empty())
                    {
                        // Add customdata directory
                        VirtFs::searchAndAddArchives(
                            "customdata/",
                            "zip",
                            Append_false);
                    }

                    if (!settings.options.skipUpdate)
                    {
                        VirtFs::searchAndAddArchives(pathJoin(
                            settings.updatesDir, "local/"),
                            "zip",
                            Append_false);

                        VirtFs::mountDir(pathJoin(
                            settings.localDataDir,
                            settings.updatesDir,
                            "local/"),
                            Append_false);
                    }

                    logger->log("Init paths");
                    paths.init("paths.xml",
                        UseVirtFs_true,
                        SkipError_false);
                    setPathsDefaults(paths);
                    initPaths();
                    TranslationManager::loadCurrentLang();
                    TranslationManager::loadDictionaryLang();

                    if (desktop != nullptr)
                        desktop->reloadWallpaper();

                    mState = State::GET_CHARACTERS;
                    BLOCK_END("Client::gameExec State::LOAD_DATA")
                    break;
                }
                case State::START:
                default:
                    mState = State::FORCE_QUIT;
                    break;
            }
            PRAGMA45(GCC diagnostic pop)
            BLOCK_END("Client::gameExec 8")
        }
        PROFILER_END();
    }

    return 0;
}

void Client::action(const ActionEvent &event)
{
    std::string tab;
    const std::string &eventId = event.getId();

    if (eventId == "close")
    {
        setState(State::FORCE_QUIT);
        return;
    }
    if (eventId == "Setup")
    {
        tab.clear();
    }
    else if (eventId == "help")
    {
        inputManager.executeAction(InputAction::WINDOW_HELP);
        return;
    }
    else if (eventId == "about")
    {
        inputManager.executeAction(InputAction::WINDOW_ABOUT);
        return;
    }
    else if (eventId == "Video")
    {
        tab = "Video";
    }
    else if (eventId == "Themes")
    {
        tab = "Theme";
    }
    else if (eventId == "Perfomance")
    {
        tab = "Perfomance";
    }
    else
    {
        return;
    }
}

void Client::moveButtons(const int width)
{
    if (mSetupButton != nullptr)
    {
        int x = width - mSetupButton->getWidth() - mButtonPadding;
        mSetupButton->setPosition(x, mButtonPadding);
#ifndef WIN32
        x -= mPerfomanceButton->getWidth() + mButtonSpacing;
        mPerfomanceButton->setPosition(x, mButtonPadding);

        x -= mVideoButton->getWidth() + mButtonSpacing;
        mVideoButton->setPosition(x, mButtonPadding);

        x -= mThemesButton->getWidth() + mButtonSpacing;
        mThemesButton->setPosition(x, mButtonPadding);

        x -= mAboutButton->getWidth() + mButtonSpacing;
        mAboutButton->setPosition(x, mButtonPadding);

        x -= mHelpButton->getWidth() + mButtonSpacing;
        mHelpButton->setPosition(x, mButtonPadding);
#ifdef ANDROID
        x -= mCloseButton->getWidth() + mButtonSpacing;
        mCloseButton->setPosition(x, mButtonPadding);
#endif  // ANDROID
#endif  // WIN32
    }
}

void Client::windowRemoved(const Window *const window)
{
    if (mCurrentDialog == window)
        mCurrentDialog = nullptr;
}

void Client::focusWindow()
{
    if (mCurrentDialog != nullptr)
    {
        mCurrentDialog->requestFocus();
    }
}

void Client::updatePinState()
{
}

void Client::logVars()
{
#ifdef ANDROID
    logger->log("APPDIR: %s", getenv("APPDIR"));
    logger->log("DATADIR2: %s", getSdStoragePath().c_str());
#endif  // ANDROID
}

void Client::slowLogic()
{
}

int Client::testsExec()
{
    return 0;
}

bool Client::isTmw()
{
    const std::string &name = settings.serverName;
    if (name == "server.themanaworld.org"
        || name == "themanaworld.org"
        || name == "167.114.185.71")
    {
        return true;
    }
    return false;
}

void Client::initPaths()
{
}
