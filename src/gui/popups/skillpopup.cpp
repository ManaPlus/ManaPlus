/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "gui/popups/skillpopup.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/skill/skilldata.h"
#include "resources/skill/skillinfo.h"

#include "debug.h"

SkillPopup *skillPopup = nullptr;

SkillPopup::SkillPopup() :
    Popup("SkillPopup", "skillpopup.xml"),
    mSkillName(new Label(this)),
    mSkillDesc(new TextBox(this)),
    mSkillEffect(new TextBox(this)),
    mSkillLevel(new TextBox(this)),
    mSkillCastType(new TextBox(this)),
    mCastType(CastType::Default),
    mLastId(0U),
    mLastLevel(-1),
    mOffsetX(0),
    mOffsetY(0)
{
    mSkillName->setFont(boldFont);
    mSkillName->setPosition(0, 0);
    mSkillName->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));

    const int fontHeight = getFont()->getHeight();

    mSkillDesc->setEditable(false);
    mSkillDesc->setPosition(0, fontHeight);
    mSkillDesc->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));

    mSkillEffect->setEditable(false);
    mSkillEffect->setPosition(0, 2 * fontHeight);
    mSkillEffect->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));

    mSkillLevel->setEditable(false);
    mSkillLevel->setPosition(0, 3 * fontHeight);
    mSkillLevel->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));

    mSkillCastType->setEditable(false);
    mSkillCastType->setPosition(0, 4 * fontHeight);
    mSkillCastType->setForegroundColorAll(
        getThemeColor(ThemeColorId::POPUP, 255U),
        getThemeColor(ThemeColorId::POPUP_OUTLINE, 255U));
}

void SkillPopup::postInit()
{
    Popup::postInit();
    add(mSkillName);
    add(mSkillDesc);
    add(mSkillEffect);
    add(mSkillLevel);
    add(mSkillCastType);

    addMouseListener(this);
}

SkillPopup::~SkillPopup()
{
}

void SkillPopup::show(const SkillInfo *const skill,
                      const int level,
                      const CastTypeT castType,
                      const int offsetX,
                      const int offsetY)
{
    if ((skill == nullptr) ||
        (skill->data == nullptr) ||
        (skill->id == mLastId &&
        level == mLastLevel &&
        castType == mCastType &&
        offsetX == mOffsetX &&
        offsetY == mOffsetY))
    {
        return;
    }

    mLastId = skill->id;
    mLastLevel = level;
    mCastType = castType;
    mOffsetX = offsetX;
    mOffsetY = offsetY;

    mSkillName->setCaption(skill->data->dispName);
    mSkillName->adjustSize();
    mSkillName->setPosition(0, 0);

    std::string description = skill->data->description;
    std::string effect = skill->skillEffect;
    if (description.empty())
    {
        description = effect;
        effect.clear();
    }
    mSkillDesc->setTextWrapped(description, 196);
    mSkillEffect->setTextWrapped(effect, 196);
    if (level != 0)
    {
        mSkillLevel->setTextWrapped(strprintf(
            // TRANSLATORS: skill level
            _("Level: %d / %d"), level, skill->level),
            196);
    }
    else
    {
        if (skill->level != 0)
        {
            mSkillLevel->setTextWrapped(strprintf(
                // TRANSLATORS: skill level
                _("Level: %d"), skill->level),
                196);
        }
        else
        {
            mSkillLevel->setTextWrapped(
                // TRANSLATORS: skill level for tmw fake skills
                _("Level: Unknown"),
                196);
        }
    }
    std::string castStr;
    switch (castType)
    {
        case CastType::Default:
        default:
            // TRANSLATORS: skill cast type
            castStr = _("Default");
            break;
        case CastType::Target:
            // TRANSLATORS: skill cast type
            castStr = _("Target");
            break;
        case CastType::Position:
            // TRANSLATORS: skill cast type
            castStr = _("Mouse position");
            break;
        case CastType::Self:
            // TRANSLATORS: skill cast type
            castStr = _("Self position");
            break;
    }
    if (offsetX != 0 ||
        offsetY != 0)
    {
        castStr.append(strprintf(" (%+d,%+d)",
            offsetX,
            offsetY));
    }
    mSkillCastType->setTextWrapped(strprintf(
        // TRANSLATORS: skill cast type
        _("Cast type: %s"),
        castStr.c_str()), 196);

    int minWidth = mSkillName->getWidth();

    if (mSkillName->getWidth() > minWidth)
        minWidth = mSkillName->getWidth();
    if (mSkillDesc->getMinWidth() > minWidth)
        minWidth = mSkillDesc->getMinWidth();
    if (mSkillEffect->getMinWidth() > minWidth)
        minWidth = mSkillEffect->getMinWidth();
    if (mSkillLevel->getMinWidth() > minWidth)
        minWidth = mSkillLevel->getMinWidth();
    if (mSkillCastType->getMinWidth() > minWidth)
        minWidth = mSkillCastType->getMinWidth();

    const int numRowsDesc = mSkillDesc->getNumberOfRows();
    const int numRowsLevel = mSkillLevel->getNumberOfRows();
    const int numRowsCast = mSkillCastType->getNumberOfRows();
    const int height = getFont()->getHeight();

    if (skill->skillEffect.empty())
    {
        setContentSize(minWidth,
            (numRowsDesc + numRowsLevel + numRowsCast + 1) * height);
        mSkillLevel->setPosition(0, (numRowsDesc + 1) * height);
        mSkillCastType->setPosition(0, (numRowsDesc + 2) * height);
    }
    else
    {
        const int numRowsEffect = mSkillEffect->getNumberOfRows();
        setContentSize(minWidth,
            (numRowsDesc + numRowsLevel + numRowsEffect + numRowsCast + 1) *
            height);
        mSkillEffect->setPosition(0, (numRowsDesc + 1) * height);
        mSkillLevel->setPosition(0,
            (numRowsDesc + numRowsEffect + 1) * height);
        mSkillCastType->setPosition(0,
            (numRowsDesc + numRowsEffect + 2) * height);
    }

    mSkillDesc->setPosition(0, 1 * height);
}

void SkillPopup::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(Visible_false);
    mLastId = 0;
}

void SkillPopup::reset()
{
    mLastId = 0;
    mLastLevel = 0;
    mCastType = CastType::Default;
    mOffsetX = 0;
    mOffsetY = 0;
}
