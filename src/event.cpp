/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "event.h"

#include "listener.h"
#include "variabledata.h"

#include "debug.h"

namespace Mana
{

ListenMap Event::mBindings;

Event::~Event()
{
    VariableMap::iterator it = mData.begin();
    while (it != mData.end())
    {
        delete it->second;
        it->second = 0;
        ++it;
    }
}

void Event::setInt(const std::string &key, int value) throw (BadEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new IntData(value);
}

int Event::getInt(const std::string &key) const throw (BadEvent)
{
    VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (it->second->getType() != VariableData::DATA_INT)
        throw BAD_VALUE;

    return static_cast<IntData *>(it->second)->getData();
}

void Event::setString(const std::string &key,
                      const std::string &value) throw (BadEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new StringData(value);
}

const std::string &Event::getString(const std::string &key)
                                    const throw (BadEvent)
{
    VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (it->second->getType() != VariableData::DATA_STRING)
        throw BAD_VALUE;

    return static_cast<StringData *>(it->second)->getData();
}


void Event::setFloat(const std::string &key, double value) throw (BadEvent)
{
    if (mData.find(key) != mData.end())
        throw KEY_ALREADY_EXISTS;

    mData[key] = new FloatData(value);
}

double Event::getFloat(const std::string &key) const throw (BadEvent)
{
    VariableMap::const_iterator it = mData.find(key);
    if (it == mData.end())
        throw BAD_KEY;

    if (it->second->getType() != VariableData::DATA_FLOAT)
        throw BAD_VALUE;

    return static_cast<FloatData *>(it->second)->getData();
}

void Event::trigger(Channels channel, const Event &event)
{
    ListenMap::const_iterator it = mBindings.find(channel);

    // Make sure something is listening
    if (it == mBindings.end())
        return;

    // Loop though all listeners
    ListenerSet::const_iterator lit = it->second.begin();
    while (lit != it->second.end())
    {
        if (*lit)
            (*lit)->event(channel, event);
        ++lit;
    }
}

void Event::remove(Listener *listener)
{
    ListenMap::iterator it = mBindings.begin();
    while (it != mBindings.end())
    {
        it->second.erase(listener);
        ++it;
    }
}

void Event::bind(Listener *listener, Channels channel)
{
    mBindings[channel].insert(listener);
}

void Event::unbind(Listener *listener, Channels channel)
{
    mBindings[channel].erase(listener);
}

}
