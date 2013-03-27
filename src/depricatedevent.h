/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef DEPRICATED_EVENT_H
#define DEPRICATED_EVENT_H

#include <map>
#include <set>
#include <string>

#include "localconsts.h"

enum Channels
{
    CHANNEL_ATTRIBUTES = 0,
    CHANNEL_CLIENT,
    CHANNEL_GAME,
    CHANNEL_NOTICES
};

enum DepricatedEvents
{
    EVENT_CONSTRUCTED = 0,
    EVENT_DESTRUCTED,
    EVENT_ENGINESINITALIZING,
    EVENT_GUIWINDOWSLOADED,
    EVENT_GUIWINDOWSLOADING,
    EVENT_GUIWINDOWSUNLOADED,
    EVENT_GUIWINDOWSUNLOADING,
    EVENT_SERVERNOTICE,
    EVENT_STATECHANGE,
    EVENT_UPDATEATTRIBUTE,
    EVENT_UPDATESTAT
};

class Listener;
class VariableData;
typedef std::map<std::string, VariableData *> VariableMap;

typedef std::set<Listener *> ListenerSet;
typedef std::map<Channels, ListenerSet > ListenMap;

class DepricatedEvent final
{
    public:
        // String passed can be retivered with getName()
        // and is to used to identify what type of event
        // this is.
        DepricatedEvent(const DepricatedEvents name) :
            mDepricatedEventName(name)
        { }

        ~DepricatedEvent();

        DepricatedEvents getName() const A_WARN_UNUSED
        { return mDepricatedEventName; }

        // Sets or gets a interger with a key to identify
        void setInt(const std::string &key, int value);

        int getInt(const std::string &key) const A_WARN_UNUSED;

        // Sets or gets a string with a key to identify
        void setString(const std::string &key,
                       const std::string &value);

        const std::string getString(const std::string &key)
                                    const A_WARN_UNUSED;

        // Sets or gets a floating point number with key to identify
        void setFloat(const std::string &key, double value);

        double getFloat(const std::string &key) const A_WARN_UNUSED;

        // Sends event to all listener on the channel
        static void trigger(const Channels channel,
                            const DepricatedEvent &event);

        // Removes a listener from all channels
        static void remove(Listener *const listener);

        // Adds or removes a listener to a channel.
        static void bind(Listener *const listener, const Channels channel);

        static void unbind(Listener *const listener, const Channels channel);

    private:
        DepricatedEvents mDepricatedEventName;

        static ListenMap mBindings;

        VariableMap mData;
};

#endif
