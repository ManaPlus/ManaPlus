/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef UTILS_XML_PUGIXML_H
#define UTILS_XML_PUGIXML_H

#ifdef ENABLE_PUGIXML

#define XML_INCLUDE_DEFINE

#include "enums/simpletypes/skiperror.h"
#include "enums/simpletypes/usevirtfs.h"

#include "utils/xml/pugixml.inc"

#include "resources/resource.h"

#include <pugixml.hpp>

#include <string>

#include "localconsts.h"

/**
 * XML helper functions.
 */
namespace XML
{
    /**
     * A helper class for parsing an XML document, which also cleans it up
     * again (RAII).
     */
    class Document final : public Resource
    {
        public:
            /**
             * Constructor that attempts to load the given file through the
             * resource manager. Logs errors.
             */
            Document(const std::string &filename,
                     const UseVirtFs useResman,
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
            ~Document() override final;

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
            char *mData;
            bool mIsValid;
    };

    /**
     * Gets an floating point property from an XmlNodePtr.
     */
    float getFloatProperty(XmlNodeConstPtr node,
                           const char *const name,
                           float def) A_WARN_UNUSED;

    /**
     * Gets an double point property from an XmlNodePtr.
     */
    double getDoubleProperty(XmlNodeConstPtr node,
                            const char *const name,
                            double def) A_WARN_UNUSED;

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getProperty(XmlNodeConstPtr node,
                    const char *const name,
                    int def) A_WARN_UNUSED;

    /**
     * Gets an integer property from an XmlNodePtr.
     */
    int getIntProperty(XmlNodeConstPtr node,
                       const char *const name,
                       int def,
                       const int min,
                       const int max) A_WARN_UNUSED;

    /**
     * Gets a string property from an XmlNodePtr.
     */
    std::string getProperty(XmlNodeConstPtr node,
                            const char *const name,
                            const std::string &def) A_WARN_UNUSED;

    /**
     * Gets a translated string property from an XmlNodePtr.
     */
    std::string langProperty(XmlNodeConstPtr node,
                             const char *const name,
                             const std::string &def) A_WARN_UNUSED;

    /**
     * Gets a boolean property from an XmlNodePtr.
     */
    bool getBoolProperty(XmlNodeConstPtr node,
                         const char *const name,
                         const bool def) A_WARN_UNUSED;

    /**
     * Finds the first child node with the given name
     */
    XmlNodePtr findFirstChildByName(XmlNodeConstPtrConst parent,
                                    const char *const name) A_WARN_UNUSED;

    void initXML();

    void cleanupXML();
}  // namespace XML

#define for_each_xml_child_node(var, parent) \
    for (pugi::xml_node var = parent.first_child(); \
    var; var = var.next_sibling())

#endif  // ENABLE_PUGIXML
#endif  // UTILS_XML_PUGIXML_H
