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

#ifndef UTILS_XML_H
#define UTILS_XML_H

#ifdef ENABLE_PUGIXML
#include "utils/xml/pugixml.h"
#elif defined(ENABLE_TINYXML2)
#include "utils/xml/tinyxml2.h"
#else  // ENABLE_PUGIXML
#include "utils/xml/libxml.h"
#endif  // ENABLE_PUGIXML

XML_INCLUDE_DEFINE

#endif  // UTILS_XML_H
