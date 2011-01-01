/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#include "item.h"

#include "gui/theme.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"
#include "configuration.h"

Item::Item(int id, int quantity, int refine, bool equipment, bool equipped):
    mImage(0),
    mDrawImage(0),
    mQuantity(quantity),
    mEquipment(equipment),
    mEquipped(equipped),
    mInEquipment(false),
    mRefine(refine),
    mInvIndex(0)
{
    setId(id);
}

Item::~Item()
{
    if (mImage)
        mImage->decRef();
}

void Item::setId(int id)
{
    mId = id;

    // Types 0 and 1 are not equippable items.
    mEquipment = id && getInfo().getType() >= 2;

    // Load the associated image
    if (mImage)
        mImage->decRef();

    if (mDrawImage)
        mDrawImage->decRef();

    ResourceManager *resman = ResourceManager::getInstance();
    SpriteDisplay display = getInfo().getDisplay();
    std::string imagePath = paths.getStringValue("itemIcons")
                            + display.image;
    mImage = resman->getImage(imagePath);
    mDrawImage = resman->getImage(imagePath);

    if (!mImage)
    {
        mImage = Theme::getImageFromTheme(paths.getValue("unknownItemFile",
                                          "unknown-item.png"));
    }

    if (!mDrawImage)
    {
        mDrawImage = Theme::getImageFromTheme(
            paths.getValue("unknownItemFile", "unknown-item.png"));
    }
}

Image *Item::getImage(int id)
{
    ResourceManager *resman = ResourceManager::getInstance();
    SpriteDisplay display = ItemDB::get(id).getDisplay();
    std::string imagePath = "graphics/items/" + display.image;
    Image *image = resman->getImage(imagePath);

    if (!image)
        image = Theme::getImageFromTheme("unknown-item.png");
    return image;
}
