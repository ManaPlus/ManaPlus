/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef PARTY_H
#define PARTY_H

#include "gui/models/avatarlistmodel.h"

#include "utils/dtor.h"
#include "utils/stringvector.h"

#include <map>
#include <set>

class Party;

class PartyMember final : public Avatar
{
public:
    A_DELETE_COPY(PartyMember)

    const Party *getParty() const A_WARN_UNUSED
    { return mParty; }

    bool getLeader() const A_WARN_UNUSED
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
    A_DELETE_COPY(Party)

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
    PartyMember *getMember(const int id) const A_WARN_UNUSED;

    /**
     * Find a member by name.
     *
     * @return the member with the given name, or NULL if they don't exist.
     */
    PartyMember *getMember(const std::string &name) const A_WARN_UNUSED;

    /**
     * Get the name of the party.
     * @return returns name of the party
     */
    const std::string &getName() const A_WARN_UNUSED
    { return mName; }

    /**
     * Get the id of the party.
     * @return Returns the id of the party
     */
    int16_t getId() const A_WARN_UNUSED
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
    int getNumberOfElements() override final A_WARN_UNUSED
    { return static_cast<int>(mMembers.size()); }

    Avatar *getAvatarAt(const int i) override final;

    /**
     * Get whether user can invite users to this party.
     * @return Returns true if user can invite users
     */
    bool getInviteRights() const A_WARN_UNUSED
    { return mCanInviteUsers; }

    void setRights(const int16_t rights);

    bool isMember(const PartyMember *const member) const A_WARN_UNUSED;

    bool isMember(const int id) const A_WARN_UNUSED;

    bool isMember(const std::string &name) const A_WARN_UNUSED;

    void getNames(StringVect &names) const;

    void getNamesSet(std::set<std::string> &names) const;

    void sort();

    typedef std::vector<PartyMember*> MemberList;

    const MemberList *getMembers() const A_WARN_UNUSED
    { return &mMembers; }

    static Party *getParty(const int16_t id) A_WARN_UNUSED;

    static void clearParties();

private:
    typedef std::map<int, Party*> PartyMap;
    static PartyMap parties;

    /**
     * Constructor with party id passed to it.
     */
    explicit Party(const int16_t id);

    ~Party();

    MemberList mMembers;
    std::string mName;
    int16_t mId;
    bool mCanInviteUsers;
};

#endif  // PARTY_H
