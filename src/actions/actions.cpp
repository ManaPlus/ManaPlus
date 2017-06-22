/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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
#include "game.h"
#ifdef USE_OPENGL
#include "graphicsmanager.h"
#endif  // USE_OPENGL
#include "main.h"
#include "spellmanager.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/spells.h"

#include "const/resources/skill.h"

#include "fs/files.h"

#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/windowmanager.h"

#include "gui/shortcut/dropshortcut.h"
#include "gui/shortcut/emoteshortcut.h"
#include "gui/shortcut/itemshortcut.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/whoisonline.h"

#include "gui/widgets/createwidget.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "input/inputactionoperators.h"

#if defined USE_OPENGL
#include "render/normalopenglgraphics.h"
#endif  // USE_OPENGL

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/buyingstorehandler.h"
#include "net/buysellhandler.h"
#include "net/chathandler.h"
#include "net/download.h"
#include "net/homunculushandler.h"
#include "net/gamehandler.h"
#include "net/inventoryhandler.h"
#include "net/ipc.h"
#include "net/mercenaryhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/playerhandler.h"
#include "net/pethandler.h"
#include "net/serverfeatures.h"
#include "net/uploadcharinfo.h"
#include "net/tradehandler.h"
#include "net/vendinghandler.h"

#include "resources/iteminfo.h"
#include "resources/memorymanager.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/mathutils.h"
#include "utils/parameters.h"
#include "utils/timer.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif  // USE_OPENGL
#endif  // ANDROID
PRAGMA48(GCC diagnostic pop)

#include <sstream>

#include "debug.h"

extern std::string tradePartnerName;
extern QuitDialog *quitDialog;
extern time_t start_time;
extern char **environ;

