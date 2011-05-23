/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
                         std::string command, SpellTarget type,
                         std::string icon, unsigned int basicLvl,
                         MagicSchool school, unsigned int schoolLvl,
                         int mana) :
    mImage(0)
{
    mId = id;
    mCommand = command;
    mSymbol = symbol;
    mTargetType = type;
    mIcon = icon;
    mBaseLvl = basicLvl;
    mSchool = school;
    mSchoolLvl = schoolLvl;
    mMana = mana;
    mCommandType = TEXT_COMMAND_MAGIC;
    loadImage();
}


TextCommand::TextCommand(unsigned int id, std::string symbol,
                         std::string command, SpellTarget type,
                         std::string icon) :
    mImage(0)
{
    mId = id;
    mCommand = command;
    mSymbol = symbol;
    mTargetType = type;
    mIcon = icon;
    mCommandType = TEXT_COMMAND_TEXT;
    mBaseLvl = 0;
    mSchool = SKILL_MAGIC;
    mSchoolLvl = 0;
    mMana = 0;
    mCommandType = TEXT_COMMAND_TEXT;
    loadImage();
}

TextCommand::TextCommand(unsigned int id) :
    mImage(0)
{
    mId = id;
    mCommand = "";
    mSymbol = "";
    mTargetType = NOTARGET;
    mIcon = "";
    mCommandType = TEXT_COMMAND_TEXT;
    mBaseLvl = 0;
    mSchool = SKILL_MAGIC;
    mSchoolLvl = 0;
    mMana = 0;
    mCommandType = TEXT_COMMAND_TEXT;
    loadImage();
}


TextCommand::~TextCommand()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = 0;
    }
}

void TextCommand::loadImage()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = 0;
    }

    if (getIcon().empty())
        return;

    ResourceManager *resman = ResourceManager::getInstance();
    SpriteDisplay display = ItemDB::get(getIcon()).getDisplay();
    mImage = resman->getImage("graphics/items/" + display.image);

    if (!mImage)
        mImage = Theme::getImageFromTheme("unknown-item.png");
}