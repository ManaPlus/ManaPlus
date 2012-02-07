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

#include <iostream>
#include <sstream>

#include "logger.h"

#include "configuration.h"

#include "gui/widgets/chattab.h"

#ifdef WIN32
#include <windows.h>
#elif defined __APPLE__
#include <Carbon/Carbon.h>
#elif __linux__ || __linux
#include <stdlib.h>
#endif

#include <sys/time.h>

#include "debug.h"

Logger::Logger():
    mLogToStandardOut(true),
    mChatWindow(nullptr),
    mDebugLog(false)
{
}

Logger::~Logger()
{
    if (mLogFile.is_open())
        mLogFile.close();
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

void Logger::log(std::string str)
{
    log("%s", str.c_str());
}

void Logger::dlog(std::string str)
{
    if (!mDebugLog)
        return;

    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    std::stringstream timeStr;
    timeStr << "["
        << ((((tv.tv_sec / 60) / 60) % 24 < 10) ? "0" : "")
        << static_cast<int>(((tv.tv_sec / 60) / 60) % 24)
        << ":"
        << (((tv.tv_sec / 60) % 60 < 10) ? "0" : "")
        << static_cast<int>((tv.tv_sec / 60) % 60)
        << ":"
        << ((tv.tv_sec % 60 < 10) ? "0" : "")
        << static_cast<int>(tv.tv_sec % 60)
        << "."
        << (((tv.tv_usec / 10000) % 100) < 10 ? "0" : "")
        << static_cast<int>((tv.tv_usec / 10000) % 100)
        << "] ";

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << str << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << str << std::endl;

    if (mChatWindow && debugChatTab)
        debugChatTab->chatLog(str, BY_LOGGER);
}

void Logger::log1(const char *buf)
{
    // Get the current system time
    timeval tv;
    gettimeofday(&tv, nullptr);

    // Print the log entry
    std::stringstream timeStr;
    timeStr << "["
        << ((((tv.tv_sec / 60) / 60) % 24 < 10) ? "0" : "")
        << static_cast<int>(((tv.tv_sec / 60) / 60) % 24)
        << ":"
        << (((tv.tv_sec / 60) % 60 < 10) ? "0" : "")
        << static_cast<int>((tv.tv_sec / 60) % 60)
        << ":"
        << ((tv.tv_sec % 60 < 10) ? "0" : "")
        << static_cast<int>(tv.tv_sec % 60)
        << "."
        << (((tv.tv_usec / 10000) % 100) < 10 ? "0" : "")
        << static_cast<int>((tv.tv_usec / 10000) % 100)
        << "] ";

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << buf << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << buf << std::endl;

    if (mChatWindow && debugChatTab)
        debugChatTab->chatLog(buf, BY_LOGGER);
}

void Logger::log(const char *log_text, ...)
{
    unsigned size = 1024;
    char* buf = nullptr;
    if (strlen(log_text) * 3> size)
        size = static_cast<unsigned>(strlen(log_text) * 3);

    buf = new char[size + 1];
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
    timeStr << "["
        << ((((tv.tv_sec / 60) / 60) % 24 < 10) ? "0" : "")
        << static_cast<int>(((tv.tv_sec / 60) / 60) % 24)
        << ":"
        << (((tv.tv_sec / 60) % 60 < 10) ? "0" : "")
        << static_cast<int>((tv.tv_sec / 60) % 60)
        << ":"
        << ((tv.tv_sec % 60 < 10) ? "0" : "")
        << static_cast<int>(tv.tv_sec % 60)
        << "."
        << (((tv.tv_usec / 10000) % 100) < 10 ? "0" : "")
        << static_cast<int>((tv.tv_usec / 10000) % 100)
        << "] ";

    if (mLogFile.is_open())
        mLogFile << timeStr.str() << buf << std::endl;

    if (mLogToStandardOut)
        std::cout << timeStr.str() << buf << std::endl;

    if (mChatWindow && debugChatTab)
        debugChatTab->chatLog(buf, BY_LOGGER);

    // Delete temporary buffer
    delete [] buf;
}

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
#elif defined __linux__ || __linux
    std::cerr << "Error: " << error_text << std::endl;
    std::string msg = "xmessage \"" + error_text + "\"";
    if (system(msg.c_str()) == -1)
        std::cerr << "Error: " << error_text << std::endl;
#else
    std::cerr << "Error: " << error_text << std::endl;
#endif
    exit(1);
}
