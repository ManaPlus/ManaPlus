/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef DEPRICATED_EVENT_H
#define DEPRICATED_EVENT_H

#include <map>
#include <set>
#include <string>

enum Channels
{
    CHANNEL_ACTORSPRITE = 0,
    CHANNEL_ATTRIBUTES,
    CHANNEL_BUYSELL,
    CHANNEL_CHAT,
    CHANNEL_CLIENT,
    CHANNEL_GAME,
    CHANNEL_ITEM,
    CHANNEL_NOTICES,
    CHANNEL_NPC,
    CHANNEL_STATUS,
    CHANNEL_STORAGE
};

enum DepricatedEvents
{
    EVENT_ANNOUNCEMENT = 0,
    EVENT_BEING,
    EVENT_CLOSE,
    EVENT_CLOSEALL,
    EVENT_CONSTRUCTED,
    EVENT_DBSLOADING,
    EVENT_DESTROYED,
    EVENT_DESTRUCTED,
    EVENT_DESTRUCTING,
    EVENT_DOCLOSE,
    EVENT_DOCLOSEINVENTORY,
    EVENT_DODROP,
    EVENT_DOEQUIP,
    EVENT_DOINTEGERINPUT,
    EVENT_DOMENU,
    EVENT_DOMOVE,
    EVENT_DONEXT,
    EVENT_DOSENDLETTER,
    EVENT_DOSPLIT,
    EVENT_DOSTRINGINPUT,
    EVENT_DOTALK,
    EVENT_DOUNEQUIP,
    EVENT_DOUSE,
    EVENT_END,
    EVENT_ENGINESINITALIZED,
    EVENT_ENGINESINITALIZING,
    EVENT_GUIWINDOWSLOADED,
    EVENT_GUIWINDOWSLOADING,
    EVENT_GUIWINDOWSUNLOADED,
    EVENT_GUIWINDOWSUNLOADING,
    EVENT_INTEGERINPUT,
    EVENT_MAPLOADED,
    EVENT_MENU,
    EVENT_MESSAGE,
    EVENT_NEXT,
    EVENT_NPCCOUNT,
    EVENT_PLAYER,
    EVENT_POST,
    EVENT_POSTCOUNT,
    EVENT_SERVERNOTICE,
    EVENT_STATECHANGE,
    EVENT_STORAGECOUNT,
    EVENT_STRINGINPUT,
    EVENT_STUN,
    EVENT_TRADING,
    EVENT_UPDATEATTRIBUTE,
    EVENT_UPDATESTAT,
    EVENT_UPDATESTATUSEFFECT,
    EVENT_WHISPER,
    EVENT_WHISPERERROR
};

// Possible exception that can be thrown
enum BadDepricatedEvent
{
    BAD_KEY = 0,
    BAD_VALUE,
    KEY_ALREADY_EXISTS
};

class Listener;
class VariableData;
typedef std::map<std::string, VariableData *> VariableMap;

typedef std::set<Listener *> ListenerSet;
typedef std::map<Channels, ListenerSet > ListenMap;

#define SERVER_NOTICE(message) { \
DepricatedEvent event(EVENT_SERVERNOTICE); \
event.setString("message", message); \
DepricatedEvent::trigger(CHANNEL_NOTICES, event); }

class DepricatedEvent
{
    public:
        // String passed can be retivered with getName()
        // and is to used to identify what type of event
        // this is.
        DepricatedEvent(const DepricatedEvents name) :
            mDepricatedEventName(name)
        { }

        ~DepricatedEvent();

        DepricatedEvents getName() const
        { return mDepricatedEventName; }

        // Sets or gets a interger with a key to identify
        void setInt(const std::string &key, int value)
        throw (BadDepricatedEvent);

        int getInt(const std::string &key)
        const throw (BadDepricatedEvent);

        // Sets or gets a string with a key to identify
        void setString(const std::string &key,
                       const std::string &value)
        throw (BadDepricatedEvent);

        const std::string &getString(const std::string &key)
        const throw (BadDepricatedEvent);

        // Sets or gets a floating point number with key to identify
        void setFloat(const std::string &key, double value)
        throw (BadDepricatedEvent);
        double getFloat(const std::string &key)
        const throw (BadDepricatedEvent);

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
