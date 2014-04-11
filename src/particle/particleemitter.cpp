/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "particle/particleemitter.h"

#include "logger.h"
#include "map.h"

#include "particle/animationparticle.h"
#include "particle/rotationalparticle.h"

#include "resources/dye.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include <cmath>

#include "debug.h"

static const float SIN45 = 0.707106781F;
static const float DEG_RAD_FACTOR = 0.017453293F;

typedef std::vector<ImageSet*>::const_iterator ImageSetVectorCIter;
typedef std::list<ParticleEmitter>::const_iterator ParticleEmitterListCIter;

ParticleEmitter::ParticleEmitter(const XmlNodePtrConst emitterNode,
                                 Particle *const target,
                                 Map *const map, const int rotation,
                                 const std::string& dyePalettes) :
    mParticleTarget(target),
    mMap(map),
    mParticleImage(nullptr),
    mOutputPauseLeft(0),
    mDeathEffectConditions(0),
    mParticleFollow(false)
{
    // Initializing default values
    mParticlePosX.set(0.0F);
    mParticlePosY.set(0.0F);
    mParticlePosZ.set(0.0F);
    mParticleAngleHorizontal.set(0.0F);
    mParticleAngleVertical.set(0.0F);
    mParticlePower.set(0.0F);
    mParticleGravity.set(0.0F);
    mParticleRandomness.set(0);
    mParticleBounce.set(0.0F);
    mParticleAcceleration.set(0.0F);
    mParticleDieDistance.set(-1.0F);
    mParticleMomentum.set(1.0F);
    mParticleLifetime.set(-1);
    mParticleFadeOut.set(0);
    mParticleFadeIn.set(0);
    mOutput.set(1);
    mOutputPause.set(0);
    mParticleAlpha.set(1.0F);

    for_each_xml_child_node(propertyNode, emitterNode)
    {
        if (xmlNameEqual(propertyNode, "property"))
        {
            const std::string name = XML::getProperty(
                propertyNode, "name", "");

            if (name == "position-x")
            {
                mParticlePosX = readParticleEmitterProp(propertyNode, 0.0F);
            }
            else if (name == "position-y")
            {
                mParticlePosY = readParticleEmitterProp(propertyNode, 0.0F);
                mParticlePosY.minVal *= SIN45;
                mParticlePosY.maxVal *= SIN45;
                mParticlePosY.changeAmplitude *= SIN45;
            }
            else if (name == "position-z")
            {
                mParticlePosZ = readParticleEmitterProp(propertyNode, 0.0F);
                mParticlePosZ.minVal *= SIN45;
                mParticlePosZ.maxVal *= SIN45;
                mParticlePosZ.changeAmplitude *= SIN45;
            }
            else if (name == "image")
            {
                std::string image = XML::getProperty(
                    propertyNode, "value", "");
                // Don't leak when multiple images are defined
                if (!image.empty() && !mParticleImage)
                {
                    if (!dyePalettes.empty())
                        Dye::instantiate(image, dyePalettes);

                    ResourceManager *const resman
                        = ResourceManager::getInstance();
                    mParticleImage = resman->getImage(image);
                }
            }
            else if (name == "subimage")
            {
                std::string image = XML::getProperty(
                    propertyNode, "value", "");
                // Don't leak when multiple images are defined
                if (!image.empty() && !mParticleImage)
                {
                    if (!dyePalettes.empty())
                        Dye::instantiate(image, dyePalettes);

                    ResourceManager *const resman
                        = ResourceManager::getInstance();
                    Image *img = resman->getImage(image);
                    if (img)
                    {
                        mParticleImage = resman->getSubImage(img,
                            XML::getProperty(propertyNode, "x", 0),
                            XML::getProperty(propertyNode, "y", 0),
                            XML::getProperty(propertyNode, "width", 0),
                            XML::getProperty(propertyNode, "height", 0));
                        img->decRef();
                    }
                    else
                    {
                        mParticleImage = nullptr;
                    }
                }
            }
            else if (name == "horizontal-angle")
            {
                mParticleAngleHorizontal =
                    readParticleEmitterProp(propertyNode, 0.0F);
                mParticleAngleHorizontal.minVal
                    += static_cast<float>(rotation);
                mParticleAngleHorizontal.minVal *= DEG_RAD_FACTOR;
                mParticleAngleHorizontal.maxVal
                    += static_cast<float>(rotation);
                mParticleAngleHorizontal.maxVal *= DEG_RAD_FACTOR;
                mParticleAngleHorizontal.changeAmplitude *= DEG_RAD_FACTOR;
            }
            else if (name == "vertical-angle")
            {
                mParticleAngleVertical =
                    readParticleEmitterProp(propertyNode, 0.0F);
                mParticleAngleVertical.minVal *= DEG_RAD_FACTOR;
                mParticleAngleVertical.maxVal *= DEG_RAD_FACTOR;
                mParticleAngleVertical.changeAmplitude *= DEG_RAD_FACTOR;
            }
            else if (name == "power")
            {
                mParticlePower = readParticleEmitterProp(propertyNode, 0.0F);
            }
            else if (name == "gravity")
            {
                mParticleGravity = readParticleEmitterProp(propertyNode, 0.0F);
            }
            else if (name == "randomnes"
                     || name == "randomness")  // legacy bug
            {
                mParticleRandomness = readParticleEmitterProp(propertyNode, 0);
            }
            else if (name == "bounce")
            {
                mParticleBounce = readParticleEmitterProp(propertyNode, 0.0F);
            }
            else if (name == "lifetime")
            {
                mParticleLifetime = readParticleEmitterProp(propertyNode, 0);
                mParticleLifetime.minVal += 1;
            }
            else if (name == "output")
            {
                mOutput = readParticleEmitterProp(propertyNode, 0);
                mOutput.maxVal += 1;
            }
            else if (name == "output-pause")
            {
                mOutputPause = readParticleEmitterProp(propertyNode, 0);
                mOutputPauseLeft = mOutputPause.value(0);
            }
            else if (name == "acceleration")
            {
                mParticleAcceleration = readParticleEmitterProp(
                    propertyNode, 0.0F);
            }
            else if (name == "die-distance")
            {
                mParticleDieDistance = readParticleEmitterProp(
                    propertyNode, 0.0F);
            }
            else if (name == "momentum")
            {
                mParticleMomentum = readParticleEmitterProp(
                    propertyNode, 1.0F);
            }
            else if (name == "fade-out")
            {
                mParticleFadeOut = readParticleEmitterProp(propertyNode, 0);
            }
            else if (name == "fade-in")
            {
                mParticleFadeIn = readParticleEmitterProp(propertyNode, 0);
            }
            else if (name == "alpha")
            {
                mParticleAlpha = readParticleEmitterProp(propertyNode, 1.0F);
            }
            else if (name == "follow-parent")
            {
                mParticleFollow = true;
            }
            else
            {
                logger->log("Particle Engine: Warning, "
                            "unknown emitter property \"%s\"",
                            name.c_str());
            }
        }
        else if (xmlNameEqual(propertyNode, "emitter"))
        {
            ParticleEmitter newEmitter(propertyNode, mParticleTarget, map,
                                       rotation, dyePalettes);
            mParticleChildEmitters.push_back(newEmitter);
        }
        else if (xmlNameEqual(propertyNode, "rotation"))
        {
            ImageSet *const imageset = getImageSet(propertyNode);
            if (!imageset)
            {
                logger->log1("Error: no valid imageset");
                continue;
            }
            mTempSets.push_back(imageset);

            // Get animation frames
            for_each_xml_child_node(frameNode, propertyNode)
            {
                const int delay = XML::getIntProperty(
                    frameNode, "delay", 0, 0, 100000);
                const int offsetX = XML::getProperty(frameNode, "offsetX", 0)
                    - imageset->getWidth() / 2 + mapTileSize / 2;
                const int offsetY = XML::getProperty(frameNode, "offsetY", 0)
                    - imageset->getHeight() + mapTileSize;
                const int rand = XML::getIntProperty(
                    frameNode, "rand", 100, 0, 100);

                if (xmlNameEqual(frameNode, "frame"))
                {
                    const int index = XML::getProperty(frameNode, "index", -1);

                    if (index < 0)
                    {
                        logger->log1("No valid value for 'index'");
                        continue;
                    }

                    Image *const img = imageset->get(index);

                    if (!img)
                    {
                        logger->log("No image at index %d", index);
                        continue;
                    }

                    mParticleRotation.addFrame(img, delay,
                        offsetX, offsetY, rand);
                }
                else if (xmlNameEqual(frameNode, "sequence"))
                {
                    int start = XML::getProperty(frameNode, "start", -1);
                    const int end = XML::getProperty(frameNode, "end", -1);

                    if (start < 0 || end < 0)
                    {
                        logger->log1("No valid value for 'start' or 'end'");
                        continue;
                    }

                    while (end >= start)
                    {
                        Image *const img = imageset->get(start);
                        if (!img)
                        {
                            logger->log("No image at index %d", start);
                            continue;
                        }

                        mParticleRotation.addFrame(img, delay,
                            offsetX, offsetY, rand);
                        start ++;
                    }
                }
                else if (xmlNameEqual(frameNode, "end"))
                {
                    mParticleRotation.addTerminator(rand);
                }
            }  // for frameNode
        }
        else if (xmlNameEqual(propertyNode, "animation"))
        {
            ImageSet *const imageset = getImageSet(propertyNode);
            if (!imageset)
            {
                logger->log1("Error: no valid imageset");
                continue;
            }
            mTempSets.push_back(imageset);

            // Get animation frames
            for_each_xml_child_node(frameNode, propertyNode)
            {
                const int delay = XML::getIntProperty(
                    frameNode, "delay", 0, 0, 100000);
                const int offsetX = XML::getProperty(frameNode, "offsetX", 0)
                    - imageset->getWidth() / 2 + mapTileSize / 2;
                const int offsetY = XML::getProperty(frameNode, "offsetY", 0)
                    - imageset->getHeight() + mapTileSize;
                const int rand = XML::getIntProperty(
                    frameNode, "rand", 100, 0, 100);

                if (xmlNameEqual(frameNode, "frame"))
                {
                    const int index = XML::getProperty(frameNode, "index", -1);
                    if (index < 0)
                    {
                        logger->log1("No valid value for 'index'");
                        continue;
                    }

                    Image *const img = imageset->get(index);

                    if (!img)
                    {
                        logger->log("No image at index %d", index);
                        continue;
                    }

                    mParticleAnimation.addFrame(img, delay,
                        offsetX, offsetY, rand);
                }
                else if (xmlNameEqual(frameNode, "sequence"))
                {
                    int start = XML::getProperty(frameNode, "start", -1);
                    const int end = XML::getProperty(frameNode, "end", -1);

                    if (start < 0 || end < 0)
                    {
                        logger->log1("No valid value for 'start' or 'end'");
                        continue;
                    }

                    while (end >= start)
                    {
                        Image *const img = imageset->get(start);

                        if (!img)
                        {
                            logger->log("No image at index %d", start);
                            continue;
                        }

                        mParticleAnimation.addFrame(img, delay,
                            offsetX, offsetY, rand);
                        start++;
                    }
                }
                else if (xmlNameEqual(frameNode, "end"))
                {
                    mParticleAnimation.addTerminator(rand);
                }
            }  // for frameNode
        }
        else if (xmlNameEqual(propertyNode, "deatheffect"))
        {
            mDeathEffect = reinterpret_cast<const char*>(
                propertyNode->xmlChildrenNode->content);
            mDeathEffectConditions = 0x00;
            if (XML::getBoolProperty(propertyNode, "on-floor", true))
            {
                mDeathEffectConditions += static_cast<signed char>(
                    Particle::DEAD_FLOOR);
            }
            if (XML::getBoolProperty(propertyNode, "on-sky", true))
            {
                mDeathEffectConditions += static_cast<signed char>(
                    Particle::DEAD_SKY);
            }
            if (XML::getBoolProperty(propertyNode, "on-other", false))
            {
                mDeathEffectConditions += static_cast<signed char>(
                    Particle::DEAD_OTHER);
            }
            if (XML::getBoolProperty(propertyNode, "on-impact", true))
            {
                mDeathEffectConditions += static_cast<signed char>(
                    Particle::DEAD_IMPACT);
            }
            if (XML::getBoolProperty(propertyNode, "on-timeout", true))
            {
                mDeathEffectConditions += static_cast<signed char>(
                    Particle::DEAD_TIMEOUT);
            }
        }
    }
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &o)
{
    *this = o;
}

