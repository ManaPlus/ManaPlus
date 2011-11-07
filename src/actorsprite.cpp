/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "actorsprite.h"
#include "actorspritelistener.h"

#include "client.h"
#include "effectmanager.h"
#include "imagesprite.h"
#include "localplayer.h"
#include "logger.h"
#include "simpleanimation.h"
#include "sound.h"
#include "statuseffect.h"

#include "gui/theme.h"

#include "net/net.h"

#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "debug.h"

#define EFFECTS_FILE "effects.xml"

ImageSet *ActorSprite::targetCursorImages[2][NUM_TC];
SimpleAnimation *ActorSprite::targetCursor[2][NUM_TC];
bool ActorSprite::loaded = false;

ActorSprite::ActorSprite(int id):
    mId(id),
    mStunMode(0),
    mStatusParticleEffects(&mStunParticleEffects, false),
    mChildParticleEffects(&mStatusParticleEffects, false),
    mMustResetParticles(false),
    mUsedTargetCursor(0)
{
}

ActorSprite::~ActorSprite()
{
    setMap(0);

    mUsedTargetCursor = 0;

    if (player_node && player_node->getTarget() == this)
        player_node->setTarget(0);

    // Notify listeners of the destruction.
    for (ActorSpriteListenerIterator iter = mActorSpriteListeners.begin(),
         e = mActorSpriteListeners.end(); iter != e; ++iter)
    {
        if (*iter)
            (*iter)->actorSpriteDestroyed(*this);
    }
}

bool ActorSprite::draw(Graphics *graphics, int offsetX, int offsetY) const
{
    // TODO: Eventually, we probably should fix all sprite offsets so that
    //       these translations aren't necessary anymore. The sprites know
    //       best where their base point should be.
    const int px = getPixelX() + offsetX - 16;
    // Temporary fix to the Y offset.
#ifdef MANASERV_SUPPORT
    const int py = getPixelY() + offsetY -
        ((Net::getNetworkType() == ServerInfo::MANASERV) ? 15 : 32);
#else
    const int py = getPixelY() + offsetY - 32;
#endif

    if (mUsedTargetCursor)
    {
        mUsedTargetCursor->reset();
        mUsedTargetCursor->update(tick_time * MILLISECONDS_IN_A_TICK);
        mUsedTargetCursor->draw(graphics, px + getTargetOffsetX(),
            py + getTargetOffsetY());
    }

    return drawSpriteAt(graphics, px, py);
}

bool ActorSprite::drawSpriteAt(Graphics *graphics, int x, int y) const
{
    return CompoundSprite::draw(graphics, x, y);
}

void ActorSprite::logic()
{
    // Update sprite animations
    update(tick_time * MILLISECONDS_IN_A_TICK);

    // Restart status/particle effects, if needed
    if (mMustResetParticles)
    {
        mMustResetParticles = false;
        for (std::set<int>::const_iterator it = mStatusEffects.begin();
             it != mStatusEffects.end(); ++it)
        {
            const StatusEffect *effect
                = StatusEffect::getStatusEffect(*it, true);
            if (effect && effect->particleEffectIsPersistent())
                updateStatusEffect(*it, true);
        }
    }

    // Update particle effects
    mChildParticleEffects.moveTo(mPos.x, mPos.y);
}

void ActorSprite::actorLogic()
{
}

void ActorSprite::setMap(Map* map)
{
    Actor::setMap(map);

    // Clear particle effect list because child particles became invalid
    mChildParticleEffects.clear();
    mMustResetParticles = true; // Reset status particles on next redraw
}

void ActorSprite::controlParticle(Particle *particle)
{
    mChildParticleEffects.addLocally(particle);
}

void ActorSprite::setTargetType(TargetCursorType type)
{
    if (type == TCT_NONE)
        untarget();
    else
        mUsedTargetCursor = targetCursor[type][getTargetCursorSize()];
}

struct EffectDescription
{
    std::string mGFXEffect;
    std::string mSFXEffect;
};

static EffectDescription *default_effect = 0;
static std::map<int, EffectDescription *> effects;
static bool effects_initialized = false;

