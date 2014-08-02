/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "being/actorsprite.h"

#include "animatedsprite.h"
#include "configuration.h"
#include "imagesprite.h"
#include "statuseffect.h"

#include "being/localplayer.h"

#include "gui/theme.h"

#include "listeners/actorspritelistener.h"

#include "particle/particle.h"

#include "resources/resourcemanager.h"
#include "resources/spritereference.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/timer.h"

#include "debug.h"

#define for_each_cursors() \
    for (int size = TargetCursorSize::SMALL; \
         size < TargetCursorSize::NUM_TC; \
         size ++) \
    { \
        for (int type = TargetCursorType::NORMAL; \
             type < TargetCursorType::NUM_TCT; \
             type ++) \

#define end_foreach }

AnimatedSprite *ActorSprite::targetCursor[TargetCursorType::NUM_TCT]
    [TargetCursorSize::NUM_TC];
bool ActorSprite::loaded = false;

ActorSprite::ActorSprite(const int id) :
    CompoundSprite(),
    Actor(),
    mStatusEffects(),
    mStunParticleEffects(),
    mStatusParticleEffects(&mStunParticleEffects, false),
    mChildParticleEffects(&mStatusParticleEffects, false),
    mId(id),
    mStunMode(0),
    mUsedTargetCursor(nullptr),
    mActorSpriteListeners(),
    mCursorPaddingX(0),
    mCursorPaddingY(0),
    mMustResetParticles(false),
    mPoison(false)
{
}

ActorSprite::~ActorSprite()
{
    mChildParticleEffects.clear();
    mMustResetParticles = true;

    mUsedTargetCursor = nullptr;

    if (localPlayer && localPlayer->getTarget() == this)
        localPlayer->setTarget(nullptr);

    // Notify listeners of the destruction.
    FOR_EACH (ActorSpriteListenerIterator, iter, mActorSpriteListeners)
    {
        if (reportFalse(*iter))
            (*iter)->actorSpriteDestroyed(*this);
    }
}

void ActorSprite::draw1(Graphics *const graphics,
                        const int offsetX, const int offsetY) const
{
    FUNC_BLOCK("ActorSprite::draw1", 1)
    if (mUsedTargetCursor)
    {
        mUsedTargetCursor->update(tick_time * MILLISECONDS_IN_A_TICK);
        mUsedTargetCursor->draw(graphics,
            offsetX + getTargetOffsetX() - mCursorPaddingX,
            offsetY + getTargetOffsetY() - mCursorPaddingY);
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
                = StatusEffect::getStatusEffect(*it, true);
            if (effect && effect->particleEffectIsPersistent())
                updateStatusEffect(*it, true);
        }
    }

    // Update particle effects
    mChildParticleEffects.moveTo(mPos.x, mPos.y);
    BLOCK_END("ActorSprite::logic")
}

void ActorSprite::actorLogic()
{
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

void ActorSprite::setTargetType(const TargetCursorType::Type type)
{
    if (type == TargetCursorType::NONE)
    {
        untarget();
    }
    else
    {
        const size_t sz = static_cast<size_t>(getTargetCursorSize());
        mUsedTargetCursor = targetCursor[static_cast<int>(type)][sz];
        if (mUsedTargetCursor)
        {
            static const int targetWidths[TargetCursorSize::NUM_TC]
                = {0, 0, 0};
            static const int targetHeights[TargetCursorSize::NUM_TC]
                = {-mapTileSize / 2, -mapTileSize / 2, -mapTileSize};

            mCursorPaddingX = static_cast<int>(targetWidths[sz]);
            mCursorPaddingY = static_cast<int>(targetHeights[sz]);
        }
    }
}

struct EffectDescription final
{
    std::string mGFXEffect;
    std::string mSFXEffect;
};

void ActorSprite::setStatusEffect(const int index, const bool active)
{
    const bool wasActive = mStatusEffects.find(index) != mStatusEffects.end();

    if (active != wasActive)
    {
        updateStatusEffect(index, active);
        if (active)
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
        const int index = StatusEffect::blockEffectIndexToEffectIndex(
            offset + i);

        if (index != -1)
            setStatusEffect(index, (newEffects & (1 << i)) > 0);
    }
}

void ActorSprite::updateStunMode(const int oldMode, const int newMode)
{
    handleStatusEffect(StatusEffect::getStatusEffect(oldMode, false), -1);
    handleStatusEffect(StatusEffect::getStatusEffect(newMode, true), -1);
}

void ActorSprite::updateStatusEffect(const int index, const bool newStatus)
{
    StatusEffect *const effect = StatusEffect::getStatusEffect(
        index, newStatus);
    if (!effect)
        return;
    if (effect->isPoison() && getType() == ActorType::PLAYER)
        setPoison(newStatus);
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
                                     const bool forceDisplay,
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
    if (empty() && forceDisplay)
    {
        if (display.image.empty())
        {
            addSprite(AnimatedSprite::delayedLoad(
                paths.getStringValue("sprites").append(
                paths.getStringValue("spriteErrorFile"))));
        }
        else
        {
            ResourceManager *const resman = ResourceManager::getInstance();
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

            Image *img = resman->getImage(imagePath);

            if (!img)
                img = Theme::getImageFromTheme("unknown-item.png");

            addSprite(new ImageSprite(img));
            if (img)
                img->decRef();
        }
    }

    mChildParticleEffects.clear();

    // setup particle effects
    if (Particle::enabled && particleEngine)
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

static const char *cursorType(const int type)
{
    switch (type)
    {
        case TargetCursorType::IN_RANGE:
            return "in-range";
        default:
        case TargetCursorType::NORMAL:
            return "normal";
    }
}

static const char *cursorSize(const int size)
{
    switch (size)
    {
        case TargetCursorSize::LARGE:
            return "l";
        case TargetCursorSize::MEDIUM:
            return "m";
        default:
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
            cursorType(type),
            cursorSize(size))));
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
