/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_TABS_SOCIALPLAYERSTAB_H
#define GUI_WIDGETS_TABS_SOCIALPLAYERSTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "actormanager.h"
#include "party.h"

#include "being/being.h"

#include "gui/models/beingslistmodel.h"

#include "utils/gettext.h"
#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "localconsts.h"

class Avatar;
class Being;

class SocialPlayersTab final : public SocialTab
{
    public:
        SocialPlayersTab(const Widget2 *const widget,
                         std::string name,
                         const Opaque showBackground) :
            SocialTab(widget),
            mBeings(new BeingsListModel)
        {
            createControls(mBeings, showBackground);

            getPlayersAvatars();
            setCaption(name);
            mMenuAction = "players";
        }

        A_DELETE_COPY(SocialPlayersTab)

        ~SocialPlayersTab()
        {
            delete2(mList)
            delete2(mScroll)
            delete2(mBeings)
        }

        void updateList() override final
        {
            getPlayersAvatars();
        }

        void updateAvatar(const std::string &name) override final
        {
            if (!actorManager)
                return;

            BLOCK_START("SocialPlayersTab::updateAvatar")
            Avatar *const avatar = findAvatarbyName(name);
            if (!avatar)
                return;
            if (Party::getParty(1))
            {
                const PartyMember *const
                    pm = Party::getParty(1)->getMember(name);
                if (pm && pm->getMaxHp() > 0)
                {
                    avatar->setMaxHp(pm->getMaxHp());
                    avatar->setHp(pm->getHp());
                }
            }
            const Being *const being = actorManager->findBeingByName(
                name, ActorType::Player);
            if (being)
            {
                avatar->setDamageHp(being->getDamageTaken());
                avatar->setLevel(being->getLevel());
                avatar->setGender(being->getGender());
                avatar->setIp(being->getIp());
                avatar->setPoison(being->getPoison());
            }
            BLOCK_END("SocialPlayersTab::updateAvatar")
        }

        void resetDamage(const std::string &name) override final
        {
            if (!actorManager)
                return;

            Avatar *const avatar = findAvatarbyName(name);
            if (!avatar)
                return;
            avatar->setDamageHp(0);
            Being *const being = actorManager->findBeingByName(
                name, ActorType::Player);

            if (being)
                being->setDamageTaken(0);
        }

        Avatar* findAvatarbyName(const std::string &name)
        {
            std::vector<Avatar*> *const avatars = mBeings->getMembers();
            Avatar *ava = nullptr;
            std::vector<Avatar*>::const_iterator i = avatars->begin();
            const std::vector<Avatar*>::const_iterator i_end = avatars->end();
            while (i != i_end)
            {
                ava = (*i);
                if (ava && ava->getName() == name)
                    return ava;
                ++i;
            }
            ava = new Avatar(name);
            ava->setOnline(true);
            avatars->push_back(ava);
            return ava;
        }

        void getPlayersAvatars()
        {
            std::vector<Avatar*> *const avatars = mBeings->getMembers();
            if (actorManager)
            {
                StringVect names;
                actorManager->getPlayerNames(names, NpcNames_false);

                std::vector<Avatar*>::iterator ai = avatars->begin();
                while (ai != avatars->end())
                {
                    bool finded = false;
                    const Avatar *const ava = (*ai);
                    if (!ava)
                        break;

                    StringVectCIter i = names.begin();
                    const StringVectCIter i_end = names.end();
                    while (i != i_end)
                    {
                        if (ava->getName() == (*i) && (*i) != "")
                        {
                            finded = true;
                            break;
                        }
                        ++i;
                    }

                    if (!finded)
                    {
                        delete *ai;
                        ai = avatars->erase(ai);
                    }
                    else
                    {
                        ++ai;
                    }
                }

                StringVectCIter i = names.begin();
                const StringVectCIter i_end = names.end();

                while (i != i_end)
                {
                    if ((*i) != "")
                        updateAvatar(*i);
                    ++i;
                }
            }
            // TRANSLATORS: social window label
            mCounterString = strprintf(_("Visible players: %d"),
                CAST_S32(avatars->size()));
            updateCounter();
        }

    private:
        BeingsListModel *mBeings;
};

#endif  // GUI_WIDGETS_TABS_SOCIALPLAYERSTAB_H
