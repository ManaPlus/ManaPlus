/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef UTILS_XML_LIBXML_INC
#define UTILS_XML_LIBXML_INC

#ifdef ENABLE_LIBXML

#include <libxml/xmlwriter.h>

__XML_XMLWRITER_H__

#define XmlNodePtr xmlNodePtr
#define XmlNodePtrConst xmlNode *const
#define XmlStrEqual(str1, str2) xmlStrEqual(str1, \
    reinterpret_cast<const xmlChar*>(str2))
#define xmlNameEqual(node, str) xmlStrEqual((node)->name, \
    reinterpret_cast<const xmlChar*>(str))
#define XmlTextWriterPtr xmlTextWriterPtr
#define xmlTypeEqual(node, typ) ((node)->type == (typ))
#define XmlHasProp(node, name) (xmlHasProp(node, \
    reinterpret_cast<const xmlChar*>(name)) != nullptr)
#define XmlGetProp(node, name) xmlGetProp(node, \
    reinterpret_cast<const xmlChar*>(name))
#define XmlTextWriterStartElement(writer, name) \
    xmlTextWriterStartElement(writer, reinterpret_cast<const xmlChar*>(name))
#define XmlTextWriterEndElement(writer) xmlTextWriterEndElement(writer)
#define XmlTextWriterWriteAttribute(writer, name, content) \
    xmlTextWriterWriteAttribute(writer, \
    reinterpret_cast<const xmlChar*>(name), \
    reinterpret_cast<const xmlChar*>(content))
#define XmlNodeGetContent(node) xmlNodeGetContent(node)
#define XmlNewTextWriterFilename(name, flags) \
    xmlNewTextWriterFilename(name, flags)
#define XmlTextWriterSetIndent(writer, flags) \
    xmlTextWriterSetIndent(writer, flags)
#define XmlTextWriterStartDocument(writer, p1, p2, p3) \
    xmlTextWriterStartDocument(writer, p1, p2, p3)
#define XmlTextWriterEndDocument(writer) xmlTextWriterEndDocument(writer)
#define XmlFreeTextWriter(writer) xmlFreeTextWriter(writer)
#define XmlHaveChildContent(node) ((node)->xmlChildrenNode != nullptr && \
    (node)->xmlChildrenNode->content != nullptr)
#define XmlChildContent(node) reinterpret_cast<const char*>(\
    (node)->xmlChildrenNode->content)
#define XmlFree(ptr) xmlFree(ptr)
#define XmlNodeDefault nullptr

#endif  // ENABLE_LIBXML
#endif  // UTILS_XML_LIBXML_INC
