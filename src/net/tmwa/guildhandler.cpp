/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/tmwa/guildhandler.h"

#include "debug.h"

namespace TmwAthena
{

GuildHandler::GuildHandler() :
    Net::GuildHandler()
{
    guildHandler = this;
}

GuildHandler::~GuildHandler()
{
    guildHandler = nullptr;
}

void GuildHandler::clear() const
{
}

ChatTab *GuildHandler::getTab() const
{
    return nullptr;
}

void GuildHandler::create(const std::string &name A_UNUSED) const
{
}

void GuildHandler::invite(const std::string &name A_UNUSED) const
{
}

void GuildHandler::invite(const Being *const being A_UNUSED) const
{
}

void GuildHandler::inviteResponse(const int guildId A_UNUSED,
                                  const bool response A_UNUSED) const
{
}

void GuildHandler::leave(const int guildId A_UNUSED) const
{
}

void GuildHandler::kick(const GuildMember *restrict const member A_UNUSED,
                        const std::string &restrict reason A_UNUSED) const
{
}

void GuildHandler::chat(const std::string &text A_UNUSED) const
{
}

void GuildHandler::memberList() const
{
}

void GuildHandler::info() const
{
}

void GuildHandler::changeMemberPostion(const GuildMember *const member
                                       A_UNUSED,
                                       const int level A_UNUSED) const
{
}

void GuildHandler::changeNotice(const int guildId A_UNUSED,
                                const std::string &restrict msg1 A_UNUSED,
                                const std::string &restrict msg2 A_UNUSED)
                                const
{
}

void GuildHandler::checkMaster() const
{
}

void GuildHandler::requestAlliance(const Being *const being A_UNUSED) const
{
}

void GuildHandler::requestAllianceResponse(const int beingId A_UNUSED,
                                           const bool accept A_UNUSED) const
{
}

void GuildHandler::endAlliance(const int guildId A_UNUSED,
                               const int flag A_UNUSED) const
{
}

void GuildHandler::changePostionInfo(const int posId A_UNUSED,
                                     const int mode A_UNUSED,
                                     const int ranking A_UNUSED,
                                     const int payRate A_UNUSED,
                                     const std::string &name A_UNUSED) const
{
}

void GuildHandler::requestOpposition(const Being *const being A_UNUSED) const
{
}

void GuildHandler::breakGuild(const std::string &name A_UNUSED) const
{
}

void GuildHandler::changeEmblem(std::string emblem A_UNUSED) const
{
}

void GuildHandler::requestEmblem(const int guildId A_UNUSED) const
{
}

}  // namespace TmwAthena
