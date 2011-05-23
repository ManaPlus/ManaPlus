/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "guild.h"

#include "actorspritemanager.h"

#include "debug.h"

class SortGuildFunctor
{
    public:
        bool operator() (GuildMember* m1,  GuildMember* m2)
        {
            if (!m1 || !m2)
                return false;
            if (m1->getPos() != m2->getPos())
                return m1->getPos() < m2->getPos();

            return m1->getName() < m2->getName();
        }
} guildSorter;

GuildMember::GuildMember(Guild *guild, int accountId, int charId,
                         const std::string &name):
        Avatar(name), mGuild(guild), mPos(0)
{
    mId = accountId;
    mCharId = charId;
}

GuildMember::GuildMember(Guild *guild, const std::string &name):
        Avatar(name), mGuild(guild), mPos(0)
{
}

std::string GuildMember::getAdditionString() const
{
    if (!mGuild)
        return "";

    return " - " + mGuild->getPos(mPos);
}

Guild::GuildMap Guild::guilds;

Guild::Guild(short id):
    mId(id),
    mCanInviteUsers(false),
    mEmblemId(0)
{
    guilds[id] = this;
}

GuildMember *Guild::addMember(int accountId, int charId,
                              const std::string &name)
{
    GuildMember *m;
    if ((m = getMember(accountId, charId)))
        return m;

    m = new GuildMember(this, accountId, charId, name);

    mMembers.push_back(m);

    return m;
}

GuildMember *Guild::addMember(const std::string &name)
{
    GuildMember *m;
    if ((m = getMember(name)))
        return m;

    m = new GuildMember(this, name);

    mMembers.push_back(m);

    return m;
}

GuildMember *Guild::getMember(int id) const
{
    MemberList::const_iterator itr = mMembers.begin(),
                               itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == id)
            return (*itr);
        ++itr;
    }

    return NULL;
}

GuildMember *Guild::getMember(int accountId, int charId) const
{
    MemberList::const_iterator itr = mMembers.begin(),
                               itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == accountId && (*itr)->mCharId == charId)
            return (*itr);
        ++itr;
    }

    return NULL;
}

GuildMember *Guild::getMember(const std::string &name) const
{
    MemberList::const_iterator itr = mMembers.begin(),
                               itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->getName() == name)
            return (*itr);
        ++itr;
    }

    return NULL;
}

void Guild::removeMember(GuildMember *member)
{
    MemberList::iterator itr = mMembers.begin(),
                               itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == member->mId &&
            (*itr)->mCharId == member->mCharId &&
            (*itr)->getName() == member->getName())
        {
            GuildMember *member = *itr;
            mMembers.erase(itr);
            delete member;
            return;
        }
        ++itr;
    }
}

void Guild::removeMember(int id)
{
    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin(),
            itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr)->mId == id)
            {
                GuildMember *member = *itr;
                mMembers.erase(itr);
                delete member;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Guild::removeMember(const std::string &name)
{
    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin(),
            itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr)->getName() == name)
            {
                GuildMember *member = *itr;
                mMembers.erase(itr);
                delete member;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Guild::removeFromMembers()
{
    if (!actorSpriteManager)
        return;

    MemberList::iterator itr = mMembers.begin(),
                               itr_end = mMembers.end();
    while (itr != itr_end)
    {
        Being *b = actorSpriteManager->findBeing((*itr)->getID());
        if (b)
            b->removeGuild(getId());
        ++itr;
    }
}

Avatar *Guild::getAvatarAt(int index)
{
    return mMembers[index];
}

void Guild::setRights(short rights)
{
    // to invite, rights must be greater than 0
    if (rights > 0)
        mCanInviteUsers = true;
}

bool Guild::isMember(GuildMember *member) const
{
    if (member->mGuild > 0 && member->mGuild != this)
        return false;

    MemberList::const_iterator itr = mMembers.begin(),
                                     itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == member->mId &&
            (*itr)->getName() == member->getName())
        {
            return true;
        }
        ++itr;
    }

    return false;
}

bool Guild::isMember(int id) const
{
    MemberList::const_iterator itr = mMembers.begin(),
                                     itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == id)
            return true;
        ++itr;
    }

    return false;
}

bool Guild::isMember(const std::string &name) const
{
    MemberList::const_iterator itr = mMembers.begin(),
                                     itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->getName() == name)
            return true;
        ++itr;
    }

    return false;
}

void Guild::getNames(std::vector<std::string> &names) const
{
    names.clear();
    MemberList::const_iterator it = mMembers.begin(),
        it_end = mMembers.end();

    while (it != it_end)
    {
        names.push_back((*it)->getName());
        ++it;
    }
}

void Guild::addPos(int id, std::string name)
{
    mPositions[id] = name;
}

Guild *Guild::getGuild(short id)
{
    GuildMap::iterator it = guilds.find(id);
    if (it != guilds.end())
        return it->second;

    return new Guild(id);
}

std::string Guild::getPos(int id) const
{
    PositionsMap::const_iterator it = mPositions.find(id);
    if (it == mPositions.end())
        return "";
    else
        return it->second;
}

void Guild::sort()
{
    std::sort(mMembers.begin(), mMembers.end(), guildSorter);
}