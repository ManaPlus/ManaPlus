/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "options.h"

#include <string>
#include <vector>

#include "localconsts.h"

class Settings final
{
    public:
        Settings() :
            updateHost(),
            oldUpdates(),
            updatesDir(),
            configDir(),
            localDataDir(),
            tempDir(),
            serverName(),
            screenshotDir(),
            onlineListUrl(),
            serverConfigDir(),
            usersDir(),
            npcsDir(),
            supportUrl(),
            logFileName(),
            rootDir(),
            windowCaption(),
            updateMirrors(),
            options(),
            guiAlpha(1.0F),
            textureSize(1024),
            persistentIp(true),
            limitFps(false),
            inputFocused(true),
            mouseFocused(true)
        { }

        std::string updateHost;
        std::string oldUpdates;
        std::string updatesDir;
        std::string configDir;
        std::string localDataDir;
        std::string tempDir;
        std::string serverName;
        std::string screenshotDir;
        std::string onlineListUrl;
        std::string serverConfigDir;
        std::string usersDir;
        std::string npcsDir;
        std::string supportUrl;
        std::string logFileName;
        std::string rootDir;
        std::string windowCaption;
        std::vector<std::string> updateMirrors;
        Options options;
        float guiAlpha;
        unsigned int textureSize;
        bool persistentIp;
        bool limitFps;
        bool inputFocused;
        bool mouseFocused;
};

extern Settings settings;

#endif  // SETTINGS_H
