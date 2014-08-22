/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_SETUPACTIONDATA_H
#define GUI_SETUPACTIONDATA_H

#include <string>

struct SetupActionData final
{
#ifdef ADVGCC
    SetupActionData(const std::string &name0,
                    const int actionId0,
                    const std::string &text0) :
        name(name0),
        actionId(actionId0),
        text(text0)
    { }

    A_DELETE_COPY(SetupActionData)
#endif

    std::string name;
    const int actionId;
    std::string text;
};

const int touchActionDataSize = 4;
extern SetupActionData *const touchActionData[];

extern SetupActionData *const setupActionData[];
extern const char *const pages[];
extern SetupActionData setupActionData0[];
extern SetupActionData setupActionData1[];
extern SetupActionData setupActionData2[];
extern SetupActionData setupActionData3[];
extern SetupActionData setupActionData4[];
extern SetupActionData setupActionData5[];
extern SetupActionData setupActionData6[];
extern SetupActionData setupActionData7[];
extern SetupActionData setupActionData8[];

#endif  // GUI_SETUPACTIONDATA_H
