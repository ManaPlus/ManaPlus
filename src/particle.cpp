/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "particle.h"

#include "animationparticle.h"
#include "configuration.h"
#include "resources/dye.h"
#include "logger.h"
#include "map.h"
#include "particleemitter.h"
#include "rotationalparticle.h"
#include "textparticle.h"

#include "resources/resourcemanager.h"

#include "gui/sdlfont.h"

#include "utils/dtor.h"
#include "utils/mathutils.h"

#include <guichan/color.hpp>

#include <algorithm>
#include <cmath>

#include "debug.h"

static const float SIN45 = 0.707106781f;

class Graphics;
class Image;

int Particle::particleCount = 0;
int Particle::maxCount = 0;
int Particle::fastPhysics = 0;
int Particle::emitterSkip = 1;
bool Particle::enabled = true;
const float Particle::PARTICLE_SKY = 800.0f;

Particle::Particle(Map *const map) :
    mAlpha(1.0f),
    mLifetimeLeft(-1),
    mLifetimePast(0),
    mFadeOut(0),
    mFadeIn(0),
    mAlive(ALIVE),
    mAutoDelete(true),
    mAllowSizeAdjust(false),
    mDeathEffectConditions(0x00),
    mGravity(0.0f),
    mRandomness(0),
    mBounce(0.0f),
    mFollow(false),
    mTarget(nullptr),
    mAcceleration(0.0f),
    mInvDieDistance(-1.0f),
    mMomentum(1.0f)
{
    setMap(map);
    Particle::particleCount++;
}

Particle::~Particle()
{
    // Delete child emitters and child particles
    clear();
    //update particle count
    Particle::particleCount--;
}

void Particle::setupEngine()
{
    Particle::maxCount = config.getIntValue("particleMaxCount");
    Particle::fastPhysics = config.getIntValue("particleFastPhysics");
    Particle::emitterSkip = config.getIntValue("particleEmitterSkip") + 1;
    if (!Particle::emitterSkip)
        Particle::emitterSkip = 1;
    Particle::enabled = config.getBoolValue("particleeffects");
    disableAutoDelete();
    logger->log1("Particle engine set up");
}

bool Particle::draw(Graphics *, int, int) const
{
    return false;
}

