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

#include "resources/sprite/spritedef.h"

#include "configuration.h"
#include "settings.h"

#include "const/resources/spriteaction.h"

#include "const/resources/map/map.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"

#include "resources/action.h"
#include "resources/imageset.h"

#include "resources/animation/animation.h"

#include "resources/dye/dye.h"

#include "resources/loaders/imagesetloader.h"
#include "resources/loaders/xmlloader.h"

#include "resources/sprite/spritereference.h"

#include "debug.h"

SpriteReference *SpriteReference::Empty = nullptr;

const Action *SpriteDef::getAction(const std::string &action,
                                   const unsigned num) const
{
    Actions::const_iterator i = mActions.find(num);
    if (i == mActions.end() && num != 100)
        i = mActions.find(100);

    if (i == mActions.end() || ((*i).second == nullptr))
        return nullptr;

    const ActionMap *const actMap = (*i).second;
    if (actMap == nullptr)
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
    BLOCK_START("SpriteDef::load")
    const size_t pos = animationFile.find('|');
    std::string palettes;
    if (pos != std::string::npos)
        palettes = animationFile.substr(pos + 1);

    XML::Document *const doc = Loader::getXml(animationFile.substr(0, pos),
        UseVirtFs_true,
        SkipError_false);
    if (doc == nullptr)
        return nullptr;
    XmlNodePtrConst rootNode = doc->rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "sprite"))
    {
        reportAlways("Error, failed to parse sprite %s",
            animationFile.c_str());
        const std::string errorFile = pathJoin(paths.getStringValue("sprites"),
            paths.getStringValue("spriteErrorFile"));
        BLOCK_END("SpriteDef::load")
        doc->decRef();
        if (animationFile != errorFile)
            return load(errorFile, 0, prot);
        else
            return nullptr;
    }

    SpriteDef *const def = new SpriteDef;
    def->mSource = animationFile;
    def->mProcessedFiles.insert(animationFile);
    def->loadSprite(rootNode, variant, palettes);
    def->substituteActions();
    if (settings.fixDeadAnimation)
        def->fixDeadAction();
    if (prot)
    {
        def->incRef();
        def->mProtected = true;
    }
    doc->decRef();
    BLOCK_END("SpriteDef::load")
    return def;
}

void SpriteDef::fixDeadAction()
{
    FOR_EACH (ActionsIter, it, mActions)
    {
        ActionMap *const d = (*it).second;
        if (d == nullptr)
            continue;
        const ActionMap::iterator i = d->find(SpriteAction::DEAD);
        const ActionMap::iterator i2 = d->find(SpriteAction::STAND);
        // search dead action and check what it not same with stand action
        if (i != d->end() && (i->second != nullptr) && i->second != i2->second)
            (i->second)->setLastFrameDelay(0);
    }
}

