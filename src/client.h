/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef CLIENT_H
#define CLIENT_H

#include "configlistener.h"

#include "net/serverinfo.h"

#include <guichan/actionlistener.hpp>

#include <SDL.h>

#ifdef USE_SDL2
#include <SDL2_framerate.h>
#else
#include <SDL_framerate.h>
#endif

#include <string>

#include <sys/time.h>

#include "localconsts.h"

class Button;
class Desktop;
class Game;
class LoginData;
class Skin;
class Window;
class QuitDialog;

extern bool isSafeMode;
extern int serverVersion;
extern unsigned int tmwServerVersion;
extern int start_time;
extern int textures_count;

class ErrorListener : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &event) override final;
};

extern std::string errorMessage;
extern ErrorListener errorListener;
extern LoginData loginData;

/**
 * All client states.
 */
enum State
{
    STATE_ERROR = -1,
    STATE_START = 0,
    STATE_CHOOSE_SERVER,
    STATE_CONNECT_SERVER,
    STATE_PRE_LOGIN,
    STATE_LOGIN,
    STATE_LOGIN_ATTEMPT,
    STATE_WORLD_SELECT,            // 5
    STATE_WORLD_SELECT_ATTEMPT,
    STATE_UPDATE,
    STATE_LOAD_DATA,
    STATE_GET_CHARACTERS,
    STATE_CHAR_SELECT,             // 10
    STATE_CONNECT_GAME,
    STATE_GAME,
    STATE_CHANGE_MAP,              // Switch map-server/gameserver
    STATE_LOGIN_ERROR,
    STATE_ACCOUNTCHANGE_ERROR,     // 15
    STATE_REGISTER_PREP,
    STATE_REGISTER,
    STATE_REGISTER_ATTEMPT,
    STATE_CHANGEPASSWORD,
    STATE_CHANGEPASSWORD_ATTEMPT,  // 20
    STATE_CHANGEPASSWORD_SUCCESS,
    STATE_CHANGEEMAIL,
    STATE_CHANGEEMAIL_ATTEMPT,
    STATE_CHANGEEMAIL_SUCCESS,
    STATE_UNREGISTER,              // 25
    STATE_UNREGISTER_ATTEMPT,
    STATE_UNREGISTER_SUCCESS,
    STATE_SWITCH_SERVER,
    STATE_SWITCH_LOGIN,
    STATE_SWITCH_CHARACTER,        // 30
    STATE_LOGOUT_ATTEMPT,
    STATE_WAIT,
    STATE_EXIT,
    STATE_FORCE_QUIT,
    STATE_AUTORECONNECT_SERVER = 1000
};

enum PacketTypes
{
    PACKET_CHAT = 0,
    PACKET_PICKUP = 1,
    PACKET_DROP = 2,
    PACKET_NPC_NEXT = 3,
    PACKET_NPC_TALK = 4,
    PACKET_NPC_INPUT = 5,
    PACKET_EMOTE = 6,
    PACKET_SIT = 7,
    PACKET_DIRECTION = 8,
    PACKET_ATTACK = 9,
    PACKET_STOPATTACK = 10,
    PACKET_ONLINELIST = 11,
    PACKET_WHISPER = 12,
    PACKET_SIZE
};

struct PacketLimit
{
    int lastTime;
    int timeLimit;
    int cnt;
    int cntLimit;
};

/**
 * The core part of the client. This class initializes all subsystems, runs
 * the event loop, and shuts everything down again.
 */
class Client final : public ConfigListener, public gcn::ActionListener
{
public:
    /**
     * A structure holding the values of various options that can be passed
     * from the command line.
     */
    struct Options
    {
        Options() :
            username(),
            password(),
            character(),
            brandingPath(),
            updateHost(),
            dataPath(),
            homeDir(),
            logFileName(),
            chatLogDir(),
            configDir(),
            localDataDir(),
            screenshotDir(),
            test(),
            serverName(),
            serverPort(0),
            printHelp(false),
            printVersion(false),
            skipUpdate(false),
            chooseDefault(false),
            noOpenGL(false),
            safeMode(false),
            testMode(false)
        {}

        std::string username;
        std::string password;
        std::string character;
        std::string brandingPath;
        std::string updateHost;
        std::string dataPath;
        std::string homeDir;
        std::string logFileName;
        std::string chatLogDir;
        std::string configDir;
        std::string localDataDir;
        std::string screenshotDir;
        std::string test;
        std::string serverName;
        uint16_t serverPort;
        bool printHelp;
        bool printVersion;
        bool skipUpdate;
        bool chooseDefault;
        bool noOpenGL;
        bool safeMode;
        bool testMode;
    };

    explicit Client(const Options &options);

    A_DELETE_COPY(Client)

    ~Client();

    void gameInit();

    void testsInit();

    int gameExec();

    int testsExec() const;

    void setState(const State state)
    { mState = state; }

    State getState() const A_WARN_UNUSED
    { return mState; }

    const std::string &getConfigDirectory() const A_WARN_UNUSED
    { return mConfigDir; }

    const std::string &getLocalDataDirectory() const A_WARN_UNUSED
    { return mLocalDataDir; }