bool Particle::update()
{
    if (!mMap)
        return false;

    if (mLifetimeLeft == 0 && mAlive == ALIVE)
        mAlive = DEAD_TIMEOUT;

    const Vector oldPos = mPos;

    if (mAlive == ALIVE)
    {
        //calculate particle movement
        if (mMomentum != 1.0f)
            mVelocity *= mMomentum;

        if (mTarget && mAcceleration != 0.0f)
        {
            Vector dist = mPos - mTarget->getPosition();
            dist.x *= SIN45;
            float invHypotenuse;

            switch (Particle::fastPhysics)
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

                    invHypotenuse = 2.0f / (static_cast<float>(fabs(dist.x))
                                    + static_cast<float>(fabs(dist.y))
                                    + static_cast<float>(fabs(dist.z)));
                    break;
                default:
                    invHypotenuse = 1.0f / static_cast<float>(sqrt(
                        dist.x * dist.x + dist.y * dist.y + dist.z * dist.z));
                    break;
            }

            if (invHypotenuse)
            {
                if (mInvDieDistance > 0.0f && invHypotenuse > mInvDieDistance)
                    mAlive = DEAD_IMPACT;
                const float accFactor = invHypotenuse * mAcceleration;
                mVelocity -= dist * accFactor;
            }
        }

        if (mRandomness > 0)
        {
            mVelocity.x += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0f;
            mVelocity.y += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0f;
            mVelocity.z += static_cast<float>((rand() % mRandomness - rand()
                           % mRandomness)) / 1000.0f;
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

        if (mPos.z < 0.0f)
        {
            if (mBounce > 0.0f)
            {
                mPos.z *= -mBounce;
                mVelocity *= mBounce;
                mVelocity.z = -mVelocity.z;
            }
            else
            {
                mAlive = DEAD_FLOOR;
            }
        }
        else if (mPos.z > PARTICLE_SKY)
        {
            mAlive = DEAD_SKY;
        }

        // Update child emitters
        if ((mLifetimePast - 1) % Particle::emitterSkip == 0)
        {
            for (EmitterConstIterator e = mChildEmitters.begin(),
                 e2 = mChildEmitters.end(); e != e2; ++ e)
            {
                Particles newParticles = (*e)->createParticles(mLifetimePast);
                for (ParticleConstIterator p = newParticles.begin(),
                     p2 = newParticles.end(); p != p2; ++ p)
                {
                    (*p)->moveBy(mPos);
                    mChildParticles.push_back (*p);
                }
            }
        }
    }

    // create death effect when the particle died
    if (mAlive != ALIVE && mAlive != DEAD_LONG_AGO)
    {
        if ((mAlive & mDeathEffectConditions) > 0x00  && !mDeathEffect.empty())
        {
            Particle *const deathEffect = particleEngine->addEffect(
                mDeathEffect, 0, 0);
            if (deathEffect)
                deathEffect->moveBy(mPos);
        }
        mAlive = DEAD_LONG_AGO;
    }

    const Vector change = mPos - oldPos;

    // Update child particles

    for (ParticleIterator p = mChildParticles.begin(),
         p2 = mChildParticles.end(); p != p2; )
    {
        //move particle with its parent if desired
        if ((*p)->doesFollow())
            (*p)->moveBy(change);

        //update particle
        if ((*p)->update())
        {
            ++p;
        }
        else
        {
            delete (*p);
            p = mChildParticles.erase(p);
        }
    }
    if (mAlive != ALIVE && mChildParticles.empty() && mAutoDelete)
        return false;

    return true;
}

void Particle::moveBy(const Vector &change)
{
    mPos += change;
    for (ParticleConstIterator p = mChildParticles.begin(),
         p2 = mChildParticles.end(); p != p2; ++p)
    {
        if ((*p)->doesFollow())
            (*p)->moveBy(change);
    }
}

void Particle::moveTo(const float x, const float y)
{
    moveTo(Vector(x, y, mPos.z));
}

Particle *Particle::createChild()
{
    Particle *const newParticle = new Particle(mMap);
    mChildParticles.push_back(newParticle);
    return newParticle;
}

