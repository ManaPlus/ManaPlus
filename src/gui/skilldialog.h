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

#ifndef SKILLDIALOG_H
#define SKILLDIALOG_H

#include "gui/widgets/window.h"

#include "resources/soundinfo.h"

#include <guichan/actionlistener.hpp>
#include <guichan/mouselistener.hpp>

#include <map>
#include <vector>

const int SKILL_MIN_ID = 200000;
const unsigned int SKILL_VAR_MIN_ID = 1000000;

class Button;
class Label;
class ScrollArea;
class SkillModel;
class Tab;
class TabbedArea;

struct SkillData final
{
    std::string name;
    std::string shortName;
    std::string dispName;
    std::string description;
    Image *icon;

    std::string particle;
    SoundInfo soundHit;
    SoundInfo soundMiss;

    SkillData();
    A_DELETE_COPY(SkillData)
    ~SkillData();

    void setIcon(const std::string &iconPath);
};

typedef std::map<int, SkillData*> SkillDataMap;
typedef SkillDataMap::iterator SkillDataMapIter;
typedef SkillDataMap::const_iterator SkillDataMapCIter;

struct SkillInfo final
{
    int level;
    std::string skillLevel;
    int skillLevelWidth;
    unsigned int id;
    bool modifiable;
    bool visible;
    SkillModel *model;
    std::string skillExp;
    float progress;
    int range;
    gcn::Color color;

    SkillData *data;
    SkillDataMap dataMap;

    SkillInfo();
    A_DELETE_COPY(SkillInfo)
    ~SkillInfo();

    void update();

    SkillData *getData(const int level);

    SkillData *getData1(const int level);

    void addData(const int level, SkillData *const data);
};

typedef std::vector<SkillInfo*> SkillList;
typedef SkillList::iterator SkillListIter;

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

        /**
         * Called when receiving actions from widget.
         */
        void action(const gcn::ActionEvent &event) override;

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

        SkillInfo* getSkill(int id) A_WARN_UNUSED;

        bool hasSkills() const A_WARN_UNUSED
        { return !mSkills.empty(); }

        void widgetResized(const gcn::Event &event) override;

        void useItem(const int itemId);

        void updateTabSelection();

        void updateQuest(const int var, const int val);

        void playUpdateEffect(const int id);

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
