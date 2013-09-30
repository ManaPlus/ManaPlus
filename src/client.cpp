/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "main.h"

#include "client.h"

#include "auctionmanager.h"
#include "chatlogger.h"
#include "configuration.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "guild.h"
#include "guildmanager.h"
#include "graphicsmanager.h"
#include "itemshortcut.h"
#include "party.h"
#include "soundconsts.h"
#include "soundmanager.h"
#include "statuseffect.h"
#include "units.h"
#include "touchmanager.h"

#include "being/playerrelations.h"

#include "input/inputmanager.h"
#include "input/joystick.h"
#include "input/keyboardconfig.h"

#include "gui/gui.h"
#include "gui/theme.h"

#include "gui/windows/buyselldialog.h"
#include "gui/windows/buydialog.h"
#include "gui/windows/changeemaildialog.h"
#include "gui/windows/changepassworddialog.h"
#include "gui/windows/charselectdialog.h"
#include "gui/windows/confirmdialog.h"
#include "gui/windows/connectiondialog.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/logindialog.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/registerdialog.h"
#include "gui/windows/selldialog.h"
#include "gui/windows/serverdialog.h"
#include "gui/windows/setup.h"
#include "gui/windows/unregisterdialog.h"
#include "gui/windows/updaterwindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/worldselectdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/desktop.h"

#include "net/gamehandler.h"
#include "net/generalhandler.h"
#include "net/guildhandler.h"
#include "net/inventoryhandler.h"
#include "net/loginhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"

#include "resources/avatardb.h"
#include "resources/chardb.h"
#include "resources/colordb.h"
#include "resources/deaddb.h"
#include "resources/emotedb.h"
#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"
#include "resources/palettedb.h"
#include "resources/surfaceimagehelper.h"
#include "resources/sounddb.h"
#include "resources/itemdb.h"
#include "resources/mapdb.h"
#include "resources/monsterdb.h"
#ifdef MANASERV_SUPPORT
#include "resources/specialdb.h"
#endif
#include "resources/npcdb.h"
#include "resources/petdb.h"
#include "resources/resourcemanager.h"

#include "utils/cpu.h"
#include "utils/files.h"
#include "utils/fuzzer.h"
#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/paths.h"
#include "utils/physfstools.h"
#include "utils/process.h"
#include "utils/sdlcheckutils.h"
#include "utils/sdlhelper.h"

#include "utils/translation/translationmanager.h"

#include "test/testlauncher.h"
#include "test/testmain.h"

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

#include <SDL_image.h>

#ifdef WIN32
#include <SDL_syswm.h>
#include "utils/specialfolder.h"
#endif

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include <sys/stat.h>

#include <climits>
#include <fstream>

#include "mumblemanager.h"

#include "debug.h"

/**
 * Tells the max tick value,
 * setting it back to zero (and start again).
 */
static const int MAX_TICK_VALUE = INT_MAX / 2;

std::string errorMessage;
ErrorListener errorListener;
LoginData loginData;

Configuration config;              // XML file configuration reader
Configuration serverConfig;        // XML file server configuration reader
Configuration features;            // XML file features
Configuration branding;            // XML branding information reader
Configuration paths;               // XML default paths information reader
Client *client = nullptr;
Logger *logger = nullptr;          // Log object
ChatLogger *chatLogger = nullptr;  // Chat log object
KeyboardConfig keyboard;
UserPalette *userPalette = nullptr;
Graphics *mainGraphics = nullptr;

SoundManager soundManager;
RenderType openGLMode = RENDER_SOFTWARE;

static uint32_t nextTick(uint32_t interval, void *param A_UNUSED);
static uint32_t nextSecond(uint32_t interval, void *param A_UNUSED);

void ErrorListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "yes")
        openBrowser(client->getSupportUrl());
    client->setState(STATE_CHOOSE_SERVER);
}

volatile int tick_time;       /**< Tick counter */
volatile int fps = 0;         /**< Frames counted in the last second */
volatile int lps = 0;         /**< Logic processed per second */
volatile int frame_count = 0; /**< Counts the frames during one second */
volatile int logic_count = 0; /**< Counts the logic during one second */
volatile int cur_time;
volatile bool runCounters;
bool isSafeMode = false;
int serverVersion = 0;
unsigned int tmwServerVersion = 0;
int start_time;

int textures_count = 0;

#ifdef WIN32
extern "C" char const *_nl_locale_name_default(void);
#endif

/**
 * Advances game logic counter.
 * Called every 10 milliseconds by SDL_AddTimer()
 * @see MILLISECONDS_IN_A_TICK value
 */
static uint32_t nextTick(uint32_t interval, void *param A_UNUSED)
{
    tick_time++;
    if (tick_time == MAX_TICK_VALUE)
        tick_time = 0;
    return interval;
}

/**
 * Updates fps.
 * Called every seconds by SDL_AddTimer()
 */
static uint32_t nextSecond(uint32_t interval, void *param A_UNUSED)
{
    fps = frame_count;
    lps = logic_count;
    frame_count = 0;
    logic_count = 0;

    return interval;
}

/**
 * @return the elapsed time in milliseconds
 * between two tick values.
 */
int get_elapsed_time(const int startTime)
{
    const int time = tick_time;
    if (startTime <= time)
    {
        return (time - startTime) * MILLISECONDS_IN_A_TICK;
    }
    else
    {
        return (time + (MAX_TICK_VALUE - startTime))
            * MILLISECONDS_IN_A_TICK;
    }
}

int get_elapsed_time1(const int startTime)
{
    const int time = tick_time;
    if (startTime <= time)
        return time - startTime;
    else
        return time + (MAX_TICK_VALUE - startTime);
}

class AccountListener final : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &)
        {
            client->setState(STATE_CHAR_SELECT);
        }
} accountListener;

class LoginListener final : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &)
        {
            client->setState(STATE_PRE_LOGIN);
        }
} loginListener;

Client::Client(const Options &options) :
    gcn::ActionListener(),
    mOptions(options),
    mPackageDir(),
    mConfigDir(),
    mServerConfigDir(),
    mLocalDataDir(),
    mTempDir(),
    mUpdateHost(),
    mUpdatesDir(),
    mScreenshotDir(),
    mUsersDir(),
    mNpcsDir(),
    mRootDir(),
    mServerName(),
    mOnlineListUrl(),
    mCurrentServer(),
    mGame(nullptr),
    mCurrentDialog(nullptr),
    mQuitDialog(nullptr),
    mDesktop(nullptr),
    mSetupButton(nullptr),
    mVideoButton(nullptr),
    mThemesButton(nullptr),
    mPerfomanceButton(nullptr),
#ifdef ANDROID
    mCloseButton(nullptr),
#endif
    mState(STATE_CHOOSE_SERVER),
    mOldState(STATE_START),
    mIcon(nullptr),
#ifdef USE_SDL2
    mLogicCounterId(0),
    mSecondsCounterId(0),
#else
    mLogicCounterId(nullptr),
    mSecondsCounterId(nullptr),
#endif
    mCaption(),
    mFpsManager(),
    mSkin(nullptr),
    mButtonPadding(1),
    mButtonSpacing(3),
    mKeyboardHeight(0),
    mOldUpdates(),
    mGuiAlpha(1.0F),
    mLimitFps(false),
    mConfigAutoSaved(false),
    mIsMinimized(false),
    mInputFocused(true),
    mMouseFocused(true),
    mNewMessageFlag(false),
    mLogInput(false)
{
    // Initialize frame limiting
    mFpsManager.framecount = 0;
    mFpsManager.rateticks = 0;
    mFpsManager.lastticks = 0;
    mFpsManager.rate = 0;
}

void Client::testsInit()
{
    if (!mOptions.test.empty() && mOptions.test != "99")
    {
        gameInit();
    }
    else
    {
        logger = new Logger;
        initLocalDataDir();
        initTempDir();
        initConfigDir();
    }
}

void Client::gameInit()
{
    logger = new Logger;

    // Load branding information
    if (!mOptions.brandingPath.empty())
        branding.init(mOptions.brandingPath);
    branding.setDefaultValues(getBrandingDefaults());

    initRootDir();
    initHomeDir();

    // Configure logger
    if (!mOptions.logFileName.empty())
        logger->setLogFile(mOptions.logFileName);
    else
        logger->setLogFile(mLocalDataDir + "/manaplus.log");

#ifdef USE_FUZZER
    Fuzzer::init();
#endif
    initConfiguration();
    paths.setDefaultValues(getPathsDefaults());
    initFeatures();
    logger->log("init 4");
    logger->setDebugLog(config.getBoolValue("debugLog"));

    config.incValue("runcount");

    storeSafeParameters();

    const ResourceManager *const resman = ResourceManager::getInstance();
    if (!resman->setWriteDir(mLocalDataDir))
    {
        logger->error(strprintf("%s couldn't be set as home directory! "
                                "Exiting.", mLocalDataDir.c_str()));
    }

#ifdef ANDROID
#ifdef USE_SDL2
    extractAssets();

    const std::string zipName = std::string(getenv(
        "APPDIR")).append("/data.zip");
    const std::string dirName = std::string(getenv(
        "APPDIR")).append("/data");
    Files::extractZip(zipName, "data", dirName);
    Files::extractLocale();
#endif
#endif

#ifdef ENABLE_NLS
    std::string lang = config.getStringValue("lang");
#ifdef WIN32
    if (lang.empty())
        lang = std::string(_nl_locale_name_default());

    putenv(const_cast<char*>(("LANG=" + lang).c_str()));
    putenv(const_cast<char*>(("LANGUAGE=" + lang).c_str()));
    // mingw doesn't like LOCALEDIR to be defined for some reason
    if (lang != "C")
        bindTextDomain("manaplus", "translations/");
#else
    if (!lang.empty())
    {
        setEnv("LANG", lang.c_str());
        setEnv("LANGUAGE", lang.c_str());
    }
#ifdef ANDROID
#ifdef USE_SDL2
    bindTextDomain("manaplus", (std::string(getenv("APPDIR")).append(
        "/locale")).c_str());
#else
    bindTextDomain("manaplus", (std::string(PhysFs::getBaseDir())
        .append("/locale")).c_str());
#endif
#else
#ifdef ENABLE_PORTABLE
    bindTextDomain("manaplus", (std::string(PhysFs::getBaseDir())
        .append("../locale/")).c_str());
#else
#ifdef __APPLE__
    bindTextDomain("manaplus", (std::string(PhysFs::getBaseDir())
        .append("ManaPlus.app/Contents/Resources/locale/")).c_str());
#else
    bindTextDomain("manaplus", LOCALEDIR);
#endif
#endif
#endif
#endif
    char *locale = setlocale(LC_MESSAGES, lang.c_str());
    if (locale)
    {
        logger->log("locale: %s", locale);
    }
    else
    {
        locale = setlocale(LC_MESSAGES, (lang + ".utf8").c_str());
        if (locale)
            logger->log("locale: %s", locale);
        else
            logger->log("locale empty");
    }
    bind_textdomain_codeset("manaplus", "UTF-8");
    textdomain("manaplus");
#endif

#if defined(WIN32) || defined(__APPLE__)
    if (config.getBoolValue("centerwindow"))
        setEnv("SDL_VIDEO_CENTERED", "1");
    else
        setEnv("SDL_VIDEO_CENTERED", "0");
#endif

    if (config.getBoolValue("allowscreensaver"))
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "1");
    else
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "0");

    chatLogger = new ChatLogger;
    if (mOptions.chatLogDir.empty())
        chatLogger->setBaseLogDir(mLocalDataDir + std::string("/logs/"));
    else
        chatLogger->setBaseLogDir(mOptions.chatLogDir);

    logger->setLogToStandardOut(config.getBoolValue("logToStandardOut"));

    // Log the client version
    logger->log1(FULL_VERSION);
    logger->log("Start configPath: " + config.getConfigPath());

    initScreenshotDir();

    // Initialize SDL
    logger->log1("Initializing SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        logger->safeError(strprintf("Could not initialize SDL: %s",
            SDL_GetError()));
    }
    atexit(SDL_Quit);

    initPacketLimiter();
