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
#include "settings.h"
#include "statuseffect.h"

#include "being/localplayer.h"

#include "const/utils/timer.h"

#include "gui/theme.h"

#include "listeners/debugmessagelistener.h"

#include "particle/particle.h"

#include "resources/db/statuseffectdb.h"

#include "resources/loaders/imageloader.h"

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
    mStatusParticleEffects(),
    mChildParticleEffects(&mStatusParticleEffects, false),
    mHorseId(0),
    mId(id),
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
        FOR_EACH (std::set<int32_t>::const_iterator, it, mStatusEffects)
        {
            const StatusEffect *const effect
                = StatusEffectDB::getStatusEffect(*it, Enable_true);
            if (effect && effect->mIsPersistent)
            {
                updateStatusEffect(*it,
                    Enable_true,
                    IsStart_false);
            }
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

void ActorSprite::controlAutoParticle(Particle *const particle)
{
    if (particle)
    {
        particle->setActor(mId);
        mChildParticleEffects.addLocally(particle);
    }
}

void ActorSprite::controlCustomParticle(Particle *const particle)
{
    if (particle)
    {
        // The effect may not die without the beings permission or we segfault
        particle->disableAutoDelete();
        mChildParticleEffects.addLocally(particle);
    }
}

void ActorSprite::controlParticleDeleted(const Particle *const particle)
{
    if (particle)
        mChildParticleEffects.removeLocally(particle);
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

void ActorSprite::setStatusEffect(const int32_t index,
                                  const Enable active,
                                  const IsStart start)
{
    const Enable wasActive = fromBool(
        mStatusEffects.find(index) != mStatusEffects.end(), Enable);

    if (active != wasActive)
    {
        updateStatusEffect(index, active, start);
        if (active == Enable_true)
        {
            mStatusEffects.insert(index);
        }
        else
        {
            mStatusEffects.erase(index);
        }
    }
}

void ActorSprite::setStatusEffectBlock(const int offset,
                                       const uint16_t newEffects)
{
    for (unsigned i = 0; i < STATUS_EFFECTS; i++)
    {
        const bool val = (newEffects & (1 << i)) > 0;
        const int32_t index = StatusEffectDB::blockIdToId(
            offset + i);  // block-id (offset + i) to id (index)

        if (index != -1)
        {
            setStatusEffect(index,
                fromBool(val, Enable),
                IsStart_false);
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

static void applyEffectByOption(ActorSprite *const actor,
                                uint32_t option,
                                const char *const name,
                                const OptionsMap& options)
{
    FOR_EACH (OptionsMapCIter, it, options)
    {
        const uint32_t opt = (*it).first;
        const int32_t id = (*it).second;
        const Enable enable = (opt & option) != 0 ? Enable_true : Enable_false;
        option |= opt;
        option ^= opt;
        actor->setStatusEffect(id,
            enable,
            IsStart_false);
    }
    if (option && config.getBoolValue("unimplimentedLog"))
    {
        const std::string str = strprintf(
            "Error: unknown effect by %s. "
            "Left value: %u",
            name,
            option);
            logger->log(str);
            DebugMessageListener::distributeEvent(str);
    }
}

static void applyEffectByOption1(ActorSprite *const actor,
                                 uint32_t option,
                                 const char *const name,
                                 const OptionsMap& options)
{
    FOR_EACH (OptionsMapCIter, it, options)
    {
        const uint32_t opt = (*it).first;
        const int32_t id = (*it).second;
        if (opt == option)
        {
            actor->setStatusEffect(id,
                Enable_true,
                IsStart_false);
            option = 0U;
        }
        else
        {
            actor->setStatusEffect(id,
                Enable_false,
                IsStart_false);
        }
    }
    if (option && config.getBoolValue("unimplimentedLog"))
    {
        const std::string str = strprintf(
            "Error: unknown effect by %s. "
            "Left value: %u",
            name,
            option);
            logger->log(str);
            DebugMessageListener::distributeEvent(str);
    }
}

void ActorSprite::setStatusEffectOpitons(const uint32_t option,
                                         const uint32_t opt1,
                                         const uint32_t opt2,
                                         const uint32_t opt3)
{
    if (settings.legacyEffects == false)
    {
        applyEffectByOption(this, option, "option",
            StatusEffectDB::getOptionMap());
        applyEffectByOption1(this, opt1, "opt1",
            StatusEffectDB::getOpt1Map());
        applyEffectByOption(this, opt2, "opt2",
            StatusEffectDB::getOpt2Map());
        applyEffectByOption(this, opt3, "opt3",
            StatusEffectDB::getOpt3Map());
    }
    else
    {
        uint32_t statusEffects = opt2;
        statusEffects |= option << 16;
        setStatusEffectBlock(0,
            CAST_U16((statusEffects >> 16) & 0xffffU));
        setStatusEffectBlock(16,
            CAST_U16(statusEffects & 0xffffU));
        setStatusEffectBlock(32,
            CAST_U16(opt3));
    }
}

void ActorSprite::setStatusEffectOpitons(const uint32_t option,
                                         const uint32_t opt1,
                                         const uint32_t opt2)
{
    if (settings.legacyEffects == false)
    {
        applyEffectByOption(this, option, "option",
            StatusEffectDB::getOptionMap());
        applyEffectByOption1(this, opt1, "opt1",
            StatusEffectDB::getOpt1Map());
        applyEffectByOption(this, opt2, "opt2",
            StatusEffectDB::getOpt2Map());
    }
    else
    {
        uint32_t statusEffects = opt2;
        statusEffects |= option << 16;
        setStatusEffectBlock(0,
            CAST_U16((statusEffects >> 16) & 0xffffU));
        setStatusEffectBlock(16,
            CAST_U16(statusEffects & 0xffffU));
    }
}

void ActorSprite::setStatusEffectOpiton0(const uint32_t option)
{
    if (settings.legacyEffects == false)
    {
        applyEffectByOption(this, option, "option",
            StatusEffectDB::getOptionMap());
    }
    else
    {
        const uint32_t statusEffects = option << 16;
        setStatusEffectBlock(0,
            CAST_U16((statusEffects >> 16) & 0xffff));
    }
}

void ActorSprite::updateStatusEffect(const int32_t index,
                                     const Enable newStatus,
                                     const IsStart start)
{
    StatusEffect *const effect = StatusEffectDB::getStatusEffect(
        index, newStatus);
    if (!effect)
        return;
    if (effect->mIsPoison && getType() == ActorType::Player)
        setPoison(newStatus == Enable_true);
    else if (effect->mIsCart && localPlayer == this)
        setHaveCart(newStatus == Enable_true);
    else if (effect->mIsRiding)
        setRiding(newStatus == Enable_true);
    else if (effect->mIsTrickDead)
        setTrickDead(newStatus == Enable_true);
    else if (effect->mIsPostDelay)
        stopCast(newStatus == Enable_true);
    handleStatusEffect(effect, index, newStatus, start);
}

void ActorSprite::handleStatusEffect(const StatusEffect *const effect,
                                     const int32_t effectId,
                                     const Enable newStatus,
                                     const IsStart start)
{
    if (effect == nullptr)
        return;

    if (newStatus == Enable_true)
    {
        if (effectId >= 0)
        {
            Particle *particle = nullptr;
            if (start == IsStart_true)
                particle = effect->getStartParticle();
            if (!particle)
                particle = effect->getParticle();
            if (particle)
                mStatusParticleEffects.setLocally(effectId, particle);
        }
    }
    else
    {
        mStatusParticleEffects.delLocally(effectId);
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
            Image *img = Loader::getImage(imagePath);

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
            controlAutoParticle(p);
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

std::string ActorSprite::getStatusEffectsString() const
{
    std::string effectsStr;
    if (!mStatusEffects.empty())
    {
        FOR_EACH (std::set<int32_t>::const_iterator, it, mStatusEffects)
        {
            const StatusEffect *const effect =
                StatusEffectDB::getStatusEffect(
                *it,
                Enable_true);
            if (!effect)
                continue;
            if (!effectsStr.empty())
                effectsStr.append(", ");
            effectsStr.append(effect->mName);
        }
    }
    return effectsStr;
}
