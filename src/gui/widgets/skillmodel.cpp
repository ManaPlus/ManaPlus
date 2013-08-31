/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/skillmodel.h"

#include "configuration.h"
#include "effectmanager.h"
#include "itemshortcut.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/setup.h"
#include "gui/shortcutwindow.h"
#include "gui/textpopup.h"
#include "gui/viewport.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tab.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/skillhandler.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include <guichan/font.hpp>

#include <set>
#include <string>

#include "debug.h"

SkillModel::SkillModel() :
    mSkills(),
    mVisibleSkills()
{
}

SkillInfo *SkillModel::getSkillAt(const int i) const
{
    if (i < 0 || i >= static_cast<int>(mVisibleSkills.size()))
        return nullptr;
    return mVisibleSkills.at(i);
}

std::string SkillModel::getElementAt(int i)
{
    if (getSkillAt(i))
        return getSkillAt(i)->data->name;
    else
        return std::string();
}

void SkillModel::updateVisibilities()
{
    mVisibleSkills.clear();

    FOR_EACH (SkillList::const_iterator, it, mSkills)
    {
        if ((*it) && (*it)->visible)
            mVisibleSkills.push_back((*it));
    }
}