#ifndef USE_SDL2
    SDL_EnableUNICODE(1);
#endif
    applyKeyRepeat();

    // disable unused SDL events
#ifndef USE_SDL2
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
#endif
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    if (mOptions.test.empty())
    {
        mCaption = strprintf("%s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION);
    }
    else
    {
        mCaption = strprintf(
            "Please wait - VIDEO MODE TEST - %s %s - Please wait",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION);
    }

    resman->addToSearchPath(PKG_DATADIR "data/perserver/default", false);
    resman->addToSearchPath("data/perserver/default", false);

#if defined __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (uint8_t*)path,
        PATH_MAX))
    {
        fprintf(stderr, "Can't find Resources directory\n");
    }
    CFRelease(resourcesURL);
    // possible crash
    strncat(path, "/data", PATH_MAX - 1);
    resman->addToSearchPath(path, false);
// possible this need for support run client from dmg images.
//    mPackageDir = path;
#endif
    resman->addToSearchPath(PKG_DATADIR "data", false);
    mPackageDir = PKG_DATADIR "data";
    resman->addToSearchPath("data", false);

#ifdef ANDROID
#ifdef USE_SDL2
    if (getenv("APPDIR"))
    {
        resman->addToSearchPath(std::string(getenv("APPDIR"))
            + "/data", false);
    }
#endif
#endif
    // Add branding/data to PhysFS search path
    if (!mOptions.brandingPath.empty())
    {
        std::string path = mOptions.brandingPath;

        // Strip blah.mana from the path
#ifdef WIN32
        const int loc1 = path.find_last_of('/');
        const int loc2 = path.find_last_of('\\');
        const int loc = static_cast<int>(std::max(loc1, loc2));
#else
        const int loc = static_cast<int>(path.find_last_of('/'));
#endif
        if (loc > 0)
        {
            resman->addToSearchPath(path.substr(
                0, loc + 1).append("data"), false);
        }
    }

    if (mOptions.dataPath.empty()
        && !branding.getStringValue("dataPath").empty())
    {
        if (isRealPath(branding.getStringValue("dataPath")))
        {
            mOptions.dataPath = branding.getStringValue("dataPath");
        }
        else
        {
            mOptions.dataPath = branding.getDirectory().append(dirSeparator)
                + branding.getStringValue("dataPath");
        }
        mOptions.skipUpdate = true;
    }

    // Add the main data directories to our PhysicsFS search path
    if (!mOptions.dataPath.empty())
        resman->addToSearchPath(mOptions.dataPath, false);

    // Add the local data directory to PhysicsFS search path
    resman->addToSearchPath(mLocalDataDir, false);
    TranslationManager::loadCurrentLang();

#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__APPLE__)
    if (!mOptions.safeMode && mOptions.test.empty()
        && !config.getBoolValue("videodetected"))
    {
        graphicsManager.detectVideoSettings();
    }
#endif

    openGLMode = intToRenderType(config.getIntValue("opengl"));
#ifdef USE_OPENGL
    OpenGLImageHelper::setBlur(config.getBoolValue("blur"));
    SurfaceImageHelper::SDLSetEnableAlphaCache(
        config.getBoolValue("alphaCache") && !openGLMode);
    ImageHelper::setEnableAlpha(config.getFloatValue("guialpha") != 1.0F
        || openGLMode);
#else
    SurfaceImageHelper::SDLSetEnableAlphaCache(
        config.getBoolValue("alphaCache"));
    ImageHelper::setEnableAlpha(config.getFloatValue("guialpha") != 1.0F);
#endif
    logVars();
    Cpu::detect();
    graphicsManager.initGraphics(mOptions.noOpenGL);
    graphicsManager.detectPixelSize();
    runCounters = config.getBoolValue("packetcounters");
    applyVSync();
    graphicsManager.setVideoMode();
    checkConfigVersion();
    getConfigDefaults2(config.getDefaultValues());
    applyGrabMode();
    applyGamma();

    SDL::SetWindowTitle(mainGraphics->getWindow(), mCaption.c_str());
    setIcon();

    mainGraphics->_beginDraw();

    Theme::selectSkin();
    touchManager.init();

    // Initialize the item and emote shortcuts.
    for (unsigned f = 0; f < SHORTCUT_TABS; f ++)
        itemShortcut[f] = new ItemShortcut(f);
    emoteShortcut = new EmoteShortcut;
    dropShortcut = new DropShortcut;

    gui = new Gui(mainGraphics);

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
        mState = STATE_ERROR;
        errorMessage = err;
        logger->log("Warning: %s", err);
    }

    // Initialize keyboard
    keyboard.init();
    inputManager.init();

    // Initialise player relations
    player_relations.init();

    Joystick::init();

    userPalette = new UserPalette;
    setupWindow = new Setup;
    helpWindow = new HelpWindow;
    didYouKnowWindow = new DidYouKnowWindow;

    soundManager.playMusic(branding.getValue(
        "loginMusic", "Magick - Real.ogg"));

    // Initialize default server
    mCurrentServer.hostname = mOptions.serverName;
    mCurrentServer.port = mOptions.serverPort;

    loginData.username = mOptions.username;
    loginData.password = mOptions.password;
    loginData.remember = serverConfig.getValue("remember", 1);
    loginData.registerLogin = false;

    if (mCurrentServer.hostname.empty())
    {
        mCurrentServer.hostname =
            branding.getValue("defaultServer", "").c_str();
        mOptions.serverName = mCurrentServer.hostname;
    }

    if (mCurrentServer.port == 0)
    {
        mCurrentServer.port = static_cast<uint16_t>(branding.getValue(
            "defaultPort", static_cast<int>(DEFAULT_PORT)));
        mCurrentServer.type = ServerInfo::parseType(
            branding.getValue("defaultServerType", "tmwathena"));
    }

    if (chatLogger)
        chatLogger->setServerName(mCurrentServer.hostname);

    if (loginData.username.empty() && loginData.remember)
        loginData.username = serverConfig.getValue("username", "");

    if (mState != STATE_ERROR)
        mState = STATE_CHOOSE_SERVER;

    // Initialize logic and seconds counters
    tick_time = 0;
    mLogicCounterId = SDL_AddTimer(MILLISECONDS_IN_A_TICK, nextTick, nullptr);
    mSecondsCounterId = SDL_AddTimer(1000, nextSecond, nullptr);

    const int fpsLimit = config.getIntValue("fpslimit");
    mLimitFps = fpsLimit > 0;

    SDL_initFramerate(&mFpsManager);
    setFramerate(fpsLimit);
    config.addListener("fpslimit", this);
    config.addListener("guialpha", this);
    config.addListener("gamma", this);
    config.addListener("enableGamma", this);
    config.addListener("particleEmitterSkip", this);
    config.addListener("vsync", this);
    config.addListener("repeateDelay", this);
    config.addListener("repeateInterval", this);
    config.addListener("logInput", this);
    setGuiAlpha(config.getFloatValue("guialpha"));
    mLogInput = config.getBoolValue("logInput");
    optionChanged("fpslimit");

    start_time = static_cast<int>(time(nullptr));

    PlayerInfo::init();

#ifdef ANDROID
#ifndef USE_SDL2
    updateScreenKeyboard(SDL_GetScreenKeyboardHeight(nullptr));
#endif
#endif
}

Client::~Client()
{
    if (!mOptions.testMode)
        gameClear();
    else
        testsClear();
}

void Client::bindTextDomain(const char *const name, const char *const path)
{
    const char *const dir = bindtextdomain(name, path);
    if (dir)
        logger->log("bindtextdomain: %s", dir);
    else
        logger->log("bindtextdomain failed");
}

void Client::setEnv(const char *const name, const char *const value)
{
    if (!name || !value)
        return;
#ifdef WIN32
    if (putenv(const_cast<char*>((std::string(name)
        + "=" + value).c_str())))
#else
    if (setenv(name, value, 1))
#endif
    {
        logger->log("setenv failed: %s=%s", name, value);
    }
}

void Client::testsClear()
{
    if (!mOptions.test.empty())
        gameClear();
    else
        BeingInfo::clear();
}

void Client::gameClear()
{
    if (logger)
        logger->log1("Quitting1");
    config.removeListeners(this);

    delete setupWindow;
    setupWindow = nullptr;
    delete helpWindow;
    helpWindow = nullptr;
    delete didYouKnowWindow;
    didYouKnowWindow = nullptr;

    SDL_RemoveTimer(mLogicCounterId);
    SDL_RemoveTimer(mSecondsCounterId);

    // Unload XML databases
    CharDB::unload();
    DeadDB::unload();
    ColorDB::unload();
    SoundDB::unload();
    EmoteDB::unload();
    ItemDB::unload();
    MonsterDB::unload();
    NPCDB::unload();
    AvatarDB::unload();
    PaletteDB::unload();
    PETDB::unload();
    StatusEffect::unload();

    if (Net::getLoginHandler())
        Net::getLoginHandler()->clearWorlds();

#ifdef USE_MUMBLE
    delete mumbleManager;
    mumbleManager = nullptr;
#endif

    PlayerInfo::deinit();

    // Before config.write() since it writes the shortcuts to the config
    for (unsigned f = 0; f < SHORTCUT_TABS; f ++)
    {
        delete itemShortcut[f];
        itemShortcut[f] = nullptr;
    }
    delete emoteShortcut;
    emoteShortcut = nullptr;
    delete dropShortcut;
    dropShortcut = nullptr;

    player_relations.store();

    if (logger)
        logger->log1("Quitting2");

    delete mCurrentDialog;
    mCurrentDialog = nullptr;

    delete gui;
    gui = nullptr;

    if (Net::getInventoryHandler())
        Net::getInventoryHandler()->clear();

    if (logger)
        logger->log1("Quitting3");

    delete mainGraphics;
    mainGraphics = nullptr;

    if (imageHelper != surfaceImageHelper)
        delete surfaceImageHelper;
    surfaceImageHelper = nullptr;
    delete imageHelper;
    imageHelper = nullptr;

    if (logger)
        logger->log1("Quitting4");

    XML::cleanupXML();

    if (logger)
        logger->log1("Quitting5");

    BeingInfo::clear();

    // Shutdown sound
    soundManager.close();

    if (logger)
        logger->log1("Quitting6");

    ActorSprite::unload();

    touchManager.clear();
    ResourceManager::deleteInstance();

    if (logger)
        logger->log1("Quitting8");

    MSDL_FreeSurface(mIcon);

    if (logger)
        logger->log1("Quitting9");

    delete userPalette;
    userPalette = nullptr;

    delete joystick;
    joystick = nullptr;

    keyboard.deinit();

    if (logger)
        logger->log1("Quitting10");

    soundManager.shutdown();
    touchManager.shutdown();

#ifdef DEBUG_CONFIG
    config.enableKeyLogging();
#endif
    config.removeOldKeys();
    config.write();
    serverConfig.write();

    config.clear();
    serverConfig.clear();

    if (logger)
        logger->log1("Quitting11");

#ifdef USE_PROFILER
    Perfomance::clear();
#endif

#ifdef DEBUG_OPENGL_LEAKS
    if (logger)
        logger->log("textures left: %d", textures_count);
#endif

    delete chatLogger;
    chatLogger = nullptr;
    TranslationManager::close();
}

int Client::testsExec() const
{
#ifdef USE_OPENGL
    if (mOptions.test.empty())
    {
        TestMain test;
        return test.exec();
    }
    else
    {
        TestLauncher launcher(mOptions.test);
        return launcher.exec();
    }
#else
    return 0;
#endif
}

#define ADDBUTTON(var, object) var = object; \
    x -= var->getWidth() + mButtonSpacing; \
    var->setPosition(x, mButtonPadding); \
    top->add(var);


