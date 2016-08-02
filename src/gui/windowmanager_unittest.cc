/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2016  The ManaPlus Developers
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

#include "catch.hpp"
#include "client.h"
#include "configuration.h"
#include "graphicsmanager.h"
#include "settings.h"
#include "units.h"
#include "textcommand.h"

#include "being/being.h"

#include "gui/gui.h"
#include "gui/mailmessage.h"
#include "gui/userpalette.h"

#include "gui/popups/beingpopup.h"
#include "gui/popups/itempopup.h"
#include "gui/popups/popupmenu.h"
#include "gui/popups/spellpopup.h"
#include "gui/popups/textboxpopup.h"
#include "gui/popups/textpopup.h"

#include "gui/widgets/desktop.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/windowcontainer.h"

#include "gui/windows/bankwindow.h"
#include "gui/windows/buydialog.h"
#include "gui/windows/buyingstoreselldialog.h"
#include "gui/windows/buyselldialog.h"
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

#include "net/eathena/inventoryhandler.h"
#include "net/eathena/serverfeatures.h"
#include "net/eathena/playerhandler.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "resources/item/item.h"

#include "resources/map/map.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/gettext.h"
#include "utils/physfstools.h"

#include "utils/translation/translationmanager.h"

#include "debug.h"

extern QuitDialog *quitDialog;

TEST_CASE("Windows tests", "windowmanager")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);
    branding.setValue("onlineServerFile", "test/serverlistplus.xml");
    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;
    userPalette = new UserPalette;
    theme = new Theme;
    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);
    touchManager.init();
    Units::loadUnits();
    serverFeatures = new EAthena::ServerFeatures;
    inventoryHandler = new EAthena::InventoryHandler;
    playerHandler = new EAthena::PlayerHandler;
    paths.setValue("itemIcons", "");
    TranslationManager::init();

    mainGraphics->setVideoMode(640, 480, 1, 8, false, false, false, false);

    SECTION("bankWindow")
    {
        CREATEWIDGETV0(bankWindow, BankWindow);
        delete2(bankWindow);
    }
    SECTION("buyDialog")
    {
        BuyDialog *dialog;
        CREATEWIDGETV(dialog, BuyDialog, BeingId_zero);
        delete2(dialog);
    }
    SECTION("BuyingStoreSellDialog")
    {
        BuyingStoreSellDialog *dialog;
        CREATEWIDGETV(dialog, BuyingStoreSellDialog,
            BeingId_zero, 0);
        delete2(dialog);
    }
    SECTION("BuySellDialog")
    {
        BuySellDialog *dialog;
        CREATEWIDGETV(dialog, BuySellDialog, BeingId_zero);
        delete2(dialog);
    }
    SECTION("ChangeEmailDialog")
    {
        LoginData data;
        ChangeEmailDialog *dialog;
        CREATEWIDGETV(dialog, ChangeEmailDialog, data);
        delete2(dialog);
    }
    SECTION("ChangePasswordDialog")
    {
        LoginData data;
        ChangePasswordDialog *dialog;
        CREATEWIDGETV(dialog, ChangePasswordDialog, data);
        delete2(dialog);
    }
