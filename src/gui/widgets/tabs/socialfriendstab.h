/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SOCIALFRIENDSTAB_H
#define GUI_WIDGETS_TABS_SOCIALFRIENDSTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "actormanager.h"

#include "being/playerrelations.h"

#include "gui/models/beingslistmodel.h"

#include "gui/windows/whoisonline.h"

#include "gui/widgets/tabs/socialfriendsfunctor.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "localconsts.h"

class SocialFriendsTab final : public SocialTab
{
    public:
        SocialFriendsTab(const Widget2 *const widget,
                         const std::string &name,
                         const Opaque showBackground) :
            SocialTab(widget),
            mBeings(new BeingsListModel),
            mFriendSorter()
        {
            createControls(mBeings, showBackground);

            getPlayersAvatars();
            setCaption(name);
            mMenuAction = "friends";
        }

        A_DELETE_COPY(SocialFriendsTab)

        ~SocialFriendsTab() override final
        {
            delete2(mList)
            delete2(mScroll)
            delete2(mBeings)
        }

        void updateList() override final
        {
            getPlayersAvatars();
        }

        void getPlayersAvatars()
        {
            if (actorManager == nullptr)
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();

            STD_VECTOR<Avatar*>::iterator ia = avatars->begin();
            while (ia != avatars->end())
            {
                delete *ia;
                ++ ia;
            }
            avatars->clear();

            const StringVect *const players
                = playerRelations.getPlayersByRelation(Relation::FRIEND);

            const std::set<std::string> &players2
                = whoIsOnline->getOnlineNicks();

            if (players == nullptr)
                return;

            int online = 0;
            int total = 0;

            FOR_EACHP (StringVectCIter, it, players)
            {
                Avatar *const ava = new Avatar(*it);
                if (actorManager->findBeingByName(*it, ActorType::Player) !=
                    nullptr || players2.find(*it) != players2.end())
                {
                    ava->setOnline(true);
                    online ++;
                }
                total ++;
                avatars->push_back(ava);
            }
            std::sort(avatars->begin(), avatars->end(), mFriendSorter);
            delete players;

            // TRANSLATORS: social window label
            mCounterString = strprintf(_("Friends: %u/%u"),
                CAST_U32(online),
                CAST_U32(total));
            updateCounter();
        }

    private:
        BeingsListModel *mBeings;
        SortFriendsFunctor mFriendSorter;
};

#endif  // GUI_WIDGETS_TABS_SOCIALFRIENDSTAB_H
