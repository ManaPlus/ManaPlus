/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef UTILS_XML_PUGIXMLWRITER_H
#define UTILS_XML_PUGIXMLWRITER_H

#ifdef ENABLE_PUGIXML

#include "enums/simpletypes/skiperror.h"
#include "enums/simpletypes/usevirtfs.h"

#include "utils/xml/pugixml.h"

#include <pugixml.hpp>

#include <string>

#include "localconsts.h"

namespace XML
{
    class Writer final
    {
        public:
            explicit Writer(const std::string &filename);

            A_DELETE_COPY(Writer)

            ~Writer();

            void startNode(const std::string &name) const;

            void endNode() const;

            void endDocument() const;

            void addAttribute(const std::string &name,
                              const std::string &value) const;

        private:
            mutable pugi::xml_document mDoc;
            mutable pugi::xml_node mNode;
            std::string mName;
    };
}  // namespace XML

#endif  // ENABLE_PUGIXML
#endif  // UTILS_XML_PUGIXMLWRITER_H
