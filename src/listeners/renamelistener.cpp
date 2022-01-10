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

#include "listeners/renamelistener.h"

#include "gui/viewport.h"

#include "gui/windows/socialwindow.h"
#include "gui/windows/textdialog.h"

#include "resources/map/map.h"
#include "resources/map/mapitem.h"
#include "resources/map/speciallayer.h"

#include "debug.h"

RenameListener::RenameListener() :
    ActionListener(),
    mMapItemX(0),
    mMapItemY(0),
    mDialog(nullptr)
{
}

void RenameListener::setMapItem(const MapItem *const mapItem)
{
    if (mapItem != nullptr)
    {
        mMapItemX = mapItem->getX();
        mMapItemY = mapItem->getY();
    }
    else
    {
        mMapItemX = 0;
        mMapItemY = 0;
    }
}

void RenameListener::action(const ActionEvent &event)
{
    if (event.getId() == "ok" && (viewport != nullptr) && (mDialog != nullptr))
    {
        const Map *const map = viewport->getMap();
        if (map == nullptr)
            return;

        const SpecialLayer *const sl = map->getSpecialLayer();
        MapItem *item = nullptr;
        if (sl != nullptr)
        {
            item = sl->getTile(mMapItemX, mMapItemY);
            if (item != nullptr)
                item->setComment(mDialog->getText());
        }
        item = map->findPortalXY(mMapItemX, mMapItemY);
        if (item != nullptr)
            item->setComment(mDialog->getText());

        if (socialWindow != nullptr)
            socialWindow->updatePortalNames();
    }
    mDialog = nullptr;
}
