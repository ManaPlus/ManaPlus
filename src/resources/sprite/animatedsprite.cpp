/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "resources/sprite/animatedsprite.h"

#include "const/resources/spriteaction.h"

#include "render/graphics.h"

#include "resources/action.h"
#include "resources/delayedmanager.h"

#include "resources/animation/animation.h"

#include "resources/image/image.h"

#include "resources/loaders/spritedefloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sprite/animationdelayload.h"

#include "utils/delete2.h"
#include "utils/likely.h"
#include "utils/mrand.h"

#include "debug.h"

bool AnimatedSprite::mEnableCache = false;

AnimatedSprite::AnimatedSprite(SpriteDef *restrict const sprite) :
    mDirection(SpriteDirection::DOWN),
    mLastTime(0),
    mFrameIndex(0),
    mFrameTime(0),
    mSprite(sprite),
    mAction(nullptr),
    mAnimation(nullptr),
    mFrame(nullptr),
    mNumber(100),
    mNumber1(100),
    mDelayLoad(nullptr),
    mTerminated(false)
{
    mAlpha = 1.0F;

    // Take possession of the sprite
    if (mSprite != nullptr)
        mSprite->incRef();
}

AnimatedSprite *AnimatedSprite::load(const std::string &restrict filename,
                                     const int variant)
{
    SpriteDef *restrict const s = Loader::getSprite(
        filename, variant);
    if (s == nullptr)
        return nullptr;
    AnimatedSprite *restrict const as = new AnimatedSprite(s);
#ifdef DEBUG_ANIMATIONS
    as->setSpriteName(filename);
#endif  // DEBUG_ANIMATIONS

    as->play(SpriteAction::STAND);
    s->decRef();
    return as;
}

AnimatedSprite *AnimatedSprite::delayedLoad(const std::string &restrict
                                            filename,
                                            const int variant)
{
    if (!mEnableCache)
        return load(filename, variant);
    Resource *restrict const res = ResourceManager::getFromCache(
        filename, variant);
    if (res != nullptr)
    {
        res->decRef();
        return load(filename, variant);
    }

    AnimatedSprite *restrict const as = new AnimatedSprite(nullptr);
#ifdef DEBUG_ANIMATIONS
    as->setSpriteName(filename);
#endif  // DEBUG_ANIMATIONS

    as->play(SpriteAction::STAND);
    as->setDelayLoad(filename, variant);
    return as;
}

AnimatedSprite *AnimatedSprite::clone(const AnimatedSprite *restrict const
                                      anim)
{
    if (anim == nullptr)
        return nullptr;
    AnimatedSprite *restrict const sprite = new AnimatedSprite(anim->mSprite);
#ifdef DEBUG_ANIMATIONS
    sprite->setSpriteName(anim->getSpriteName());
#endif  // DEBUG_ANIMATIONS

    sprite->play(SpriteAction::STAND);
    return sprite;
}

AnimatedSprite::~AnimatedSprite()
{
    if (mSprite != nullptr)
    {
        mSprite->decRef();
        mSprite = nullptr;
    }
    if (mDelayLoad != nullptr)
    {
        mDelayLoad->clearSprite();
        DelayedManager::removeDelayLoad(mDelayLoad);
        delete2(mDelayLoad);
    }
}

bool AnimatedSprite::reset() restrict2
{
    const bool ret = mFrameIndex !=0 ||
        mFrameTime != 0 ||
        mLastTime != 0;

    mFrameIndex = 0;
    mFrameTime = 0;
    mLastTime = 0;

    if (mAnimation != nullptr)
        mFrame = &mAnimation->mFrames[0];
    else
        mFrame = nullptr;
    return ret;
}

