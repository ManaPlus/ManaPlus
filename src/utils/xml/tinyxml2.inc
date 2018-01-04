/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef UTILS_XML_TINYXML2_INC
#define UTILS_XML_TINYXML2_INC

#ifdef ENABLE_TINYXML2

PRAGMA49(GCC diagnostic push)
PRAGMA49(GCC diagnostic ignored "-Wzero-as-null-pointer-constant")
PRAGMA49(GCC diagnostic ignored "-Wsuggest-override")
PRAGMACLANG6GCC(GCC diagnostic push)
PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
#include <tinyxml2.h>
PRAGMACLANG6GCC(GCC diagnostic pop)
PRAGMA49(GCC diagnostic pop)

TINYXML2_INCLUDED

#define XML_ELEMENT_NODE tinyxml2::XMLElement

#define XmlNodePtr const tinyxml2::XMLElement*
#define XmlNodePtrConst const tinyxml2::XMLElement *const
#define XmlNodeConstPtr const tinyxml2::XMLElement*
#define XmlNodeConstPtrConst const tinyxml2::XMLElement *const
#define xmlNameEqual(node, str) !strcmp((node)->Value(), str)
#define XmlTextWriterPtr tinyxml2::XMLPrinter*
// +++ need replace xmlTypeEqual to isXmlElementNode
#define xmlTypeEqual(node, typ) true
#define XmlHasProp(node, name) ((node)->Attribute(name) != nullptr)

#define XmlNodeGetContent(node) (node)->GetText()
#define XmlHaveChildContent(node) ((node)->GetText() != nullptr)
#define XmlChildContent(node) ((node)->GetText())

#define XmlFree(ptr)
#define XmlNodeDefault nullptr
#define XmlChar const char
#define XmlConstChar const char

#define XmlTextWriterStartElement(writer, name) (writer)->OpenElement(name)
#define XmlTextWriterStartRootElement(writer, name) (writer)->OpenElement(name)
#define XmlTextWriterEndElement(writer) (writer)->CloseElement()
#define XmlTextWriterWriteAttribute(writer, name, content) \
    (writer)->PushAttribute(name, content)
#define XmlNewTextWriterFilename(name, flags) new tinyxml2::XMLPrinter
#define XmlSaveTextWriterFilename(writer, name) \
    { \
        FILE *const writer##File = fopen(name, "wb"); \
        fwrite((writer)->CStr(), 1, (writer)->CStrSize() - 1, writer##File); \
        fclose(writer##File); \
    }
#define XmlTextWriterSetIndent(writer, flags)
#define XmlTextWriterStartDocument(writer, p1, p2, p3) \
    (writer)->PushDeclaration("xml version=\"1.0\" encoding=\"utf-8\"")
#define XmlTextWriterEndDocument(writer) (writer)->CloseElement()
#define XmlFreeTextWriter(writer) delete writer

#endif  // ENABLE_TINYXML2
#endif  // UTILS_XML_TINYXML2_INC
