/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef UTILS_GETTEXT_H
#define UTILS_GETTEXT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#ifdef ENABLE_NLS

#include <libintl.h>

#define _(s) (const_cast <char*>(gettext(s)))
#define N_(s) (const_cast <char*>(s))

#elif defined(ENABLE_CUSTOMNLS)  // ENABLE_NLS

#include "utils/translation/podict.h"

#define gettext(s) const_cast <char*>(mainTranslator->getChar(s))
#define _(s) const_cast <char*>(mainTranslator->getChar(s))
#define N_(s) (const_cast <char*>(s))
#define ngettext(s1, s2, i1) const_cast <char*>(mainTranslator->getChar(s1))

#else  // ENABLE_NLS

#define gettext(s) (const_cast <char*>(s))
#define _(s) (const_cast <char*>(s))
#define N_(s) (const_cast <char*>(s))
#define ngettext(s1, s2, i1) (const_cast <char*>(s1))

#endif  // ENABLE_NLS

#endif  // UTILS_GETTEXT_H
