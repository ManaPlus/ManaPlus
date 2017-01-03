/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/popups/itempopup.h"

#include "actormanager.h"
#include "configuration.h"
#include "units.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include "resources/iteminfo.h"

#include "resources/image/image.h"

#include "resources/item/item.h"

#include "resources/loaders/imageloader.h"

#include "net/beinghandler.h"
#include "net/serverfeatures.h"

#include "debug.h"

ItemPopup *itemPopup = nullptr;

ItemPopup::ItemPopup() :
    Popup("ItemPopup", "itempopup.xml"),
    mItemName(new Label(this)),
    mItemDesc(new TextBox(this)),
    mItemEffect(new TextBox(this)),
    mItemWeight(new TextBox(this)),
    mItemCards(new TextBox(this)),
    mItemType(ItemDbType::UNUSABLE),
    mIcon(new Icon(this, nullptr)),
    mLastName(),
    mLastId(0),
    mLastColor(ItemColor_one)
{
    // Item name
    mItemName->setFont(boldFont);
    mItemName->setPosition(0, 0);

    const int fontHeight = getFont()->getHeight();

    // Item description
    mItemDesc->setEditable(false);
    mItemDesc->setPosition(0, fontHeight);
    mItemDesc->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));

    // Item effect
    mItemEffect->setEditable(false);
    mItemEffect->setPosition(0, 2 * fontHeight);
    mItemEffect->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));

    // Item weight
    mItemWeight->setEditable(false);
    mItemWeight->setPosition(0, 3 * fontHeight);
    mItemWeight->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));

    // Item cards
    mItemCards->setEditable(false);
    mItemCards->setPosition(0, 4 * fontHeight);
    mItemCards->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));
}