ImageSet *ParticleEmitter::getImageSet(XmlNodePtrConst node)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    ImageSet *imageset = nullptr;
    const int subX = XML::getProperty(node, "subX", -1);
    if (subX != -1)
    {
        Image *const img = resman->getImage(XML::getProperty(
            node, "imageset", ""));
        if (!img)
            return nullptr;

        Image *const img2 = resman->getSubImage(img, subX,
            XML::getProperty(node, "subY", 0),
            XML::getProperty(node, "subWidth", 0),
            XML::getProperty(node, "subHeight", 0));
        if (!img2)
        {
            img->decRef();
            return nullptr;
        }

        imageset = resman->getSubImageSet(img2,
            XML::getProperty(node, "width", 0),
            XML::getProperty(node, "height", 0));
        img2->decRef();
        img->decRef();
    }
    else
    {
        imageset = resman->getImageSet(
            XML::getProperty(node, "imageset", ""),
            XML::getProperty(node, "width", 0),
            XML::getProperty(node, "height", 0));
    }
    return imageset;
}

ParticleEmitter & ParticleEmitter::operator=(const ParticleEmitter &o)
{
    mParticlePosX = o.mParticlePosX;
    mParticlePosY = o.mParticlePosY;
    mParticlePosZ = o.mParticlePosZ;
    mParticleAngleHorizontal = o.mParticleAngleHorizontal;
    mParticleAngleVertical = o.mParticleAngleVertical;
    mParticlePower = o.mParticlePower;
    mParticleGravity = o.mParticleGravity;
    mParticleRandomness = o.mParticleRandomness;
    mParticleBounce = o.mParticleBounce;
    mParticleFollow = o.mParticleFollow;
    mParticleTarget = o.mParticleTarget;
    mParticleAcceleration = o.mParticleAcceleration;
    mParticleDieDistance = o.mParticleDieDistance;
    mParticleMomentum = o.mParticleMomentum;
    mParticleLifetime = o.mParticleLifetime;
    mParticleFadeOut = o.mParticleFadeOut;
    mParticleFadeIn = o.mParticleFadeIn;
    mParticleAlpha = o.mParticleAlpha;
    mMap = o.mMap;
    mOutput = o.mOutput;
    mOutputPause = o.mOutputPause;
    mParticleImage = o.mParticleImage;
    mParticleAnimation = o.mParticleAnimation;
    mParticleRotation = o.mParticleRotation;
    mParticleChildEmitters = o.mParticleChildEmitters;
    mDeathEffectConditions = o.mDeathEffectConditions;
    mDeathEffect = o.mDeathEffect;
    mTempSets = o.mTempSets;

    FOR_EACH (ImageSetVectorCIter, i, mTempSets)
    {
        if (*i)
            (*i)->incRef();
    }

    mOutputPauseLeft = 0;

    if (mParticleImage)
        mParticleImage->incRef();

    return *this;
}

