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

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include "resources/iteminfo.h"

#include "resources/db/itemoptiondb.h"
#include "resources/db/unitsdb.h"

#include "resources/image/image.h"

#include "resources/item/item.h"
#include "resources/item/itemfieldtype.h"
#include "resources/item/itemoptionslist.h"

#include "resources/loaders/imageloader.h"

#include "net/beinghandler.h"
#include "net/net.h"

#include "utils/translation/podict.h"

#include "debug.h"

ItemPopup *itemPopup = nullptr;

ItemPopup::ItemPopup() :
    Popup("ItemPopup", "itempopup.xml"),
    mItemName(new Label(this)),
    mItemDesc(new TextBox(this)),
    mItemEffect(new TextBox(this)),
    mItemWeight(new TextBox(this)),
    mItemCards(new TextBox(this)),
    mItemOptions(new TextBox(this)),
    mItemType(ItemDbType::UNUSABLE),
    mIcon(new Icon(this, nullptr)),
    mLastName(),
    mCardsStr(),
    mItemOptionsStr(),
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

    // Item options
    mItemOptions->setEditable(false);
    mItemOptions->setPosition(0, 5 * fontHeight);
    mItemOptions->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
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
    add(mItemOptions);
    add(mIcon);

    addMouseListener(this);
}

ItemPopup::~ItemPopup()
{
    if (mIcon != nullptr)
    {
        Image *const image = mIcon->getImage();
        if (image != nullptr)
            image->decRef();
    }
}

void ItemPopup::setItem(const Item *const item,
                        const bool showImage)
{
    if (item == nullptr)
        return;

    const ItemInfo &ii = item->getInfo();
    setItem(ii,
        item->getColor(),
        showImage,
        item->getId(),
        item->getCards(),
        item->getOptions());
    if (item->getRefine() > 0)
    {
        mLastName = ii.getName();
        mLastColor = item->getColor();
        mLastId = item->getId();
#ifdef TMWA_SUPPORT
        if (Net::getNetworkType() == ServerType::TMWATHENA)
        {
            mItemName->setCaption(strprintf("%s (+%u), %d",
                ii.getName().c_str(),
                CAST_U32(item->getRefine()),
                ii.getId()));
        }
        else
#endif  // TMWA_SUPPORT
        {
            mItemName->setCaption(strprintf("%s (+%u), %d",
                ii.getName(item->getColor()).c_str(),
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
                        const int *const cards,
                        const ItemOptionsList *const options)
{
    if (mIcon == nullptr)
        return;

    std::string cardsStr;
    std::string optionsStr;

    if (item.getName() == mLastName &&
        color == mLastColor &&
        id == mLastId)
    {
        cardsStr = getCardsString(cards);
        optionsStr = getOptionsString(options);
        if (mItemOptionsStr == optionsStr &&
            mCardsStr == cardsStr)
        {
            return;
        }
    }
    else
    {
        cardsStr = getCardsString(cards);
        optionsStr = getOptionsString(options);
    }
    mItemOptionsStr = optionsStr;
    mCardsStr = cardsStr;

    if (id == -1)
        id = item.getId();

    int space = 0;

    Image *const oldImage = mIcon->getImage();
    if (oldImage != nullptr)
        oldImage->decRef();

    if (showImage)
    {
        Image *const image = Loader::getImage(combineDye2(
            pathJoin(paths.getStringValue("itemIcons"),
            item.getDisplay().image),
            item.getDyeIconColorsString(color)));

        mIcon->setImage(image);
        if (image != nullptr)
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

#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() == ServerType::TMWATHENA)
    {
        mItemName->setCaption(strprintf("%s, %d",
            item.getName().c_str(), id));
        mItemDesc->setTextWrapped(item.getDescription(), 196);
    }
    else
#endif  // TMWA_SUPPORT
    {
        mItemName->setCaption(strprintf("%s, %d",
            item.getName(color).c_str(), id));
        mItemDesc->setTextWrapped(item.getDescription(color), 196);
    }

    mItemName->adjustSize();
    setLabelColor(mItemName, mItemType);
    mItemName->setPosition(space, 0);

    mItemEffect->setTextWrapped(item.getEffect(), 196);
    // TRANSLATORS: popup label
    mItemWeight->setTextWrapped(strprintf(_("Weight: %s"),
        UnitsDb::formatWeight(item.getWeight()).c_str()), 196);
    mItemCards->setTextWrapped(mCardsStr, 196);
    mItemOptions->setTextWrapped(optionsStr, 196);

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
    if (mItemOptions->getMinWidth() > minWidth)
        minWidth = mItemOptions->getMinWidth();

    const int numRowsDesc = mItemDesc->getNumberOfRows();
    const int numRowsEffect = mItemEffect->getNumberOfRows();
    const int numRowsWeight = mItemWeight->getNumberOfRows();
    const int numRowsCards = mItemCards->getNumberOfRows();
    const int numRowsOptions = mItemOptions->getNumberOfRows();
    const int height = getFont()->getHeight();

    if (item.getEffect().empty())
    {
        setContentSize(minWidth,
            (numRowsDesc + 2 + numRowsWeight + numRowsCards + numRowsOptions) *
            height);
        mItemWeight->setPosition(0, (numRowsDesc + 2) * height);
        mItemCards->setPosition(0, (numRowsDesc + numRowsWeight + 2) * height);
        mItemOptions->setPosition(0,
            (numRowsDesc + numRowsWeight + numRowsCards + 2) * height);
    }
    else
    {
        setContentSize(minWidth, (numRowsDesc + numRowsEffect + 2
            + numRowsWeight + numRowsCards + numRowsOptions) * height);
        mItemEffect->setPosition(0, (numRowsDesc + 2) * height);
        mItemWeight->setPosition(0, (numRowsDesc + numRowsEffect + 2)
            * height);
        mItemCards->setPosition(0, (numRowsDesc + numRowsEffect
            + numRowsWeight + 2) * height);
        mItemOptions->setPosition(0, (numRowsDesc + numRowsEffect
            + numRowsWeight + numRowsCards + 2) * height);
    }

    mItemDesc->setPosition(0, 2 * height);
}

std::string ItemPopup::getCardsString(const int *const cards)
{
    if (cards == nullptr)
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
                if (id != 0)
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

std::string ItemPopup::getOptionsString(const ItemOptionsList *const options)
{
    if (options == nullptr || translator == nullptr)
        return std::string();
    const size_t sz = options->size();
    std::string effect;
    for (size_t f = 0; f < sz; f ++)
    {
        const ItemOption &option = options->get(f);
        if (option.index == 0)
            continue;
        const std::vector<ItemFieldType*> &fields = ItemOptionDb::getFields(
            option.index);
        if (fields.empty())
            continue;
        const std::string valueStr = toString(option.value);
        FOR_EACH (std::vector<ItemFieldType*>::const_iterator, it, fields)
        {
            const ItemFieldType *const field = *it;
            std::string value = valueStr;
            if (!effect.empty())
                effect.append(" / ");
            if (field->sign && value[0] != '-')
                value = std::string("+").append(value);
            const std::string format = translator->getStr(field->description);
            effect.append(strprintf(format.c_str(),
                value.c_str()));
        }
    }
    if (effect.empty())
        return effect;
    // TRANSLATORS: popup label
    return _("Options: ") + effect;
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
