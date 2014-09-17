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
#include "configuration.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "graphicsmanager.h"
#include "itemshortcut.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/dialogtype.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "render/graphics.h"
#if defined USE_OPENGL
#include "render/normalopenglgraphics.h"
#endif

#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/download.h"
#include "net/gamehandler.h"
#include "net/ipc.h"
#include "net/net.h"
#include "net/pethandler.h"
#include "net/playerhandler.h"
#include "net/uploadcharinfo.h"
#include "net/tradehandler.h"

#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "resources/db/itemdb.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/timer.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include "debug.h"

extern std::string tradePartnerName;
extern QuitDialog *quitDialog;
extern int start_time;
extern char **environ;

namespace Actions
{

static int uploadUpdate(void *ptr,
                        DownloadStatus::Type status,
                        size_t total A_UNUSED,
                        size_t remaining A_UNUSED)
{
    if (status == DownloadStatus::Idle || status == DownloadStatus::Starting)
        return 0;

    UploadChatInfo *const info = reinterpret_cast<UploadChatInfo*>(ptr);
    if (status == DownloadStatus::Complete)
    {
        std::string str = Net::Download::getUploadResponse();
        const size_t sz = str.size();
        if (sz > 0)
        {
            if (str[sz - 1] == '\n')
                str = str.substr(0, sz - 1);
            str.append(info->addStr);
            ChatTab *const tab = info->tab;
            if (chatWindow && (!tab || chatWindow->isTabPresent(tab)))
            {
                str = strprintf("%s [@@%s |%s@@]",
                    info->text.c_str(), str.c_str(), str.c_str());
                outStringNormal(tab, str, str);
            }
            else
            {
                // TRANSLATORS: file uploaded message
                new OkDialog(_("File uploaded"), str,
                    // TRANSLATORS: ok dialog button
                    _("OK"),
                    DialogType::OK,
                    true, false, nullptr, 260);
            }
        }
    }
    delete2(info->upload);
    delete info;
    return 0;
}

static void uploadFile(const std::string &str,
                       const std::string &fileName,
                       const std::string &addStr,
                       ChatTab *const tab)
{
    UploadChatInfo *const info = new UploadChatInfo();
    Net::Download *const upload = new Net::Download(info,
        "http://sprunge.us",
        &uploadUpdate,
        false, true, false);
    info->upload = upload;
    info->text = str;
    info->addStr = addStr;
    info->tab = tab;
    upload->setFile(fileName);
    upload->start();
}

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

impHandler(petEmote)
{
    if (event.action >= InputAction::PET_EMOTE_1
        && event.action <= InputAction::PET_EMOTE_48)
    {
        const int emotion = event.action - InputAction::PET_EMOTE_1;
        if (emoteShortcut)
            petHandler->emote(emoteShortcut->getEmote(emotion), 0);
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

impHandler(heal)
{
    if (actorManager && localPlayer)
    {
        if (!event.args.empty())
        {
            const Being *const being = actorManager->findBeingByName(
                event.args, ActorType::Player);
            if (being)
                actorManager->heal(being);
        }
        else
        {
            Being *target = localPlayer->getTarget();
            if (inputManager.isActionActive(InputAction::STOP_ATTACK))
            {
                if (!target || target->getType() != ActorType::Player)
                {
                    target = actorManager->findNearestLivingBeing(
                        localPlayer, 10, ActorType::Player, true);
                }
            }
            else
            {
                if (!target)
                    target = localPlayer;
            }
            actorManager->heal(target);
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
        if (playerHandler && playerHandler->canUseMagic()
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
                localPlayer, 1, ActorType::Npc, true);
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
            else if (target->getType() == ActorType::Player)
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

impHandler(attack)
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
                localPlayer, 90, ActorType::Monster, true);
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
        localPlayer, 10, ActorType::Player, true);
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
        popupMenu->showWindowsPopup();
        return true;
    }
    return false;
}

impHandler0(openTrade)
{
    const Being *const being = localPlayer->getTarget();
    if (being && being->getType() == ActorType::Player)
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
    chatHandler->who();
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
        event.args, ActorType::Player);
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
    gameHandler->disconnect2();
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
        beingHandler->undress(target);
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
    chatHandler->ignoreAll();
    return true;
}

impHandler0(serverUnIgnoreAll)
{
    chatHandler->unIgnoreAll();
    return true;
}

impHandler0(error)
{
    const int *const ptr = nullptr;
    logger->log("test %d", *ptr);
    exit(1);
}

impHandler(dumpGraphics)
{
    std::string str = strprintf("%s,%s,%dX%dX%d,", PACKAGE_OS, SMALL_VERSION,
        mainGraphics->getWidth(), mainGraphics->getHeight(),
        mainGraphics->getBpp());

    if (mainGraphics->getFullScreen())
        str.append("F");
    else
        str.append("W");
    if (mainGraphics->getHWAccel())
        str.append("H");
    else
        str.append("S");

    if (mainGraphics->getDoubleBuffer())
        str.append("D");
    else
        str.append("_");

#if defined USE_OPENGL
    str.append(strprintf(",%d", mainGraphics->getOpenGL()));
#else
    str.append(",0");
#endif

    str.append(strprintf(",%f,", static_cast<double>(settings.guiAlpha)))
        .append(config.getBoolValue("adjustPerfomance") ? "1" : "0")
        .append(config.getBoolValue("alphaCache") ? "1" : "0")
        .append(config.getBoolValue("enableMapReduce") ? "1" : "0")
        .append(config.getBoolValue("beingopacity") ? "1" : "0")
        .append(",")
        .append(config.getBoolValue("enableAlphaFix") ? "1" : "0")
        .append(config.getBoolValue("disableAdvBeingCaching") ? "1" : "0")
        .append(config.getBoolValue("disableBeingCaching") ? "1" : "0")
        .append(config.getBoolValue("particleeffects") ? "1" : "0")
        .append(strprintf(",%d-%d", fps, config.getIntValue("fpslimit")));
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler0(dumpEnvironment)
{
    logger->log1("Start environment variables");
    for (char **env = environ; *env; ++ env)
        logger->log1(*env);
    logger->log1("End environment variables");
    if (debugChatTab)
    {
        // TRANSLATORS: dump environment command
        debugChatTab->chatLog(_("Environment variables dumped"));
    }
    return true;
}

impHandler(dumpTests)
{
    const std::string str = config.getStringValue("testInfo");
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler0(dumpOGL)
{
#if defined USE_OPENGL && !defined ANDROID
    NormalOpenGLGraphics::dumpSettings();
#endif
    return true;
}

#ifdef USE_OPENGL
impHandler(dumpGL)
{
    std::string str = graphicsManager.getGLVersion();
    outStringNormal(event.tab, str, str);
    return true;
}
#else
impHandler0(dumpGL)
{
    return true;
}
#endif

impHandler(dumpMods)
{
    std::string str = "enabled mods: " + serverConfig.getValue("mods", "");
    outStringNormal(event.tab, str, str);
    return true;
}

#if defined USE_OPENGL && defined DEBUG_SDLFONT
impHandler0(testSdlFont)
{
    Font *font = new Font("fonts/dejavusans.ttf", 18);
    timespec time1;
    timespec time2;
    NullOpenGLGraphics *nullGraphics = new NullOpenGLGraphics;
    std::vector<std::string> data;
    volatile int width = 0;

    for (int f = 0; f < 300; f ++)
        data.push_back("test " + toString(f) + "string");
    nullGraphics->beginDraw();

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for (int f = 0; f < 500; f ++)
    {
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
        {
            width += font->getWidth(*it);
            font->drawString(nullGraphics, *it, 10, 10);
        }
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
            font->drawString(nullGraphics, *it, 10, 10);

        font->doClean();
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);

    delete nullGraphics;
    delete font;

    int64_t diff = (static_cast<long long int>(
        time2.tv_sec) * 1000000000LL + static_cast<long long int>(
        time2.tv_nsec)) / 100000 - (static_cast<long long int>(
        time1.tv_sec) * 1000000000LL + static_cast<long long int>(
        time1.tv_nsec)) / 100000;
    if (debugChatTab)
        debugChatTab->chatLog("sdlfont time: " + toString(diff));
    return true;
}
#endif

impHandler0(createItems)
{
    BuyDialog *const dialog = new BuyDialog();
    const ItemDB::ItemInfos &items = ItemDB::getItemInfos();
    FOR_EACH (ItemDB::ItemInfos::const_iterator, it, items)
    {
        const ItemInfo *const info = (*it).second;
        const int id = info->getId();
        if (id <= 500)
            continue;

        int colors = info->getColorsSize();
        if (colors >= 255)
            colors = 254;

        if (!colors)
        {
            dialog->addItem(id, 1, 100, 0);
        }
        else
        {
            for (unsigned char f = 0; f < colors; f ++)
            {
                if (!info->getColor(f).empty())
                    dialog->addItem(id, f, 100, 0);
            }
        }
    }
    dialog->sort();
    return true;
}

impHandler(uploadConfig)
{
    uploadFile(_("Uploaded config into:"),
        config.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadServerConfig)
{
    uploadFile(_("Uploaded server config into:"),
        serverConfig.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadLog)
{
    uploadFile(_("Uploaded log into:"),
        settings.logFileName,
        "?txt",
        event.tab);
    return true;
}

impHandler(catchPet)
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
        petHandler->catchPet(target);
    return true;
}

}  // namespace Actions
