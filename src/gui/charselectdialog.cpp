/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/charselectdialog.h"

#include "client.h"
#include "units.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/changeemaildialog.h"
#include "gui/changepassworddialog.h"
#include "gui/charcreatedialog.h"
#include "gui/confirmdialog.h"
#include "gui/logindialog.h"
#include "gui/okdialog.h"
#include "gui/textdialog.h"
#include "gui/unregisterdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/textfield.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/messageout.h"
#include "net/net.h"

#include "resources/colordb.h"

#include "utils/gettext.h"

#include <guichan/focushandler.hpp>
#include <guichan/font.hpp>

#include <string>

#include "debug.h"

// Character slots per row in the dialog
static const int SLOTS_PER_ROW = 5;

/**
 * Listener for confirming character deletion.
 */
class CharDeleteConfirm final : public ConfirmDialog
{
    public:
        CharDeleteConfirm(CharSelectDialog *const m, const int index) :
            ConfirmDialog(_("Confirm Character Delete"),
                          _("Are you sure you want to delete this character?"),
                          false, false, m),
            mMaster(m),
            mIndex(index)
        {
        }

        void action(const gcn::ActionEvent &event)
        {
            if (event.getId() == "yes" && mMaster)
                mMaster->askPasswordForDeletion(mIndex);

            ConfirmDialog::action(event);
        }

    private:
        CharSelectDialog *mMaster;
        int mIndex;
};

class CharacterDisplay final : public Container
{
    public:
        CharacterDisplay(CharSelectDialog *const charSelectDialog);

        void setCharacter(Net::Character *const character);

        Net::Character *getCharacter() const
        { return mCharacter; }

        void requestFocus();

        void setActive(const bool active);

        bool isSelectFocused() const
        { return mButton->isFocused(); }

        bool isDeleteFocused() const
        { return mDelete->isFocused(); }

        void focusSelect()
        { mFocusHandler->requestFocus(mButton); }

        void focusDelete()
        { mFocusHandler->requestFocus(mDelete); }

    private:
        void update();

        Net::Character *mCharacter;

        PlayerBox *mPlayerBox;
        Label *mName;
        Label *mLevel;
        Label *mMoney;
        Button *mButton;
        Button *mDelete;
};

CharSelectDialog::CharSelectDialog(LoginData *const data):
    Window(_("Account and Character Management"), false, nullptr, "char.xml"),
    ActionListener(),
    KeyListener(),
    mLocked(false),
    mLoginData(data),
    mAccountNameLabel(new Label(mLoginData->username)),
    mLastLoginLabel(new Label(mLoginData->lastLogin)),
    mSwitchLoginButton(new Button(_("Switch Login"), "switch", this)),
    mChangePasswordButton(new Button(_("Change Password"),
                          "change_password", this)),
    mUnregisterButton(nullptr),
    mChangeEmailButton(nullptr),
    mCharacterEntries(0),
    mCharHandler(Net::getCharHandler()),
    mDeleteDialog(nullptr),
    mDeleteIndex(-1)
{
    setCloseButton(false);

    const int optionalActions = Net::getLoginHandler()
        ->supportedOptionalActions();

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mAccountNameLabel, 2);
    placer(2, 0, mLastLoginLabel);
    placer(0, 1, mSwitchLoginButton);

    if (optionalActions & Net::LoginHandler::Unregister)
    {
        mUnregisterButton = new Button(_("Unregister"),
                                       "unregister", this);
        placer(3, 1, mUnregisterButton);
    }

    placer(0, 2, mChangePasswordButton);

    if (optionalActions & Net::LoginHandler::ChangeEmail)
    {
        mChangeEmailButton = new Button(_("Change Email"),
                                        "change_email", this);
        placer(3, 2, mChangeEmailButton);
    }

    placer = getPlacer(0, 1);

    for (int i = 0; i < static_cast<int>(mLoginData->characterSlots); i++)
    {
        mCharacterEntries.push_back(new CharacterDisplay(this));
        placer(i % SLOTS_PER_ROW, static_cast<int>(i) / SLOTS_PER_ROW,
            mCharacterEntries[i]);
    }

    reflowLayout();

    addKeyListener(this);

    center();
    setVisible(true);

    Net::getCharHandler()->setCharSelectDialog(this);
    if (mCharacterEntries[0])
        mCharacterEntries[0]->requestFocus();
}

