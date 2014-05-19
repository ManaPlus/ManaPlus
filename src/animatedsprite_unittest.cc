/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "client.h"

#include "gui/theme.h"

#include "resources/animation.h"
#include "resources/resourcemanager.h"
#include "resources/sdlimagehelper.h"
#include "resources/spriteaction.h"

#include "gtest/gtest.h"

#include "utils/physfstools.h"

#include "debug.h"

static void init()
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager *resman = ResourceManager::getInstance();
    resman->addToSearchPath("data", false);
    resman->addToSearchPath("../data", false);

    imageHelper = new SDLImageHelper();
    SDL_SetVideoMode(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
}

TEST(AnimatedSprite, basic)
{
    Client::Options options;
    client = new Client(options);

    init();
    AnimatedSprite *sprite = AnimatedSprite::load(
        "graphics/sprites/error.xml", 0);
    sprite->play(SpriteAction::DEFAULT);

    EXPECT_NE(nullptr, sprite);
    EXPECT_NE(nullptr, sprite->getSprite());
    EXPECT_NE(nullptr, sprite->getAnimation());
    EXPECT_NE(nullptr, sprite->getFrame());
    EXPECT_EQ(0, sprite->getFrameIndex());
    EXPECT_EQ(0, sprite->getFrameTime());
    EXPECT_EQ(false, sprite->update(1));
    EXPECT_EQ(0, sprite->getFrameTime());
    EXPECT_EQ(false, sprite->update(11));
    EXPECT_EQ(10, sprite->getFrameTime());
    EXPECT_EQ(0, sprite->getFrameIndex());
    delete client;
    client = nullptr;
}

TEST(AnimatedSprite, basic2)
{
    Client::Options options;
    client = new Client(options);

    init();
    AnimatedSprite *sprite = AnimatedSprite::load(
        "graphics/sprites/test.xml", 0);
    sprite->play(SpriteAction::STAND);

    EXPECT_EQ(10, const_cast<Animation*>(sprite->getAnimation())
        ->getFrames().size());

    EXPECT_NE(nullptr, sprite);

    EXPECT_EQ(false, sprite->update(1));
    EXPECT_EQ(0, sprite->getFrameTime());
    EXPECT_EQ(10, sprite->getFrame()->delay);

    EXPECT_EQ(false, sprite->update(1 + 10));
    EXPECT_EQ(0, sprite->getFrameIndex());
    EXPECT_EQ(10, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 5));
    EXPECT_EQ(1, sprite->getFrameIndex());
    EXPECT_EQ(5, sprite->getFrameTime());

    EXPECT_EQ(false, sprite->update(1 + 10 + 5));
    EXPECT_EQ(1, sprite->getFrameIndex());
    EXPECT_EQ(5, sprite->getFrameTime());

    EXPECT_EQ(false, sprite->update(1 + 10 + 20));
    EXPECT_EQ(1, sprite->getFrameIndex());
    EXPECT_EQ(20, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 20 + 1));
    EXPECT_EQ(2, sprite->getFrameIndex());
    EXPECT_EQ(1, sprite->getFrameTime());

    EXPECT_EQ(false, sprite->update(1 + 10 + 20 + 10));
    EXPECT_EQ(2, sprite->getFrameIndex());
    EXPECT_EQ(10, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 20 + 10 + 1));
    EXPECT_EQ(4, sprite->getFrameIndex());
    EXPECT_EQ(1, sprite->getFrameTime());

    EXPECT_EQ(false, sprite->update(1 + 10 + 20 + 10 + 25));
    EXPECT_EQ(4, sprite->getFrameIndex());
    EXPECT_EQ(25, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 20 + 10 + 25 + 1));
    EXPECT_EQ(6, sprite->getFrameIndex());
    EXPECT_EQ(1, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 20 + 10 + 25 + 10 + 1));
    EXPECT_EQ(8, sprite->getFrameIndex());
    EXPECT_EQ(1, sprite->getFrameTime());

    EXPECT_EQ(true, sprite->update(1 + 10 + 20 + 10 + 25 + 10 + 10 + 1));
    EXPECT_EQ(4, sprite->getFrameIndex());
    EXPECT_EQ(1, sprite->getFrameTime());

    AnimatedSprite *sprite2 = AnimatedSprite::load(
        "graphics/sprites/test.xml", 0);
    sprite2->play(SpriteAction::SIT);

    EXPECT_EQ(false, sprite2->update(1));
    EXPECT_EQ(2, const_cast<Animation*>(sprite2->getAnimation())
        ->getFrames().size());
    EXPECT_EQ(0, sprite2->getFrameTime());
    EXPECT_EQ(85, sprite2->getFrame()->delay);

    EXPECT_EQ(true, sprite2->update(1 + 10 + 20 + 10 + 25 + 10 + 10 + 1));
    EXPECT_EQ(1, sprite2->getFrameIndex());
    EXPECT_EQ(1, sprite2->getFrameTime());

    delete client;
    client = nullptr;
}
