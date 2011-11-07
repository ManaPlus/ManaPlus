/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "textcommand.h"

#include "gui/theme.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "debug.h"

TextCommand::TextCommand(unsigned int id, std::string symbol,
                         std::string command,  std::string comment,
                         SpellTarget type, std::string icon,
                         unsigned int basicLvl, MagicSchool school,
                         unsigned int schoolLvl, int mana) :
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


TextCommand::TextCommand(unsigned int id, std::string symbol,
                         std::string command, std::string comment,
                         SpellTarget type, std::string icon) :
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

TextCommand::TextCommand(unsigned int id) :
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

    ResourceManager *resman = ResourceManager::getInstance();
    SpriteDisplay display = ItemDB::get(getIcon()).getDisplay();
    mImage = resman->getImage("graphics/items/" + display.image);

    if (!mImage)
        mImage = Theme::getImageFromTheme("unknown-item.png");
}
