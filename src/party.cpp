/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "party.h"

#include "actorspritemanager.h"

#include "debug.h"

class SortPartyFunctor final
{
    public:
        bool operator() (const PartyMember *const p1,
                         const PartyMember *const p2) const
        {
            if (!p1 || !p2)
                return false;
            if (p1->getLeader())
                return true;
            if (p2->getLeader())
                return false;

            if (p1->getName() != p2->getName())
            {
                std::string s1 = p1->getName();
                std::string s2 = p2->getName();
                toLower(s1);
                toLower(s2);
                return s1 < s2;
            }
            return false;
        }
} partySorter;

PartyMember::PartyMember(Party *const party, const int id,
                         const std::string &name) :
        Avatar(name), mParty(party), mLeader(false)
{
    mId = id;
}

Party::PartyMap Party::parties;

Party::Party(const short id) :
    mId(id),
    mCanInviteUsers(false)
{
    parties[id] = this;
}

Party::~Party()
{
    clearMembers();
}

PartyMember *Party::addMember(const int id, const std::string &name)
{
    PartyMember *m;
    if ((m = getMember(id)))
        return m;

    m = new PartyMember(this, id, name);

    mMembers.push_back(m);

    return m;
}

PartyMember *Party::getMember(const int id) const
{
    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr)->mId == id)
            return (*itr);
        ++itr;
    }

    return nullptr;
}

PartyMember *Party::getMember(const std::string &name) const
{
    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr) && (*itr)->getName() == name)
            return (*itr);

        ++itr;
    }

    return nullptr;
}

void Party::removeMember(const PartyMember *const member)
{
    if (!member)
        return;

    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin();
        const MemberList::iterator itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr) && (*itr)->mId == member->mId &&
                (*itr)->getName() == member->getName())
            {
                PartyMember *m = (*itr);
                mMembers.erase(itr);
                delete m;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Party::removeMember(const int id)
{
    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin();
        const MemberList::iterator itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr) && (*itr)->mId == id)
            {
                PartyMember *member = (*itr);
                mMembers.erase(itr);
                delete member;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Party::removeMember(const std::string &name)
{
    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin();
        const MemberList::iterator itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr) && (*itr)->getName() == name)
            {
                PartyMember *member = (*itr);
                mMembers.erase(itr);
                delete member;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Party::removeFromMembers()
{
    if (!actorSpriteManager)
        return;

    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();

    while (itr != itr_end)
    {
        Being *const b = actorSpriteManager->findBeing((*itr)->getID());
        if (b)
            b->setParty(nullptr);
        ++itr;
    }
}

Avatar *Party::getAvatarAt(const int index)
{
    return mMembers[index];
}

void Party::setRights(const short rights)
{
    // to invite, rights must be greater than 0
    if (rights > 0)
        mCanInviteUsers = true;
}

bool Party::isMember(const PartyMember *const member) const
{
    if (!member)
        return false;

    if (member->mParty && member->mParty != this)
        return false;

    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr) && (*itr)->mId == member->mId &&
            (*itr)->getName() == member->getName())
        {
            return true;
        }
        ++itr;
    }

    return false;
}

bool Party::isMember(const int id) const
{
    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr) && (*itr)->mId == id)
            return true;
        ++itr;
    }

    return false;
}

bool Party::isMember(const std::string &name) const
{
    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        if ((*itr) && (*itr)->getName() == name)
            return true;
        ++itr;
    }

    return false;
}

void Party::getNames(StringVect &names) const
{
    names.clear();
    MemberList::const_iterator it = mMembers.begin();
    const MemberList::const_iterator it_end = mMembers.end();
    while (it != it_end)
    {
        if (*it)
            names.push_back((*it)->getName());
        ++it;
    }
}

void Party::getNamesSet(std::set<std::string> &names) const
{
    names.clear();
    MemberList::const_iterator it = mMembers.begin();
    const MemberList::const_iterator it_end = mMembers.end();
    while (it != it_end)
    {
        if (*it)
            names.insert((*it)->getName());
        ++it;
    }
}

Party *Party::getParty(const short id)
{
    const PartyMap::const_iterator it = parties.find(id);
    if (it != parties.end())
        return it->second;
    Party *const party = new Party(id);
    parties[id] = party;
    return party;
}

void Party::sort()
{
    std::sort(mMembers.begin(), mMembers.end(), partySorter);
}

void Party::clearParties()
{
    PartyMap::iterator it = parties.begin();
    while (it != parties.end())
    {
        delete (*it).second;
        ++ it;
    }
    parties.clear();
}