void ItemPopup::postInit()
{
    Popup::postInit();
    add(mItemName);
    add(mItemDesc);
    add(mItemEffect);
    add(mItemWeight);
    add(mItemCards);
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

void ItemPopup::setItem(const Item *const item,
                        const bool showImage)
{
    if (!item)
        return;

    const ItemInfo &ii = item->getInfo();
    setItem(ii,
        item->getColor(),
        showImage,
        item->getId(),
        item->getCards());
    if (item->getRefine() > 0)
    {
        mLastName = ii.getName();
        mLastColor = item->getColor();
        mLastId = item->getId();
        if (serverFeatures->haveItemColors())
        {
            mItemName->setCaption(strprintf("%s (+%u), %d",
                ii.getName(item->getColor()).c_str(),
                CAST_U32(item->getRefine()),
                ii.getId()));
        }
        else
        {
            mItemName->setCaption(strprintf("%s (+%u), %d",
                ii.getName().c_str(),
                CAST_U32(item->getRefine()),
                ii.getId()));
        }
        mItemName->adjustSize();
        const unsigned minWidth = mItemName->getWidth() + 8;
        if (CAST_U32(getWidth()) < minWidth)
            setWidth(minWidth);
    }
}

void ItemPopup::setItem(const ItemInfo &item,
                        const ItemColor color,
                        const bool showImage,
                        int id,
                        const int *const cards)
{
    if (!mIcon || (item.getName() == mLastName && color == mLastColor
        && id == mLastId))
    {
        return;
    }

    if (id == -1)
        id = item.getId();

    int space = 0;

    Image *const oldImage = mIcon->getImage();
    if (oldImage)
        oldImage->decRef();

    if (showImage)
    {
        Image *const image = Loader::getImage(combineDye2(
            paths.getStringValue("itemIcons").append(item.getDisplay().image),
            item.getDyeIconColorsString(color)));

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
    mLastId = id;

    if (serverFeatures->haveItemColors())
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
    mItemCards->setTextWrapped(getCardsString(cards), 196);

    int minWidth = mItemName->getWidth() + space;

    if (mItemName->getWidth() + space > minWidth)
        minWidth = mItemName->getWidth() + space;
    if (mItemDesc->getMinWidth() > minWidth)
        minWidth = mItemDesc->getMinWidth();
    if (mItemEffect->getMinWidth() > minWidth)
        minWidth = mItemEffect->getMinWidth();
    if (mItemWeight->getMinWidth() > minWidth)
        minWidth = mItemWeight->getMinWidth();
    if (mItemCards->getMinWidth() > minWidth)
        minWidth = mItemCards->getMinWidth();

    const int numRowsDesc = mItemDesc->getNumberOfRows();
    const int numRowsEffect = mItemEffect->getNumberOfRows();
    const int numRowsWeight = mItemWeight->getNumberOfRows();
    const int numRowsCards = mItemCards->getNumberOfRows();
    const int height = getFont()->getHeight();

    if (item.getEffect().empty())
    {
        setContentSize(minWidth,
            (numRowsDesc + 2 + numRowsWeight + numRowsCards) * height);
        mItemWeight->setPosition(0, (numRowsDesc + 2) * height);
        mItemCards->setPosition(0, (numRowsDesc + numRowsWeight + 2) * height);
    }
    else
    {
        setContentSize(minWidth, (numRowsDesc + numRowsEffect + 2
            + numRowsWeight + numRowsCards) * height);
        mItemEffect->setPosition(0, (numRowsDesc + 2) * height);
        mItemWeight->setPosition(0, (numRowsDesc + numRowsEffect + 2)
            * height);
        mItemCards->setPosition(0, (numRowsDesc + numRowsEffect
            + numRowsWeight + 2) * height);
    }

    mItemDesc->setPosition(0, 2 * height);
}

std::string ItemPopup::getCardsString(const int *const cards)
{
    if (!cards)
        return std::string();

    std::string label;

    switch (cards[0])
    {
        case 0xfe:  // named item
        {
            const int32_t charId = cards[2] + 65536 * cards[3];
            std::string name = actorManager->findCharById(charId);
            if (name.empty())
            {
                name = toString(charId);
                beingHandler->requestNameByCharId(charId);
                mLastId = 0;  // allow recreate popup with same data
            }
            // TRANSLATORS: named item description
            label.append(strprintf(_("Item named: %s"), name.c_str()));
            return label;
        }
        case 0x00FFU:  // forged item
        {
            return label;
        }
        case 0xFF00U:
        {
            return label;
        }
        default:
        {
            for (int f = 0; f < maxCards; f ++)
            {
                const int id = cards[f];
                if (id)
                {
                    if (!label.empty())
                        label.append(" / ");
                    const ItemInfo &info = ItemDB::get(id);
                    label.append(info.getName());
                }
            }
            if (label.empty())
                return label;
            // TRANSLATORS: popup label
            return _("Cards: ") + label;
        }
    }
}

#define caseSetColor(name1, name2) \
    case name1: \
    { \
        return label->setForegroundColorAll(getThemeColor(name2), \
        getThemeColor(name2##_OUTLINE)); \
    }
void ItemPopup::setLabelColor(Label *label,
                              const ItemDbTypeT type) const
{
    switch (type)
    {
        caseSetColor(ItemDbType::UNUSABLE, ThemeColorId::GENERIC)
        caseSetColor(ItemDbType::USABLE, ThemeColorId::USABLE)
        caseSetColor(ItemDbType::EQUIPMENT_ONE_HAND_WEAPON,
            ThemeColorId::ONEHAND)
        caseSetColor(ItemDbType::EQUIPMENT_TWO_HANDS_WEAPON,
            ThemeColorId::TWOHAND)
        caseSetColor(ItemDbType::EQUIPMENT_TORSO, ThemeColorId::TORSO)
        caseSetColor(ItemDbType::EQUIPMENT_ARMS, ThemeColorId::ARMS)
        caseSetColor(ItemDbType::EQUIPMENT_HEAD, ThemeColorId::HEAD)
        caseSetColor(ItemDbType::EQUIPMENT_LEGS, ThemeColorId::LEGS)
        caseSetColor(ItemDbType::EQUIPMENT_SHIELD, ThemeColorId::SHIELD)
        caseSetColor(ItemDbType::EQUIPMENT_RING, ThemeColorId::RING)
        caseSetColor(ItemDbType::EQUIPMENT_NECKLACE, ThemeColorId::NECKLACE)
        caseSetColor(ItemDbType::EQUIPMENT_FEET, ThemeColorId::FEET)
        caseSetColor(ItemDbType::EQUIPMENT_AMMO, ThemeColorId::AMMO)
        caseSetColor(ItemDbType::EQUIPMENT_CHARM, ThemeColorId::CHARM)
        caseSetColor(ItemDbType::SPRITE_RACE, ThemeColorId::UNKNOWN_ITEM)
        caseSetColor(ItemDbType::SPRITE_HAIR, ThemeColorId::UNKNOWN_ITEM)
        caseSetColor(ItemDbType::CARD, ThemeColorId::CARD)
        default:
        {
            return label->setForegroundColorAll(getThemeColor(
                ThemeColorId::UNKNOWN_ITEM), getThemeColor(
                ThemeColorId::UNKNOWN_ITEM_OUTLINE));
        }
    }
}
#undef caseSetColor

void ItemPopup::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(Visible_false);
    resetPopup();
}

void ItemPopup::resetPopup()
{
    mLastName.clear();
    mLastColor = ItemColor_one;
    mLastId = 0;
}