namespace Actions
{

static int uploadUpdate(void *ptr,
                        const DownloadStatusT status,
                        size_t total A_UNUSED,
                        const size_t remaining A_UNUSED) A_NONNULL(1);

static int uploadUpdate(void *ptr,
                        const DownloadStatusT status,
                        size_t total A_UNUSED,
                        const size_t remaining A_UNUSED)
{
    if (status == DownloadStatus::Idle || status == DownloadStatus::Starting)
        return 0;

    UploadChatInfo *const info = reinterpret_cast<UploadChatInfo*>(ptr);
    if (info == nullptr)
        return 0;

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
            if (chatWindow != nullptr &&
                (tab == nullptr || chatWindow->isTabPresent(tab)))
            {
                str = strprintf("%s [@@%s |%s@@]",
                    info->text.c_str(), str.c_str(), str.c_str());
                outStringNormal(tab, str, str);
            }
            else
            {
                CREATEWIDGET(OkDialog,
                    // TRANSLATORS: file uploaded message
                    _("File uploaded"),
                    str,
                    // TRANSLATORS: ok dialog button
                    _("OK"),
                    DialogType::OK,
                    Modal_true,
                    ShowCenter_false,
                    nullptr,
                    260);
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
    UploadChatInfo *const info = new UploadChatInfo;
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

static Being *findBeing(const std::string &name, const bool npc)
{
    if ((localPlayer == nullptr) || (actorManager == nullptr))
        return nullptr;

    Being *being = nullptr;

    if (name.empty())
    {
        being = localPlayer->getTarget();
    }
    else
    {
        being = actorManager->findBeingByName(
            name, ActorType::Unknown);
    }
    if ((being == nullptr) && npc)
    {
        being = actorManager->findNearestLivingBeing(
            localPlayer, 1, ActorType::Npc, AllowSort_true);
        if (being != nullptr)
        {
            if (abs(being->getTileX() - localPlayer->getTileX()) > 1
                || abs(being->getTileY() - localPlayer->getTileY()) > 1)
            {
                being = nullptr;
            }
        }
    }
    if ((being == nullptr) && npc)
    {
        being = actorManager->findNearestLivingBeing(
            localPlayer, 1, ActorType::Player, AllowSort_true);
        if (being != nullptr)
        {
            if (abs(being->getTileX() - localPlayer->getTileX()) > 1
                || abs(being->getTileY() - localPlayer->getTileY()) > 1)
            {
                being = nullptr;
            }
        }
    }
    return being;
}

static Item *getItemByInvIndex(const InputEvent &event,
                               const InventoryTypeT invType)
{
    const int index = atoi(event.args.c_str());
    const Inventory *inv = nullptr;
    switch (invType)
    {
        case InventoryType::Storage:
            inv = PlayerInfo::getStorageInventory();
            break;

        case InventoryType::Inventory:
            inv = PlayerInfo::getInventory();
            break;
        case InventoryType::Trade:
        case InventoryType::Npc:
        case InventoryType::Cart:
        case InventoryType::Vending:
        case InventoryType::Mail:
        case InventoryType::Craft:
        case InventoryType::TypeEnd:
        default:
            break;
    }
    if (inv != nullptr)
        return inv->getItem(index);
    return nullptr;
}

static int getAmountFromEvent(const InputEvent &event,
                              Item *&item0,
                              const InventoryTypeT invType)
{
    Item *const item = getItemByInvIndex(event, invType);
    item0 = item;
    if (item == nullptr)
        return 0;

    std::string str = event.args;
    removeToken(str, " ");

    if (str.empty())
        return 0;

    int amount = 0;
    if (str[0] == '-')
    {
        if (str.size() > 1)
        {
            amount = item->getQuantity() - atoi(str.substr(1).c_str());
            if (amount <= 0 || amount > item->getQuantity())
                amount = item->getQuantity();
        }
    }
    else if (str == "/")
    {
        amount = item->getQuantity() / 2;
    }
    else if (str == "all")
    {
        amount = item->getQuantity();
    }
    else
    {
        amount = atoi(str.c_str());
    }
    return amount;
}

impHandler(emote)
{
    const int emotion = 1 + (event.action - InputAction::EMOTE_1);
    if (emotion > 0)
    {
        if (emoteShortcut != nullptr)
            emoteShortcut->useEmotePlayer(emotion);
        if (Game::instance() != nullptr)
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
        if ((outfitWindow != nullptr) && num >= 0)
        {
            outfitWindow->wearOutfit(num);
            if (Game::instance() != nullptr)
                Game::instance()->setValidSpeed();
            return true;
        }
    }
    else if (inputManager.isActionActive(InputAction::COPY_OUTFIT))
    {
        const int num = event.action - InputAction::OUTFIT_1;
        if ((outfitWindow != nullptr) && num >= 0)
        {
            outfitWindow->copyOutfit(num);
            if (Game::instance() != nullptr)
                Game::instance()->setValidSpeed();
            return true;
        }
    }

    return false;
}

impHandler0(mouseClick)
{
    if ((guiInput == nullptr) || (gui == nullptr))
        return false;

    int mouseX, mouseY;
    Gui::getMouseState(mouseX, mouseY);
    guiInput->simulateMouseClick(mouseX, mouseY, MouseButton::RIGHT);
    return true;
}

impHandler0(ok)
{
    // Close the Browser if opened
    if ((helpWindow != nullptr) && helpWindow->isWindowVisible())
    {
        helpWindow->setVisible(Visible_false);
        return true;
    }
    // Close the config window, cancelling changes if opened
    else if ((setupWindow != nullptr) && setupWindow->isWindowVisible())
    {
        setupWindow->action(ActionEvent(nullptr, "cancel"));
        return true;
    }
    else if (NpcDialog *const dialog = NpcDialog::getActive())
    {
        dialog->action(ActionEvent(nullptr, "ok"));
        return true;
    }
    else if (popupMenu->isPopupVisible())
    {
        popupMenu->select();
    }
    return false;
}

impHandler(shortcut)
{
    if (itemShortcutWindow != nullptr)
    {
        const int num = itemShortcutWindow->getTabIndex();
        if (num >= 0 && num < CAST_S32(SHORTCUT_TABS))
        {
            if (itemShortcut[num] != nullptr)
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
    if (Game::instance() == nullptr)
        return false;
    if ((popupManager != nullptr) && popupManager->isPopupMenuVisible())
    {
        popupManager->closePopupMenu();
        return true;
    }
    else if (quitDialog == nullptr)
    {
        CREATEWIDGETV(quitDialog, QuitDialog,
            &quitDialog);
        quitDialog->requestMoveToTop();
        return true;
    }
    return false;
}

impHandler0(dropItem0)
{
    if (dropShortcut != nullptr)
    {
        dropShortcut->dropFirst();
        return true;
    }
    return false;
}

impHandler0(dropItem)
{
    if (dropShortcut != nullptr)
    {
        dropShortcut->dropItems();
        return true;
    }
    return false;
}

impHandler(dropItemId)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return false;

    // +++ ignoring item color for now
    Item *const item = inv->findItem(atoi(event.args.c_str()),
        ItemColor_one);

    if ((item != nullptr) && !PlayerInfo::isItemProtected(item->getId()))
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::ItemDrop,
            inventoryWindow, item);
    }
    return true;
}

impHandler(dropItemInv)
{
    Item *const item = getItemByInvIndex(event, InventoryType::Inventory);
    if ((item != nullptr) && !PlayerInfo::isItemProtected(item->getId()))
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::ItemDrop,
            inventoryWindow, item);
    }
    return true;
}

impHandler(dropItemIdAll)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return false;

    // +++ ignoring item color for now
    Item *const item = inv->findItem(atoi(event.args.c_str()),
        ItemColor_one);

    if ((item != nullptr) && !PlayerInfo::isItemProtected(item->getId()))
        PlayerInfo::dropItem(item, item->getQuantity(), Sfx_true);
    return true;
}