Particle *Particle::addEffect(const std::string &particleEffectFile,
                              const int pixelX, const int pixelY,
                              const int rotation)
{
    Particle *newParticle = nullptr;

    size_t pos = particleEffectFile.find('|');
    std::string dyePalettes;
    if (pos != std::string::npos)
        dyePalettes = particleEffectFile.substr(pos + 1);

    XML::Document doc(particleEffectFile.substr(0, pos));
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "effect"))
    {
        logger->log("Error loading particle: %s", particleEffectFile.c_str());
        return nullptr;
    }

    ResourceManager *const resman = ResourceManager::getInstance();

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
            newParticle = new AnimationParticle(mMap, node, dyePalettes);
        }
        // Rotational
        else if ((node = XML::findFirstChildByName(
                 effectChildNode, "rotation")))
        {
            newParticle = new RotationalParticle(mMap, node, dyePalettes);
        }
        // Image
        else if ((node = XML::findFirstChildByName(effectChildNode, "image")))
        {
            std::string imageSrc = reinterpret_cast<const char*>(
                node->xmlChildrenNode->content);
            if (!imageSrc.empty() && !dyePalettes.empty())
                Dye::instantiate(imageSrc, dyePalettes);
            Image *const img = resman->getImage(imageSrc);

            newParticle = new ImageParticle(mMap, img);
        }
        // Other
        else
        {
            newParticle = new Particle(mMap);
        }

        // Read and set the basic properties of the particle
        const float offsetX = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-x", 0));
        const float offsetY = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-y", 0));
        const float offsetZ = static_cast<float>(XML::getFloatProperty(
            effectChildNode, "position-z", 0));
        Vector position (mPos.x + static_cast<float>(pixelX) + offsetX,
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
                ParticleEmitter *newEmitter;
                newEmitter = new ParticleEmitter(emitterNode, newParticle,
                    mMap, rotation, dyePalettes);
                newParticle->addEmitter(newEmitter);
            }
            else if (xmlNameEqual(emitterNode, "deatheffect"))
            {
                std::string deathEffect = reinterpret_cast<const char*>(
                    emitterNode->xmlChildrenNode->content);

                char deathEffectConditions = 0x00;
                if (XML::getBoolProperty(emitterNode, "on-floor", true))
                {
                    deathEffectConditions += static_cast<char>(
                        Particle::DEAD_FLOOR);
                }
                if (XML::getBoolProperty(emitterNode, "on-sky", true))
                {
                    deathEffectConditions += static_cast<char>(
                        Particle::DEAD_SKY);
                }
                if (XML::getBoolProperty(emitterNode, "on-other", false))
                {
                    deathEffectConditions += static_cast<char>(
                        Particle::DEAD_OTHER);
                }
                if (XML::getBoolProperty(emitterNode, "on-impact", true))
                {
                    deathEffectConditions += static_cast<char>(
                        Particle::DEAD_IMPACT);
                }
                if (XML::getBoolProperty(emitterNode, "on-timeout", true))
                {
                    deathEffectConditions += static_cast<char>(
                        Particle::DEAD_TIMEOUT);
                }
                newParticle->setDeathEffect(
                    deathEffect, deathEffectConditions);
            }
        }

        mChildParticles.push_back(newParticle);
    }

    return newParticle;
}

Particle *Particle::addTextSplashEffect(const std::string &text,
                                        const int x, const int y,
                                        const gcn::Color *const color,
                                        gcn::Font *const font,
                                        const bool outline)
{
    Particle *const newParticle = new TextParticle(
        mMap, text, color, font, outline);
    newParticle->moveTo(static_cast<float>(x), static_cast<float>(y));
    newParticle->setVelocity(
        static_cast<float>((rand() % 100) - 50) / 200.0f,    // X
        static_cast<float>((rand() % 100) - 50) / 200.0f,    // Y
        (static_cast<float>((rand() % 100)) / 200.0f) + 4.0f); // Z

    newParticle->setGravity(0.1f);
    newParticle->setBounce(0.5f);
    newParticle->setLifetime(200);
    newParticle->setFadeOut(100);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

Particle *Particle::addTextRiseFadeOutEffect(const std::string &text,
                                             const int x, const int y,
                                             const gcn::Color *const color,
                                             gcn::Font *const font,
                                             const bool outline)
{
    Particle *const newParticle = new TextParticle(
        mMap, text, color, font, outline);
    newParticle->moveTo(static_cast<float>(x), static_cast<float>(y));
    newParticle->setVelocity(0.0f, 0.0f, 0.5f);
    newParticle->setGravity(0.0015f);
    newParticle->setLifetime(300);
    newParticle->setFadeOut(50);
    newParticle->setFadeIn(200);

    mChildParticles.push_back(newParticle);

    return newParticle;
}

void Particle::adjustEmitterSize(const int w, const int h)
{
    if (mAllowSizeAdjust)
    {
        for (EmitterConstIterator e = mChildEmitters.begin(),
             e2 = mChildEmitters.end(); e != e2; ++e)
        {
            (*e)->adjustSize(w, h);
        }
    }
}

void Particle::clear()
{
    delete_all(mChildEmitters);
    mChildEmitters.clear();

    delete_all(mChildParticles);
    mChildParticles.clear();
}
