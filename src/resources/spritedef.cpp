/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "resources/spritedef.h"

#include "logger.h"
#include "map.h"

#include "resources/action.h"
#include "resources/animation.h"
#include "resources/dye.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "configuration.h"

#include "debug.h"

SpriteReference *SpriteReference::Empty = nullptr;
extern int serverVersion;

const Action *SpriteDef::getAction(const std::string &action,
                                   const unsigned num) const
{
    Actions::const_iterator i = mActions.find(num);
    if (i == mActions.end() && num != 100)
        i = mActions.find(100);

    if (i == mActions.end() || !(*i).second)
        return nullptr;

    const ActionMap *const actMap = (*i).second;
    if (!actMap)
        return nullptr;
    const ActionMap::const_iterator it = actMap->find(action);

    if (it == actMap->end())
    {
        logger->log("Warning: no action \"%s\" defined!", action.c_str());
        return nullptr;
    }

    return (*it).second;
}

unsigned SpriteDef::findNumber(const unsigned num) const
{
    unsigned min = 101;
    FOR_EACH (Actions::const_iterator, it, mActions)
    {
        const unsigned n = (*it).first;
        if (n >= num && n < min)
            min = n;
    }
    if (min == 101)
        return 0;
    return min;
}

SpriteDef *SpriteDef::load(const std::string &animationFile,
                           const int variant, const bool prot)
{
    const size_t pos = animationFile.find('|');
    std::string palettes;
    if (pos != std::string::npos)
        palettes = animationFile.substr(pos + 1);

    XML::Document doc(animationFile.substr(0, pos));
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "sprite"))
    {
        logger->log("Error, failed to parse %s", animationFile.c_str());

        const std::string errorFile = paths.getStringValue("sprites").append(
            paths.getStringValue("spriteErrorFile"));
        if (animationFile != errorFile)
            return load(errorFile, 0, prot);
        else
            return nullptr;
    }

    SpriteDef *const def = new SpriteDef;
    def->mProcessedFiles.insert(animationFile);
    def->loadSprite(rootNode, variant, palettes);
    def->substituteActions();
    if (serverVersion < 1)
        def->fixDeadAction();
    if (prot)
    {
        def->incRef();
        def->setProtected(true);
    }
    return def;
}

void SpriteDef::fixDeadAction()
{
    FOR_EACH (ActionsIter, it, mActions)
    {
        ActionMap *const d = (*it).second;
        if (!d)
            continue;
        const ActionMap::iterator i = d->find(SpriteAction::DEAD);
        const ActionMap::iterator i2 = d->find(SpriteAction::STAND);
        // search dead action and check what it not same with stand action
        if (i != d->end() && i->second && i->second != i2->second)
            (i->second)->setLastFrameDelay(0);
    }
}

void SpriteDef::substituteAction(const std::string &restrict complete,
                                 const std::string &restrict with)
{
    FOR_EACH (ActionsConstIter, it, mActions)
    {
        ActionMap *const d = (*it).second;
        if (!d)
            continue;
        if (d->find(complete) == d->end())
        {
            const ActionMap::iterator i = d->find(with);
            if (i != d->end())
                (*d)[complete] = i->second;
        }
    }
}