impHandler(dropItemInvAll)
{
    Item *const item = getItemByInvIndex(event, InventoryType::Inventory);
    if ((item != nullptr) && !PlayerInfo::isItemProtected(item->getId()))
        PlayerInfo::dropItem(item, item->getQuantity(), Sfx_true);
    return true;
}

#ifdef TMWA_SUPPORT
impHandler(heal)
{
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return false;
    if (actorManager != nullptr &&
        localPlayer != nullptr)
    {
        std::string args = event.args;

        if (!args.empty())
        {
            const Being *being = nullptr;
            if (args[0] == ':')
            {
                being = actorManager->findBeing(fromInt(atoi(
                    args.substr(1).c_str()), BeingId));
                if (being != nullptr && being->getType() == ActorType::Monster)
                    being = nullptr;
            }
            else
            {
                being = actorManager->findBeingByName(args, ActorType::Player);
            }
            if (being != nullptr)
                actorManager->heal(being);
        }
        else
        {
            Being *target = localPlayer->getTarget();
            if (inputManager.isActionActive(InputAction::STOP_ATTACK))
            {
                if (target == nullptr ||
                    target->getType() != ActorType::Player)
                {
                    target = actorManager->findNearestLivingBeing(
                        localPlayer, 10, ActorType::Player, AllowSort_true);
                }
            }
            else
            {
                if (target == nullptr)
                    target = localPlayer;
            }
            actorManager->heal(target);
        }

        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}
#else  // TMWA_SUPPORT

impHandler0(heal)
{
    return false;
}
#endif  // TMWA_SUPPORT

impHandler0(healmd)
{
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return false;
    if (actorManager != nullptr)
    {
        const int matk = PlayerInfo::getStatEffective(Attributes::PLAYER_MATK);
        int maxHealingRadius;

        // magic levels < 2
        if (PlayerInfo::getSkillLevel(340) < 2
            || PlayerInfo::getSkillLevel(341) < 2)
        {
            maxHealingRadius = matk / 100 + 1;
        }
        else
        {
            maxHealingRadius = (12 * fastSqrtInt(matk) + matk) / 100 + 1;
        }

        Being *target = actorManager->findMostDamagedPlayer(maxHealingRadius);
        if (target != nullptr)
            actorManager->heal(target);

        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }
#endif  // TMWA_SUPPORT

    return false;
}

impHandler0(itenplz)
{
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return false;
    if (actorManager != nullptr)
    {
        if (playerHandler != nullptr &&
            playerHandler->canUseMagic() &&
            PlayerInfo::getAttribute(Attributes::PLAYER_MP) >= 3)
        {
            actorManager->itenplz();
        }
        return true;
    }
#endif  // TMWA_SUPPORT

    return false;
}

impHandler0(setHome)
{
    if (localPlayer != nullptr)
    {
        localPlayer->setHome();
        return true;
    }
    return false;
}

impHandler0(magicAttack)
{
#ifdef TMWA_SUPPORT
    if (Net::getNetworkType() != ServerType::TMWATHENA)
        return false;
    if (localPlayer != nullptr)
    {
        localPlayer->magicAttack();
        return true;
    }
#endif  // TMWA_SUPPORT

    return false;
}

impHandler0(copyEquippedToOutfit)
{
    if (outfitWindow != nullptr)
    {
        outfitWindow->copyFromEquiped();
        return true;
    }
    return false;
}

impHandler(pickup)
{
    if (localPlayer == nullptr)
        return false;

    const std::string args = event.args;
    if (args.empty())
    {
        localPlayer->pickUpItems();
    }
    else
    {
        FloorItem *const item = actorManager->findItem(fromInt(
            atoi(args.c_str()), BeingId));
        if (item != nullptr)
            localPlayer->pickUp(item);
    }
    return true;
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
    if (localPlayer != nullptr)
    {
        doSit();
        return true;
    }
    return false;
}

impHandler(screenshot)
{
    Game::createScreenshot(event.args);
    return true;
}

impHandler0(ignoreInput)
{
    return true;
}

impHandler(buy)
{
    if (serverFeatures == nullptr)
        return false;
    const std::string args = event.args;
    Being *being = findBeing(args, false);
    if ((being == nullptr) && Net::getNetworkType() == ServerType::TMWATHENA)
    {
        if (whoIsOnline != nullptr)
        {
            const std::set<std::string> &players =
                whoIsOnline->getOnlineNicks();
            if (players.find(args) != players.end())
            {
                if (buySellHandler != nullptr)
                    buySellHandler->requestSellList(args);
                return true;
            }
        }
        return false;
    }

    if (being == nullptr)
        being = findBeing(args, true);

    if (being == nullptr)
        return false;

    if (being->getType() == ActorType::Npc)
    {
        if (npcHandler != nullptr)
            npcHandler->buy(being);
        return true;
    }
    else if (being->getType() == ActorType::Player)
    {
        if (vendingHandler != nullptr &&
            Net::getNetworkType() != ServerType::TMWATHENA)
        {
            vendingHandler->open(being);
        }
        else if (buySellHandler != nullptr)
        {
            buySellHandler->requestSellList(being->getName());
        }
        return true;
    }
    return false;
}

impHandler(sell)
{
    if (serverFeatures == nullptr)
        return false;

    const std::string args = event.args;
    Being *being = findBeing(args, false);
    if (being == nullptr &&
        Net::getNetworkType() == ServerType::TMWATHENA)
    {
        if (whoIsOnline != nullptr)
        {
            const std::set<std::string> &players =
                whoIsOnline->getOnlineNicks();
            if (players.find(args) != players.end())
            {
                if (buySellHandler != nullptr)
                    buySellHandler->requestBuyList(args);
                return true;
            }
        }
        return false;
    }

    if (being == nullptr)
        being = findBeing(args, true);

    if (being == nullptr)
        return false;

    if (being->getType() == ActorType::Npc)
    {
        if (npcHandler != nullptr)
            npcHandler->sell(being->getId());
        return true;
    }
    else if (being->getType() == ActorType::Player)
    {
        if ((buyingStoreHandler != nullptr) &&
            Net::getNetworkType() != ServerType::TMWATHENA)
        {
            buyingStoreHandler->open(being);
        }
        else if (buySellHandler != nullptr)
        {
            buySellHandler->requestBuyList(being->getName());
        }
        return true;
    }
    return false;
}

impHandler(talk)
{
    Being *being = findBeing(event.args, true);
    if (being == nullptr)
        return false;

    if (being->canTalk())
    {
        being->talkTo();
    }
    else if (being->getType() == ActorType::Player)
    {
        CREATEWIDGET(BuySellDialog,
            being->getName());
    }
    return true;
}

impHandler0(stopAttack)
{
    if (localPlayer != nullptr)
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
    if (localPlayer != nullptr)
    {
        localPlayer->untarget();
        return true;
    }
    return false;
}

impHandler(attack)
{
    if ((localPlayer == nullptr) || (actorManager == nullptr))
        return false;

    Being *target = nullptr;
    std::string args = event.args;
    if (!args.empty())
    {
        if (args[0] != ':')
        {
            target = actorManager->findNearestByName(args);
        }
        else
        {
            target = actorManager->findBeing(fromInt(atoi(
                args.substr(1).c_str()), BeingId));
            if (target != nullptr &&
                target->getType() != ActorType::Monster)
            {
                target = nullptr;
            }
        }
    }
    if (target == nullptr)
        target = localPlayer->getTarget();
    else
        localPlayer->setTarget(target);
    if (target != nullptr)
        localPlayer->attack(target, true);
    return true;
}

impHandler(targetAttack)
{
    if ((localPlayer != nullptr) && (actorManager != nullptr))
    {
        Being *target = nullptr;
        std::string args = event.args;
        const bool newTarget = !inputManager.isActionActive(
            InputAction::STOP_ATTACK);

        if (!args.empty())
        {
            if (args[0] != ':')
            {
                target = actorManager->findNearestByName(args);
            }
            else
            {
                target = actorManager->findBeing(fromInt(atoi(
                    args.substr(1).c_str()), BeingId));
                if (target != nullptr &&
                    target->getType() != ActorType::Monster)
                {
                    target = nullptr;
                }
            }
        }

        if ((target == nullptr) && (settings.targetingType == 0u))
            target = localPlayer->getTarget();

        if (target == nullptr)
        {
            target = actorManager->findNearestLivingBeing(
                localPlayer, 90, ActorType::Monster, AllowSort_true);
        }

        localPlayer->attack2(target, newTarget);
        return true;
    }
    return false;
}

impHandler0(attackHuman)
{
    if ((actorManager == nullptr) || (localPlayer == nullptr))
        return false;

    Being *const target = actorManager->findNearestPvpPlayer();
    if (target != nullptr)
    {
        localPlayer->setTarget(target);
        localPlayer->attack2(target, true);
    }
    return true;
}

impHandler0(safeVideoMode)
{
    WindowManager::setFullScreen(false);

    return true;
}

impHandler0(stopSit)
{
    if (localPlayer != nullptr)
    {
        localPlayer->stopAttack();
        // not consume if target attack key pressed
        if (inputManager.isActionActive(InputAction::TARGET_ATTACK))
            return false;
        if (localPlayer->getTarget() == nullptr)
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
#else  // USE_SDL2

    SDL_ANDROID_ToggleScreenKeyboardTextInput(nullptr);
#endif  // USE_SDL2

    return true;
#else  // ANDROID

    return false;
#endif  // ANDROID
}

impHandler0(showWindows)
{
    if (popupMenu != nullptr)
    {
        popupMenu->showWindowsPopup();
        return true;
    }
    return false;
}

impHandler0(openTrade)
{
    const Being *const being = localPlayer->getTarget();
    if ((being != nullptr) && being->getType() == ActorType::Player)
    {
        if (tradeHandler != nullptr)
            tradeHandler->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow != nullptr)
            tradeWindow->clear();
        return true;
    }
    return false;
}

impHandler0(ipcToggle)
{
    if (ipc != nullptr)
    {
        IPC::stop();
        if (ipc == nullptr)
        {
            debugChatTab->chatLog("IPC service stopped.",
                ChatMsgType::BY_SERVER);
        }
        else
        {
            debugChatTab->chatLog("Unable to stop IPC service.",
                ChatMsgType::BY_SERVER);
        }
    }
    else
    {
        IPC::start();
        if (ipc != nullptr)
        {
            debugChatTab->chatLog(
                strprintf("IPC service available on port %d", ipc->getPort()),
                ChatMsgType::BY_SERVER);
        }
        else
        {
            debugChatTab->chatLog("Unable to start IPC service",
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(where)
{
    ChatTab *const tab = event.tab != nullptr ? event.tab : debugChatTab;
    if (tab == nullptr)
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
    if (chatHandler != nullptr)
        chatHandler->who();
    return true;
}

impHandler0(cleanGraphics)
{
    ResourceManager::clearCache();

    if (debugChatTab != nullptr)
    {
        // TRANSLATORS: clear graphics command message
        debugChatTab->chatLog(_("Cache cleaned"),
            ChatMsgType::BY_SERVER);
    }
    return true;
}

impHandler0(cleanFonts)
{
    if (gui != nullptr)
        gui->clearFonts();
    if (debugChatTab != nullptr)
    {
        // TRANSLATORS: clear fonts cache message
        debugChatTab->chatLog(_("Cache cleaned"),
            ChatMsgType::BY_SERVER);
    }
    return true;
}

impHandler(trade)
{
    if (actorManager == nullptr)
        return false;

    const Being *being = actorManager->findBeingByName(
        event.args, ActorType::Player);
    if (being == nullptr)
        being = localPlayer->getTarget();
    if (being != nullptr)
    {
        if (tradeHandler != nullptr)
            tradeHandler->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow != nullptr)
            tradeWindow->clear();
    }
    return true;
}

impHandler0(priceLoad)
{
    if (shopWindow != nullptr)
    {
        shopWindow->loadList();
        return true;
    }
    return false;
}

impHandler0(priceSave)
{
    if (shopWindow != nullptr)
    {
        shopWindow->saveList();
        return true;
    }
    return false;
}

impHandler0(cacheInfo)
{
    if ((chatWindow == nullptr) || (debugChatTab == nullptr))
        return false;

/*
    Font *const font = chatWindow->getFont();
    if (!font)
        return;

    const TextChunkList *const cache = font->getCache();
    if (!cache)
        return;

    unsigned int all = 0;
    // TRANSLATORS: chat fonts message
    debugChatTab->chatLog(_("font cache size"), ChatMsgType::BY_SERVER);
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].size)
        {
            const unsigned int sz = CAST_S32(cache[f].size);
            all += sz;
            str.append(strprintf("%d: %u, ", f, sz));
        }
    }
    debugChatTab->chatLog(str, ChatMsgType::BY_SERVER);
    // TRANSLATORS: chat fonts message
    debugChatTab->chatLog(strprintf("%s %d", _("Cache size:"), all),
        ChatMsgType::BY_SERVER);
#ifdef DEBUG_FONT_COUNTERS
    debugChatTab->chatLog("", ChatMsgType::BY_SERVER);
    debugChatTab->chatLog(strprintf("%s %d",
        // TRANSLATORS: chat fonts message
        _("Created:"), font->getCreateCounter()),
        ChatMsgType::BY_SERVER);
    debugChatTab->chatLog(strprintf("%s %d",
        // TRANSLATORS: chat fonts message
        _("Deleted:"), font->getDeleteCounter()),
        ChatMsgType::BY_SERVER);
#endif
*/
    return true;
}

impHandler0(disconnect)
{
    if (gameHandler != nullptr)
        gameHandler->disconnect2();
    return true;
}

impHandler(undress)
{
    if ((actorManager == nullptr) || (localPlayer == nullptr))
        return false;

    const std::string args = event.args;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;
    Being *target = nullptr;
    const size_t sz = pars.size();
    if (sz == 0)
    {
        target = localPlayer->getTarget();
    }
    else
    {
        if (pars[0][0] == ':')
        {
            target = actorManager->findBeing(fromInt(atoi(
                pars[0].substr(1).c_str()), BeingId));
            if ((target != nullptr) && target->getType() == ActorType::Monster)
                target = nullptr;
        }
        else
        {
            target = actorManager->findNearestByName(args);
        }
    }

    if (sz == 2)
    {
        const int itemId = atoi(pars[1].c_str());
        if (target != nullptr)
            target->undressItemById(itemId);
    }
    else
    {
        if ((target != nullptr) && (beingHandler != nullptr))
            beingHandler->undress(target);
    }

    return true;
}

impHandler0(dirs)
{
    if (debugChatTab == nullptr)
        return false;

    debugChatTab->chatLog("config directory: "
        + settings.configDir,
        ChatMsgType::BY_SERVER);
    debugChatTab->chatLog("logs directory: "
        + settings.localDataDir,
        ChatMsgType::BY_SERVER);
    debugChatTab->chatLog("screenshots directory: "
        + settings.screenshotDir,
        ChatMsgType::BY_SERVER);
    debugChatTab->chatLog("temp directory: "
        + settings.tempDir,
        ChatMsgType::BY_SERVER);
    return true;
}

impHandler0(uptime)
{
    if (debugChatTab == nullptr)
        return false;

    if (cur_time < start_time)
    {
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), "unknown"),
            ChatMsgType::BY_SERVER);
    }
    else
    {
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"),
            timeDiffToString(CAST_S32(cur_time - start_time)).c_str()),
            ChatMsgType::BY_SERVER);
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
        debugChatTab->chatLog(str, ChatMsgType::BY_SERVER);
    logger->log(str);
    ResourceManager::ResourceIterator iter = res->begin();
    const ResourceManager::ResourceIterator iter_end = res->end();
    while (iter != iter_end)
    {
        if (iter->second && iter->second->mRefCount)
        {
            char type = ' ';
            char isNew = 'N';
            if (iter->second->getDumped())
                isNew = 'O';
            else
                iter->second->setDumped(true);

            SubImage *const subImage = dynamic_cast<SubImage *>(
                iter->second);
            Image *const image = dynamic_cast<Image *>(iter->second);
            int id = 0;
            if (subImage)
                type = 'S';
            else if (image)
                type = 'I';
            if (image)
                id = image->getGLImage();
            logger->log("Resource %c%c: %s (%d) id=%d", type,
                isNew, iter->second->getIdPath().c_str(),
                iter->second->mRefCount, id);
        }
        ++ iter;
    }
}

