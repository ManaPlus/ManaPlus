/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/itempopup.h"

#include "client.h"
#include "item.h"
#include "units.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include <guichan/font.hpp>

#include "debug.h"

ItemPopup::ItemPopup() :
    Popup("ItemPopup", "itempopup.xml"),
    mItemName(new Label(this)),
    mItemDesc(new TextBox(this)),
    mItemEffect(new TextBox(this)),
    mItemWeight(new TextBox(this)),
    mItemType(ITEM_UNUSABLE),
    mIcon(new Icon(this, nullptr)),
    mLastName(),
    mLastColor(1)
{
    // Item Name
    mItemName->setFont(boldFont);
    mItemName->setPosition(0, 0);

    const int fontHeight = getFont()->getHeight();

    // Item Description
    mItemDesc->setEditable(false);
    mItemDesc->setPosition(0, fontHeight);
    mItemDesc->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));

    // Item Effect
    mItemEffect->setEditable(false);
    mItemEffect->setPosition(0, 2 * fontHeight);
    mItemEffect->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));

    // Item Weight
    mItemWeight->setEditable(false);
    mItemWeight->setPosition(0, 3 * fontHeight);
    mItemWeight->setForegroundColorAll(getThemeColor(Theme::POPUP),
        getThemeColor(Theme::POPUP_OUTLINE));

    add(mItemName);
    add(mItemDesc);
    add(mItemEffect);
    add(mItemWeight);
    add(mIcon);

    addMouseListener(this);
}

ItemPopup::~ItemPopup()
{
    if (mIcon)
    {
        Image *const image = mIcon->getImage();
        if (image)
            image->decRef();
    }
}

void ItemPopup::setItem(const Item *const item, const bool showImage)
{
    if (!item)
        return;

    const ItemInfo &ii = item->getInfo();
    setItem(ii, item->getColor(), showImage, item->getId());
    if (item->getRefine() > 0)
    {
        mLastName = ii.getName();
        mLastColor = item->getColor();
        if (serverVersion > 0)
        {
            mItemName->setCaption(strprintf("%s (+%d), %d", ii.getName(
                item->getColor()).c_str(), item->getRefine(), ii.getId()));
        }
        else
        {
            mItemName->setCaption(strprintf("%s (+%d), %d",
                ii.getName().c_str(), item->getRefine(), ii.getId()));
        }
        mItemName->adjustSize();
        const unsigned minWidth = mItemName->getWidth() + 8;
        if (static_cast<unsigned>(getWidth()) < minWidth)
            setWidth(minWidth);
    }
}