int Client::gameExec()
{
    int lastTickTime = tick_time;

#ifdef USE_MUMBLE
    if (!mumbleManager)
        mumbleManager = new MumbleManager();
#endif

    SDL_Event event;

    if (Theme::instance())
        mSkin = Theme::instance()->load("windowmenu.xml", "");
    if (mSkin)
    {
        mButtonPadding = mSkin->getPadding();
        mButtonSpacing = mSkin->getOption("spacing", 3);
    }

    while (mState != STATE_EXIT)
    {
        PROFILER_START();
        BLOCK_START("Client::gameExec 1")
        if (mGame)
        {
            // Let the game handle the events while it is active
            mGame->handleInput();
        }
        else
        {
            BLOCK_START("Client::gameExec 2")
            // Handle SDL events
#ifdef USE_SDL2
            while (SDL_WaitEventTimeout(&event, 0))
#else
            while (SDL_PollEvent(&event))
#endif
            {
                if (mLogInput)
                    logEvent(event);
                switch (event.type)
                {
                    case SDL_QUIT:
                        mState = STATE_EXIT;
                        logger->log1("force exit");
                        break;

                    case SDL_KEYDOWN:
                        if (inputManager.handleAssignKey(
                            event, INPUT_KEYBOARD))
                        {
                            continue;
                        }
                        inputManager.updateConditionMask();
                        break;

                    case SDL_KEYUP:
                        if (inputManager.handleAssignKey(
                            event, INPUT_KEYBOARD))
                        {
                            continue;
                        }
                        inputManager.updateConditionMask();
                        break;

                    case SDL_JOYBUTTONDOWN:
                        inputManager.handleAssignKey(event, INPUT_JOYSTICK);
                        break;

                    case SDL_MOUSEMOTION:
                        break;

#ifdef ANDROID
#ifdef USE_SDL2
                    // +++ need add support
#else
                    case SDL_ACTIVEEVENT:
                        if ((event.active.state & SDL_APPACTIVE)
                            && !event.active.gain)
                        {
                            mState = STATE_EXIT;
                            logger->log1("exit on lost focus");
                        }
                        break;

                    case SDL_KEYBOARDSHOW:
                        updateScreenKeyboard(event.user.code);
                        break;

                    case SDL_ACCELEROMETER:
                        break;
#endif
#endif

                    default:
//                        logger->log("unknown event: %d", event.type);
                        break;

#ifdef USE_SDL2
                    case SDL_WINDOWEVENT:
                        handleSDL2WindowEvent(event);
                        break;
#else
#ifndef ANDROID
                    case SDL_ACTIVEEVENT:
                        handleActive(event);
                        break;
#endif
                    case SDL_VIDEORESIZE:
                        resizeVideo(event.resize.w, event.resize.h, false);
                        break;
#endif
                }

                if (inputManager.handleEvent(event))
                    continue;

#ifdef USE_MUMBLE
                if (player_node && mumbleManager)
                {
                    mumbleManager->setPos(player_node->getTileX(),
                        player_node->getTileY(), player_node->getDirection());
                }
#endif
            }
            if (mState == STATE_EXIT)
                continue;
            BLOCK_END("Client::gameExec 2")
        }

        BLOCK_START("Client::gameExec 3")
        if (Net::getGeneralHandler())
            Net::getGeneralHandler()->flushNetwork();
        BLOCK_END("Client::gameExec 3")

        BLOCK_START("Client::gameExec 4")
        if (gui)
            gui->logic();
        int k = 0;
        while (lastTickTime != tick_time && k < 40)
        {
            if (mGame)
                mGame->logic();
            else if (gui)
                gui->handleInput();

            ++lastTickTime;
            k ++;
        }
        soundManager.logic();

        logic_count += k;
        if (gui)
            gui->slowLogic();
        if (mGame)
            mGame->slowLogic();
        BLOCK_END("Client::gameExec 4")

        // This is done because at some point tick_time will wrap.
        lastTickTime = tick_time;

        // Update the screen when application is visible, delay otherwise.
        if (!mIsMinimized)
        {
            frame_count++;
            if (gui)
                gui->draw();
            mainGraphics->updateScreen();
        }
        else
        {
            SDL_Delay(100);
        }

        BLOCK_START("~Client::SDL_framerateDelay")
        if (mLimitFps)
            SDL_framerateDelay(&mFpsManager);
        BLOCK_END("~Client::SDL_framerateDelay")

        BLOCK_START("Client::gameExec 6")
        if (mState == STATE_CONNECT_GAME &&
            Net::getGameHandler()->isConnected())
        {
            Net::getLoginHandler()->disconnect();
        }
        else if (mState == STATE_CONNECT_SERVER &&
                 mOldState == STATE_CHOOSE_SERVER)
        {
            mServerName = mCurrentServer.hostname;
            initServerConfig(mCurrentServer.hostname);
            initFeatures();
            PlayerInfo::loadData();
            loginData.registerUrl = mCurrentServer.registerUrl;
            if (!mCurrentServer.onlineListUrl.empty())
                mOnlineListUrl = mCurrentServer.onlineListUrl;
            else
                mOnlineListUrl = mServerName;

            if (mOptions.username.empty())
            {
                if (loginData.remember)
                    loginData.username = serverConfig.getValue("username", "");
                else
                    loginData.username.clear();
            }
            else
            {
                loginData.username = mOptions.username;
            }

            loginData.remember = serverConfig.getValue("remember", 1);
            Net::connectToServer(mCurrentServer);

#ifdef USE_MUMBLE
            if (mumbleManager)
                mumbleManager->setServer(mCurrentServer.hostname);
#endif

            GuildManager::init();
            AuctionManager::init();

            if (!mConfigAutoSaved)
            {
                mConfigAutoSaved = true;
                config.write();
            }
        }
        else if (mState == STATE_CONNECT_SERVER &&
                 mOldState != STATE_CHOOSE_SERVER &&
                 Net::getLoginHandler()->isConnected())
        {
            mState = STATE_PRE_LOGIN;
        }
        else if (mState == STATE_WORLD_SELECT && mOldState == STATE_UPDATE)
        {
            if (Net::getLoginHandler()->getWorlds().size() < 2)
                mState = STATE_PRE_LOGIN;
        }
        else if (mOldState == STATE_START ||
                 (mOldState == STATE_GAME && mState != STATE_GAME))
        {
            if (!gui)
                break;

            gcn::Container *const top = static_cast<gcn::Container*>(
                gui->getTop());

            if (!top)
                break;

            mDesktop = new Desktop(nullptr);
            top->add(mDesktop);
            int x = top->getWidth() - mButtonPadding;
            ADDBUTTON(mSetupButton, new Button(mDesktop,
                // TRANSLATORS: setup tab quick button
                _("Setup"), "Setup", this))
#ifndef WIN32
            ADDBUTTON(mPerfomanceButton, new Button(mDesktop,
                // TRANSLATORS: perfoamance tab quick button
                _("Performance"), "Perfomance", this))
            ADDBUTTON(mVideoButton, new Button(mDesktop,
                // TRANSLATORS: video tab quick button
                _("Video"), "Video", this))
            ADDBUTTON(mThemesButton, new Button(mDesktop,
                // TRANSLATORS: theme tab quick button
                _("Theme"), "Themes", this))
#ifdef ANDROID
            ADDBUTTON(mCloseButton, new Button(mDesktop,
                // TRANSLATORS: close quick button
                _("Close"), "close", this))
#endif
#endif
            mDesktop->setSize(mainGraphics->getWidth(),
                mainGraphics->getHeight());
        }
        BLOCK_END("Client::gameExec 6")

        if (mState == STATE_SWITCH_LOGIN && mOldState == STATE_GAME)
            Net::getGameHandler()->disconnect();

        if (mState != mOldState)
        {
            BLOCK_START("Client::gameExec 7")
            PlayerInfo::stateChange(mState);

            if (mOldState == STATE_GAME)
            {
                delete mGame;
                mGame = nullptr;
                Game::clearInstance();
                ResourceManager *const resman = ResourceManager::getInstance();
                if (resman)
                    resman->cleanOrphans();
                Party::clearParties();
                Guild::clearGuilds();
                NpcDialog::clearDialogs();
                Net::getGuildHandler()->clear();
                Net::getPartyHandler()->clear();
                if (chatLogger)
                    chatLogger->clear();
            }

            mOldState = mState;

            // Get rid of the dialog of the previous state
            delete mCurrentDialog;
            mCurrentDialog = nullptr;
            // State has changed, while the quitDialog was active, it might
            // not be correct anymore
            if (mQuitDialog)
            {
                mQuitDialog->scheduleDelete();
                mQuitDialog = nullptr;
            }
            BLOCK_END("Client::gameExec 7")

            BLOCK_START("Client::gameExec 8")
            switch (mState)
            {
                case STATE_CHOOSE_SERVER:
                {
                    BLOCK_START("Client::gameExec STATE_CHOOSE_SERVER")
                    logger->log1("State: CHOOSE SERVER");
                    mCurrentServer.supportUrl.clear();
                    ResourceManager *const resman
                        = ResourceManager::getInstance();
                    if (mOptions.dataPath.empty())
                    {
                        // Add customdata directory
                        resman->searchAndRemoveArchives(
                            "customdata/",
                            "zip");
                    }

                    if (!mOldUpdates.empty())
                    {
                        UpdaterWindow::unloadUpdates(mOldUpdates);
                        mOldUpdates.clear();
                    }

                    if (!mOptions.skipUpdate)
                    {
                        resman->searchAndRemoveArchives(
                            mUpdatesDir + "/local/",
                            "zip");

                        resman->removeFromSearchPath(mLocalDataDir
                            + dirSeparator + mUpdatesDir + "/local/");
                    }

                    resman->clearCache();

                    loginData.clearUpdateHost();
                    serverVersion = 0;

                    // Allow changing this using a server choice dialog
                    // We show the dialog box only if the command-line
                    // options weren't set.
                    if (mOptions.serverName.empty() && mOptions.serverPort == 0
                        && !branding.getValue("onlineServerList", "a").empty())
                    {
                        // Don't allow an alpha opacity
                        // lower than the default value
                        Theme::instance()->setMinimumOpacity(0.8F);

                        mCurrentDialog = new ServerDialog(&mCurrentServer,
                                                          mConfigDir);
                    }
                    else
                    {
                        mState = STATE_CONNECT_SERVER;

                        // Reset options so that cancelling or connect
                        // timeout will show the server dialog.
                        mOptions.serverName.clear();
                        mOptions.serverPort = 0;
                    }
                    BLOCK_END("Client::gameExec STATE_CHOOSE_SERVER")
                    break;
                }

                case STATE_CONNECT_SERVER:
                    BLOCK_START("Client::gameExec STATE_CONNECT_SERVER")
                    logger->log1("State: CONNECT SERVER");
                    loginData.updateHosts.clear();
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Connecting to server"), STATE_SWITCH_SERVER);
                    TranslationManager::loadCurrentLang();
                    BLOCK_END("Client::gameExec STATE_CONNECT_SERVER")
                    break;

                case STATE_PRE_LOGIN:
                    logger->log1("State: PRE_LOGIN");
//                    if (serverVersion < 5)
//                        setState(STATE_LOGIN);
                    break;

                case STATE_LOGIN:
                    BLOCK_START("Client::gameExec STATE_LOGIN")
                    logger->log1("State: LOGIN");
                    // Don't allow an alpha opacity
                    // lower than the default value
                    Theme::instance()->setMinimumOpacity(0.8F);

                    loginData.updateType
                        = serverConfig.getValue("updateType", 1);

                    if (mOptions.username.empty()
                        || mOptions.password.empty())
                    {
                        mCurrentDialog = new LoginDialog(&loginData,
                            mCurrentServer.hostname, &mOptions.updateHost);
                    }
                    else
                    {
                        mState = STATE_LOGIN_ATTEMPT;
                        // Clear the password so that when login fails, the
                        // dialog will show up next time.
                        mOptions.password.clear();
                    }
                    BLOCK_END("Client::gameExec STATE_LOGIN")
                    break;

                case STATE_LOGIN_ATTEMPT:
                    BLOCK_START("Client::gameExec STATE_LOGIN_ATTEMPT")
                    logger->log1("State: LOGIN ATTEMPT");
                    accountLogin(&loginData);
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Logging in"), STATE_SWITCH_SERVER);
                    BLOCK_END("Client::gameExec STATE_LOGIN_ATTEMPT")
                    break;

                case STATE_WORLD_SELECT:
                    BLOCK_START("Client::gameExec STATE_WORLD_SELECT")
                    logger->log1("State: WORLD SELECT");
                    {
                        TranslationManager::loadCurrentLang();
                        Worlds worlds = Net::getLoginHandler()->getWorlds();

                        if (worlds.empty())
                        {
                            // Trust that the netcode knows what it's doing
                            mState = STATE_UPDATE;
                        }
                        else if (worlds.size() == 1)
                        {
                            Net::getLoginHandler()->chooseServer(0);
                            mState = STATE_UPDATE;
                        }
                        else
                        {
                            mCurrentDialog = new WorldSelectDialog(worlds);
                            if (mOptions.chooseDefault)
                            {
                                static_cast<WorldSelectDialog*>(mCurrentDialog)
                                    ->action(gcn::ActionEvent(nullptr, "ok"));
                            }
                        }
                    }
                    BLOCK_END("Client::gameExec STATE_WORLD_SELECT")
                    break;

                case STATE_WORLD_SELECT_ATTEMPT:
                    BLOCK_START("Client::gameExec STATE_WORLD_SELECT_ATTEMPT")
                    logger->log1("State: WORLD SELECT ATTEMPT");
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Entering game world"), STATE_WORLD_SELECT);
                    BLOCK_END("Client::gameExec STATE_WORLD_SELECT_ATTEMPT")
                    break;

                case STATE_UPDATE:
                    BLOCK_START("Client::gameExec STATE_UPDATE")
                    logger->log1("State: UPDATE");
                    // Determine which source to use for the update host
                    if (!mOptions.updateHost.empty())
                        mUpdateHost = mOptions.updateHost;
                    else
                        mUpdateHost = loginData.updateHost;
                    initUpdatesDir();

                    if (!mOldUpdates.empty())
                        UpdaterWindow::unloadUpdates(mOldUpdates);

                    if (mOptions.skipUpdate)
                    {
                        mState = STATE_LOAD_DATA;
                        mOldUpdates = "";
                    }
                    else if (loginData.updateType & LoginData::Upd_Skip)
                    {
                        mOldUpdates = mLocalDataDir
                            + dirSeparator + mUpdatesDir;
                        UpdaterWindow::loadLocalUpdates(mOldUpdates);
                        mState = STATE_LOAD_DATA;
                    }
                    else
                    {
                        mOldUpdates = mLocalDataDir
                            + dirSeparator + mUpdatesDir;
                        mCurrentDialog = new UpdaterWindow(mUpdateHost,
                            mOldUpdates,
                            mOptions.dataPath.empty(),
                            loginData.updateType);
                    }
                    BLOCK_END("Client::gameExec STATE_UPDATE")
                    break;

                case STATE_LOAD_DATA:
                {
                    BLOCK_START("Client::gameExec STATE_LOAD_DATA")
                    logger->log1("State: LOAD DATA");

                    const ResourceManager *const resman
                        = ResourceManager::getInstance();

                    // If another data path has been set,
                    // we don't load any other files...
                    if (mOptions.dataPath.empty())
                    {
                        // Add customdata directory
                        resman->searchAndAddArchives(
                            "customdata/",
                            "zip",
                            false);
                    }

                    if (!mOptions.skipUpdate)
                    {
                        resman->searchAndAddArchives(
                            mUpdatesDir + "/local/",
                            "zip",
                            false);

                        resman->addToSearchPath(mLocalDataDir + dirSeparator
                            + mUpdatesDir + "/local/", false);
                    }

                    logger->log("Init paths");
                    paths.init("paths.xml", true);
                    paths.setDefaultValues(getPathsDefaults());
                    if (!SpriteReference::Empty)
                    {
                        SpriteReference::Empty = new SpriteReference(
                            paths.getStringValue("spriteErrorFile"), 0);
                    }

                    if (!BeingInfo::unknown)
                        BeingInfo::unknown = new BeingInfo;

                    TranslationManager::loadCurrentLang();
                    PlayerInfo::stateChange(mState);

                    // Load XML databases
                    CharDB::load();
                    DeadDB::load();
                    PaletteDB::load();
                    ColorDB::load();
                    SoundDB::load();
                    MapDB::load();
                    ItemDB::load();
                    Being::load();
                    MonsterDB::load();
#ifdef MANASERV_SUPPORT
                    SpecialDB::load();
#endif
                    AvatarDB::load();
                    NPCDB::load();
                    PETDB::load();
                    EmoteDB::load();
                    StatusEffect::load();
                    Units::loadUnits();

                    ActorSprite::load();

                    if (mDesktop)
                        mDesktop->reloadWallpaper();

                    mState = STATE_GET_CHARACTERS;
                    BLOCK_END("Client::gameExec STATE_LOAD_DATA")
                    break;
                }
                case STATE_GET_CHARACTERS:
                    BLOCK_START("Client::gameExec STATE_GET_CHARACTERS")
                    logger->log1("State: GET CHARACTERS");
                    Net::getCharServerHandler()->requestCharacters();
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Requesting characters"),
                        STATE_SWITCH_SERVER);
                    BLOCK_END("Client::gameExec STATE_GET_CHARACTERS")
                    break;

                case STATE_CHAR_SELECT:
                    BLOCK_START("Client::gameExec STATE_CHAR_SELECT")
                    logger->log1("State: CHAR SELECT");
                    // Don't allow an alpha opacity
                    // lower than the default value
                    Theme::instance()->setMinimumOpacity(0.8F);

                    mCurrentDialog = new CharSelectDialog(&loginData);

                    if (!(static_cast<CharSelectDialog*>(mCurrentDialog))
                        ->selectByName(mOptions.character,
                        CharSelectDialog::Choose))
                    {
                        (static_cast<CharSelectDialog*>(mCurrentDialog))
                            ->selectByName(
                            serverConfig.getValue("lastCharacter", ""),
                            mOptions.chooseDefault ?
                            CharSelectDialog::Choose :
                            CharSelectDialog::Focus);
                    }

                    // Choosing character on the command line should work only
                    // once, clear it so that 'switch character' works.
                    mOptions.character.clear();
                    BLOCK_END("Client::gameExec STATE_CHAR_SELECT")
                    break;

                case STATE_CONNECT_GAME:
                    BLOCK_START("Client::gameExec STATE_CONNECT_GAME")
                    logger->log1("State: CONNECT GAME");

                    Net::getGameHandler()->connect();
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Connecting to the game server"),
                        Net::getNetworkType() != ServerInfo::MANASERV ?
                        STATE_CHOOSE_SERVER : STATE_SWITCH_CHARACTER);
                    BLOCK_END("Client::gameExec STATE_CONNECT_GAME")
                    break;

                case STATE_CHANGE_MAP:
                    BLOCK_START("Client::gameExec STATE_CHANGE_MAP")
                    logger->log1("State: CHANGE_MAP");

                    Net::getGameHandler()->connect();
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Changing game servers"),
                        STATE_SWITCH_CHARACTER);
                    BLOCK_END("Client::gameExec STATE_CHANGE_MAP")
                    break;

                case STATE_GAME:
                    BLOCK_START("Client::gameExec STATE_GAME")
                    if (player_node)
                    {
                        logger->log("Memorizing selected character %s",
                            player_node->getName().c_str());
                        serverConfig.setValue("lastCharacter",
                            player_node->getName());
#ifdef USE_MUMBLE
                        if (mumbleManager)
                            mumbleManager->setPlayer(player_node->getName());
#endif
                    }

                    // Fade out logon-music here too to give the desired effect
                    // of "flowing" into the game.
                    soundManager.fadeOutMusic(1000);

                    // Allow any alpha opacity
                    Theme::instance()->setMinimumOpacity(-1.0F);

                    if (chatLogger)
                        chatLogger->setServerName(mServerName);

#ifdef ANDROID
                    delete mCloseButton;
                    mCloseButton = nullptr;
#endif
                    delete mSetupButton;
                    mSetupButton = nullptr;
                    delete mVideoButton;
                    mVideoButton = nullptr;
                    delete mThemesButton;
                    mThemesButton = nullptr;
                    delete mPerfomanceButton;
                    mPerfomanceButton = nullptr;
                    delete mDesktop;
                    mDesktop = nullptr;

                    mCurrentDialog = nullptr;

                    logger->log1("State: GAME");
                    if (Net::getGeneralHandler())
                        Net::getGeneralHandler()->reloadPartially();
                    mGame = new Game;
                    BLOCK_END("Client::gameExec STATE_GAME")
                    break;

                case STATE_LOGIN_ERROR:
                    BLOCK_START("Client::gameExec STATE_LOGIN_ERROR")
                    logger->log1("State: LOGIN ERROR");
                    // TRANSLATORS: error dialog header
                    mCurrentDialog = new OkDialog(_("Error"),
                        errorMessage, DIALOG_ERROR);
                    mCurrentDialog->addActionListener(&loginListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    BLOCK_END("Client::gameExec STATE_LOGIN_ERROR")
                    break;

                case STATE_ACCOUNTCHANGE_ERROR:
                    BLOCK_START("Client::gameExec STATE_ACCOUNTCHANGE_ERROR")
                    logger->log1("State: ACCOUNT CHANGE ERROR");
                    // TRANSLATORS: error dialog header
                    mCurrentDialog = new OkDialog(_("Error"),
                        errorMessage, DIALOG_ERROR);
                    mCurrentDialog->addActionListener(&accountListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    BLOCK_END("Client::gameExec STATE_ACCOUNTCHANGE_ERROR")
                    break;

                case STATE_REGISTER_PREP:
                    BLOCK_START("Client::gameExec STATE_REGISTER_PREP")
                    logger->log1("State: REGISTER_PREP");
                    Net::getLoginHandler()->getRegistrationDetails();
                    mCurrentDialog = new ConnectionDialog(
                        // TRANSLATORS: connection dialog header
                        _("Requesting registration details"), STATE_LOGIN);
                    BLOCK_END("Client::gameExec STATE_REGISTER_PREP")
                    break;

                case STATE_REGISTER:
                    logger->log1("State: REGISTER");
                    mCurrentDialog = new RegisterDialog(&loginData);
                    break;

                case STATE_REGISTER_ATTEMPT:
                    BLOCK_START("Client::gameExec STATE_REGISTER_ATTEMPT")
                    logger->log("Username is %s", loginData.username.c_str());
                    Net::getLoginHandler()->registerAccount(&loginData);
                    BLOCK_END("Client::gameExec STATE_REGISTER_ATTEMPT")
                    break;

                case STATE_CHANGEPASSWORD:
                    BLOCK_START("Client::gameExec STATE_CHANGEPASSWORD")
                    logger->log1("State: CHANGE PASSWORD");
                    mCurrentDialog = new ChangePasswordDialog(&loginData);
                    BLOCK_END("Client::gameExec STATE_CHANGEPASSWORD")
                    break;

                case STATE_CHANGEPASSWORD_ATTEMPT:
                    BLOCK_START("Client::gameExec "
                        "STATE_CHANGEPASSWORD_ATTEMPT")
                    logger->log1("State: CHANGE PASSWORD ATTEMPT");
                    Net::getLoginHandler()->changePassword(loginData.username,
                        loginData.password, loginData.newPassword);
                    BLOCK_END("Client::gameExec STATE_CHANGEPASSWORD_ATTEMPT")
                    break;

                case STATE_CHANGEPASSWORD_SUCCESS:
                    BLOCK_START("Client::gameExec "
                        "STATE_CHANGEPASSWORD_SUCCESS")
                    logger->log1("State: CHANGE PASSWORD SUCCESS");
                    // TRANSLATORS: password change message header
                    mCurrentDialog = new OkDialog(_("Password Change"),
                        // TRANSLATORS: password change message text
                        _("Password changed successfully!"), DIALOG_ERROR);
                    mCurrentDialog->addActionListener(&accountListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    loginData.password = loginData.newPassword;
                    loginData.newPassword.clear();
                    BLOCK_END("Client::gameExec STATE_CHANGEPASSWORD_SUCCESS")
                    break;

                case STATE_CHANGEEMAIL:
                    logger->log1("State: CHANGE EMAIL");
                    mCurrentDialog = new ChangeEmailDialog(&loginData);
                    break;

                case STATE_CHANGEEMAIL_ATTEMPT:
                    logger->log1("State: CHANGE EMAIL ATTEMPT");
                    Net::getLoginHandler()->changeEmail(loginData.email);
                    break;

                case STATE_CHANGEEMAIL_SUCCESS:
                    logger->log1("State: CHANGE EMAIL SUCCESS");
                    // TRANSLATORS: email change message header
                    mCurrentDialog = new OkDialog(_("Email Change"),
                        // TRANSLATORS: email change message text
                        _("Email changed successfully!"), DIALOG_ERROR);
                    mCurrentDialog->addActionListener(&accountListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    break;

                case STATE_UNREGISTER:
                    logger->log1("State: UNREGISTER");
                    mCurrentDialog = new UnRegisterDialog(&loginData);
                    break;

                case STATE_UNREGISTER_ATTEMPT:
                    logger->log1("State: UNREGISTER ATTEMPT");
                    Net::getLoginHandler()->unregisterAccount(
                            loginData.username, loginData.password);
                    break;

                case STATE_UNREGISTER_SUCCESS:
                    logger->log1("State: UNREGISTER SUCCESS");
                    Net::getLoginHandler()->disconnect();

                    mCurrentDialog = openErrorDialog(
                        // TRANSLATORS: unregister message header
                        _("Unregister Successful"),
                        // TRANSLATORS: unregister message text
                        _("Farewell, come back any time..."), true);
                    loginData.clear();
                    // The errorlistener sets the state to STATE_CHOOSE_SERVER
                    mCurrentDialog->addActionListener(&errorListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    break;

                case STATE_SWITCH_SERVER:
                    BLOCK_START("Client::gameExec STATE_SWITCH_SERVER")
                    logger->log1("State: SWITCH SERVER");

                    Net::getLoginHandler()->disconnect();
                    Net::getGameHandler()->disconnect();
                    Net::getGameHandler()->clear();
                    mServerName.clear();
                    serverConfig.unload();

                    mState = STATE_CHOOSE_SERVER;
                    BLOCK_END("Client::gameExec STATE_SWITCH_SERVER")
                    break;

                case STATE_SWITCH_LOGIN:
                    BLOCK_START("Client::gameExec STATE_SWITCH_LOGIN")
                    logger->log1("State: SWITCH LOGIN");

                    Net::getLoginHandler()->logout();
                    Net::getLoginHandler()->disconnect();
                    Net::getGameHandler()->disconnect();
                    Net::getLoginHandler()->connect();

                    mState = STATE_LOGIN;
                    BLOCK_END("Client::gameExec STATE_SWITCH_LOGIN")
                    break;

                case STATE_SWITCH_CHARACTER:
                    BLOCK_START("Client::gameExec STATE_SWITCH_CHARACTER")
                    logger->log1("State: SWITCH CHARACTER");

                    // Done with game
                    Net::getGameHandler()->disconnect();

                    mState = STATE_GET_CHARACTERS;
                    BLOCK_END("Client::gameExec STATE_SWITCH_CHARACTER")
                    break;

                case STATE_LOGOUT_ATTEMPT:
                    logger->log1("State: LOGOUT ATTEMPT");
                    break;

                case STATE_WAIT:
                    logger->log1("State: WAIT");
                    break;

                case STATE_EXIT:
                    BLOCK_START("Client::gameExec STATE_EXIT")
                    logger->log1("State: EXIT");
                    Net::unload();
                    BLOCK_END("Client::gameExec STATE_EXIT")
                    break;

                case STATE_FORCE_QUIT:
                    BLOCK_START("Client::gameExec STATE_FORCE_QUIT")
                    logger->log1("State: FORCE QUIT");
                    if (Net::getGeneralHandler())
                        Net::getGeneralHandler()->unload();
                    mState = STATE_EXIT;
                    BLOCK_END("Client::gameExec STATE_FORCE_QUIT")
                  break;

                case STATE_ERROR:
                    BLOCK_START("Client::gameExec STATE_ERROR")
                    config.write();
                    if (mOldState == STATE_GAME)
                        serverConfig.write();
                    logger->log1("State: ERROR");
                    logger->log("Error: %s\n", errorMessage.c_str());
                    // TRANSLATORS: error message header
                    mCurrentDialog = openErrorDialog(_("Error"),
                        errorMessage, true);
                    mCurrentDialog->addActionListener(&errorListener);
                    mCurrentDialog = nullptr;  // OkDialog deletes itself
                    Net::getGameHandler()->disconnect();
                    BLOCK_END("Client::gameExec STATE_ERROR")
                    break;

                case STATE_AUTORECONNECT_SERVER:
                    // ++++++
                    break;

                case STATE_START:
                default:
                    mState = STATE_FORCE_QUIT;
                    break;
            }
            BLOCK_END("Client::gameExec 8")
        }
        BLOCK_END("Client::gameExec 1")
    }

    return 0;
}

void Client::optionChanged(const std::string &name)
{
    if (name == "fpslimit")
    {
        const int fpsLimit = config.getIntValue("fpslimit");
        mLimitFps = fpsLimit > 0;
        setFramerate(fpsLimit);
    }
    else if (name == "guialpha")
    {
        const float alpha = config.getFloatValue("guialpha");
        setGuiAlpha(alpha);
        ImageHelper::setEnableAlpha(alpha != 1.0F);
    }
    else if (name == "gamma" || name == "enableGamma")
    {
        applyGamma();
    }
    else if (name == "particleEmitterSkip")
    {
        Particle::emitterSkip = config.getIntValue("particleEmitterSkip") + 1;
    }
    else if (name == "vsync")
    {
        applyVSync();
    }
    else if (name == "repeateInterval" || name == "repeateDelay")
    {
        applyKeyRepeat();
    }
    else if (name == "logInput")
    {
        mLogInput = config.getBoolValue("logInput");
    }
}

void Client::action(const gcn::ActionEvent &event)
{
    std::string tab;
    const std::string &eventId = event.getId();

    if (eventId == "close")
    {
        setState(STATE_FORCE_QUIT);
        return;
    }
    if (eventId == "Setup")
        tab.clear();
    else if (eventId == "Video")
        tab = "Video";
    else if (eventId == "Themes")
        tab = "Theme";
    else if (eventId == "Perfomance")
        tab = "Perfomance";
    else
        return;

    if (setupWindow)
    {
        setupWindow->setVisible(!setupWindow->isWindowVisible());
        if (setupWindow->isWindowVisible())
        {
            if (!tab.empty())
                setupWindow->activateTab(tab);
            setupWindow->requestMoveToTop();
        }
    }
}

void Client::initRootDir()
{
    mRootDir = PhysFs::getBaseDir();
    const std::string portableName = mRootDir + "portable.xml";
    struct stat statbuf;

    if (!stat(portableName.c_str(), &statbuf) && S_ISREG(statbuf.st_mode))
    {
        std::string dir;
        Configuration portable;
        portable.init(portableName);

        logger->log("Portable file: %s", portableName.c_str());

        if (mOptions.localDataDir.empty())
        {
            dir = portable.getValue("dataDir", "");
            if (!dir.empty())
            {
                mOptions.localDataDir = mRootDir + dir;
                logger->log("Portable data dir: %s",
                    mOptions.localDataDir.c_str());
            }
        }

        if (mOptions.configDir.empty())
        {
            dir = portable.getValue("configDir", "");
            if (!dir.empty())
            {
                mOptions.configDir = mRootDir + dir;
                logger->log("Portable config dir: %s",
                    mOptions.configDir.c_str());
            }
        }

        if (mOptions.screenshotDir.empty())
        {
            dir = portable.getValue("screenshotDir", "");
            if (!dir.empty())
            {
                mOptions.screenshotDir = mRootDir + dir;
                logger->log("Portable screenshot dir: %s",
                    mOptions.screenshotDir.c_str());
            }
        }
    }
}

/**
 * Initializes the home directory. On UNIX and FreeBSD, ~/.mana is used. On
 * Windows and other systems we use the current working directory.
 */
void Client::initHomeDir()
{
    initLocalDataDir();
    initTempDir();
    initConfigDir();
}

void Client::initLocalDataDir()
{
    mLocalDataDir = mOptions.localDataDir;

    if (mLocalDataDir.empty())
    {
#ifdef __APPLE__
        // Use Application Directory instead of .mana
        mLocalDataDir = std::string(PhysFs::getUserDir()) +
            "/Library/Application Support/" +
            branding.getValue("appName", "ManaPlus");
#elif defined __HAIKU__
        mLocalDataDir = std::string(PhysFs::getUserDir()) +
           "/config/data/Mana";
#elif defined WIN32
        mLocalDataDir = getSpecialFolderLocation(CSIDL_LOCAL_APPDATA);
        if (mLocalDataDir.empty())
            mLocalDataDir = std::string(PhysFs::getUserDir());
        mLocalDataDir.append("/Mana");
#elif defined __ANDROID__
        mLocalDataDir = getSdStoragePath() + branding.getValue(
            "appShort", "ManaPlus") + "/local";
#else
        mLocalDataDir = std::string(PhysFs::getUserDir()) +
            ".local/share/mana";
#endif
    }

    if (mkdir_r(mLocalDataDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), mLocalDataDir.c_str()));
    }
#ifdef USE_PROFILER
    Perfomance::init(mLocalDataDir + "/profiler.log");
#endif
}