static EffectDescription *getEffectDescription(xmlNodePtr node, int *id)
{
    EffectDescription *ed = new EffectDescription;

    *id = atoi(XML::getProperty(node, "id", "-1").c_str());
    ed->mSFXEffect = XML::getProperty(node, "audio", "");
    ed->mGFXEffect = XML::getProperty(node, "particle", "");

    return ed;
}

static EffectDescription *getEffectDescription(int effectId)
{
    if (!effects_initialized)
    {
        XML::Document doc(EFFECTS_FILE);
        xmlNodePtr root = doc.rootNode();

        if (!root || !xmlStrEqual(root->name, BAD_CAST "being-effects"))
        {
            logger->log1("Error loading being effects file: "
                    EFFECTS_FILE);
            return nullptr;
        }

        for_each_xml_child_node(node, root)
        {
            int id;

            if (xmlStrEqual(node->name, BAD_CAST "effect"))
            {
                EffectDescription *EffectDescription =
                    getEffectDescription(node, &id);
                effects[id] = EffectDescription;
            }
            else if (xmlStrEqual(node->name, BAD_CAST "default"))
            {
                EffectDescription *effectDescription =
                    getEffectDescription(node, &id);

                delete default_effect;

                default_effect = effectDescription;
            }
        }

        effects_initialized = true;
    } // done initializing

    EffectDescription *ed = effects[effectId];

    return ed ? ed : default_effect;
}

void ActorSprite::setStatusEffect(int index, bool active)
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

void ActorSprite::setStatusEffectBlock(int offset, Uint16 newEffects)
{
    for (int i = 0; i < STATUS_EFFECTS; i++)
    {
        int index = StatusEffect::blockEffectIndexToEffectIndex(offset + i);

        if (index != -1)
            setStatusEffect(index, (newEffects & (1 << i)) > 0);
    }
}

void ActorSprite::internalTriggerEffect(int effectId, bool sfx, bool gfx)
{
    if (!particleEngine)
        return;

    if (player_node)
    {
        logger->log("Special effect #%d on %s", effectId,
            getId() == player_node->getId() ? "self" : "other");
    }

    EffectDescription *ed = getEffectDescription(effectId);

    if (!ed)
    {
        logger->log1("Unknown special effect and no default recorded");
        return;
    }

    if (gfx && !ed->mGFXEffect.empty() && particleEngine)
    {
        Particle *selfFX;

        selfFX = particleEngine->addEffect(ed->mGFXEffect, 0, 0);
        controlParticle(selfFX);
    }

    if (sfx && !ed->mSFXEffect.empty())
        sound.playSfx(ed->mSFXEffect);
}

void ActorSprite::updateStunMode(int oldMode, int newMode)
{
    handleStatusEffect(StatusEffect::getStatusEffect(oldMode, false), -1);
    handleStatusEffect(StatusEffect::getStatusEffect(newMode, true), -1);
}

void ActorSprite::updateStatusEffect(int index, bool newStatus)
{
    handleStatusEffect(StatusEffect::getStatusEffect(index, newStatus), index);
}

