/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef SKILLDIALOG_H
#define SKILLDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/mouselistener.hpp>

#include <map>
#include <vector>

const int SKILL_MIN_ID = 200000;

class Button;
class Label;
class ScrollArea;
class SkillModel;
class Tab;
class TabbedArea;

struct SkillInfo
{
    unsigned short id;
    std::string name;
    std::string shortName;
    std::string dispName;
    Image *icon;
    bool modifiable;
    bool visible;
    SkillModel *model;

    int level;
    std::string skillLevel;
    int skillLevelWidth;

    std::string skillExp;
    float progress;
    gcn::Color color;
    int range;
    std::string particle;
    std::string soundHit;
    std::string soundMiss;

    SkillInfo();

    ~SkillInfo();

    void setIcon(const std::string &iconPath);

    void update();

    void draw(Graphics *graphics, int y, int width);
};

typedef std::vector<SkillInfo*> SkillList;

/**
 * The skill dialog.
 *
 * \ingroup Interface
 */
class SkillDialog : public Window, public gcn::ActionListener
{
    public:
        SkillDialog();

        ~SkillDialog();

        /**
         * Called when receiving actions from widget.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Update the given skill's display
         */
        std::string update(int id);

        /**
         * Update other parts of the display
         */
        void update();

        void loadSkills(const std::string &file);

        bool updateSkill(int id, int range, bool modifiable);

        void addSkill(int id, int level, int range, bool modifiable);

        SkillInfo* getSkill(int id);

        bool hasSkills() const
        { return !mSkills.empty(); }

        void widgetResized(const gcn::Event &event);

        void useItem(int itemId);

        void updateTabSelection();

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

#endif