void Client::initTempDir()
{
    mTempDir = mLocalDataDir + dirSeparator + "temp";

    if (mkdir_r(mTempDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), mTempDir.c_str()));
    }
//    ResourceManager::deleteFilesInDirectory(mTempDir);
}

void Client::initConfigDir()
{
    mConfigDir = mOptions.configDir;

    if (mConfigDir.empty())
    {
#ifdef __APPLE__
        mConfigDir = mLocalDataDir + dirSeparator
            + branding.getValue("appShort", "mana");
#elif defined __HAIKU__
        mConfigDir = std::string(PhysFs::getUserDir()) +
           "/config/settings/Mana" +
           branding.getValue("appName", "ManaPlus");
#elif defined WIN32
        mConfigDir = getSpecialFolderLocation(CSIDL_APPDATA);
        if (mConfigDir.empty())
        {
            mConfigDir = mLocalDataDir;
        }
        else
        {
            mConfigDir.append("/mana/").append(branding.getValue(
                "appShort", "mana"));
        }
#elif defined __ANDROID__
        mConfigDir = getSdStoragePath() + branding.getValue(
            "appShort", "ManaPlus").append("/config");
#else
        mConfigDir = std::string(PhysFs::getUserDir()).append(
            "/.config/mana/").append(branding.getValue("appShort", "mana"));
#endif
        logger->log("Generating config dir: " + mConfigDir);
    }

    if (mkdir_r(mConfigDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
                                  "Exiting."), mConfigDir.c_str()));
    }
}