bool AnimatedSprite::play(const std::string &restrict spriteAction) restrict2
{
    if (mSprite == nullptr)
    {
        if (mDelayLoad == nullptr)
            return false;
        mDelayLoad->setAction(spriteAction);
        return true;
    }

    const Action *const action = mSprite->getAction(spriteAction, mNumber);
    if (action == nullptr)
        return false;

    mAction = action;
    const Animation *const animation = mAction->getAnimation(mDirection);

    if ((animation != nullptr) &&
        animation != mAnimation &&
        animation->getLength() > 0)
    {
        mAnimation = animation;
        reset();

        return true;
    }

    return false;
}

bool AnimatedSprite::update(const int time) restrict2
{
    // Avoid freaking out at first frame or when tick_time overflows
    if (A_UNLIKELY(time < mLastTime || mLastTime == 0))
        mLastTime = time;

    // If not enough time has passed yet, do nothing
    if (time <= mLastTime || (mAnimation == nullptr))
        return false;

    const unsigned int dt = time - mLastTime;
    mLastTime = time;

    const Animation *restrict const animation = mAnimation;
    const Frame *restrict const frame = mFrame;

    if (A_UNLIKELY(!updateCurrentAnimation(dt)))
    {
        // Animation finished, reset to default
        play(SpriteAction::STAND);
        mTerminated = true;
    }

    // Make sure something actually changed
    return animation != mAnimation || frame != mFrame;
}

bool AnimatedSprite::updateCurrentAnimation(const unsigned int time) restrict2
{
    // move code from Animation::isTerminator(*mFrame)
    if (mFrame == nullptr ||
        mAnimation == nullptr ||
        (mFrame->image == nullptr && mFrame->type == FrameType::ANIMATION))
    {
        return false;
    }

    mFrameTime += time;

    while ((mFrameTime > CAST_U32(mFrame->delay) &&
           mFrame->delay > 0) ||
           (mFrame->type != FrameType::ANIMATION &&
           mFrame->type != FrameType::PAUSE))
    {
        bool fail(true);
        mFrameTime -= CAST_U32(mFrame->delay);
        mFrameIndex++;

        if (mFrameIndex >= CAST_U32(mAnimation->getLength()))
            mFrameIndex = 0;

        mFrame = &mAnimation->mFrames[mFrameIndex];
        if ((mFrame->type == FrameType::LABEL &&
            !mFrame->nextAction.empty()))
        {
            fail = false;
        }
        else if (mFrame->type == FrameType::GOTO &&
                 !mFrame->nextAction.empty())
        {
            const int rand = mFrame->rand;
            if (rand == 100 ||
                ((rand != 0) && rand >= mrand() % 100))
            {
                for (size_t i = 0; i < mAnimation->getLength(); i ++)
                {
                    const Frame *restrict const frame =
                        &mAnimation->mFrames[i];
                    if (frame->type == FrameType::LABEL &&
                        mFrame->nextAction == frame->nextAction)
                    {
                        mFrameIndex = CAST_U32(i);
                        if (mFrameIndex >= CAST_U32(
                            mAnimation->getLength()))
                        {
                            mFrameIndex = 0;
                        }

                        mFrame = &mAnimation->mFrames[mFrameIndex];

                        fail = false;
                        break;
                    }
                }
            }
            else
            {
                fail = false;
            }
        }
        else if (mFrame->type == FrameType::JUMP &&
                 !mFrame->nextAction.empty())
        {
            const int rand = mFrame->rand;
            if (rand == 100 ||
                ((rand != 0) && rand >= mrand() % 100))
            {
                play(mFrame->nextAction);
                return true;
            }
        }
        // copy code from Animation::isTerminator(*mFrame)
        else if ((mFrame->image == nullptr) &&
                 mFrame->type == FrameType::ANIMATION)
        {
            const int rand = mFrame->rand;
            if (rand == 100 ||
                ((rand != 0) && rand >= mrand() % 100))
            {
                mAnimation = nullptr;
                mFrame = nullptr;
                return false;
            }
        }
        else
        {
            const int rand = mFrame->rand;
            if (rand == 100 ||
                mFrameIndex >= CAST_U32(mAnimation->getLength()))
            {
                fail = false;
            }
            else
            {
                if ((rand != 0) && mrand() % 100 <= rand)
                    fail = false;
            }
        }
        if (fail)
        {
            if (mFrame != nullptr)
                mFrameTime = mFrame->delay + 1;
            else
                mFrameTime ++;
        }
    }
    return true;
}

