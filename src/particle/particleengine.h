/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef PARTICLE_PARTICLEENGINE_H
#define PARTICLE_PARTICLEENGINE_H

#include "enums/particle/particlephysics.h"

#include <list>
#include <string>

#include "localconsts.h"

class Color;
class Font;
class Map;
class Particle;
class ParticleEmitter;

typedef std::list<Particle *> Particles;
typedef Particles::iterator ParticleIterator;
typedef Particles::const_iterator ParticleConstIterator;
typedef std::list<ParticleEmitter *> Emitters;
typedef Emitters::iterator EmitterIterator;
typedef Emitters::const_iterator EmitterConstIterator;

class ParticleEngine final
{
    public:
        static const float PARTICLE_SKY;  // Maximum Z position of particles
        static ParticlePhysicsT fastPhysics;  // Mode of squareroot calculation
        static int particleCount;         // Current number of particles
        static int maxCount;              // Maximum number of particles
        static int emitterSkip;           // Duration of pause between two
                                          // emitter updates in ticks
        static bool enabled;  // true when non-crucial particle effects
                              // are disabled

        ParticleEngine();

        A_DELETE_COPY(ParticleEngine)

        /**
         * Destructor.
         */
        ~ParticleEngine();

        /**
         * Deletes all child particles and emitters.
         */
        void clear() restrict2;

        /**
         * Gives a particle the properties of an engine root particle and loads
         * the particle-related config settings.
         */
        static void setupEngine();

        /**
         * Updates particle position, returns false when the particle should
         * be deleted.
         */
        bool update() restrict2;

        /**
         * Creates a blank particle as a child of the current particle
         * Useful for creating target particles
         */
        Particle *createChild() restrict2;

        /**
         * Creates a child particle that hosts some emitters described in the
         * particleEffectFile.
         */
        Particle *addEffect(const std::string &restrict particleEffectFile,
                            const int pixelX,
                            const int pixelY,
                            const int rotation) restrict2;

        /**
         * Creates a standalone text particle.
         */
        Particle *addTextSplashEffect(const std::string &restrict text,
                                      const int x,
                                      const int y,
                                      const Color *restrict const color,
                                      Font *restrict const font,
                                      const bool outline)
                                      restrict2 A_NONNULL(5, 6);

        /**
         * Creates a standalone text particle.
         */
        Particle *addTextRiseFadeOutEffect(const std::string &restrict text,
                                           const int x,
                                           const int y,
                                           const Color *restrict const color,
                                           Font *restrict const font,
                                           const bool outline)
                                           restrict2 A_NONNULL(5, 6);

        void setMap(Map *const map)
        { mMap = map; }

    private:
        // List of particles controlled by this particle
        Particles mChildParticles;
        Particles mChildMoveParticles;
        Map *mMap;
};

extern ParticleEngine *particleEngine;

#endif  // PARTICLE_PARTICLEENGINE_H
