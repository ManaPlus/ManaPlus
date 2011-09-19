/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TEXTCOMMANDEDITOR_H
#define TEXTCOMMANDEDITOR_H

#include "gui/widgets/window.h"

#include "textcommand.h"

#include <guichan/actionlistener.hpp>

class Button;
class DropDown;
class IconsModal;
class IntTextField;
class Label;
class ListModel;
class MagicSchoolModel;
class RadioButton;
class TargetTypeModel;
class TextBox;
class TextCommand;
class TextField;

class TextCommandEditor : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        TextCommandEditor(TextCommand *command);

        /**
         * Destructor.
         */
        ~TextCommandEditor();

        void action(const gcn::ActionEvent &event);

        void update();

        void widgetResized(const gcn::Event &event);

        void updateList();

        void reset();

        void scheduleDelete();

    private:
        void showControls(bool show);

        void save();

        void deleteCommand();

        TextCommand *mCommand;
        bool mAdvanced;

        RadioButton *mIsMagic;
        RadioButton *mIsOther;
        Label *mSymbolLabel;
        TextField *mSymbolTextField;
        Label *mCommandLabel;
        TextField *mCommandTextField;

        Label *mCommentLabel;
        TextField *mCommentTextField;

        Label *mTypeLabel;
        DropDown *mTypeDropDown;
        Label *mIconLabel;
        DropDown *mIconDropDown;
        Label *mManaLabel;
        IntTextField *mManaField;
        Label *mMagicLvlLabel;
        IntTextField *mMagicLvlField;
        Label *mSchoolLabel;
        DropDown *mSchoolDropDown;
        Label *mSchoolLvlLabel;
        IntTextField *mSchoolLvlField;
        IconsModal *mIconsModal;
        TargetTypeModel *mTargetTypeModel;
        MagicSchoolModel *mMagicSchoolModel;

        //Button *mAdvancedButton;
        Button *mCancelButton;
        Button *mSaveButton;
        Button *mDeleteButton;

        bool mEnabledKeyboard;
        bool mIsMagicCommand;
};

#endif
