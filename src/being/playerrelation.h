/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef BEING_PLAYERRELATION_H
#define BEING_PLAYERRELATION_H

#include "enums/being/relation.h"

#include "localconsts.h"

struct PlayerRelation final
{
    static const unsigned int EMOTE        = (1U << 0);
    static const unsigned int SPEECH_FLOAT = (1U << 1);
    static const unsigned int SPEECH_LOG   = (1U << 2);
    static const unsigned int WHISPER      = (1U << 3);
    static const unsigned int TRADE        = (1U << 4);
    static const unsigned int INVISIBLE    = (1U << 5);
    static const unsigned int BLACKLIST    = (1U << 6);
    static const unsigned int ENEMY        = (1U << 7);

    static const unsigned int RELATIONS_NR = 7;
    static const unsigned int RELATION_PERMISSIONS[RELATIONS_NR];

    static const unsigned int DEFAULT = EMOTE
                                      | SPEECH_FLOAT
                                      | SPEECH_LOG
                                      | WHISPER
                                      | TRADE;

    explicit PlayerRelation(const RelationT relation);

    A_DELETE_COPY(PlayerRelation)

    RelationT mRelation;  // bitmask for all of the above
};

#endif  // BEING_PLAYERRELATION_H
