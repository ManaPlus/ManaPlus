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

#include "gui/popupmanager.h"

#include "sdlshared.h"

#include "gui/viewport.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "utils/delete2.h"

#include "debug.h"

PopupManager *popupManager = nullptr;

PopupManager::PopupManager() :
    mBeingPopup(new BeingPopup),
    mTextPopup(new TextPopup)
{
    mBeingPopup->postInit();
    mTextPopup->postInit();
}

PopupManager::~PopupManager()
{
    delete2(mBeingPopup);
    delete2(mTextPopup);
}

void PopupManager::showPopup(Window *const parent,
                             const int x, const int y,
                             Item *const item,
                             const bool isInventory)
{
    popupMenu->showPopup(parent, x, y, item, isInventory);
}

void PopupManager::showPopup(MapItem *const item)
{
    popupMenu->showPopup(viewport->getMouseX(), viewport->getMouseY(), item);
}

void PopupManager::showPopup(Window *const parent,
                             Item *const item,
                             const bool isInventory)
{
    popupMenu->showPopup(parent, viewport->getMouseX(), viewport->getMouseY(),
        item, isInventory);
}

void PopupManager::showPopup(const int x, const int y,
                             const std::vector<ActorSprite*> &beings)
{
    popupMenu->showPopup(x, y, beings);
}

void PopupManager::showPopup(const int x, const int y,
                             const FloorItem *const floorItem)
{
    popupMenu->showPopup(x, y, floorItem);
}

void PopupManager::showPopup(const int x, const int y, MapItem *const mapItem)
{
    popupMenu->showPopup(x, y, mapItem);
}

void PopupManager::showItemPopup(Item *const item)
{
    popupMenu->showItemPopup(viewport->getMouseX(), viewport->getMouseY(),
        item);
}

void PopupManager::showItemPopup(const int itemId,
                                 const unsigned char color)
{
    popupMenu->showItemPopup(viewport->getMouseX(), viewport->getMouseY(),
        itemId, color);
}

void PopupManager::showDropPopup(Item *const item)
{
    popupMenu->showDropPopup(viewport->getMouseX(), viewport->getMouseY(),
        item);
}

void PopupManager::showOutfitsPopup(const int x, const int y)
{
    popupMenu->showOutfitsPopup(x, y);
}

void PopupManager::showOutfitsPopup()
{
    popupMenu->showOutfitsPopup(viewport->getMouseX(), viewport->getMouseY());
}

void PopupManager::showSpellPopup(TextCommand *const cmd)
{
    popupMenu->showSpellPopup(viewport->getMouseX(), viewport->getMouseY(),
        cmd);
}

void PopupManager::showChatPopup(const int x, const int y,
                                 ChatTab *const tab)
{
    popupMenu->showChatPopup(x, y, tab);
}

void PopupManager::showChatPopup(ChatTab *const tab)
{
    popupMenu->showChatPopup(viewport->getMouseX(), viewport->getMouseY(),
        tab);
}

void PopupManager::showPopup(const int x, const int y,
                             const Being *const being)
{
    popupMenu->showPopup(x, y, being);
}

void PopupManager::showPopup(const Being *const being)
{
    popupMenu->showPopup(viewport->getMouseX(), viewport->getMouseY(), being);
}

void PopupManager::showPlayerPopup(const std::string &nick)
{
    popupMenu->showPlayerPopup(viewport->getMouseX(), viewport->getMouseY(),
        nick);
}

void PopupManager::showPopup(const int x, const int y,
                             Button *const button)
{
    popupMenu->showPopup(x, y, button);
}

void PopupManager::showPopup(const int x, const int y,
                             const ProgressBar *const bar)
{
    popupMenu->showPopup(x, y, bar);
}

void PopupManager::showAttackMonsterPopup(const std::string &name,
                                          const ActorType::Type &type)
{
    popupMenu->showAttackMonsterPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        name,
        type);
}

void PopupManager::showPickupItemPopup(const std::string &name)
{
    popupMenu->showPickupItemPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        name);
}

void PopupManager::showUndressPopup(const int x, const int y,
                                    const Being *const being,
                                    Item *const item)
{
    popupMenu->showUndressPopup(x, y, being, item);
}

void PopupManager::showMapPopup(const int x, const int y)
{
    popupMenu->showMapPopup(viewport->getMouseX(), viewport->getMouseY(),
        x, y);
}

void PopupManager::showMapPopup(const int x1, const int y1,
                                const int x2, const int y2)
{
    popupMenu->showMapPopup(x1, y1, x2, y2);
}

void PopupManager::showTextFieldPopup(TextField *const input)
{
    popupMenu->showTextFieldPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        input);
}

void PopupManager::showLinkPopup(const std::string &link)
{
    popupMenu->showLinkPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        link);
}

void PopupManager::showWindowsPopup()
{
    popupMenu->showWindowsPopup(viewport->getMouseX(), viewport->getMouseY());
}

void PopupManager::showNpcDialogPopup(const int npcId)
{
    popupMenu->showNpcDialogPopup(npcId,
        viewport->getMouseX(),
        viewport->getMouseY());
}

void PopupManager::showWindowPopup(Window *const window)
{
    popupMenu->showWindowPopup(window, viewport->getMouseX(),
        viewport->getMouseY());
}

void PopupManager::closePopupMenu()
{
    if (popupMenu)
        popupMenu->handleLink("cancel", nullptr);
}

void PopupManager::hideBeingPopup()
{
    if (mBeingPopup)
        mBeingPopup->setVisible(false);
}

void PopupManager::hideTextPopup()
{
    if (mTextPopup)
        mTextPopup->setVisible(false);
}

void PopupManager::hidePopupMenu()
{
    if (popupMenu)
        popupMenu->setVisible(false);
}

bool PopupManager::isPopupMenuVisible() const
{
    return popupMenu ? popupMenu->isPopupVisible() : false;
}

void PopupManager::clearPopup()
{
    if (popupMenu)
        popupMenu->clear();
}

void PopupManager::showBeingPopup(const int x, const int y,
                                  Being *const b)
{
    if (mBeingPopup)
        mBeingPopup->show(x, y, b);
}

void PopupManager::showTextPopup(const int x, const int y,
                                 const std::string &text)
{
    if (mTextPopup)
        mTextPopup->show(x, y, text);
}

bool PopupManager::isTextPopupVisible() const
{
    return mTextPopup ? mTextPopup->isPopupVisible() : false;
}

bool PopupManager::isBeingPopupVisible() const
{
    return mBeingPopup ? mBeingPopup->isPopupVisible() : false;
}
