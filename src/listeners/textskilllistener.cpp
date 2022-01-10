/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "listeners/textskilllistener.h"

#include "gui/windows/textdialog.h"

#include "net/skillhandler.h"

#include "debug.h"

TextSkillListener::TextSkillListener() :
    ActionListener(),
    mDialog(nullptr),
    mSkillId(0),
    mX(0),
    mY(0),
    mLevel(0)
{
}

void TextSkillListener::setSkill(const int skillId,
                                 const int x,
                                 const int y,
                                 const int level)
{
    mSkillId = skillId;
    mX = x;
    mY = y;
    mLevel = level;
}

void TextSkillListener::action(const ActionEvent &event)
{
    if (event.getId() == "ok" && (mDialog != nullptr))
    {
        const std::string text = mDialog->getText();
        if (text.empty())
            return;
        skillHandler->usePos(mSkillId,
            mLevel,
            mX, mY,
            text);
    }
    mDialog = nullptr;
}
