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

#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/gamehandler.h"
#include "net/ipc.h"
#include "net/net.h"
#include "net/playerhandler.h"
#include "net/tradehandler.h"

#include "listeners/updatestatuslistener.h"

#include "resources/resourcemanager.h"

#include "resources/map/map.h"

#include "utils/gettext.h"
#include "utils/timer.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;
extern std::string tradePartnerName;
extern QuitDialog *quitDialog;
extern int start_time;

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
        if (!event.args.empty())
        {
            const Being *const being = actorManager->findBeingByName(
                event.args, ActorType::PLAYER);
            if (being)
                actorManager->heal(being);
        }
        else if (inputManager.isActionActive(InputAction::STOP_ATTACK))
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
        else
        {
            actorManager->heal(localPlayer);
        }

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
    if (!localPlayer || !actorManager)
        return false;

    Being *target = nullptr;
    if (!event.args.empty())
        target = actorManager->findNearestByName(event.args);
    if (!target)
        target = localPlayer->getTarget();
    else
        localPlayer->setTarget(target);
    if (target)
        localPlayer->attack(target, true);
    return true;
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

impHandler0(attackHuman)
{
    if (!actorManager || !localPlayer)
        return false;

    Being *const target = actorManager->findNearestLivingBeing(
        localPlayer, 10, ActorType::PLAYER, true);
    if (target)
    {
        if (localPlayer->checAttackPermissions(target))
        {
            localPlayer->setTarget(target);
            localPlayer->attack2(target, true);
        }
    }
    return true;
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

impHandler(where)
{
    ChatTab *const tab = event.tab != nullptr ? event.tab : debugChatTab;
    if (!tab)
        return false;
    std::ostringstream where;
    where << Game::instance()->getCurrentMapName() << ", coordinates: "
        << ((localPlayer->getPixelX() - mapTileSize / 2) / mapTileSize)
        << ", " << ((localPlayer->getPixelY() - mapTileSize) / mapTileSize);
    tab->chatLog(where.str(), ChatMsgType::BY_SERVER);
    return true;
}

impHandler0(who)
{
    Net::getChatHandler()->who();
    return true;
}

impHandler0(cleanGraphics)
{
    ResourceManager::getInstance()->clearCache();

    if (debugChatTab)
    {
        // TRANSLATORS: clear graphics command message
        debugChatTab->chatLog(_("Cache cleaned"));
    }
    return true;
}

impHandler0(cleanFonts)
{
    if (gui)
        gui->clearFonts();
    if (debugChatTab)
    {
        // TRANSLATORS: clear fonts cache message
        debugChatTab->chatLog(_("Cache cleaned"));
    }
    return true;
}

impHandler(trade)
{
    if (!actorManager)
        return false;

    const Being *being = actorManager->findBeingByName(
        event.args, ActorType::PLAYER);
    if (!being)
        being = localPlayer->getTarget();
    if (being)
    {
        Net::getTradeHandler()->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow)
            tradeWindow->clear();
    }
    return true;
}

impHandler0(priceLoad)
{
    if (shopWindow)
    {
        shopWindow->loadList();
        return true;
    }
    return false;
}

impHandler0(priceSave)
{
    if (shopWindow)
    {
        shopWindow->saveList();
        return true;
    }
    return false;
}

impHandler0(cacheInfo)
{
    if (!chatWindow || !debugChatTab)
        return false;

/*
    Font *const font = chatWindow->getFont();
    if (!font)
        return;

    const TextChunkList *const cache = font->getCache();
    if (!cache)
        return;

    unsigned int all = 0;
    debugChatTab->chatLog(_("font cache size"));
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].size)
        {
            const unsigned int sz = static_cast<int>(cache[f].size);
            all += sz;
            str.append(strprintf("%d: %u, ", f, sz));
        }
    }
    debugChatTab->chatLog(str);
    debugChatTab->chatLog(strprintf("%s %d", _("Cache size:"), all));
#ifdef DEBUG_FONT_COUNTERS
    debugChatTab->chatLog("");
    debugChatTab->chatLog(strprintf("%s %d",
        _("Created:"), font->getCreateCounter()));
    debugChatTab->chatLog(strprintf("%s %d",
        _("Deleted:"), font->getDeleteCounter()));
#endif
*/
    return true;
}