void SpriteDef::substituteActions()
{
    substituteAction(SpriteAction::STAND, SpriteAction::DEFAULT);
    substituteAction(SpriteAction::MOVE, SpriteAction::STAND);
    substituteAction(SpriteAction::ATTACK, SpriteAction::STAND);
    substituteAction(SpriteAction::CAST_MAGIC, SpriteAction::ATTACK);
    substituteAction(SpriteAction::USE_ITEM, SpriteAction::CAST_MAGIC);
    substituteAction(SpriteAction::SIT, SpriteAction::STAND);
    substituteAction(SpriteAction::SITTOP, SpriteAction::SIT);
    substituteAction(SpriteAction::SLEEP, SpriteAction::SIT);
    substituteAction(SpriteAction::HURT, SpriteAction::STAND);
    substituteAction(SpriteAction::DEAD, SpriteAction::HURT);
    substituteAction(SpriteAction::SPAWN, SpriteAction::STAND);
    substituteAction(SpriteAction::FLY, SpriteAction::MOVE);
    substituteAction(SpriteAction::SWIM, SpriteAction::MOVE);
    substituteAction(SpriteAction::STANDSKY, SpriteAction::STAND);
    substituteAction(SpriteAction::STANDWATER, SpriteAction::STAND);
    substituteAction(SpriteAction::SITSKY, SpriteAction::SIT);
    substituteAction(SpriteAction::SITWATER, SpriteAction::SIT);
    substituteAction(SpriteAction::ATTACKSKY, SpriteAction::ATTACK);
    substituteAction(SpriteAction::ATTACKWATER, SpriteAction::ATTACK);
    substituteAction(SpriteAction::SPAWNSKY, SpriteAction::SPAWN);
    substituteAction(SpriteAction::SPAWNWATER, SpriteAction::SPAWN);
    substituteAction(SpriteAction::DEADSKY, SpriteAction::DEAD);
    substituteAction(SpriteAction::DEADWATER, SpriteAction::DEAD);
}

void SpriteDef::loadSprite(const XmlNodePtr spriteNode, const int variant,
                           const std::string &palettes)
{
    // Get the variant
    const int variantCount = XML::getProperty(spriteNode, "variants", 0);
    int variant_offset = 0;

    if (variantCount > 0 && variant < variantCount)
    {
        variant_offset =
            variant * XML::getProperty(spriteNode, "variant_offset", 0);
    }

    for_each_xml_child_node(node, spriteNode)
    {
        if (xmlNameEqual(node, "imageset"))
            loadImageSet(node, palettes);
        else if (xmlNameEqual(node, "action"))
            loadAction(node, variant_offset);
        else if (xmlNameEqual(node, "include"))
            includeSprite(node);
    }
}

void SpriteDef::loadImageSet(const XmlNodePtr node,
                             const std::string &palettes)
{
    const std::string name = XML::getProperty(node, "name", "");

    // We don't allow redefining image sets. This way, an included sprite
    // definition will use the already loaded image set with the same name.
    if (mImageSets.find(name) != mImageSets.end())
        return;

    const int width = XML::getProperty(node, "width", 0);
    const int height = XML::getProperty(node, "height", 0);
    std::string imageSrc = XML::getProperty(node, "src", "");
    Dye::instantiate(imageSrc, palettes);

    ResourceManager *const resman = ResourceManager::getInstance();
    ImageSet *const imageSet = resman->getImageSet(imageSrc, width, height);

    if (!imageSet)
    {
        logger->log1("Couldn't load imageset!");
        return;
    }

    imageSet->setOffsetX(XML::getProperty(node, "offsetX", 0));
    imageSet->setOffsetY(XML::getProperty(node, "offsetY", 0));
    mImageSets[name] = imageSet;
}

void SpriteDef::loadAction(const XmlNodePtr node, const int variant_offset)
{
    const std::string actionName = XML::getProperty(node, "name", "");
    const std::string imageSetName = XML::getProperty(node, "imageset", "");
    const unsigned hp = XML::getProperty(node, "hp", 100);

    const ImageSetIterator si = mImageSets.find(imageSetName);
    if (si == mImageSets.end())
    {
        logger->log("Warning: imageset \"%s\" not defined in %s",
            imageSetName.c_str(), getIdPath().c_str());
        return;
    }
    const ImageSet *const imageSet = si->second;

    if (actionName == SpriteAction::INVALID)
    {
        logger->log("Warning: Unknown action \"%s\" defined in %s",
            actionName.c_str(), getIdPath().c_str());
        return;
    }
    Action *const action = new Action;
    action->setNumber(hp);
    addAction(hp, actionName, action);

    // dirty hack to fix bad resources in tmw server
    if (actionName == "attack_stab")
        addAction(hp, "attack", action);

    // When first action set it as default direction
    const Actions::const_iterator i = mActions.find(hp);
    if ((*i).second->size() == 1)
        addAction(hp, SpriteAction::DEFAULT, action);

    // Load animations
    for_each_xml_child_node(animationNode, node)
    {
        if (xmlNameEqual(animationNode, "animation"))
            loadAnimation(animationNode, action, imageSet, variant_offset);
    }
}

