/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2021  Andrei Karas
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
#ifdef TMWA_SUPPORT
class MagicSchoolModel;
#endif  // TMWA_SUPPORT

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
        ~TextCommandEditor() override final;

        void postInit() override final;

        void action(const ActionEvent &event) override final;

        void scheduleDelete() override final;

    private:
#ifdef TMWA_SUPPORT
        void showControls(const Visible show);
#endif  // TMWA_SUPPORT

        void save();

        void deleteCommand();

#ifdef TMWA_SUPPORT
        bool mIsMagicCommand;
#endif  // TMWA_SUPPORT

        TextCommand*  mCommand;
#ifdef TMWA_SUPPORT
        RadioButton*  mIsMagic          A_NONNULLPOINTER;
#endif  // TMWA_SUPPORT

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
#ifdef TMWA_SUPPORT
        Label*        mManaLabel        A_NONNULLPOINTER;
        IntTextField* mManaField        A_NONNULLPOINTER;
        Label*        mMagicLvlLabel    A_NONNULLPOINTER;
        IntTextField* mMagicLvlField    A_NONNULLPOINTER;
        MagicSchoolModel* mMagicSchoolModel A_NONNULLPOINTER;
        Label*        mSchoolLabel      A_NONNULLPOINTER;
        DropDown*     mSchoolDropDown   A_NONNULLPOINTER;
        Label*        mSchoolLvlLabel   A_NONNULLPOINTER;
        IntTextField* mSchoolLvlField   A_NONNULLPOINTER;
#endif  // TMWA_SUPPORT

        Button*       mCancelButton     A_NONNULLPOINTER;
        Button*       mSaveButton       A_NONNULLPOINTER;
        Button*       mDeleteButton     A_NONNULLPOINTER;

        bool mEnabledKeyboard;
};

#endif  // GUI_WINDOWS_TEXTCOMMANDEDITOR_H
