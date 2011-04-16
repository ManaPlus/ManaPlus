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

#include "gui/textcommandeditor.h"

#include <SDL.h>
#include <SDL_thread.h>
#include <vector>
#include <algorithm>

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/table.h"
#include "gui/widgets/textfield.h"

#include "chatwindow.h"
#include "configuration.h"
#include "item.h"
#include "localplayer.h"
#include "main.h"
#include "keyboardconfig.h"
#include "spellmanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/itemdb.h"
#include "resources/iteminfo.h"

class IconsModal : public gcn::ListModel
{
public:
    IconsModal()
    {
        std::map<int, ItemInfo*> info = ItemDB::getItemInfos();
        std::list<std::string> tempStrings;

        for (std::map<int, ItemInfo*>::const_iterator
             i = info.begin(), i_end = info.end();
             i != i_end; ++i)
        {
            if (i->first < 0)
                continue;

            ItemInfo info = (*i->second);
            std::string name = info.getName();
            if (name != "unnamed" && !info.getName().empty()
                && info.getName() != "unnamed")
            {
                tempStrings.push_back(name);
            }
        }
        tempStrings.sort();
        mStrings.push_back("");
        for (std::list<std::string>::const_iterator i = tempStrings.begin(),
             i_end = tempStrings.end(); i != i_end; ++i)
        {
            mStrings.push_back(*i);
        }
    }

    virtual ~IconsModal()
    {}

    virtual int getNumberOfElements()
    {
        return static_cast<int>(mStrings.size());
    }

    virtual std::string getElementAt(int i)
    {
        if (i < 0 || i >= getNumberOfElements())
            return _("???");

        return mStrings.at(i);
    }
private:
    std::vector<std::string> mStrings;
};


const char *TARGET_TYPE_TEXT[3] =
{
    N_("No Target"),
    N_("Allow Target"),
    N_("Need Target"),
};

const char *MAGIC_SCHOOL_TEXT[6] =
{
    N_("General Magic"),
    N_("Life Magic"),
    N_("War Magic"),
    N_("Transmute Magic"),
    N_("Nature Magic"),
    N_("Astral Magic")
};

class TargetTypeModel : public gcn::ListModel
{
public:
    virtual ~TargetTypeModel() { }

    virtual int getNumberOfElements()
    {
        return 3;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return TARGET_TYPE_TEXT[i];
    }
};

class MagicSchoolModel : public gcn::ListModel
{
public:
    virtual ~MagicSchoolModel() { }

    virtual int getNumberOfElements()
    {
        return 6;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return MAGIC_SCHOOL_TEXT[i];
    }
};


TextCommandEditor::TextCommandEditor(TextCommand *command):
    Window(_("Command Editor"))
{
    int w = 350;
    int h = 350;

    mEnabledKeyboard = keyboard.isEnabled();
    keyboard.setEnabled(false);

    setWindowName("TextCommandEditor");
    //setCloseButton(true);
    setDefaultSize(w, h, ImageRect::CENTER);

    mAdvanced = false;
    mCommand = command;

    mIsMagicCommand = (command->getCommandType() == TEXT_COMMAND_MAGIC);

    mIsMagic = new RadioButton(_("magic"), "magic", mIsMagicCommand);
    mIsMagic->setActionEventId("magic");
    mIsMagic->addActionListener(this);

    mIsOther = new RadioButton(_("other"), "magic", !mIsMagicCommand);
    mIsOther->setActionEventId("other");
    mIsOther->addActionListener(this);


    mSymbolLabel = new Label(_("Symbol:"));
    mSymbolTextField = new TextField();

    mCommandLabel = new Label(_("Command:"));
    mCommandTextField = new TextField();

    mManaLabel = new Label(_("Mana:"));
    mManaField = new IntTextField(0);
    mManaField->setRange(0, 500);
    mManaField->setWidth(20);

    mTargetTypeModel = new TargetTypeModel;
    mTypeLabel = new Label(_("Target Type:"));
    mTypeDropDown = new DropDown(mTargetTypeModel);
    mTypeDropDown->setActionEventId("type");
    mTypeDropDown->addActionListener(this);

    mIconsModal = new IconsModal;
    mIconLabel = new Label(_("Icon:"));
    mIconDropDown = new DropDown(mIconsModal);
    mIconDropDown->setActionEventId("icon");
    mIconDropDown->addActionListener(this);
    mIconDropDown->setSelectedString(mCommand->getIcon());

    mMagicLvlLabel = new Label(_("Magic level:"));
    mMagicLvlField = new IntTextField(0);
    mMagicLvlField->setRange(0, 5);
    mMagicLvlField->setWidth(20);

    mMagicSchoolModel = new MagicSchoolModel;
    mSchoolLabel = new Label(_("Magic School:"));
    mSchoolDropDown = new DropDown(mMagicSchoolModel);
    mSchoolDropDown->setActionEventId("school");
    mSchoolDropDown->addActionListener(this);
    mSchoolDropDown->setSelected(0);

    mSchoolLvlLabel = new Label(_("School level:"));
    mSchoolLvlField = new IntTextField(0);
    mSchoolLvlField->setRange(0, 5);
    mSchoolLvlField->setWidth(20);

    mSaveButton = new Button(_("Save"), "save", this);
    mSaveButton->adjustSize();

    mCancelButton = new Button(_("Cancel"), "cancel", this);
    mCancelButton->adjustSize();

    mDeleteButton = new Button(_("Delete"), "delete", this);
    mDeleteButton->adjustSize();

    if (command->getCommandType() == TEXT_COMMAND_MAGIC)
        showControls(true);
    else
        showControls(false);

    mSymbolTextField->setText(command->getSymbol());
    mCommandTextField->setText(command->getCommand());
    mManaField->setValue(command->getMana());
    mTypeDropDown->setSelected(command->getTargetType());
    mMagicLvlField->setValue(command->getBaseLvl());
    mSchoolDropDown->setSelected(command->getSchool() - MAGIC_START_ID);
    mSchoolLvlField->setValue(command->getSchoolLvl());

    ContainerPlacer place;
    place = getPlacer(0, 0);

    place(0, 0, mIsMagic, 1);
    place(2, 0, mIsOther, 1);
    place(0, 1, mSymbolLabel, 2).setPadding(3);
    place(2, 1, mSymbolTextField, 3).setPadding(3);
    place(0, 2, mCommandLabel, 2).setPadding(3);
    place(2, 2, mCommandTextField, 4).setPadding(3);
    place(0, 3, mTypeLabel, 2).setPadding(3);
    place(2, 3, mTypeDropDown, 3).setPadding(3);

    place(0, 4, mIconLabel, 2).setPadding(3);
    place(2, 4, mIconDropDown, 3).setPadding(3);

    place(0, 5, mManaLabel, 2).setPadding(3);
    place(2, 5, mManaField, 3).setPadding(3);
    place(0, 6, mMagicLvlLabel, 2).setPadding(3);
    place(2, 6, mMagicLvlField, 3).setPadding(3);

    place(0, 7, mSchoolLabel, 2).setPadding(3);
    place(2, 7, mSchoolDropDown, 3).setPadding(3);
    place(0, 8, mSchoolLvlLabel, 2).setPadding(3);
    place(2, 8, mSchoolLvlField, 3).setPadding(3);

    place(0, 9, mSaveButton, 2).setPadding(3);
    place(2, 9, mCancelButton, 2).setPadding(3);
    place(4, 9, mDeleteButton, 2).setPadding(3);

    setWidth(w);
    setHeight(h);

    center();

    setVisible(true);
}

