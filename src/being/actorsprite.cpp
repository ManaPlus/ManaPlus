/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "being/actorsprite.h"

#include "configuration.h"
#include "statuseffect.h"

#include "being/localplayer.h"

#include "const/utils/timer.h"

#include "gui/theme.h"

#include "listeners/debugmessagelistener.h"

#include "particle/particle.h"

#include "resources/resourcemanager.h"

#include "resources/sprite/animatedsprite.h"
#include "resources/sprite/imagesprite.h"
#include "resources/sprite/spritereference.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/timer.h"

#include "debug.h"

#define for_each_cursors() \
    for (int size = CAST_S32(TargetCursorSize::SMALL); \
         size < CAST_S32(TargetCursorSize::NUM_TC); \
         size ++) \
    { \
        for (int type = CAST_S32(TargetCursorType::NORMAL); \
             type < CAST_S32(TargetCursorType::NUM_TCT); \
             type ++) \

#define end_foreach }

static const unsigned int STATUS_EFFECTS = 32;

AnimatedSprite *ActorSprite::targetCursor
    [CAST_SIZE(TargetCursorType::NUM_TCT)]
    [CAST_SIZE(TargetCursorSize::NUM_TC)];
bool ActorSprite::loaded = false;

ActorSprite::ActorSprite(const BeingId id) :
    CompoundSprite(),
    Actor(),
    mStatusEffects(),
    mStunParticleEffects(),
    mStatusParticleEffects(&mStunParticleEffects, false),
    mChildParticleEffects(&mStatusParticleEffects, false),
    mHorseId(0),
    mId(id),
    mStunMode(0),
    mUsedTargetCursor(nullptr),
    mActorSpriteListeners(),
    mCursorPaddingX(0),
    mCursorPaddingY(0),
    mMustResetParticles(false),
    mPoison(false),
    mHaveCart(false),
    mTrickDead(false)
{
}

ActorSprite::~ActorSprite()
{
    mChildParticleEffects.clear();
    mMustResetParticles = true;

    mUsedTargetCursor = nullptr;

    if (localPlayer && localPlayer != this && localPlayer->getTarget() == this)
        localPlayer->setTarget(nullptr);

    // Notify listeners of the destruction.
    FOR_EACH (ActorSpriteListenerIterator, iter, mActorSpriteListeners)
    {
        if (reportFalse(*iter))
            (*iter)->actorSpriteDestroyed(*this);
    }
}

void ActorSprite::logic()
{
    BLOCK_START("ActorSprite::logic")
    // Update sprite animations
    update(tick_time * MILLISECONDS_IN_A_TICK);

    // Restart status/particle effects, if needed
    if (mMustResetParticles)
    {
        mMustResetParticles = false;
        FOR_EACH (std::set<int>::const_iterator, it, mStatusEffects)
        {
            const StatusEffect *const effect
                = StatusEffect::getStatusEffect(*it, Enable_true);
            if (effect && effect->particleEffectIsPersistent())
                updateStatusEffect(*it, Enable_true);
        }
    }

    // Update particle effects
    mChildParticleEffects.moveTo(mPos.x, mPos.y);
    BLOCK_END("ActorSprite::logic")
}

void ActorSprite::setMap(Map *const map)
{
    Actor::setMap(map);

    // Clear particle effect list because child particles became invalid
    mChildParticleEffects.clear();
    mMustResetParticles = true;  // Reset status particles on next redraw
}

void ActorSprite::controlParticle(Particle *const particle)
{
    mChildParticleEffects.addLocally(particle);
}

void ActorSprite::setTargetType(const TargetCursorTypeT type)
{
    if (type == TargetCursorType::NONE)
    {
        untarget();
    }
    else
    {
        const size_t sz = CAST_SIZE(getTargetCursorSize());
        mUsedTargetCursor = targetCursor[CAST_S32(type)][sz];
        if (mUsedTargetCursor)
        {
            static const int targetWidths[CAST_SIZE(
                TargetCursorSize::NUM_TC)]
                = {0, 0, 0};
            static const int targetHeights[CAST_SIZE(
                TargetCursorSize::NUM_TC)]
                = {-mapTileSize / 2, -mapTileSize / 2, -mapTileSize};

            mCursorPaddingX = CAST_S32(targetWidths[sz]);
            mCursorPaddingY = CAST_S32(targetHeights[sz]);
        }
    }
}

struct EffectDescription final
{
    std::string mGFXEffect;
    std::string mSFXEffect;
};

void ActorSprite::setStatusEffect(const int index, const Enable active)
{
    const Enable wasActive = fromBool(
        mStatusEffects.find(index) != mStatusEffects.end(), Enable);

    if (active != wasActive)
    {
        updateStatusEffect(index, active);
        if (active == Enable_true)
            mStatusEffects.insert(index);
        else
            mStatusEffects.erase(index);
    }
}

void ActorSprite::setStatusEffectBlock(const int offset,
                                       const uint16_t newEffects)
{
    for (unsigned i = 0; i < STATUS_EFFECTS; i++)
    {
        const bool val = (newEffects & (1 << i)) > 0;
        const int index = StatusEffect::blockEffectIndexToEffectIndex(
            offset + i);

        if (index != -1)
        {
            setStatusEffect(index, fromBool(val, Enable));
        }
        else if (val && config.getBoolValue("unimplimentedLog"))
        {
            const std::string str = strprintf(
                "Error: unknown effect by block-index. "
                "Offset: %d, effect int: %d, i: %u",
                offset, CAST_S32(newEffects), i);
            logger->log(str);
            DebugMessageListener::distributeEvent(str);
        }
    }
}

void ActorSprite::updateStunMode(const int oldMode, const int newMode)
{
    handleStatusEffect(StatusEffect::getStatusEffect(
        oldMode, Enable_false), -1);
    handleStatusEffect(StatusEffect::getStatusEffect(
        newMode, Enable_true), -1);
}

void ActorSprite::updateStatusEffect(const int index, const Enable newStatus)
{
    StatusEffect *const effect = StatusEffect::getStatusEffect(
        index, newStatus);
    if (!effect)
        return;
    if (effect->isPoison() && getType() == ActorType::Player)
        setPoison(newStatus == Enable_true);
#ifdef EATHENA_SUPPORT
    else if (effect->isCart() && localPlayer == this)
        setHaveCart(newStatus == Enable_true);
    else if (effect->isRiding())
        setRiding(newStatus == Enable_true);
    else if (effect->isTrickDead())
        setTrickDead(newStatus == Enable_true);
    else if (effect->isPostDelay())
        stopCast(newStatus == Enable_true);
#endif
    handleStatusEffect(effect, index);
}

void ActorSprite::handleStatusEffect(const StatusEffect *const effect,
                                     const int effectId)
{
    if (!effect)
        return;

    Particle *const particle = effect->getParticle();

    if (effectId >= 0)
    {
        mStatusParticleEffects.setLocally(effectId, particle);
    }
    else
    {
        mStunParticleEffects.clearLocally();
        if (particle)
            mStunParticleEffects.addLocally(particle);
    }
}

void ActorSprite::setupSpriteDisplay(const SpriteDisplay &display,
                                     const ForceDisplay forceDisplay,
                                     const int imageType,
                                     const std::string &color)
{
    clear();

    FOR_EACH (SpriteRefs, it, display.sprites)
    {
        if (!*it)
            continue;
        const std::string file = paths.getStringValue("sprites").append(
            combineDye3((*it)->sprite, color));

        const int variant = (*it)->variant;
        addSprite(AnimatedSprite::delayedLoad(file, variant));
    }

    // Ensure that something is shown, if desired
    if (mSprites.empty() && forceDisplay == ForceDisplay_true)
    {
        if (display.image.empty())
        {
            addSprite(AnimatedSprite::delayedLoad(
                paths.getStringValue("sprites").append(
                paths.getStringValue("spriteErrorFile"))));
        }
        else
        {
            std::string imagePath;
            switch (imageType)
            {
                case 0:
                default:
                    imagePath = paths.getStringValue("itemIcons").append(
                        display.image);
                    break;
                case 1:
                    imagePath = paths.getStringValue("itemIcons").append(
                        display.floor);
                    break;
            }
            imagePath = combineDye2(imagePath, color);
            Image *img = resourceManager->getImage(imagePath);

            if (!img)
                img = Theme::getImageFromTheme("unknown-item.png");

            addSprite(new ImageSprite(img));
            if (img)
                img->decRef();
        }
    }

    mChildParticleEffects.clear();

    // setup particle effects
    if (ParticleEngine::enabled && particleEngine)
    {
        FOR_EACH (StringVectCIter, itr, display.particles)
        {
            Particle *const p = particleEngine->addEffect(*itr, 0, 0);
            controlParticle(p);
        }
    }

    mMustResetParticles = true;
}

void ActorSprite::load()
{
    if (loaded)
        unload();

    initTargetCursor();

    loaded = true;
}

void ActorSprite::unload()
{
    if (reportTrue(!loaded))
        return;

    cleanupTargetCursors();
    loaded = false;
}

void ActorSprite::addActorSpriteListener(ActorSpriteListener *const listener)
{
    mActorSpriteListeners.push_front(listener);
}

void ActorSprite::removeActorSpriteListener(ActorSpriteListener *const
                                            listener)
{
    mActorSpriteListeners.remove(listener);
}

static const char *cursorType(const TargetCursorTypeT type)
{
    switch (type)
    {
        case TargetCursorType::IN_RANGE:
            return "in-range";
        default:
        case TargetCursorType::NONE:
        case TargetCursorType::NUM_TCT:
        case TargetCursorType::NORMAL:
            return "normal";
    }
}

static const char *cursorSize(const TargetCursorSizeT size)
{
    switch (size)
    {
        case TargetCursorSize::LARGE:
            return "l";
        case TargetCursorSize::MEDIUM:
            return "m";
        default:
        case TargetCursorSize::NUM_TC:
        case TargetCursorSize::SMALL:
            return "s";
    }
}

void ActorSprite::initTargetCursor()
{
    static const std::string targetCursorFile("target-cursor-%s-%s.xml");

    // Load target cursors
    for_each_cursors()
    {
        targetCursor[type][size] = AnimatedSprite::load(
            Theme::resolveThemePath(strprintf(
            targetCursorFile.c_str(),
            cursorType(static_cast<TargetCursorTypeT>(type)),
            cursorSize(static_cast<TargetCursorSizeT>(size)))));
    }
    end_foreach
}

void ActorSprite::cleanupTargetCursors()
{
    for_each_cursors()
    {
        if (targetCursor[type][size])
            delete2(targetCursor[type][size])
    }
    end_foreach
}
