/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/item/item.h"

#include "configuration.h"
#include "dragdrop.h"
#include "itemcolormanager.h"

#include "gui/theme.h"

#include "resources/iteminfo.h"

#include "resources/item/itemoptionslist.h"

#include "resources/loaders/imageloader.h"

#include "net/net.h"

#include "utils/delete2.h"

#include "debug.h"

DragDrop dragDrop(nullptr, DragDropSource::Empty);

Item::Item(const int id,
           const ItemTypeT type,
           const int quantity,
           const uint8_t refine,
           const ItemColor color,
           const Identified identified,
           const Damaged damaged,
           const Favorite favorite,
           const Equipm equipment,
           const Equipped equipped) :
    mId(0),
    mColor(ItemColor_zero),
    mQuantity(quantity),
    mTag(0),
    mImage(nullptr),
    mDescription(),
    mTags(),
    mCards(),
    mOptions(nullptr),
    mRefine(refine),
    mInvIndex(0),
    mType(type),
    mEquipment(equipment),
    mEquipped(equipped),
    mInEquipment(false),
    mIdentified(identified),
    mDamaged(damaged),
    mFavorite(favorite)
{
    setId(id, color);
    for (int f = 0; f < maxCards; f ++)
        mCards[f] = 0;
}

Item::~Item()
{
    if (mImage != nullptr)
    {
        mImage->decRef();
        mImage = nullptr;
    }
    delete2(mOptions);
    dragDrop.clearItem(this);
}

void Item::setId(const int id,
                 const ItemColor color)
{
    mId = id;
    mColor = color;

    // Types 0 and 1 are not equippable items.
    mEquipment = fromBool(id && CAST_S32(getInfo().getType())
        >= 2, Equipm);

    if (mImage != nullptr)
        mImage->decRef();

    const ItemInfo &info = getInfo();
    mTags = info.getTags();

    const std::string dye = combineDye2(pathJoin(
        paths.getStringValue("itemIcons"), info.getDisplay().image),
        info.getDyeIconColorsString(color));
    mImage = Loader::getImage(dye);

    if (mImage == nullptr)
    {
        mImage = Theme::getImageFromTheme(paths.getValue("unknownItemFile",
            "unknown-item.png"));
    }
}

bool Item::isHaveTag(const int tagId) const
{
    const std::map <int, int>::const_iterator it = mTags.find(tagId);
    if (it == mTags.end())
        return false;
    return (*it).second > 0;
}

Image *Item::getImage(const int id,
                      const ItemColor color)
{
    const ItemInfo &info = ItemDB::get(id);
    Image *image = Loader::getImage(combineDye2(pathJoin(paths.getStringValue(
        "itemIcons"), info.getDisplay().image),
        info.getDyeIconColorsString(color)));

    if (image == nullptr)
        image = Theme::getImageFromTheme("unknown-item.png");
    return image;
}

std::string Item::getName() const
{
    const ItemInfo &info = ItemDB::get(mId);
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() == ServerType::TMWATHENA)
    {
        return info.getName();
    }
#endif  // TWMA_SUPPORT
    return info.getName(mColor);
}

void Item::setCard(const int index, const int id)
{
    if (index < 0 || index >= maxCards)
        return;
    mCards[index] = id;
}

int Item::getCard(const int index) const
{
    if (index < 0 || index >= maxCards)
        return 0;
    return mCards[index];
}

void Item::setCards(const int *const cards, const int size)
{
    if (size < 0 || (cards == nullptr))
        return;
    int sz = size;
    if (sz > maxCards)
        sz = maxCards;
    for (int f = 0; f < sz; f ++)
        mCards[f] = cards[f];
}

void Item::addCard(const int card)
{
    for (int f = 0; f < maxCards; f ++)
    {
        if (mCards[f] == 0)
        {
            mCards[f] = card;
            return;
        }
    }
}

void Item::setOptions(const ItemOptionsList *const options)
{
    delete2(mOptions);
    mOptions = ItemOptionsList::copy(options);
}

void Item::updateColor()
{
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        setId(mId, ItemColorManager::getColorFromCards(&mCards[0]));
}
