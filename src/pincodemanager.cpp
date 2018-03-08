/*
 *  The ManaPlus Client
 *  Copyright (C) 2018  The ManaPlus Developers
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

#include "pincodemanager.h"

#include "client.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/pincodedialog.h"

#include "gui/widgets/createwidget.h"

#include "listeners/changepincodelistener.h"
#include "listeners/newpincodelistener.h"
#include "listeners/newpincoderetrylistener.h"
#include "listeners/pincodelistener.h"

#include "net/charserverhandler.h"

#include "utils/gettext.h"

#include "debug.h"

PincodeManager pincodeManager;

PincodeManager::PincodeManager() :
    mOldPincode(),
    mNewPincode(),
    mSeed(0U),
    mAccountId(BeingId_zero),
    mDialog(nullptr),
    mState(PincodeState::None)
{
}

PincodeManager::~PincodeManager()
{
}

void PincodeManager::init()
{
    mSeed = 0;
    mAccountId = BeingId_zero;
    mState = PincodeState::None;
    mNewPincode.clear();
}

void PincodeManager::clearDialog(const PincodeDialog *const PincodeDialog)
{
    if (mDialog == PincodeDialog)
        mDialog = nullptr;
}

void PincodeManager::updateState()
{
    switch (mState)
    {
        case PincodeState::Ask:
            CREATEWIDGETV(mDialog, PincodeDialog,
                // TRANSLATORS: dialog caption
                _("Pincode"),
                // TRANSLATORS: dialog label
                _("Enter pincode"),
                mSeed,
                nullptr);
            mDialog->requestFocus();
            mDialog->setActionEventId("ok");
            mDialog->addActionListener(&pincodeListener);
            break;
        case PincodeState::Create:
            mNewPincode.clear();
            CREATEWIDGETV(mDialog, PincodeDialog,
                // TRANSLATORS: dialog caption
                _("New pincode"),
                // TRANSLATORS: dialog label
                _("Enter new pincode"),
                mSeed,
                nullptr);
            mDialog->requestFocus();
            mDialog->setActionEventId("ok");
            mDialog->addActionListener(&newPincodeListener);
            break;
        case PincodeState::Change:
            mOldPincode.clear();
            mNewPincode.clear();
            CREATEWIDGETV(mDialog, PincodeDialog,
                // TRANSLATORS: dialog caption
                _("Change pincode"),
                // TRANSLATORS: dialog label
                _("Enter old pincode"),
                mSeed,
                nullptr);
            mDialog->requestFocus();
            mDialog->setActionEventId("ok");
            mDialog->addActionListener(&changePincodeListener);
            break;
        case PincodeState::None:
        default:
            break;
    }
}

void PincodeManager::setNewPincode(const std::string &pincode)
{
    if (mNewPincode.empty())
    {   // first pincode
        mNewPincode = pincode;
        CREATEWIDGETV(mDialog, PincodeDialog,
            // TRANSLATORS: dialog caption
            _("New pincode"),
            // TRANSLATORS: dialog label
            _("Confirm new pincode"),
            mSeed,
            nullptr);
        mDialog->requestFocus();
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&newPincodeListener);
    }
    else
    {   // pincode confirmation
        if (mNewPincode != pincode)
        {
            mNewPincode.clear();
            CREATEWIDGETV(mDialog, OkDialog,
                // TRANSLATORS: error header
                _("Pincode"),
                // TRANSLATORS: error message
                _("Wrong pincode confirmation!"),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
            mDialog->addActionListener(&newPincodeRetryListener);
        }
        else
        {
            charServerHandler->setNewPincode(mAccountId,
                mNewPincode);
            mNewPincode.clear();
        }
    }
}

void PincodeManager::changePincode(const std::string &pincode)
{
    if (mOldPincode.empty())
    {   // set old pincode
        mOldPincode = pincode;
        CREATEWIDGETV(mDialog, PincodeDialog,
            // TRANSLATORS: dialog caption
            _("Change pincode"),
            // TRANSLATORS: dialog label
            _("Enter new pincode"),
            mSeed,
            nullptr);
        mDialog->requestFocus();
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&changePincodeListener);
    }
    else if (mNewPincode.empty())
    {   // set first new pincode
        mNewPincode = pincode;
        CREATEWIDGETV(mDialog, PincodeDialog,
            // TRANSLATORS: dialog caption
            _("Change pincode"),
            // TRANSLATORS: dialog label
            _("Confirm new pincode"),
            mSeed,
            nullptr);
        mDialog->requestFocus();
        mDialog->setActionEventId("ok");
        mDialog->addActionListener(&changePincodeListener);
    }
    else
    {   // new pincode confirmation
        if (mNewPincode != pincode)
        {
            mOldPincode.clear();
            mNewPincode.clear();
            CREATEWIDGETV(mDialog, OkDialog,
                // TRANSLATORS: error header
                _("Pincode"),
                // TRANSLATORS: error message
                _("Wrong pincode confirmation!"),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
            mDialog->addActionListener(&newPincodeRetryListener);
        }
        else
        {
            charServerHandler->changePincode(mAccountId,
                mOldPincode,
                mNewPincode);
            mOldPincode.clear();
            mNewPincode.clear();
        }
    }
}

void PincodeManager::sendPincode(const std::string &pincode)
{
    charServerHandler->sendCheckPincode(mAccountId,
        pincode);
}

void PincodeManager::pinOk()
{
    mState = PincodeState::None;
    if (client)
        client->focusWindow();
}

void PincodeManager::wrongPin()
{
    mState = PincodeState::Ask;
    updateState();
}

void PincodeManager::closeDialogs()
{
    if (mDialog)
    {
        mDialog->scheduleDelete();
        mDialog = nullptr;
    }
}

bool PincodeManager::isBlocked()
{
    return mState != PincodeState::None;
}
