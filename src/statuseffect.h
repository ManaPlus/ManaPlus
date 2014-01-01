/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef STATUSEFFECT_H
#define STATUSEFFECT_H

#include "animatedsprite.h"

#include "particle/particle.h"

#include "localconsts.h"

class StatusEffect final
{
public:
    StatusEffect();

    A_DELETE_COPY(StatusEffect)

    ~StatusEffect();

    /**
     * Plays the sound effect associated with this status effect, if possible.
     */
    void playSFX() const;

    /**
     * Delivers the chat message associated with this status effect, if
     * possible.
     */
    void deliverMessage() const;

    /**
     * Creates the particle effect associated with this status effect, if
     * possible.
     */
    Particle *getParticle() const A_WARN_UNUSED;

    /**
     * Retrieves the status icon for this effect, if applicable
     */
    AnimatedSprite *getIcon() const A_WARN_UNUSED;

    /**
     * Retrieves an action to perform, or SpriteAction::INVALID
     */
    std::string getAction() const A_WARN_UNUSED;

    /**
     * Determines whether the particle effect should be restarted when the
     * being changes maps
     */
    bool particleEffectIsPersistent() const A_WARN_UNUSED
    { return mPersistentParticleEffect; }


    /**
     * Retrieves a status effect.
     *
     * \param index Index of the status effect.
     * \param enabling Whether to retrieve the activating effect (true) or
     *    the deactivating effect (false).
     */
    static StatusEffect *getStatusEffect(const int index,
                                         const bool enabling) A_WARN_UNUSED;

    /**
     * Retrieves a stun effect.
     *
     * \param index Index of the stun effect.
     * \param enabling Whether to retrieve the activating effect (true) or
     *    the deactivating effect (false).
     */
    static StatusEffect *getStunEffect(const int index,
                                       const bool enabling) A_WARN_UNUSED;

    /**
     * Maps a block effect index to its corresponding effect index.  Block
     * effect indices are used for opt2/opt3/status.option blocks; their
     * mapping to regular effect indices is handled in the config file.
     *
     * Returns -1 on failure.
     */
    static int blockEffectIndexToEffectIndex(const int blocKIndex)
                                             A_WARN_UNUSED;

    static void load();

    static void unload();
private:
    static bool mLoaded;

    std::string mMessage;
    std::string mSFXEffect;
    std::string mParticleEffect;
    std::string mIcon;
    std::string mAction;
    bool mPersistentParticleEffect;
};

#endif  // STATUSEFFECT_H
