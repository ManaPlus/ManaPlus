/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "resources/db/itemdb.h"

#include "debug.h"

TextCommand::TextCommand(const unsigned int id, const std::string &symbol,
                         const std::string &command,
                         const std::string &comment,
                         const SpellTarget type, const std::string &icon,
                         const unsigned int basicLvl, const MagicSchool school,
                         const unsigned int schoolLvl, const int mana) :
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
    mCommandType(TEXT_COMMAND_MAGIC),
    mImage(nullptr)
{
    loadImage();
}


TextCommand::TextCommand(const unsigned int id, const std::string &symbol,
                         const std::string &command,
                         const std::string &comment,
                         const SpellTarget type, const std::string &icon) :
    mCommand(command),
    mComment(comment),
    mSymbol(symbol),
    mTargetType(type),
    mIcon(icon),
    mId(id),
    mMana(0),
    mSchool(SKILL_MAGIC),
    mBaseLvl(0),
    mSchoolLvl(0),
    mCommandType(TEXT_COMMAND_TEXT),
    mImage(nullptr)
{
    loadImage();
}

TextCommand::TextCommand(const unsigned int id) :
    mCommand(""),
    mComment(""),
    mSymbol(""),
    mTargetType(NOTARGET),
    mIcon(""),
    mId(id),
    mMana(0),
    mSchool(SKILL_MAGIC),
    mBaseLvl(0),
    mSchoolLvl(0),
    mCommandType(TEXT_COMMAND_TEXT),
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

    ResourceManager *const resman = ResourceManager::getInstance();
    const SpriteDisplay display = ItemDB::get(getIcon()).getDisplay();
    mImage = resman->getImage(paths.getStringValue("itemIcons")
        .append(display.image));

    if (!mImage)
        mImage = Theme::getImageFromTheme("unknown-item.png");
}
