/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "statuseffect.h"

#include "configuration.h"
#include "soundmanager.h"

#include "const/resources/spriteaction.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "particle/particleengine.h"

#include "resources/sprite/animatedsprite.h"

#include "debug.h"

StatusEffect::StatusEffect() :
    mMessage(),
    mSFXEffect(),
    mStartParticleEffect(),
    mParticleEffect(),
    mIcon(),
    mAction(),
    mName(),
    mIsPersistent(false),
    mIsPoison(false),
    mIsCart(false),
    mIsRiding(false),
    mIsTrickDead(false),
    mIsPostDelay(false)
{
}

StatusEffect::~StatusEffect()
{
}

void StatusEffect::playSFX() const
{
    if (!mSFXEffect.empty())
        soundManager.playSfx(mSFXEffect, 0, 0);
}

void StatusEffect::deliverMessage() const
{
    if (!mMessage.empty() &&
        localChatTab != nullptr)
    {
        localChatTab->chatLog(mMessage,
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }
}

Particle *StatusEffect::getStartParticle() const
{
    if (particleEngine == nullptr || mStartParticleEffect.empty())
        return nullptr;
    return particleEngine->addEffect(mStartParticleEffect, 0, 0, 0);
}

Particle *StatusEffect::getParticle() const
{
    if (particleEngine == nullptr || mParticleEffect.empty())
        return nullptr;
    return particleEngine->addEffect(mParticleEffect, 0, 0, 0);
}

AnimatedSprite *StatusEffect::getIcon() const
{
    if (mIcon.empty())
    {
        return nullptr;
    }
    return AnimatedSprite::load(
        pathJoin(paths.getStringValue("sprites"), mIcon),
        0);
}

std::string StatusEffect::getAction() const
{
    if (mAction.empty())
        return SpriteAction::INVALID;
    return mAction;
}