TextCommandEditor::~TextCommandEditor()
{
    delete mIconsModal;
    mIconsModal = 0;
    delete mTargetTypeModel;
    mTargetTypeModel = 0;
    delete mMagicSchoolModel;
    mMagicSchoolModel = 0;
}

void TextCommandEditor::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();

    if (eventId == "magic")
    {
        mIsMagicCommand = true;
        showControls(true);
    }
    else if (eventId == "other")
    {
        mIsMagicCommand = false;
        showControls(false);
    }
    else if (eventId == "save")
    {
        save();
        scheduleDelete();
    }
    else if (eventId == "cancel")
    {
        scheduleDelete();
    }
    else if (eventId == "delete")
    {
        deleteCommand();
        scheduleDelete();
    }
}

void TextCommandEditor::update()
{
}

void TextCommandEditor::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);
}

void TextCommandEditor::updateList()
{
}

void TextCommandEditor::reset()
{
}

void TextCommandEditor::showControls(bool show)
{
    mManaField->setVisible(show);
    mManaLabel->setVisible(show);
    mMagicLvlLabel->setVisible(show);
    mMagicLvlField->setVisible(show);
    mSchoolLabel->setVisible(show);
    mSchoolDropDown->setVisible(show);
    mSchoolLvlLabel->setVisible(show);
    mSchoolLvlField->setVisible(show);
}

void TextCommandEditor::scheduleDelete()
{
    keyboard.setEnabled(mEnabledKeyboard);
    Window::scheduleDelete();
}

void TextCommandEditor::save()
{
    if (mIsMagicCommand)
        mCommand->setCommandType(TEXT_COMMAND_MAGIC);
    else
        mCommand->setCommandType(TEXT_COMMAND_TEXT);

    mCommand->setSymbol(mSymbolTextField->getText());
    mCommand->setCommand(mCommandTextField->getText());
    mCommand->setMana(mManaField->getValue());
    mCommand->setTargetType(
            static_cast<SpellTarget>(mTypeDropDown->getSelected()));
    mCommand->setIcon(mIconDropDown->getSelectedString());
    mCommand->setBaseLvl(mMagicLvlField->getValue());
    mCommand->setSchool(static_cast<MagicSchool>(
            mSchoolDropDown->getSelected() + MAGIC_START_ID));
    mCommand->setSchoolLvl(mSchoolLvlField->getValue());
    if (spellManager)
        spellManager->save();
}

void TextCommandEditor::deleteCommand()
{
    mCommand->setCommandType(TEXT_COMMAND_TEXT);
    mCommand->setSymbol("");
    mCommand->setCommand("");
    mCommand->setMana(0);
    mCommand->setTargetType(NOTARGET);
    mCommand->setIcon("");
    mCommand->setBaseLvl(0);
    mCommand->setSchool(SKILL_MAGIC);
    mCommand->setSchoolLvl(0);
    if (spellManager)
        spellManager->save();
}
