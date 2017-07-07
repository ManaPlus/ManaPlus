/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#ifndef UNITTESTS_H
#define UNITTESTS_H

#include "localconsts.h"

#ifdef UNITTESTS_CATCH
#include "test/catch.hpp"
#endif  // UNITTESTS_CATCH

#ifdef UNITTESTS_DOCTEST

#ifdef __GNUC__
#if GCC_VERSION >= 50000
#define PRAGMA5(str) _Pragma(#str)
#elif defined(__clang__)
#define PRAGMA5(str) _Pragma(#str)
#else  // GCC_VERSION > 50000
#define PRAGMA5(str)
#endif  // GCC_VERSION > 50000
#endif  // __GNUC__

PRAGMA5(GCC diagnostic push)
PRAGMA5(GCC diagnostic ignored "-Wsuggest-override")
#include "test/doctest.h"
PRAGMA5(GCC diagnostic pop)

#undef TEST_CASE
#define TEST_CASE(name, tags) DOCTEST_TEST_CASE(tags " " name)
#define SECTION(name) DOCTEST_SUBCASE(name)
#endif  // UNITTESTS_DOCTEST

#endif  // UNITTESTS_H
