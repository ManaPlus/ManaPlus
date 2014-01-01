/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WINDOWS_SKILLDIALOG_H
#define GUI_WINDOWS_SKILLDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

const int SKILL_MIN_ID = 200000;
const unsigned int SKILL_VAR_MIN_ID = 1000000;

class Button;
class Label;
class SkillModel;
class Tab;
class TabbedArea;

struct SkillInfo;

/**
 * The skill dialog.
 *
 * \ingroup Interface
 */
class SkillDialog final : public Window, public gcn::ActionListener
{
    public:
        SkillDialog();

        A_DELETE_COPY(SkillDialog)

        ~SkillDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from widget.
         */
        void action(const gcn::ActionEvent &event) override final;

        /**
         * Update the given skill's display
         */
        std::string update(const int id);

        /**
         * Update other parts of the display
         */
        void update();

        void clearSkills();

        void loadSkills();

        bool updateSkill(const int id, const int range, const bool modifiable);

        void addSkill(const int id, const int level, const int range,
                      const bool modifiable);

        SkillInfo* getSkill(const int id) const A_WARN_UNUSED;

        SkillInfo* getSkillByItem(const int itemId) const A_WARN_UNUSED;

        bool hasSkills() const A_WARN_UNUSED
        { return !mSkills.empty(); }

        void widgetResized(const gcn::Event &event) override final;

        void useItem(const int itemId) const;

        void updateTabSelection();

        void updateQuest(const int var, const int val);

        void playUpdateEffect(const int id) const;

    private:
        typedef std::map<int, SkillInfo*> SkillMap;
        SkillMap mSkills;
        TabbedArea *mTabs;
        std::list<Tab*> mDeleteTabs;
        Label *mPointsLabel;
        Button *mUseButton;
        Button *mIncreaseButton;
        SkillModel *mDefaultModel;
};

extern SkillDialog *skillDialog;

#endif  // GUI_WINDOWS_SKILLDIALOG_H
