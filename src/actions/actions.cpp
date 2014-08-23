/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "actions/actions.h"

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "render/graphics.h"

#include "net/ipc.h"
#include "net/net.h"
#include "net/playerhandler.h"
#include "net/tradehandler.h"

#include "listeners/updatestatuslistener.h"

#include "resources/map/map.h"

#include "utils/gettext.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;
extern std::string tradePartnerName;
extern QuitDialog *quitDialog;

namespace Actions
{

impHandler(emote)
{
    const int emotion = 1 + event.action - InputAction::EMOTE_1;
    if (emotion > 0)
    {
        if (emoteShortcut)
            emoteShortcut->useEmote(emotion);
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }

    return false;
}

impHandler(outfit)
{
    if (inputManager.isActionActive(InputAction::WEAR_OUTFIT))
    {
        const int num = event.action - InputAction::OUTFIT_1;
        if (outfitWindow && num >= 0)
        {
            outfitWindow->wearOutfit(num);
            if (Game::instance())
                Game::instance()->setValidSpeed();
            return true;
        }
    }
    else if (inputManager.isActionActive(InputAction::COPY_OUTFIT))
    {
        const int num = event.action - InputAction::OUTFIT_1;
        if (outfitWindow && num >= 0)
        {
            outfitWindow->copyOutfit(num);
            if (Game::instance())
                Game::instance()->setValidSpeed();
            return true;
        }
    }

    return false;
}

impHandler0(mouseClick)
{
    if (!guiInput || !gui)
        return false;

    int mouseX, mouseY;
    Gui::getMouseState(&mouseX, &mouseY);
    guiInput->simulateMouseClick(mouseX, mouseY, MouseButton::RIGHT);
    return true;
}

impHandler0(ok)
{
    // Close the Browser if opened
    if (helpWindow && helpWindow->isWindowVisible())
    {
        helpWindow->setVisible(false);
        return true;
    }
    // Close the config window, cancelling changes if opened
    else if (setupWindow && setupWindow->isWindowVisible())
    {
        setupWindow->action(ActionEvent(nullptr, "cancel"));
        return true;
    }
    else if (NpcDialog *const dialog = NpcDialog::getActive())
    {
        dialog->action(ActionEvent(nullptr, "ok"));
        return true;
    }
    return false;
}

impHandler(shortcut)
{
    if (itemShortcutWindow)
    {
        const int num = itemShortcutWindow->getTabIndex();
        if (num >= 0 && num < static_cast<int>(SHORTCUT_TABS))
        {
            if (itemShortcut[num])
            {
                itemShortcut[num]->useItem(event.action
                    - InputAction::SHORTCUT_1);
            }
        }
        return true;
    }
    return false;
}

impHandler0(quit)
{
    if (!Game::instance())
        return false;
    if (popupManager && popupManager->isPopupMenuVisible())
    {
        popupManager->closePopupMenu();
        return true;
    }
    else if (!quitDialog)
    {
        quitDialog = new QuitDialog(&quitDialog);
        quitDialog->postInit();
        quitDialog->requestMoveToTop();
        return true;
    }
    return false;
}

impHandler0(dropItem0)
{
    if (dropShortcut)
    {
        dropShortcut->dropFirst();
        return true;
    }
    return false;
}

impHandler0(dropItem)
{
    if (dropShortcut)
    {
        dropShortcut->dropItems();
        return true;
    }
    return false;
}

impHandler0(heal)
{
    if (actorManager)
    {
        if (inputManager.isActionActive(InputAction::STOP_ATTACK))
        {
            Being *target = localPlayer->getTarget();
            if (!target || target->getType() != ActorType::PLAYER)
            {
                target = actorManager->findNearestLivingBeing(
                    localPlayer, 10, ActorType::PLAYER, true);
                if (target)
                    localPlayer->setTarget(target);
            }
        }

        actorManager->healTarget();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(itenplz)
{
    if (actorManager)
    {
        if (Net::getPlayerHandler() && Net::getPlayerHandler()->canUseMagic()
            && PlayerInfo::getAttribute(Attributes::MP) >= 3)
        {
            actorManager->itenplz();
        }
        return true;
    }
    return false;
}

impHandler0(setHome)
{
    if (localPlayer)
    {
        localPlayer->setHome();
        return true;
    }
    return false;
}

impHandler0(magicAttack)
{
    if (localPlayer)
    {
        localPlayer->magicAttack();
        return true;
    }
    return false;
}

impHandler0(copyEquippedToOutfit)
{
    if (outfitWindow)
    {
        outfitWindow->copyFromEquiped();
        return true;
    }
    return false;
}

impHandler0(pickup)
{
    if (localPlayer)
    {
        localPlayer->pickUpItems();
        return true;
    }
    return false;
}

static void doSit()
{
    if (inputManager.isActionActive(InputAction::EMOTE))
        localPlayer->updateSit();
    else
        localPlayer->toggleSit();
}

impHandler0(sit)
{
    if (localPlayer)
    {
        doSit();
        return true;
    }
    return false;
}

impHandler0(screenshot)
{
    Game::createScreenshot();
    return true;
}

impHandler0(ignoreInput)
{
    return true;
}

impHandler0(talk)
{
    if (localPlayer)
    {
        Being *target = localPlayer->getTarget();
        if (!target && actorManager)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 1, ActorType::NPC, true);
            // ignore closest target if distance in each direction more than 1
            if (target)
            {
                if (abs(target->getTileX() - localPlayer->getTileX()) > 1
                    || abs(target->getTileY() - localPlayer->getTileY()) > 1)
                {
                    return true;
                }
            }
        }
        if (target)
        {
            if (target->canTalk())
                target->talkTo();
            else if (target->getType() == ActorType::PLAYER)
                new BuySellDialog(target->getName());
        }
        return true;
    }
    return false;
}

impHandler0(stopAttack)
{
    if (localPlayer)
    {
        localPlayer->stopAttack();
        // not consume if target attack key pressed
        if (inputManager.isActionActive(InputAction::TARGET_ATTACK))
            return false;
        return true;
    }
    return false;
}

impHandler0(untarget)
{
    if (localPlayer)
    {
        localPlayer->untarget();
        return true;
    }
    return false;
}

impHandler0(attack)
{
    if (localPlayer)
    {
        Being *const target = localPlayer->getTarget();
        if (target)
            localPlayer->attack(target, true);
        return true;
    }
    return false;
}

impHandler0(targetAttack)
{
    if (localPlayer && actorManager)
    {
        Being *target = nullptr;

        const bool newTarget = !inputManager.isActionActive(
            InputAction::STOP_ATTACK);
        // A set target has highest priority
        if (!localPlayer->getTarget())
        {
            // Only auto target Monsters
            target = actorManager->findNearestLivingBeing(
                localPlayer, 90, ActorType::MONSTER, true);
        }
        else
        {
            target = localPlayer->getTarget();
        }

        localPlayer->attack2(target, newTarget);
        return true;
    }
    return false;
}

impHandler0(safeVideoMode)
{
    if (mainGraphics)
        mainGraphics->setFullscreen(false);

    return true;
}

impHandler0(stopSit)
{
    if (localPlayer)
    {
        localPlayer->stopAttack();
        // not consume if target attack key pressed
        if (inputManager.isActionActive(InputAction::TARGET_ATTACK))
            return false;
        if (!localPlayer->getTarget())
        {
            doSit();
            return true;
        }
        return true;
    }
    return false;
}

impHandler0(showKeyboard)
{
#ifdef ANDROID
#ifdef USE_SDL2
    if (SDL_IsTextInputActive())
        SDL_StopTextInput();
    else
        SDL_StartTextInput();
#else
    SDL_ANDROID_ToggleScreenKeyboardTextInput(nullptr);
#endif
    return true;
#else
    return false;
#endif
}

impHandler0(showWindows)
{
    if (popupMenu)
    {
        popupMenu->showWindowsPopup(viewport->mMouseX,
            viewport->mMouseY);
        return true;
    }
    return false;
}

impHandler0(openTrade)
{
    const Being *const being = localPlayer->getTarget();
    if (being && being->getType() == ActorType::PLAYER)
    {
        Net::getTradeHandler()->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow)
            tradeWindow->clear();
        return true;
    }
    return false;
}

impHandler0(ipcToggle)
{
    if (ipc)
    {
        IPC::stop();
        if (!ipc)
            debugChatTab->chatLog("IPC service stopped.");
        else
            debugChatTab->chatLog("Unable to stop IPC service.");
    }
    else
    {
        IPC::start();
        if (ipc)
        {
            debugChatTab->chatLog(strprintf("IPC service available on port %d",
                ipc->getPort()));
        }
        else
        {
            debugChatTab->chatLog("Unable to start IPC service");
        }
    }
    return true;
}

}  // namespace Actions