ParticleEmitter::~ParticleEmitter()
{
    FOR_EACH (ImageSetVectorCIter, i, mTempSets)
    {
        if (*i)
            (*i)->decRef();
    }
    mTempSets.clear();

    if (mParticleImage)
    {
        mParticleImage->decRef();
        mParticleImage = nullptr;
    }
}

template <typename T> ParticleEmitterProp<T>
ParticleEmitter::readParticleEmitterProp(XmlNodePtrConst propertyNode, T def)
{
    ParticleEmitterProp<T> retval;

    def = static_cast<T>(XML::getFloatProperty(propertyNode, "value",
        static_cast<double>(def)));
    retval.set(static_cast<T>(XML::getFloatProperty(propertyNode, "min",
        static_cast<double>(def))), static_cast<T>(XML::getFloatProperty(
        propertyNode, "max", static_cast<double>(def))));

    const std::string change = XML::getProperty(
        propertyNode, "change-func", "none");
    T amplitude = static_cast<T>(XML::getFloatProperty(propertyNode,
        "change-amplitude", 0.0));

    const int period = XML::getProperty(propertyNode, "change-period", 0);
    const int phase = XML::getProperty(propertyNode, "change-phase", 0);
    if (change == "saw" || change == "sawtooth")
        retval.setFunction(FUNC_SAW, amplitude, period, phase);
    else if (change == "sine" || change == "sinewave")
        retval.setFunction(FUNC_SINE, amplitude, period, phase);
    else if (change == "triangle")
        retval.setFunction(FUNC_TRIANGLE, amplitude, period, phase);
    else if (change == "square")
        retval.setFunction(FUNC_SQUARE, amplitude, period, phase);

    return retval;
}

