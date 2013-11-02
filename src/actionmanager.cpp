/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "actionmanager.h"

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "input/inputevent.h"
#include "input/inputmanager.h"

#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
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
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setup.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "render/graphics.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/gettext.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include "debug.h"

#define impHandler(name) bool name(const InputEvent &event)
#define impHandler0(name) bool name(const InputEvent &event A_UNUSED)

extern ShortcutWindow *spellShortcutWindow;
extern QuitDialog *quitDialog;

namespace ActionManager
{

impHandler(moveUp)
{
    if (NpcDialog *const dialog = NpcDialog::getActive())
    {
        dialog->refocus();
        return false;
    }
    if (inputManager.isActionActive(Input::KEY_EMOTE))
        return directUp(event);
    return false;
}

impHandler(moveDown)
{
    if (NpcDialog *const dialog = NpcDialog::getActive())
    {
        dialog->refocus();
        return false;
    }
    if (inputManager.isActionActive(Input::KEY_EMOTE))
        return directDown(event);
    return false;
}

impHandler(moveLeft)
{
    if (outfitWindow && inputManager.isActionActive(Input::KEY_WEAR_OUTFIT))
    {
        outfitWindow->wearPreviousOutfit();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    if (inputManager.isActionActive(Input::KEY_EMOTE))
        return directLeft(event);
    return false;
}

impHandler(moveRight)
{
    if (outfitWindow && inputManager.isActionActive(Input::KEY_WEAR_OUTFIT))
    {
        outfitWindow->wearNextOutfit();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    if (inputManager.isActionActive(Input::KEY_EMOTE))
        return directRight(event);
    return false;
}

impHandler(moveForward)
{
    if (inputManager.isActionActive(Input::KEY_EMOTE))
        return directRight(event);
    return false;
}

impHandler(emote)
{
    const int emotion = 1 + event.action - Input::KEY_EMOTE_1;
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

impHandler(moveToPoint)
{
    const int num = event.action - Input::KEY_MOVE_TO_POINT_1;
    if (socialWindow && num >= 0)
    {
        socialWindow->selectPortal(num);
        return true;
    }

    return false;
}

impHandler(outfit)
{
    if (inputManager.isActionActive(Input::KEY_WEAR_OUTFIT))
    {
        const int num = event.action - Input::KEY_OUTFIT_1;
        if (outfitWindow && num >= 0)
        {
            outfitWindow->wearOutfit(num);
            if (Game::instance())
                Game::instance()->setValidSpeed();
            return true;
        }
    }
    else if (inputManager.isActionActive(Input::KEY_COPY_OUTFIT))
    {
        const int num = event.action - Input::KEY_OUTFIT_1;
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
    if (!guiInput)
        return false;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    guiInput->simulateMouseClick(mouseX, mouseY, gcn::MouseEvent::RIGHT);
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
        setupWindow->action(gcn::ActionEvent(nullptr, "cancel"));
        return true;
    }
    else if (NpcDialog *const dialog = NpcDialog::getActive())
    {
        dialog->action(gcn::ActionEvent(nullptr, "ok"));
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
                    - Input::KEY_SHORTCUT_1);
            }
        }
        return true;
    }
    return false;
}

impHandler0(toggleChat)
{
    return chatWindow ? chatWindow->requestChatFocus() : false;
}

impHandler0(prevChatTab)
{
    if (chatWindow)
    {
        chatWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextChatTab)
{
    if (chatWindow)
    {
        chatWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(closeChatTab)
{
    if (chatWindow)
    {
        chatWindow->closeTab();
        return true;
    }
    return false;
}

impHandler0(prevSocialTab)
{
    if (socialWindow)
    {
        socialWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextSocialTab)
{
    if (socialWindow)
    {
        socialWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(scrollChatUp)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler0(scrollChatDown)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler0(quit)
{
    if (!Game::instance())
        return false;
    if (viewport && viewport->isPopupMenuVisible())
    {
        viewport->closePopupMenu();
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

impHandler0(switchQuickDrop)
{
    if (player_node && !player_node->getDisableGameModifiers())
    {
        player_node->changeQuickDropCounter();
        return true;
    }
    return false;
}

impHandler0(heal)
{
    if (actorManager)
    {
        if (inputManager.isActionActive(Input::KEY_MOD))
        {
            Being *target = player_node->getTarget();
            if (!target || target->getType() != ActorSprite::PLAYER)
            {
                target = actorManager->findNearestLivingBeing(
                    player_node, 10, ActorSprite::PLAYER);
                if (target)
                    player_node->setTarget(target);
            }
        }

        actorManager->healTarget();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(crazyMoves)
{
    if (player_node)
    {
        player_node->crazyMove();
        return true;
    }
    return false;
}

impHandler0(itenplz)
{
    if (actorManager)
    {
        if (Net::getPlayerHandler() && Net::getPlayerHandler()->canUseMagic()
            && PlayerInfo::getAttribute(PlayerInfo::MP) >= 3)
        {
            actorManager->itenplz();
        }
        return true;
    }
    return false;
}

impHandler0(changeCrazyMove)
{
    if (player_node)
    {
        player_node->changeCrazyMoveType();
        return true;
    }
    return false;
}

impHandler0(changePickupType)
{
    if (player_node)
    {
        player_node->changePickUpType();
        return true;
    }
    return false;
}

impHandler0(moveToTarget)
{
    if (player_node && !inputManager.isActionActive(Input::KEY_TARGET_ATTACK)
        && !inputManager.isActionActive(Input::KEY_ATTACK))
    {
        player_node->moveToTarget();
        return true;
    }
    return false;
}

impHandler0(moveToHome)
{
    if (player_node && !inputManager.isActionActive(Input::KEY_TARGET_ATTACK)
        && !inputManager.isActionActive(Input::KEY_ATTACK))
    {
        player_node->moveToHome();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(setHome)
{
    if (player_node)
    {
        player_node->setHome();
        return true;
    }
    return false;
}

impHandler0(changeMoveType)
{
    if (player_node)
    {
        player_node->invertDirection();
        return true;
    }
    return false;
}

impHandler0(changeAttackWeaponType)
{
    if (player_node)
    {
        player_node->changeAttackWeaponType();
        return true;
    }
    return false;
}

impHandler0(changeAttackType)
{
    if (player_node)
    {
        player_node->changeAttackType();
        return true;
    }
    return false;
}

impHandler0(changeFollowMode)
{
    if (player_node)
    {
        player_node->changeFollowMode();
        return true;
    }
    return false;
}

impHandler0(changeImitationMode)
{
    if (player_node)
    {
        player_node->changeImitationMode();
        return true;
    }
    return false;
}

impHandler0(magicAttack)
{
    if (player_node)
    {
        player_node->magicAttack();
        return true;
    }
    return false;
}

impHandler0(changeMagicAttack)
{
    if (player_node)
    {
        player_node->switchMagicAttack();
        return true;
    }
    return false;
}

impHandler0(changePvpMode)
{
    if (player_node)
    {
        player_node->switchPvpAttack();
        return true;
    }
    return false;
}

impHandler0(changeMoveToTarget)
{
    if (player_node)
    {
        player_node->changeMoveToTargetType();
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

impHandler0(changeGameModifier)
{
    if (player_node)
    {
        player_node->switchGameModifiers();
        return true;
    }
    return false;
}

impHandler0(changeAudio)
{
    soundManager.changeAudio();
    return true;
}

impHandler0(away)
{
    if (player_node)
    {
        player_node->changeAwayMode();
        player_node->updateStatus();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(camera)
{
    if (viewport)
    {
        viewport->toggleCameraMode();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(setupWindowShow)
{
    if (setupWindow)
    {
        if (setupWindow->isWindowVisible())
        {
            setupWindow->doCancel();
        }
        else
        {
            setupWindow->setVisible(true);
            setupWindow->requestMoveToTop();
        }
        return true;
    }
    return false;
}

impHandler0(pickup)
{
    if (player_node)
    {
        player_node->pickUpItems();
        return true;
    }
    return false;
}

impHandler0(sit)
{
    if (player_node)
    {
        if (inputManager.isActionActive(Input::KEY_EMOTE))
            player_node->updateSit();
        else
            player_node->toggleSit();
        return true;
    }
    return false;
}

impHandler0(hideWindows)
{
    if (statusWindow)
        statusWindow->setVisible(false);
    if (inventoryWindow)
        inventoryWindow->setVisible(false);
    if (shopWindow)
        shopWindow->setVisible(false);
    if (skillDialog)
        skillDialog->setVisible(false);
    if (setupWindow)
        setupWindow->setVisible(false);
    if (equipmentWindow)
        equipmentWindow->setVisible(false);
    if (helpWindow)
        helpWindow->setVisible(false);
    if (debugWindow)
        debugWindow->setVisible(false);
    if (outfitWindow)
        outfitWindow->setVisible(false);
    if (dropShortcutWindow)
        dropShortcutWindow->setVisible(false);
    if (spellShortcutWindow)
        spellShortcutWindow->setVisible(false);
    if (botCheckerWindow)
        botCheckerWindow->setVisible(false);
    if (socialWindow)
        socialWindow->setVisible(false);
    if (questsWindow)
        questsWindow->setVisible(false);
    return true;
}

impHandler0(helpWindowShow)
{
    if (helpWindow)
    {
        if (helpWindow->isWindowVisible())
        {
            helpWindow->setVisible(false);
        }
        else
        {
            helpWindow->loadHelp("index");
            helpWindow->requestMoveToTop();
        }
        return true;
    }
    return false;
}

static void showHideWindow(Window *const window)
{
    if (window)
    {
        window->setVisible(!window->isWindowVisible());
        if (window->isWindowVisible())
            window->requestMoveToTop();
    }
}

impHandler0(statusWindowShow)
{
    showHideWindow(statusWindow);
    return true;
}

impHandler0(inventoryWindowShow)
{
    showHideWindow(inventoryWindow);
    return true;
}

impHandler0(equipmentWindowShow)
{
    showHideWindow(equipmentWindow);
    return true;
}

impHandler0(skillDialogShow)
{
    showHideWindow(skillDialog);
    return true;
}

impHandler0(minimapWindowShow)
{
    if (minimap)
    {
        minimap->toggle();
        return true;
    }
    return false;
}

impHandler0(chatWindowShow)
{
    showHideWindow(chatWindow);
    return true;
}

impHandler0(shortcutWindowShow)
{
    showHideWindow(itemShortcutWindow);
    return true;
}

impHandler0(debugWindowShow)
{
    showHideWindow(debugWindow);
    return true;
}

impHandler0(socialWindowShow)
{
    showHideWindow(socialWindow);
    return true;
}

impHandler0(emoteShortcutWindowShow)
{
    showHideWindow(emoteShortcutWindow);
    return true;
}

impHandler0(outfitWindowShow)
{
    showHideWindow(outfitWindow);
    return true;
}

impHandler0(shopWindowShow)
{
    showHideWindow(shopWindow);
    return true;
}

impHandler0(dropShortcutWindowShow)
{
    showHideWindow(dropShortcutWindow);
    return true;
}

impHandler0(killStatsWindowShow)
{
    showHideWindow(killStats);
    return true;
}

impHandler0(spellShortcutWindowShow)
{
    showHideWindow(spellShortcutWindow);
    return true;
}

impHandler0(botcheckerWindowShow)
{
    showHideWindow(botCheckerWindow);
    return true;
}

impHandler0(whoIsOnlineWindowShow)
{
    showHideWindow(whoIsOnline);
    return true;
}

impHandler0(didYouKnowWindowShow)
{
    showHideWindow(didYouKnowWindow);
    return true;
}

impHandler0(questsWindowShow)
{
    showHideWindow(questsWindow);
    return true;
}

impHandler0(changeMapMode)
{
    if (viewport)
        viewport->toggleDebugPath();
    if (miniStatusWindow)
        miniStatusWindow->updateStatus();
    if (Game::instance())
    {
        if (Map *const map = Game::instance()->getCurrentMap())
            map->redrawMap();
    }
    return true;
}

impHandler0(changeTrade)
{
    unsigned int deflt = player_relations.getDefault();
    if (deflt & PlayerRelation::TRADE)
    {
        if (localChatTab)
        {
            // TRANSLATORS: disable trades message
            localChatTab->chatLog(_("Ignoring incoming trade requests"),
                BY_SERVER);
        }
        deflt &= ~PlayerRelation::TRADE;
    }
    else
    {
        if (localChatTab)
        {
            // TRANSLATORS: enable trades message
            localChatTab->chatLog(_("Accepting incoming trade requests"),
                BY_SERVER);
        }
        deflt |= PlayerRelation::TRADE;
    }

    player_relations.setDefault(deflt);
    return true;
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

impHandler0(directUp)
{
    if (player_node)
    {
        if (player_node->getDirection() != Being::UP)
        {
//            if (client->limitPackets(PACKET_DIRECTION))
            {
                player_node->setDirection(Being::UP);
                if (Net::getPlayerHandler())
                    Net::getPlayerHandler()->setDirection(Being::UP);
            }
        }
        return true;
    }
    return false;
}

impHandler0(directDown)
{
    if (player_node)
    {
        if (player_node->getDirection() != Being::DOWN)
        {
//            if (client->limitPackets(PACKET_DIRECTION))
            {
                player_node->setDirection(Being::DOWN);
                if (Net::getPlayerHandler())
                    Net::getPlayerHandler()->setDirection(Being::DOWN);
            }
        }
        return true;
    }
    return false;
}

impHandler0(directLeft)
{
    if (player_node)
    {
        if (player_node->getDirection() != Being::LEFT)
        {
//            if (client->limitPackets(PACKET_DIRECTION))
            {
                player_node->setDirection(Being::LEFT);
                if (Net::getPlayerHandler())
                    Net::getPlayerHandler()->setDirection(Being::LEFT);
            }
        }
        return true;
    }
    return false;
}

impHandler0(directRight)
{
    if (player_node)
    {
        if (player_node->getDirection() != Being::RIGHT)
        {
//            if (client->limitPackets(PACKET_DIRECTION))
            {
                player_node->setDirection(Being::RIGHT);
                if (Net::getPlayerHandler())
                    Net::getPlayerHandler()->setDirection(Being::RIGHT);
            }
        }
        return true;
    }
    return false;
}

impHandler0(talk)
{
    if (player_node)
    {
        Being *target = player_node->getTarget();
        if (!target && actorManager)
        {
            target = actorManager->findNearestLivingBeing(
                player_node, 1, ActorSprite::NPC);
            // ignore closest target if distance in each direction more than 1
            if (target)
            {
                if (abs(target->getTileX() - player_node->getTileX()) > 1
                    || abs(target->getTileY() - player_node->getTileY()) > 1)
                {
                    return true;
                }
            }
        }
        if (target)
        {
            if (target->canTalk())
                target->talkTo();
            else if (target->getType() == Being::PLAYER)
                new BuySellDialog(target->getName());
        }
        return true;
    }
    return false;
}

impHandler0(stopAttack)
{
    if (player_node)
    {
        player_node->stopAttack();
        // not consume if target attack key pressed
        if (inputManager.isActionActive(Input::KEY_TARGET_ATTACK))
            return false;
        return true;
    }
    return false;
}

impHandler0(untarget)
{
    if (player_node)
    {
        player_node->untarget();
        return true;
    }
    return false;
}

impHandler0(attack)
{
    if (player_node)
    {
        Being *const target = player_node->getTarget();
        if (target)
            player_node->attack(target, true);
        return true;
    }
    return false;
}

impHandler0(targetAttack)
{
    if (player_node && actorManager)
    {
        Being *target = nullptr;

        const bool newTarget = !inputManager.isActionActive(
            Input::KEY_STOP_ATTACK);
        // A set target has highest priority
        if (!player_node->getTarget())
        {
            // Only auto target Monsters
            target = actorManager->findNearestLivingBeing(
                player_node, 90, ActorSprite::MONSTER);
        }
        else
        {
            target = player_node->getTarget();
        }

        player_node->attack2(target, newTarget);
        return true;
    }
    return false;
}

static bool setTarget(const ActorSprite::Type type)
{
    if (actorManager && player_node)
    {
        Being *const target = actorManager->findNearestLivingBeing(
            player_node, 20, type);

        if (target && target != player_node->getTarget())
            player_node->setTarget(target);

        return true;
    }
    return false;
}

impHandler0(targetPlayer)
{
    return setTarget(ActorSprite::PLAYER);
}

impHandler0(targetMonster)
{
    return setTarget(ActorSprite::MONSTER);
}

impHandler0(targetNPC)
{
    return setTarget(ActorSprite::NPC);
}

impHandler0(safeVideoMode)
{
    if (mainGraphics)
        mainGraphics->setFullscreen(false);

    return true;
}

impHandler0(stopSit)
{
    if (player_node)
    {
        player_node->stopAttack();
        // not consume if target attack key pressed
        if (inputManager.isActionActive(Input::KEY_TARGET_ATTACK))
            return false;
        if (!player_node->getTarget())
        {
            if (inputManager.isActionActive(Input::KEY_EMOTE))
                player_node->updateSit();
            else
                player_node->toggleSit();
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
    if (viewport)
    {
        viewport->showWindowsPopup();
        return true;
    }
    return false;
}

}  // namespace ActionManager
