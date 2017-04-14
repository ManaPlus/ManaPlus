/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "chatlogger.h"

#include <iostream>
#include <dirent.h>

#ifdef WIN32
#include <windows.h>
#include <sys/time.h>
#elif defined __APPLE__
#include <Carbon/Carbon.h>
#endif  // WIN32

#include "configuration.h"

#include "fs/mkdir.h"

#include "debug.h"

ChatLogger *chatLogger = nullptr;

ChatLogger::ChatLogger() :
    mLogFile(),
    mLogDir(),
    mBaseLogDir(),
    mServerName(),
    mLogFileName()
{
}

ChatLogger::~ChatLogger()
{
    if (mLogFile.is_open())
        mLogFile.close();
}

void ChatLogger::setLogFile(const std::string &logFilename)
{
    if (mLogFile.is_open())
        mLogFile.close();

    mLogFile.open(logFilename.c_str(),
        std::ios_base::app);
    mLogFileName = logFilename;

    if (!mLogFile.is_open())
    {
        std::cout << "Warning: error while opening " <<
            logFilename <<
            " for writing.\n";
    }
}

void ChatLogger::setLogDir(const std::string &logDir)
{
    mLogDir = logDir;

    if (mLogFile.is_open())
        mLogFile.close();

    DIR *const dir = opendir(mLogDir.c_str());
    if (!dir)
        mkdir_r(mLogDir.c_str());
    else
        closedir(dir);
}

void ChatLogger::log(std::string str)
{
    const std::string &dateStr = getDir();
    const std::string logFileName = strprintf(
        "%s/#General.log", dateStr.c_str());
    if (!mLogFile.is_open() ||
        logFileName != mLogFileName)
    {
        setLogDir(dateStr);
        setLogFile(logFileName);
    }

    str = removeColors(str);
    writeTo(mLogFile, str);
}

void ChatLogger::log(std::string name,
                     std::string str)
{
    const std::string &dateStr = getDir();
    const std::string logFileName = strprintf("%s/%s.log",
        dateStr.c_str(), secureName(name).c_str());

    if (!mLogFile.is_open() || logFileName != mLogFileName)
    {
        setLogDir(dateStr);
        setLogFile(logFileName);
    }

    str = removeColors(str);
    writeTo(mLogFile, str);
}

std::string ChatLogger::getDir() const
{
    std::string date;

    time_t rawtime;
    char buffer [81];

    time(&rawtime);
    struct tm *const timeinfo = localtime(&rawtime);

    strftime(buffer, 79, "%Y-%m/%d", timeinfo);

    date = strprintf("%s/%s/%s",
        mBaseLogDir.c_str(),
        mServerName.c_str(),
        buffer);

    return date;
}

std::string ChatLogger::secureName(std::string &name)
{
    const size_t sz = name.length();
    for (size_t f = 0; f < sz; f ++)
    {
        const unsigned char ch = name[f];
        if ((ch < '0' || ch > '9') &&
            (ch < 'a' || ch > 'z') &&
            (ch < 'A' || ch > 'Z') &&
            ch != '-' &&
            ch != '+' &&
            ch != '=' &&
            ch != '.' &&
            ch != ',' &&
            ch != ')' &&
            ch != '(' &&
            ch != '[' &&
            ch != ']' &&
            ch != '#')
        {
            name[f] = '_';
        }
    }
    return name;
}

void ChatLogger::writeTo(std::ofstream &file,
                         const std::string &str)
{
    file << str << std::endl;
}

void ChatLogger::setServerName(const std::string &serverName)
{
    mServerName = serverName;
    if (mServerName.empty())
        mServerName = config.getStringValue("MostUsedServerName0");

    if (mLogFile.is_open())
        mLogFile.close();

    secureName(mServerName);
    if (!mLogDir.empty())
    {
        const std::string name = pathJoin(mLogDir, mServerName);
        DIR *const dir = opendir(name.c_str());
        if (!dir)
            mkdir_r(name.c_str());
        else
            closedir(dir);
    }
}

void ChatLogger::loadLast(std::string name,
                          std::list<std::string> &list,
                          const unsigned int n) const
{
    std::ifstream logFile;
    std::string fileName = strprintf("%s/%s.log",
        getDir().c_str(),
        secureName(name).c_str());

    logFile.open(fileName.c_str(), std::ios::in);

    if (!logFile.is_open())
        return;

    char line[710];
    unsigned sz = CAST_U32(list.size());
    while (logFile.getline(line, 700))
    {
        list.push_back(line);
        sz ++;
        if (sz > n)
        {
            list.pop_front();
            sz --;
        }
    }

    if (logFile.is_open())
        logFile.close();
}

void ChatLogger::clear()
{
    mLogDir.clear();
    mServerName.clear();
    mLogFileName.clear();
}
