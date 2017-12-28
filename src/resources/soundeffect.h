/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_SOUNDEFFECT_H
#define RESOURCES_SOUNDEFFECT_H

#include "resources/resource.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_mixer.h>
PRAGMA48(GCC diagnostic pop)

#include "localconsts.h"

/**
 * Defines a class for loading and storing sound effects.
 */
class SoundEffect final : public Resource
{
    public:
        /**
         * Constructor.
         */
        SoundEffect(Mix_Chunk *const soundEffect,
                    const std::string &name) :
            Resource(),
            mName(name),
            mChunk(soundEffect)
        { }

        A_DELETE_COPY(SoundEffect)

        /**
         * Destructor.
         */
        ~SoundEffect() override final;

        /**
         * Plays the sample.
         *
         * @param loops     Number of times to repeat the playback.
         * @param volume    Sample playback volume.
         * @param channel   Sample playback channel.
         *
         * @return <code>true</code> if the playback started properly
         *         <code>false</code> otherwise.
         */
        bool play(const int loops,
                  const int volume,
                  const int channel) const;

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override final
        { return mName; }

    protected:
        std::string mName;
        Mix_Chunk *mChunk;
};

#endif  // RESOURCES_SOUNDEFFECT_H
