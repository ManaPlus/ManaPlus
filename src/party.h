/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef PARTY_H
#define PARTY_H

#include "gui/widgets/avatarlistbox.h"

#include "utils/dtor.h"
#include "utils/stringvector.h"

#include <map>
#include <set>

class Party;

class PartyMember final : public Avatar
{
public:
    A_DELETE_COPY(PartyMember);

    Party *getParty() const
    { return mParty; }

    bool getLeader() const
    { return mLeader; }

    void setLeader(const bool leader)
    { mLeader = leader; setDisplayBold(leader); }

protected:
    friend class Party;

    PartyMember(Party *const party, const int id, const std::string &name);

    Party *mParty;
    bool mLeader;
};

class Party final : public AvatarListModel
{
public:
    A_DELETE_COPY(Party);

    /**
     * Set the party's name.
     */
    void setName(const std::string &name)
    { mName = name; }

    /**
     * Adds member to the list.
     */
    PartyMember *addMember(const int id, const std::string &name);

    /**
     * Find a member by ID.
     *
     * @return the member with the given ID, or NULL if they don't exist.
     */
    PartyMember *getMember(const int id) const;

    /**
     * Find a member by name.
     *
     * @return the member with the given name, or NULL if they don't exist.
     */
    PartyMember *getMember(const std::string &name) const;

    /**
     * Get the name of the party.
     * @return returns name of the party
     */
    const std::string &getName() const
    { return mName; }

    /**
     * Get the id of the party.
     * @return Returns the id of the party
     */
    short getId() const
    { return mId; }

    /**
     * Removes a member from the party.
     */
    void removeMember(const PartyMember *const member);

    /**
     * Removes a member from the party.
     */
    void removeMember(const int id);

    /**
     * Removes a member from the party.
     */
    void removeMember(const std::string &name);

    void clearMembers()
    { delete_all(mMembers); mMembers.clear(); }

    void removeFromMembers();

    /**
     * Get size of members list.
     * @return Returns the number of members in the party.
     */
    int getNumberOfElements() override
    { return static_cast<int>(mMembers.size()); }

    Avatar *getAvatarAt(const int i) override;

    /**
     * Get whether user can invite users to this party.
     * @return Returns true if user can invite users
     */
    bool getInviteRights() const
    { return mCanInviteUsers; }

    void setRights(const short rights);

    bool isMember(const PartyMember *const member) const;

    bool isMember(const int id) const;

    bool isMember(const std::string &name) const;

    void getNames(StringVect &names) const;

    void getNamesSet(std::set<std::string> &names) const;

    void sort();

    typedef std::vector<PartyMember*> MemberList;

    MemberList *getMembers()
    { return &mMembers; }

    static Party *getParty(const short id);

    static void clearParties();

private:
    typedef std::map<int, Party*> PartyMap;
    static PartyMap parties;

    /**
     * Constructor with party id passed to it.
     */
    Party(const short id);

    virtual ~Party();

    MemberList mMembers;
    std::string mName;
    short mId;
    bool mCanInviteUsers;
};

#endif // PARTY_H
