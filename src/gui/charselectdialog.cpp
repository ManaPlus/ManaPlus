/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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
#include "configuration.h"
#include "units.h"
#include "keyevent.h"

#include "gui/charcreatedialog.h"
#include "gui/confirmdialog.h"
#include "gui/logindialog.h"
#include "gui/okdialog.h"
#include "gui/textdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/characterdisplay.h"
#include "gui/widgets/characterviewnormal.h"
#include "gui/widgets/characterviewsmall.h"
#include "gui/widgets/container.h"
#include "gui/widgets/layout.h"

#include "net/logindata.h"
#include "net/loginhandler.h"

#include "utils/gettext.h"

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
            // TRANSLATORS: char deletion message
            ConfirmDialog(_("Confirm Character Delete"),
                          // TRANSLATORS: char deletion message
                          _("Are you sure you want to delete this character?"),
                          false, false, m),
            mMaster(m),
            mIndex(index)
        {
        }

        A_DELETE_COPY(CharDeleteConfirm)

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

CharSelectDialog::CharSelectDialog(LoginData *const data):
    // TRANSLATORS: char select dialog name
    Window(strprintf(_("Account %s (last login time %s)"),
        data->username.c_str(), data->lastLogin.c_str()),
        false, nullptr, "char.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mLoginData(data),
    // TRANSLATORS: char select dialog. button.
    mSwitchLoginButton(new Button(this, _("Switch Login"), "switch", this)),
    // TRANSLATORS: char select dialog. button.
    mChangePasswordButton(new Button(this, _("Change Password"),
                          "change_password", this)),
    mUnregisterButton(nullptr),
    mChangeEmailButton(nullptr),
    // TRANSLATORS: char select dialog. button.
    mPlayButton(new Button(this, _("Play"), "use", this)),
    // TRANSLATORS: char select dialog. button.
    mInfoButton(new Button(this, _("Info"), "info", this)),
    // TRANSLATORS: char select dialog. button.
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    mCharacterView(nullptr),
    mCharacterEntries(0),
    mCharServerHandler(Net::getCharServerHandler()),
    mDeleteDialog(nullptr),
    mDeleteIndex(-1),
    mLocked(false),
    mSmallScreen(mainGraphics->getWidth() < 470
                 || mainGraphics->getHeight() < 370)
{
    setCloseButton(true);
    setFocusable(true);

    const int optionalActions = Net::getLoginHandler()
        ->supportedOptionalActions();

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mSwitchLoginButton);

    int n = 1;
    if (optionalActions & Net::LoginHandler::Unregister)
    {
        // TRANSLATORS: char select dialog. button.
        mUnregisterButton = new Button(this, _("Unregister"),
                                       "unregister", this);
        placer(n, 0, mUnregisterButton);
        n ++;
    }

    placer(n, 0, mChangePasswordButton);
    n ++;

    if (optionalActions & Net::LoginHandler::ChangeEmail)
    {
        // TRANSLATORS: char select dialog. button.
        mChangeEmailButton = new Button(this, _("Change Email"),
                                        "change_email", this);
        placer(n, 0, mChangeEmailButton);
        n ++;
    }

    placer(n, 0, mDeleteButton);
    n ++;
    placer(n, 0, mInfoButton);
    n ++;

    for (int i = 0; i < static_cast<int>(mLoginData->characterSlots); i++)
    {
        CharacterDisplay *const character = new CharacterDisplay(this, this);
        character->setVisible(false);
        mCharacterEntries.push_back(character);
    }

    placer(0, 2, mPlayButton);

    if (!mSmallScreen)
    {
        mCharacterView = new CharacterViewNormal(
            this, &mCharacterEntries, mPadding);
        placer(0, 1, mCharacterView, 10);
        if (config.getIntValue("fontSize") > 18)
            reflowLayout(500 + 2 * mPadding);
        else
            reflowLayout(410 + 2 * mPadding);
    }
    else
    {
        // TRANSLATORS: char select dialog name
        setCaption(strprintf(_("Account %s"), mLoginData->username.c_str()));
        mCharacterView = new CharacterViewSmall(
            this, &mCharacterEntries, mPadding);
        mCharacterView->setWidth(mainGraphics->getWidth()
            - 2 * getPadding());
        placer(0, 1, mCharacterView, 10);
        reflowLayout();
    }
    addKeyListener(this);
    center();
    setVisible(true);
    requestFocus();

    Net::getCharServerHandler()->setCharSelectDialog(this);
    mCharacterView->show(0);
    updateState();
}

