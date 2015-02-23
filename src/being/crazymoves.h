/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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
        void crazyMove1();
        void crazyMove2();
        void crazyMove3();
        void crazyMove4();
        void crazyMove5();
        void crazyMove6();
        void crazyMove7();
        void crazyMove8();
        void crazyMove9();
        void crazyMoveA();
        void crazyMoveAd();
        void crazyMoveAe();
        void crazyMoveAm();
        void crazyMoveAo();
        void crazyMoveAs();

        std::string mMoveProgram;

        // temporary disable crazy moves in moves
        bool mDisableCrazyMove;
};

extern CrazyMoves *crazyMoves;

#endif  // BEING_CRAZYMOVES_H
