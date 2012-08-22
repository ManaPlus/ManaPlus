/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/tmwa/gui/guildtab.h"

#include "chatlogger.h"
#include "commandhandler.h"
#include "guild.h"
#include "localplayer.h"

#include "gui/theme.h"

#include "net/net.h"
#include "net/guildhandler.h"

#include "resources/iteminfo.h"
#include "resources/itemdb.h"

#include "utils/gettext.h"

#include "debug.h"

namespace TmwAthena
{
GuildTab::GuildTab() :
    Ea::GuildTab()
{
}

GuildTab::~GuildTab()
{
}

} // namespace TmwAthena