void SpriteDef::loadAnimation(const XmlNodePtr animationNode,
                              Action *const action,
                              const ImageSet *const imageSet,
                              const int variant_offset) const
{
    if (!action || !imageSet)
        return;

    const std::string directionName =
        XML::getProperty(animationNode, "direction", "");
    const SpriteDirection directionType = makeSpriteDirection(directionName);

    if (directionType == DIRECTION_INVALID)
    {
        logger->log("Warning: Unknown direction \"%s\" used in %s",
                directionName.c_str(), getIdPath().c_str());
        return;
    }

    Animation *const animation = new Animation;
    action->setAnimation(directionType, animation);

    // Get animation frames
    for_each_xml_child_node(frameNode, animationNode)
    {
        const int delay = XML::getIntProperty(
            frameNode, "delay", 0, 0, 100000);
        const int offsetX = XML::getProperty(frameNode, "offsetX", 0)
            + imageSet->getOffsetX() - imageSet->getWidth() / 2
            + mapTileSize / 2;
        const int offsetY = XML::getProperty(frameNode, "offsetY", 0)
            + imageSet->getOffsetY() - imageSet->getHeight() + mapTileSize;
        const int rand = XML::getIntProperty(frameNode, "rand", 100, 0, 100);

        if (xmlNameEqual(frameNode, "frame"))
        {
            const int index = XML::getProperty(frameNode, "index", -1);

            if (index < 0)
            {
                logger->log1("No valid value for 'index'");
                continue;
            }

            Image *const img = imageSet->get(index + variant_offset);

            if (!img)
            {
                logger->log("No image at index %d", index + variant_offset);
                continue;
            }

            animation->addFrame(img, delay, offsetX, offsetY, rand);
        }
        else if (xmlNameEqual(frameNode, "sequence"))
        {
            const int start = XML::getProperty(frameNode, "start", -1);
            const int end = XML::getProperty(frameNode, "end", -1);
            const std::string value = XML::getProperty(frameNode, "value", "");
            const int repeat = XML::getIntProperty(
                frameNode, "repeat", 1, 0, 100);

            if (repeat < 1)
            {
                logger->log1("No valid value for 'repeat'");
                continue;
            }

            if (value.empty())
            {
                if (addSequence(start, end, delay, offsetX, offsetY,
                    variant_offset, repeat, rand, imageSet, animation))
                {
                    continue;
                }
            }
            else
            {
                StringVect vals;
                splitToStringVector(vals, value, ',');
                FOR_EACH (StringVectCIter, it, vals)
                {
                    const std::string str = *it;
                    const size_t idx = str.find("-");
                    if (str == "p")
                    {
                        animation->addPause(delay, rand);
                    }
                    else if (idx != std::string::npos)
                    {
                        const int v1 = atoi(str.substr(0, idx).c_str());
                        const int v2 = atoi(str.substr(idx + 1).c_str());
                        addSequence(v1, v2, delay, offsetX, offsetY,
                            variant_offset, repeat, rand, imageSet, animation);
                    }
                    else
                    {
                        Image *const img = imageSet->get(atoi(
                            str.c_str()) + variant_offset);
                        if (img)
                        {
                            animation->addFrame(img, delay,
                                offsetX, offsetY, rand);
                        }
                    }
                }
            }
        }
        else if (xmlNameEqual(frameNode, "pause"))
        {
            animation->addPause(delay, rand);
        }
        else if (xmlNameEqual(frameNode, "end"))
        {
            animation->addTerminator(rand);
        }
        else if (xmlNameEqual(frameNode, "jump"))
        {
            animation->addJump(XML::getProperty(
                frameNode, "action", ""), rand);
        }
        else if (xmlNameEqual(frameNode, "label"))
        {
            const std::string name = XML::getProperty(frameNode, "name", "");
            if (!name.empty())
                animation->addLabel(name);
        }
        else if (xmlNameEqual(frameNode, "goto"))
        {
            const std::string name = XML::getProperty(frameNode, "label", "");
            if (!name.empty())
                animation->addGoto(name, rand);
        }
    }  // for frameNode
}