CharSelectDialog::~CharSelectDialog()
{
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
            mCharacterView->show(i);
            updateState();
            break;
        }
    }
    if (selected == -1)
        selected = mCharacterView->getSelected();

    const std::string &eventId = event.getId();

    if (selected >= 0)
    {
        if (eventId == "select")
        {
            logger->log("select");
        }
        else if (eventId == "use")
        {
            use(selected);
            return;
        }
        else if (eventId == "delete"
                 && mCharacterEntries[selected]->getCharacter())
        {
            new CharDeleteConfirm(this, selected);
            return;
        }
        else if (eventId == "info")
        {
            Net::Character *const character = mCharacterEntries[
                selected]->getCharacter();
            if (!character)
                return;

            const LocalPlayer *const data = character->dummy;
            if (!data)
                return;

            const std::string msg = strprintf(
                // TRANSLATORS: char select dialog. player info message.
                _("Hp: %u/%u\nMp: %u/%u\nLevel: %u\n"
                "Experience: %u\nMoney: %s"),
                character->data.mAttributes[PlayerInfo::HP],
                character->data.mAttributes[PlayerInfo::MAX_HP],
                character->data.mAttributes[PlayerInfo::MP],
                character->data.mAttributes[PlayerInfo::MAX_MP],
                character->data.mAttributes[PlayerInfo::LEVEL],
                character->data.mAttributes[PlayerInfo::EXP],
                Units::formatCurrency(
                character->data.mAttributes[PlayerInfo::MONEY]).c_str());
            new OkDialog(data->getName(), msg, DIALOG_SILENCE);
        }
    }
    if (eventId == "switch")
    {
        Net::getCharServerHandler()->clear();
        close();
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
        Net::getCharServerHandler()->clear();
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
            // TRANSLATORS: error message
            new OkDialog(_("Error"), _("Incorrect password"), DIALOG_ERROR);
        }
        mDeleteIndex = -1;
    }
}

void CharSelectDialog::use(const int selected)
{
    if (mCharacterEntries[selected]
        && mCharacterEntries[selected]->getCharacter())
    {
        attemptCharacterSelect(selected);
    }
    else
    {
        CharCreateDialog *const charCreateDialog =
            new CharCreateDialog(this, selected);
        mCharServerHandler->setCharCreateDialog(charCreateDialog);
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
            int idx = mCharacterView->getSelected();
            if (idx >= 0)
            {
                idx ++;
                if (idx == SLOTS_PER_ROW)
                    break;
                mCharacterView->show(idx);
                updateState();
            }
            break;
        }

        case Input::KEY_GUI_LEFT:
        {
            keyEvent.consume();
            int idx = mCharacterView->getSelected();
            if (idx >= 0)
            {
                if (!idx || idx == SLOTS_PER_ROW)
                    break;
                idx --;
                mCharacterView->show(idx);
                updateState();
            }
            break;
        }

        case Input::KEY_GUI_UP:
        {
            keyEvent.consume();
            int idx = mCharacterView->getSelected();
            if (idx >= 0)
            {
                if (idx < SLOTS_PER_ROW)
                    break;
                idx -= SLOTS_PER_ROW;
                mCharacterView->show(idx);
                updateState();
            }
            break;
        }

        case Input::KEY_GUI_DOWN:
        {
            keyEvent.consume();
            int idx = mCharacterView->getSelected();
            if (idx >= 0)
            {
                if (idx >= SLOTS_PER_ROW)
                    break;
                idx += SLOTS_PER_ROW;
                mCharacterView->show(idx);
                updateState();
            }
            break;
        }

        case Input::KEY_GUI_DELETE:
        {
            keyEvent.consume();
            int idx = mCharacterView->getSelected();
            if (idx >= 0 && mCharacterEntries[idx]
                && mCharacterEntries[idx]->getCharacter())
            {
                new CharDeleteConfirm(this, idx);
            }
            break;
        }

        case Input::KEY_GUI_SELECT:
        {
            keyEvent.consume();
            use(mCharacterView->getSelected());
            break;
        }
        default:
            break;
    }
}

/**
 * Communicate character deletion to the server.
 */
void CharSelectDialog::attemptCharacterDelete(const int index)
{
    if (mLocked)
        return;

    if (mCharacterEntries[index])
    {
        mCharServerHandler->deleteCharacter(
            mCharacterEntries[index]->getCharacter());
    }
    lock();
}

void CharSelectDialog::askPasswordForDeletion(const int index)
{
    mDeleteIndex = index;
    mDeleteDialog = new TextDialog(
        // TRANSLATORS: char deletion question.
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
    if (mCharServerHandler)
    {
        mCharServerHandler->chooseCharacter(
            mCharacterEntries[index]->getCharacter());
    }
    lock();
}

void CharSelectDialog::setCharacters(const Net::Characters &characters)
{
    // Reset previous characters
    FOR_EACH (std::vector<CharacterDisplay*>::const_iterator,
              iter, mCharacterEntries)
    {
        if (*iter)
            (*iter)->setCharacter(nullptr);
    }

    FOR_EACH (Net::Characters::const_iterator, i, characters)
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

        if (mCharacterEntries[characterSlot])
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
    if (mPlayButton)
        mPlayButton->setEnabled(!locked);
    if (mDeleteButton)
        mDeleteButton->setEnabled(!locked);

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
                    mCharacterView->show(static_cast<int>(i));
                    updateState();
                    if (selAction == Choose)
                        attemptCharacterSelect(static_cast<int>(i));
                    return true;
                }
            }
        }
    }

    return false;
}

void CharSelectDialog::close()
{
    Client::setState(STATE_SWITCH_LOGIN);
    Window::close();
}

void CharSelectDialog::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);
    if (mCharacterView)
        mCharacterView->resize();
}

void CharSelectDialog::updateState()
{
    const int idx = mCharacterView->getSelected();
    if (idx == -1)
    {
        mPlayButton->setEnabled(false);
        return;
    }
    mPlayButton->setEnabled(true);

    if (mCharacterEntries[idx] && mCharacterEntries[idx]->getCharacter())
    {
        // TRANSLATORS: char select dialog. button.
        mPlayButton->setCaption(_("Play"));
    }
    else
    {
        // TRANSLATORS: char select dialog. button.
        mPlayButton->setCaption(_("Create"));
    }
}
