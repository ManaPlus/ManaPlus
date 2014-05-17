/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_TABS_SOCIALATTACKTAB_H
#define GUI_WIDGETS_TABS_SOCIALATTACKTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "localconsts.h"

#define addAvatars(mob, str, type) \
{\
    ava = new Avatar(str);\
    ava->setOnline(false);\
    ava->setLevel(-1);\
    ava->setType(MapItem::SEPARATOR);\
    ava->setX(0);\
    ava->setY(0);\
    avatars->push_back(ava);\
    mobs = actorManager->get##mob##s();\
    i = mobs.begin();\
    i_end = mobs.end();\
    while (i != i_end)\
    {\
        std::string name;\
        int level = -1;\
        if (*i == "")\
        {\
            name = _("(default)");\
            level = 0;\
        }\
        else\
        {\
            name = *i;\
        }\
        ava = new Avatar(name);\
        ava->setOnline(true);\
        ava->setLevel(level);\
        ava->setType(MapItem::type);\
        ava->setX(0);\
        ava->setY(0);\
        avatars->push_back(ava);\
        ++ i;\
    }\
}

#define updateAtkListStart() \
    if (!socialWindow || !player_node || !actorManager)\
        return;\
    std::vector<Avatar*> *const avatars = mBeings->getMembers();\
    std::vector<Avatar*>::iterator ia = avatars->begin();\
    while (ia != avatars->end())\
    {\
        delete *ia;\
        ++ ia;\
    }\
    avatars->clear();\
    Avatar *ava = nullptr;\
    std::list<std::string> mobs;\
    std::list<std::string>::const_iterator i;\
    std::list<std::string>::const_iterator i_end;

class SocialAttackTab final : public SocialTab
{
    public:
        SocialAttackTab(const Widget2 *const widget,
                        const bool showBackground) :
            SocialTab(widget),
            mBeings(new BeingsListModel)
        {
            mList = new AvatarListBox(this, mBeings);
            mList->postInit();
            mScroll = new ScrollArea(this, mList, showBackground,
                "social_background.xml");

            mScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_AUTO);
            mScroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);

            // TRANSLATORS: Attack filter tab name in social window.
            // TRANSLATORS: Should be small
            setCaption(_("Atk"));
        }

        A_DELETE_COPY(SocialAttackTab)

        ~SocialAttackTab()
        {
            delete2(mList)
            delete2(mScroll)
            delete2(mBeings)
        }

        void updateList() override final
        {
            updateAtkListStart();
            // TRANSLATORS: mobs group name in social window
            addAvatars(PriorityAttackMob, _("Priority mobs"), PRIORITY);
            // TRANSLATORS: mobs group name in social window
            addAvatars(AttackMob, _("Attack mobs"), ATTACK);
            // TRANSLATORS: mobs group name in social window
            addAvatars(IgnoreAttackMob, _("Ignore mobs"), IGNORE_);
        }

    private:
        BeingsListModel *mBeings;
};

#endif  // GUI_WIDGETS_TABS_SOCIALATTACKTAB_H
