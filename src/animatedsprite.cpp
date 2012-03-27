/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "animatedsprite.h"

#include "graphics.h"
#include "logger.h"

#include "resources/action.h"
#include "resources/animation.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/xml.h"

#include "debug.h"

AnimatedSprite::AnimatedSprite(SpriteDef *sprite):
    mDirection(DIRECTION_DOWN),
    mLastTime(0),
    mFrameIndex(0),
    mFrameTime(0),
    mSprite(sprite),
    mAction(nullptr),
    mAnimation(nullptr),
    mFrame(nullptr),
    mNumber(100),
    mNumber1(100)
{
    mAlpha = 1.0f;

    // Take possession of the sprite
    if (mSprite)
        mSprite->incRef();

    // Play the stand animation by default
    play(SpriteAction::STAND);
}

AnimatedSprite *AnimatedSprite::load(const std::string &filename, int variant)
{
    ResourceManager *resman = ResourceManager::getInstance();
    SpriteDef *s = resman->getSprite(filename, variant);
    if (!s)
        return nullptr;
    AnimatedSprite *as = new AnimatedSprite(s);
    s->decRef();
    return as;
}

AnimatedSprite::~AnimatedSprite()
{
    if (mSprite)
    {
        mSprite->decRef();
        mSprite = nullptr;
    }
}

bool AnimatedSprite::reset()
{
    bool ret = mFrameIndex !=0 || mFrameTime != 0 || mLastTime != 0;

    mFrameIndex = 0;
    mFrameTime = 0;
    mLastTime = 0;

    return ret;
}

bool AnimatedSprite::play(std::string spriteAction)
{
    if (!mSprite)
        return false;

    Action *action = mSprite->getAction(spriteAction, mNumber);
    if (!action)
        return false;

    mAction = action;
    Animation *animation = mAction->getAnimation(mDirection);

    if (animation && animation != mAnimation && animation->getLength() > 0)
    {
        mAnimation = animation;
        mFrame = mAnimation->getFrame(0);

        reset();

        return true;
    }

    return false;
}

bool AnimatedSprite::update(int time)
{
    // Avoid freaking out at first frame or when tick_time overflows
    if (time < mLastTime || mLastTime == 0)
        mLastTime = time;

    // If not enough time has passed yet, do nothing
    if (time <= mLastTime || !mAnimation)
        return false;

    unsigned int dt = time - mLastTime;
    mLastTime = time;

    Animation *animation = mAnimation;
    Frame *frame = mFrame;

    if (!updateCurrentAnimation(dt))
    {
        // Animation finished, reset to default
        play(SpriteAction::STAND);
    }

    // Make sure something actually changed
    return animation != mAnimation || frame != mFrame;
}

bool AnimatedSprite::updateCurrentAnimation(unsigned int time)
{
    if (!mFrame || !mAnimation || Animation::isTerminator(*mFrame))
        return false;

    mFrameTime += time;

    while ((mFrameTime > static_cast<unsigned>(mFrame->delay)
           && mFrame->delay > 0) || (mFrame->type != Frame::ANIMATION
           && mFrame->type != Frame::PAUSE))
    {
        bool fail(true);
        mFrameTime -= static_cast<unsigned>(mFrame->delay);
        mFrameIndex++;

        if (mFrameIndex >= mAnimation->getLength())
            mFrameIndex = 0;

        mFrame = mAnimation->getFrame(mFrameIndex);
        if (!mFrame || (mFrame->type == Frame::LABEL
            && !mFrame->nextAction.empty()))
        {
            fail = true;
        }
        else if (mFrame->type == Frame::GOTO && !mFrame->nextAction.empty())
        {
            if (mFrame->rand == 100 || rand() % 100 <= mFrame->rand)
            {
                for (unsigned i = 0; i < mAnimation->getLength(); i ++)
                {
                    Frame *frame = mAnimation->getFrame(i);
                    if (frame->type == Frame::LABEL
                        && mFrame->nextAction == frame->nextAction)
                    {
                        mFrameTime = 0;
                        mFrameIndex = i;
                        if (mFrameIndex >= mAnimation->getLength())
                            mFrameIndex = 0;

                        mFrame = mAnimation->getFrame(mFrameIndex);

                        fail = true;
                        break;
                    }
                }
            }
        }
        else if (mFrame->type == Frame::JUMP && !mFrame->nextAction.empty())
        {
            if (mFrame->rand == 100 || rand() % 100 <= mFrame->rand)
            {
                play(mFrame->nextAction);
                return true;
            }
        }
        else if (Animation::isTerminator(*mFrame))
        {
            if (mFrame->rand == 100 || rand() % 100 <= mFrame->rand)
            {
                mAnimation = nullptr;
                mFrame = nullptr;
                return false;
            }
        }
        else
        {
            if (mFrame->rand == 100 || mFrameIndex >= mAnimation->getLength())
            {
                fail = false;
            }
            else
            {
                if (rand() % 100 <= mFrame->rand)
                    fail = false;
            }
        }
        if (fail)
        {
            if (mFrame)
                mFrameTime = mFrame->delay + 1;
            else
                mFrameTime ++;
        }
    }
    return true;
}

bool AnimatedSprite::draw(Graphics *graphics, int posX, int posY) const
{
    if (!mFrame || !mFrame->image)
        return false;

    if (mFrame->image->getAlpha() != mAlpha)
        mFrame->image->setAlpha(mAlpha);

    return graphics->drawImage(mFrame->image,
                               posX + mFrame->offsetX,
                               posY + mFrame->offsetY);
}

bool AnimatedSprite::setSpriteDirection(SpriteDirection direction)
{
    if (mDirection != direction)
    {
        mDirection = direction;

        if (!mAction)
            return false;

        Animation *animation = mAction->getAnimation(mDirection);

        if (animation && animation != mAnimation && animation->getLength() > 0)
        {
            mAnimation = animation;
            mFrame = mAnimation->getFrame(0);
            reset();
        }

        return true;
    }

    return false;
}

unsigned int AnimatedSprite::getCurrentFrame() const
{
    return mFrameIndex;
}

unsigned int AnimatedSprite::getFrameCount() const
{
    if (mAnimation)
        return mAnimation->getLength();
    else
        return 0;
}

int AnimatedSprite::getWidth() const
{
    if (mFrame && mFrame->image)
        return mFrame->image->mBounds.w;
    else
        return 0;
}

int AnimatedSprite::getHeight() const
{
    if (mFrame && mFrame->image)
        return mFrame->image->mBounds.h;
    else
        return 0;
}

std::string AnimatedSprite::getIdPath() const
{
    if (!mSprite)
        return "";
    return mSprite->getIdPath();
}

const Image* AnimatedSprite::getImage() const
{
    return mFrame ? mFrame->image : nullptr;
}

void AnimatedSprite::setAlpha(float alpha)
{
    mAlpha = alpha;

    if (mFrame && mFrame->image && mFrame->image->getAlpha() != mAlpha)
        mFrame->image->setAlpha(mAlpha);
}

void *AnimatedSprite::getHash()
{
    if (mFrame)
        return mFrame;
//    if (mFrame && mFrame->image)
//        return mFrame->image;
//    if (mAnimation)
//        return mAnimation;
    return this;
}

bool AnimatedSprite::updateNumber(unsigned num)
{
    if (mNumber1 != num)
    {
        mNumber1 = num;
        mNumber = mSprite->findNumber(num);
        if (!mNumber)
        {
            mNumber = 100;
            return false;
        }
        return true;
    }
    return false;
}
