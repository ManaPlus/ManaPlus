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

#ifndef SKILLMODEL_H
#define SKILLMODEL_H

#include "configuration.h"
#include "effectmanager.h"
#include "itemshortcut.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/setup.h"
#include "gui/shortcutwindow.h"
#include "gui/textpopup.h"
#include "gui/viewport.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/skillinfo.h"
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

#include "localconsts.h"

struct SkillInfo;

class SkillModel final : public gcn::ListModel
{
    public:
        SkillModel();

        SkillInfo *getSkillAt(const int i) const;

        std::string getElementAt(int i) override;

        int getNumberOfElements() override
        { return static_cast<int>(mVisibleSkills.size()); }

        void addSkill(SkillInfo *const info)
        { mSkills.push_back(info); }

        void updateVisibilities();

    private:
        SkillList mSkills;
        SkillList mVisibleSkills;
};

#endif
