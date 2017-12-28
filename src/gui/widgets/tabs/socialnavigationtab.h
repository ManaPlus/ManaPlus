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

#ifndef GUI_WIDGETS_TABS_SOCIALNAVIGATIONTAB_H
#define GUI_WIDGETS_TABS_SOCIALNAVIGATIONTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "actormanager.h"
#include "configuration.h"

#include "gui/models/beingslistmodel.h"

#include "gui/windows/outfitwindow.h"

#include "being/localplayer.h"

#include "input/keyboardconfig.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"

#include "localconsts.h"

class Avatar;

class SocialNavigationTab final : public SocialTab
{
    public:
        SocialNavigationTab(const Widget2 *const widget,
                            const Opaque showBackground) :
            SocialTab(widget),
            mBeings(new BeingsListModel)
        {
            createControls(mBeings, showBackground);

            // TRANSLATORS: Navigation tab name in social window.
            // TRANSLATORS: Should be small
            setCaption(_("Nav"));
            mMenuAction = "navigation";
        }

        A_DELETE_COPY(SocialNavigationTab)

        ~SocialNavigationTab() override final
        {
            delete2(mList)
            delete2(mScroll)
            delete2(mBeings)
        }

        void updateList() override final
        {
            if ((socialWindow == nullptr) || (localPlayer == nullptr))
                return;

            const Map *const map = socialWindow->getMap();
            if ((map == nullptr) || map->empty())
                return;

            if (socialWindow->getProcessedPortals())
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();
            STD_VECTOR<MapItem*> portals = map->getPortals();

            STD_VECTOR<MapItem*>::const_iterator i = portals.begin();
            const SpecialLayer *const specialLayer = map->getSpecialLayer();

            STD_VECTOR<Avatar*>::iterator ia = avatars->begin();

            while (ia != avatars->end())
            {
                delete *ia;
                ++ ia;
            }

            avatars->clear();

            int online = 0;
            int total = 0;

            int idx = 0;
            while (i != portals.end())
            {
                MapItem *portal = *i;
                if (portal == nullptr)
                    continue;

                const int x = portal->getX();
                const int y = portal->getY();

                const std::string name = strprintf("%s [%d %d]",
                    portal->getComment().c_str(), x, y);

                Avatar *const ava = new Avatar(name);
                if (localPlayer != nullptr)
                    ava->setOnline(localPlayer->isReachable(x, y, true));
                else
                    ava->setOnline(false);
                ava->setLevel(-1);
                ava->setType(portal->getType());
                ava->setX(x);
                ava->setY(y);
                avatars->push_back(ava);

                if (ava->getOnline())
                    online ++;
                total ++;

                if (config.getBoolValue("drawHotKeys")
                    && idx < 80 && (outfitWindow != nullptr))
                {
                    Being *const being = actorManager
                        ->findPortalByTile(x, y);
                    if (being != nullptr)
                    {
                        being->setName(KeyboardConfig::getKeyShortString(
                            OutfitWindow::keyName(idx)));
                    }

                    if (specialLayer != nullptr)
                    {
                        portal = specialLayer->getTile(
                            ava->getX(), ava->getY());
                        if (portal != nullptr)
                        {
                            portal->setName(KeyboardConfig::getKeyShortString(
                                OutfitWindow::keyName(idx)));
                        }
                    }
                }

                ++i;
                idx ++;
            }
            if (socialWindow != nullptr)
                socialWindow->setProcessedPortals(true);

            // TRANSLATORS: social window label
            mCounterString = strprintf(_("Portals: %u/%u"),
                CAST_U32(online),
                CAST_U32(total));
            updateCounter();
        }

        void selectIndex(const unsigned num) override final
        {
            if (localPlayer == nullptr)
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();
            if (avatars->size() <= CAST_SIZE(num))
                return;

            const Avatar *const ava = avatars->at(num);
            if ((ava != nullptr) && (localPlayer != nullptr))
                localPlayer->navigateTo(ava->getX(), ava->getY());
        }

        void updateNames()
        {
            if (socialWindow == nullptr)
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();

            const Map *const map = socialWindow->getMap();
            if (map == nullptr)
                return;

            STD_VECTOR<Avatar*>::const_iterator i = avatars->begin();
            const STD_VECTOR<Avatar*>::const_iterator i_end = avatars->end();
            while (i != i_end)
            {
                Avatar *const ava = *i;
                if (ava == nullptr)
                    break;

                const  MapItem *const item = map->findPortalXY(
                    ava->getX(), ava->getY());
                if (item != nullptr)
                {
                    const std::string name = strprintf("%s [%d %d]",
                        item->getComment().c_str(),
                        item->getX(), item->getY());
                    ava->setName(name);
                    ava->setOriginalName(name);
                }

                ++i;
            }
        }

        int getPortalIndex(const int x, const int y)
        {
            if (socialWindow == nullptr)
                return -1;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();
            const Map *const map = socialWindow->getMap();
            if (map == nullptr)
                return -1;

            STD_VECTOR<Avatar*>::const_iterator i = avatars->begin();
            const STD_VECTOR<Avatar*>::const_iterator i_end = avatars->end();
            unsigned num = 0;
            while (i != i_end)
            {
                const Avatar *const ava = *i;
                if (ava == nullptr)
                    break;

                if (ava->getX() == x && ava->getY() == y)
                    return num;

                ++i;
                num ++;
            }
            return -1;
        }

        void addPortal(const int x, const int y)
        {
            if ((socialWindow == nullptr) || (localPlayer == nullptr))
                return;

            const Map *const map = socialWindow->getMap();
            if (map == nullptr)
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();
            const MapItem *const portal = map->findPortalXY(x, y);
            if (portal == nullptr)
                return;

            const std::string name = strprintf("%s [%d %d]",
                portal->getComment().c_str(), x, y);

            Avatar *const ava = new Avatar(name);
            if (localPlayer != nullptr)
                ava->setOnline(localPlayer->isReachable(x, y, true));
            else
                ava->setOnline(false);
            ava->setLevel(-1);
            ava->setType(portal->getType());
            ava->setX(x);
            ava->setY(y);
            avatars->push_back(ava);
        }

        void removePortal(const int x, const int y)
        {
            if ((socialWindow == nullptr) || (localPlayer == nullptr))
                return;

            const Map *const map = socialWindow->getMap();
            if (map == nullptr)
                return;

            STD_VECTOR<Avatar*> *const avatars = mBeings->getMembers();
            STD_VECTOR<Avatar*>::iterator i = avatars->begin();
            const STD_VECTOR<Avatar*>::iterator i_end = avatars->end();

            while (i != i_end)
            {
                Avatar *ava = (*i);

                if (ava == nullptr)
                    break;

                if (ava->getX() == x && ava->getY() == y)
                {
                    delete ava;
                    avatars->erase(i);
                    return;
                }

                ++ i;
            }
        }

    private:
        BeingsListModel *mBeings;
};

#endif  // GUI_WIDGETS_TABS_SOCIALNAVIGATIONTAB_H
