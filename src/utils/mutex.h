/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef UTILS_MUTEX_H
#define UTILS_MUTEX_H

#include "logger.h"

/**
 * A mutex provides mutual exclusion of access to certain data that is
 * accessed by multiple threads.
 */
class Mutex final
{
    public:
        Mutex();

        A_DELETE_COPY(Mutex)

        ~Mutex();

        void lock();

        void unlock();

    private:
//        Mutex(const Mutex&);  // prevent copying
//        Mutex& operator=(const Mutex&);

        SDL_mutex *mMutex;
};

/**
 * A convenience class for locking a mutex.
 */
class MutexLocker final
{
    public:
        explicit MutexLocker(Mutex *const mutex);

        MutexLocker(const MutexLocker &m) :
            mMutex(m.mMutex)
        {
        }

        A_DEFAULT_COPY(MutexLocker)

        ~MutexLocker();

    private:
        Mutex *mMutex;
};


inline Mutex::Mutex() :
    mMutex(SDL_CreateMutex())
{
}

inline Mutex::~Mutex()
{
    SDL_DestroyMutex(mMutex);
}

inline void Mutex::lock()
{
    if (SDL_mutexP(mMutex) == -1)
        logger->log("Mutex locking failed: %s", SDL_GetError());
}

inline void Mutex::unlock()
{
    if (SDL_mutexV(mMutex) == -1)
        logger->log("Mutex unlocking failed: %s", SDL_GetError());
}


inline MutexLocker::MutexLocker(Mutex *const mutex) :
    mMutex(mutex)
{
    if (mMutex != nullptr)
        mMutex->lock();
}

inline MutexLocker::~MutexLocker()
{
    if (mMutex != nullptr)
        mMutex->unlock();
}

#endif  // UTILS_MUTEX_H