void AnimatedSprite::draw(Graphics *restrict const graphics,
                          const int posX,
                          const int posY) const restrict2
{
    FUNC_BLOCK("AnimatedSprite::draw", 1)
    if ((mFrame == nullptr) || (mFrame->image == nullptr))
        return;

    Image *restrict const image = mFrame->image;
    image->setAlpha(mAlpha);
    graphics->drawImage(image,
        posX + mFrame->offsetX, posY + mFrame->offsetY);
}

void AnimatedSprite::drawRaw(Graphics *restrict const graphics,
                             const int posX,
                             const int posY) const restrict2
{
    if ((mFrame == nullptr) || (mFrame->image == nullptr))
        return;

    Image *restrict const image = mFrame->image;
    image->setAlpha(mAlpha);
    graphics->drawImage(image,
        posX,
        posY);
}

bool AnimatedSprite::setSpriteDirection(const SpriteDirection::Type direction)
                                        restrict2
{
    if (mDirection != direction)
    {
        mDirection = direction;

        if (mAction == nullptr)
            return false;

        const Animation *restrict const animation =
            mAction->getAnimation(mDirection);

        if ((animation != nullptr) &&
            animation != mAnimation &&
            animation->getLength() > 0)
        {
            mAnimation = animation;
            reset();
        }

        return true;
    }

    return false;
}

unsigned int AnimatedSprite::getFrameCount() const restrict2
{
    if (mAnimation != nullptr)
        return CAST_U32(mAnimation->getLength());
    return 0;
}

int AnimatedSprite::getWidth() const restrict2
{
    if ((mFrame != nullptr) && (mFrame->image != nullptr))
        return mFrame->image->mBounds.w;
    return 0;
}

int AnimatedSprite::getHeight() const restrict2
{
    if ((mFrame != nullptr) && (mFrame->image != nullptr))
        return mFrame->image->mBounds.h;
    return 0;
}

std::string AnimatedSprite::getIdPath() const restrict2
{
    if (mSprite == nullptr)
        return "";
    return mSprite->mIdPath;
}

const Image* AnimatedSprite::getImage() const restrict2 noexcept2
{
    return mFrame != nullptr ? mFrame->image : nullptr;
}

void AnimatedSprite::setAlpha(float alpha) restrict2
{
    mAlpha = alpha;

    if (mFrame != nullptr)
    {
        Image *restrict const image = mFrame->image;
        if (image != nullptr)
            image->setAlpha(mAlpha);
    }
}

const void *AnimatedSprite::getHash() const restrict2
{
    if (mFrame != nullptr)
        return mFrame;
    return this;
}

bool AnimatedSprite::updateNumber(const unsigned num) restrict2
{
    if (mSprite == nullptr)
        return false;

    if (mNumber1 != num)
    {
        mNumber1 = num;
        mNumber = mSprite->findNumber(num);
        if (mNumber == 0u)
        {
            mNumber = 100;
            return false;
        }
        return true;
    }
    return false;
}

void AnimatedSprite::setDelayLoad(const std::string &restrict filename,
                                  const int variant) restrict2
{
    if (mDelayLoad != nullptr)
    {
        mDelayLoad->clearSprite();
        DelayedManager::removeDelayLoad(mDelayLoad);
        delete mDelayLoad;
    }
    mDelayLoad = new AnimationDelayLoad(filename, variant, this);
    DelayedManager::addDelayedAnimation(mDelayLoad);
}