void SpriteDef::substituteAction(const std::string &restrict complete,
                                 const std::string &restrict with)
{
    FOR_EACH (ActionsConstIter, it, mActions)
    {
        ActionMap *const d = (*it).second;
        if (reportTrue(d == nullptr))
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
    substituteAction(SpriteAction::CAST, SpriteAction::ATTACK);
    substituteAction(SpriteAction::SIT, SpriteAction::STAND);
    substituteAction(SpriteAction::SITTOP, SpriteAction::SIT);
    substituteAction(SpriteAction::DEAD, SpriteAction::STAND);
    substituteAction(SpriteAction::SPAWN, SpriteAction::STAND);
    substituteAction(SpriteAction::FLY, SpriteAction::MOVE);
    substituteAction(SpriteAction::SWIM, SpriteAction::MOVE);
    substituteAction(SpriteAction::RIDE, SpriteAction::MOVE);
    substituteAction(SpriteAction::STANDSKY, SpriteAction::STAND);
    substituteAction(SpriteAction::STANDWATER, SpriteAction::STAND);
    substituteAction(SpriteAction::STANDRIDE, SpriteAction::STAND);
    substituteAction(SpriteAction::SITSKY, SpriteAction::SIT);
    substituteAction(SpriteAction::SITWATER, SpriteAction::SIT);
    substituteAction(SpriteAction::SITRIDE, SpriteAction::SIT);
    substituteAction(SpriteAction::ATTACKSKY, SpriteAction::ATTACK);
    substituteAction(SpriteAction::ATTACKWATER, SpriteAction::ATTACK);
    substituteAction(SpriteAction::ATTACKRIDE, SpriteAction::ATTACK);
    substituteAction(SpriteAction::CASTSKY, SpriteAction::CAST);
    substituteAction(SpriteAction::CASTWATER, SpriteAction::CAST);
    substituteAction(SpriteAction::CASTRIDE, SpriteAction::CAST);
    substituteAction(SpriteAction::SPAWNSKY, SpriteAction::SPAWN);
    substituteAction(SpriteAction::SPAWNWATER, SpriteAction::SPAWN);
    substituteAction(SpriteAction::SPAWNRIDE, SpriteAction::SPAWN);
    substituteAction(SpriteAction::DEADSKY, SpriteAction::DEAD);
    substituteAction(SpriteAction::DEADWATER, SpriteAction::DEAD);
    substituteAction(SpriteAction::DEADRIDE, SpriteAction::DEAD);
}

void SpriteDef::loadSprite(XmlNodeConstPtr spriteNode,
                           const int variant,
                           const std::string &palettes)
{
    BLOCK_START("SpriteDef::loadSprite")
    if (spriteNode == nullptr)
    {
        BLOCK_END("SpriteDef::loadSprite")
        return;
    }
    // Get the variant
    const int variantCount = XML::getProperty(spriteNode, "variants", 0);
    int variant_offset = 0;

    if (variantCount > 0 && variant < variantCount)
    {
        variant_offset = variant * XML::getProperty(spriteNode,
            "variant_offset",
            0);
    }

    for_each_xml_child_node(node, spriteNode)
    {
        if (xmlNameEqual(node, "imageset"))
            loadImageSet(node, palettes);
        else if (xmlNameEqual(node, "action"))
            loadAction(node, variant_offset);
        else if (xmlNameEqual(node, "include"))
            includeSprite(node, variant);
    }
    BLOCK_END("SpriteDef::loadSprite")
}

void SpriteDef::loadImageSet(XmlNodeConstPtr node,
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

    ImageSet *const imageSet = Loader::getImageSet(imageSrc,
        width, height);

    if (imageSet == nullptr)
    {
        reportAlways("%s: Couldn't load imageset: %s",
            mSource.c_str(),
            imageSrc.c_str());
        return;
    }

    imageSet->setOffsetX(XML::getProperty(node, "offsetX", 0));
    imageSet->setOffsetY(XML::getProperty(node, "offsetY", 0));
    mImageSets[name] = imageSet;
}

const ImageSet *SpriteDef::getImageSet(const std::string &imageSetName) const
{
    const ImageSetCIterator si = mImageSets.find(imageSetName);
    if (si == mImageSets.end())
    {
        reportAlways("%s: Imageset \"%s\" not defined in %s",
            mSource.c_str(),
            imageSetName.c_str(),
            mIdPath.c_str());
        return nullptr;
    }
    return si->second;
}

void SpriteDef::loadAction(XmlNodeConstPtr node,
                           const int variant_offset)
{
    if (node == nullptr)
        return;

    const std::string actionName = XML::getProperty(node, "name", "");
    const std::string imageSetName = XML::getProperty(node, "imageset", "");
    const unsigned hp = XML::getProperty(node, "hp", 100);
    const ImageSet *const imageSet = getImageSet(imageSetName);

    if (actionName == SpriteAction::INVALID)
    {
        reportAlways("%s: Unknown action \"%s\" defined in %s",
            mSource.c_str(),
            actionName.c_str(),
            mIdPath.c_str());
        return;
    }
    Action *const action = new Action(actionName);
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

void SpriteDef::loadAnimation(XmlNodeConstPtr animationNode,
                              Action *const action,
                              const ImageSet *const imageSet0,
                              const int variant_offset) const
{
    if (action == nullptr ||
        imageSet0 == nullptr ||
        animationNode == nullptr)
    {
        return;
    }

    const std::string directionName =
        XML::getProperty(animationNode, "direction", "");
    const SpriteDirection::Type directionType
        = makeSpriteDirection(directionName);

    if (directionType == SpriteDirection::INVALID)
    {
        reportAlways("%s: Unknown direction \"%s\" used in %s",
            mSource.c_str(),
            directionName.c_str(),
            mIdPath.c_str());
        return;
    }

    Animation *const animation = new Animation(directionName);
    action->setAnimation(directionType, animation);

    // Get animation frames
    for_each_xml_child_node(frameNode, animationNode)
    {
        const int delay = XML::getIntProperty(
            frameNode, "delay", 0, 0, 100000);
        const std::string imageSetName = XML::getProperty(frameNode,
            "imageset",
            "");
        const ImageSet *imageSet = imageSet0;
        if (!imageSetName.empty())
        {
            imageSet = getImageSet(imageSetName);
            if (imageSet == nullptr)
                imageSet = imageSet0;
        }
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
                reportAlways(
                    "%s: No valid value for 'index' at direction '%s'",
                    mSource.c_str(),
                    directionName.c_str());
                continue;
            }

            Image *const img = imageSet->get(index + variant_offset);
            if (img == nullptr)
            {
                reportAlways("%s: No image at index %d at direction '%s'",
                    mSource.c_str(),
                    index + variant_offset,
                    directionName.c_str());
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
                reportAlways("%s: No valid value for 'repeat' at direction %s",
                    mSource.c_str(),
                    directionName.c_str());
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
                    const size_t idx = str.find('-');
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
                        if (img != nullptr)
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

void SpriteDef::includeSprite(XmlNodeConstPtr includeNode, const int variant)
{
    std::string filename = XML::getProperty(includeNode, "file", "");

    if (filename.empty())
        return;
    filename = pathJoin(paths.getStringValue("sprites"), filename);

    if (mProcessedFiles.find(filename) != mProcessedFiles.end())
    {
        reportAlways("%s: Tried to include %s which already is included.",
            mSource.c_str(),
            filename.c_str());
        return;
    }
    mProcessedFiles.insert(filename);

    XML::Document *const doc = Loader::getXml(filename,
        UseVirtFs_true,
        SkipError_false);
    if (doc == nullptr)
        return;
    XmlNodeConstPtr rootNode = doc->rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "sprite"))
    {
        reportAlways("%s: No sprite root node in %s",
            mSource.c_str(),
            filename.c_str());
        doc->decRef();
        return;
    }

    loadSprite(rootNode, variant);
    doc->decRef();
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
        if (i->second != nullptr)
        {
            i->second->decRef();
            i->second = nullptr;
        }
    }
    mImageSets.clear();
}

SpriteDirection::Type SpriteDef::makeSpriteDirection(const std::string
                                                     &direction)
{
    if (direction.empty() || direction == "default")
        return SpriteDirection::DEFAULT;
    else if (direction == "up")
        return SpriteDirection::UP;
    else if (direction == "left")
        return SpriteDirection::LEFT;
    else if (direction == "right")
        return SpriteDirection::RIGHT;
    else if (direction == "down")
        return SpriteDirection::DOWN;
    else if (direction == "upleft")
        return SpriteDirection::UPLEFT;
    else if (direction == "upright")
        return SpriteDirection::UPRIGHT;
    else if (direction == "downleft")
        return SpriteDirection::DOWNLEFT;
    else if (direction == "downright")
        return SpriteDirection::DOWNRIGHT;
    else
        return SpriteDirection::INVALID;
}

void SpriteDef::addAction(const unsigned hp, const std::string &name,
                          Action *const action)
{
    const Actions::const_iterator i = mActions.find(hp);
    if (i == mActions.end())
        mActions[hp] = new ActionMap;

    (*mActions[hp])[name] = action;
}

bool SpriteDef::addSequence(const int start,
                            const int end,
                            const int delay,
                            const int offsetX,
                            const int offsetY,
                            const int variant_offset,
                            int repeat,
                            const int rand,
                            const ImageSet *const imageSet,
                            Animation *const animation) const
{
    if ((imageSet == nullptr) || (animation == nullptr))
        return true;

    if (start < 0 || end < 0)
    {
        reportAlways("%s: No valid value for 'start' or 'end'",
            mSource.c_str());
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

                if (img == nullptr)
                {
                    reportAlways("%s: No image at index %d",
                        mSource.c_str(),
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

                if (img == nullptr)
                {
                    reportAlways("%s: No image at index %d",
                        mSource.c_str(),
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

int SpriteDef::calcMemoryLocal() const
{
    int sz = static_cast<int>(sizeof(SpriteDef) +
        sizeof(ImageSets) +
        sizeof(Actions) +
        sizeof(std::set<std::string>)) +
        Resource::calcMemoryLocal();
    FOR_EACH (std::set<std::string>::const_iterator, it, mProcessedFiles)
    {
        sz += static_cast<int>((*it).capacity());
    }
    return sz;
}

int SpriteDef::calcMemoryChilds(const int level) const
{
    int sz = 0;
    FOR_EACH (ImageSets::const_iterator, it, mImageSets)
    {
        sz += static_cast<int>((*it).first.capacity());
        const ImageSet *const imageSet = (*it).second;
        sz += imageSet->calcMemory(level + 1);
    }
    FOR_EACH (ActionsCIter, it, mActions)
    {
        sz += sizeof(unsigned);
        const ActionMap *const actionMap = (*it).second;
        FOR_EACHP (ActionMap::const_iterator, it2, actionMap)
        {
            sz += static_cast<int>((*it2).first.capacity());
            const Action *const action = (*it2).second;
            sz += action->calcMemory(level + 1);
        }
    }
    return sz;
}
