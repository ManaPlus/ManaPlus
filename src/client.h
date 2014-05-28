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

#include "state.h"

#include "listeners/configlistener.h"
#include "listeners/errorlistener.h"

#include "net/serverinfo.h"

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

extern std::string errorMessage;
extern LoginData loginData;

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
class Client final : public ConfigListener,
                     public ActionListener
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

    const Options &getOptions() const A_WARN_UNUSED
    { return mOptions; }

    bool isKeyboardVisible() const;

    void setGuiAlpha(const float n)
    { mGuiAlpha = n; }

    float getGuiAlpha() const A_WARN_UNUSED
    { return mGuiAlpha; }

    void setFramerate(const int fpsLimit) const;

    int getFramerate() const A_WARN_UNUSED;

    bool isTmw() const A_WARN_UNUSED;

    static void applyGrabMode();

    static void applyGamma();

    static void applyVSync();

    static void applyKeyRepeat();

    void optionChanged(const std::string &name) override final;

    void action(const ActionEvent &event) override final;

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

private:
    static void createWindows();

    static void initLang();

    void initSoundManager();

    void initConfigListeners();

    void initGraphics();

    void updateEnv();

    void initTitle();

    void updateDataPath();

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

    void initServerConfig(const std::string &serverName);

    static void initFeatures();

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
    static void extractAssets();
#endif
#endif

    Options mOptions;

    std::string mRootDir;
    std::string mLogFileName;

    ServerInfo mCurrentServer;

    Game *mGame;
    Window *mCurrentDialog;
    QuitDialog *mQuitDialog;
    Desktop *mDesktop;
    Button *mSetupButton;
    Button *mVideoButton;
    Button *mHelpButton;
    Button *mAboutButton;
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
    float mGuiAlpha;
    int mButtonPadding;
    int mButtonSpacing;
    int mKeyboardHeight;
    bool mLimitFps;
    bool mConfigAutoSaved;
    bool mIsMinimized;
    bool mInputFocused;
    bool mMouseFocused;
    bool mNewMessageFlag;
};

extern Client *client;
extern unsigned int mLastHost;
extern unsigned long mSearchHash;

#endif  // CLIENT_H
