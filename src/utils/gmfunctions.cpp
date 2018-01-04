/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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

#include "utils/gmfunctions.h"

#include "settings.h"

#include "being/localplayer.h"

#include "const/gui/chat.h"

#include "net/chathandler.h"

#include "debug.h"

namespace Gm
{

void runCommand(const std::string &command,
                const std::string &params)
{
    if (params.empty())
    {
        chatHandler->talk(std::string(
            settings.gmCommandSymbol).append(
            command),
            GENERAL_CHANNEL);
    }
    else
    {
        chatHandler->talk(std::string(
            settings.gmCommandSymbol).append(
            command).append(
            " ").append(
            params),
            GENERAL_CHANNEL);
    }
}

void runCommand(const std::string &command)
{
    chatHandler->talk(std::string(
        settings.gmCommandSymbol).append(
        command),
        GENERAL_CHANNEL);
}

void runCharCommand(const std::string &command,
                    const std::string &name,
                    const std::string &params)
{
    if ((localPlayer != nullptr) && name == localPlayer->getName())
    {
        if (params.empty())
        {
            chatHandler->talk(std::string(
                settings.gmCommandSymbol).append(
                command),
                GENERAL_CHANNEL);
        }
        else
        {
            chatHandler->talk(std::string(
                settings.gmCommandSymbol).append(
                command).append(
                " ").append(
                params),
                GENERAL_CHANNEL);
        }
    }
    else
    {
        chatHandler->talk(std::string(
            settings.gmCharCommandSymbol).append(
            command).append(
            " \"").append(
            name).append(
            "\" ").append(
            params),
            GENERAL_CHANNEL);
    }
}

void runCharCommand(const std::string &command,
                    const std::string &name)
{
    if ((localPlayer != nullptr) && name == localPlayer->getName())
    {
        chatHandler->talk(std::string(
            settings.gmCommandSymbol).append(
            command),
            GENERAL_CHANNEL);
    }
    else
    {
        chatHandler->talk(std::string(
            settings.gmCharCommandSymbol).append(
            command).append(
            " ").append(
            name),
            GENERAL_CHANNEL);
    }
}

}  // namespace Gm
