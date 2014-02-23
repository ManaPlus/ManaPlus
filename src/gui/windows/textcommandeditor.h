/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_WINDOWS_TEXTCOMMANDEDITOR_H
#define GUI_WINDOWS_TEXTCOMMANDEDITOR_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class DropDown;
class IconsModel;
class IntTextField;
class Label;
class MagicSchoolModel;
class RadioButton;
class TargetTypeModel;
class TextCommand;
class TextField;

class TextCommandEditor final : public Window,
                                public ActionListener
{
    public:
        /**
         * Constructor.
         */
        explicit TextCommandEditor(TextCommand *const command);

        A_DELETE_COPY(TextCommandEditor)

        /**
         * Destructor.
         */
        ~TextCommandEditor();

        void postInit() override final;

        void action(const ActionEvent &event) override final;

        void scheduleDelete() override final;

    private:
        void showControls(const bool show);

        void save();

        void deleteCommand();

        bool mIsMagicCommand;
        TextCommand *mCommand;

        RadioButton *mIsMagic;
        RadioButton *mIsOther;
        Label *mSymbolLabel;
        TextField *mSymbolTextField;
        Label *mCommandLabel;
        TextField *mCommandTextField;

        Label *mCommentLabel;
        TextField *mCommentTextField;

        TargetTypeModel *mTargetTypeModel;
        Label *mTypeLabel;
        DropDown *mTypeDropDown;
        IconsModel *mIconsModel;
        Label *mIconLabel;
        DropDown *mIconDropDown;
        Label *mManaLabel;
        IntTextField *mManaField;
        Label *mMagicLvlLabel;
        IntTextField *mMagicLvlField;
        MagicSchoolModel *mMagicSchoolModel;
        Label *mSchoolLabel;
        DropDown *mSchoolDropDown;
        Label *mSchoolLvlLabel;
        IntTextField *mSchoolLvlField;

        Button *mCancelButton;
        Button *mSaveButton;
        Button *mDeleteButton;

        bool mEnabledKeyboard;
};

#endif  // GUI_WINDOWS_TEXTCOMMANDEDITOR_H
