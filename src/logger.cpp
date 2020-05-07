/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "logger.h"

#include "settings.h"

#include "listeners/debugmessagelistener.h"

#include "utils/cast.h"
#include "utils/foreach.h"
#include "utils/stringutils.h"

#include <iostream>

#ifdef WIN32
#include <windows.h>
#elif defined __APPLE__
#include <Carbon/Carbon.h>
#endif  // WIN32

#include <sys/time.h>

#include <sstream>

#ifdef USE_SDL2
#include <SDL_messagebox.h>
#endif  // USE_SDl2

#ifdef ENABLEDEBUGLOG
#if defined(__ANDROID__)
#include <android/log.h>
#ifdef SPECIAL_LOGGING
#define SPECIALLOG(x) __android_log_print(ANDROID_LOG_INFO, "manaplus", x);
#define DSPECIALLOG(x) __android_log_print(ANDROID_LOG_VERBOSE, \
    "manaplus", x);
#else  // SPECIAL_LOGGING
#define SPECIALLOG(x) if (mDebugLog) \
    __android_log_print(ANDROID_LOG_INFO, "manaplus", x);
#define DSPECIALLOG(x) if (mDebugLog) \
    __android_log_print(ANDROID_LOG_VERBOSE, "manaplus", x);
#endif  // SPECIAL_LOGGING
#elif defined __native_client__
#ifdef SPECIAL_LOGGING
#define SPECIALLOG(x) std::cerr << x;
#define DSPECIALLOG(x) std::cerr << x;
#else  // SPECIAL_LOGGING
#define SPECIALLOG(x) if (mDebugLog) \
    std::cerr << x;
#define DSPECIALLOG(x) if (mDebugLog) \
    std::cerr << x;
#endif  // SPECIAL_LOGGING
#else  // defined(__ANDROID__)
#define SPECIALLOG(x)
#define DSPECIALLOG(x)
#endif  // defined(__ANDROID__)
#endif  // ENABLEDEBUGLOG

#include "debug.h"

#define DATESTREAM \
    std::string timeStr = strprintf( \
        "[%02d:%02d:%02d.%02d]", \
        CAST_S32(((tv.tv_sec / 60) / 60) % 24), \
        CAST_S32((tv.tv_sec / 60) % 60), \
        CAST_S32(tv.tv_sec % 60), \
        CAST_S32((tv.tv_usec / 10000) % 100));

Logger *logger = nullptr;          // Log object

Logger::Logger() :
    mLogFile(nullptr),
    mDelayedLog(),
    mMutex(SDL_CreateMutex()),
    mThreadLocked(false),
    mLogToStandardOut(true),
    mDebugLog(false),
    mReportUnimplemented(false)
{
#if defined __native_client__ && defined(NACL_LOG)
    std::cout.setf(std::ios_base::unitbuf);
#endif  // defined __native_client__ && defined(NACL_LOG)
}

Logger::~Logger()
{
    closeFile();
    SDL_DestroyMutex(mMutex);
}

void Logger::closeFile()
{
    if (mLogFile != nullptr)
    {
        fclose(mLogFile);
        mLogFile = nullptr;
    }
}

void Logger::setLogFile(const std::string &logFilename)
{
    closeFile();

#ifdef __SWITCH__
    mLogFile = nullptr;
#else
    mLogFile = fopen(logFilename.c_str(), "wt");
#endif

    if (mLogFile == nullptr)
    {
        std::cout << "Warning: error while opening " << logFilename <<
            " for writing.\n";
        mLogToStandardOut = true;
    }
    else
    {
        mLogToStandardOut = false;
    }
}

void Logger::log(const std::string &str)
{
    log("%s", str.c_str());
}

#ifdef ENABLEDEBUGLOG
void Logger::dlog(const std::string &str)
{
    if (!mDebugLog)
        return;

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    DSPECIALLOG(str.c_str())

    if (mLogFile != nullptr)
    {
        fprintf(mLogFile,
            "%s %s\n",
            timeStr.c_str(),
            str.c_str());
    }

    if (mLogToStandardOut)
    {
        fprintf(stdout,
            "%s %s\n",
            timeStr.c_str(),
            str.c_str());
    }
}

