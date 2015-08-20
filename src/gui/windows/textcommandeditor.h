/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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
        void showControls(const Visible show);

        void save();

        void deleteCommand();

        bool mIsMagicCommand;

        TextCommand*  mCommand;
        RadioButton*  mIsMagic          A_NONNULLPOINTER;
        RadioButton*  mIsOther          A_NONNULLPOINTER;
        Label*        mSymbolLabel      A_NONNULLPOINTER;
        TextField*    mSymbolTextField  A_NONNULLPOINTER;
        Label*        mCommandLabel     A_NONNULLPOINTER;
        TextField*    mCommandTextField A_NONNULLPOINTER;
        Label*        mCommentLabel     A_NONNULLPOINTER;
        TextField*    mCommentTextField A_NONNULLPOINTER;
        TargetTypeModel* mTargetTypeModel A_NONNULLPOINTER;
        Label*        mTypeLabel        A_NONNULLPOINTER;
        DropDown*     mTypeDropDown     A_NONNULLPOINTER;
        IconsModel*   mIconsModel       A_NONNULLPOINTER;
        Label*        mIconLabel        A_NONNULLPOINTER;
        DropDown*     mIconDropDown     A_NONNULLPOINTER;
        Label*        mManaLabel        A_NONNULLPOINTER;
        IntTextField* mManaField        A_NONNULLPOINTER;
        Label*        mMagicLvlLabel    A_NONNULLPOINTER;
        IntTextField* mMagicLvlField    A_NONNULLPOINTER;
        MagicSchoolModel* mMagicSchoolModel A_NONNULLPOINTER;
        Label*        mSchoolLabel      A_NONNULLPOINTER;
        DropDown*     mSchoolDropDown   A_NONNULLPOINTER;
        Label*        mSchoolLvlLabel   A_NONNULLPOINTER;
        IntTextField* mSchoolLvlField   A_NONNULLPOINTER;
        Button*       mCancelButton     A_NONNULLPOINTER;
        Button*       mSaveButton       A_NONNULLPOINTER;
        Button*       mDeleteButton     A_NONNULLPOINTER;

        bool mEnabledKeyboard;
};

#endif  // GUI_WINDOWS_TEXTCOMMANDEDITOR_H