/**
 * Initializes the home directory. On UNIX and FreeBSD, ~/.mana is used. On
 * Windows and other systems we use the current working directory.
 */
void Client::initServerConfig(std::string serverName)
{
    mServerConfigDir = mConfigDir + dirSeparator + serverName;

    if (mkdir_r(mServerConfigDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
                                  "Exiting."), mServerConfigDir.c_str()));
    }
    const std::string configPath = mServerConfigDir + "/config.xml";
    FILE *configFile = fopen(configPath.c_str(), "r");
    if (!configFile)
    {
        configFile = fopen(configPath.c_str(), "wt");
        logger->log("Creating new server config: " + configPath);
    }
    if (configFile)
    {
        fclose(configFile);
        serverConfig.init(configPath);
        serverConfig.setDefaultValues(getConfigDefaults());
        logger->log("serverConfigPath: " + configPath);
    }
    initPacketLimiter();
    initTradeFilter();
    initUsersDir();
    player_relations.init();

    // Initialize the item and emote shortcuts.
    for (unsigned f = 0; f < SHORTCUT_TABS; f ++)
    {
        delete itemShortcut[f];
        itemShortcut[f] = new ItemShortcut(f);
    }
    delete emoteShortcut;
    emoteShortcut = new EmoteShortcut;

    // Initialize the drop shortcuts.
    delete dropShortcut;
    dropShortcut = new DropShortcut;
}

