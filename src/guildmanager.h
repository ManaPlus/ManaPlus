/*
 *  The ManaPlus Client
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

#ifndef GUILDMANAGER_H
#define GUILDMANAGER_H

#include "utils/stringvector.h"

#include <map>

class Guild;
class GuildChatTab;

class GuildManager
{
    public:
        GuildManager();

        ~GuildManager();

        static void init();

        void chat(std::string msg);

        void send(std::string msg) const;

        bool processGuildMessage(std::string msg);

        void getNames(StringVect &names) const;

        void requestGuildInfo();

        void updateList();

        static bool getEnableGuildBot()
        { return mEnableGuildBot; }

        void kick(std::string msg) const;

        void invite(std::string msg) const;

        void leave() const;

        void notice(std::string msg) const;

        void createTab(Guild *const guild);

        Guild *createGuild() const;

        void clear() const;

        void reload();

        void inviteResponse(const bool response) const;

        bool afterRemove();

        bool havePower() const
        { return mHavePower; }

    private:
        bool process(std::string msg);

        static bool mEnableGuildBot;
        bool mGotInfo;
        bool mGotName;
        bool mSentInfoRequest;
        bool mSentNameRequest;
        bool mHavePower;
        StringVect mTempList;
        GuildChatTab *mTab;
        bool mRequest;
};

extern GuildManager *guildManager;

#endif // GUILDMANAGER_H
