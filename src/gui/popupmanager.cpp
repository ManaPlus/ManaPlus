/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/popupmanager.h"

#include "sdlshared.h"

#ifndef DYECMD
#include "gui/popups/beingpopup.h"
#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#endif  // DYECMD
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
#ifndef DYECMD
    if (popupMenu != nullptr)
        popupMenu->handleLink("cancel", nullptr);
#endif  // DYECMD
}

void PopupManager::hideBeingPopup()
{
#ifndef DYECMD
    if (beingPopup != nullptr)
        beingPopup->setVisible(Visible_false);
#endif  // DYECMD
}

void PopupManager::hideTextPopup()
{
    if (textPopup != nullptr)
        textPopup->setVisible(Visible_false);
}

void PopupManager::hidePopupMenu()
{
#ifndef DYECMD
    if (popupMenu != nullptr)
        popupMenu->setVisible(Visible_false);
#endif  // DYECMD
}

void PopupManager::hideItemPopup()
{
#ifndef DYECMD
    if (itemPopup)
        itemPopup->hide();
#endif  // DYECMD
}

bool PopupManager::isPopupMenuVisible()
{
#ifndef DYECMD
    return popupMenu != nullptr ? popupMenu->isPopupVisible() : false;
#else
    return false;
#endif  // DYECMD
}

void PopupManager::clearPopup()
{
#ifndef DYECMD
    if (popupMenu != nullptr)
        popupMenu->clear();
#endif  // DYECMD
}

bool PopupManager::isTextPopupVisible()
{
    return textPopup != nullptr ? textPopup->isPopupVisible() : false;
}

bool PopupManager::isBeingPopupVisible()
{
#ifndef DYECMD
    return beingPopup != nullptr ? beingPopup->isPopupVisible() : false;
#else  // DYECMD
    return false;
#endif  // DYECMD
}
