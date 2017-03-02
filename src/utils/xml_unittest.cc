/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2017  The ManaPlus Developers
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
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs.h"

#include "gui/gui.h"
#include "gui/theme.h"

#include "utils/delete2.h"
#include "utils/env.h"
#ifdef ENABLE_PUGIXML
#include "utils/xmlwriter.h"
#endif  // ENABLE_PUGIXML

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("xml doc")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    dirSeparator = "/";
    XML::initXML();
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);
    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    theme = new Theme;
    Theme::selectSkin();
    const char *const tempXmlName = "tempxml.xml";
    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);

    SECTION("load1")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseVirtFs_true,
            SkipError_false);
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "skinset") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "skinset123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
//        REQUIRE(XmlHaveChildContent(doc.rootNode()) == true);
    }

    SECTION("load2")
    {
        const char *const xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<root><data option1=\"false\" option2=\"true\"/>"
            "<cont>this is test</cont></root>";
        XML::Document doc(xml, strlen(xml));
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
        REQUIRE(XmlHasProp(doc.rootNode(), "option1") == false);
        REQUIRE(XmlHasProp(doc.rootNode(), "option123") == false);
        REQUIRE(XmlHaveChildContent(doc.rootNode()) == false);
    }

    SECTION("load3")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<!-- comment here\nand here -->"
            "<root><data option1=\"false\" option2=\"true\"/></root>";
        XML::Document doc(xml.c_str(), xml.size());
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
        REQUIRE(XmlHasProp(doc.rootNode(), "option1") == false);
        REQUIRE(XmlHasProp(doc.rootNode(), "option123") == false);
        REQUIRE(XmlHaveChildContent(doc.rootNode()) == false);
    }

    SECTION("load4")
    {
        const char *const xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<root>this is test</root>";
        XML::Document doc(xml, strlen(xml));
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
        REQUIRE(XmlHasProp(doc.rootNode(), "option1") == false);
        REQUIRE(XmlHasProp(doc.rootNode(), "option123") == false);
        REQUIRE(XmlHaveChildContent(doc.rootNode()) == true);
        REQUIRE(!strcmp(XmlChildContent(doc.rootNode()), "this is test"));
    }

    SECTION("properties")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseVirtFs_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
        REQUIRE(XML::getProperty(rootNode, "image", "") == "window.png");
    }

    SECTION("for each")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseVirtFs_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
