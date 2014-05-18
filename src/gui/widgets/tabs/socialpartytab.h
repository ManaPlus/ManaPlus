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

#ifndef GUI_WIDGETS_TABS_SOCIALPARTYTAB_H
#define GUI_WIDGETS_TABS_SOCIALPARTYTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "party.h"

#include "being/localplayer.h"

#include "net/net.h"
#include "net/partyhandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "localconsts.h"

class SocialPartyTab final : public SocialTab,
                             public ActionListener
{
    public:
        SocialPartyTab(const Widget2 *const widget,
                       Party *const party,
                       const bool showBackground) :
            SocialTab(widget),
            ActionListener(),
            mParty(party)
        {
            // TRANSLATORS: tab in social window
            setCaption(_("Party"));

            setTabColor(&getThemeColor(Theme::PARTY_SOCIAL_TAB),
                &getThemeColor(Theme::PARTY_SOCIAL_TAB_OUTLINE));
            setHighlightedTabColor(&getThemeColor(
                Theme::PARTY_SOCIAL_TAB_HIGHLIGHTED), &getThemeColor(
                Theme::PARTY_SOCIAL_TAB_HIGHLIGHTED_OUTLINE));
            setSelectedTabColor(&getThemeColor(
                Theme::PARTY_SOCIAL_TAB_SELECTED),
                &getThemeColor(Theme::PARTY_SOCIAL_TAB_SELECTED_OUTLINE));

            createControls(party, showBackground);
        }

        A_DELETE_COPY(SocialPartyTab)

        ~SocialPartyTab()
        {
            delete2(mList)
            delete2(mScroll)
        }

        void action(const ActionEvent &event) override final
        {
            const std::string &eventId = event.getId();
            if (eventId == "do invite")
            {
                const std::string name = mInviteDialog->getText();
                Net::getPartyHandler()->invite(name);

                if (localChatTab)
                {
                    // TRANSLATORS: chat message
                    localChatTab->chatLog(strprintf(
                        _("Invited user %s to party."),
                        name.c_str()),
                        ChatMsgType::BY_SERVER);
                }
                mInviteDialog = nullptr;
            }
            else if (eventId == "~do invite")
            {
                mInviteDialog = nullptr;
            }
            else if (eventId == "yes")
            {
                Net::getPartyHandler()->leave();
                if (localChatTab)
                {
                    // TRANSLATORS: tab in social window
                    localChatTab->chatLog(strprintf(
                        _("Party %s quit requested."),
                        mParty->getName().c_str()),
                        ChatMsgType::BY_SERVER);
                }
                mConfirmDialog = nullptr;
            }
            else if (eventId == "~yes")
            {
                mConfirmDialog = nullptr;
            }
        }

        void invite() override final
        {
            // TRANSLATORS: party invite message
            mInviteDialog = new TextDialog(_("Member Invite to Party"),
                // TRANSLATORS: party invite message
                strprintf(_("Who would you like to invite to party %s?"),
                mParty->getName().c_str()), socialWindow);
            mInviteDialog->postInit();
            mInviteDialog->setActionEventId("do invite");
            mInviteDialog->addActionListener(this);
        }

        void leave() override final
        {
            // TRANSLATORS: party leave message
            mConfirmDialog = new ConfirmDialog(_("Leave Party?"),
                // TRANSLATORS: party leave message
                strprintf(_("Are you sure you want to leave party %s?"),
                mParty->getName().c_str()), SOUND_REQUEST, socialWindow);
            mConfirmDialog->postInit();
            mConfirmDialog->addActionListener(this);
        }

        void buildCounter(const int online0 A_UNUSED,
                          const int total0 A_UNUSED)
        {
            if (!player_node)
                return;

            const Party *const party = player_node->getParty();
            if (!party)
                return;

            const Party::MemberList *const members = party->getMembers();
            int online = 0;
            int total = 0;
            FOR_EACHP (Party::MemberList::const_iterator, it, members)
            {
                if ((*it)->getOnline())
                    online ++;
                total ++;
            }

            // TRANSLATORS: social window label
            mCounterString = strprintf(_("Players: %u/%u"),
                static_cast<uint32_t>(online),
                static_cast<uint32_t>(total));
            updateCounter();
        }

    private:
        Party *mParty;
};

#endif  // GUI_WIDGETS_TABS_SOCIALPARTYTAB_H