CharSelectDialog::~CharSelectDialog()
{
    Net::getCharHandler()->clear();
}

void CharSelectDialog::action(const gcn::ActionEvent &event)
{
    // Check if a button of a character was pressed
    const gcn::Widget *const sourceParent = event.getSource()->getParent();
    int selected = -1;
    for (unsigned int i = 0, sz = static_cast<unsigned int>(
         mCharacterEntries.size()); i < sz; ++i)
    {
        if (mCharacterEntries[i] == sourceParent)
        {
            selected = i;
            break;
        }
    }

    const std::string &eventId = event.getId();

    if (selected >= 0)
    {
        if (eventId == "use")
        {
            attemptCharacterSelect(selected);
        }
        else if (eventId == "new" &&
                 !mCharacterEntries[selected]->getCharacter())
        {
            // Start new character dialog
            CharCreateDialog *const charCreateDialog =
                new CharCreateDialog(this, selected);
            mCharHandler->setCharCreateDialog(charCreateDialog);
        }
        else if (eventId == "delete"
                 && mCharacterEntries[selected]->getCharacter())
        {
            new CharDeleteConfirm(this, selected);
        }
    }
    else if (eventId == "switch")
    {
        Client::setState(STATE_SWITCH_LOGIN);
    }
    else if (eventId == "change_password")
    {
        Client::setState(STATE_CHANGEPASSWORD);
    }
    else if (eventId == "change_email")
    {
        Client::setState(STATE_CHANGEEMAIL);
    }
    else if (eventId == "unregister")
    {
        Client::setState(STATE_UNREGISTER);
    }
    else if (eventId == "try delete character")
    {
        if (mDeleteDialog && mDeleteIndex != -1 && mDeleteDialog->getText()
            == LoginDialog::savedPassword)
        {
            attemptCharacterDelete(mDeleteIndex);
            mDeleteDialog = nullptr;
        }
        else
        {
            new OkDialog(_("Error"), _("Incorrect password"), DIALOG_ERROR);
        }
        mDeleteIndex = -1;
    }
}

void CharSelectDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int actionId = static_cast<KeyEvent*>(&keyEvent)->getActionId();
    switch (actionId)
    {
        case Input::KEY_GUI_CANCEL:
            keyEvent.consume();
            action(gcn::ActionEvent(mSwitchLoginButton,
                mSwitchLoginButton->getActionEventId()));
            break;

        case Input::KEY_GUI_RIGHT:
        {
            keyEvent.consume();
            int idx;
            int idx2;
            if (getFocusedContainer(idx, idx2))
            {
                idx ++;
                if (idx == SLOTS_PER_ROW)
                    break;
                setFocusedContainer(idx, idx2);
            }
            break;
        }

        case Input::KEY_GUI_LEFT:
        {
            keyEvent.consume();
            int idx;
            int idx2;
            if (getFocusedContainer(idx, idx2))
            {
                if (!idx || idx == SLOTS_PER_ROW)
                    break;
                idx --;
                setFocusedContainer(idx, idx2);
            }
            break;
        }

        case Input::KEY_GUI_UP:
        {
            keyEvent.consume();
            int idx;
            int idx2;
            if (getFocusedContainer(idx, idx2))
            {
                if (idx < SLOTS_PER_ROW && !idx2)
                    break;
                if (idx2 > 0)
                {
                    idx2 = 0;
                }
                else
                {
                    idx -= SLOTS_PER_ROW;
                    if (mCharacterEntries[idx]->getCharacter())
                        idx2 = 1;
                    else
                        idx2 = 0;
                }
                setFocusedContainer(idx, idx2);
            }
            break;
        }

        case Input::KEY_GUI_DOWN:
        {
            keyEvent.consume();
            int idx;
            int idx2;
            if (getFocusedContainer(idx, idx2))
            {
                if (idx >= SLOTS_PER_ROW && idx2)
                    break;
                if (idx2 > 0)
                {
                    idx += SLOTS_PER_ROW;
                    idx2 = 0;
                }
                else
                {
                    if (mCharacterEntries[idx]->getCharacter())
                        idx2 = 1;
                    else
                        idx += SLOTS_PER_ROW;
                }
                setFocusedContainer(idx, idx2);
            }
            break;
        }

        case Input::KEY_GUI_DELETE:
        {
            keyEvent.consume();
            int idx;
            int idx2;
            if (getFocusedContainer(idx, idx2)
                && mCharacterEntries[idx]->getCharacter())
            {
                new CharDeleteConfirm(this, idx);
            }
            break;
        }

        default:
            break;
    }
}

