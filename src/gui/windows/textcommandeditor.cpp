/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/windows/textcommandeditor.h"

#include "spellmanager.h"
#include "textcommand.h"

#include "input/keyboardconfig.h"

#include "gui/models/iconsmodel.h"
#include "gui/models/targettypemodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layoutcell.h"

#ifdef TMWA_SUPPORT
#include "gui/models/magicschoolmodel.h"

#include "gui/widgets/radiobutton.h"
#endif  // TMWA_SUPPORT

#include "utils/delete2.h"

#include "debug.h"

#ifdef TMWA_SUPPORT
const unsigned int MAGIC_START_ID = 340;
#endif  // TMWA_SUPPORT

TextCommandEditor::TextCommandEditor(TextCommand *const command) :
    // TRANSLATORS: command editor name
    Window(_("Command Editor"), Modal_false, nullptr, "commandeditor.xml"),
    ActionListener(),
#ifdef TMWA_SUPPORT
    mIsMagicCommand(command != nullptr ?
        (command->getCommandType() == TextCommandType::Magic) : false),
#endif  // TMWA_SUPPORT
    mCommand(command),
#ifdef TMWA_SUPPORT
    // TRANSLATORS: command editor button
    mIsMagic(new RadioButton(this, _("magic"), "magic", mIsMagicCommand)),
    // TRANSLATORS: command editor button
    mIsOther(new RadioButton(this, _("other"), "magic", !mIsMagicCommand)),
#endif  // TMWA_SUPPORT
    // TRANSLATORS: command editor label
    mSymbolLabel(new Label(this, _("Symbol:"))),
    mSymbolTextField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    // TRANSLATORS: command editor label
    mCommandLabel(new Label(this, _("Command:"))),
    mCommandTextField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    // TRANSLATORS: command editor label
    mCommentLabel(new Label(this, _("Comment:"))),
    mCommentTextField(new TextField(this, std::string(),
         LoseFocusOnTab_true, nullptr, std::string(), false)),
    mTargetTypeModel(new TargetTypeModel),
    // TRANSLATORS: command editor label
    mTypeLabel(new Label(this, _("Target Type:"))),
    mTypeDropDown(new DropDown(this, mTargetTypeModel)),
    mIconsModel(new IconsModel),
    // TRANSLATORS: command editor label
    mIconLabel(new Label(this, _("Icon:"))),
    mIconDropDown(new DropDown(this, mIconsModel)),
#ifdef TMWA_SUPPORT
    // TRANSLATORS: command editor label
    mManaLabel(new Label(this, _("Mana:"))),
    mManaField(new IntTextField(this, 0, 0, 0, Enable_true, 0)),
    // TRANSLATORS: command editor label
    mMagicLvlLabel(new Label(this, _("Magic level:"))),
    mMagicLvlField(new IntTextField(this, 0, 0, 0, Enable_true, 0)),
    mMagicSchoolModel(new MagicSchoolModel),
    // TRANSLATORS: command editor label
    mSchoolLabel(new Label(this, _("Magic School:"))),
    mSchoolDropDown(new DropDown(this, mMagicSchoolModel)),
    // TRANSLATORS: command editor label
    mSchoolLvlLabel(new Label(this, _("School level:"))),
    mSchoolLvlField(new IntTextField(this, 0, 0, 0, Enable_true, 0)),
