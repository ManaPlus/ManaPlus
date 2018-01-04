/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef BEING_CRAZYMOVES_H
#define BEING_CRAZYMOVES_H

#include <string>

#include "localconsts.h"

class CrazyMoves final
{
    public:
        CrazyMoves();
        A_DELETE_COPY(CrazyMoves)
        void crazyMove();

    protected:
        static void crazyMove1();
        static void crazyMove2();
        static void crazyMove3();
        static void crazyMove4();
        static void crazyMove5();
        static void crazyMove6();
        static void crazyMove7();
        static void crazyMove8();
        static void crazyMove9();
        void crazyMoveA();
        void crazyMoveAd() const;
        void crazyMoveAe() const;
        void crazyMoveAm() const;
        void crazyMoveAo() const;
        static void crazyMoveAs();

        std::string mMoveProgram;

        // temporary disable crazy moves in moves
        bool mDisableCrazyMove;
};

extern CrazyMoves *crazyMoves;

#endif  // BEING_CRAZYMOVES_H
