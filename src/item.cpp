/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "item.h"

#include "gui/theme.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"
#include "configuration.h"

#include "debug.h"

Item::Item(int id, int quantity, int refine, unsigned char color,
           bool equipment, bool equipped):
    mImage(0),
    mDrawImage(0),
    mQuantity(quantity),
    mEquipment(equipment),
    mEquipped(equipped),
    mInEquipment(false),
    mRefine(refine),
    mInvIndex(0)
{
    setId(id, color);
}

Item::~Item()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = 0;
    }
}

void Item::setId(int id, unsigned char color)
{
    mId = id;
    mColor = color;

    // Types 0 and 1 are not equippable items.
    mEquipment = id && getInfo().getType() >= 2;

    // Load the associated image
    if (mImage)
        mImage->decRef();

    if (mDrawImage)
        mDrawImage->decRef();

    ResourceManager *resman = ResourceManager::getInstance();
    const ItemInfo &info = getInfo();
    mTags = info.getTags();
//    logger->log("tag0=" + toString(mTags[1]));

//    for (int f = 0; f < mTags->size(); f ++)
//        logger->log("tag: %d", (*mTags)[f]);

    SpriteDisplay display = info.getDisplay();
    std::string imagePath = paths.getStringValue("itemIcons")
                            + display.image;
    std::string dye = combineDye2(imagePath, info.getDyeColorsString(color));
    mImage = resman->getImage(dye);
    mDrawImage = resman->getImage(dye);

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

bool Item::isHaveTag(int tagId)
{
    if (mTags.find(tagId) == mTags.end())
        return false;
    return mTags[tagId] > 0;
}

Image *Item::getImage(int id, unsigned char color)
{
    ResourceManager *resman = ResourceManager::getInstance();
    const ItemInfo &info = ItemDB::get(id);
    SpriteDisplay display = info.getDisplay();
    std::string imagePath = "graphics/items/" + display.image;
    Image *image;
    image = resman->getImage(combineDye2(imagePath,
        info.getDyeColorsString(color)));

    if (!image)
        image = Theme::getImageFromTheme("unknown-item.png");
    return image;
}