bool CharSelectDialog::getFocusedContainer(int &container, int &idx)
{
    for (int f = 0; f < static_cast<int>(mLoginData->characterSlots); f ++)
    {
        if (mCharacterEntries[f]->isSelectFocused())
        {
            container = f;
            idx = 0;
            return true;
        }
        else if (mCharacterEntries[f]->isDeleteFocused())
        {
            container = f;
            idx = 1;
            return true;
        }
    }
    return false;
}

void CharSelectDialog::setFocusedContainer(const int i, const int button)
{
    if (i >= 0 && i < static_cast<int>(mLoginData->characterSlots))
    {
        CharacterDisplay *const container = mCharacterEntries[i];
        if (button)
            container->focusDelete();
        else
            container->focusSelect();
    }
}

/**
 * Communicate character deletion to the server.
 */
void CharSelectDialog::attemptCharacterDelete(const int index)
{
    if (mLocked)
        return;

    mCharHandler->deleteCharacter(mCharacterEntries[index]->getCharacter());
    lock();
}

void CharSelectDialog::askPasswordForDeletion(const int index)
{
    mDeleteIndex = index;
    mDeleteDialog = new TextDialog(
        _("Enter password for deleting character"), _("Enter password:"),
        this, true);
    mDeleteDialog->setActionEventId("try delete character");
    mDeleteDialog->addActionListener(this);
}

/**
 * Communicate character selection to the server.
 */
void CharSelectDialog::attemptCharacterSelect(const int index)
{
    if (mLocked || !mCharacterEntries[index])
        return;

    setVisible(false);
    if (mCharHandler && mCharacterEntries[index])
    {
        mCharHandler->chooseCharacter(
            mCharacterEntries[index]->getCharacter());
    }
    lock();
}

void CharSelectDialog::setCharacters(const Net::Characters &characters)
{
    // Reset previous characters
    for (std::vector<CharacterDisplay*>::const_iterator
         iter = mCharacterEntries.begin(), iter_end = mCharacterEntries.end();
         iter != iter_end; ++iter)
    {
        (*iter)->setCharacter(nullptr);
    }

    for (Net::Characters::const_iterator i = characters.begin(),
         i_end = characters.end(); i != i_end; ++i)
    {
        if (!*i)
            continue;

        Net::Character *const character = *i;

        // Slots Number start at 1 for Manaserv, so we offset them by one.
#ifdef MANASERV_SUPPORT
        int characterSlot = character->slot;
        if (Net::getNetworkType() == ServerInfo::MANASERV && characterSlot > 0)
            --characterSlot;
#else
        const int characterSlot = character->slot;
#endif

        if (characterSlot >= static_cast<int>(mCharacterEntries.size()))
        {
            logger->log("Warning: slot out of range: %d", character->slot);
            continue;
        }

        mCharacterEntries[characterSlot]->setCharacter(character);
    }
}

void CharSelectDialog::lock()
{
    if (!mLocked)
        setLocked(true);
}

void CharSelectDialog::unlock()
{
    setLocked(false);
}

