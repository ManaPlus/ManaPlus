/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_SOCIALWINDOW_H
#define GUI_WINDOWS_SOCIALWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/playerrelationslistener.h"

class Button;
class ConfirmDialog;
class Guild;
class Label;
class Map;
class Party;
class SocialTab;
class TabbedArea;
class TextDialog;

/**
 * Party window.
 *
 * \ingroup Interface
 */
class SocialWindow final : public Window,
                           private ActionListener,
                           public PlayerRelationsListener
{
    public:
        SocialWindow();

        A_DELETE_COPY(SocialWindow)

        ~SocialWindow();

        void postInit() override final;

        bool addTab(Guild *const guild);

        bool removeTab(Guild *const guild);

        bool addTab(Party *const party);

        bool removeTab(Party *const party);

        void action(const ActionEvent &event) override final;

        void showGuildInvite(const std::string &restrict guildName,
                             const int guildId,
                             const std::string &restrict inviterName);

        void showPartyInvite(const std::string &restrict partyName,
                             const std::string &restrict inviter,
                             const int partyId);

        void updateActiveList();

        void updateAvatar(const std::string &name);

        void resetDamage(const std::string &name);

        void slowLogic();

        void updatePortals();

        void updatePortalNames();

        void updateParty();

        int getPortalIndex(const int x, const int y) A_WARN_UNUSED;

        void addPortal(const int x, const int y);

        void removePortal(const int x, const int y);

        void nextTab();

        void prevTab();

        const Map* getMap() const noexcept2 A_WARN_UNUSED
        { return mMap; }

        void setMap(Map *const map)
        { mMap = map; mProcessedPortals = false; }

        bool getProcessedPortals() const noexcept2 A_WARN_UNUSED
        { return mProcessedPortals; }

        void setProcessedPortals(const bool n) noexcept2
        { mProcessedPortals = n; }

        void selectPortal(const unsigned num);

        void updateAttackFilter();

        void updatePickupFilter();

        void widgetResized(const Event &event) override final;

        void updateCounter(const SocialTab *const tab,
                           const std::string &count);

        void updateMenu(const SocialTab *const tab,
                        const std::string &menu);

        void updateGuildCounter(const int online = 0, const int total = 0);

        void updatedPlayer(const std::string &name) override final;

        void updateAll() override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif  // USE_PROFILER

    protected:
        friend class SocialTab;
        typedef std::map<Guild*, SocialTab*> GuildMap;
        typedef std::map<Party*, SocialTab*> PartyMap;

        void updateButtons();

        int mGuildInvited;
        ConfirmDialog *mGuildAcceptDialog;
        TextDialog *mGuildCreateDialog;
        std::string mPartyInviter;
        GuildMap mGuilds;
        PartyMap mParties;

        ConfirmDialog *mPartyAcceptDialog;

        SocialTab *mAttackFilter;
        SocialTab *mPickupFilter;
        SocialTab *mPlayers A_NONNULLPOINTER;
        SocialTab *mNavigation A_NONNULLPOINTER;
        SocialTab *mFriends A_NONNULLPOINTER;

        Button *mMenuButton A_NONNULLPOINTER;
        Label *mCountLabel A_NONNULLPOINTER;
        TabbedArea *mTabs A_NONNULLPOINTER;
        Map *mMap;

        time_t mLastUpdateTime;
        int mPartyId;
        bool mNeedUpdate;
        bool mProcessedPortals;
};

extern SocialWindow *socialWindow;

#endif  // GUI_WINDOWS_SOCIALWINDOW_H
