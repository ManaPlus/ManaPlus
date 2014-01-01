/*
 *  The ManaPlus Client
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

#ifndef GUILDMANAGER_H
#define GUILDMANAGER_H

#include "utils/stringvector.h"

#include "localconsts.h"

class ChatTab;
class Guild;
class GuildChatTab;

class GuildManager final
{
    public:
        GuildManager();

        A_DELETE_COPY(GuildManager)

        ~GuildManager();

        static void init();

        void chat(const std::string &msg);

        static void send(const std::string &msg);

        bool processGuildMessage(const std::string &msg);

        void getNames(StringVect &names) const;

        void requestGuildInfo();

        void updateList();

        static bool getEnableGuildBot() A_WARN_UNUSED
        { return mEnableGuildBot; }

        static void kick(const std::string &msg);

        static void invite(const std::string &msg);

        static void leave();

        static void notice(const std::string &msg);

        void createTab(Guild *const guild);

        Guild *createGuild() const A_WARN_UNUSED;

        static void clear();

        void reload();

        static void inviteResponse(const bool response);

        bool afterRemove();

        void slowLogic();

        bool havePower() const A_WARN_UNUSED
        { return mHavePower; }

        ChatTab *getTab();

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
        int mRequestTime;
        bool mGotOnlineInfo;
};

extern GuildManager *guildManager;

#endif  // GUILDMANAGER_H
