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

#include "gui/popupmanager.h"

#include "sdlshared.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "debug.h"

PopupManager *popupManager = nullptr;

PopupManager::PopupManager()
{
}

PopupManager::~PopupManager()
{
}

void PopupManager::closePopupMenu()
{
    if (popupMenu)
        popupMenu->handleLink("cancel", nullptr);
}

void PopupManager::hideBeingPopup()
{
    if (beingPopup)
        beingPopup->setVisible(Visible_false);
}

void PopupManager::hideTextPopup()
{
    if (textPopup)
        textPopup->setVisible(Visible_false);
}

void PopupManager::hidePopupMenu()
{
    if (popupMenu)
        popupMenu->setVisible(Visible_false);
}

bool PopupManager::isPopupMenuVisible()
{
    return popupMenu ? popupMenu->isPopupVisible() : false;
}

void PopupManager::clearPopup()
{
    if (popupMenu)
        popupMenu->clear();
}

bool PopupManager::isTextPopupVisible()
{
    return textPopup ? textPopup->isPopupVisible() : false;
}

bool PopupManager::isBeingPopupVisible()
{
    return beingPopup ? beingPopup->isPopupVisible() : false;
}
