/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "particle/particle.h"

#include "configuration.h"
#include "logger.h"

#include "gui/viewport.h"

#include "particle/animationparticle.h"
#include "particle/particleemitter.h"
#include "particle/rotationalparticle.h"
#include "particle/textparticle.h"

#include "resources/resourcemanager.h"

#include "resources/animation/simpleanimation.h"

#include "resources/dye/dye.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/mathutils.h"

#include "debug.h"

static const float SIN45 = 0.707106781F;

class Graphics;
class Image;

Particle::Particle() :
    Actor(),
    mAlpha(1.0F),
    mLifetimeLeft(-1),
    mLifetimePast(0),
    mFadeOut(0),
    mFadeIn(0),
    mVelocity(),
    mAlive(AliveStatus::ALIVE),
    mType(ParticleType::Normal),
    mAnimation(nullptr),
    mChildEmitters(),
    mChildParticles(),
    mDeathEffect(),
    mGravity(0.0F),
    mBounce(0.0F),
    mAcceleration(0.0F),
    mInvDieDistance(-1.0F),
    mMomentum(1.0F),
    mTarget(nullptr),
    mRandomness(0),
    mDeathEffectConditions(0x00),
    mAutoDelete(true),
    mAllowSizeAdjust(false),
    mFollow(false)
{
    ParticleEngine::particleCount++;
}

Particle::~Particle()
{
    // Delete child emitters and child particles
    clear();
    delete2(mAnimation);
    ParticleEngine::particleCount--;
}

void Particle::draw(Graphics *restrict const,
                    const int, const int) const restrict2
{
}