void Client::initFeatures() const
{
    features.init(paths.getStringValue("featuresFile"), true);
    features.setDefaultValues(getFeaturesDefaults());
}

void Client::initConfiguration() const
{
#ifdef DEBUG_CONFIG
    config.setIsMain(true);
#endif

    // Fill configuration with defaults
    config.setValue("hwaccel", false);
#ifdef USE_OPENGL
#if (defined __APPLE__)
    config.setValue("opengl", 1);
#elif (defined ANDROID)
    config.setValue("opengl", 3);
#elif (defined WIN32)
    config.setValue("opengl", 2);
#else
    config.setValue("opengl", 0);
#endif
#else
    config.setValue("opengl", 0);
#endif
    config.setValue("screen", false);
    config.setValue("sound", true);
    config.setValue("guialpha", 0.8F);
//    config.setValue("remember", true);
    config.setValue("sfxVolume", 100);
    config.setValue("musicVolume", 60);
    config.setValue("fpslimit", 60);
    std::string defaultUpdateHost = branding.getValue("defaultUpdateHost", "");
    if (!checkPath(defaultUpdateHost))
        defaultUpdateHost.clear();
    config.setValue("updatehost", defaultUpdateHost);
    config.setValue("customcursor", true);
    config.setValue("useScreenshotDirectorySuffix", true);
    config.setValue("ChatLogLength", 128);

    std::string configPath;

    if (mOptions.test.empty())
        configPath = mConfigDir + "/config.xml";
    else
        configPath = mConfigDir + "/test.xml";

    FILE *configFile = fopen(configPath.c_str(), "r");
    if (!configFile)
    {
        configFile = fopen(configPath.c_str(), "wt");
        logger->log1("Creating new config");
    }
    if (!configFile)
    {
        logger->log("Can't create %s. Using defaults.", configPath.c_str());
    }
    else
    {
        fclose(configFile);
        config.init(configPath);
        logger->log1("init 3");
        config.setDefaultValues(getConfigDefaults());
        logger->log("configPath: " + configPath);
    }
}

/**
 * Parse the update host and determine the updates directory
 * Then verify that the directory exists (creating if needed).
 */
void Client::initUpdatesDir()
{
    std::stringstream updates;

    // If updatesHost is currently empty, fill it from config file
    if (mUpdateHost.empty())
        mUpdateHost = config.getStringValue("updatehost");
    if (!checkPath(mUpdateHost))
        return;

    // Don't go out of range int he next check
    if (mUpdateHost.length() < 2)
        return;

    const int sz = mUpdateHost.size();
    // Remove any trailing slash at the end of the update host
    if (mUpdateHost.at(sz - 1) == '/')
        mUpdateHost.resize(sz - 1);

    // Parse out any "http://" or "https://", and set the updates directory
    const size_t pos = mUpdateHost.find("://");
    if (pos != mUpdateHost.npos)
    {
        if (pos + 3 < mUpdateHost.length() && !mUpdateHost.empty())
        {
            updates << "updates/" << mUpdateHost.substr(pos + 3);
            mUpdatesDir = updates.str();
        }
        else
        {
            logger->log("Error: Invalid update host: %s", mUpdateHost.c_str());
            // TRANSLATORS: update server initialisation error
            errorMessage = strprintf(_("Invalid update host: %s."),
                                     mUpdateHost.c_str());
            mState = STATE_ERROR;
        }
    }
    else
    {
        logger->log1("Warning: no protocol was specified for the update host");
        updates << "updates/" << mUpdateHost;
        mUpdatesDir = updates.str();
    }

#ifdef WIN32
    if (mUpdatesDir.find(":") != std::string::npos)
        replaceAll(mUpdatesDir, ":", "_");
#endif

    const ResourceManager *const resman = ResourceManager::getInstance();
    const std::string updateDir("/" + mUpdatesDir);

    // Verify that the updates directory exists. Create if necessary.
    if (!resman->isDirectory(updateDir))
    {
        if (!resman->mkdir(updateDir))
        {
#if defined WIN32
            std::string newDir = mLocalDataDir + "\\" + mUpdatesDir;
            size_t loc = newDir.find("/", 0);

            while (loc != std::string::npos)
            {
                newDir.replace(loc, 1, "\\");
                loc = newDir.find("/", loc);
            }

            if (!CreateDirectory(newDir.c_str(), nullptr) &&
                GetLastError() != ERROR_ALREADY_EXISTS)
            {
                logger->log("Error: %s can't be made, but doesn't exist!",
                            newDir.c_str());
                // TRANSLATORS: update server initialisation error
                errorMessage = _("Error creating updates directory!");
                mState = STATE_ERROR;
            }
#else
            logger->log("Error: %s/%s can't be made, but doesn't exist!",
                        mLocalDataDir.c_str(), mUpdatesDir.c_str());
            // TRANSLATORS: update server initialisation error
            errorMessage = _("Error creating updates directory!");
            mState = STATE_ERROR;
#endif
        }
    }
    const std::string updateLocal = updateDir + "/local";
    const std::string updateFix = updateDir + "/fix";
    if (!resman->isDirectory(updateLocal))
        resman->mkdir(updateLocal);
    if (!resman->isDirectory(updateFix))
        resman->mkdir(updateFix);
}

void Client::initScreenshotDir()
{
    if (!mOptions.screenshotDir.empty())
    {
        mScreenshotDir = mOptions.screenshotDir;
        if (mkdir_r(mScreenshotDir.c_str()))
        {
            // TRANSLATORS: directory creation error
            logger->log(strprintf(
                _("Error: %s doesn't exist and can't be created! "
                "Exiting."), mScreenshotDir.c_str()));
        }
    }
    else if (mScreenshotDir.empty())
    {
#ifdef __ANDROID__
        mScreenshotDir = getSdStoragePath() + std::string("/images");

        if (mkdir_r(mScreenshotDir.c_str()))
        {
            // TRANSLATORS: directory creation error
            logger->log(strprintf(
                _("Error: %s doesn't exist and can't be created! "
                "Exiting."), mScreenshotDir.c_str()));
        }
#else
        const std::string configScreenshotDir =
            config.getStringValue("screenshotDirectory");
        if (!configScreenshotDir.empty())
            mScreenshotDir = configScreenshotDir;
        else
            mScreenshotDir = getDesktopDir();
#endif

//      config.setValue("screenshotDirectory", mScreenshotDir);
        logger->log("screenshotDirectory: " + mScreenshotDir);

        if (config.getBoolValue("useScreenshotDirectorySuffix"))
        {
            const std::string configScreenshotSuffix =
                branding.getValue("screenshots", "ManaPlus");

            if (!configScreenshotSuffix.empty())
            {
                mScreenshotDir.append(dirSeparator).append(
                    configScreenshotSuffix);
            }
        }
    }
}

void Client::accountLogin(LoginData *const data) const
{
    if (!data)
        return;

    logger->log("Username is %s", data->username.c_str());

    // Send login infos
    if (data->registerLogin)
        Net::getLoginHandler()->registerAccount(data);
    else
        Net::getLoginHandler()->loginAccount(data);

    // Clear the password, avoids auto login when returning to login
    data->password.clear();

    const bool remember = data->remember;
    if (remember)
        serverConfig.setValue("username", data->username);
    else
        serverConfig.setValue("username", "");
    serverConfig.setValue("remember", remember);
}

void Client::storeSafeParameters() const
{
    bool tmpHwaccel;
    RenderType tmpOpengl;
    int tmpFpslimit;
    int tmpAltFpslimit;
    bool tmpSound;
    int width;
    int height;
    std::string font;
    std::string bFont;
    std::string particleFont;
    std::string helpFont;
    std::string secureFont;
    std::string japanFont;
    bool showBackground;
    bool enableMumble;
    bool enableMapReduce;

    isSafeMode = config.getBoolValue("safemode");
    if (isSafeMode)
        logger->log1("Run in safe mode");

    tmpOpengl = intToRenderType(config.getIntValue("opengl"));

    width = config.getIntValue("screenwidth");
    height = config.getIntValue("screenheight");
    tmpHwaccel = config.getBoolValue("hwaccel");

    tmpFpslimit = config.getIntValue("fpslimit");
    tmpAltFpslimit = config.getIntValue("altfpslimit");
    tmpSound = config.getBoolValue("sound");

    font = config.getStringValue("font");
    bFont = config.getStringValue("boldFont");
    particleFont = config.getStringValue("particleFont");
    helpFont = config.getStringValue("helpFont");
    secureFont = config.getStringValue("secureFont");
    japanFont = config.getStringValue("japanFont");

    showBackground = config.getBoolValue("showBackground");
    enableMumble = config.getBoolValue("enableMumble");
    enableMapReduce = config.getBoolValue("enableMapReduce");

    if (!mOptions.safeMode && !tmpOpengl)
    {
        // if video mode configured reset most settings to safe
        config.setValue("hwaccel", false);

#if defined(ANDROID)
        config.setValue("opengl", 2);
#elif defined(__APPLE__)
        config.setValue("opengl", 1);
#else
        config.setValue("opengl", 0);
#endif
        config.setValue("altfpslimit", 3);
        config.setValue("sound", false);
        config.setValue("safemode", true);
#ifdef ANDROID
        config.setValue("screenwidth", 0);
        config.setValue("screenheight", 0);
#else
        config.setValue("screenwidth", 640);
        config.setValue("screenheight", 480);
#endif
        config.setValue("font", "fonts/dejavusans.ttf");
        config.setValue("boldFont", "fonts/dejavusans-bold.ttf");
        config.setValue("particleFont", "fonts/dejavusans.ttf");
        config.setValue("helpFont", "fonts/dejavusansmono.ttf");
        config.setValue("secureFont", "fonts/dejavusansmono.ttf");
        config.setValue("japanFont", "fonts/mplus-1p-regular.ttf");
        config.setValue("showBackground", false);
        config.setValue("enableMumble", false);
        config.setValue("enableMapReduce", false);
    }
    else
    {
        // if video mode not configured reset only video mode to safe
        config.setValue("opengl", 0);
#ifdef ANDROID
        config.setValue("screenwidth", 0);
        config.setValue("screenheight", 0);
#else
        config.setValue("screenwidth", 640);
        config.setValue("screenheight", 480);
#endif
    }

    config.write();

    if (mOptions.safeMode)
    {
        isSafeMode = true;
        return;
    }

    config.setValue("safemode", false);
    if (!tmpOpengl)
    {
        config.setValue("hwaccel", tmpHwaccel);
        config.setValue("opengl", static_cast<int>(tmpOpengl));
        config.setValue("fpslimit", tmpFpslimit);
        config.setValue("altfpslimit", tmpAltFpslimit);
        config.setValue("sound", tmpSound);
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
        config.setValue("font", font);
        config.setValue("boldFont", bFont);
        config.setValue("particleFont", particleFont);
        config.setValue("helpFont", helpFont);
        config.setValue("secureFont", secureFont);
        config.setValue("japanFont", japanFont);
        config.setValue("showBackground", showBackground);
        config.setValue("enableMumble", enableMumble);
        config.setValue("enableMapReduce", enableMapReduce);
    }
    else
    {
        config.setValue("opengl", static_cast<int>(tmpOpengl));
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
}

void Client::initTradeFilter() const
{
    const std::string tradeListName =
        getServerConfigDirectory() + "/tradefilter.txt";

    std::ofstream tradeFile;
    struct stat statbuf;

    if (stat(tradeListName.c_str(), &statbuf) || !S_ISREG(statbuf.st_mode))
    {
        tradeFile.open(tradeListName.c_str(), std::ios::out);
        if (tradeFile.is_open())
        {
            tradeFile << ": sell" << std::endl;
            tradeFile << ": buy" << std::endl;
            tradeFile << ": trade" << std::endl;
            tradeFile << "i sell" << std::endl;
            tradeFile << "i buy" << std::endl;
            tradeFile << "i trade" << std::endl;
            tradeFile << "i trading" << std::endl;
            tradeFile << "i am buy" << std::endl;
            tradeFile << "i am sell" << std::endl;
            tradeFile << "i am trade" << std::endl;
            tradeFile << "i am trading" << std::endl;
            tradeFile << "i'm buy" << std::endl;
            tradeFile << "i'm sell" << std::endl;
            tradeFile << "i'm trade" << std::endl;
            tradeFile << "i'm trading" << std::endl;
        }
        tradeFile.close();
    }
}

void Client::initUsersDir()
{
    mUsersDir = getServerConfigDirectory() + "/users/";
    if (mkdir_r(mUsersDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), mUsersDir.c_str()));
    }

    mNpcsDir = getServerConfigDirectory() + "/npcs/";
    if (mkdir_r(mNpcsDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), mNpcsDir.c_str()));
    }
}

