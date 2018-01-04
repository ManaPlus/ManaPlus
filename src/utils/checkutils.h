/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#include "logger.h"
LOGGER_H

#ifdef ENABLE_ASSERTS

#define reportFalseReal(val) \
    (val ? true : (reportAssertStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), false))

#define reportTrueReal(val) \
    (val ? (reportAssertStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), true) : false)

#define reportAlwaysReal(...) \
    { \
        logger->log("Assert:"); \
        logger->assertLog( \
            __VA_ARGS__); \
        reportLogStack(__FILE__, __LINE__, __func__); \
    }

#define returnFalseVReal(val) \
    if (!val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return; \
    }

#define returnTrueVReal(val) \
    if (val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return; \
    }

#define returnFalseReal(ret, val) \
    if (!val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        return ret; \
    }

#define returnTrueReal(ret, val) \
    if (val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        return ret; \
    }

#define returnNullptrVReal(val) \
    if ((val) == nullptr) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return; \
    }

#define returnNullptrReal(ret, val) \
    if ((val) == nullptr) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        return ret; \
    }

#define failFalse(val) \
    (val ? true : (reportAssertStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), false))

#define failTrue(val) \
    (val ? (reportAssertStack(__FILE__, __LINE__, __func__, \
        "Detected false value", #val), \
        throw new std::exception(), true) : false)

#define returnFailFalseV(val) \
    if (!val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        throw new std::exception(); \
    }

#define returnFailTrueV(val) \
    if (val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        throw new std::exception(); \
    }

#define returnFailFalse(ret, val) \
    if (!val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected false value", #val); \
        throw new std::exception(); \
    }

#define returnFailTrue(ret, val) \
    if (val) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected true value", #val); \
        throw new std::exception(); \
    }

#define returnFailNullptrV(val) \
    if ((val) == nullptr) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        throw new std::exception(); \
    }

#define returnFailNullptr(ret, val) \
    if ((val) == nullptr) \
    { \
        reportAssertStack(__FILE__, __LINE__, __func__, \
            "Detected null value", #val); \
        throw new std::exception(); \
    }

#define failAlways(...) \
    { \
        logger->log("Assert:"); \
        logger->assertLog( \
            __VA_ARGS__); \
        reportLogStack(__FILE__, __LINE__, __func__); \
        throw new std::exception(); \
    }

void reportAssertStack(const char *const file,
                       const unsigned int line,
                       const char *const func,
                       const char *const name,
                       const char *const text);

void reportLogStack(const char *const file,
                    const unsigned int line,
                    const char *const func);

void reportStack();

#else  // ENABLE_ASSERTS

#define reportFalseReal(val) (val)
#define reportTrueReal(val) (val)

#define reportAlwaysReal(...) \
    { \
        logger->log("Error:"); \
        logger->log( \
            __VA_ARGS__); \
    }

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
        return;

#define returnNullptrReal(ret, val) \
    if ((val) == nullptr) \
        return ret;

#define failFalse(val) (val)
#define failTrue(val) (val)

#define returnFailFalseV(val) \
    if (!val) \
        return;

#define returnFailTrueV(val) \
    if (val) \
        return;

#define returnFailFalse(ret, val) \
    if (!val) \
        return ret;

#define returnFailTrue(ret, val) \
    if (val) \
        return ret;

#define returnFailNullptrV(val) \
    if ((val) == nullptr) \
        return;

#define returnFailNullptr(ret, val) \
    if ((val) == nullptr) \
        return ret;

#define failAlways(...) ;

#endif  // ENABLE_ASSERTS

#ifdef UNITTESTS
#define reportFalse(val) failFalse(val)
#define reportTrue(val) failTrue(val)
#define reportAlways(...) failAlways(__VA_ARGS__)
#define returnFalseV(val) returnFailFalseV(val)
#define returnTrueV(val) returnFailTrueV(val)
#define returnFalse(ret, val) returnFailFalse(ret, val)
#define returnTrue(ret, val) returnFailTrue(ret, val)
#define returnNullptrV(val) returnFailNullptrV(val)
#define returnNullptr(ret, val) returnFailNullptr(ret, val)
#else  // UNITTESTS
#define reportFalse(val) reportFalseReal(val)
#define reportTrue(val) reportTrueReal(val)
#define reportAlways(...) reportAlwaysReal(__VA_ARGS__)
#define returnFalseV(val) returnFalseVReal(val)
#define returnTrueV(val) returnTrueVReal(val)
#define returnFalse(ret, val) returnFalseReal(ret, val)
#define returnTrue(ret, val) returnTrueReal(ret, val)
#define returnNullptrV(val) returnNullptrVReal(val)
#define returnNullptr(ret, val) returnNullptrReal(ret, val)
#endif  // UNITTESTS

#endif  // UTILS_CHECKUTILS_H