void ActorSprite::handleStatusEffect(StatusEffect *effect, int effectId)
{
    if (!effect)
        return;

    // TODO: Find out how this is meant to be used
    // (SpriteAction != Being::Action)
    //SpriteAction action = effect->getAction();
    //if (action != ACTION_INVALID)
    //    setAction(action);

    Particle *particle = effect->getParticle();

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
                                     bool forceDisplay, int imageType,
                                     std::string color)
{
    clear();

    SpriteRefs it, it_end;

    for (it = display.sprites.begin(), it_end = display.sprites.end();
         it != it_end; ++it)
    {
        if (!*it)
            continue;
        std::string file = "graphics/sprites/"
            + combineDye2((*it)->sprite, color);

        int variant = (*it)->variant;
        addSprite(AnimatedSprite::load(file, variant));
    }

    // Ensure that something is shown, if desired
    if (empty() && forceDisplay)
    {
        if (display.image.empty())
        {
            addSprite(AnimatedSprite::load("graphics/sprites/error.xml"));
        }
        else
        {
            ResourceManager *resman = ResourceManager::getInstance();
            std::string imagePath;
            switch (imageType)
            {
                case 0:
                default:
                    imagePath = "graphics/items/" + display.image;
                    break;
                case 1:
                    imagePath = "graphics/items/" + display.floor;
                    break;
            }
            imagePath = combineDye2(imagePath, color);

            Image *img = resman->getImage(imagePath);

            if (!img)
                img = Theme::getImageFromTheme("unknown-item.png");

            addSprite(new ImageSprite(img));
        }
    }

    mChildParticleEffects.clear();

    //setup particle effects
    if (Particle::enabled && particleEngine)
    {
        std::vector<std::string>::const_iterator itr, itr_end;
        for (itr = display.particles.begin(),
             itr_end = display.particles.end();
             itr != itr_end; ++itr)
        {
            Particle *p = particleEngine->addEffect(*itr, 0, 0);
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
    if (!loaded)
        return;

    cleanupTargetCursors();
    loaded = false;
}

void ActorSprite::addActorSpriteListener(ActorSpriteListener *listener)
{
    mActorSpriteListeners.push_front(listener);
}

void ActorSprite::removeActorSpriteListener(ActorSpriteListener *listener)
{
    mActorSpriteListeners.remove(listener);
}

static const char *cursorType(int type)
{
    switch (type)
    {
        case ActorSprite::TCT_IN_RANGE:
            return "in-range";
        default:
        case ActorSprite::TCT_NORMAL:
            return "normal";
    }
}

static const char *cursorSize(int size)
{
    switch (size)
    {
        case ActorSprite::TC_LARGE:
            return "l";
        case ActorSprite::TC_MEDIUM:
            return "m";
        default:
        case ActorSprite::TC_SMALL:
            return "s";
    }
}

void ActorSprite::initTargetCursor()
{
    static std::string targetCursorFile = "graphics/target-cursor-%s-%s.png";
    static int targetWidths[NUM_TC] = {44, 62, 82};
    static int targetHeights[NUM_TC] = {35, 44, 60};

    // Load target cursors
    for (int size = TC_SMALL; size < NUM_TC; size++)
    {
        for (int type = TCT_NORMAL; type < NUM_TCT; type++)
        {
            loadTargetCursor(strprintf(targetCursorFile.c_str(),
                cursorType(type), cursorSize(size)), targetWidths[size],
                targetHeights[size], type, size);
        }
    }
}

void ActorSprite::cleanupTargetCursors()
{
    for (int size = TC_SMALL; size < NUM_TC; size++)
    {
        for (int type = TCT_NORMAL; type < NUM_TCT; type++)
        {
            if (targetCursor[type][size])
            {
                delete targetCursor[type][size];
                targetCursor[type][size] = 0;
            }
            if (targetCursorImages[type][size])
            {
                targetCursorImages[type][size]->decRef();
                targetCursorImages[type][size] = 0;
            }
        }
    }
}

void ActorSprite::loadTargetCursor(const std::string &filename,
                                   int width, int height, int type, int size)
{
    if (size < TC_SMALL || size >= NUM_TC)
        return;

    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *currentImageSet = resman->getImageSet(filename, width, height);

    if (!currentImageSet)
    {
        logger->log("Error loading target cursor: %s", filename.c_str());
        return;
    }

    Animation *anim = new Animation;

    for (unsigned int i = 0; i < currentImageSet->size(); ++i)
    {
        anim->addFrame(currentImageSet->get(i), 75,
            (16 - (currentImageSet->getWidth() / 2)),
            (16 - (currentImageSet->getHeight() / 2)),
            100);
    }

    SimpleAnimation *currentCursor = new SimpleAnimation(anim);

    if (targetCursor[type][size])
    {
        delete targetCursor[type][size];
        targetCursor[type][size] = 0;
        if (targetCursorImages[type][size])
            targetCursorImages[type][size]->decRef();
    }

    targetCursorImages[type][size] = currentImageSet;
    targetCursor[type][size] = currentCursor;
}