void Client::initPacketLimiter()
{
    // here i setting packet limits. but current server is broken,
    // and this limits may not help.

    mPacketLimits[PACKET_CHAT].timeLimit = 10 + 5;
    mPacketLimits[PACKET_CHAT].lastTime = 0;
    mPacketLimits[PACKET_CHAT].cntLimit = 1;
    mPacketLimits[PACKET_CHAT].cnt = 0;

    // 10
    mPacketLimits[PACKET_PICKUP].timeLimit = 10 + 5;
    mPacketLimits[PACKET_PICKUP].lastTime = 0;
    mPacketLimits[PACKET_PICKUP].cntLimit = 1;
    mPacketLimits[PACKET_PICKUP].cnt = 0;

    // 10 5
    mPacketLimits[PACKET_DROP].timeLimit = 5;
    mPacketLimits[PACKET_DROP].lastTime = 0;
    mPacketLimits[PACKET_DROP].cntLimit = 1;
    mPacketLimits[PACKET_DROP].cnt = 0;

    // 100
    mPacketLimits[PACKET_NPC_NEXT].timeLimit = 0;
    mPacketLimits[PACKET_NPC_NEXT].lastTime = 0;
    mPacketLimits[PACKET_NPC_NEXT].cntLimit = 1;
    mPacketLimits[PACKET_NPC_NEXT].cnt = 0;

    mPacketLimits[PACKET_NPC_INPUT].timeLimit = 100;
    mPacketLimits[PACKET_NPC_INPUT].lastTime = 0;
    mPacketLimits[PACKET_NPC_INPUT].cntLimit = 1;
    mPacketLimits[PACKET_NPC_INPUT].cnt = 0;

    // 50
    mPacketLimits[PACKET_NPC_TALK].timeLimit = 60;
    mPacketLimits[PACKET_NPC_TALK].lastTime = 0;
    mPacketLimits[PACKET_NPC_TALK].cntLimit = 1;
    mPacketLimits[PACKET_NPC_TALK].cnt = 0;

    // 10
    mPacketLimits[PACKET_EMOTE].timeLimit = 10 + 5;
    mPacketLimits[PACKET_EMOTE].lastTime = 0;
    mPacketLimits[PACKET_EMOTE].cntLimit = 1;
    mPacketLimits[PACKET_EMOTE].cnt = 0;

    // 100
    mPacketLimits[PACKET_SIT].timeLimit = 100;
    mPacketLimits[PACKET_SIT].lastTime = 0;
    mPacketLimits[PACKET_SIT].cntLimit = 1;
    mPacketLimits[PACKET_SIT].cnt = 0;

    mPacketLimits[PACKET_DIRECTION].timeLimit = 50;
    mPacketLimits[PACKET_DIRECTION].lastTime = 0;
    mPacketLimits[PACKET_DIRECTION].cntLimit = 1;
    mPacketLimits[PACKET_DIRECTION].cnt = 0;

    // 2+
    mPacketLimits[PACKET_ATTACK].timeLimit = 2 + 10;
    mPacketLimits[PACKET_ATTACK].lastTime = 0;
    mPacketLimits[PACKET_ATTACK].cntLimit = 1;
    mPacketLimits[PACKET_ATTACK].cnt = 0;

    mPacketLimits[PACKET_STOPATTACK].timeLimit = 2 + 10;
    mPacketLimits[PACKET_STOPATTACK].lastTime = 0;
    mPacketLimits[PACKET_STOPATTACK].cntLimit = 1;
    mPacketLimits[PACKET_STOPATTACK].cnt = 0;

    mPacketLimits[PACKET_ONLINELIST].timeLimit = 1800;
    mPacketLimits[PACKET_ONLINELIST].lastTime = 0;
    mPacketLimits[PACKET_ONLINELIST].cntLimit = 1;
    mPacketLimits[PACKET_ONLINELIST].cnt = 0;

    // 300ms + 50 fix
    mPacketLimits[PACKET_WHISPER].timeLimit = 30 + 5;
    mPacketLimits[PACKET_WHISPER].lastTime = 0;
    mPacketLimits[PACKET_WHISPER].cntLimit = 1;
    mPacketLimits[PACKET_WHISPER].cnt = 0;

    if (!mServerConfigDir.empty())
    {
        const std::string packetLimitsName = getServerConfigDirectory()
            + "/packetlimiter.txt";

        std::ifstream inPacketFile;
        struct stat statbuf;

        if (stat(packetLimitsName.c_str(), &statbuf)
            || !S_ISREG(statbuf.st_mode))
        {
            // wtiting new file
            writePacketLimits(packetLimitsName);
        }
        else
        {   // reading existent file
            inPacketFile.open(packetLimitsName.c_str(), std::ios::in);
            char line[101];

            if (!inPacketFile.is_open() || !inPacketFile.getline(line, 100))
            {
                inPacketFile.close();
                return;
            }

            const int ver = atoi(line);

            for (int f = 0; f < PACKET_SIZE; f ++)
            {
                if (!inPacketFile.getline(line, 100))
                    break;

                if (!(ver == 1 && (f == PACKET_DROP || f == PACKET_NPC_NEXT)))
                    mPacketLimits[f].timeLimit = atoi(line);
            }
            inPacketFile.close();
            if (ver < 5)
                writePacketLimits(packetLimitsName);
        }
    }
}

void Client::writePacketLimits(const std::string &packetLimitsName) const
{
    std::ofstream outPacketFile;
    outPacketFile.open(packetLimitsName.c_str(), std::ios::out);
    if (!outPacketFile.is_open())
    {
        outPacketFile.close();
        return;
    }
    outPacketFile << "4" << std::endl;
    for (int f = 0; f < PACKET_SIZE; f ++)
    {
        outPacketFile << toString(mPacketLimits[f].timeLimit)
                      << std::endl;
    }

    outPacketFile.close();
}

bool Client::checkPackets(const int type) const
{
    if (type > PACKET_SIZE)
        return false;

    if (!serverConfig.getValueBool("enableBuggyServers", true))
        return true;

    const PacketLimit &limit = mPacketLimits[type];
    const int timeLimit = limit.timeLimit;

    if (!timeLimit)
        return true;

    const int time = tick_time;
    const int lastTime = limit.lastTime;
    const int cnt = limit.cnt;
    const int cntLimit = limit.cntLimit;

    if (lastTime > tick_time)
    {
//        instance()->mPacketLimits[type].lastTime = time;
//        instance()->mPacketLimits[type].cnt = 0;

        return true;
    }
    else if (lastTime + timeLimit > time)
    {
        if (cnt >= cntLimit)
        {
            return false;
        }
        else
        {
//            instance()->mPacketLimits[type].cnt ++;
            return true;
        }
    }
//    instance()->mPacketLimits[type].lastTime = time;
//    instance()->mPacketLimits[type].cnt = 1;
    return true;
}

bool Client::limitPackets(const int type)
{
    if (type < 0 || type > PACKET_SIZE)
        return false;

    if (!serverConfig.getValueBool("enableBuggyServers", true))
        return true;

    PacketLimit &pack = mPacketLimits[type];
    const int timeLimit = pack.timeLimit;

    if (!timeLimit)
        return true;

    const int time = tick_time;
    const int lastTime = pack.lastTime;
    const int cnt = pack.cnt;
    const int cntLimit = pack.cntLimit;

    if (lastTime > tick_time)
    {
        pack.lastTime = time;
        pack.cnt = 0;
        return true;
    }
    else if (lastTime + timeLimit > time)
    {
        if (cnt >= cntLimit)
        {
            return false;
        }
        else
        {
            pack.cnt ++;
            return true;
        }
    }
    pack.lastTime = time;
    pack.cnt = 1;
    return true;
}

void Client::setFramerate(const int fpsLimit)
{
    if (!fpsLimit)
        return;

    if (!mLimitFps)
        return;

    SDL_setFramerate(&client->mFpsManager, fpsLimit);
}

int Client::getFramerate() const
{
    if (!mLimitFps)
        return 0;

    return SDL_getFramerate(&client->mFpsManager);
}

void Client::closeDialogs()
{
    NpcDialog::clearDialogs();
    BuyDialog::closeAll();
    BuySellDialog::closeAll();
    NpcDialog::closeAll();
    SellDialog::closeAll();
}

bool Client::isTmw() const
{
    const std::string &name = getServerName();
    if (name == "server.themanaworld.org"
        || name == "themanaworld.org"
        || name == "192.31.187.185")
    {
        return true;
    }
    return false;
}

void Client::resizeVideo(int width, int height, const bool always)
{
    // Keep a minimum size. This isn't adhered to by the actual window, but
    // it keeps some window positions from getting messed up.
    width = std::max(470, width);
    height = std::max(320, height);

    if (!mainGraphics)
        return;
    if (!always && mainGraphics->mWidth == width
        && mainGraphics->mHeight == height)
    {
        return;
    }

    touchManager.resize(width, height);

    if (mainGraphics->resizeScreen(width, height))
    {
        if (gui)
            gui->videoResized();

        if (mDesktop)
            mDesktop->setSize(width, height);

        if (mSetupButton)
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
#ifdef ANDROID
            x -= mCloseButton->getWidth() + mButtonSpacing;
            mCloseButton->setPosition(x, mButtonPadding);
#endif
#endif
        }

        if (mGame)
            mGame->videoResized(width, height);

        if (gui)
            gui->draw();

        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
}

void Client::applyGrabMode()
{
    SDL::grabInput(mainGraphics->getWindow(),
        config.getBoolValue("grabinput"));
}

void Client::applyGamma()
{
    if (config.getFloatValue("enableGamma"))
    {
        SDL::setGamma(mainGraphics->getWindow(),
            config.getFloatValue("gamma"));
    }
}

void Client::applyVSync()
{
    const int val = config.getIntValue("vsync");
    if (val > 0 && val < 2)
        SDL::setVsync(val);
}

void Client::applyKeyRepeat()
{
#ifndef USE_SDL2
    SDL_EnableKeyRepeat(config.getIntValue("repeateDelay"),
        config.getIntValue("repeateInterval"));
#endif
}

void Client::setIsMinimized(const bool n)
{
    mIsMinimized = n;
    if (!n && mNewMessageFlag)
    {
        mNewMessageFlag = false;
        SDL::SetWindowTitle(mainGraphics->getWindow(), mCaption.c_str());
    }
}

void Client::newChatMessage()
{
    if (!mNewMessageFlag && mIsMinimized)
    {
        // show * on window caption
        SDL::SetWindowTitle(mainGraphics->getWindow(),
            ("*" + client->mCaption).c_str());
        mNewMessageFlag = true;
    }
}

void Client::logVars()
{
#ifdef ANDROID
    logger->log("APPDIR: %s", getenv("APPDIR"));
    logger->log("DATADIR2: %s", getSdStoragePath().c_str());
#endif
}

