/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "actormanager.h"

#include "being/being.h"

#include "utils/stringutils.h"

#include "debug.h"

static class SortPartyFunctor final
{
    public:
        A_DEFAULT_COPY(SortPartyFunctor)

        bool operator() (const PartyMember *const p1,
                         const PartyMember *const p2) const
        {
            if (p1 == nullptr || p2 == nullptr)
                return false;
            const int leader1 = CAST_S32(p1->getLeader());
            const int leader2 = CAST_S32(p2->getLeader());
            if (leader1 != leader2)
                return leader1 >= leader2;
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

PartyMember::PartyMember(Party *const party,
                         const BeingId id,
                         const std::string &name) :
    Avatar(name),
    mParty(party),
    mLeader(false)
{
    mId = id;
}

Party::PartyMap Party::parties;

Party::Party(const int16_t id) :
    mMembers(),
    mName(),
    mId(id),
    mCanInviteUsers(false)
{
    parties[id] = this;
}

Party::~Party()
{
    clearMembers();
}

PartyMember *Party::addMember(const BeingId id,
                              const std::string &name)
{
    PartyMember *m = getMember(id);
    if (m != nullptr)
        return m;

    m = new PartyMember(this, id, name);

    mMembers.push_back(m);

    return m;
}

PartyMember *Party::getMember(const BeingId id) const
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
        if (((*itr) != nullptr) && (*itr)->getName() == name)
            return (*itr);

        ++itr;
    }

    return nullptr;
}

void Party::removeMember(const PartyMember *const member)
{
    if (member == nullptr)
        return;

    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin();
        const MemberList::iterator itr_end = mMembers.end();
        while (itr != itr_end)
        {
            PartyMember *const m = *itr;
            if ((m != nullptr) && m->mId == member->mId
                && m->getName() == member->getName())
            {
                mMembers.erase(itr);
                delete m;
                deleted = true;
                break;
            }
            ++itr;
        }
    }
}

void Party::removeMember(const BeingId id)
{
    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin();
        const MemberList::iterator itr_end = mMembers.end();
        while (itr != itr_end)
        {
            PartyMember *const member = *itr;
            if ((member != nullptr) && member->mId == id)
            {
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
            PartyMember *const member = *itr;
            if ((member != nullptr) && member->getName() == name)
            {
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
    if (actorManager == nullptr)
        return;

    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();

    while (itr != itr_end)
    {
        Being *const b = actorManager->findBeing((*itr)->getID());
        if (b != nullptr)
            b->setParty(nullptr);
        ++itr;
    }
}

Avatar *Party::getAvatarAt(const int index)
{
    return mMembers[index];
}

void Party::setRights(const int16_t rights)
{
    // to invite, rights must be greater than 0
    if (rights > 0)
        mCanInviteUsers = true;
}

bool Party::isMember(const PartyMember *const member) const
{
    if (member == nullptr)
        return false;

    if ((member->mParty != nullptr) && member->mParty != this)
        return false;

    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        const PartyMember *const m = *itr;
        if (m != nullptr &&
            m->mId == member->mId &&
            m->getName() == member->getName())
        {
            return true;
        }
        ++itr;
    }

    return false;
}

bool Party::isMember(const BeingId id) const
{
    MemberList::const_iterator itr = mMembers.begin();
    const MemberList::const_iterator itr_end = mMembers.end();
    while (itr != itr_end)
    {
        const PartyMember *const m = *itr;
        if ((m != nullptr) && m->mId == id)
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
        const PartyMember *const m = *itr;
        if ((m != nullptr) && m->getName() == name)
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
        const PartyMember *const m = *it;
        if (m != nullptr)
            names.push_back(m->getName());
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
        const PartyMember *const m = *it;
        if (m != nullptr)
            names.insert(m->getName());
        ++it;
    }
}

Party *Party::getParty(const int16_t id)
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
