/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/dialogsmanager.h"

#include "configuration.h"
#include "settings.h"

#include "enums/being/attributes.h"

#include "enums/gui/dialogtype.h"

#include "gui/widgets/selldialog.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/buydialog.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/updaterwindow.h"

#include "listeners/playerpostdeathlistener.h"
#include "listeners/weightlistener.h"

#include "net/inventoryhandler.h"

#include "resources/db/deaddb.h"

#include "utils/gettext.h"

#include "debug.h"

#ifdef WIN32
#undef ERROR
#endif

OkDialog *deathNotice = nullptr;
DialogsManager *dialogsManager = nullptr;
OkDialog *weightNotice = nullptr;
int weightNoticeTime = 0;

namespace
{
    PlayerPostDeathListener postDeathListener;
    WeightListener weightListener;
}  // namespace

DialogsManager::DialogsManager() :
    AttributeListener(),
    PlayerDeathListener()
{
}

void DialogsManager::closeDialogs()
{
    NpcDialog::clearDialogs();
    BuyDialog::closeAll();
    BuySellDialog::closeAll();
    NpcDialog::closeAll();
    SellDialog::closeAll();
    if (inventoryHandler)
        inventoryHandler->closeStorage();
    if (deathNotice)
    {
        deathNotice->scheduleDelete();
        deathNotice = nullptr;
    }
}

void DialogsManager::createUpdaterWindow()
{
    updaterWindow = new UpdaterWindow(settings.updateHost,
        settings.oldUpdates,
        false,
        0);
    updaterWindow->postInit();
}

Window *DialogsManager::openErrorDialog(const std::string &header,
                                        const std::string &message,
                                        const bool modal)
{
    if (settings.supportUrl.empty() || config.getBoolValue("hidesupport"))
    {
        return new OkDialog(header, message,
            // TRANSLATORS: ok dialog button
            _("Close"),
            DialogType::ERROR,
            modal, true, nullptr, 260);
    }
    else
    {
        ConfirmDialog *const dialog = new ConfirmDialog(
            header, strprintf("%s %s", message.c_str(),
            // TRANSLATORS: error message question
            _("Do you want to open support page?")),
            SOUND_ERROR, false, modal);
        dialog->postInit();
        return dialog;
    }
}

void DialogsManager::playerDeath()
{
    if (!deathNotice)
    {
        // TRANSLATORS: message header
        deathNotice = new OkDialog(_("Message"),
            DeadDB::getRandomString(),
            // TRANSLATORS: ok dialog button
            _("Revive"),
            DialogType::OK,
            false, true, nullptr, 260);
        deathNotice->addActionListener(&postDeathListener);
    }
}

void DialogsManager::attributeChanged(const int id,
                                      const int oldVal,
                                      const int newVal)
{
    if (id == Attributes::TOTAL_WEIGHT)
    {
        if (!weightNotice && config.getBoolValue("weightMsg"))
        {
            const int max = PlayerInfo::getAttribute(
                Attributes::MAX_WEIGHT) / 2;
            const int total = oldVal;
            if (newVal >= max && total < max)
            {
                weightNoticeTime = cur_time + 5;
                // TRANSLATORS: message header
                weightNotice = new OkDialog(_("Message"),
                    // TRANSLATORS: weight message
                    _("You are carrying more than "
                    "half your weight. You are "
                    "unable to regain health."),
                    // TRANSLATORS: ok dialog button
                    _("OK"),
                    DialogType::OK,
                    false, true, nullptr, 260);
                weightNotice->addActionListener(
                    &weightListener);
            }
            else if (newVal < max && total >= max)
            {
                weightNoticeTime = cur_time + 5;
                // TRANSLATORS: message header
                weightNotice = new OkDialog(_("Message"),
                    // TRANSLATORS: weight message
                    _("You are carrying less than "
                    "half your weight. You "
                    "can regain health."),
                    // TRANSLATORS: ok dialog button
                    _("OK"),
                    DialogType::OK,
                    false, true, nullptr, 260);
                weightNotice->addActionListener(
                    &weightListener);
            }
        }
    }
}