void Logger::dlog2(const std::string &str,
                   const int pos,
                   const char* const comment)
{
    if (!mDebugLog)
        return;

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    DSPECIALLOG(str.c_str())

    if (mLogFile != nullptr)
    {
        if (comment != nullptr)
        {
            fprintf(mLogFile,
                "%s %04d %s: %s\n",
                timeStr.c_str(),
                pos,
                str.c_str(),
                comment);
        }
        else
        {
            fprintf(mLogFile,
                "%s %04d %s\n",
                timeStr.c_str(),
                pos,
                str.c_str());
        }
        fflush(mLogFile);
    }

    if (mLogToStandardOut)
    {
        if (comment != nullptr)
        {
            fprintf(stdout,
                "%s %04d %s: %s\n",
                timeStr.c_str(),
                pos,
                str.c_str(),
                comment);
        }
        else
        {
            fprintf(stdout,
                "%s %04d %s\n",
                timeStr.c_str(),
                pos,
                str.c_str());
        }
    }
}
#endif  // ENABLEDEBUGLOG

void Logger::log1(const char *const buf)
{
    if (settings.disableLoggingInGame)
        return;

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    SPECIALLOG(buf)

    if (mLogFile != nullptr)
    {
        fprintf(mLogFile,
            "%s %s\n",
            timeStr.c_str(),
            buf);
        fflush(mLogFile);
    }

    if (mLogToStandardOut)
    {
        fprintf(stdout,
            "%s %s\n",
            timeStr.c_str(),
            buf);
    }
}

void Logger::log(const char *const log_text, ...)
{
    if (settings.disableLoggingInGame)
        return;

    unsigned size = 1024;
    if (strlen(log_text) * 3 > size)
        size = CAST_U32(strlen(log_text) * 3);

    char* buf = new char[CAST_SIZE(size + 1)];
    va_list ap;

    // Use a temporary buffer to fill in the variables
    va_start(ap, log_text);
    vsnprintf(buf, size, log_text, ap);
    buf[size] = 0;
    va_end(ap);

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    SPECIALLOG(buf)

    if (mLogFile != nullptr)
    {
        fprintf(mLogFile,
            "%s %s\n",
            timeStr.c_str(),
            buf);
        fflush(mLogFile);
    }

    if (mLogToStandardOut)
    {
        fprintf(stdout,
            "%s %s\n",
            timeStr.c_str(),
            buf);
    }

    // Delete temporary buffer
    delete [] buf;
}

void Logger::assertLog(const char *const log_text, ...)
{
    if (settings.disableLoggingInGame)
        return;

    unsigned size = 1024;
    if (strlen(log_text) * 3 > size)
        size = CAST_U32(strlen(log_text) * 3);

    char* buf = new char[CAST_SIZE(size + 1)];
    va_list ap;

    // Use a temporary buffer to fill in the variables
    va_start(ap, log_text);
    vsnprintf(buf, size, log_text, ap);
    buf[size] = 0;
    va_end(ap);

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    SPECIALLOG(buf)

    if (mLogFile != nullptr)
    {
        fprintf(mLogFile,
            "%s %s\n",
            timeStr.c_str(),
            buf);
        fflush(mLogFile);
    }

    if (mLogToStandardOut)
    {
        fprintf(stdout,
            "%s %s\n",
            timeStr.c_str(),
            buf);
    }

    DebugMessageListener::distributeEvent(buf);

    // Delete temporary buffer
    delete [] buf;
}

void Logger::log_r(const char *const log_text, ...)
{
    if (settings.disableLoggingInGame)
        return;

    SDL_mutexP(mMutex);

    unsigned size = 1024;
    if (strlen(log_text) * 3 > size)
        size = CAST_U32(strlen(log_text) * 3);

    char* buf = new char[CAST_SIZE(size + 1)];
    va_list ap;

    // Use a temporary buffer to fill in the variables
    va_start(ap, log_text);
    vsnprintf(buf, size, log_text, ap);
    buf[size] = 0;
    va_end(ap);

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    DATESTREAM
    SPECIALLOG(buf)

    if (mLogFile != nullptr)
    {
        std::string tmpStr = strprintf(
            "%s %s\n",
            timeStr.c_str(),
            buf);
        mThreadLocked = true;
        mDelayedLog.push_back(tmpStr);
        mThreadLocked = false;
    }

    if (mLogToStandardOut)
    {
        fprintf(stdout,
            "%s %s\n",
            timeStr.c_str(),
            buf);
    }

    // Delete temporary buffer
    delete [] buf;

    SDL_mutexV(mMutex);
}

