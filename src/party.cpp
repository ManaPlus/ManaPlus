/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "party.h"

#include "actorspritemanager.h"

#include "debug.h"

class SortPartyFunctor
{
    public:
        bool operator() (PartyMember* p1,  PartyMember* p2)
        {
            if (!p1 || !p2)
                return false;
            if (p1->getLeader())
                return true;
            if (p2->getLeader())
                return false;

            return p1->getName() < p2->getName();
        }
} partySorter;

PartyMember::PartyMember(Party *party, int id, const std::string &name):
        Avatar(name), mParty(party), mLeader(false)
{
    mId = id;
}

Party::PartyMap Party::parties;

Party::Party(short id):
    mCanInviteUsers(false)
{
    mId = id;
    parties[id] = this;
}

Party::~Party()
{
    clearMembers();
}

PartyMember *Party::addMember(int id, const std::string &name)
{
    PartyMember *m;
    if ((m = getMember(id)))
        return m;

    m = new PartyMember(this, id, name);

    mMembers.push_back(m);

    return m;
}

PartyMember *Party::getMember(int id) const
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

PartyMember *Party::getMember(const std::string &name) const
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

void Party::removeMember(PartyMember *member)
{
    if (!member)
        return;

    bool deleted = true;
    while (deleted)
    {
        deleted = false;
        MemberList::iterator itr = mMembers.begin(),
            itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr)->mId == member->mId &&
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

void Party::removeMember(int id)
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
        MemberList::iterator itr = mMembers.begin(),
            itr_end = mMembers.end();
        while (itr != itr_end)
        {
            if ((*itr)->getName() == name)
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
    MemberList::const_iterator itr_end = mMembers.end();

    while (itr != itr_end)
    {
        Being *b = actorSpriteManager->findBeing((*itr)->getID());
        if (b)
            b->setParty(0);
        ++itr;
    }
}

Avatar *Party::getAvatarAt(int index)
{
    return mMembers[index];
}

void Party::setRights(short rights)
{
    // to invite, rights must be greater than 0
    if (rights > 0)
        mCanInviteUsers = true;
}

bool Party::isMember(PartyMember *member) const
{
    if (!member)
        return false;

    if (member->mParty > 0 && member->mParty != this)
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

bool Party::isMember(int id) const
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

bool Party::isMember(const std::string &name) const
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

void Party::getNames(std::vector<std::string> &names) const
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

void Party::getNamesSet(std::set<std::string> &names) const
{
    names.clear();
    MemberList::const_iterator it = mMembers.begin(),
                                     it_end = mMembers.end();
    while (it != it_end)
    {
        names.insert((*it)->getName());
        ++it;
    }
}

Party *Party::getParty(short id)
{
    PartyMap::const_iterator it = parties.find(id);
    if (it != parties.end())
        return it->second;
    Party *party = new Party(id);
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
