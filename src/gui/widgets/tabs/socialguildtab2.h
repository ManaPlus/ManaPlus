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

#ifndef GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H
#define GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H

#include "gui/widgets/tabs/socialtab.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "localconsts.h"

class SocialGuildTab2 final : public SocialTab,
                              public ActionListener
{
    public:
        SocialGuildTab2(const Widget2 *const widget,
                        Guild *const guild,
                        const bool showBackground) :
            SocialTab(widget),
            ActionListener()
        {
            // TRANSLATORS: tab in social window
            setCaption(_("Guild"));

            setTabColor(&getThemeColor(Theme::GUILD_SOCIAL_TAB),
                &getThemeColor(Theme::GUILD_SOCIAL_TAB_OUTLINE));
            setHighlightedTabColor(&getThemeColor(
                Theme::GUILD_SOCIAL_TAB_HIGHLIGHTED), &getThemeColor(
                Theme::GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE));
            setSelectedTabColor(&getThemeColor(Theme::GUILD_SOCIAL_TAB_SELECTED),
                &getThemeColor(Theme::GUILD_SOCIAL_TAB_SELECTED_OUTLINE));

            createControls(guild, showBackground);
        }

        A_DELETE_COPY(SocialGuildTab2)

        ~SocialGuildTab2()
        {
            delete2(mList)
            delete2(mScroll)
        }

        void action(const ActionEvent &event A_UNUSED) override final
        {
        }

        void buildCounter(const int online0 A_UNUSED, const int total0 A_UNUSED)
        {
            if (!player_node)
                return;

            const Guild *const guild = player_node->getGuild();
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
            updateCounter();
        }
};

#endif  // GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H
