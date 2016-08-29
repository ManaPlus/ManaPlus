/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#include "being/actorsprite.h"

#include "gui/fonts/font.h"

#include "gui/widgets/browserbox.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include <physfs.h>

#include "debug.h"

extern const char *dirSeparator;

TEST_CASE("BrowserBox tests", "browserbox")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    client = new Client;
    logger = new Logger();
    imageHelper = new SDLImageHelper();
    theme = new Theme;
    ResourceManager::init();
    ActorSprite::load();
    Widget::setGlobalFont(new Font("/usr/share/fonts/truetype/"
        "ttf-dejavu/DejaVuSans-Oblique.ttf", 18));
    BrowserBox *const box = new BrowserBox(nullptr,
        BrowserBox::AUTO_WRAP,
        true,
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

    delete client;
    client = nullptr;
}
