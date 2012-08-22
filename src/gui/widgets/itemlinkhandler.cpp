/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/widgets/itemlinkhandler.h"

#include "item.h"

#include "gui/itempopup.h"
#include "gui/viewport.h"

#include "utils/process.h"

#include <sstream>
#include <string>

#include "debug.h"

ItemLinkHandler::ItemLinkHandler() :
    mItemPopup(new ItemPopup)
{
}

ItemLinkHandler::~ItemLinkHandler()
{
    delete mItemPopup;
    mItemPopup = nullptr;
}

void ItemLinkHandler::handleLink(const std::string &link,
                                 gcn::MouseEvent *event A_UNUSED)
{
    if (!strStartWith(link, "http://"))
    {
        if (!mItemPopup)
            return;

        int id = 0;
        std::stringstream stream;
        stream << link;
        stream >> id;

        if (id > 0)
        {
            const ItemInfo &itemInfo = ItemDB::get(id);
            //+++ need add color to links?
            mItemPopup->setItem(itemInfo, 1, true);

            if (mItemPopup->isVisible())
            {
                mItemPopup->setVisible(false);
            }
            else if (viewport)
            {
                mItemPopup->position(viewport->getMouseX(),
                    viewport->getMouseY());
            }
        }
    }
    else
    {
        openBrowser(link);
    }
}