/*
    SECTION("CharSelectDialog")
    {
        LoginData data;
        CharSelectDialog *dialog;
        CREATEWIDGETV(dialog, CharSelectDialog, data);
    }
    SECTION("CharCreateDialog")
    {
        LoginData data;
        CharSelectDialog *dialog2;
        CREATEWIDGETV(dialog2, CharSelectDialog, data);
        CharCreateDialog *dialog;
        CREATEWIDGETV(dialog, CharCreateDialog, dialog2, 0);
        delete2(dialog);
        delete2(dialog2);
    }
*/
    SECTION("ChatWindow")
    {
        CREATEWIDGETV0(chatWindow, ChatWindow);
        delete2(chatWindow);
    }
    SECTION("ConfirmDialog")
    {
        ConfirmDialog *dialog;
        CREATEWIDGETV(dialog, ConfirmDialog,
            "", "", "", false, Modal_false, nullptr);
        delete2(dialog);
    }
    SECTION("CutinWindow")
    {
        CREATEWIDGETV0(cutInWindow, CutInWindow);
        delete2(cutInWindow);
    }
    SECTION("DebugWindow")
    {
        CREATEWIDGETV0(debugWindow, DebugWindow);
        delete2(debugWindow);
    }
    SECTION("didYouKnowWindow")
    {
        CREATEWIDGETV0(didYouKnowWindow, DidYouKnowWindow);
        delete2(didYouKnowWindow);
    }
    SECTION("EditDialog")
    {
        EditDialog *dialog;
        CREATEWIDGETV(dialog, EditDialog,
            "", "", "", 100, nullptr, Modal_false);
        delete2(dialog);
    }
    SECTION("EditServerDialog")
    {
        ServerInfo mCurrentServer;
        settings.configDir = PhysFs::getRealDir("test/serverlistplus.xml");
        ServerDialog *serverDialog = CREATEWIDGETR(ServerDialog,
            &mCurrentServer,
            settings.configDir);
        EditServerDialog *editServerDialog = CREATEWIDGETR(EditServerDialog,
            serverDialog, mCurrentServer, 0);
        delete2(editServerDialog);
        delete2(serverDialog);
    }
    SECTION("EggSelectionDialog")
    {
        EggSelectionDialog *dialog = CREATEWIDGETR0(EggSelectionDialog);
        delete2(dialog);
    }
    SECTION("EmoteWindow")
    {
        EmoteWindow *dialog = CREATEWIDGETR0(EmoteWindow);
        delete2(dialog);
    }
    SECTION("EquipmentWindow")
    {
        Equipment *equipment = new Equipment;
        Map *map = new Map("test", 100, 100, 32, 32);
        Being *being = new Being(BeingId_zero,
            ActorType::Player,
            BeingTypeId_zero,
            map);
        EquipmentWindow *dialog = CREATEWIDGETR(EquipmentWindow,
            equipment, being, false);
        delete2(dialog);
        delete2(being);
        delete2(map);
        delete2(equipment);
    }
    SECTION("helpWindow")
    {
        CREATEWIDGETV0(helpWindow, HelpWindow);
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
        delete2(dialog);
        delete2(item);
    }
    SECTION("InventoryWindow")
    {
        Inventory *inventory = new Inventory(InventoryType::Inventory);
        InventoryWindow *dialog = CREATEWIDGETR(InventoryWindow,
            inventory);
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
        delete2(dialog);
        delete2(item);
    }
    SECTION("KillStats")
    {
        CREATEWIDGETV0(killStats, KillStats);
        delete2(killStats);
    }
    SECTION("loginDialog")
    {
        ServerInfo mCurrentServer;
        LoginDialog *loginDialog = CREATEWIDGETR(LoginDialog,
            loginData,
            &mCurrentServer,
            &settings.options.updateHost);
        delete2(loginDialog);
    }
    SECTION("MailEditWindow")
    {
        CREATEWIDGETV0(mailEditWindow, MailEditWindow);
        delete2(mailEditWindow);
    }
    SECTION("MailViewWindow")
    {
        MailMessage *message = new MailMessage;
        CREATEWIDGETV(mailViewWindow, MailViewWindow,
            message);
        delete2(mailViewWindow);
    }
    SECTION("MailWindow")
    {
        CREATEWIDGETV0(mailWindow, MailWindow);
        delete2(mailWindow);
    }
    SECTION("Minimap")
    {
        CREATEWIDGETV0(minimap, Minimap);
        delete2(minimap);
    }
    SECTION("MiniStatusWindow")
    {
        CREATEWIDGETV0(miniStatusWindow, MiniStatusWindow);
        delete2(miniStatusWindow);
    }
    SECTION("NpcDialog")
    {
        NpcDialog *dialog = CREATEWIDGETR(NpcDialog, BeingId_zero);
        delete2(dialog);
    }
    SECTION("NpcSellDialog")
    {
        NpcSellDialog *dialog = CREATEWIDGETR(NpcSellDialog, BeingId_zero);
        delete2(dialog);
    }
    SECTION("OkDialog")
    {
        OkDialog *dialog = CREATEWIDGETR(OkDialog,
            "", "", "", DialogType::SILENCE, Modal_false,
            ShowCenter_true, nullptr, 100);
        delete2(dialog);
    }
    SECTION("OutfitWindow")
    {
        CREATEWIDGETV0(outfitWindow, OutfitWindow);
        delete2(outfitWindow);
    }
    SECTION("QuestsWindow")
    {
        CREATEWIDGETV0(questsWindow, QuestsWindow);
        delete2(questsWindow);
    }
    SECTION("QuitDialog")
    {
        CREATEWIDGETV(quitDialog, QuitDialog,
            &quitDialog);
        delete2(quitDialog);
    }
    SECTION("RegisterDialog")
    {
        RegisterDialog *dialog = CREATEWIDGETR(RegisterDialog,
            loginData);
        delete2(dialog);
    }
    SECTION("serversDialog")
    {
        ServerInfo mCurrentServer;
        settings.configDir = PhysFs::getRealDir("test/serverlistplus.xml");
        ServerDialog *serverDialog = CREATEWIDGETR(ServerDialog,
            &mCurrentServer,
            settings.configDir);
        delete2(serverDialog);
    }
    SECTION("setupWindow")
    {
        CREATEWIDGETV0(setupWindow, SetupWindow);
        delete2(setupWindow);
    }
    SECTION("ShopSellDialog")
    {
        // only tmwa skipping
    }
    SECTION("ShopWindow")
    {
        CREATEWIDGETV0(shopWindow, ShopWindow);
        delete2(shopWindow);
    }
    SECTION("ShortcutWindow")
    {
        CREATEWIDGETV(itemShortcutWindow, ShortcutWindow, "");
        delete2(itemShortcutWindow);
    }
    SECTION("SkillDialog")
    {
        CREATEWIDGETV0(skillDialog, SkillDialog);
        delete2(skillDialog);
    }
    SECTION("SocialWindow")
    {
        CREATEWIDGETV0(socialWindow, SocialWindow);
        delete2(socialWindow);
    }
    SECTION("StatusWindow")
    {
        CREATEWIDGETV0(statusWindow, StatusWindow);
        delete2(statusWindow);
    }
    SECTION("TextCommandEditor")
    {
        TextCommand *textCommand = new TextCommand(1, "", "", "",
            CommandTarget::NoTarget, "");
        TextCommandEditor *dialog = CREATEWIDGETR(TextCommandEditor,
            textCommand);
        delete2(dialog);
    }
    SECTION("TextDialog")
    {
        TextDialog *dialog = CREATEWIDGETR(TextDialog,
            "", "", nullptr, false);
        delete2(dialog);
    }
    SECTION("TextSelectDialog")
    {
        TextSelectDialog *dialog = CREATEWIDGETR(TextSelectDialog,
            "", "", AllowQuit_false);
        delete2(dialog);
    }
    SECTION("TradeWindow")
    {
        CREATEWIDGETV0(tradeWindow, TradeWindow);
        delete2(tradeWindow);
    }
    SECTION("UpdaterWindow")
    {
        CREATEWIDGETV(updaterWindow, UpdaterWindow,
            "", "", false, UpdateType::Skip);
        delete2(updaterWindow);
    }
    SECTION("WhoIsOnline")
    {
        CREATEWIDGETV0(whoIsOnline, WhoIsOnline);
        delete2(whoIsOnline);
    }
    SECTION("WorldSelectDialog")
    {
        Worlds worlds;
        WorldSelectDialog *dialog = CREATEWIDGETR(WorldSelectDialog,
            worlds);
        delete2(dialog);
    }
    SECTION("popupMenu")
    {
        CREATEWIDGETV0(popupMenu, PopupMenu);
        delete2(popupMenu);
    }
    SECTION("beingPopup")
    {
        CREATEWIDGETV0(beingPopup, BeingPopup);
        delete2(beingPopup);
    }
    SECTION("textPopup")
    {
        CREATEWIDGETV0(textPopup, TextPopup);
        delete2(textPopup);
    }
    SECTION("textBoxPopup")
    {
        CREATEWIDGETV0(textBoxPopup, TextBoxPopup);
        delete2(textBoxPopup);
    }
    SECTION("itemPopup")
    {
        CREATEWIDGETV0(itemPopup, ItemPopup);
        delete2(itemPopup);
    }
    SECTION("spellPopup")
    {
        CREATEWIDGETV0(spellPopup, SpellPopup);
        delete2(spellPopup);
    }
    SECTION("desktop")
    {
        CREATEWIDGETV(desktop, Desktop, nullptr);
        delete2(desktop);
    }
    SECTION("connectionDialog")
    {
        ConnectionDialog *connectionDialog = CREATEWIDGETR(ConnectionDialog,
            // TRANSLATORS: connection dialog header
            _("Logging in"),
            State::SWITCH_SERVER);
        delete2(connectionDialog);
    }

    gui->draw();
    mainGraphics->updateScreen();

    delete2(userPalette);
    delete2(client);
    delete2(serverFeatures);
    delete2(inventoryHandler);
    windowContainer = nullptr;
}
