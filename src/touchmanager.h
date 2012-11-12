/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef TOUCHMANAGER_H
#define TOUCHMANAGER_H

#include "resources/image.h"

#include <guichan/mouseinput.hpp>
#include <guichan/rectangle.hpp>

#include <map>
#include <string>
#include <vector>

#include "localconsts.h"

typedef void (*TouchFuncPtr) (const gcn::MouseInput &mouseInput);

struct TouchItem final
{
    TouchItem(const gcn::Rectangle rect0, Image *const img, int x0, int y0,
              TouchFuncPtr ptrAll, TouchFuncPtr ptrPressed,
              TouchFuncPtr ptrReleased) :
        rect(rect0),
        image(img),
        x(x0),
        y(y0),
        funcAll(ptrAll),
        funcPressed(ptrPressed),
        funcReleased(ptrReleased)
    {
    }

    A_DELETE_COPY(TouchItem)

    gcn::Rectangle rect;
    Image *image;
    int x;
    int y;
    TouchFuncPtr funcAll;
    TouchFuncPtr funcPressed;
    TouchFuncPtr funcReleased;
};

typedef std::vector<TouchItem*> TouchItemVector;
typedef TouchItemVector::const_iterator TouchItemVectorCIter;

class TouchManager final
{
    public:
        TouchManager();

        ~TouchManager();

        A_DELETE_COPY(TouchManager)

        void init();

        void clear();

        void draw();

        bool processEvent(const gcn::MouseInput &mouseInput);

    private:
        TouchItemVector mObjects;
//        std::map <std::string, int> mNameToRect;
};

extern TouchManager touchManager;
#endif
