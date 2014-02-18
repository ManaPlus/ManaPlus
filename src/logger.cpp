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

#include "logger.h"

#include <sstream>

#include <iostream>

#ifdef WIN32
#include <windows.h>
#elif defined __APPLE__
#include <Carbon/Carbon.h>
#endif

#include <sys/time.h>

#if defined(__ANDROID__) && defined(ANDROID_LOG)
#include <android/log.h>
#define LOG_ANDROID(x) __android_log_print(ANDROID_LOG_INFO, "manaplus", x);
#define DLOG_ANDROID(x) __android_log_print(ANDROID_LOG_VERBOSE, \
    "manaplus", x);
#else
#define LOG_ANDROID(x)
#define DLOG_ANDROID(x)
#endif

#include "debug.h"

#define DATESTREAM \
    timeStr << "[" \
        << ((((tv.tv_sec / 60) / 60) % 24 < 10) ? "0" : "") \
        << static_cast<int>(((tv.tv_sec / 60) / 60) % 24) \
        << ":" \
        << (((tv.tv_sec / 60) % 60 < 10) ? "0" : "") \
        << static_cast<int>((tv.tv_sec / 60) % 60) \
        << ":" \
        << ((tv.tv_sec % 60 < 10) ? "0" : "") \
        << static_cast<int>(tv.tv_sec % 60) \
        << "." \
        << (((tv.tv_usec / 10000) % 100) < 10 ? "0" : "") \
        << static_cast<int>((tv.tv_usec / 10000) % 100) \
        << "] ";

Logger *logger = nullptr;          // Log object

Logger::Logger() :
    mLogFile(),
    mDelayedLog(),
    mMutex(SDL_CreateMutex()),
    mThreadLocked(false),
    mLogToStandardOut(true),
    mDebugLog(false)
{
}

Logger::~Logger()
{
    if (mLogFile.is_open())
        mLogFile.close();
    SDL_DestroyMutex(mMutex);
}

void Logger::setLogFile(const std::string &logFilename)
{
    if (mLogFile.is_open())
        mLogFile.close();

    mLogFile.open(logFilename.c_str(), std::ios_base::trunc);

    if (!mLogFile.is_open())
    {
        std::cout << "Warning: error while opening " << logFilename <<
            " for writing.\n";
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
    std::stringstream timeStr;
    DATESTREAM
    DLOG_ANDROID(str.c_str())

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << str << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << str << std::endl;
}
#endif

void Logger::log1(const char *const buf)
{
    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    std::stringstream timeStr;
    DATESTREAM
    LOG_ANDROID(buf)

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << buf << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << buf << std::endl;
}

void Logger::log(const char *const log_text, ...)
{
    unsigned size = 1024;
    if (strlen(log_text) * 3 > size)
        size = static_cast<unsigned>(strlen(log_text) * 3);

    char* buf = new char[size + 1];
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
    std::stringstream timeStr;
    DATESTREAM
    LOG_ANDROID(buf)

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << buf << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << buf << std::endl;

    // Delete temporary buffer
    delete [] buf;
}

void Logger::log_r(const char *const log_text, ...)
{
    SDL_mutexP(mMutex);

    unsigned size = 1024;
    if (strlen(log_text) * 3 > size)
        size = static_cast<unsigned>(strlen(log_text) * 3);

    char* buf = new char[size + 1];
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
    std::stringstream timeStr;
    DATESTREAM
    LOG_ANDROID(buf)

    if (mLogFile.is_open())
    {
        timeStr << buf << std::endl;
        mThreadLocked = true;
        mDelayedLog.push_back(timeStr.str());
        mThreadLocked = false;
    }

    if (mLogToStandardOut)
        std::cout << timeStr.str() << buf << std::endl;

    // Delete temporary buffer
    delete [] buf;

    SDL_mutexV(mMutex);
}

void Logger::flush()
{
    if (!mThreadLocked)
    {
        SDL_mutexP(mMutex);
        FOR_EACH (std::vector<std::string>::const_iterator, it, mDelayedLog)
            mLogFile << *it;
        mDelayedLog.clear();
        SDL_mutexV(mMutex);
    }
}

// here string must be safe for any usage
void Logger::safeError(const std::string &error_text)
{
    log("Error: %s", error_text.c_str());
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
#else
    std::cerr << "Error: " << error_text << std::endl;
#endif
    exit(1);
}

// here string can be unsafe strings
void Logger::error(const std::string &error_text)
{
    log("Error: %s", error_text.c_str());
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
#elif defined(__linux__) || defined(_linux)
    std::cerr << "Error: " << error_text << std::endl;
    const std::string msg("xmessage \"Error happend. "
        "Please see log file for more information.\"");
    if (system(msg.c_str()) == -1)
        std::cerr << "Error: " << error_text << std::endl;
#else
    std::cerr << "Error: " << error_text << std::endl;
#endif
    exit(1);
}
