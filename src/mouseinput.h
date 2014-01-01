/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <guichan/mouseinput.hpp>

#include "localconsts.h"

class MouseInput final : public gcn::MouseInput
{
    public:
        MouseInput();

        ~MouseInput();

        void setReal(const int x, const int y)
        { mRealX = x; mRealY = y; }

        int getRealX() const A_WARN_UNUSED
        { return mRealX; }

        int getRealY() const A_WARN_UNUSED
        { return mRealY; }

#ifdef ANDROID
        int getTouchX() const A_WARN_UNUSED
        { return mRealX; }

        int getTouchY() const A_WARN_UNUSED
        { return mRealY; }
#else
        int getTouchX() const A_WARN_UNUSED
        { return mX; }

        int getTouchY() const A_WARN_UNUSED
        { return mY; }
#endif

    protected:
        int mRealX;
        int mRealY;
};

#endif  // MOUSEINPUT_H
