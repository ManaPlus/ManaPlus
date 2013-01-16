/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <list>
#include <string>

#include "localconsts.h"

class Channel;

class ChannelManager final
{
public:
    ChannelManager();
    A_DELETE_COPY(ChannelManager)
    ~ChannelManager();

    Channel *findById(const int id) const A_WARN_UNUSED;
    Channel *findByName(const std::string &name) const A_WARN_UNUSED;

    void addChannel(Channel *const channel);
    void removeChannel(Channel *const channel);

private:
    std::list<Channel*> mChannels;
};

extern ChannelManager *channelManager;

#endif
