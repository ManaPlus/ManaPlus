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

#ifndef GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H
#define GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H

#include "gui/widgets/tabs/socialtab.h"

#include "guild.h"

#include "being/localplayer.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "localconsts.h"

class SocialGuildTab2 final : public SocialTab,
                              public ActionListener
{
    public:
        SocialGuildTab2(const Widget2 *const widget,
                        Guild *const guild,
                        const Opaque showBackground) :
            SocialTab(widget),
            ActionListener()
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

        A_DELETE_COPY(SocialGuildTab2)

        ~SocialGuildTab2() override final
        {
            delete2(mList)
            delete2(mScroll)
        }

        void action(const ActionEvent &event A_UNUSED) override final
        {
        }

        void buildCounter(const int online0 A_UNUSED,
                          const int total0 A_UNUSED) override final
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
            updateCounter();
        }
};

#endif  // GUI_WIDGETS_TABS_SOCIALGUILDTAB2_H