impHandler(dump)
{
    if (!debugChatTab)
        return false;

    if (!event.args.empty())
    {
        ResourceManager::Resources *res = ResourceManager::getResources();
        // TRANSLATORS: dump command
        showRes(_("Resource images:"), res);
        res = ResourceManager::getOrphanedResources();
        // TRANSLATORS: dump command
        showRes(_("Resource orphaned images:"), res);
    }
    else
    {
        ResourceManager::Resources *res = ResourceManager::getResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource images:") + toString(res->size()),
            ChatMsgType::BY_SERVER);
        res = ResourceManager::getOrphanedResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource orphaned images:")
            + toString(res->size()),
            ChatMsgType::BY_SERVER);
    }
    return true;
}

#elif defined ENABLE_MEM_DEBUG
impHandler0(dump)
{
    check_leaks();
    return true;
}
#else  // DEBUG_DUMP_LEAKS1

impHandler0(dump)
{
    return true;
}
#endif  // DEBUG_DUMP_LEAKS1

impHandler0(serverIgnoreAll)
{
    if (chatHandler != nullptr)
        chatHandler->ignoreAll();
    return true;
}

impHandler0(serverUnIgnoreAll)
{
    if (chatHandler != nullptr)
        chatHandler->unIgnoreAll();
    return true;
}

