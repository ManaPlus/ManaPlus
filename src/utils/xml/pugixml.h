/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef UTILS_XML_PUGIXML_H
#define UTILS_XML_PUGIXML_H

#ifdef ENABLE_PUGIXML

#include "enums/simpletypes/skiperror.h"
#include "enums/simpletypes/useresman.h"

#include <pugixml.hpp>

#include <string>

#include "localconsts.h"

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

/**
 * XML helper functions.
 */
namespace XML
{
    /**
     * A helper class for parsing an XML document, which also cleans it up
     * again (RAII).
     */
    class Document final
    {
        public:
            /**
             * Constructor that attempts to load the given file through the
             * resource manager. Logs errors.
             */
            Document(const std::string &filename,
                     const UseResman useResman,
                     const SkipError skipError);

            /**
             * Constructor that attempts to load an XML document from memory.
             * Does not log errors.
             *
             * @param data the string to parse as XML
             * @param size the length of the string in bytes
             */
            Document(const char *const data, const int size);

            A_DELETE_COPY(Document)

            /**
             * Destructor. Frees the loaded XML file.
             */
            ~Document();

            /**
             * Returns the root node of the document (or NULL if there was a
             * load error).
             */
            XmlNodePtr rootNode() A_WARN_UNUSED;

            bool isLoaded() const
            { return !mDoc.empty(); }

            bool isValid() const
            { return mIsValid; }

            static bool validateXml(const std::string &fileName);

        private:
            pugi::xml_document mDoc;
            pugi::xml_node mRoot;
            char *mData;
            bool mIsValid;
    };

    /**
     * Gets an floating point property from an XmlNodePtr.
     */
    double getFloatProperty(const XmlNodePtr node,
                            const char *const name,
                            double def) A_WARN_UNUSED;

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getProperty(const XmlNodePtr node,
                    const char *const name,
                    int def) A_WARN_UNUSED;

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getIntProperty(const XmlNodePtr node,
                       const char *const name,
                       int def,
                       const int min,
                       const int max) A_WARN_UNUSED;

    /**
     * Gets a string property from an XmlNodePtr.
     */
    std::string getProperty(const XmlNodePtr node,
                            const char *const name,
                            const std::string &def) A_WARN_UNUSED;

    /**
     * Gets a translated string property from an XmlNodePtr.
     */
    std::string langProperty(const XmlNodePtr node,
                             const char *const name,
                             const std::string &def) A_WARN_UNUSED;

    /**
     * Gets a boolean property from an XmlNodePtr.
     */
    bool getBoolProperty(const XmlNodePtr node,
                         const char *const name,
                         const bool def) A_WARN_UNUSED;

    /**
     * Finds the first child node with the given name
     */
    XmlNodePtr findFirstChildByName(const XmlNodePtrConst parent,
                                    const char *const name) A_WARN_UNUSED;

    void initXML();

    void cleanupXML();
}  // namespace XML

#define for_each_xml_child_node(var, parent) \
    for (pugi::xml_node var = parent.first_child(); var; var = var.next_sibling())

#endif  // ENABLE_PUGIXML
#endif  // UTILS_XML_PUGIXML_H
