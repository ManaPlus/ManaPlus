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

#include "particle/particle.h"

#include "actormanager.h"
#include "logger.h"

#include "being/actorsprite.h"

#include "particle/animationparticle.h"
#include "particle/particleemitter.h"
#include "particle/rotationalparticle.h"

#include "resources/animation/simpleanimation.h"

#include "resources/dye/dye.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"
#include "resources/loaders/xmlloader.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/mathutils.h"
#include "utils/mrand.h"

#include "debug.h"

static const float SIN45 = 0.707106781F;
static const double PI = M_PI;
static const float PI2 = 2 * M_PI;

class Graphics;

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
    mImage(nullptr),
    mActor(BeingId_zero),
    mChildEmitters(),
    mChildParticles(),
    mChildMoveParticles(),
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
    if (mActor != BeingId_zero &&
        actorManager)
    {
        ActorSprite *const actor = actorManager->findActor(mActor);
        if (actor)
            actor->controlParticleDeleted(this);
    }
    // Delete child emitters and child particles
    clear();
    delete2(mAnimation);
    if (mImage)
    {
        if (mType == ParticleType::Image)
        {
            const std::string &restrict name = mImage->mIdPath;
            StringIntMapIter it
                = ImageParticle::imageParticleCountByName.find(name);
            if (it != ImageParticle::imageParticleCountByName.end())
            {
                int &cnt = (*it).second;
                if (cnt > 0)
                    cnt --;
            }
            mImage->decRef();
        }
        mImage = nullptr;
    }

    ParticleEngine::particleCount--;
}

void Particle::draw(Graphics *restrict const,
                    const int, const int) const restrict2
{
}

void Particle::updateSelf() restrict2
{
    // calculate particle movement
    if (A_LIKELY(mMomentum != 1.0F))
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

    if (A_LIKELY(mRandomness >= 10))  // reduce useless calculations
    {
        const int rand2 = mRandomness * 2;
        mVelocity.x += static_cast<float>(mrand() % rand2 - mRandomness)
            / 1000.0F;
        mVelocity.y += static_cast<float>(mrand() % rand2 - mRandomness)
            / 1000.0F;
        mVelocity.z += static_cast<float>(mrand() % rand2 - mRandomness)
            / 1000.0F;
    }

    mVelocity.z -= mGravity;

    // Update position
    mPos.x += mVelocity.x;
    mPos.y += mVelocity.y * SIN45;
    mPos.z += mVelocity.z * SIN45;

    // Update other stuff
    if (A_LIKELY(mLifetimeLeft > 0))
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
                if (p->mFollow)
                    mChildMoveParticles.push_back(p);
            }
        }
    }

    // create death effect when the particle died
    if (A_UNLIKELY(mAlive != AliveStatus::ALIVE &&
        mAlive != AliveStatus::DEAD_LONG_AGO))
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
    if (A_LIKELY(mAlive == AliveStatus::ALIVE))
    {
        if (A_UNLIKELY(mLifetimeLeft == 0))
        {
            mAlive = AliveStatus::DEAD_TIMEOUT;
            if (mChildParticles.empty())
            {
                if (mAutoDelete)
                    return false;
                return true;
            }
        }
        else
        {
            if (mAnimation)
            {
                if (mType == ParticleType::Animation)
                {
                    // particle engine is updated every 10ms
                    mAnimation->update(10);
                }
                else  // ParticleType::Rotational
                {
                    // TODO: cache velocities to avoid spamming atan2()
                    const int size = mAnimation->getLength();
                    if (!size)
                        return false;

                    float rad = static_cast<float>(atan2(mVelocity.x,
                        mVelocity.y));
                    if (rad < 0)
                        rad = PI2 + rad;

                    const float range = static_cast<const float>(PI / size);

                    // Determines which frame the particle should play
                    if (A_UNLIKELY(rad < range || rad > PI2 - range))
                    {
                        mAnimation->setFrame(0);
                    }
                    else
                    {
                        const float range2 = 2 * range;
                        // +++ need move condition outside of for
                        for (int c = 1; c < size; c++)
                        {
                            const float cRange = static_cast<float>(c) *
                                range2;
                            if (cRange - range < rad &&
                                rad < cRange + range)
                            {
                                mAnimation->setFrame(c);
                                break;
                            }
                        }
                    }
                }
                mImage = mAnimation->getCurrentImage();
            }
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
            for (ParticleIterator p = mChildMoveParticles.begin(),
                 fp2 = mChildMoveParticles.end(); p != fp2; )
            {
                // move particle with its parent if desired
                (*p)->moveBy(change);
                ++p;
            }
        }

        // Update child particles
        for (ParticleIterator p = mChildParticles.begin(),
             fp2 = mChildParticles.end(); p != fp2; )
        {
            Particle *restrict const particle = *p;
            // update particle
            if (A_LIKELY(particle->update()))
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
        if (A_UNLIKELY(mAlive != AliveStatus::ALIVE &&
            mChildParticles.empty() &&
            mAutoDelete))
        {
            return false;
        }
    }
    else
    {
        if (mChildParticles.empty())
        {
            if (mAutoDelete)
                return false;
            return true;
        }
        // Update child particles
        for (ParticleIterator p = mChildParticles.begin(),
             fp2 = mChildParticles.end(); p != fp2; )
        {
            Particle *restrict const particle = *p;
            // update particle
            if (A_LIKELY(particle->update()))
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
        if (A_UNLIKELY(mChildParticles.empty() &&
            mAutoDelete))
        {
            return false;
        }
    }

    return true;
}

void Particle::moveBy(const Vector &restrict change) restrict2
{
    mPos += change;
    FOR_EACH (ParticleConstIterator, p, mChildMoveParticles)
    {
        (*p)->moveBy(change);
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
    XML::Document *doc = Loader::getXml(particleEffectFile.substr(0, pos),
        UseVirtFs_true,
        SkipError_false);
    if (!doc)
        return nullptr;
    XmlNodeConstPtrConst rootNode = doc->rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "effect"))
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

    doc->decRef();
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

    mChildMoveParticles.clear();
}