PRAGMA6(GCC diagnostic push)
PRAGMA6(GCC diagnostic ignored "-Wnull-dereference")
impHandler0(error)
{
    int *const ptr = nullptr;
    *(ptr + 1) = 20;
//    logger->log("test %d", *ptr);
    exit(1);
}
PRAGMA6(GCC diagnostic pop)

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
#else  // defined USE_OPENGL

    str.append(",0");
#endif  // defined USE_OPENGL

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
    for (char **env = environ; *env != nullptr; ++ env)
        logger->log1(*env);
    logger->log1("End environment variables");
    if (debugChatTab != nullptr)
    {
        // TRANSLATORS: dump environment command
        debugChatTab->chatLog(_("Environment variables dumped"),
            ChatMsgType::BY_SERVER);
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
#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__native_client__)
    NormalOpenGLGraphics::dumpSettings();
#endif  // defined(USE_OPENGL) && !defined(ANDROID) &&
        // !defined(__native_client__)

    return true;
}

#ifdef USE_OPENGL
impHandler(dumpGL)
{
    std::string str = graphicsManager.getGLVersion();
    outStringNormal(event.tab, str, str);
    return true;
}
#else  // USE_OPENGL

impHandler0(dumpGL)
{
    return true;
}
#endif  // USE_OPENGL

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
    Color color(0, 0, 0, 255);

    for (int f = 0; f < 500; f ++)
    {
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
        {
            width += font->getWidth(*it);
            font->drawString(nullGraphics, color, color, *it, 10, 10);
        }
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
            font->drawString(nullGraphics, color, color, *it, 10, 10);

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
    {
        debugChatTab->chatLog("sdlfont time: " + toString(diff),
            ChatMsgType::BY_SERVER);
    }
    return true;
}
#endif  // defined USE_OPENGL && defined DEBUG_SDLFONT

