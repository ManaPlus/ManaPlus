/*
 *  An effects manager
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
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

#include "effectmanager.h"

#include "configuration.h"
#include "logger.h"
#include "soundmanager.h"

#include "being/being.h"

#include "particle/particle.h"

#include "debug.h"

EffectManager::EffectManager() :
    mEffects()
{
    XML::Document doc(paths.getStringValue("effectsFile"));
    const XmlNodePtr root = doc.rootNode();

    if (!root || !xmlNameEqual(root, "being-effects"))
    {
        logger->log("Error loading being effects file: "
            + paths.getStringValue("effectsFile"));
        return;
    }
    else
    {
        logger->log1("Effects are now loading");
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "effect"))
        {
            mEffects.push_back(EffectDescription(
                XML::getProperty(node, "id", -1),
                XML::getProperty(node, "particle", ""),
                XML::getProperty(node, "audio", ""),
                XML::getProperty(node, "sprite", "")));
        }
    }
}

EffectManager::~EffectManager()
{
}

bool EffectManager::trigger(const int id, Being *const being,
                            const int rotation)
{
    if (!being || !particleEngine)
        return false;

    bool rValue = false;
    FOR_EACH (std::vector<EffectDescription>::const_iterator, i, mEffects)
    {
        const EffectDescription &effect = *i;
        if (effect.id == id)
        {
            rValue = true;
            if (!effect.gfx.empty())
            {
                Particle *const selfFX = particleEngine->addEffect(
                    effect.gfx, 0, 0, rotation);
                being->controlParticle(selfFX);
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx);
            if (!effect.sprite.empty())
                being->addEffect(effect.sprite);
            break;
        }
    }
    return rValue;
}

Particle *EffectManager::triggerReturn(const int id, Being *const being,
                                       const int rotation)
{
    if (!being || !particleEngine)
        return nullptr;

    Particle *rValue = nullptr;
    FOR_EACH (std::vector<EffectDescription>::const_iterator, i, mEffects)
    {
        const EffectDescription &effect = *i;
        if (effect.id == id)
        {
            if (!effect.gfx.empty())
            {
                rValue = particleEngine->addEffect(
                    effect.gfx, 0, 0, rotation);
                being->controlParticle(rValue);
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx);
            if (!effect.sprite.empty())
                being->addEffect(effect.sprite);
            break;
        }
    }
    return rValue;
}

bool EffectManager::trigger(const int id, const int x, const int y,
                            const int rotation)
{
    if (!particleEngine)
        return false;

    bool rValue = false;
    FOR_EACH (std::vector<EffectDescription>::const_iterator, i, mEffects)
    {
        const EffectDescription &effect = *i;
        if (effect.id == id)
        {
            rValue = true;
            if (!effect.gfx.empty() && particleEngine)
                particleEngine->addEffect(effect.gfx, x, y, rotation);
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx);
            // TODO add sprite effect to position
            break;
        }
    }
    return rValue;
}
