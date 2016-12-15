/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_TABS_SKILLTAB_H
#define GUI_WIDGETS_TABS_SKILLTAB_H

#include "gui/windows/skilldialog.h"

#include "gui/widgets/skilllistbox.h"
#include "gui/widgets/skillrectanglelistbox.h"

#include "gui/widgets/tabs/tab.h"

#include "localconsts.h"

class SkillTab final : public Tab
{
    public:
        SkillTab(const Widget2 *const widget,
                 const std::string &name,
                 SkillListBox *const listBox) :
            Tab(widget),
            mListBox(listBox),
            mRectangleListBox(nullptr)
        {
            setCaption(name);
        }

        SkillTab(const Widget2 *const widget,
                 const std::string &name,
                 SkillRectangleListBox *const listBox) :
            Tab(widget),
            mListBox(nullptr),
            mRectangleListBox(listBox)
        {
            setCaption(name);
        }

        A_DELETE_COPY(SkillTab)

        ~SkillTab()
        {
            delete2(mListBox)
            delete2(mRectangleListBox)
        }

        SkillInfo *getSelectedInfo() const
        {
            if (mListBox)
                return mListBox->getSelectedInfo();
            else if (mRectangleListBox)
                return mRectangleListBox->getSelectedInfo();
            else
                return nullptr;
        }

    protected:
        void setCurrent() override
        {
            if (skillDialog)
                skillDialog->updateTabSelection();
        }

    private:
        SkillListBox *mListBox;
        SkillRectangleListBox *mRectangleListBox;
};

#endif  // GUI_WIDGETS_TABS_SKILLTAB_H