impHandler0(createItems)
{
    BuyDialog *const dialog = CREATEWIDGETR0(BuyDialog);
    const ItemDB::ItemInfos &items = ItemDB::getItemInfos();
    FOR_EACH (ItemDB::ItemInfos::const_iterator, it, items)
    {
        const ItemInfo *const info = (*it).second;
        if (info == nullptr)
            continue;
        const int id = info->getId();
        if (id <= 500)
            continue;

        dialog->addItem(id,
            ItemType::Unknown,
            ItemColor_one,
            100,
            0);
    }
    dialog->sort();
    return true;
}

impHandler(createItem)
{
    int id = 0;
    int amount = 0;

    if (adminHandler == nullptr)
        return false;

    if (parse2Int(event.args, id, amount))
        adminHandler->createItems(id, ItemColor_one, amount);
    else
        adminHandler->createItems(atoi(event.args.c_str()), ItemColor_one, 1);
    return true;
}

impHandler(uploadConfig)
{
    // TRANSLATORS: upload config chat message
    uploadFile(_("Uploaded config into:"),
        config.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadServerConfig)
{
    // TRANSLATORS: upload config chat message
    uploadFile(_("Uploaded server config into:"),
        serverConfig.getFileName(),
        "?xml",
        event.tab);
    return true;
}

impHandler(uploadLog)
{
    // TRANSLATORS: upload log chat message
    uploadFile(_("Uploaded log into:"),
        settings.logFileName,
        "?txt",
        event.tab);
    return true;
}

impHandler0(mercenaryFire)
{
    if (mercenaryHandler != nullptr)
        mercenaryHandler->fire();
    return true;
}

impHandler0(mercenaryToMaster)
{
    if (mercenaryHandler != nullptr)
        mercenaryHandler->moveToMaster();
    return true;
}

impHandler0(homunculusToMaster)
{
    if (homunculusHandler != nullptr)
        homunculusHandler->moveToMaster();
    return true;
}

impHandler0(homunculusFeed)
{
    if (homunculusHandler != nullptr)
        homunculusHandler->feed();
    return true;
}

impHandler0(petFeed)
{
    if (petHandler != nullptr)
        petHandler->feed();
    return true;
}

impHandler(useItem)
{
    const int itemId = atoi(event.args.c_str());

    if (itemId < SPELL_MIN_ID)
    {
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv != nullptr)
        {
            // +++ ignoring item color for now
            const Item *const item = inv->findItem(itemId,
                ItemColor_one);
            PlayerInfo::useEquipItem(item, Sfx_true);
        }
    }
    else if (itemId < SKILL_MIN_ID && (spellManager != nullptr))
    {
        spellManager->useItem(itemId);
    }
    else if (skillDialog != nullptr)
    {
        // +++ probably need get data parameter from args
        skillDialog->useItem(itemId,
            fromBool(config.getBoolValue("skillAutotarget"), AutoTarget),
            0,
            std::string());
    }
    return true;
}

