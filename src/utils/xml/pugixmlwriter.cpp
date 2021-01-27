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

#ifdef ENABLE_PUGIXML

#include "utils/xml/pugixmlwriter.h"

#include "logger.h"

#include "fs/virtfs/fs.h"

#include "utils/delete2.h"
#include "utils/fuzzer.h"
#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace XML
{
    Writer::Writer(const std::string &filename) :
        mDoc(),
        mNode(),
        mName(filename)
    {
        mNode = mDoc;
    }

    Writer::~Writer()
    {
    }

    void Writer::startNode(const std::string &name) const
    {
        mNode = mNode.append_child(name.c_str());
    }

    void Writer::endNode() const
    {
        mNode = mNode.parent();
    }

    void Writer::endDocument() const
    {
        mDoc.save_file(mName.c_str());
    }

    void Writer::addAttribute(const std::string &name,
                              const std::string &value) const
    {
        mNode.append_attribute(name.c_str()) = value.c_str();
    }

}  // namespace XML

#endif  // ENABLE_PUGIXML
