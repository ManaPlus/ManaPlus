/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef CLIENT_H
#define CLIENT_H

#include "configlistener.h"

#include "net/serverinfo.h"

#include <guichan/actionlistener.hpp>

#include <SDL.h>
#include <SDL_framerate.h>

#include <string>

#include <sys/time.h>

class Button;
class Desktop;
class Game;
class LoginData;
class Window;
class QuitDialog;

/**
 * Set the milliseconds value of a tick time.
 */
static const int MILLISECONDS_IN_A_TICK = 10;

//manaserv uses 9601
//static const short DEFAULT_PORT = 9601;
static const short DEFAULT_PORT = 6901;

extern volatile int fps;
extern volatile int lps;
extern volatile int tick_time;
extern volatile int cur_time;
extern bool isSafeMode;
extern int serverVersion;
extern int start_time;
extern int textures_count;

class ErrorListener : public gcn::ActionListener
{
    public:
        void action(const gcn::ActionEvent &event) override;
};

extern std::string errorMessage;
extern ErrorListener errorListener;
extern LoginData loginData;

/**
 * Returns elapsed time. (Warning: supposes the delay is always < 100 seconds)
 */
int get_elapsed_time(const int startTime);

int get_elapsed_time1(const int startTime);

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
    STATE_WORLD_SELECT,           // 5
    STATE_WORLD_SELECT_ATTEMPT,
    STATE_UPDATE,
    STATE_LOAD_DATA,
    STATE_GET_CHARACTERS,
    STATE_CHAR_SELECT,            // 10
    STATE_CONNECT_GAME,
    STATE_GAME,
    STATE_CHANGE_MAP,             // Switch map-server/gameserver
    STATE_LOGIN_ERROR,
    STATE_ACCOUNTCHANGE_ERROR,    // 15
    STATE_REGISTER_PREP,
    STATE_REGISTER,
    STATE_REGISTER_ATTEMPT,
    STATE_CHANGEPASSWORD,
    STATE_CHANGEPASSWORD_ATTEMPT, // 20
    STATE_CHANGEPASSWORD_SUCCESS,
    STATE_CHANGEEMAIL,
    STATE_CHANGEEMAIL_ATTEMPT,
    STATE_CHANGEEMAIL_SUCCESS,
    STATE_UNREGISTER,             // 25
    STATE_UNREGISTER_ATTEMPT,
    STATE_UNREGISTER_SUCCESS,
    STATE_SWITCH_SERVER,
    STATE_SWITCH_LOGIN,
    STATE_SWITCH_CHARACTER,       // 30
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
        Options():
            printHelp(false),
            printVersion(false),
            skipUpdate(false),
            chooseDefault(false),
            noOpenGL(false),
            safeMode(false),
            testMode(false),
            serverPort(0)
        {}

        bool printHelp;
        bool printVersion;
        bool skipUpdate;
        bool chooseDefault;
        bool noOpenGL;
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
        bool safeMode;
        bool testMode;
        std::string test;

        std::string serverName;
        short serverPort;
    };

    Client(const Options &options);

    A_DELETE_COPY(Client);

    ~Client();

    /**
     * Provides access to the client instance.
     */
    static Client *instance()
    { return mInstance; }

    void gameInit();

    void testsInit();

    int gameExec();

    int testsExec() const;

    static void setState(const State state)
    { instance()->mState = state; }

    static State getState()
    { return instance()->mState; }

    static const std::string &getPackageDirectory()
    { return instance()->mPackageDir; }

    static const std::string &getConfigDirectory()
    { return instance()->mConfigDir; }

    static const std::string &getLocalDataDirectory()
    { return instance()->mLocalDataDir; }

    static const std::string &getTempDirectory()
    { return instance()->mTempDir; }

    static const std::string &getScreenshotDirectory()
    { return instance()->mScreenshotDir; }

    static const std::string getServerConfigDirectory();

    static const std::string getUsersDirectory();

    static const std::string getNpcsDirectory();

    static bool getIsMinimized()
    { return instance()->mIsMinimized; }

    static void setIsMinimized(const bool n)
    { instance()->mIsMinimized = n; }

    static bool getInputFocused()
    { return instance()->mInputFocused; }

    static void setInputFocused(const bool n)
    { instance()->mInputFocused = n; }

    static bool getMouseFocused()
    { return instance()->mMouseFocused; }

    static void setMouseFocused(const bool n)
    { instance()->mMouseFocused = n; }

    static std::string getUpdatesDir()
    { return instance()->mUpdatesDir; }

    static std::string getServerName()
    { return instance()->mServerName; }

    static void resize(const int width, const int height,
                       const bool always = false)
    { instance()->resizeVideo(width, height, always); }

    static void setGuiAlpha(const float n);

    static float getGuiAlpha();

    static void closeDialogs();

    static void setFramerate(const int fpsLimit);

    static int getFramerate();

    static bool isTmw();

    static void applyGrabMode();

    static void applyGamma();

    static void applyVSync();

    static void applyKeyRepeat();

    void optionChanged(const std::string &name) override;

    void action(const gcn::ActionEvent &event) override;

    void initTradeFilter() const;

    void initUsersDir();

    void initPacketLimiter();

    void writePacketLimits(std::string packetLimitsName) const;

    void resizeVideo(int width, int height, const bool always);

    static bool limitPackets(const int type);

    static bool checkPackets(const int type);

    PacketLimit mPacketLimits[PACKET_SIZE + 1];

private:
    void initRootDir();

    void initHomeDir();

    void initConfiguration() const;

    void initLocalDataDir();

    void initTempDir();

    void initConfigDir();

    void initUpdatesDir();

    void initScreenshotDir();

    void initServerConfig(std::string serverName);

    bool copyFile(const std::string &configPath,
                  const std::string &oldConfigPath) const;

    void accountLogin(LoginData *const data) const;

    void storeSafeParameters() const;

    void gameClear();

    void testsClear();

    static Client *mInstance;

    Options mOptions;

    std::string mPackageDir;
    std::string mConfigDir;
    std::string mLocalDataDir;
    std::string mTempDir;
    std::string mUpdateHost;
    std::string mUpdatesDir;
    std::string mScreenshotDir;
    std::string mServerConfigDir;
    std::string mUsersDir;
    std::string mNpcsDir;
    std::string mRootDir;
    std::string mServerName;

    ServerInfo mCurrentServer;

    Game *mGame;
    Window *mCurrentDialog;
    QuitDialog *mQuitDialog;
    Desktop *mDesktop;
    Button *mSetupButton;
    Button *mVideoButton;
    Button *mThemesButton;
    Button *mPerfomanceButton;

    State mState;
    State mOldState;

    SDL_Surface *mIcon;

    SDL_TimerID mLogicCounterId;
    SDL_TimerID mSecondsCounterId;

    bool mLimitFps;
    bool mConfigAutoSaved;
    bool mIsMinimized;
    bool mInputFocused;
    bool mMouseFocused;
    float mGuiAlpha;
    FPSmanager mFpsManager;
};

#endif // CLIENT_H