impHandler(useItemInv)
{
    Item *const item = getItemByInvIndex(event, InventoryType::Inventory);
    PlayerInfo::useEquipItem(item, Sfx_true);
    return true;
}

impHandler(invToStorage)
{
    Item *item = nullptr;
    const int amount = getAmountFromEvent(event, item,
        InventoryType::Inventory);
    if (item == nullptr)
        return true;
    if (amount != 0)
    {
        if (inventoryHandler != nullptr)
        {
            inventoryHandler->moveItem2(InventoryType::Inventory,
                item->getInvIndex(),
                amount,
                InventoryType::Storage);
        }
    }
    else
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::StoreAdd,
            inventoryWindow, item);
    }
    return true;
}

impHandler(tradeAdd)
{
    Item *item = nullptr;
    const int amount = getAmountFromEvent(event, item,
        InventoryType::Inventory);
    if ((item == nullptr) || PlayerInfo::isItemProtected(item->getId()))
        return true;

    if (amount != 0)
    {
        if (tradeWindow != nullptr)
            tradeWindow->tradeItem(item, amount, true);
    }
    else
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::TradeAdd,
            tradeWindow, item);
    }
    return true;
}

impHandler(storageToInv)
{
    Item *item = nullptr;
    const int amount = getAmountFromEvent(event, item, InventoryType::Storage);
    if (amount != 0)
    {
        if ((inventoryHandler != nullptr) && (item != nullptr))
        {
            inventoryHandler->moveItem2(InventoryType::Storage,
                item->getInvIndex(),
                amount,
                InventoryType::Inventory);
        }
    }
    else
    {
        ItemAmountWindow::showWindow(ItemAmountWindowUsage::StoreRemove,
            storageWindow, item);
    }
    return true;
}

