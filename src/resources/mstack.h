/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifndef RESOURCES_MSTACK_H
#define RESOURCES_MSTACK_H

#include "logger.h"

#include "localconsts.h"

template<typename T>
struct MStack final
{
    constexpr2 explicit MStack(const size_t maxSize) :
        mStack(new T[maxSize]),
        mMaxSize(maxSize),
        mPointer(mStack - 1),
        mStartPointer(mStack - 1),
        mEndPointer(mStack + maxSize - 1)
    {
    }

    ~MStack()
    {
        delete [] mStack;
    }

    A_DELETE_COPY(MStack)

    T &push()
    {
        if (mPointer == mEndPointer)
        {
            logger->log("error: max stack size reached");
            return *mPointer;
        }
        return *(++mPointer);
    }

    T &getPop()
    {
        return *(mPointer--);
    }

    const T &getPopConst()
    {
        return *(mPointer--);
    }

    void pop()
    {
        mPointer --;
    }

    T &top() const
    {
        return *mPointer;
    }

    const T &topConst() const
    {
        return *mPointer;
    }

    void clear()
    {
        mPointer = mStack - 1;
    }

    bool empty() const
    {
        return mPointer == mStartPointer;
    }

    T *mStack;

    size_t mMaxSize;
    T *mPointer;
    const T *const mStartPointer;
    const T *const mEndPointer;
};

#endif  // RESOURCES_MSTACK_H
