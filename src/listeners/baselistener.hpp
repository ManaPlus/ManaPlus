/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2017  The ManaPlus Developers
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

#ifndef LISTENERS_BASELISTENER_HPP
#define LISTENERS_BASELISTENER_HPP

#include "utils/vector.h"

#define defineListener(name) \
    STD_VECTOR<name*> name::mListeners; \
    \
    name::name() \
    { \
        addListener(this); \
    } \
    \
    name::~name() \
    { \
        removeListener(this); \
    } \
    \
    void name::addListener(name *const listener) \
    { \
        if (listener) \
            mListeners.push_back(listener); \
    } \
    \
    void name::removeListener(const name *const listener) \
    { \
        STD_VECTOR<name*>::iterator it = mListeners.begin(); \
        while (it != mListeners.end()) \
        { \
            if (*it == listener) \
                it = mListeners.erase(it); \
            else \
                ++ it; \
        } \
    }

#define defineListenerHeader(name) \
    public: \
        name(); \
        \
        virtual ~name(); \
        \
        static void addListener(name *const listener); \
        \
        static void removeListener(const name *const listener); \
        \
    private: \
        static STD_VECTOR<name*> mListeners;

#endif  // LISTENERS_BASELISTENER_HPP