void CharSelectDialog::setLocked(const bool locked)
{
    mLocked = locked;

    if (mSwitchLoginButton)
        mSwitchLoginButton->setEnabled(!locked);
    if (mChangePasswordButton)
        mChangePasswordButton->setEnabled(!locked);
    if (mUnregisterButton)
        mUnregisterButton->setEnabled(!locked);
    if (mChangeEmailButton)
        mChangeEmailButton->setEnabled(!locked);

    for (size_t i = 0, sz = mCharacterEntries.size(); i < sz; ++i)
    {
        if (mCharacterEntries[i])
            mCharacterEntries[i]->setActive(!mLocked);
    }
}

bool CharSelectDialog::selectByName(const std::string &name,
                                    const SelectAction selAction)
{
    if (mLocked)
        return false;

    for (size_t i = 0, sz = mCharacterEntries.size(); i < sz; ++i)
    {
        if (mCharacterEntries[i])
        {
            const Net::Character *const character
                = mCharacterEntries[i]->getCharacter();
            if (character)
            {
                if (character->dummy && character->dummy->getName() == name)
                {
                    if (mCharacterEntries[i])
                        mCharacterEntries[i]->requestFocus();
                    if (selAction == Choose)
                        attemptCharacterSelect(static_cast<int>(i));
                    return true;
                }
            }
        }
    }

    return false;
}


CharacterDisplay::CharacterDisplay(CharSelectDialog *const charSelectDialog) :
    Container(),
    mCharacter(nullptr),
    mPlayerBox(new PlayerBox(nullptr)),
    mName(new Label("wwwwwwwwwwwwwwwwwwwwwwww")),
    mLevel(new Label("(888)")),
    mMoney(new Label("wwwwwwwww")),
    mButton(new Button("wwwwwwwww", "go", charSelectDialog)),
    mDelete(new Button(_("Delete"), "delete", charSelectDialog))
{
    LayoutHelper h(this);
    ContainerPlacer placer = h.getPlacer(0, 0);

    placer(0, 0, mPlayerBox, 3, 5);
    placer(0, 5, mName, 3);
    placer(0, 6, mLevel, 3);
    placer(0, 7, mMoney, 3);
    placer(0, 8, mButton, 3);
    placer(0, 9, mDelete, 3);

    update();

    // Setting the width so that the largest label fits.
    mName->adjustSize();
    mMoney->adjustSize();
/*
    int width = 74;
    if (width - 20 < mName->getWidth())
        width = 20 + mName->getWidth();
    if (width - 20 < mMoney->getWidth())
        width = 20 + mMoney->getWidth();
    h.reflowLayout(width, 112 + mName->getHeight() + mLevel->getHeight() +
            mMoney->getHeight() + mButton->getHeight() + mDelete->getHeight());
*/

    setWidth(110);
    setHeight(200);
}

void CharacterDisplay::setCharacter(Net::Character *const character)
{
    if (mCharacter == character)
        return;

    mCharacter = character;
    mPlayerBox->setPlayer(character ? character->dummy : nullptr);
    update();
}

void CharacterDisplay::requestFocus()
{
    mButton->requestFocus();
}

void CharacterDisplay::setActive(const bool active)
{
    mButton->setEnabled(active);
    mDelete->setEnabled(active);
}

void CharacterDisplay::update()
{
    if (mCharacter)
    {
        const LocalPlayer *const character = mCharacter->dummy;
        mButton->setCaption(_("Choose"));
        mButton->setActionEventId("use");
        mName->setCaption(strprintf("%s", character->getName().c_str()));
        mLevel->setCaption(strprintf(_("Level %d"),
            mCharacter->data.mAttributes[PlayerInfo::LEVEL]));
        mMoney->setCaption(Units::formatCurrency(
            mCharacter->data.mAttributes[PlayerInfo::MONEY]));

        mDelete->setVisible(true);
    }
    else
    {
        mButton->setCaption(_("Create"));
        mButton->setActionEventId("new");
        mName->setCaption(_("(empty)"));
        mLevel->setCaption(_("(empty)"));
        mMoney->setCaption(Units::formatCurrency(0));

        mDelete->setVisible(false);
    }

    // Recompute layout
    distributeResizedEvent();
}
