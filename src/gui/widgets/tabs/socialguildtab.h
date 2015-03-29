/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_SOCIALGUILDTAB_H
#define GUI_WIDGETS_TABS_SOCIALGUILDTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "being/localplayer.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "net/guildhandler.h"

#include "localconsts.h"

class SocialGuildTab final : public SocialTab,
                             public ActionListener
{
    public:
        SocialGuildTab(const Widget2 *const widget,
                       Guild *const guild,
                       const bool showBackground) :
            SocialTab(widget),
            ActionListener(),
            mGuild(guild)
        {
            // TRANSLATORS: tab in social window
            setCaption(_("Guild"));

            setTabColor(&getThemeColor(Theme::GUILD_SOCIAL_TAB),
                &getThemeColor(Theme::GUILD_SOCIAL_TAB_OUTLINE));
            setHighlightedTabColor(&getThemeColor(
                Theme::GUILD_SOCIAL_TAB_HIGHLIGHTED), &getThemeColor(
                Theme::GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE));
            setSelectedTabColor(&getThemeColor(
                Theme::GUILD_SOCIAL_TAB_SELECTED),
                &getThemeColor(Theme::GUILD_SOCIAL_TAB_SELECTED_OUTLINE));

            createControls(guild, showBackground);
        }

        A_DELETE_COPY(SocialGuildTab)

        ~SocialGuildTab()
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
                guildHandler->invite(name);

                if (localChatTab)
                {
                    localChatTab->chatLog(strprintf(
                        // TRANSLATORS: chat message
                        _("Invited user %s to guild %s."),
                        name.c_str(),
                        mGuild->getName().c_str()),
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
                guildHandler->leave(mGuild->getId());
                if (localChatTab)
                {
                    // TRANSLATORS: chat message
                    localChatTab->chatLog(strprintf(
                        _("Guild %s quit requested."),
                        mGuild->getName().c_str()),
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
            // TRANSLATORS: guild invite message
            mInviteDialog = new TextDialog(_("Member Invite to Guild"),
                // TRANSLATORS: guild invite message
                strprintf(_("Who would you like to invite to guild %s?"),
                mGuild->getName().c_str()), socialWindow);
            mInviteDialog->postInit();
            mInviteDialog->setActionEventId("do invite");
            mInviteDialog->addActionListener(this);
        }

        void leave() override final
        {
            // TRANSLATORS: guild leave message
            mConfirmDialog = new ConfirmDialog(_("Leave Guild?"),
                // TRANSLATORS: guild leave message
                strprintf(_("Are you sure you want to leave guild %s?"),
                mGuild->getName().c_str()), SOUND_REQUEST, socialWindow);
            mConfirmDialog->postInit();
            mConfirmDialog->addActionListener(this);
        }

        void buildCounter(const int online0, const int total0) override final
        {
            if (online0 || total0)
            {
                // TRANSLATORS: social window label
                mCounterString = strprintf(_("Members: %u/%u"),
                    static_cast<uint32_t>(online0),
                    static_cast<uint32_t>(total0));
            }
            else
            {
                if (!localPlayer)
                    return;

                const Guild *const guild = localPlayer->getGuild();
                if (!guild)
                    return;

                const Guild::MemberList *const members = guild->getMembers();
                int online = 0;
                int total = 0;
                FOR_EACHP (Guild::MemberList::const_iterator, it, members)
                {
                    if ((*it)->getOnline())
                        online ++;
                    total ++;
                }

                // TRANSLATORS: social window label
                mCounterString = strprintf(_("Players: %u/%u"),
                    static_cast<uint32_t>(online),
                    static_cast<uint32_t>(total));
            }
            updateCounter();
        }

    private:
        Guild *mGuild;
};

#endif  // GUI_WIDGETS_TABS_SOCIALGUILDTAB_H
