/*
 *  An effects manager
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
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

#include "being.h"
#include "effectmanager.h"
#include "logger.h"
#include "particle.h"
#include "sound.h"

#include "utils/xml.h"

#include "debug.h"

EffectManager::EffectManager()
{
    XML::Document doc("effects.xml");
    XmlNodePtr root = doc.rootNode();

    if (!root || !xmlStrEqual(root->name, BAD_CAST "being-effects"))
    {
        logger->log1("Error loading being effects file: effects.xml");
        return;
    }
    else
    {
        logger->log1("Effects are now loading");
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlStrEqual(node->name, BAD_CAST "effect"))
        {
            EffectDescription ed;
            ed.id = XML::getProperty(node, "id", -1);
            ed.GFX = XML::getProperty(node, "particle", "");
            ed.SFX = XML::getProperty(node, "audio", "");
            mEffects.push_back(ed);
        }
    }
}

EffectManager::~EffectManager()
{
}

bool EffectManager::trigger(int id, Being* being)
{
    if (!being || !particleEngine)
        return false;

    bool rValue = false;
    for (std::vector<EffectDescription>::const_iterator i = mEffects.begin();
         i != mEffects.end(); ++i)
    {
        if ((*i).id == id)
        {
            rValue = true;
            if (!(*i).GFX.empty())
            {
                Particle *selfFX = particleEngine->addEffect((*i).GFX, 0, 0);
                being->controlParticle(selfFX);
            }
            if (!(*i).SFX.empty())
                sound.playSfx((*i).SFX);
            break;
        }
    }
    return rValue;
}

bool EffectManager::trigger(int id, int x, int y)
{
    if (!particleEngine)
        return false;

    bool rValue = false;
    for (std::vector<EffectDescription>::const_iterator i = mEffects.begin();
         i != mEffects.end(); ++i)
    {
        if ((*i).id == id)
        {
            rValue = true;
            if (!(*i).GFX.empty() && particleEngine)
                particleEngine->addEffect((*i).GFX, x, y);
            if (!(*i).SFX.empty())
                sound.playSfx((*i).SFX);
            break;
        }
    }
    return rValue;
}