#endif  // TMWA_SUPPORT
    // TRANSLATORS: command editor button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    // TRANSLATORS: command editor button
    mSaveButton(new Button(this, _("Save"), "save", this)),
    // TRANSLATORS: command editor button
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    mEnabledKeyboard(keyboard.isEnabled())
{
    const int w = 350;
    const int h = 370;

    keyboard.setEnabled(false);

    setWindowName("TextCommandEditor");
    setDefaultSize(w, h, ImagePosition::CENTER);

#ifdef TMWA_SUPPORT
    mIsMagic->setActionEventId("magic");
    mIsMagic->addActionListener(this);

    mIsOther->setActionEventId("other");
    mIsOther->addActionListener(this);

    mManaField->setRange(0, 500);
    mManaField->setWidth(20);

    mMagicLvlField->setRange(0, 5);
    mMagicLvlField->setWidth(20);

    mSchoolDropDown->setActionEventId("school");
    mSchoolDropDown->addActionListener(this);
    mSchoolDropDown->setSelected(0);

    mSchoolLvlField->setRange(0, 5);
    mSchoolLvlField->setWidth(20);
#endif  // TMWA_SUPPORT

    mTypeDropDown->setActionEventId("type");
    mTypeDropDown->addActionListener(this);

    mIconDropDown->setActionEventId("icon");
    mIconDropDown->addActionListener(this);
    if (mCommand != nullptr)
        mIconDropDown->setSelectedString(mCommand->getIcon());

    mSaveButton->adjustSize();
    mCancelButton->adjustSize();
    mDeleteButton->adjustSize();

    if (command != nullptr)
    {
#ifdef TMWA_SUPPORT
        if (command->getCommandType() == TextCommandType::Magic)
            showControls(Visible_true);
        else
            showControls(Visible_false);

        mManaField->setValue(command->getMana());
        mMagicLvlField->setValue(command->getBaseLvl());
        mSchoolDropDown->setSelected(CAST_S32(command->getSchool())
            - MAGIC_START_ID);
        mSchoolLvlField->setValue(command->getSchoolLvl());
#endif  // TMWA_SUPPORT
    }

    ContainerPlacer placer(nullptr, nullptr);
    placer = getPlacer(0, 0);

#ifdef TMWA_SUPPORT
    placer(0, 0, mIsMagic, 1, 1);
    placer(2, 0, mIsOther, 1, 1);
    placer(0, 1, mSymbolLabel, 2, 1).setPadding(3);
    placer(2, 1, mSymbolTextField, 3, 1).setPadding(3);
    placer(0, 2, mCommandLabel, 2, 1).setPadding(3);
    placer(2, 2, mCommandTextField, 4, 1).setPadding(3);

    placer(0, 3, mCommentLabel, 2, 1).setPadding(3);
    placer(2, 3, mCommentTextField, 4, 1).setPadding(3);

    placer(0, 4, mTypeLabel, 2, 1).setPadding(3);
    placer(2, 4, mTypeDropDown, 3, 1).setPadding(3);

    placer(0, 5, mIconLabel, 2, 1).setPadding(3);
    placer(2, 5, mIconDropDown, 3, 1).setPadding(3);

    placer(0, 6, mManaLabel, 2, 1).setPadding(3);
    placer(2, 6, mManaField, 3, 1).setPadding(3);
    placer(0, 7, mMagicLvlLabel, 2, 1).setPadding(3);
    placer(2, 7, mMagicLvlField, 3, 1).setPadding(3);

    placer(0, 8, mSchoolLabel, 2, 1).setPadding(3);
    placer(2, 8, mSchoolDropDown, 3, 1).setPadding(3);
    placer(0, 9, mSchoolLvlLabel, 2, 1).setPadding(3);
    placer(2, 9, mSchoolLvlField, 3, 1).setPadding(3);

    placer(0, 10, mSaveButton, 2, 1).setPadding(3);
    placer(2, 10, mCancelButton, 2, 1).setPadding(3);
    placer(4, 10, mDeleteButton, 2, 1).setPadding(3);
#else  // TMWA_SUPPORT

    placer(0, 0, mSymbolLabel, 2, 1).setPadding(3);
    placer(2, 0, mSymbolTextField, 3, 1).setPadding(3);
    placer(0, 1, mCommandLabel, 2, 1).setPadding(3);
    placer(2, 1, mCommandTextField, 4, 1).setPadding(3);

    placer(0, 2, mCommentLabel, 2, 1).setPadding(3);
    placer(2, 2, mCommentTextField, 4, 1).setPadding(3);

    placer(0, 3, mTypeLabel, 2, 1).setPadding(3);
    placer(2, 3, mTypeDropDown, 3, 1).setPadding(3);

    placer(0, 4, mIconLabel, 2, 1).setPadding(3);
    placer(2, 4, mIconDropDown, 3, 1).setPadding(3);

    placer(0, 5, mSaveButton, 2, 1).setPadding(3);
    placer(2, 5, mCancelButton, 2, 1).setPadding(3);
    placer(4, 5, mDeleteButton, 2, 1).setPadding(3);
#endif  // TMWA_SUPPORT

    setWidth(w);
    setHeight(h);

    reflowLayout(w);

    center();
}

void TextCommandEditor::postInit()
{
    Window::postInit();
    enableVisibleSound(true);
    setVisible(Visible_true);
}

TextCommandEditor::~TextCommandEditor()
{
    delete2(mIconsModel);
    delete2(mTargetTypeModel);
#ifdef TMWA_SUPPORT
    delete2(mMagicSchoolModel);
#endif  // TMWA_SUPPORT
}

void TextCommandEditor::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "save")
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
#ifdef TMWA_SUPPORT
    else if (eventId == "magic")
    {
        mIsMagicCommand = true;
        showControls(Visible_true);
    }
    else if (eventId == "other")
    {
        mIsMagicCommand = false;
        showControls(Visible_false);
    }
#endif  // TMWA_SUPPORT
}

#ifdef TMWA_SUPPORT
void TextCommandEditor::showControls(const Visible show)
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
#endif  // TMWA_SUPPORT

void TextCommandEditor::scheduleDelete()
{
    keyboard.setEnabled(mEnabledKeyboard);
    Window::scheduleDelete();
}

void TextCommandEditor::save()
{
    if (mCommand == nullptr)
        return;
#ifdef TMWA_SUPPORT
    if (mIsMagicCommand)
        mCommand->setCommandType(TextCommandType::Magic);
    else
        mCommand->setCommandType(TextCommandType::Text);
#endif  // TMWA_SUPPORT

    mCommand->setSymbol(mSymbolTextField->getText());
    mCommand->setCommand(mCommandTextField->getText());
    mCommand->setComment(mCommentTextField->getText());
    mCommand->setTargetType(
            static_cast<CommandTargetT>(mTypeDropDown->getSelected()));
    mCommand->setIcon(mIconDropDown->getSelectedString());

#ifdef TMWA_SUPPORT
    mCommand->setMana(mManaField->getValue());
    mCommand->setBaseLvl(mMagicLvlField->getValue());
    mCommand->setSchool(static_cast<MagicSchoolT>(
            mSchoolDropDown->getSelected() + MAGIC_START_ID));
    mCommand->setSchoolLvl(mSchoolLvlField->getValue());
#endif  // TMWA_SUPPORT

    if (spellManager != nullptr)
        spellManager->save();
}

void TextCommandEditor::deleteCommand()
{
    if (mCommand == nullptr)
        return;
    mCommand->setSymbol("");
    mCommand->setCommand("");
    mCommand->setComment("");
    mCommand->setTargetType(CommandTarget::NoTarget);
    mCommand->setIcon("");
#ifdef TMWA_SUPPORT
    mCommand->setCommandType(TextCommandType::Text);
    mCommand->setMana(0);
    mCommand->setBaseLvl(0);
    mCommand->setSchool(MagicSchool::SkillMagic);
    mCommand->setSchoolLvl(0);
#endif  // TMWA_SUPPORT

    if (spellManager != nullptr)
        spellManager->save();
}
