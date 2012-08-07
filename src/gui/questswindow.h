/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus developers
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

#ifndef QUESTS_WINDOW_H
#define QUESTS_WINDOW_H

#include "localconsts.h"

#include "gui/widgets/window.h"

#include "utils/xml.h"

#include <guichan/actionlistener.hpp>

#include <map>

class Button;
class BrowserBox;
class ExtendedListBox;
class ItemLinkHandler;
class ScrollArea;
class QuestsModel;

struct QuestItem;

class QuestsWindow : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        QuestsWindow();

        ~QuestsWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        void updateQuest(int var, int val);

        void rebuild(bool playSound);

        void showQuest(QuestItem *quest);

    private:
        void loadXml();

        void loadQuest(int var, XmlNodePtr node);

        QuestsModel *mQuestsModel;
        ExtendedListBox *mQuestsListBox;
        ScrollArea *mQuestScrollArea;
        ItemLinkHandler *mItemLinkHandler;
        BrowserBox *mText;
        ScrollArea *mTextScrollArea;
        Button *mCloseButton;
        std::map<int, int> mVars;
        std::map<int, std::vector<QuestItem*> > mQuests;
        std::vector<QuestItem*> mQuestLinks;
        Image *mCompleteIcon;
        Image *mIncompleteIcon;
};

extern QuestsWindow *questsWindow;

#endif
