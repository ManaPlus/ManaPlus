/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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
#include "configuration.h"
#include "configmanager.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/staticbrowserbox.h"

#include "utils/delete2.h"
#include "utils/env.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

TEST_CASE("StaticBrowserBox tests", "browserbox")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();
    setConfigDefaults2(config);
    setBrandingDefaults(branding);

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);

    Widget::setGlobalFont(new Font(
        "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans-Oblique.ttf",
        18,
        TTF_STYLE_NORMAL));
    StaticBrowserBox *const box = new StaticBrowserBox(nullptr,
        Opaque_true,
        "");
    box->setWidth(100);
    std::string row = "test";
    box->addRow(row, false);
    REQUIRE(box->hasRows() == true);
    box->clearRows();
    row = "@@";
    box->addRow(row, false);
    row = "@@|";
    box->addRow(row, false);
    row = "|@@";
    box->addRow(row, false);
    row = "@@|@@";
    box->addRow(row, false);
    row = "|@@@@";
    box->addRow(row, false);
    row = "@@11|22@@";
    box->addRow(row, false);
    row = "##@@11|22@@";
    box->addRow(row, false);
    row = "@@##|22@@";
    box->addRow(row, false);
    row = "@@11|##22@@";
    box->addRow(row, false);
    row = "@@11|22##@@";
    box->addRow(row, false);
    row = "@@11|22@@##";
    box->addRow(row, false);
    row = "<##@@11|22@@";
    box->addRow(row, false);
    row = "@@<##|22@@";
    box->addRow(row, false);
    row = "@@11|<##22@@";
    box->addRow(row, false);
    row = "@@11|22<##@@";
    box->addRow(row, false);
    row = "@@11|22@@<##";
    box->addRow(row, false);
    row = "<##11|22@@";
    box->addRow(row, false);
    row = "<##|22@@";
    box->addRow(row, false);
    row = "11|<##22@@";
    box->addRow(row, false);
    row = "11|22<##@@";
    box->addRow(row, false);
    row = "11|22@@<##";
    box->addRow(row, false);
    row = "##>@@11|22@@";
    box->addRow(row, false);
    row = "@@##>|22@@";
    box->addRow(row, false);
    row = "@@11|##>22@@";
    box->addRow(row, false);
    row = "@@11|22##>@@";
    box->addRow(row, false);
    row = "@@11|22@@##>";
    box->addRow(row, false);
    row = "<##11|22##>";
    box->addRow(row, false);
    row = "<##|22##>";
    box->addRow(row, false);
    row = "11|<##22##>";
    box->addRow(row, false);
    row = "11|22<####>";
    box->addRow(row, false);
    row = "11|22##><##";
    box->addRow(row, false);
    row = "%%@@11|22@@";
    box->addRow(row, false);
    row = "%%2@@11|22@@";
    box->addRow(row, false);
    row = "<%%11|22@@";
    box->addRow(row, false);
    row = "@@%%>|22@@";
    box->addRow(row, false);
    row = "<%%|22%%>";
    box->addRow(row, false);
    row = "11|22<%%%%>";
    box->addRow(row, false);
    row = "%%";
    box->addRow(row, false);
    row = "%%1";
    box->addRow(row, false);
    row = "%%##";
    box->addRow(row, false);
    row = "%%###";
    box->addRow(row, false);
    row = "##%%";
    box->addRow(row, false);
    row = "##1%%";
    box->addRow(row, false);
    row = "##%%2";
    box->addRow(row, false);
    row = "##1%%2";
    box->addRow(row, false);

    delete Widget::getGloablFont();
    Widget::setGlobalFont(nullptr);
    delete box;
    delete2(client);
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
