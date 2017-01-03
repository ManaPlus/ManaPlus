/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#ifndef RESOURCES_NOTIFICATIONINFO_H
#define RESOURCES_NOTIFICATIONINFO_H

#include "localconsts.h"

#include "enums/resources/notifyflags.h"

struct NotificationInfo final
{
#ifdef ADVGCC
    NotificationInfo(const char *const sound0,
                     const char *const text0,
                     const NotifyFlagsT flags0) :
        sound(sound0),
        text(text0),
        flags(flags0)
    { }

    A_DELETE_COPY(NotificationInfo)
#endif  // ADVGCC

    const char *sound;
    const char *text;
    const NotifyFlagsT flags;
};

#endif  // RESOURCES_NOTIFICATIONINFO_H
