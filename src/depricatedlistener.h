/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef DEPRICATEDLISTENER_H
#define DEPRICATEDLISTENER_H

#include "depricatedevent.h"

class DepricatedListener
{
    public:
        virtual ~DepricatedListener();

        void listen(Channels channel);

        void ignore(Channels channel);

        virtual void processEvent(const Channels channel,
                                  const DepricatedEvent &event) = 0;
};

#endif  // DEPRICATEDLISTENER_H
