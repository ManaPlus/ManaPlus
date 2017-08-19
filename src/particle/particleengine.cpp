/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "configuration.h"

#include "gui/viewport.h"

#include "particle/animationparticle.h"
#include "particle/particleemitter.h"
#include "particle/rotationalparticle.h"
#include "particle/textparticle.h"

#include "resources/dye/dye.h"

#include "resources/loaders/imageloader.h"
#include "resources/loaders/xmlloader.h"

#include "utils/dtor.h"

#include "debug.h"

ParticleEngine *particleEngine = nullptr;

class Image;

int ParticleEngine::particleCount = 0;
int ParticleEngine::maxCount = 0;
int ParticleEngine::fastPhysics = 0;
int ParticleEngine::emitterSkip = 1;
bool ParticleEngine::enabled = true;
const float ParticleEngine::PARTICLE_SKY = 800.0F;

ParticleEngine::ParticleEngine() :
    mChildParticles(),
    mChildMoveParticles(),
    mMap(nullptr)
{
    ParticleEngine::particleCount++;
}

ParticleEngine::~ParticleEngine()
{
    // Delete child emitters and child particles
    clear();
    ParticleEngine::particleCount--;
}

void ParticleEngine::setupEngine() restrict2
{
    ParticleEngine::maxCount = config.getIntValue("particleMaxCount");
    ParticleEngine::fastPhysics = config.getIntValue("particleFastPhysics");
    ParticleEngine::emitterSkip =
        config.getIntValue("particleEmitterSkip") + 1;
    if (ParticleEngine::emitterSkip == 0)
        ParticleEngine::emitterSkip = 1;
    ParticleEngine::enabled = config.getBoolValue("particleeffects");
    logger->log1("Particle engine set up");
}

bool ParticleEngine::update() restrict2
{
    if (mChildParticles.empty() || (mMap == nullptr))
        return true;

    // Update child particles

    const int cameraX = viewport->getCameraX();
    const int cameraY = viewport->getCameraY();
    const float x1 = static_cast<float>(cameraX - 3000);
    const float y1 = static_cast<float>(cameraY - 2000);
    const float x2 = static_cast<float>(cameraX + 3000);
    const float y2 = static_cast<float>(cameraY + 2000);

    for (ParticleIterator p = mChildParticles.begin(),
         fp2 = mChildParticles.end(); p != fp2; )
    {
        Particle *restrict const particle = *p;
        const float posX = particle->mPos.x;
        const float posY = particle->mPos.y;
        if (posX < x1 || posX > x2 || posY < y1 || posY > y2)
        {
            ++p;
            continue;
        }
        // update particle
        if (particle->update())
        {
            ++p;
        }
        else
        {
            mChildMoveParticles.remove(*p);
            delete particle;
            p = mChildParticles.erase(p);
        }
    }
    return true;
}

Particle *ParticleEngine::createChild() restrict2
{
    Particle *const newParticle = new Particle;
    newParticle->setMap(mMap);
    mChildParticles.push_back(newParticle);
    return newParticle;
}