impHandler(protectItem)
{
    const int id = atoi(event.args.c_str());
    if (id > 0)
        PlayerInfo::protectItem(id);
    return true;
}

impHandler(unprotectItem)
{
    const int id = atoi(event.args.c_str());
    if (id > 0)
        PlayerInfo::unprotectItem(id);
    return true;
}

impHandler(kick)
{
    if ((localPlayer == nullptr) || (actorManager == nullptr))
        return false;

    Being *target = nullptr;
    std::string args = event.args;
    if (!args.empty())
    {
        if (args[0] != ':')
        {
            target = actorManager->findNearestByName(args);
        }
        else
        {
            target = actorManager->findBeing(fromInt(atoi(
                args.substr(1).c_str()), BeingId));
        }
    }
    if (target == nullptr)
        target = localPlayer->getTarget();
    if ((target != nullptr) && (adminHandler != nullptr))
        adminHandler->kick(target->getId());
    return true;
}

impHandler0(clearDrop)
{
    if (dropShortcut != nullptr)
        dropShortcut->clear();
    return true;
}

impHandler0(testInfo)
{
    if (actorManager != nullptr)
    {
        logger->log("actors count: %d", CAST_S32(
            actorManager->size()));
        return true;
    }
    return false;
}

impHandler(craftKey)
{
    const int slot = (event.action - InputAction::CRAFT_1);
    if (slot >= 0 && slot < 9)
    {
        if (inventoryWindow != nullptr)
            inventoryWindow->moveItemToCraft(slot);
        return true;
    }
    return false;
}

impHandler0(resetGameModifiers)
{
    GameModifiers::resetModifiers();
    return true;
}

impHandler(barToChat)
{
    if (chatWindow != nullptr)
    {
        chatWindow->addInputText(event.args);
        return true;
    }
    return false;
}

impHandler(seen)
{
    if (actorManager == nullptr)
        return false;

    ChatTab *tab = event.tab;
    if (tab == nullptr)
        tab = localChatTab;
    if (tab == nullptr)
        return false;

    if (config.getBoolValue("enableIdCollecting") == false)
    {
        // TRANSLATORS: last seen disabled warning
        tab->chatLog(_("Last seen disabled. "
            "Enable in players / collect players id and seen log."),
            ChatMsgType::BY_SERVER);
        return true;
    }

    const std::string name = event.args;
    if (name.empty())
        return false;

    std::string dir = settings.usersDir;
    dir.append(stringToHexPath(name)).append("/seen.txt");
    if (Files::existsLocal(dir))
    {
        StringVect lines;
        Files::loadTextFileLocal(dir, lines);
        if (lines.size() < 3)
        {
            // TRANSLATORS: last seen error
            tab->chatLog(_("You not saw this nick."),
                ChatMsgType::BY_SERVER);
            return true;
        }
        const std::string message = strprintf(
            // TRANSLATORS: last seen message
            _("Last seen for %s: %s"),
            name.c_str(),
            lines[2].c_str());
        tab->chatLog(message, ChatMsgType::BY_SERVER);
    }
    else
    {
        // TRANSLATORS: last seen error
        tab->chatLog(_("You not saw this nick."),
            ChatMsgType::BY_SERVER);
    }

    return true;
}

impHandler(dumpMemoryUsage)
{
    if (event.tab != nullptr)
        memoryManager.printAllMemory(event.tab);
    else
        memoryManager.printAllMemory(localChatTab);
    return true;
}

impHandler(setEmoteType)
{
    const std::string &args = event.args;
    if (args == "player" || args.empty())
    {
        settings.emoteType = EmoteType::Player;
    }
    else if (args == "pet")
    {
        settings.emoteType = EmoteType::Pet;
    }
    else if (args == "homun" || args == "homunculus")
    {
        settings.emoteType = EmoteType::Homunculus;
    }
    else if (args == "merc" || args == "mercenary")
    {
        settings.emoteType = EmoteType::Mercenary;
    }
    return true;
}

}  // namespace Actions