void SpriteDef::includeSprite(const XmlNodePtr includeNode)
{
    std::string filename = XML::getProperty(includeNode, "file", "");

    if (filename.empty())
        return;
    filename = paths.getStringValue("sprites").append(filename);

    if (mProcessedFiles.find(filename) != mProcessedFiles.end())
    {
        logger->log("Error, Tried to include %s which already is included.",
            filename.c_str());
        return;
    }
    mProcessedFiles.insert(filename);

    XML::Document doc(filename);
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "sprite"))
    {
        logger->log("Error, no sprite root node in %s", filename.c_str());
        return;
    }

    loadSprite(rootNode, 0);
}

SpriteDef::~SpriteDef()
{
    // Actions are shared, so ensure they are deleted only once.
    std::set<Action*> actions;
    FOR_EACH (Actions::iterator, i, mActions)
    {
        FOR_EACHP (ActionMap::iterator, it, (*i).second)
            actions.insert(it->second);
        delete (*i).second;
    }

    FOR_EACH (std::set<Action*>::const_iterator, i, actions)
        delete *i;

    mActions.clear();

    FOR_EACH (ImageSetIterator, i, mImageSets)
    {
        if (i->second)
        {
            i->second->decRef();
            i->second = nullptr;
        }
    }
    mImageSets.clear();
}

SpriteDirection SpriteDef::makeSpriteDirection(const std::string &direction)
{
    if (direction.empty() || direction == "default")
        return DIRECTION_DEFAULT;
    else if (direction == "up")
        return DIRECTION_UP;
    else if (direction == "left")
        return DIRECTION_LEFT;
    else if (direction == "right")
        return DIRECTION_RIGHT;
    else if (direction == "down")
        return DIRECTION_DOWN;
    else if (direction == "upleft")
        return DIRECTION_UPLEFT;
    else if (direction == "upright")
        return DIRECTION_UPRIGHT;
    else if (direction == "downleft")
        return DIRECTION_DOWNLEFT;
    else if (direction == "downright")
        return DIRECTION_DOWNRIGHT;
    else
        return DIRECTION_INVALID;
}

void SpriteDef::addAction(const unsigned hp, const std::string &name,
                          Action *const action)
{
    const Actions::const_iterator i = mActions.find(hp);
    if (i == mActions.end())
        mActions[hp] = new ActionMap();

    (*mActions[hp])[name] = action;
}

bool SpriteDef::addSequence(const int start, const int end, const int delay,
                            const int offsetX, const int offsetY,
                            const int variant_offset,
                            int repeat, const int rand,
                            const ImageSet *const imageSet,
                            Animation *const animation) const
{
    if (!imageSet || !animation)
        return true;

    if (start < 0 || end < 0)
    {
        logger->log1("No valid value for 'start' or 'end'");
        return true;
    }

    if (start <= end)
    {
        while (repeat > 0)
        {
            int pos = start;
            while (end >= pos)
            {
                Image *const img = imageSet->get(pos + variant_offset);

                if (!img)
                {
                    logger->log("No image at index %d",
                        pos + variant_offset);
                    pos ++;
                    continue;
                }

                animation->addFrame(img, delay,
                    offsetX, offsetY, rand);
                pos ++;
            }
            repeat --;
        }
    }
    else
    {
        while (repeat > 0)
        {
            int pos = start;
            while (end <= pos)
            {
                Image *const img = imageSet->get(pos + variant_offset);

                if (!img)
                {
                    logger->log("No image at index %d",
                        pos + variant_offset);
                    pos ++;
                    continue;
                }

                animation->addFrame(img, delay,
                    offsetX, offsetY, rand);
                pos --;
            }
            repeat --;
        }
    }
    return false;
}
