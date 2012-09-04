/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef XML_H
#define XML_H

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>

#include <string>

#define XmlNodePtr xmlNodePtr
#define xmlNameEqual(node, str) xmlStrEqual((node)->name, BAD_CAST (str))
#define XmlTextWriterPtr xmlTextWriterPtr
#define xmlTypeEqual(node, typ) ((node)->type == (typ))

/**
 * XML helper functions.
 */
namespace XML
{
    /**
     * A helper class for parsing an XML document, which also cleans it up
     * again (RAII).
     */
    class Document
    {
        public:
            /**
             * Constructor that attempts to load the given file through the
             * resource manager. Logs errors.
             */
            Document(const std::string &filename, const bool useResman = true);

            /**
             * Constructor that attempts to load an XML document from memory.
             * Does not log errors.
             *
             * @param data the string to parse as XML
             * @param size the length of the string in bytes
             */
            Document(const char *const data, const int size);

            /**
             * Destructor. Frees the loaded XML file.
             */
            ~Document();

            /**
             * Returns the root node of the document (or NULL if there was a
             * load error).
             */
            XmlNodePtr rootNode();

        private:
            xmlDocPtr mDoc;
    };

    /**
     * Gets an floating point property from an XmlNodePtr.
     */
    double getFloatProperty(const XmlNodePtr node, const char *const name,
                            double def);

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getProperty(const XmlNodePtr node, const char *const name, int def);

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getIntProperty(const XmlNodePtr node, const char *const name, int def,
                       const int min, const int max);

    /**
     * Gets a string property from an XmlNodePtr.
     */
    std::string getProperty(const XmlNodePtr node, const char *const name,
                            const std::string &def);

    /**
     * Gets a translated string property from an XmlNodePtr.
     */
    std::string langProperty(const XmlNodePtr node, const char *const name,
                             const std::string &def);

    /**
     * Gets a boolean property from an XmlNodePtr.
     */
    bool getBoolProperty(const XmlNodePtr node, const char *const name,
                         const bool def);

    /**
     * Finds the first child node with the given name
     */
    XmlNodePtr findFirstChildByName(const XmlNodePtr parent,
                                    const char *const name);

    void initXML();

    void cleanupXML();
}

#define for_each_xml_child_node(var, parent) \
    for (XmlNodePtr var = parent->xmlChildrenNode; var; var = var->next)

#endif // XML_H