//        REQUIRE(XmlHaveChildContent(rootNode) == true);
        XmlNodePtr node = XmlNodeDefault;
        for_each_xml_child_node(widgetNode, rootNode)
        {
            node = widgetNode;
            if (xmlNameEqual(node, "widget"))
                break;
        }
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "widget") == true);
//        REQUIRE(XmlHaveChildContent(node) == true);
        for_each_xml_child_node(optionNode, node)
        {
            node = optionNode;
            if (xmlNameEqual(node, "option"))
                break;
        }
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "option") == true);
        REQUIRE(XmlHaveChildContent(node) == false);
        REQUIRE(XmlHasProp(node, "name") == true);
        REQUIRE(XmlHasProp(node, "value") == true);
        REQUIRE(XmlHasProp(node, "option123") == false);
        REQUIRE(XML::getProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name", "") == "padding");
        REQUIRE(XML::getProperty(node, "value", 0) == 1);
        REQUIRE(XML::getBoolProperty(node, "value", true) == true);
        REQUIRE(XML::getBoolProperty(node, "value", false) == false);
        REQUIRE(XML::getIntProperty(node, "value", -1, -10, 100) == 1);
    }

    SECTION("child1")
    {
        XML::Document doc("graphics/gui/browserbox.xml",
            UseVirtFs_true,
            SkipError_false);

        const XmlNodePtr rootNode = doc.rootNode();
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "widget");
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "widget") == true);
//        REQUIRE(XmlHaveChildContent(node) == true);
        node = XML::findFirstChildByName(node, "option");
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "option") == true);
        REQUIRE(XmlHaveChildContent(node) == false);
        REQUIRE(XmlHasProp(node, "name") == true);
        REQUIRE(XmlHasProp(node, "value") == true);
        REQUIRE(XmlHasProp(node, "option123") == false);
        REQUIRE(XML::getProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name", "") == "padding");
        REQUIRE(XML::langProperty(node, "name123", "") == "");
        REQUIRE(XML::getProperty(node, "value", 0) == 1);
        REQUIRE(XML::getProperty(node, "value123", -1) == -1);
        REQUIRE(XML::getBoolProperty(node, "value", true) == true);
        REQUIRE(XML::getBoolProperty(node, "value", false) == false);
        REQUIRE(XML::getBoolProperty(node, "value123", true) == true);
        REQUIRE(XML::getIntProperty(node, "value", -1, -10, 100) == 1);
        REQUIRE(XML::getIntProperty(node, "value123", -1, -10, 100) == -1);
    }

    SECTION("child2")
    {
        const char *const xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<root><data option1=\"false\" option2=\"true\" "
            "option3=\"10.5\"/></root>";
        XML::Document doc(xml, strlen(xml));
        const XmlNodePtr rootNode = doc.rootNode();
        REQUIRE(XmlHaveChildContent(rootNode) == false);
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "data");
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "data") == true);
        REQUIRE(XmlHaveChildContent(node) == false);
        REQUIRE(XmlHasProp(node, "option1") == true);
        REQUIRE(XmlHasProp(node, "option123") == false);
        REQUIRE(XML::getBoolProperty(node, "option1", true) == false);
        REQUIRE(XML::getBoolProperty(node, "option2", false) == true);
        const float opt3 = XML::getFloatProperty(node, "option3", 0.0);
        REQUIRE(opt3 > 10);
        REQUIRE(opt3 < 11);
    }

    SECTION("child3")
    {
        const std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<!-- comment --><root><!-- comment -->"
            "<data option1=\"false\" option2=\"true\" "
            "option3=\"10.5\"/><!-- comment --></root>";
        XML::Document doc(xml.c_str(), xml.size());
        const XmlNodePtr rootNode = doc.rootNode();
//        REQUIRE(XmlHaveChildContent(rootNode) == true);
        XmlNodePtr node = XML::findFirstChildByName(rootNode, "data");
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "data") == true);
        REQUIRE(XmlHaveChildContent(node) == false);
        REQUIRE(XmlHasProp(node, "option1") == true);
        REQUIRE(XmlHasProp(node, "option123") == false);
        REQUIRE(XML::getBoolProperty(node, "option1", true) == false);
        REQUIRE(XML::getBoolProperty(node, "option2", false) == true);
        const float opt3 = XML::getFloatProperty(node, "option3", 0.0);
        REQUIRE(opt3 > 10);
        REQUIRE(opt3 < 11);
    }

    SECTION("validate")
    {
//        REQUIRE(XML::Document::validateXml(
//            "graphics/gui/browserbox.xml") == true);
        REQUIRE(XML::Document::validateXml(
            "graphics/gui/bubble.png") == false);
        REQUIRE(XML::Document::validateXml(
            "graphics/gui/testfile123.xml") == false);
    }

    SECTION("save1")
    {
        // clean
        ::remove(tempXmlName);

        // save
        FILE *const testFile = fopen(tempXmlName, "w");
        REQUIRE(testFile);
        fclose(testFile);
        const XmlTextWriterPtr writer = XmlNewTextWriterFilename(
            tempXmlName,
            0);
        XmlTextWriterSetIndent(writer, 1);
        XmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
        XmlTextWriterStartElement(writer, "root");
        XmlTextWriterEndDocument(writer);
        XmlFreeTextWriter(writer);

        // load
        XML::Document doc(tempXmlName,
            UseVirtFs_false,
            SkipError_false);
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "skinset123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
//        REQUIRE(XmlHaveChildContent(doc.rootNode()) == true);

        // clean again
        ::remove(tempXmlName);
    }

    SECTION("save2")
    {
        // clean
        ::remove(tempXmlName);

        // save
        FILE *const testFile = fopen(tempXmlName, "w");
        REQUIRE(testFile);
        fclose(testFile);
        const XmlTextWriterPtr writer = XmlNewTextWriterFilename(
            tempXmlName,
            0);
        XmlTextWriterSetIndent(writer, 1);
        XmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
        XmlTextWriterStartElement(writer, "root");

        XmlTextWriterStartElement(writer, "option");
        XmlTextWriterWriteAttribute(writer, "name", "the name");
        XmlTextWriterWriteAttribute(writer, "value", "the value");
        XmlTextWriterEndElement(writer);

        XmlTextWriterEndDocument(writer);
        XmlFreeTextWriter(writer);

        // load
        XML::Document doc(tempXmlName,
            UseVirtFs_false,
            SkipError_false);
        REQUIRE(doc.isLoaded() == true);
        REQUIRE(doc.isValid() == true);
        REQUIRE(doc.rootNode() != nullptr);
        REQUIRE(xmlNameEqual(doc.rootNode(), "root") == true);
        REQUIRE(xmlNameEqual(doc.rootNode(), "skinset123") == false);
        REQUIRE(xmlTypeEqual(doc.rootNode(), XML_ELEMENT_NODE) == true);
//        REQUIRE(XmlHaveChildContent(doc.rootNode()) == true);
        XmlNodePtr node = XML::findFirstChildByName(doc.rootNode(), "option");
        REQUIRE(node != nullptr);
        REQUIRE(xmlTypeEqual(node, XML_ELEMENT_NODE) == true);
        REQUIRE(xmlNameEqual(node, "option") == true);
        REQUIRE(XmlHaveChildContent(node) == false);
        REQUIRE(XmlHasProp(node, "name") == true);
        REQUIRE(XmlHasProp(node, "value") == true);
        REQUIRE(XmlHasProp(node, "option123") == false);
        REQUIRE(XML::getProperty(node, "name", "") == "the name");
        REQUIRE(XML::getProperty(node, "value", "") == "the value");

        // clean again
        ::remove(tempXmlName);
    }
    delete2(theme);
    delete2(client);
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");

    delete2(logger);
//    VirtFs::deinit();
}
