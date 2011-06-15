/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GUI_SETUP_CHAT_H
#define GUI_SETUP_CHAT_H

#include "guichanfwd.h"

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class IntTextField;
class EditDialog;

class Setup_Chat : public SetupTab, public gcn::ActionListener
{
    public:
        Setup_Chat();

        void apply();
        void cancel();

        void action(const gcn::ActionEvent &event);

    private:
        gcn::CheckBox *mRemoveColorsCheckBox;
        bool mRemoveColors;

        gcn::CheckBox *mMagicInDebugCheckBox;
        bool mMagicInDebug;

        gcn::CheckBox *mAllowCommandsInChatTabsCheckBox;
        bool mAllowCommandsInChatTabs;

        gcn::CheckBox *mServerMsgInDebugCheckBox;
        bool mServerMsgInDebug;

        gcn::CheckBox *mShowChatColorsCheckBox;
        bool mShowChatColors;

        gcn::CheckBox *mMaxCharCheckBox;
        IntTextField *mMaxCharField;
        gcn::Button *mMaxCharButton;
        bool mMaxChar;

        gcn::CheckBox *mMaxLinesCheckBox;
        IntTextField *mMaxLinesField;
        gcn::Button *mMaxLinesButton;
        bool mMaxLines;

        gcn::CheckBox *mEnableChatLoggerCheckBox;
        bool mEnableChatLogger;

        gcn::CheckBox *mEnableTradeTabCheckBox;
        bool mEnableTradeTab;

        gcn::CheckBox *mHideShopMessagesCheckBox;
        bool mHideShopMessages;

        gcn::CheckBox *mShowChatHistoryCheckBox;
        bool mShowChatHistory;

        gcn::CheckBox *mEnableBattleTabCheckBox;
        bool mEnableBattleTab;

        gcn::CheckBox *mShowBattleEventsCheckBox;
        bool mShowBattleEvents;

        gcn::CheckBox *mHideChatInputCheckBox;
        bool mHideChatInput;

        gcn::CheckBox *mLocalTimeCheckBox;
        bool mLocalTime;

        EditDialog *mEditDialog;
};

#endif
