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
    (val ? true : (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), false))

#define reportTrue(val) \
    (val ? (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), true) : false)

#define failFalse(val) \
    (val ? true : (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), false))

#define failTrue(val) \
    (val ? (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), true) : false)

#define returnFalseV(val) \
    if (!val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return; \
    }

#define returnTrueV(val) \
    if (val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return; \
    }

#define returnFalse(ret, val) \
    if (!val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return ret; \
    }

#define returnTrue(ret, val) \
    if (val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return ret; \
    }

#define returnNullptrV(val) \
    if ((val) == nullptr) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return; \
    }

#define returnNullptr(ret, val) \
    if ((val) == nullptr) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return ret; \
    }

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

#define returnFalseV(val) \
    if (!val) \
        return;

#define returnTrueV(val) \
    if (val) \
        return;

#define returnFalse(ret, val) \
    if (!val) \
        return ret;

#define returnTrue(ret, val) \
    if (val) \
        return ret;

#define returnNullptrV(val) \
    if ((val) == nullptr) \
        return; \

#define returnNullptr(ret, val) \
    if ((val) == nullptr) \
        return ret; \

#endif  // ENABLE_ASSERTS
#endif  // UTILS_CHECKUTILS_H
