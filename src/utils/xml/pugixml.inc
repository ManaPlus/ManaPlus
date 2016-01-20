/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef UTILS_XML_PUGIXML_INC
#define UTILS_XML_PUGIXML_INC

#ifdef ENABLE_PUGIXML

#define XML_ELEMENT_NODE pugi::node_element

#define XmlNodePtr pugi::xml_node
#define XmlNodePtrConst pugi::xml_node 
#define xmlNameEqual(node, str) !strcmp((node).name(), str)
#define xmlTypeEqual(node, typ) ((node).type() == (typ))
#define XmlHasProp(node, name) (!((node).attribute(name).empty()))
#define XmlHaveChildContent(node) ((node).child_value() != nullptr && \
    *(node).child_value())
#define XmlChildContent(node) (node).child_value()
#define xmlChar char
#define XmlFree(ptr)
#define XmlNodeDefault pugi::xml_node()

// +++ need impliment get context 
#define XmlNodeGetContent(node) (node).child_value()

// +++ need impliment writing code
#define XmlTextWriterPtr pugi::xml_writer*
#define XmlTextWriterStartElement(writer, name)
#define XmlTextWriterEndElement(writer)
#define XmlTextWriterWriteAttribute(writer, name, content)
#define XmlNewTextWriterFilename(name, flags) nullptr;
#define XmlTextWriterSetIndent(writer, flags)
#define XmlTextWriterStartDocument(writer, p1, p2, p3)
#define XmlTextWriterEndDocument(writer)
#define XmlFreeTextWriter(writer)

#endif  // ENABLE_PUGIXML
#endif  // UTILS_XML_PUGIXML_INC
