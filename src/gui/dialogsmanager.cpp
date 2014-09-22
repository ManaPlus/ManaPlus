/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/dialogsmanager.h"

#include "configuration.h"
#include "settings.h"

#include "gui/dialogtype.h"

#include "gui/widgets/selldialog.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/buydialog.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/updaterwindow.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

#ifdef WIN32
#undef ERROR
#endif

extern OkDialog *deathNotice;

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
