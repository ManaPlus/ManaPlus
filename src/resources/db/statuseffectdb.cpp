/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/db/statuseffectdb.h"

#include "configuration.h"
#include "soundmanager.h"
#include "statuseffect.h"

#include "const/resources/spriteaction.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "particle/particle.h"

#include "listeners/debugmessagelistener.h"

#include "resources/beingcommon.h"

#include "resources/sprite/animatedsprite.h"

#include "debug.h"

namespace
{
    typedef std::map<int, StatusEffect *> status_effect_map[2];
    bool mLoaded = false;
    status_effect_map statusEffects;
    std::map<int, int> blockEffectIndexMap;
}  // namespace

int StatusEffectDB::blockEffectIndexToEffectIndex(const int blockIndex)
{
    if (blockEffectIndexMap.find(blockIndex) == blockEffectIndexMap.end())
        return -1;
    return blockEffectIndexMap[blockIndex];
}

StatusEffect *StatusEffectDB::getStatusEffect(const int index,
                                              const Enable enabling)
{
    std::map<int, StatusEffect *> &effects
        = statusEffects[enabling == Enable_true];
    const std::map<int, StatusEffect *>::iterator it = effects.find(index);
    if (it != effects.end())
        return (*it).second;
    if (config.getBoolValue("unimplimentedLog"))
    {
        const std::string str = strprintf("Missing status effect: %d", index);
        logger->log(str);
        DebugMessageListener::distributeEvent(str);
    }
    return nullptr;
}

void StatusEffectDB::load()
{
    if (mLoaded)
        unload();

    loadXmlFile(paths.getStringValue("statusEffectsFile"));
    loadXmlFile(paths.getStringValue("statusEffectsPatchFile"));
    loadXmlDir("statusEffectsPatchDir", loadXmlFile);

    mLoaded = true;
}

void StatusEffectDB::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName, UseResman_true, SkipError_false);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "status-effects"))
    {
        logger->log("Error loading status effects file: " + fileName);
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string incName = XML::getProperty(node, "name", "");
            if (!incName.empty())
                loadXmlFile(incName);
            continue;
        }

        status_effect_map *the_map = nullptr;

        const int index = atoi(XML::getProperty(node, "id", "-1").c_str());

        if (xmlNameEqual(node, "status-effect"))
        {
            the_map = &statusEffects;
            const int block_index = atoi(XML::getProperty(
                node, "block-id", "-1").c_str());

            if (index >= 0 && block_index >= 0)
                blockEffectIndexMap[block_index] = index;
        }

        if (the_map)
        {
            StatusEffect *startEffect = (*the_map)[1][index];
            StatusEffect *endEffect = (*the_map)[0][index];
            const std::string name = XML::getProperty(node, "name", "");
            if (!startEffect)
                startEffect = new StatusEffect;
            if (!endEffect)
                endEffect = new StatusEffect;

            startEffect->mName = name;
            startEffect->mIsPoison = 
                (name == paths.getStringValue("poisonEffectName"));
            startEffect->mIsCart = 
                (name == paths.getStringValue("cartEffectName"));
            startEffect->mIsRiding = 
                (name == paths.getStringValue("ridingEffectName"));
            startEffect->mIsTrickDead = 
                (name == paths.getStringValue("trickDeadEffectName"));
            startEffect->mIsPostDelay = 
                (name == paths.getStringValue("postDelayName"));
            startEffect->mMessage = XML::getProperty(
                node, "start-message", "");
            startEffect->mSFXEffect = XML::getProperty(
                node, "start-audio", "");
            startEffect->mParticleEffect = XML::getProperty(
                node, "start-particle", "");

            startEffect->mIcon = XML::getProperty(node, "icon", "");
            startEffect->mAction = XML::getProperty(node, "action", "");
            startEffect->mPersistentParticleEffect = (XML::getProperty(
                node, "persistent-particle-effect", "no")) != "no";

            endEffect->mName = startEffect->mName;
            endEffect->mIsPoison = startEffect->mIsPoison;
            endEffect->mIsCart = startEffect->mIsCart;
            endEffect->mIsRiding = startEffect->mIsRiding;
            endEffect->mIsTrickDead = startEffect->mIsTrickDead;
            endEffect->mIsPostDelay = startEffect->mIsPostDelay;
            endEffect->mMessage = XML::getProperty(node, "end-message", "");
            endEffect->mSFXEffect = XML::getProperty(node, "end-audio", "");
            endEffect->mParticleEffect = XML::getProperty(
                node, "end-particle", "");

            (*the_map)[1][index] = startEffect;
            (*the_map)[0][index] = endEffect;
        }
    }
}

static void unloadMap(std::map<int, StatusEffect *> &map)
{
    for (std::map<int, StatusEffect *>::iterator it = map.begin();
         it != map.end(); ++it)
    {
        delete (*it).second;
    }

    map.clear();
}

void StatusEffectDB::unload()
{
    if (!mLoaded)
        return;

    unloadMap(statusEffects[0]);
    unloadMap(statusEffects[1]);

    mLoaded = false;
}
