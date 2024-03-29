/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUILD_H
#define GUILD_H

#include "gui/models/avatarlistmodel.h"

#include "utils/cast.h"
#include "utils/dtor.h"
#include "utils/stringvector.h"

#include <map>

class Guild;

typedef std::map<unsigned, std::string> PositionsMap;

class GuildMember final : public Avatar
{
    public:
        A_DELETE_COPY(GuildMember)

        const Guild *getGuild() const noexcept2 A_WARN_UNUSED
        { return mGuild; }

        int getPos() const noexcept2 A_WARN_UNUSED
        { return mPos; }

        void setPos(const int pos)
        { mPos = pos; }

        std::string getAdditionString() const override final A_WARN_UNUSED;

    protected:
        friend class Guild;

        GuildMember(Guild *const guild,
                    const BeingId accountId,
                    const int charId,
                    const std::string &name);

        GuildMember(Guild *const guild, const std::string &name);

        Guild *mGuild;
        int mPos;
};

class Guild final : public AvatarListModel
{
    public:
        A_DELETE_COPY(Guild)

        ~Guild() override final;

        /**
         * Set the guild's name.
         */
        void setName(const std::string &name)
        { mName = name; }

        /**
         * Adds member to the list.
         */
        GuildMember *addMember(const BeingId accountId,
                               const int charId,
                               const std::string &name);

        /**
         * Adds member to the list.
         */
        GuildMember *addMember(const std::string &name);

        /**
         * Find a member by ID.
         *
         * @return the member with the given ID, or NULL if they don't exist.
         */
        GuildMember *getMember(const BeingId id) const;

        /**
         * Find a member by account ID and char ID.
         *
         * @return the member with the given ID, or NULL if they don't exist.
         */
        GuildMember *getMember(const BeingId accountId,
                               const int charId)
                               const A_WARN_UNUSED;

        /**
         * Find a member by char ID.
         *
         * @return the member with the given ID, or NULL if they don't exist.
         */
        GuildMember *getMemberByCharId(const int charId) const A_WARN_UNUSED;

        /**
         * Find a member by name.
         *
         * @return the member with the given name, or NULL if they don't exist.
         */
        GuildMember *getMember(const std::string &name) const A_WARN_UNUSED;

        /**
         * Get the name of the guild.
         * @return returns name of the guild
         */
        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        /**
         * Get the id of the guild.
         * @return Returns the id of the guild
         */
        int16_t getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        /**
         * Removes a member from the guild.
         */
        void removeMember(const GuildMember *const member);

        /**
         * Removes a member from the guild by account id.
         */
        void removeMember(const BeingId id);

        /**
         * Removes a member from the guild.
         */
        void removeMember(const std::string &name);

        void removeFromMembers();

        void clearMembers()
        { delete_all(mMembers); mMembers.clear(); }

        /**
         * Get size of members list.
         * @return Returns the number of members in the guild.
         */
        int getNumberOfElements() override final A_WARN_UNUSED
        { return CAST_S32(mMembers.size()); }

        Avatar *getAvatarAt(const int index) override final A_WARN_UNUSED;

        /**
         * Get whether user can invite users to this guild.
         * @return Returns true if user can invite users
         */
        bool getInviteRights() const noexcept2 A_WARN_UNUSED
        { return mCanInviteUsers; }

        void setRights(const int16_t rights);

        bool isMember(const GuildMember *const member) const A_WARN_UNUSED;

        bool isMember(const BeingId id) const A_WARN_UNUSED;

        bool isMember(const std::string &name) const A_WARN_UNUSED;

        void getNames(StringVect &names) const;

        void addPos(const int id, const std::string &name);

        void sort();

        std::string getPos(const int id) const A_WARN_UNUSED;

        static Guild *getGuild(const int16_t id) A_WARN_UNUSED;

        const PositionsMap &getPositions() const noexcept2 A_WARN_UNUSED
        { return mPositions; }

        void setEmblemId(const int id)
        { mEmblemId = id; }

        int getEmblemId() const noexcept2 A_WARN_UNUSED
        { return mEmblemId; }

        static void clearGuilds();

        void setServerGuild(const bool b)
        { mServerGuild = b; }

        bool getServerGuild() const noexcept2 A_WARN_UNUSED
        { return mServerGuild; }

        typedef STD_VECTOR<GuildMember*> MemberList;

        const MemberList *getMembers() const RETURNS_NONNULL A_WARN_UNUSED
        { return &mMembers; }

    private:
        typedef std::map<int, Guild*> GuildMap;
        static GuildMap guilds;

        /**
         * Constructor with guild id passed to it.
         */
        explicit Guild(const int16_t id);

        MemberList mMembers;
        PositionsMap mPositions;
        std::string mName;
        int mEmblemId;
        int16_t mId;
        bool mCanInviteUsers;
        bool mServerGuild;
};

#endif  // GUILD_H