void ItemPopup::setItem(const ItemInfo &item, const unsigned char color,
                        const bool showImage, int id)
{
    if (!mIcon || (item.getName() == mLastName && color == mLastColor))
        return;

    if (id == -1)
        id = item.getId();

    int space = 0;

    Image *const oldImage = mIcon->getImage();
    if (oldImage)
        oldImage->decRef();

    if (showImage)
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        Image *const image = resman->getImage(combineDye2(
            paths.getStringValue("itemIcons").append(
            item.getDisplay().image), item.getDyeColorsString(color)));

        mIcon->setImage(image);
        if (image)
        {
            mIcon->setPosition(0, 0);
            space = mIcon->getWidth();
        }
    }
    else
    {
        mIcon->setImage(nullptr);
    }

    mItemType = item.getType();

    mLastName = item.getName();
    mLastColor = color;

    if (serverVersion > 0)
    {
        mItemName->setCaption(strprintf("%s, %d",
            item.getName(color).c_str(), id));
        mItemDesc->setTextWrapped(item.getDescription(color), 196);
    }
    else
    {
        mItemName->setCaption(strprintf("%s, %d",
            item.getName().c_str(), id));
        mItemDesc->setTextWrapped(item.getDescription(), 196);
    }

    mItemName->adjustSize();
    setLabelColor(mItemName, mItemType);
    mItemName->setPosition(space, 0);

    mItemEffect->setTextWrapped(item.getEffect(), 196);
    // TRANSLATORS: popup label
    mItemWeight->setTextWrapped(strprintf(_("Weight: %s"),
        Units::formatWeight(item.getWeight()).c_str()), 196);

    int minWidth = mItemName->getWidth() + space;

    if (mItemName->getWidth() + space > minWidth)
        minWidth = mItemName->getWidth() + space;
    if (mItemDesc->getMinWidth() > minWidth)
        minWidth = mItemDesc->getMinWidth();
    if (mItemEffect->getMinWidth() > minWidth)
        minWidth = mItemEffect->getMinWidth();
    if (mItemWeight->getMinWidth() > minWidth)
        minWidth = mItemWeight->getMinWidth();

    const int numRowsDesc = mItemDesc->getNumberOfRows();
    const int numRowsEffect = mItemEffect->getNumberOfRows();
    const int numRowsWeight = mItemWeight->getNumberOfRows();
    const int height = getFont()->getHeight();

    if (item.getEffect().empty())
    {
        setContentSize(minWidth, (numRowsDesc + 2 + numRowsWeight) * height);
        mItemWeight->setPosition(0, (numRowsDesc + 2) * height);
    }
    else
    {
        setContentSize(minWidth, (numRowsDesc + numRowsEffect + 2
            + numRowsWeight) * height);
        mItemWeight->setPosition(0, (numRowsDesc + numRowsEffect + 2)
            * height);
        mItemEffect->setPosition(0, (numRowsDesc + 2) * height);
    }

    mItemDesc->setPosition(0, 2 * height);
}

#define caseSetColor(name1, name2) \
    case name1: \
    { \
        return label->setForegroundColorAll(getThemeColor(name2), \
        getThemeColor(name2##_OUTLINE)); \
    }
void ItemPopup::setLabelColor(Label *label, const ItemType type) const
{
    switch (type)
    {
        caseSetColor(ITEM_UNUSABLE, Theme::GENERIC)
        caseSetColor(ITEM_USABLE, Theme::USABLE)
        caseSetColor(ITEM_EQUIPMENT_ONE_HAND_WEAPON, Theme::ONEHAND)
        caseSetColor(ITEM_EQUIPMENT_TWO_HANDS_WEAPON, Theme::TWOHAND)
        caseSetColor(ITEM_EQUIPMENT_TORSO, Theme::TORSO)
        caseSetColor(ITEM_EQUIPMENT_ARMS, Theme::ARMS)
        caseSetColor(ITEM_EQUIPMENT_HEAD, Theme::HEAD)
        caseSetColor(ITEM_EQUIPMENT_LEGS, Theme::LEGS)
        caseSetColor(ITEM_EQUIPMENT_SHIELD, Theme::SHIELD)
        caseSetColor(ITEM_EQUIPMENT_RING, Theme::RING)
        caseSetColor(ITEM_EQUIPMENT_NECKLACE, Theme::NECKLACE)
        caseSetColor(ITEM_EQUIPMENT_FEET, Theme::FEET)
        caseSetColor(ITEM_EQUIPMENT_AMMO, Theme::AMMO)
        caseSetColor(ITEM_EQUIPMENT_CHARM, Theme::CHARM)
        caseSetColor(ITEM_SPRITE_RACE, Theme::UNKNOWN_ITEM)
        caseSetColor(ITEM_SPRITE_HAIR, Theme::UNKNOWN_ITEM)
        default:
        {
            return label->setForegroundColorAll(getThemeColor(
                Theme::UNKNOWN_ITEM), getThemeColor(
                Theme::UNKNOWN_ITEM_OUTLINE));
        }
    }
}
#undef caseSetColor

void ItemPopup::mouseMoved(gcn::MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(false);
    mLastName.clear();
    mLastColor = 1;
}
