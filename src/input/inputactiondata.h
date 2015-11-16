/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
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

#ifndef INPUT_INPUTACTIONDATA_H
#define INPUT_INPUTACTIONDATA_H

#include "actions/actionfuncptr.h"

#include "enums/input/inputtype.h"

#include "enums/simpletypes/useargs.h"
#include "enums/simpletypes/protected.h"

struct InputActionData final
{
    const char *const configField;
    const InputTypeT defaultType1;
    const int defaultValue1;
    const InputTypeT defaultType2;
    const int defaultValue2;
    const int grp;
    const ActionFuncPtr action;
    const InputActionT modKeyIndex;
    const int priority;
    const int condition;
    const std::string chatCommand;
    const UseArgs useArgs;
    const Protected isProtected;
};

namespace Input
{
    enum InputGroup
    {
        GRP_DEFAULT = 1,           // default game key
        GRP_CHAT = 2,              // chat key
        GRP_EMOTION = 4,           // emotions key
        GRP_OUTFIT = 8,            // outfit key
        GRP_GUI = 16,              // gui key
        GRP_MOVETOPOINT = 32,      // move to point key
        GRP_GUICHAN = 64,          // for guichan based controls
        GRP_REPEAT = 128,          // repeat emulation keys
        GRP_PET_EMOTION = 256,     // pet emotions key
        GRP_HOMUN_EMOTION = 512    // homunculus/mercenary emotetions key
    };

}  // namespace Input

#endif  // INPUT_INPUTACTIONDATA_H
