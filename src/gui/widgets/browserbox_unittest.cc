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

#include "catch.hpp"
#include "client.h"
#include "configuration.h"
#include "configmanager.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/browserbox.h"

#include "utils/delete2.h"

#include "render/sdlgraphics.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

extern const char *dirSeparator;

TEST_CASE("BrowserBox tests", "browserbox")
{
    client = new Client;
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    theme = new Theme;
    Theme::selectSkin();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();
    getConfigDefaults2(config.getDefaultValues());

    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);

    Widget::setGlobalFont(new Font("/usr/share/fonts/truetype/"
        "ttf-dejavu/DejaVuSans-Oblique.ttf", 18));
    BrowserBox *const box = new BrowserBox(nullptr,
        BrowserBoxMode::AUTO_WRAP,
        Opaque_true,
        "");
    box->setWidth(100);
    std::string row = "test";
    box->addRow(row);
    REQUIRE(box->hasRows() == true);
    box->clearRows();
    row = "@@";
    box->addRow(row);
    row = "@@|";
    box->addRow(row);
    row = "|@@";
    box->addRow(row);
    row = "@@|@@";
    box->addRow(row);
    row = "|@@@@";
    box->addRow(row);
    row = "@@11|22@@";
    box->addRow(row);
    row = "##@@11|22@@";
    box->addRow(row);
    row = "@@##|22@@";
    box->addRow(row);
    row = "@@11|##22@@";
    box->addRow(row);
    row = "@@11|22##@@";
    box->addRow(row);
    row = "@@11|22@@##";
    box->addRow(row);
    row = "<##@@11|22@@";
    box->addRow(row);
    row = "@@<##|22@@";
    box->addRow(row);
    row = "@@11|<##22@@";
    box->addRow(row);
    row = "@@11|22<##@@";
    box->addRow(row);
    row = "@@11|22@@<##";
    box->addRow(row);
    row = "<##11|22@@";
    box->addRow(row);
    row = "<##|22@@";
    box->addRow(row);
    row = "11|<##22@@";
    box->addRow(row);
    row = "11|22<##@@";
    box->addRow(row);
    row = "11|22@@<##";
    box->addRow(row);
    row = "##>@@11|22@@";
    box->addRow(row);
    row = "@@##>|22@@";
    box->addRow(row);
    row = "@@11|##>22@@";
    box->addRow(row);
    row = "@@11|22##>@@";
    box->addRow(row);
    row = "@@11|22@@##>";
    box->addRow(row);
    row = "<##11|22##>";
    box->addRow(row);
    row = "<##|22##>";
    box->addRow(row);
    row = "11|<##22##>";
    box->addRow(row);
    row = "11|22<####>";
    box->addRow(row);
    row = "11|22##><##";
    row = "%%@@11|22@@";
    box->addRow(row);
    row = "%%2@@11|22@@";
    box->addRow(row);
    row = "<%%11|22@@";
    box->addRow(row);
    row = "@@%%>|22@@";
    box->addRow(row);
    row = "<%%|22%%>";
    box->addRow(row);
    row = "11|22<%%%%>";
    box->addRow(row);
    row = "%%";
    box->addRow(row);
    row = "%%1";
    box->addRow(row);
    row = "%%##";
    box->addRow(row);
    row = "%%###";
    box->addRow(row);
    row = "##%%";
    box->addRow(row);
    row = "##1%%";
    box->addRow(row);
    row = "##%%2";
    box->addRow(row);
    row = "##1%%2";
    box->addRow(row);

    delete Widget::getGloablFont();
    Widget::setGlobalFont(nullptr);
    delete box;
    delete2(client);
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}
