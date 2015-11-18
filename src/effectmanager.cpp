/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "resources/beingcommon.h"

#include "debug.h"

EffectManager *effectManager = nullptr;

EffectManager::EffectManager() :
    mEffects(),
    mTimers()
{
    logger->log1("Effects are now loading");
    loadXmlFile(paths.getStringValue("effectsFile"));
    loadXmlFile(paths.getStringValue("effectsPatchFile"));
    loadXmlDir("effectsPatchDir", loadXmlFile);
}

void EffectManager::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName, UseResman_true, SkipError_false);
    const XmlNodePtrConst root = doc.rootNode();

    if (!root ||
        !xmlNameEqual(root, "being-effects"))
    {
        logger->log("Error loading being effects file: " + fileName);
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        else if (xmlNameEqual(node, "effect"))
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

bool EffectManager::triggerDirection(const int id,
                                     Being *const being,
                                     const SpriteDirection::Type &direction)
{
    int rotation;
    switch (direction)
    {
        case SpriteDirection::DOWN:
        case SpriteDirection::DOWNRIGHT:
        case SpriteDirection::DOWNLEFT:
        case SpriteDirection::DEFAULT:
        case SpriteDirection::INVALID:
        default:
            rotation = 0;
            break;
        case SpriteDirection::LEFT:
            rotation = 90;
            break;
        case SpriteDirection::UP:
        case SpriteDirection::UPRIGHT:
        case SpriteDirection::UPLEFT:
            rotation = 180;
            break;
        case SpriteDirection::RIGHT:
            rotation = 270;
            break;
    }

    return trigger(id, being, rotation);
}

bool EffectManager::trigger(const int id,
                            Being *const being,
                            const int rotation)
{
    if (!being || !particleEngine)
        return false;

    BLOCK_START("EffectManager::trigger")
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
    BLOCK_END("EffectManager::trigger")
    return rValue;
}

Particle *EffectManager::triggerReturn(const int id,
                                       Being *const being,
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

bool EffectManager::trigger(const int id,
                            const int x, const int y,
                            const int endTime,
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
            if (!effect.gfx.empty())
            {
                Particle *const particle = particleEngine->addEffect(
                    effect.gfx,
                    x, y,
                    rotation);
                if (particle)
                    mTimers.push_back(ParticleTimer(particle, endTime));
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx);
            // TODO add sprite effect to position
            break;
        }
    }
    return rValue;
}

void EffectManager::triggerDefault(int effectId,
                                   Being *const being,
                                   const int defaultEffectId)
{
    if (effectId == -1)
        effectId = defaultEffectId;
    if (effectId == -1)
        return;
    trigger(effectId, being);
}

void EffectManager::triggerDefault(int effectId,
                                   const int x,
                                   const int y,
                                   const int endTime,
                                   const int defaultEffectId)
{
    if (effectId == -1)
        effectId = defaultEffectId;
    if (effectId == -1)
        return;
    trigger(effectId, x, y, endTime);
}

void EffectManager::logic()
{
    const int time = cur_time;
    bool found(true);
    while (found)
    {
        found = false;
        FOR_EACH (std::list<ParticleTimer>::iterator, it, mTimers)
        {
            const ParticleTimer &timer = *it;
            if (timer.endTime < time)
            {
                found = true;
                Particle *const particle = timer.particle;
                if (particle)
                    particle->kill();
                mTimers.erase(it);
                break;
            }
        }
    }
}

void EffectManager::clear()
{
    FOR_EACH (std::list<ParticleTimer>::iterator, it, mTimers)
    {
        Particle *const particle = (*it).particle;
        if (particle)
            particle->kill();
    }
    mTimers.clear();
}