void Particle::updateSelf() restrict2
{
    if (mLifetimeLeft == 0 && mAlive == AliveStatus::ALIVE)
        mAlive = AliveStatus::DEAD_TIMEOUT;

    if (mAlive == AliveStatus::ALIVE)
    {
        // calculate particle movement
        if (mMomentum != 1.0F)
            mVelocity *= mMomentum;

        if (mTarget && mAcceleration != 0.0F)
        {
            Vector dist = mPos - mTarget->mPos;
            dist.x *= SIN45;
            float invHypotenuse;

            switch (ParticleEngine::fastPhysics)
            {
                case 1:
                    invHypotenuse = fastInvSqrt(
                        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
                    break;
                case 2:
                    if (!dist.x)
                    {
                        invHypotenuse = 0;
                        break;
                    }

                    invHypotenuse = 2.0F / (static_cast<float>(fabs(dist.x))
                                    + static_cast<float>(fabs(dist.y))
                                    + static_cast<float>(fabs(dist.z)));
                    break;
                default:
                    invHypotenuse = 1.0F / static_cast<float>(sqrt(
                        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z));
                    break;
            }

            if (invHypotenuse)
            {
                if (mInvDieDistance > 0.0F && invHypotenuse > mInvDieDistance)
                    mAlive = AliveStatus::DEAD_IMPACT;
                const float accFactor = invHypotenuse * mAcceleration;
                mVelocity -= dist * accFactor;
            }
        }

        if (mRandomness > 0)
        {
            mVelocity.x += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0F;
            mVelocity.y += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0F;
            mVelocity.z += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0F;
        }

        mVelocity.z -= mGravity;

        // Update position
        mPos.x += mVelocity.x;
        mPos.y += mVelocity.y * SIN45;
        mPos.z += mVelocity.z * SIN45;

        // Update other stuff
        if (mLifetimeLeft > 0)
            mLifetimeLeft--;

        mLifetimePast++;

        if (mPos.z < 0.0F)
        {
            if (mBounce > 0.0F)
            {
                mPos.z *= -mBounce;
                mVelocity *= mBounce;
                mVelocity.z = -mVelocity.z;
            }
            else
            {
                mAlive = AliveStatus::DEAD_FLOOR;
            }
        }
        else if (mPos.z > ParticleEngine::PARTICLE_SKY)
        {
            mAlive = AliveStatus::DEAD_SKY;
        }

        // Update child emitters
        if (ParticleEngine::emitterSkip &&
            (mLifetimePast - 1) % ParticleEngine::emitterSkip == 0)
        {
            FOR_EACH (EmitterConstIterator, e, mChildEmitters)
            {
                std::vector<Particle*> newParticles;
                (*e)->createParticles(mLifetimePast, newParticles);
                FOR_EACH (std::vector<Particle*>::const_iterator,
                          it,
                          newParticles)
                {
                    Particle *const p = *it;
                    p->moveBy(mPos);
                    mChildParticles.push_back(p);
                }
            }
        }
    }

    // create death effect when the particle died
    if (mAlive != AliveStatus::ALIVE &&
        mAlive != AliveStatus::DEAD_LONG_AGO)
    {
        if ((CAST_U32(mAlive) & mDeathEffectConditions)
            > 0x00  && !mDeathEffect.empty())
        {
            Particle *restrict const deathEffect = particleEngine->addEffect(
                mDeathEffect, 0, 0);
            if (deathEffect)
                deathEffect->moveBy(mPos);
        }
        mAlive = AliveStatus::DEAD_LONG_AGO;
    }
}

bool Particle::update() restrict2
{
    const Vector oldPos = mPos;

    updateSelf();

    const Vector change = mPos - oldPos;

    if (mChildParticles.empty())
    {
        if (mAlive != AliveStatus::ALIVE &&
            mAutoDelete)
        {
            return false;
        }
        return true;
    }

    // Update child particles
    for (ParticleIterator p = mChildParticles.begin(),
         fp2 = mChildParticles.end(); p != fp2; )
    {
        Particle *restrict const particle = *p;
        // move particle with its parent if desired
        if (particle->mFollow)
            particle->moveBy(change);

        // update particle
        if (particle->update())
        {
            ++p;
        }
        else
        {
            delete particle;
            p = mChildParticles.erase(p);
        }
    }
    if (mAlive != AliveStatus::ALIVE &&
        mChildParticles.empty() &&
        mAutoDelete)
    {
        return false;
    }

    return true;
}

void Particle::moveBy(const Vector &restrict change) restrict2
{
    mPos += change;
    FOR_EACH (ParticleConstIterator, p, mChildParticles)
    {
        Particle *restrict const particle = *p;
        if (particle->mFollow)
            particle->moveBy(change);
    }
}

void Particle::moveTo(const float x, const float y) restrict2
{
    moveTo(Vector(x, y, mPos.z));
}

Particle *Particle::addEffect(const std::string &restrict particleEffectFile,
                              const int pixelX, const int pixelY,
                              const int rotation) restrict2
{
    Particle *newParticle = nullptr;

    const size_t pos = particleEffectFile.find('|');
    const std::string dyePalettes = (pos != std::string::npos)
        ? particleEffectFile.substr(pos + 1) : "";
    XML::Document doc(particleEffectFile.substr(0, pos),
        UseResman_true,
        SkipError_false);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "effect"))
    {
        logger->log("Error loading particle: %s", particleEffectFile.c_str());
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
        if ((node = XML::findFirstChildByName(effectChildNode, "animation")))
        {
            newParticle = new AnimationParticle(node, dyePalettes);
            newParticle->setMap(mMap);
        }
        // Rotational
        else if ((node = XML::findFirstChildByName(
                 effectChildNode, "rotation")))
        {
            newParticle = new RotationalParticle(node, dyePalettes);
            newParticle->setMap(mMap);
        }
        // Image
        else if ((node = XML::findFirstChildByName(effectChildNode, "image")))
        {
            std::string imageSrc;
            if (XmlHaveChildContent(node))
                imageSrc = XmlChildContent(node);
            if (!imageSrc.empty() && !dyePalettes.empty())
                Dye::instantiate(imageSrc, dyePalettes);
            Image *const img = resourceManager->getImage(imageSrc);

            newParticle = new ImageParticle(img);
            newParticle->setMap(mMap);
        }
        // Other
        else
        {
            newParticle = new Particle();
            newParticle->setMap(mMap);
        }

        // Read and set the basic properties of the particle
        const float offsetX = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-x", 0));
        const float offsetY = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-y", 0));
        const float offsetZ = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-z", 0));
        const Vector position(mPos.x + static_cast<float>(pixelX) + offsetX,
            mPos.y + static_cast<float>(pixelY) + offsetY,
            mPos.z + offsetZ);
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
                if (node && XmlHaveChildContent(node))
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

    return newParticle;
}

void Particle::adjustEmitterSize(const int w, const int h) restrict2
{
    if (mAllowSizeAdjust)
    {
        FOR_EACH (EmitterConstIterator, e, mChildEmitters)
            (*e)->adjustSize(w, h);
    }
}

void Particle::prepareToDie() restrict2
{
    FOR_EACH (ParticleIterator, p, mChildParticles)
    {
        Particle *restrict const particle = *p;
        if (!particle)
            continue;
        particle->prepareToDie();
        if (particle->isAlive() &&
            particle->mLifetimeLeft == -1 &&
            particle->mAutoDelete)
        {
            particle->kill();
        }
    }
}

void Particle::clear() restrict2
{
    delete_all(mChildEmitters);
    mChildEmitters.clear();

    delete_all(mChildParticles);
    mChildParticles.clear();
}