void Client::logEvent(const SDL_Event &event)
{
    switch (event.type)
    {
        case SDL_MOUSEMOTION:
            logger->log("event: SDL_MOUSEMOTION: %d,%d,%d",
                event.motion.state, event.motion.x, event.motion.y);
            break;
#ifdef USE_SDL2
#define winEventLog(name, name2) \
    case name: \
        str = name2; \
        break

        case SDL_FINGERDOWN:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            logger->log("event: SDL_FINGERDOWN: %ld,%ld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId, touch.x, touch.y,
                touch.dx, touch.dy);
            break;
        }
        case SDL_FINGERUP:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            logger->log("event: SDL_FINGERUP: %ld,%ld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId, touch.x, touch.y,
                touch.dx, touch.dy);
            break;
        }
        case SDL_FINGERMOTION:
        {
            const SDL_TouchFingerEvent &touch = event.tfinger;
            logger->log("event: SDL_FINGERMOTION: %ld,%ld (%f,%f) (%f,%f)",
                touch.touchId, touch.fingerId, touch.x, touch.y,
                touch.dx, touch.dy);
            break;
        }
        case SDL_KEYDOWN:
            logger->log("event: SDL_KEYDOWN: %d,%d", event.key.state,
                event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            logger->log("event: SDL_KEYUP: %d,%d", event.key.state,
                event.key.keysym.scancode);
            break;
        case SDL_WINDOWEVENT:
        {
            const int data1 = event.window.data1;
            const int data2 = event.window.data2;
            std::string str;
            switch (event.window.event)
            {
                winEventLog(SDL_WINDOWEVENT_NONE, "SDL_WINDOWEVENT_NONE");
                winEventLog(SDL_WINDOWEVENT_SHOWN, "SDL_WINDOWEVENT_SHOWN");
                winEventLog(SDL_WINDOWEVENT_HIDDEN, "SDL_WINDOWEVENT_HIDDEN");
                winEventLog(SDL_WINDOWEVENT_EXPOSED,
                    "SDL_WINDOWEVENT_EXPOSED");
                winEventLog(SDL_WINDOWEVENT_MOVED, "SDL_WINDOWEVENT_MOVED");
                winEventLog(SDL_WINDOWEVENT_RESIZED,
                    "SDL_WINDOWEVENT_RESIZED");
                winEventLog(SDL_WINDOWEVENT_SIZE_CHANGED,
                    "SDL_WINDOWEVENT_SIZE_CHANGED");
                winEventLog(SDL_WINDOWEVENT_MINIMIZED,
                    "SDL_WINDOWEVENT_MINIMIZED");
                winEventLog(SDL_WINDOWEVENT_MAXIMIZED,
                    "SDL_WINDOWEVENT_MAXIMIZED");
                winEventLog(SDL_WINDOWEVENT_RESTORED,
                    "SDL_WINDOWEVENT_RESTORED");
                winEventLog(SDL_WINDOWEVENT_ENTER, "SDL_WINDOWEVENT_ENTER");
                winEventLog(SDL_WINDOWEVENT_LEAVE, "SDL_WINDOWEVENT_LEAVE");
                winEventLog(SDL_WINDOWEVENT_FOCUS_GAINED,
                    "SDL_WINDOWEVENT_FOCUS_GAINED");
                winEventLog(SDL_WINDOWEVENT_FOCUS_LOST,
                    "SDL_WINDOWEVENT_FOCUS_LOST");
                winEventLog(SDL_WINDOWEVENT_CLOSE, "SDL_WINDOWEVENT_CLOSE");
                default:
                    str = "unknown";
                    break;
            }
            logger->log("event: SDL_WINDOWEVENT: %s: %d,%d",
                str.c_str(), data1, data2);
            break;
        }
        case SDL_TEXTINPUT:
            logger->log("event: SDL_TEXTINPUT: %s", event.text.text);
            break;
        case SDL_APP_TERMINATING:
            logger->log("SDL_APP_TERMINATING");
            break;
        case SDL_APP_LOWMEMORY:
            logger->log("SDL_APP_LOWMEMORY");
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            logger->log("SDL_APP_WILLENTERBACKGROUND");
            break;
        case SDL_APP_WILLENTERFOREGROUND:
            logger->log("SDL_APP_WILLENTERFOREGROUND");
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            logger->log("SDL_APP_DIDENTERFOREGROUND");
            break;
#else
        case SDL_KEYDOWN:
            logger->log("event: SDL_KEYDOWN: %d,%d,%d", event.key.state,
                event.key.keysym.scancode, event.key.keysym.unicode);
            break;
        case SDL_KEYUP:
            logger->log("event: SDL_KEYUP: %d,%d,%d", event.key.state,
                event.key.keysym.scancode, event.key.keysym.unicode);
            break;
        case SDL_VIDEORESIZE:
            logger->log("event: SDL_VIDEORESIZE");
            break;
        case SDL_VIDEOEXPOSE:
            logger->log("event: SDL_VIDEOEXPOSE");
            break;
        case SDL_ACTIVEEVENT:
            logger->log("event: SDL_ACTIVEEVENT: %d %d",
                event.active.state, event.active.gain);
            break;
#endif
        case SDL_MOUSEBUTTONDOWN:
            logger->log("event: SDL_MOUSEBUTTONDOWN: %d,%d,%d,%d",
                event.button.button, event.button.state,
            event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            logger->log("event: SDL_MOUSEBUTTONUP: %d,%d,%d,%d",
                event.button.button, event.button.state,
            event.button.x, event.button.y);
            break;
        case SDL_JOYAXISMOTION:
            logger->log("event: SDL_JOYAXISMOTION: %d,%d,%d",
                event.jaxis.which, event.jaxis.axis, event.jaxis.value);
            break;
        case SDL_JOYBALLMOTION:
            logger->log("event: SDL_JOYBALLMOTION: %d,%d,%d,%d",
                event.jball.which, event.jball.ball,
                event.jball.xrel, event.jball.yrel);
            break;
        case SDL_JOYHATMOTION:
            logger->log("event: SDL_JOYHATMOTION: %d,%d,%d", event.jhat.which,
                event.jhat.hat, event.jhat.value);
            break;
        case SDL_JOYBUTTONDOWN:
            logger->log("event: SDL_JOYBUTTONDOWN: %d,%d,%d",
                event.jbutton.which, event.jbutton.button,
                event.jbutton.state);
            break;
        case SDL_JOYBUTTONUP:
            logger->log("event: SDL_JOYBUTTONUP: %d,%d,%d",
                event.jbutton.which, event.jbutton.button,
                event.jbutton.state);
            break;
        case SDL_QUIT:
            logger->log("event: SDL_QUIT");
            break;
        case SDL_SYSWMEVENT:
            logger->log("event: SDL_SYSWMEVENT");
            break;
        case SDL_USEREVENT:
            logger->log("event: SDL_USEREVENT");
            break;
#ifdef ANDROID
#ifndef USE_SDL2
        case SDL_ACCELEROMETER:
            logger->log("event: SDL_ACCELEROMETER");
            break;
#endif
#endif
        default:
            logger->log("event: other: %d", event.type);
            break;
    };
}

void Client::windowRemoved(const Window *const window)
{
    if (mCurrentDialog == window)
        mCurrentDialog = nullptr;
}

void Client::checkConfigVersion()
{
    const int version = config.getIntValue("cfgver");
    if (version < 1)
    {
        if (config.getIntValue("fontSize") == 11)
            config.deleteKey("fontSize");
        if (config.getIntValue("npcfontSize") == 13)
            config.deleteKey("npcfontSize");
    }
    if (version < 2)
    {
        if (config.getIntValue("screenButtonsSize") == 1)
            config.deleteKey("screenButtonsSize");
        if (config.getIntValue("screenJoystickSize") == 1)
            config.deleteKey("screenJoystickSize");
    }
    if (version < 3)
    {
        config.setValue("audioFrequency", 44100);
#ifdef ANDROID
        config.setValue("customcursor", false);
#endif
    }

    config.setValue("cfgver", 3);
}

Window *Client::openErrorDialog(const std::string &header,
                                const std::string &message,
                                const bool modal)
{
    if (getSupportUrl().empty())
    {
        return new OkDialog(header, message, DIALOG_ERROR, modal);
    }
    else
    {
        return new ConfirmDialog(header, strprintf("%s %s", message.c_str(),
            // TRANSLATORS: error message question
            _("Do you want to open support page?")),
            SOUND_ERROR, false, modal);
    }
}

void Client::setIcon()
{
    std::string iconFile = branding.getValue("appIcon", "icons/manaplus");
#ifdef WIN32
    iconFile.append(".ico");
#else
    iconFile.append(".png");
#endif
    iconFile = ResourceManager::getInstance()->getPath(iconFile);
    logger->log("Loading icon from file: %s", iconFile.c_str());

#ifdef WIN32
    static SDL_SysWMinfo pInfo;
    SDL::getWindowWMInfo(mainGraphics->getWindow(), &pInfo);
    // Attempt to load icon from .ico file
    HICON icon = (HICON) LoadImage(nullptr, iconFile.c_str(),
        IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
    // If it's failing, we load the default resource file.
    if (!icon)
        icon = LoadIcon(GetModuleHandle(nullptr), "A");

    if (icon)
        SetClassLong(pInfo.window, GCL_HICON, reinterpret_cast<LONG>(icon));
#else
    mIcon = MIMG_Load(iconFile.c_str());
    if (mIcon)
    {
#ifdef USE_SDL2
        SDL_SetSurfaceAlphaMod(mIcon, SDL_ALPHA_OPAQUE);
#else
        SDL_SetAlpha(mIcon, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
#endif
        SDL::SetWindowIcon(mainGraphics->getWindow(), mIcon);
    }
#endif
}

bool Client::isKeyboardVisible() const
{
#ifdef USE_SDL2
    return SDL_IsTextInputActive();
#else
    return mKeyboardHeight > 1;
#endif
}

#ifdef USE_SDL2
void Client::handleSDL2WindowEvent(const SDL_Event &event)
{
    switch (event.window.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
            resizeVideo(event.window.data1, event.window.data2, false);
            break;
        case SDL_WINDOWEVENT_ENTER:
            setMouseFocused(true);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            setMouseFocused(false);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            setInputFocused(true);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            setInputFocused(false);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            setIsMinimized(true);
            break;
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_MAXIMIZED:
            setIsMinimized(false);
            break;
        default:
            break;
    }
}
#else
void Client::handleActive(const SDL_Event &event)
{
    if (event.active.state & SDL_APPACTIVE)
    {
        if (event.active.gain)
        {   // window restore
            setIsMinimized(false);
            setPriority(true);
        }
        else
        {   // window minimization
#if defined(ANDROID) && !defined(USE_SDL2)
            setState(STATE_EXIT);
#else
            setIsMinimized(true);
            setPriority(false);
#endif
        }
    }

    if (event.active.state & SDL_APPINPUTFOCUS)
        setInputFocused(event.active.gain);
    if (event.active.state & SDL_APPMOUSEFOCUS)
        setMouseFocused(event.active.gain);
}
#endif

#ifdef ANDROID
#ifdef USE_SDL2
void Client::extractAssets()
{
    if (!getenv("APPDIR"))
    {
        logger->log("error: APPDIR is not set!");
        return;
    }
    const std::string fileName = std::string(getenv(
        "APPDIR")).append("/data.zip");
    logger->log("Extracting asset into: " + fileName);
    uint8_t *buf = new uint8_t[1000000];

    FILE *const file = fopen(fileName.c_str(), "w");
    for (int f = 0; f < 100; f ++)
    {
        std::string part = strprintf("manaplus-data.zip%u%u",
            static_cast<unsigned int>(f / 10),
            static_cast<unsigned int>(f % 10));
        logger->log("testing asset: " + part);
        SDL_RWops *const rw = SDL_RWFromFile(part.c_str(), "r");
        if (rw)
        {
            const int size = SDL_RWsize(rw);
            int size2 = SDL_RWread(rw, buf, 1, size);
            logger->log("asset size: %d", size2);
            fwrite(buf, 1, size2, file);
            SDL_RWclose(rw);
        }
        else
        {
            break;
        }
    }
    fclose(file);

    const std::string fileName2 = std::string(getenv(
        "APPDIR")).append("/locale.zip");
    FILE *const file2 = fopen(fileName2.c_str(), "w");
    SDL_RWops *const rw = SDL_RWFromFile("manaplus-locale.zip", "r");
    if (rw)
    {
        const int size = SDL_RWsize(rw);
        int size2 = SDL_RWread(rw, buf, 1, size);
        fwrite(buf, 1, size2, file2);
        SDL_RWclose(rw);
    }
    fclose(file2);

    delete [] buf;
}
#endif
#endif