impHandler0(disconnect)
{
    Net::getGameHandler()->disconnect2();
    return true;
}

impHandler(undress)
{
    if (!actorManager || !localPlayer)
        return false;

    Being *target = nullptr;
    if (!event.args.empty())
        target = actorManager->findNearestByName(event.args);
    if (!target)
        target = localPlayer->getTarget();
    if (target)
        Net::getBeingHandler()->undress(target);
    return true;
}

impHandler0(dirs)
{
    if (!debugChatTab)
        return false;

    debugChatTab->chatLog("config directory: "
        + settings.configDir);
    debugChatTab->chatLog("logs directory: "
        + settings.localDataDir);
    debugChatTab->chatLog("screenshots directory: "
        + settings.screenshotDir);
    debugChatTab->chatLog("temp directory: "
        + settings.tempDir);
    return true;
}

impHandler0(uptime)
{
    if (!debugChatTab)
        return false;

    if (cur_time < start_time)
    {
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), "unknown"));
    }
    else
    {
        std::string str;
        int timeDiff = cur_time - start_time;

        const int weeks = timeDiff / 60 / 60 / 24 / 7;
        if (weeks > 0)
        {
            // TRANSLATORS: uptime command
            str = strprintf(ngettext(N_("%d week"), N_("%d weeks"),
                weeks), weeks);
            timeDiff -= weeks * 60 * 60 * 24 * 7;
        }

        const int days = timeDiff / 60 / 60 / 24;
        if (days > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d day"), N_("%d days"),
                days), days));
            timeDiff -= days * 60 * 60 * 24;
        }
        const int hours = timeDiff / 60 / 60;
        if (hours > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d hour"), N_("%d hours"),
                hours), hours));
            timeDiff -= hours * 60 * 60;
        }
        const int min = timeDiff / 60;
        if (min > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d minute"), N_("%d minutes"),
                min), min));
            timeDiff -= min * 60;
        }

        if (timeDiff > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d second"), N_("%d seconds"),
                timeDiff), timeDiff));
        }
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), str.c_str()));
    }
    return true;
}

#ifdef DEBUG_DUMP_LEAKS1
static void showRes(std::string str, ResourceManager::Resources *res)
{
    if (!res)
        return;

    str.append(toString(res->size()));
    if (debugChatTab)
        debugChatTab->chatLog(str);
    logger->log(str);
    ResourceManager::ResourceIterator iter = res->begin();
    const ResourceManager::ResourceIterator iter_end = res->end();
    while (iter != iter_end)
    {
        if (iter->second && iter->second->getRefCount())
        {
            char type = ' ';
            char isNew = 'N';
            if (iter->second->getDumped())
                isNew = 'O';
            else
                iter->second->setDumped(true);

            SubImage *const subImage = dynamic_cast<SubImage *const>(
                iter->second);
            Image *const image = dynamic_cast<Image *const>(iter->second);
            int id = 0;
            if (subImage)
                type = 'S';
            else if (image)
                type = 'I';
            if (image)
                id = image->getGLImage();
            logger->log("Resource %c%c: %s (%d) id=%d", type,
                isNew, iter->second->getIdPath().c_str(),
                iter->second->getRefCount(), id);
        }
        ++ iter;
    }
}

impHandler(dump)
{
    if (!debugChatTab)
        return false;

    ResourceManager *const resman = ResourceManager::getInstance();

    if (!event.args.empty())
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        showRes(_("Resource images:"), res);
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        showRes(_("Resource orphaned images:"), res);
    }
    else
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource images:") + toString(res->size()));
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource orphaned images:")
            + toString(res->size()));
    }
    return true;
}

#elif defined ENABLE_MEM_DEBUG
impHandler0(dump)
{
    check_leaks();
    return true;
}
#else
impHandler0(dump)
{
    return true;
}
#endif

impHandler0(serverIgnoreAll)
{
    Net::getChatHandler()->ignoreAll();
    return true;
}

impHandler0(serverUnIgnoreAll)
{
    Net::getChatHandler()->unIgnoreAll();
    return true;
}

impHandler0(error)
{
    const int *const ptr = nullptr;
    logger->log("test %d", *ptr);
    exit(1);
}

}  // namespace Actions
