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

#include "depricatedevent.h"

#include "listener.h"
#include "variabledata.h"

#include "debug.h"

ListenMap DepricatedEvent::mBindings;

DepricatedEvent::~DepricatedEvent()
{
    VariableMap::iterator it = mData.begin();
    while (it != mData.end())
    {
        delete it->second;
        it->second = nullptr;
        ++it;
    }
}

void DepricatedEvent::setInt(const std::string &key, const int value)
                             throw (BadDepricatedEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new IntData(value);
}

int DepricatedEvent::getInt(const std::string &key) const
                            throw (BadDepricatedEvent)
{
    const VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (!it->second || it->second->getType() != VariableData::DATA_INT)
        throw BAD_VALUE;

    return static_cast<IntData *>(it->second)->getData();
}

void DepricatedEvent::setString(const std::string &key,
                                const std::string &value)
                                throw (BadDepricatedEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new StringData(value);
}

const std::string &DepricatedEvent::getString(const std::string &key)
                                    const throw (BadDepricatedEvent)
{
    const VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (!it->second || it->second->getType() != VariableData::DATA_STRING)
        throw BAD_VALUE;

    return static_cast<StringData *>(it->second)->getData();
}


void DepricatedEvent::setFloat(const std::string &key, const double value)
                               throw (BadDepricatedEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new FloatData(value);
}

double DepricatedEvent::getFloat(const std::string &key) const
                                 throw (BadDepricatedEvent)
{
    const VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (!it->second || it->second->getType() != VariableData::DATA_FLOAT)
        throw BAD_VALUE;

    return static_cast<FloatData *>(it->second)->getData();
}

void DepricatedEvent::trigger(const Channels channel,
                              const DepricatedEvent &event)
{
    const ListenMap::const_iterator it = mBindings.find(channel);

    // Make sure something is listening
    if (it == mBindings.end())
        return;

    // Loop though all listeners
    ListenerSet::const_iterator lit = it->second.begin();
    const ListenerSet::const_iterator lit_end = it->second.end();
    while (lit != lit_end)
    {
        if (*lit)
            (*lit)->processEvent(channel, event);
        ++lit;
    }
}

void DepricatedEvent::remove(Listener *const listener)
{
    ListenMap::iterator it = mBindings.begin();
    while (it != mBindings.end())
    {
        it->second.erase(listener);
        ++it;
    }
}

void DepricatedEvent::bind(Listener *const listener, const Channels channel)
{
    mBindings[channel].insert(listener);
}

void DepricatedEvent::unbind(Listener *const listener, const Channels channel)
{
    mBindings[channel].erase(listener);
}
