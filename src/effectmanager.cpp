/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
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

#include "effectmanager.h"

#include "configuration.h"
#include "soundmanager.h"

#include "being/being.h"

#include "utils/checkutils.h"

#include "particle/particle.h"

#include "resources/beingcommon.h"

#include "debug.h"

EffectManager *effectManager = nullptr;

EffectManager::EffectManager() :
    mEffects(),
    mTimers()
{
    logger->log1("Effects are now loading");
    loadXmlFile(paths.getStringValue("effectsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("effectsPatchFile"), SkipError_true);
    loadXmlDir("effectsPatchDir", loadXmlFile)
}

void EffectManager::loadXmlFile(const std::string &fileName,
                                const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtrConst root = doc.rootNode();

    if ((root == nullptr) ||
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
                loadXmlFile(name, skipError);
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
    if ((being == nullptr) || (particleEngine == nullptr) || id == -1)
        return false;

    BLOCK_START("EffectManager::trigger")
    bool rValue = false;
    FOR_EACH (STD_VECTOR<EffectDescription>::const_iterator, i, mEffects)
    {
        const EffectDescription &effect = *i;
        if (effect.id == id)
        {
            rValue = true;
            if (!effect.gfx.empty())
            {
                Particle *const selfFX = particleEngine->addEffect(
                    effect.gfx, 0, 0, rotation);
                being->controlAutoParticle(selfFX);
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx, 0, 0);
            if (!effect.sprite.empty())
                being->addEffect(effect.sprite);
            return rValue;
        }
    }
    reportAlways("Missing effect %d", id)
    BLOCK_END("EffectManager::trigger")
    return rValue;
}

Particle *EffectManager::triggerReturn(const int id,
                                       Being *const being,
                                       const int rotation)
{
    if ((being == nullptr) || (particleEngine == nullptr) || id == -1)
        return nullptr;

    Particle *rValue = nullptr;
    FOR_EACH (STD_VECTOR<EffectDescription>::const_iterator, i, mEffects)
    {
        const EffectDescription &effect = *i;
        if (effect.id == id)
        {
            if (!effect.gfx.empty())
            {
                rValue = particleEngine->addEffect(
                    effect.gfx, 0, 0, rotation);
                being->controlCustomParticle(rValue);
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx, 0, 0);
            if (!effect.sprite.empty())
                being->addEffect(effect.sprite);
            return rValue;
        }
    }
    reportAlways("Missing effect %d", id)
    return rValue;
}

bool EffectManager::trigger(const int id,
                            const int x, const int y,
                            const time_t endTime,
                            const int rotation)
{
    if ((particleEngine == nullptr) || id == -1)
        return false;

    bool rValue = false;
    FOR_EACH (STD_VECTOR<EffectDescription>::const_iterator, i, mEffects)
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
                if (particle != nullptr)
                    mTimers.push_back(ParticleTimer(particle, endTime));
            }
            if (!effect.sfx.empty())
                soundManager.playSfx(effect.sfx, 0, 0);
            return rValue;
        }
    }
    reportAlways("Missing effect %d", id)
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
    trigger(effectId, being, 0);
}

void EffectManager::triggerDefault(int effectId,
                                   const int x,
                                   const int y,
                                   const time_t endTime,
                                   const int defaultEffectId)
{
    if (effectId == -1)
        effectId = defaultEffectId;
    if (effectId == -1)
        return;
    trigger(effectId, x, y, endTime, 0);
}

void EffectManager::logic()
{
    const time_t time = cur_time;
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
                if (particle != nullptr)
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
        if (particle != nullptr)
            particle->kill();
    }
    mTimers.clear();
}
