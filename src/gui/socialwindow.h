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

#ifndef SOCIALWINDOW_H
#define SOCIALWINDOW_H

#include "gui/widgets/window.h"

#include <guichan/actionevent.hpp>
#include <guichan/actionlistener.hpp>

#include <string>
#include <map>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

class AttackTab;
class Button;
class ConfirmDialog;
class CreatePopup;
class Guild;
class Map;
class NavigateTab;
class Party;
class SocialTab;
class Tab;
class TabbedArea;
class TextDialog;
class PlayersTab;

/**
 * Party window.
 *
 * \ingroup Interface
 */
class SocialWindow : public Window, gcn::ActionListener
{
public:
    SocialWindow();

    ~SocialWindow();

    bool addTab(Guild *guild);

    bool removeTab(Guild *guild);

    bool addTab(Party *party);

    bool removeTab(Party *party);

    /**
     * Handle events.
     */
    void action(const gcn::ActionEvent &event);

    void showGuildInvite(const std::string &guildName, const int guildId,
                         const std::string &inviterName);

    void showGuildCreate();

    void showPartyInvite(const std::string &partyName,
                         const std::string &inviter = "");

    void showPartyCreate();

    void updateActiveList();

    void updateAvatar(std::string name);

    void resetDamage(std::string name);

    void logic();

    void updatePortals();

    void updatePortalNames();

    int getPortalIndex(int x, int y);

    void addPortal(int x, int y);

    void removePortal(int x, int y);

    void nextTab();

    void prevTab();

    Map* getMap()
    { return mMap; }

    void setMap(Map *map)
    { mMap = map; mProcessedPortals = false; }

    bool getProcessedPortals()
    { return mProcessedPortals; }

    void setProcessedPortals(int n)
    { mProcessedPortals = n; }

    void selectPortal(unsigned num);

    void updateAttackFilter();

protected:
    friend class SocialTab;

    void updateButtons();

    int mGuildInvited;
    ConfirmDialog *mGuildAcceptDialog;
    TextDialog *mGuildCreateDialog;

    std::string mPartyInviter;
    ConfirmDialog *mPartyAcceptDialog;
    TextDialog *mPartyCreateDialog;

    typedef std::map<Guild*, SocialTab*> GuildMap;
    GuildMap mGuilds;

    typedef std::map<Party*, SocialTab*> PartyMap;
    PartyMap mParties;

    SocialTab *mAttackFilter;
    SocialTab *mPlayers;
    SocialTab *mNavigation;

    CreatePopup *mCreatePopup;

    Button *mCreateButton;
    Button *mInviteButton;
    Button *mLeaveButton;
    TabbedArea *mTabs;
    Map *mMap;

    int mLastUpdateTime;
    bool mNeedUpdate;
    bool mProcessedPortals;
};

extern SocialWindow *socialWindow;

#endif // SOCIALWINDOW_H
