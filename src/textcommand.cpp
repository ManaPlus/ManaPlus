/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "textcommand.h"

#include "configuration.h"

#include "gui/theme.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "debug.h"

#ifdef TMWA_SUPPORT
TextCommand::TextCommand(const int id,
                         const std::string &symbol,
                         const std::string &command,
                         const std::string &comment,
                         const CommandTargetT type,
                         const std::string &icon,
                         const unsigned int basicLvl,
                         const MagicSchoolT school,
                         const unsigned int schoolLvl,
                         const unsigned int mana) :
    mCommand(command),
    mComment(comment),
    mSymbol(symbol),
    mTargetType(type),
    mIcon(icon),
    mId(id),
    mMana(mana),
    mSchool(school),
    mBaseLvl(basicLvl),
    mSchoolLvl(schoolLvl),
    mCommandType(TextCommandType::Magic),
    mImage(nullptr)
{
    loadImage();
}
#endif  // TMWA_SUPPORT

TextCommand::TextCommand(const int id,
                         const std::string &symbol,
                         const std::string &command,
                         const std::string &comment,
                         const CommandTargetT type,
                         const std::string &icon) :
    mCommand(command),
    mComment(comment),
    mSymbol(symbol),
    mTargetType(type),
    mIcon(icon),
    mId(id),
#ifdef TMWA_SUPPORT
    mMana(0U),
    mSchool(MagicSchool::SkillMagic),
    mBaseLvl(0),
    mSchoolLvl(0),
    mCommandType(TextCommandType::Text),
#endif  // TMWA_SUPPORT
    mImage(nullptr)
{
    loadImage();
}

TextCommand::TextCommand(const int id) :
    mCommand(""),
    mComment(""),
    mSymbol(""),
    mTargetType(CommandTarget::NoTarget),
    mIcon(""),
    mId(id),
#ifdef TMWA_SUPPORT
    mMana(0U),
    mSchool(MagicSchool::SkillMagic),
    mBaseLvl(0),
    mSchoolLvl(0),
    mCommandType(TextCommandType::Text),
#endif  // TMWA_SUPPORT
    mImage(nullptr)
{
    loadImage();
}


TextCommand::~TextCommand()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void TextCommand::loadImage()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }

    if (getIcon().empty())
        return;

    const SpriteDisplay display = ItemDB::get(getIcon()).getDisplay();
    mImage = Loader::getImage(paths.getStringValue("itemIcons").append(
        display.image));

    if (!mImage)
        mImage = Theme::getImageFromTheme("unknown-item.png");
}
