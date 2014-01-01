/*
 *  Retrieve string pasted depending on OS mechanisms.
 *  Copyright (C) 2001-2010  Wormux Team
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef UTILS_COPYNPASTE_H
#define UTILS_COPYNPASTE_H

#include <string>

#include "localconsts.h"

/**
 * Attempts to retrieve text from the clipboard buffer and inserts it in
 * \a text at position \pos. The characters are encoded in utf-8.
 *
 * Implemented for Windows, X11 and Mac OS X.
 *
 * @return <code>true</code> when successful or <code>false</code> when there
 *         was a problem retrieving the clipboard buffer.
 */
bool retrieveBuffer(std::string& text, size_t& pos) A_WARN_UNUSED;

bool sendBuffer(std::string& text);

#endif  // UTILS_COPYNPASTE_H