Particle *ParticleEngine::addEffect(const std::string &restrict
                                    particleEffectFile,
                                    const int pixelX,
                                    const int pixelY,
                                    const int rotation) restrict2
{
    Particle *newParticle = nullptr;

    const size_t pos = particleEffectFile.find('|');
    const std::string dyePalettes = (pos != std::string::npos)
        ? particleEffectFile.substr(pos + 1) : "";
    XML::Document *doc = Loader::getXml(
        particleEffectFile.substr(0, pos),
        UseVirtFs_true,
        SkipError_false);
    if (doc == nullptr)
        return nullptr;

    XmlNodeConstPtrConst rootNode = doc->rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "effect"))
    {
        logger->log("Error loading particle: %s", particleEffectFile.c_str());
        doc->decRef();
        return nullptr;
    }

    // Parse particles
    for_each_xml_child_node(effectChildNode, rootNode)
    {
        // We're only interested in particles
        if (!xmlNameEqual(effectChildNode, "particle"))
            continue;

        // Determine the exact particle type
        XmlNodePtr node;

        // Animation
        if ((node = XML::findFirstChildByName(effectChildNode, "animation")) !=
            nullptr)
        {
            newParticle = new AnimationParticle(node, dyePalettes);
            newParticle->setMap(mMap);
        }
        // Rotational
        else if ((node = XML::findFirstChildByName(
                 effectChildNode, "rotation")) != nullptr)
        {
            newParticle = new RotationalParticle(node, dyePalettes);
            newParticle->setMap(mMap);
        }
        // Image
        else if ((node = XML::findFirstChildByName(effectChildNode,
                 "image")) != nullptr)
        {
            std::string imageSrc;
            if (XmlHaveChildContent(node))
                imageSrc = XmlChildContent(node);
            if (!imageSrc.empty() && !dyePalettes.empty())
                Dye::instantiate(imageSrc, dyePalettes);
            Image *const img = Loader::getImage(imageSrc);

            newParticle = new ImageParticle(img);
            newParticle->setMap(mMap);
        }
        // Other
        else
        {
            newParticle = new Particle;
            newParticle->setMap(mMap);
        }

        // Read and set the basic properties of the particle
        const float offsetX = XML::getFloatProperty(
            effectChildNode, "position-x", 0);
        const float offsetY = XML::getFloatProperty(
            effectChildNode, "position-y", 0);
        const float offsetZ = XML::getFloatProperty(
            effectChildNode, "position-z", 0);
        const Vector position(static_cast<float>(pixelX) + offsetX,
            static_cast<float>(pixelY) + offsetY,
            offsetZ);
        newParticle->moveTo(position);

        const int lifetime = XML::getProperty(effectChildNode, "lifetime", -1);
        newParticle->setLifetime(lifetime);
        const bool resizeable = "false" != XML::getProperty(effectChildNode,
            "size-adjustable", "false");

        newParticle->setAllowSizeAdjust(resizeable);

        // Look for additional emitters for this particle
        for_each_xml_child_node(emitterNode, effectChildNode)
        {
            if (xmlNameEqual(emitterNode, "emitter"))
            {
                ParticleEmitter *restrict const newEmitter =
                    new ParticleEmitter(
                    emitterNode,
                    newParticle,
                    mMap,
                    rotation,
                    dyePalettes);
                newParticle->addEmitter(newEmitter);
            }
            else if (xmlNameEqual(emitterNode, "deatheffect"))
            {
                std::string deathEffect;
                if ((node != nullptr) && XmlHaveChildContent(node))
                    deathEffect = XmlChildContent(emitterNode);

                char deathEffectConditions = 0x00;
                if (XML::getBoolProperty(emitterNode, "on-floor", true))
                {
                    deathEffectConditions += CAST_S8(
                        AliveStatus::DEAD_FLOOR);
                }
                if (XML::getBoolProperty(emitterNode, "on-sky", true))
                {
                    deathEffectConditions += CAST_S8(
                        AliveStatus::DEAD_SKY);
                }
                if (XML::getBoolProperty(emitterNode, "on-other", false))
                {
                    deathEffectConditions += CAST_S8(
                        AliveStatus::DEAD_OTHER);
                }
                if (XML::getBoolProperty(emitterNode, "on-impact", true))
                {
                    deathEffectConditions += CAST_S8(
                        AliveStatus::DEAD_IMPACT);
                }
                if (XML::getBoolProperty(emitterNode, "on-timeout", true))
                {
                    deathEffectConditions += CAST_S8(
                        AliveStatus::DEAD_TIMEOUT);
                }
                newParticle->setDeathEffect(
                    deathEffect, deathEffectConditions);
            }
        }

        mChildParticles.push_back(newParticle);
    }

    doc->decRef();
    return newParticle;
}

Particle *ParticleEngine::addTextSplashEffect(const std::string &restrict text,
                                              const int x,
                                              const int y,
                                              const Color *restrict const
                                              color,
                                              Font *restrict const font,
                                              const bool outline) restrict2
{
    Particle *const newParticle = new TextParticle(
        text,
        color,
        font,
        outline);
    newParticle->setMap(mMap);
    newParticle->moveTo(static_cast<float>(x),
        static_cast<float>(y));
    newParticle->setVelocity(
        static_cast<float>((rand() % 100) - 50) / 200.0F,       // X
        static_cast<float>((rand() % 100) - 50) / 200.0F,       // Y
        (static_cast<float>((rand() % 100)) / 200.0F) + 4.0F);  // Z

    newParticle->setGravity(0.1F);
    newParticle->setBounce(0.5F);
    newParticle->setLifetime(200);
    newParticle->setFadeOut(100);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

Particle *ParticleEngine::addTextRiseFadeOutEffect(const std::string &restrict
                                                   text,
                                                   const int x,
                                                   const int y,
                                                   const Color *restrict const
                                                   color,
                                                   Font *restrict const font,
                                                   const bool outline)
                                                   restrict2
{
    Particle *const newParticle = new TextParticle(
        text,
        color,
        font,
        outline);
    newParticle->setMap(mMap);
    newParticle->moveTo(static_cast<float>(x),
        static_cast<float>(y));
    newParticle->setVelocity(0.0F, 0.0F, 0.5F);
    newParticle->setGravity(0.0015F);
    newParticle->setLifetime(300);
    newParticle->setFadeOut(100);
    newParticle->setFadeIn(0);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

void ParticleEngine::clear() restrict2
{
    delete_all(mChildParticles);
    mChildParticles.clear();
    mChildMoveParticles.clear();
}
