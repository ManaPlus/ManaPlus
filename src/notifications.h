/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef NOTIFYCATIONS_H
#define NOTIFYCATIONS_H

#include "utils/gettext.h"

namespace NotifyManager
{
    struct NotificationInfo
    {
        const char *text;
    };

    enum NotifyTypes
    {
        BUY_DONE,
        BUY_FAILED,
        SELL_LIST_EMPTY,
        SOLD,
        SELL_FAILED,
        SELL_TRADE_FAILED,
        SELL_UNSELLABLE_FAILED,
        TYPE_END
    };

    static const NotificationInfo notifications[] =
    {
        {N_("Thanks for buying.")},
        {N_("Unable to buy.")},
        {N_("Nothing to sell.")},
        {N_("Thanks for selling.")},
        {N_("Unable to sell.")},
        {N_("Unable to sell while trading.")},
        {N_("Unable to sell unsellable item.")}
    };
}
#endif
