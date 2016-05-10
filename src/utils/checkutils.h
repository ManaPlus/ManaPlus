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

#define reportFalseReal(val) \
    (val ? true : (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), false))

#define reportTrueReal(val) \
    (val ? (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), true) : false)

#define returnFalseVReal(val) \
    if (!val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return; \
    }

#define returnTrueVReal(val) \
    if (val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return; \
    }

#define returnFalseReal(ret, val) \
    if (!val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return ret; \
    }

#define returnTrueReal(ret, val) \
    if (val) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return ret; \
    }

#define returnNullptrVReal(val) \
    if ((val) == nullptr) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return; \
    }

#define returnNullptrReal(ret, val) \
    if ((val) == nullptr) \
    { \
        reportStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return ret; \
    }

#define failFalse(val) \
    (val ? true : (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), false))

#define failTrue(val) \
    (val ? (reportStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), true) : false)

void reportStack(const char *const file,
                 const unsigned int line,
                 const char *const func,
                 const char *const name,
                 const char *const text);

#else  // ENABLE_ASSERTS

#define reportFalseReal(val) (val)
#define reportTrueReal(val) (val)

#define returnFalseVReal(val) \
    if (!val) \
        return;

#define returnTrueVReal(val) \
    if (val) \
        return;

#define returnFalseReal(ret, val) \
    if (!val) \
        return ret;

#define returnTrueReal(ret, val) \
    if (val) \
        return ret;

#define returnNullptrVReal(val) \
    if ((val) == nullptr) \
        return; \

#define returnNullptrReal(ret, val) \
    if ((val) == nullptr) \
        return ret; \

#define failFalse(val) (val)
#define failTrue(val) (val)

#endif  // ENABLE_ASSERTS

#define reportFalse(val) reportFalseReal(val)
#define reportTrue(val) reportTrueReal(val)
#define returnFalseV(val) returnFalseVReal(val)
#define returnTrueV(val) returnTrueVReal(val)
#define returnFalse(ret, val) returnFalseReal(ret, val)
#define returnTrue(ret, val) returnTrueReal(ret, val)
#define returnNullptrV(val) returnNullptrVReal(val)
#define returnNullptr(ret, val) returnNullptrReal(ret, val)

#endif  // UTILS_CHECKUTILS_H
