/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/userpalette.h"
#include "gui/theme.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/foreach.h"
#include "utils/xml.h"
#include "utils/itemxmlutils.h"

#include "utils/translation/translationmanager.h"

#include "render/sdlgraphics.h"

#include "resources/item/itemfieldtype.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

TEST_CASE("itemxmlutils readItemStatsString 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();

    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    Dirs::initRootDir();
    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    TranslationManager::init();

    std::string effect;
    ItemFieldInfos fields;
    fields["hp"] = new ItemFieldType(
        "hp",
        "HP %s",
        true);
    fields["mp"] = new ItemFieldType(
        "mp",
        "MP %s",
        true);
    fields["str"] = new ItemFieldType(
        "str",
        "Str %s",
        true);
    fields["weight"] = new ItemFieldType(
        "weight",
        "Weight %s",
        false);
    XML::Document doc("itemxmlutils_test.xml", UseVirtFs_true, SkipError_false);
    XmlNodeConstPtrConst root = doc.rootNode();

    SECTION("empty1")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "empty1");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect.empty());
    }

    SECTION("empty2")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "empty2");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect.empty());
    }

    SECTION("test1")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test1");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "HP +20");
    }

    SECTION("test2")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test2");
        readItemStatsString(effect, node, fields);
        if (effect[0] == 'H')
        {
            REQUIRE(effect == "HP +10 / Weight 2");
        }
        else
        {
            REQUIRE(effect == "Weight 2 / HP +10");
        }
    }

    SECTION("test3")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test3");
        readItemStatsString(effect, node, fields);
        if (effect[0] == 'H')
        {
            if (effect[9] == 'S')
                REQUIRE(effect == "HP +10 / Str +1 / Weight 2");
            else
                REQUIRE(effect == "HP +10 / Weight 2 / Str +1");
        }
        else if (effect[0] == 'W')
        {
            if (effect[11] == 'H')
                REQUIRE(effect == "Weight 2 / HP +10 / Str +1");
            else
                REQUIRE(effect == "Weight 2 / Str +1 / HP +10");
        }
        else
        {
            if (effect[9] == 'W')
                REQUIRE(effect == "Str +1 / Weight 2 / HP +10");
            else
                REQUIRE(effect == "Str +1 / HP +10 / Weight 2");
        }
    }

    SECTION("test4")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test4");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "HP +10 - +15");
    }

    SECTION("test5")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test5");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "Weight 2");
    }

    SECTION("test6")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test6");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "Weight 2 - 3");
    }

    SECTION("test7")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test7");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "Weight 2-");
    }

    SECTION("test8")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test8");
        readItemStatsString(effect, node, fields);
        REQUIRE(effect == "Weight -2");
    }

    FOR_EACH(ItemFieldInfos::iterator, it, fields)
    {
        delete (*it).second;
    }
    delete2(userPalette)
    delete2(theme)
    delete2(client)
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("itemxmlutils readItemStatsVector 1", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();

    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    Dirs::initRootDir();
    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    TranslationManager::init();

    STD_VECTOR<std::string> effect;
    ItemFieldInfos fields;
    fields["hp"] = new ItemFieldType(
        "hp",
        "HP %s",
        true);
    fields["mp"] = new ItemFieldType(
        "mp",
        "MP %s",
        true);
    fields["str"] = new ItemFieldType(
        "str",
        "Str %s",
        true);
    fields["weight"] = new ItemFieldType(
        "weight",
        "Weight %s",
        false);
    XML::Document doc("itemxmlutils_test.xml", UseVirtFs_true, SkipError_false);
    XmlNodeConstPtrConst root = doc.rootNode();

    SECTION("empty1")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "empty1");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.empty());
    }

    SECTION("empty2")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "empty2");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.empty() == true);
    }

    SECTION("test1")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test1");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "HP +20");
    }

    SECTION("test2")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test2");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 2);
        if (effect[0][0] == 'H')
        {
            REQUIRE(effect[0] == "HP +10");
        }
        else
        {
            REQUIRE(effect[1] == "Weight 2");
        }
    }

    SECTION("test3")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test3");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 3);
        if (effect[0][0] == 'H')
        {
            if (effect[1][0] == 'S')
            {
                REQUIRE(effect[0] == "HP +10");
                REQUIRE(effect[1] == "Str +1");
                REQUIRE(effect[2] == "Weight 2");
            }
            else
            {
                REQUIRE(effect[0] == "HP +10");
                REQUIRE(effect[1] == "Weight 2");
                REQUIRE(effect[2] == "Str +1");
            }
        }
        else if (effect[0][0] == 'W')
        {
            if (effect[1][0] == 'H')
            {
                REQUIRE(effect[0] == "Weight 2");
                REQUIRE(effect[1] == "HP +10");
                REQUIRE(effect[2] == "Str +1");
            }
            else
            {
                REQUIRE(effect[0] == "Weight 2");
                REQUIRE(effect[1] == "Str +1");
                REQUIRE(effect[2] == "HP +10");
            }
        }
        else
        {
            if (effect[1][0] == 'W')
            {
                REQUIRE(effect[0] == "Str +1");
                REQUIRE(effect[1] == "Weight 2");
                REQUIRE(effect[2] == "HP +10");
            }
            else
            {
                REQUIRE(effect[0] == "Str +1");
                REQUIRE(effect[1] == "HP +10");
                REQUIRE(effect[2] == "Weight 2");
            }
        }
    }

    SECTION("test4")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test4");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "HP +10 - +15");
    }

    SECTION("test5")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test5");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "Weight 2");
    }

    SECTION("test6")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test6");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "Weight 2 - 3");
    }

    SECTION("test7")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test7");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "Weight 2-");
    }

    SECTION("test8")
    {
        XmlNodePtr node = XML::findFirstChildByName(root, "test8");
        readItemStatsVector(effect, node, fields);
        REQUIRE(effect.size() == 1);
        REQUIRE(effect[0] == "Weight -2");
    }

    FOR_EACH(ItemFieldInfos::iterator, it, fields)
    {
        delete (*it).second;
    }
    delete2(userPalette)
    delete2(theme)
    delete2(client)
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
