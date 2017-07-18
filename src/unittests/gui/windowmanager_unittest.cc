/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "unittests/unittests.h"

#include "client.h"
#include "configmanager.h"
#include "configuration.h"
#include "dirs.h"
#include "graphicsmanager.h"
#include "main.h"
#include "settings.h"
#include "textcommand.h"

#include "being/localplayer.h"

#include "const/resources/currency.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"
#include "gui/mailmessage.h"
#include "gui/userpalette.h"
#include "gui/windowmanager.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/skillpopup.h"
#include "gui/popups/spellpopup.h"
#include "gui/popups/speechbubble.h"
#include "gui/popups/statuspopup.h"
#include "gui/popups/textboxpopup.h"
#include "gui/popups/textpopup.h"

#include "gui/widgets/desktop.h"
#include "gui/widgets/emoteshortcutcontainer.h"
#include "gui/widgets/createwidget.h"

#include "gui/windows/bankwindow.h"
#include "gui/windows/buydialog.h"
#include "gui/windows/buyingstoreselldialog.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/charselectdialog.h"
#include "gui/windows/changeemaildialog.h"
#include "gui/windows/changepassworddialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/connectiondialog.h"
#include "gui/windows/confirmdialog.h"
#include "gui/windows/cutinwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/editdialog.h"
#include "gui/windows/editserverdialog.h"
#include "gui/windows/eggselectiondialog.h"
#include "gui/windows/emotewindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/insertcarddialog.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/logindialog.h"
#include "gui/windows/maileditwindow.h"
#include "gui/windows/mailviewwindow.h"
#include "gui/windows/mailwindow.h"
#include "gui/windows/minimap.h"
#include "gui/windows/ministatuswindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/npcselldialog.h"
#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/registerdialog.h"
#include "gui/windows/serverdialog.h"
#include "gui/windows/serverinfowindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/textcommandeditor.h"
#include "gui/windows/textdialog.h"
#include "gui/windows/textselectdialog.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/updaterwindow.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/worldselectdialog.h"

#include "input/touch/touchmanager.h"

#include "net/logindata.h"

#include "net/eathena/charserverhandler.h"
#include "net/eathena/inventoryhandler.h"
#include "net/eathena/serverfeatures.h"
#include "net/eathena/playerhandler.h"

#include "render/sdlgraphics.h"

#include "resources/groupinfo.h"
#include "resources/sdlimagehelper.h"

#include "resources/db/groupdb.h"
#include "resources/db/unitsdb.h"

#include "resources/item/item.h"

#include "resources/map/map.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/gettext.h"

#include "utils/translation/translationmanager.h"

#include "debug.h"

extern QuitDialog *quitDialog;

TEST_CASE("windows leak test1", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}