void Logger::flush()
{
    if (!mThreadLocked)
    {
        SDL_mutexP(mMutex);
        FOR_EACH (STD_VECTOR<std::string>::const_iterator, it, mDelayedLog)
        {
            fputs((*it).c_str(), mLogFile);
            fputs("\n", mLogFile);
        }
        fflush(mLogFile);
        mDelayedLog.clear();
        SDL_mutexV(mMutex);
    }
}

// here string must be safe for any usage
void Logger::safeError(const std::string &error_text)
{
    log("Error: %s", error_text.c_str());
#ifdef USE_SDL2
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Error",
        error_text.c_str(),
        nullptr);
#else  // USE_SDL2
#ifdef WIN32
    MessageBox(nullptr, error_text.c_str(), "Error", MB_ICONERROR | MB_OK);
#elif defined __APPLE__
//    Str255 msg;
//    CFStringRef error;
//    error = CFStringCreateWithCString(nullptr,
//                                      error_text.c_str(),
//                                      kCFStringEncodingMacRoman);
//    CFStringGetPascalString(error, msg, 255, kCFStringEncodingMacRoman);
//    StandardAlert(kAlertStopAlert,
//                  (const unsigned char*)"\pError",
//                  (ConstStr255Param) msg, nullptr, nullptr);
#elif defined(__linux__) || defined(__linux)
    std::cerr << "Error: " << error_text << std::endl;
    const std::string msg = std::string("xmessage \"").append(
        error_text).append("\"");
    if (system(msg.c_str()) == -1)
        std::cerr << "Error: " << error_text << std::endl;
#else  // WIN32

    std::cerr << "Error: " << error_text << std::endl;
#endif  // WIN32
#endif  // USE_SDL2

    exit(1);
}

// here string can be unsafe strings
void Logger::error(const std::string &error_text)
{
    log("Error: %s", error_text.c_str());
#ifdef USE_SDL2
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Error",
        error_text.c_str(),
        nullptr);
#else  // USE_SDL2
#ifdef WIN32
    MessageBox(nullptr, error_text.c_str(), "Error", MB_ICONERROR | MB_OK);
#elif defined __APPLE__
//    Str255 msg;
//    CFStringRef error;
//    error = CFStringCreateWithCString(nullptr,
//                                      error_text.c_str(),
//                                      kCFStringEncodingMacRoman);
//    CFStringGetPascalString(error, msg, 255, kCFStringEncodingMacRoman);
//    StandardAlert(kAlertStopAlert,
//                  (const unsigned char*)"\pError",
//                  (ConstStr255Param) msg, nullptr, nullptr);
#elif defined(__linux__) || defined(__linux)
    std::cerr << "Error: " << error_text << std::endl;
    const std::string msg("xmessage \"Error happened. "
        "Please see log file for more information.\"");
    if (system(msg.c_str()) == -1)
        std::cerr << "Error: " << error_text << std::endl;
#else  // WIN32

    std::cerr << "Error: " << error_text << std::endl;
#endif  // WIN32
#endif  // USE_SDL2

    exit(1);
}

void Logger::unimplemented(const int id)
{
    if (!mReportUnimplemented)
        return;

    const std::string str = strprintf("Unimplimented packet: %d (0x%x)",
        id,
        CAST_U32(id));
    DebugMessageListener::distributeEvent(str);
    log(str);
}

void Logger::unimplemented(const int id,
                           const int id2)
{
    if (!mReportUnimplemented)
        return;

    const std::string str = strprintf(
        "Unimplimented field value %d for packet %d (0x%x)",
        id2,
        id,
        CAST_U32(id));
    DebugMessageListener::distributeEvent(str);
    log(str);
}

void Logger::unimplemented(const uint32_t id,
                           const uint32_t id2,
                           const uint32_t id3) const
{
    if (!mReportUnimplemented)
        return;

    const std::string str = strprintf(
        "Wrong actual or planned inbound packet size!. "
        "Packet id: %u(0x%x), Planned size: %u, Actual size: %u",
        id,
        id,
        id2,
        id3);
    DebugMessageListener::distributeEvent(str);
}

FILE *Logger::getFile() const
{
    return mLogFile;
}
