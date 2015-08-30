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

#include "net/ea/tradehandler.h"

#include "notifymanager.h"

#include "being/playerinfo.h"
#include "being/playerrelation.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/tradewindow.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "listeners/requesttradelistener.h"

#include "debug.h"

extern std::string tradePartnerName;

/**
 * Listener for request trade dialogs
 */
namespace
{
    RequestTradeListener listener;
}  // namespace

namespace Ea
{

TradeHandler::TradeHandler()
{
    confirmDlg = nullptr;
}

void TradeHandler::removeItem(const int slotNum A_UNUSED,
                              const int amount A_UNUSED) const
{
}

}  // namespace Ea
