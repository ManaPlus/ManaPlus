/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "animatedsprite.h"

#include "catch.hpp"
#include "client.h"
#include "settings.h"

#include "gui/gui.h"
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

#include "gui/windows/connectiondialog.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/connectiondialog.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/logindialog.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/serverdialog.h"

#include "render/sdlgraphics.h"

#include "resources/resourcemanager.h"
#include "resources/sdlimagehelper.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/gettext.h"
#include "utils/physfstools.h"

#include "debug.h"

TEST_CASE("Windows tests", "windowmanager")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    resourceManager->addToSearchPath("data", false);
    resourceManager->addToSearchPath("../data", false);
    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;
    userPalette = new UserPalette;
    theme = new Theme;
    gui = new Gui();
    gui->postInit(mainGraphics);

    SDL_SetVideoMode(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);

    SECTION("setupWindow")
    {
        CREATEWIDGETV0(setupWindow, SetupWindow);
        delete2(setupWindow);
    }
    SECTION("helpWindow")
    {
        CREATEWIDGETV0(helpWindow, HelpWindow);
        delete2(helpWindow);
    }
    SECTION("didYouKnowWindow")
    {
        CREATEWIDGETV0(didYouKnowWindow, DidYouKnowWindow);
        delete2(didYouKnowWindow);
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
    SECTION("serversDialog")
    {
        ServerInfo mCurrentServer;
        ServerDialog *serverDialog = CREATEWIDGETR(ServerDialog,
            &mCurrentServer,
            settings.configDir);
        delete2(serverDialog);
    }
    SECTION("connectionDialog")
    {
        ConnectionDialog *connectionDialog = CREATEWIDGETR(ConnectionDialog,
            // TRANSLATORS: connection dialog header
            _("Connecting to server"),
            STATE_SWITCH_SERVER);
        delete2(connectionDialog);
    }
    SECTION("loginDialog")
    {
        ServerInfo mCurrentServer;
        LoginDialog *loginDialog = CREATEWIDGETR(LoginDialog,
            loginData,
            mCurrentServer.hostname,
            &settings.options.updateHost);
        delete2(loginDialog);
    }
    SECTION("connectionDialog")
    {
        ConnectionDialog *connectionDialog = CREATEWIDGETR(ConnectionDialog,
            // TRANSLATORS: connection dialog header
            _("Logging in"),
            STATE_SWITCH_SERVER);
        delete2(connectionDialog);
    }

    delete2(userPalette);
    delete2(client);
    windowContainer = nullptr;
}
