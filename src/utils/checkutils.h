/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#ifndef UTILS_CHECKUTILS_H
#define UTILS_CHECKUTILS_H

#ifdef ENABLE_ASSERTS

#define reportFalse(val) \
    reportFalseReal(val, #val, __FILE__, __LINE__, __func__)

#define reportTrue(val) \
    reportTrueReal(val, #val, __FILE__, __LINE__, __func__)

#define failFalse(val) \
    failFalseReal(val, #val, __FILE__, __LINE__, __func__)

#define failTrue(val) \
    failTrueReal(val, #val, __FILE__, __LINE__, __func__)

bool reportFalseReal(const bool val,
                     const char *const text,
                     const char *const file,
                     const unsigned line,
                     const char *const func);

bool reportTrueReal(const bool val,
                    const char *const text,
                    const char *const file,
                    const unsigned line,
                    const char *const func);

bool failFalseReal(const bool val,
                   const char *const text,
                   const char *const file,
                   const unsigned line,
                   const char *const func);

bool failTrueReal(const bool val,
                  const char *const text,
                  const char *const file,
                  const unsigned line,
                  const char *const func);

void reportStack(const char *const file,
                 const unsigned int line,
                 const char *const func,
                 const char *const name,
                 const char *const text);

#else  // ENABLE_ASSERTS

#define reportFalse(val) (val)
#define reportTrue(val) (val)
#define failFalse(val) (val)
#define failTrue(val) (val)

#endif  // ENABLE_ASSERTS
#endif  // UTILS_CHECKUTILS_H
