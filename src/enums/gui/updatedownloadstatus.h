/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef ENUMS_GUI_UPDATEDOWNLOADSTATUS_H
#define ENUMS_GUI_UPDATEDOWNLOADSTATUS_H

#include "enums/simpletypes/enumdefines.h"

enumStart(UpdateDownloadStatus)
{
    UPDATE_ERROR = 0,
    UPDATE_IDLE,
    UPDATE_LIST,
    UPDATE_COMPLETE,
    UPDATE_NEWS,
    UPDATE_RESOURCES,
    UPDATE_PATCH,
    UPDATE_LIST2,
    UPDATE_RESOURCES2
}
enumEnd(UpdateDownloadStatus);

#endif  // ENUMS_GUI_UPDATEDOWNLOADSTATUS_H
