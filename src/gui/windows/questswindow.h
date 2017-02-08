/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_QUESTSWINDOW_H
#define GUI_WINDOWS_QUESTSWINDOW_H

#include "localconsts.h"

#include "enums/simpletypes/beingtypeid.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

#include "resources/questvar.h"

class Being;
class Button;
class BrowserBox;
class ExtendedListBox;
class ItemLinkHandler;
class Map;
class ScrollArea;
class QuestsModel;

struct QuestEffect;
struct QuestItem;

typedef std::map<BeingTypeId, const QuestEffect*> NpcQuestEffectMap;
typedef NpcQuestEffectMap::const_iterator NpcQuestEffectMapCIter;

class QuestsWindow final : public Window,
                           public ActionListener
{
    public:
        QuestsWindow();

        A_DELETE_COPY(QuestsWindow)

        ~QuestsWindow();

        void action(const ActionEvent &event) override final;

        void updateQuest(const int var,
                         const int val1,
                         const int val2,
                         const int val3,
                         const int time1);

        void rebuild(const bool playSound);

        void showQuest(const QuestItem *const quest);

        void setMap(const Map *const map);

        void updateEffects();

        void addEffect(Being *const being);

        void selectQuest(const int varId);

    private:
        QuestsModel *mQuestsModel A_NONNULLPOINTER;
        ExtendedListBox *mQuestsListBox A_NONNULLPOINTER;
        ScrollArea *mQuestScrollArea A_NONNULLPOINTER;
        ItemLinkHandler *mItemLinkHandler A_NONNULLPOINTER;
        BrowserBox *mText A_NONNULLPOINTER;
        ScrollArea *mTextScrollArea A_NONNULLPOINTER;
        Button *mCloseButton A_NONNULLPOINTER;
        Image *mCompleteIcon;
        Image *mIncompleteIcon;
        std::vector<const QuestEffect*> mMapEffects;
        NpcQuestVarMap *mVars;
        std::map<int, std::vector<QuestItem*> > *mQuests;
        std::vector<QuestEffect*> *mAllEffects;

        // npc effects for current map and values: npc, effect
        NpcQuestEffectMap mNpcEffects;
        std::vector<QuestItem*> mQuestLinks;
        std::map<int, int> mQuestReverseLinks;
        int mNewQuestEffectId;
        int mCompleteQuestEffectId;
        const Map *mMap;
};

extern QuestsWindow *questsWindow;

#endif  // GUI_WINDOWS_QUESTSWINDOW_H