std::list<Particle *> ParticleEmitter::createParticles(const int tick)
{
    std::list<Particle *> newParticles;

    if (mOutputPauseLeft > 0)
    {
        mOutputPauseLeft --;
        return newParticles;
    }
    mOutputPauseLeft = mOutputPause.value(tick);

    for (int i = mOutput.value(tick); i > 0; i--)
    {
        // Limit maximum particles
        if (Particle::particleCount > Particle::maxCount)
            break;

        Particle *newParticle = nullptr;
        if (mParticleImage)
        {
            const std::string &name = mParticleImage->getIdPath();
            if (ImageParticle::imageParticleCountByName.find(name) ==
                ImageParticle::imageParticleCountByName.end())
            {
                ImageParticle::imageParticleCountByName[name] = 0;
            }

            if (ImageParticle::imageParticleCountByName[name] > 200)
                break;

            newParticle = new ImageParticle(mParticleImage);
            newParticle->setMap(mMap);
        }
        else if (!mParticleRotation.mFrames.empty())
        {
            Animation *const newAnimation = new Animation(mParticleRotation);
            newParticle = new RotationalParticle(newAnimation);
            newParticle->setMap(mMap);
        }
        else if (!mParticleAnimation.mFrames.empty())
        {
            Animation *const newAnimation = new Animation(mParticleAnimation);
            newParticle = new AnimationParticle(newAnimation);
            newParticle->setMap(mMap);
        }
        else
        {
            newParticle = new Particle();
            newParticle->setMap(mMap);
        }

        const Vector position(mParticlePosX.value(tick),
            mParticlePosY.value(tick),
            mParticlePosZ.value(tick));
        newParticle->moveTo(position);

        const float angleH = mParticleAngleHorizontal.value(tick);
        const float cosAngleH = cos(angleH);
        const float sinAngleH = sin(angleH);
        const float angleV = mParticleAngleVertical.value(tick);
        const float cosAngleV = cos(angleV);
        const float sinAngleV = sin(angleV);
        const float power = mParticlePower.value(tick);
        newParticle->setVelocity(cosAngleH * cosAngleV * power,
            sinAngleH * cosAngleV * power,
            sinAngleV * power);

        newParticle->setRandomness(mParticleRandomness.value(tick));
        newParticle->setGravity(mParticleGravity.value(tick));
        newParticle->setBounce(mParticleBounce.value(tick));
        newParticle->setFollow(mParticleFollow);

        newParticle->setDestination(mParticleTarget,
            mParticleAcceleration.value(tick),
            mParticleMomentum.value(tick));

        newParticle->setDieDistance(mParticleDieDistance.value(tick));

        newParticle->setLifetime(mParticleLifetime.value(tick));
        newParticle->setFadeOut(mParticleFadeOut.value(tick));
        newParticle->setFadeIn(mParticleFadeIn.value(tick));
        newParticle->setAlpha(mParticleAlpha.value(tick));

        FOR_EACH (ParticleEmitterListCIter, it,  mParticleChildEmitters)
            newParticle->addEmitter(new ParticleEmitter(*it));

        if (!mDeathEffect.empty())
            newParticle->setDeathEffect(mDeathEffect, mDeathEffectConditions);

        newParticles.push_back(newParticle);
    }

    return newParticles;
}

void ParticleEmitter::adjustSize(const int w, const int h)
{
    if (w == 0 || h == 0)
        return;  // new dimensions are illegal

    // calculate the old rectangle
    const int oldArea = static_cast<int>(
        mParticlePosX.maxVal - mParticlePosX.minVal) * static_cast<int>(
        mParticlePosX.maxVal - mParticlePosY.minVal);
    if (oldArea == 0)
    {
        // when the effect has no dimension it is
        // not designed to be resizeable
        return;
    }

    // set the new dimensions
    mParticlePosX.set(0, static_cast<float>(w));
    mParticlePosY.set(0, static_cast<float>(h));
    const int newArea = w * h;
    // adjust the output so that the particle density stays the same
    const float outputFactor = static_cast<float>(newArea)
        / static_cast<float>(oldArea);
    mOutput.minVal = static_cast<int>(static_cast<float>(
        mOutput.minVal) * outputFactor);
    mOutput.maxVal = static_cast<int>(static_cast<float>(
        mOutput.maxVal) * outputFactor);
}
