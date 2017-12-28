/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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
#include "utils/foreach.h"
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
                       const Opaque showBackground) :
            SocialTab(widget),
            ActionListener(),
            mGuild(guild)
        {
            // TRANSLATORS: tab in social window
            setCaption(_("Guild"));

            setTabColor(&getThemeColor(ThemeColorId::GUILD_SOCIAL_TAB, 255U),
                &getThemeColor(ThemeColorId::GUILD_SOCIAL_TAB_OUTLINE, 255U));
            setHighlightedTabColor(&getThemeColor(
                ThemeColorId::GUILD_SOCIAL_TAB_HIGHLIGHTED, 255U),
                &getThemeColor(
                ThemeColorId::GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE, 255U));
            setSelectedTabColor(&getThemeColor(
                ThemeColorId::GUILD_SOCIAL_TAB_SELECTED, 255U),
                &getThemeColor(
                ThemeColorId::GUILD_SOCIAL_TAB_SELECTED_OUTLINE, 255U));

            createControls(guild, showBackground);
            mMenuAction = "guild";
        }

        A_DELETE_COPY(SocialGuildTab)

        ~SocialGuildTab() override final
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

                if (localChatTab != nullptr)
                {
                    localChatTab->chatLog(strprintf(
                        // TRANSLATORS: chat message
                        _("Invited user %s to guild %s."),
                        name.c_str(),
                        mGuild->getName().c_str()),
                        ChatMsgType::BY_SERVER,
                        IgnoreRecord_false,
                        TryRemoveColors_true);
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
                if (localChatTab != nullptr)
                {
                    localChatTab->chatLog(strprintf(
                        // TRANSLATORS: chat message
                        _("Guild %s quit requested."),
                        mGuild->getName().c_str()),
                        ChatMsgType::BY_SERVER,
                        IgnoreRecord_false,
                        TryRemoveColors_true);
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
            CREATEWIDGETV(mInviteDialog, TextDialog,
                // TRANSLATORS: guild invite message
                _("Member Invite to Guild"),
                // TRANSLATORS: guild invite message
                strprintf(_("Who would you like to invite to guild %s?"),
                mGuild->getName().c_str()),
                socialWindow,
                false);
            mInviteDialog->setActionEventId("do invite");
            mInviteDialog->addActionListener(this);
        }

        void leave() override final
        {
            CREATEWIDGETV(mConfirmDialog, ConfirmDialog,
                // TRANSLATORS: guild leave message
                _("Leave Guild?"),
                // TRANSLATORS: guild leave message
                strprintf(_("Are you sure you want to leave guild %s?"),
                mGuild->getName().c_str()),
                SOUND_REQUEST,
                false,
                Modal_false,
                socialWindow);
            mConfirmDialog->addActionListener(this);
        }

        void buildCounter(const int online0, const int total0) override final
        {
            if ((online0 != 0) || (total0 != 0))
            {
                // TRANSLATORS: social window label
                mCounterString = strprintf(_("Members: %u/%u"),
                    CAST_U32(online0),
                    CAST_U32(total0));
            }
            else
            {
                if (localPlayer == nullptr)
                    return;

                const Guild *const guild = localPlayer->getGuild();
                if (guild == nullptr)
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
                    CAST_U32(online),
                    CAST_U32(total));
            }
            updateCounter();
        }

    private:
        Guild *mGuild;
};

#endif  // GUI_WIDGETS_TABS_SOCIALGUILDTAB_H
