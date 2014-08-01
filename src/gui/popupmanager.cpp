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
    mPopupMenu(new PopupMenu),
    mBeingPopup(new BeingPopup),
    mTextPopup(new TextPopup)
{
    mBeingPopup->postInit();
    mPopupMenu->postInit();
    mTextPopup->postInit();
}

PopupManager::~PopupManager()
{
    delete2(mPopupMenu);
    delete2(mBeingPopup);
    delete2(mTextPopup);
}

void PopupManager::showPopup(Window *const parent,
                             const int x, const int y,
                             Item *const item,
                             const bool isInventory)
{
    mPopupMenu->showPopup(parent, x, y, item, isInventory);
}

void PopupManager::showPopup(MapItem *const item)
{
    mPopupMenu->showPopup(viewport->getMouseX(), viewport->getMouseY(), item);
}

void PopupManager::showPopup(Window *const parent,
                             Item *const item,
                             const bool isInventory)
{
    mPopupMenu->showPopup(parent, viewport->getMouseX(), viewport->getMouseY(),
        item, isInventory);
}

void PopupManager::showPopup(const int x, const int y,
                             const std::vector<ActorSprite*> &beings)
{
    mPopupMenu->showPopup(x, y, beings);
}

void PopupManager::showPopup(const int x, const int y,
                             const FloorItem *const floorItem)
{
    mPopupMenu->showPopup(x, y, floorItem);
}

void PopupManager::showPopup(const int x, const int y, MapItem *const mapItem)
{
    mPopupMenu->showPopup(x, y, mapItem);
}

void PopupManager::showItemPopup(Item *const item)
{
    mPopupMenu->showItemPopup(viewport->getMouseX(), viewport->getMouseY(),
        item);
}

void PopupManager::showItemPopup(const int itemId,
                                 const unsigned char color)
{
    mPopupMenu->showItemPopup(viewport->getMouseX(), viewport->getMouseY(),
        itemId, color);
}

void PopupManager::showDropPopup(Item *const item)
{
    mPopupMenu->showDropPopup(viewport->getMouseX(), viewport->getMouseY(),
        item);
}

void PopupManager::showOutfitsPopup(const int x, const int y)
{
    mPopupMenu->showOutfitsPopup(x, y);
}

void PopupManager::showOutfitsPopup()
{
    mPopupMenu->showOutfitsPopup(viewport->getMouseX(), viewport->getMouseY());
}

void PopupManager::showSpellPopup(TextCommand *const cmd)
{
    mPopupMenu->showSpellPopup(viewport->getMouseX(), viewport->getMouseY(),
        cmd);
}

void PopupManager::showChatPopup(const int x, const int y,
                                 ChatTab *const tab)
{
    mPopupMenu->showChatPopup(x, y, tab);
}

void PopupManager::showChatPopup(ChatTab *const tab)
{
    mPopupMenu->showChatPopup(viewport->getMouseX(), viewport->getMouseY(),
        tab);
}

void PopupManager::showPopup(const int x, const int y,
                             const Being *const being)
{
    mPopupMenu->showPopup(x, y, being);
}

void PopupManager::showPopup(const Being *const being)
{
    mPopupMenu->showPopup(viewport->getMouseX(), viewport->getMouseY(), being);
}

void PopupManager::showPlayerPopup(const std::string &nick)
{
    mPopupMenu->showPlayerPopup(viewport->getMouseX(), viewport->getMouseY(),
        nick);
}

void PopupManager::showPopup(const int x, const int y,
                             Button *const button)
{
    mPopupMenu->showPopup(x, y, button);
}

void PopupManager::showPopup(const int x, const int y,
                             const ProgressBar *const bar)
{
    mPopupMenu->showPopup(x, y, bar);
}

void PopupManager::showAttackMonsterPopup(const std::string &name,
                                          const int type)
{
    mPopupMenu->showAttackMonsterPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        name,
        type);
}

void PopupManager::showPickupItemPopup(const std::string &name)
{
    mPopupMenu->showPickupItemPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        name);
}

void PopupManager::showUndressPopup(const int x, const int y,
                                    const Being *const being,
                                    Item *const item)
{
    mPopupMenu->showUndressPopup(x, y, being, item);
}

void PopupManager::showMapPopup(const int x, const int y)
{
    mPopupMenu->showMapPopup(viewport->getMouseX(), viewport->getMouseY(),
        x, y);
}

void PopupManager::showMapPopup(const int x1, const int y1,
                                const int x2, const int y2)
{
    mPopupMenu->showMapPopup(x1, y1, x2, y2);
}

void PopupManager::showTextFieldPopup(TextField *const input)
{
    mPopupMenu->showTextFieldPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        input);
}

void PopupManager::showLinkPopup(const std::string &link)
{
    mPopupMenu->showLinkPopup(viewport->getMouseX(),
        viewport->getMouseY(),
        link);
}

void PopupManager::showWindowsPopup()
{
    mPopupMenu->showWindowsPopup(viewport->getMouseX(), viewport->getMouseY());
}

void PopupManager::showNpcDialogPopup(const int npcId)
{
    mPopupMenu->showNpcDialogPopup(npcId,
        viewport->getMouseX(),
        viewport->getMouseY());
}

void PopupManager::showWindowPopup(Window *const window)
{
    mPopupMenu->showWindowPopup(window, viewport->getMouseX(),
        viewport->getMouseY());
}

void PopupManager::closePopupMenu()
{
    if (mPopupMenu)
        mPopupMenu->handleLink("cancel", nullptr);
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
    if (mPopupMenu)
        mPopupMenu->setVisible(false);
}

bool PopupManager::isPopupMenuVisible() const
{
    return mPopupMenu ? mPopupMenu->isPopupVisible() : false;
}

void PopupManager::clearPopup()
{
    if (mPopupMenu)
        mPopupMenu->clear();
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
