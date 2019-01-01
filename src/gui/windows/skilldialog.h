/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "enums/simpletypes/autotarget.h"
#include "enums/simpletypes/modifiable.h"

#include "enums/resources/skill/casttype.h"
#include "enums/resources/skill/skillowner.h"
#include "enums/resources/skill/skilltype.h"

#include "listeners/actionlistener.h"

#include "utils/xml.h"

class Being;
class Button;
class Label;
class SkillModel;
class SkillTab;
class Tab;
class TabbedArea;

struct SkillData;
struct SkillInfo;

/**
 * The skill dialog.
 *
 * \ingroup Interface
 */
class SkillDialog final : public Window,
                          public ActionListener
{
    public:
        SkillDialog();

        A_DELETE_COPY(SkillDialog)

        ~SkillDialog() override final;

        void postInit() override final;

        /**
         * Called when receiving actions from widget.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Update the given skill's display
         */
        std::string update(const int id);

        /**
         * Update other parts of the display
         */
        void update();

        void updateModels();

        void updateModelsHidden();

        void loadXmlFile(const std::string &fileName,
                         const SkipError skipError);

        void clearSkills();

        void hideSkills(const SkillOwner::Type owner);

        void loadSkills();

        bool updateSkill(const int id,
                         const int range,
                         const Modifiable modifiable,
                         const SkillType::SkillType type,
                         const int sp);

        void addSkill(const SkillOwner::Type owner,
                      const int id,
                      const std::string &name,
                      const int level,
                      const int range,
                      const Modifiable modifiable,
                      const SkillType::SkillType type,
                      const int sp);

        SkillInfo* getSkill(const int id) const A_WARN_UNUSED;

        SkillInfo* getSkillByItem(const int itemId) const A_WARN_UNUSED;

        void setSkillDuration(const SkillOwner::Type owner,
                              const int id,
                              const int duration);

        bool hasSkills() const noexcept2 A_WARN_UNUSED
        { return !mSkills.empty(); }

        void widgetResized(const Event &event) override final;

        void useItem(const int itemId,
                     const AutoTarget autoTarget,
                     const int level,
                     const std::string &data) const;

        void updateTabSelection();

        void updateQuest(const int var,
                         const int val1,
                         const int val2,
                         const int val3,
                         const int time1);

        void playUpdateEffect(const int id) const;

        void playRemoveEffect(const int id) const;

        void playCastingDstTileEffect(const int id,
                                      const int level,
                                      const int x,
                                      const int y,
                                      const int delay) const;

        void slowLogic();

        void removeSkill(const int id);

        static void useSkill(const int skillId,
                             const AutoTarget autoTarget,
                             int level,
                             const bool withText,
                             const std::string &text,
                             CastTypeT castType,
                             const int offsetX,
                             const int offsetY);

        static void useSkill(const SkillInfo *const info,
                             const AutoTarget autoTarget,
                             int level,
                             const bool withText,
                             const std::string &text,
                             const CastTypeT castType,
                             const int offsetX,
                             const int offsetY);

        SkillData *getSkillData(const int id) const;

        SkillData *getSkillDataByLevel(const int id,
                                       const int level) const;

        void selectSkillLevel(const int skillId,
                              const int level);

        void selectSkillCastType(const int skillId,
                                 const CastTypeT type);

        void setSkillOffsetX(const int skillId,
                             const int offset);

        void setSkillOffsetY(const int skillId,
                             const int offset);

    private:
        static void useSkillDefault(const SkillInfo *const info,
                                    const AutoTarget autoTarget,
                                    int level,
                                    const bool withText,
                                    const std::string &text,
                                    int offsetX,
                                    int offsetY);

        static void useSkillTarget(const SkillInfo *const info,
                                   const AutoTarget autoTarget,
                                   int level,
                                   const bool withText,
                                   const std::string &text,
                                   const Being *being,
                                   int offsetX,
                                   int offsetY);

        static void useSkillPosition(const SkillInfo *const info,
                                     int level,
                                     const bool withText,
                                     const std::string &text,
                                     const int x,
                                     const int y,
                                     int offsetX,
                                     int offsetY);

        static std::string getDefaultSkillIcon(const SkillType::SkillType
                                               type);

        void addSkillDuration(SkillInfo *const skill);

        SkillInfo *loadSkill(XmlNodeConstPtr node,
                             SkillModel *const model);

        void loadSkillData(XmlNodeConstPtr node,
                           SkillInfo *const skill);

        void addDefaultTab();

        typedef std::map<int, SkillInfo*> SkillMap;
        SkillMap mSkills;
        STD_VECTOR<SkillInfo*> mDurations;
        TabbedArea *mTabs A_NONNULLPOINTER;
        std::list<Tab*> mDeleteTabs;
        Label *mPointsLabel A_NONNULLPOINTER;
        Button *mUseButton A_NONNULLPOINTER;
        Button *mIncreaseButton A_NONNULLPOINTER;
        SkillModel *mDefaultModel;
        SkillTab *mDefaultTab;
};

extern SkillDialog *skillDialog;

#endif  // GUI_WINDOWS_SKILLDIALOG_H