    const std::string &getTempDirectory() const A_WARN_UNUSED
    { return mTempDir; }

    const std::string &getScreenshotDirectory() const A_WARN_UNUSED
    { return mScreenshotDir; }

    const std::string getServerConfigDirectory() const A_WARN_UNUSED
    { return mServerConfigDir; }

    const std::string getUsersDirectory() const A_WARN_UNUSED
    { return mUsersDir; }

    const std::string getNpcsDirectory() const A_WARN_UNUSED
    { return mNpcsDir; }

    bool getIsMinimized() const A_WARN_UNUSED
    { return mIsMinimized; }

    void setIsMinimized(const bool n);

    void newChatMessage();

    bool getInputFocused() const A_WARN_UNUSED
    { return mInputFocused; }

    void setInputFocused(const bool n)
    { mInputFocused = n; }

    bool getMouseFocused() const A_WARN_UNUSED
    { return mMouseFocused; }

    void setMouseFocused(const bool n)
    { mMouseFocused = n; }

    std::string getUpdatesDir() const A_WARN_UNUSED
    { return mUpdatesDir; }

    std::string getServerName() const A_WARN_UNUSED
    { return mServerName; }

    std::string getOnlineUrl() const A_WARN_UNUSED
    { return mOnlineListUrl; }

    std::string getSupportUrl() const A_WARN_UNUSED
    { return mCurrentServer.supportUrl; }

    std::string getLogFileName() const A_WARN_UNUSED
    { return mLogFileName; }

    bool isKeyboardVisible() const;

    void setGuiAlpha(const float n)
    { mGuiAlpha = n; }

    float getGuiAlpha() const A_WARN_UNUSED
    { return mGuiAlpha; }

    static void closeDialogs();

    void setFramerate(const int fpsLimit) const;

    int getFramerate() const A_WARN_UNUSED;

    bool isTmw() const A_WARN_UNUSED;

    static void applyGrabMode();

    static void applyGamma();

    static void applyVSync();

    static void applyKeyRepeat();

    void optionChanged(const std::string &name) override final;

    void action(const gcn::ActionEvent &event) override final;

    void initTradeFilter() const;

    void initUsersDir();

    void initPacketLimiter();

    void writePacketLimits(const std::string &packetLimitsName) const;

    void resizeVideo(int actualWidth,
                     int actualHeight,
                     const bool always);

    void applyScale();

    bool limitPackets(const int type) A_WARN_UNUSED;

    bool checkPackets(const int type) const A_WARN_UNUSED;

    PacketLimit mPacketLimits[PACKET_SIZE + 1];

    void windowRemoved(const Window *const window);

    void updateScreenKeyboard(const int height)
    { mKeyboardHeight = height; }

    void reloadWallpaper();

    Window *openErrorDialog(const std::string &header,
                            const std::string &message,
                            const bool modal);

private:
    static void createWindows();

    static void initLang();

    void initSoundManager();

    void initConfigListeners();

    void initGraphics();

    void initTitle();

    static void extractDataDir();

    void mountDataDir();

    void initRootDir();

    void initHomeDir();

    void initConfiguration() const;

    void initLocalDataDir();

    void initTempDir();

    void initConfigDir();

    void initUpdatesDir();

    void initScreenshotDir();

    void initServerConfig(std::string serverName);

    static void initFeatures();

    static void accountLogin(LoginData *const data);

#ifndef ANDROID
    void storeSafeParameters() const;
#endif

    void backupConfig() const;

    void gameClear();

    void testsClear();

    static void logVars();

    static void checkConfigVersion();

    void setIcon();

    static Client *mInstance;

    static void bindTextDomain(const char *const path);

    static void setEnv(const char *const name, const char *const value);

#ifdef ANDROID
#ifdef USE_SDL2
    void extractAssets();
#endif
#endif

    Options mOptions;

    std::string mConfigDir;
    std::string mServerConfigDir;
    std::string mLocalDataDir;
    std::string mTempDir;
    std::string mUpdateHost;
    std::string mUpdatesDir;
    std::string mScreenshotDir;
    std::string mUsersDir;
    std::string mNpcsDir;
    std::string mRootDir;
    std::string mServerName;
    std::string mOnlineListUrl;
    std::string mLogFileName;

    ServerInfo mCurrentServer;

    Game *mGame;
    Window *mCurrentDialog;
    QuitDialog *mQuitDialog;
    Desktop *mDesktop;
    Button *mSetupButton;
    Button *mVideoButton;
    Button *mHelpButton;
    Button *mThemesButton;
    Button *mPerfomanceButton;
#ifdef ANDROID
    Button *mCloseButton;
#endif

    State mState;
    State mOldState;

    SDL_Surface *mIcon;

    std::string mCaption;
    FPSmanager mFpsManager;
    Skin *mSkin;
    int mButtonPadding;
    int mButtonSpacing;
    int mKeyboardHeight;
    std::string mOldUpdates;
    float mGuiAlpha;
    bool mLimitFps;
    bool mConfigAutoSaved;
    bool mIsMinimized;
    bool mInputFocused;
    bool mMouseFocused;
    bool mNewMessageFlag;
};

extern Client *client;

#endif  // CLIENT_H