TEST_CASE("Windows tests", "windowmanager")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::deleteInstance();
    ResourceManager::cleanOrphans(true);
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);
    paths.setDefaultValues(getPathsDefaults());
    branding.setValue("onlineServerFile", "test/serverlistplus.xml");
    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;
    config.setValue("fontSize", 16);
    theme = new Theme;
    Theme::selectSkin();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    const std::string cfgName = settings.configDir +
        "/nonexistserver/config.xml";
    ::remove(cfgName.c_str());

    ConfigManager::initConfiguration();
    getConfigDefaults2(config.getDefaultValues());
    branding.setDefaultValues(getBrandingDefaults());
    ConfigManager::initServerConfig("nonexistserver");

    localPlayer = new LocalPlayer(static_cast<BeingId>(1),
        BeingTypeId_zero);

    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);
    touchManager.init();
    UnitsDb::loadUnits();
    charServerHandler = new EAthena::CharServerHandler;
    serverFeatures = new EAthena::ServerFeatures;
    inventoryHandler = new EAthena::InventoryHandler;
    playerHandler = new EAthena::PlayerHandler;
    paths.setValue("itemIcons", "");

    TranslationManager::init();

    mainGraphics->setVideoMode(640, 480, 1, 8, false, false, false, false);

    SECTION("bankWindow")
    {
        CREATEWIDGETV0(bankWindow, BankWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(bankWindow);
    }
    SECTION("buyDialog1")
    {
        BuyDialog *dialog;
        CREATEWIDGETV0(dialog, BuyDialog);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("buyDialog2")
    {
        BuyDialog *dialog;
        CREATEWIDGETV(dialog, BuyDialog,
            BeingId_zero,
            DEFAULT_CURRENCY);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
#ifdef TMWA_SUPPORT
    SECTION("buyDialog3")
    {
        BuyDialog *dialog;
        CREATEWIDGETV(dialog, BuyDialog,
            "user",
            DEFAULT_CURRENCY);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
#endif  // TMWA_SUPPORT
    SECTION("buyDialog4")
    {
        BuyDialog *dialog;
        BeingTypeId id = static_cast<BeingTypeId>(1);
        Map *map = new Map("test map",
            10, 10,
            32, 32);
        Being *being = Being::createBeing(BeingId_zero,
            ActorType::Avatar,
            id,
            map);
        CREATEWIDGETV(dialog, BuyDialog,
            being,
            DEFAULT_CURRENCY);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(being);
    }
    SECTION("BuyingStoreSellDialog")
    {
        BuyingStoreSellDialog *dialog;
        CREATEWIDGETV(dialog, BuyingStoreSellDialog,
            BeingId_zero, 0);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("BuySellDialog1")
    {
        BuySellDialog *dialog;
        CREATEWIDGETV(dialog, BuySellDialog, BeingId_zero);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("BuySellDialog2")
    {
        BuySellDialog *dialog;
        CREATEWIDGETV(dialog, BuySellDialog, "user");
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("ChangeEmailDialog")
    {
        LoginData data;
        ChangeEmailDialog *dialog;
        CREATEWIDGETV(dialog, ChangeEmailDialog, data);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("ChangePasswordDialog")
    {
        LoginData data;
        ChangePasswordDialog *dialog;
        CREATEWIDGETV(dialog, ChangePasswordDialog, data);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("CharSelectDialog")
    {
        LoginData data;
        CharSelectDialog *dialog;
        CREATEWIDGETV(dialog, CharSelectDialog, data);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("CharCreateDialog")
    {
//        LoginData data;
//        CharSelectDialog *dialog2;
//        CREATEWIDGETV(dialog2, CharSelectDialog, data);
//        CharCreateDialog *dialog;
//        CREATEWIDGETV(dialog, CharCreateDialog, dialog2, 0);
//        gui->draw();
//        mainGraphics->updateScreen();
//        delete2(dialog);
//        delete2(dialog2);
    }
    SECTION("ChatWindow")
    {
        CREATEWIDGETV(chatWindow, ChatWindow,
            "Chat");
        gui->draw();
        mainGraphics->updateScreen();
        delete2(chatWindow);
    }
    SECTION("ConfirmDialog")
    {
        ConfirmDialog *dialog;
        CREATEWIDGETV(dialog, ConfirmDialog,
            "", "", "", false, Modal_false, nullptr);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("CutinWindow")
    {
        CREATEWIDGETV0(cutInWindow, CutInWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(cutInWindow);
    }
    SECTION("DebugWindow")
    {
        CREATEWIDGETV0(debugWindow, DebugWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(debugWindow);
    }
    SECTION("didYouKnowWindow")
    {
        CREATEWIDGETV0(didYouKnowWindow, DidYouKnowWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(didYouKnowWindow);
    }
    SECTION("EditDialog")
    {
        EditDialog *dialog;
        CREATEWIDGETV(dialog, EditDialog,
            "", "", "", 100, nullptr, Modal_false);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("EditServerDialog")
    {
        ServerInfo mCurrentServer;
        settings.configDir = VirtFs::getRealDir("test/serverlistplus.xml");
        ServerDialog *serverDialog = CREATEWIDGETR(ServerDialog,
            &mCurrentServer,
            settings.configDir);
        EditServerDialog *editServerDialog = CREATEWIDGETR(EditServerDialog,
            serverDialog, mCurrentServer, 0);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(editServerDialog);
        delete2(serverDialog);
    }
    SECTION("EggSelectionDialog")
    {
        EggSelectionDialog *dialog = CREATEWIDGETR0(EggSelectionDialog);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("EmoteWindow")
    {
        EmoteWindow *dialog = CREATEWIDGETR0(EmoteWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("EquipmentWindow")
    {
        Equipment *equipment = new Equipment;
        Map *map = new Map("test", 100, 100, 32, 32);
        Being *being = Being::createBeing(BeingId_zero,
            ActorType::Player,
            BeingTypeId_zero,
            map);
        EquipmentWindow *dialog = CREATEWIDGETR(EquipmentWindow,
            equipment, being, false);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(being);
        delete2(map);
        delete2(equipment);
    }
    SECTION("helpWindow")
    {
        CREATEWIDGETV0(helpWindow, HelpWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(helpWindow);
    }
    SECTION("InsertCardDialog")
    {
        Item *item = new Item(5000,
            ItemType::Card,
            1,
            0,
            ItemColor_one,
            Identified_true,
            Damaged_false,
            Favorite_false,
            Equipm_true,
            Equipped_false);
        InsertCardDialog *dialog = CREATEWIDGETR(InsertCardDialog,
            0, item);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(item);
    }
    SECTION("InventoryWindow")
    {
        Inventory *inventory = new Inventory(InventoryType::Inventory);
        InventoryWindow *dialog = CREATEWIDGETR(InventoryWindow,
            inventory);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(inventory);
    }
    SECTION("ItemAmountWindow")
    {
        Item *item = new Item(5000,
            ItemType::Card,
            1,
            0,
            ItemColor_one,
            Identified_true,
            Damaged_false,
            Favorite_false,
            Equipm_true,
            Equipped_false);
        ItemAmountWindow *dialog = CREATEWIDGETR(ItemAmountWindow,
            ItemAmountWindowUsage::ItemDrop, nullptr, item);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(item);
    }
    SECTION("KillStats")
    {
        CREATEWIDGETV0(killStats, KillStats);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(killStats);
    }
    SECTION("loginDialog")
    {
        ServerInfo mCurrentServer;
        LoginDialog *loginDialog = CREATEWIDGETR(LoginDialog,
            loginData,
            &mCurrentServer,
            &settings.options.updateHost);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(loginDialog);
    }
    SECTION("MailEditWindow")
    {
        CREATEWIDGETV0(mailEditWindow, MailEditWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(mailEditWindow);
    }
    SECTION("MailViewWindow")
    {
        MailMessage *message = new MailMessage;
        CREATEWIDGETV(mailViewWindow, MailViewWindow,
            message);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(mailViewWindow);
    }
    SECTION("MailWindow")
    {
        CREATEWIDGETV0(mailWindow, MailWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(mailWindow);
    }
    SECTION("Minimap")
    {
        CREATEWIDGETV0(minimap, Minimap);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(minimap);
    }
    SECTION("MiniStatusWindow")
    {
        CREATEWIDGETV0(miniStatusWindow, MiniStatusWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(miniStatusWindow);
    }
    SECTION("NpcDialog")
    {
        NpcDialog *dialog = CREATEWIDGETR(NpcDialog, BeingId_zero);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("NpcSellDialog")
    {
        NpcSellDialog *dialog = CREATEWIDGETR(NpcSellDialog, BeingId_zero);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("OkDialog")
    {
        OkDialog *dialog = CREATEWIDGETR(OkDialog,
            "", "", "", DialogType::SILENCE, Modal_false,
            ShowCenter_true, nullptr, 100);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("OutfitWindow")
    {
        CREATEWIDGETV0(outfitWindow, OutfitWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(outfitWindow);
    }
    SECTION("QuestsWindow")
    {
        CREATEWIDGETV0(questsWindow, QuestsWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(questsWindow);
    }
    SECTION("QuitDialog")
    {
        CREATEWIDGETV(quitDialog, QuitDialog,
            &quitDialog);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(quitDialog);
    }
    SECTION("RegisterDialog")
    {
        RegisterDialog *dialog = CREATEWIDGETR(RegisterDialog,
            loginData);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("serversDialog")
    {
        ServerInfo mCurrentServer;
        settings.configDir = VirtFs::getRealDir("test/serverlistplus.xml");
        ServerDialog *serverDialog = CREATEWIDGETR(ServerDialog,
            &mCurrentServer,
            settings.configDir);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(serverDialog);
    }
    SECTION("serversInfoWindow")
    {
        ServerInfo mCurrentServer;
        CREATEWIDGETV(serverInfoWindow, ServerInfoWindow,
            mCurrentServer);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(serverInfoWindow);
    }
    SECTION("setupWindow")
    {
        CREATEWIDGETV0(setupWindow, SetupWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(setupWindow);
    }
    SECTION("ShopSellDialog")
    {
        // only tmwa skipping
    }
    SECTION("ShopWindow")
    {
        CREATEWIDGETV0(shopWindow, ShopWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(shopWindow);
    }
    SECTION("ShortcutWindow1")
    {
        EmoteShortcutContainer *container =
            new EmoteShortcutContainer(nullptr);
        CREATEWIDGETV(itemShortcutWindow, ShortcutWindow,
            "name",
            container);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(itemShortcutWindow);
    }
    SECTION("ShortcutWindow2")
    {
        CREATEWIDGETV(itemShortcutWindow, ShortcutWindow, "");
        gui->draw();
        mainGraphics->updateScreen();
        delete2(itemShortcutWindow);
    }
    SECTION("SkillDialog")
    {
        CREATEWIDGETV0(skillDialog, SkillDialog);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(skillDialog);
    }
    SECTION("SocialWindow")
    {
        CREATEWIDGETV0(socialWindow, SocialWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(socialWindow);
    }
    SECTION("StatusWindow")
    {
        GroupDb::GroupInfos &groups = GroupDb::getGroups();
        groups[0] = new GroupInfo;
        groups[0]->name = std::string();
        groups[0]->longName = std::string();
        CREATEWIDGETV0(statusWindow, StatusWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(statusWindow);
        delete groups[0];
        groups.clear();
    }
    SECTION("TextCommandEditor")
    {
        TextCommand *textCommand = new TextCommand(1, "", "", "",
            CommandTarget::NoTarget, "");
        TextCommandEditor *dialog = CREATEWIDGETR(TextCommandEditor,
            textCommand);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
        delete2(textCommand);
    }

    SECTION("TextDialog")
    {
        TextDialog *dialog = CREATEWIDGETR(TextDialog,
            "", "", nullptr, false);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("TextSelectDialog")
    {
        TextSelectDialog *dialog = CREATEWIDGETR(TextSelectDialog,
            "", "", AllowQuit_false);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("TradeWindow")
    {
        CREATEWIDGETV0(tradeWindow, TradeWindow);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(tradeWindow);
    }
    SECTION("UpdaterWindow")
    {
        CREATEWIDGETV(updaterWindow, UpdaterWindow,
            "", "", false, UpdateType::Skip);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(updaterWindow);
    }
    SECTION("WhoIsOnline")
    {
        CREATEWIDGETV0(whoIsOnline, WhoIsOnline);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(whoIsOnline);
    }
    SECTION("WorldSelectDialog")
    {
        Worlds worlds;
        WorldSelectDialog *dialog = CREATEWIDGETR(WorldSelectDialog,
            worlds);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(dialog);
    }
    SECTION("popupMenu")
    {
        CREATEWIDGETV0(popupMenu, PopupMenu);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(popupMenu);
    }
    SECTION("skillPopup")
    {
        CREATEWIDGETV0(skillPopup, SkillPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(skillPopup);
    }
    SECTION("SpeechBubble")
    {
        SpeechBubble *bubble = CREATEWIDGETR0(SpeechBubble);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(bubble);
    }
    SECTION("beingPopup")
    {
        CREATEWIDGETV0(beingPopup, BeingPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(beingPopup);
    }
    SECTION("textPopup")
    {
        CREATEWIDGETV0(textPopup, TextPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(textPopup);
    }
    SECTION("textBoxPopup")
    {
        CREATEWIDGETV0(textBoxPopup, TextBoxPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(textBoxPopup);
    }
    SECTION("itemPopup")
    {
        CREATEWIDGETV0(itemPopup, ItemPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(itemPopup);
    }
    SECTION("spellPopup")
    {
        CREATEWIDGETV0(spellPopup, SpellPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(spellPopup);
    }
    SECTION("StatusPopup")
    {
        StatusPopup *status = CREATEWIDGETR0(StatusPopup);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(status);
    }
    SECTION("desktop")
    {
        CREATEWIDGETV(desktop, Desktop, nullptr);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(desktop);
    }
    SECTION("connectionDialog")
    {
        ConnectionDialog *connectionDialog = CREATEWIDGETR(ConnectionDialog,
            // TRANSLATORS: connection dialog header
            _("Logging in"),
            State::SWITCH_SERVER);
        gui->draw();
        mainGraphics->updateScreen();
        delete2(connectionDialog);
    }

    delete2(localPlayer);
    delete2(userPalette);
    delete2(client);
    delete2(serverFeatures);
    delete2(inventoryHandler);
    delete2(charServerHandler);
    delete2(playerHandler);
    delete2(gui);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    delete2(logger);

//    VirtFs::deinit();
}

TEST_CASE("WindowManager", "create windows")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::deleteInstance();
    ResourceManager::cleanOrphans(true);
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);
    paths.setDefaultValues(getPathsDefaults());
    branding.setValue("onlineServerFile", "test/serverlistplus.xml");
    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    config.setValue("fontSize", 16);
    theme = new Theme;
    Theme::selectSkin();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    const std::string cfgName = settings.configDir +
        "/nonexistserver/config.xml";
    ::remove(cfgName.c_str());

    ConfigManager::initConfiguration();
    getConfigDefaults2(config.getDefaultValues());
    branding.setDefaultValues(getBrandingDefaults());
    ConfigManager::initServerConfig("nonexistserver");

    localPlayer = new LocalPlayer(static_cast<BeingId>(1),
        BeingTypeId_zero);

    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);
    touchManager.init();
    UnitsDb::loadUnits();
    charServerHandler = new EAthena::CharServerHandler;
    serverFeatures = new EAthena::ServerFeatures;
    inventoryHandler = new EAthena::InventoryHandler;
    playerHandler = new EAthena::PlayerHandler;
    paths.setValue("itemIcons", "");

    TranslationManager::init();

    mainGraphics->setVideoMode(640, 480, 1, 8, false, false, false, false);

    SECTION("create windows")
    {
        WindowManager::createWindows();
        WindowManager::deleteWindows();
    }

    SECTION("init")
    {
        WindowManager::init();
    }

    SECTION("initTitle")
    {
        WindowManager::initTitle();
        REQUIRE(settings.windowCaption == strprintf("%s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION));
    }

    SECTION("updateTitle1")
    {
        settings.serverName = std::string();
        settings.login = std::string();
        WindowManager::updateTitle();
        REQUIRE(settings.windowCaption == strprintf("%s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION));
    }

    SECTION("updateTitle2")
    {
        settings.serverName = "server";
        settings.login = std::string();
        WindowManager::updateTitle();
        REQUIRE(settings.windowCaption == strprintf("%s %s - %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION,
            settings.serverName.c_str()));
    }

    SECTION("updateTitle3")
    {
        settings.serverName = "server";
        settings.login = "login";
        WindowManager::updateTitle();
        REQUIRE(settings.windowCaption == strprintf("%s %s - %s %s",
            branding.getStringValue("appName").c_str(),
            SMALL_VERSION,
            settings.login.c_str(),
            settings.serverName.c_str()));
    }

    SECTION("setFramerate1")
    {
        settings.limitFps = true;
        WindowManager::setFramerate(60);
        REQUIRE(WindowManager::getFramerate() == 60);
        WindowManager::setFramerate(10);
        REQUIRE(WindowManager::getFramerate() == 10);
        WindowManager::setFramerate(0);
        REQUIRE(WindowManager::getFramerate() == 10);
    }

    SECTION("setFramerate2")
    {
        settings.limitFps = false;
        WindowManager::setFramerate(60);
        REQUIRE(WindowManager::getFramerate() == 0);
        WindowManager::setFramerate(10);
        REQUIRE(WindowManager::getFramerate() == 0);
        WindowManager::setFramerate(0);
        REQUIRE(WindowManager::getFramerate() == 0);
    }

    settings.serverName = std::string();
    settings.login = std::string();
    settings.limitFps = true;

    delete2(localPlayer);
    delete2(client);
    delete2(serverFeatures);
    delete2(inventoryHandler);
    delete2(charServerHandler);
    delete2(playerHandler);
    delete2(gui);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    delete2(logger);

//    VirtFs::deinit();
}

TEST_CASE("windows leak test2", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}
