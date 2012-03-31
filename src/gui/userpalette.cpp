/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "userpalette.h"

#include "configuration.h"
#include "client.h"
#include "logger.h"

#include "gui/gui.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <math.h>

#include "debug.h"

const std::string ColorTypeNames[] =
{
    "ColorBeing",
    "ColorFriend",
    "ColorDisregarded",
    "ColorIgnored",
    "ColorErased",
    "ColorPlayer",
    "ColorSelf",
    "ColorGM",
    "ColorNPC",
    "ColorMonster",
    "ColorMonsterHp",
    "ColorMonsterHp2",
    "ColorParty",
    "ColorGuild",
    "ColorParticle",
    "ColorPickupInfo",
    "ColorExpInfo",
    "ColorPlayerHp",
    "ColorPlayerHp2",
    "ColorHitPlayerMonster",
    "ColorHitMonsterPlayer",
    "ColorHitPlayerPlayer",
    "ColorHitCritical",
    "ColorHitLocalPlayerMonster",
    "ColorHitLocalPlayerCritical",
    "ColorHitLocalPlayerMiss",
    "ColorMiss",
    "ColorPortalHighlight",
    "ColorCollisionHighlight",
    "ColorCollisionAirHighlight",
    "ColorCollisionWaterHighlight",
    "ColorWalkableTileHighlight",
    "ColorAttackRange",
    "ColorAttackRangeBorder",
    "ColorMonsterAttackRange",
    "ColorHomePlace",
    "ColorHomePlaceBorder",
    "ColorRoadPoint"
};

std::string UserPalette::getConfigName(const std::string &typeName)
{
    std::string res = "Color" + typeName;

    size_t pos = 5;
    for (size_t i = 0; i < typeName.length(); i++)
    {
        if (i == 0 || typeName[i] == '_')
        {
            if (i > 0)
                i++;

            res[pos] = typeName[i];
        }
        else
        {
           res[pos] = static_cast<char>(tolower(typeName[i]));
        }
        pos ++;
    }
    res.erase(pos, res.length() - pos);

    return res;
}

UserPalette::UserPalette():
        Palette(USER_COLOR_LAST)
{
    mColors[BEING] = ColorElem();
    mColors[PC] = ColorElem();
    mColors[SELF] = ColorElem();
    mColors[GM] = ColorElem();
    mColors[NPC] = ColorElem();
    mColors[MONSTER] = ColorElem();

    addColor(BEING, 0xffffff, STATIC, _("Being"));
    addColor(FRIEND, 0xb0ffb0, STATIC, _("Friend Names"));
    addColor(DISREGARDED, 0xa00000, STATIC, _("Disregarded Names"));
    addColor(IGNORED, 0xff0000, STATIC, _("Ignored Names"));
    addColor(ERASED, 0xff0000, STATIC, _("Erased Names"));
    addColor(PC, 0xffffff, STATIC, _("Other Players' Names"));
    addColor(SELF, 0xff8040, STATIC, _("Own Name"));
    addColor(GM, 0x00ff00, STATIC, _("GM Names"));
    addColor(NPC, 0xc8c8ff, STATIC, _("NPCs"));
    addColor(MONSTER, 0xff4040, STATIC, _("Monsters"));
    addColor(MONSTER_HP, 0x00ff00, STATIC, _("Monster HP bar"), 50);
    addColor(MONSTER_HP2, 0xff0000, STATIC,
        _("Monster HP bar (second color)"), 50);
    addColor(PARTY, 0xff00d8, STATIC, _("Party Members"));
    addColor(GUILD, 0xff00d8, STATIC, _("Guild Members"));
    addColor(PARTICLE, 0xffffff, STATIC, _("Particle Effects"));
    addColor(PICKUP_INFO, 0x28dc28, STATIC, _("Pickup Notification"));
    addColor(EXP_INFO, 0xffff00, STATIC, _("Exp Notification"));
    addColor(PLAYER_HP, 0x00ff00, STATIC, _("Player HP bar"), 50);
    addColor(PLAYER_HP2, 0xff0000, STATIC,
        _("Player HP bar (second color)"), 50);
    addColor(HIT_PLAYER_MONSTER, 0x0064ff, STATIC, _("Player Hits Monster"));
    addColor(HIT_MONSTER_PLAYER, 0xff3232, STATIC, _("Monster Hits Player"));
    addColor(HIT_PLAYER_PLAYER, 0xff5050, STATIC,
             _("Other Player Hits Local Player"));
    addColor(HIT_CRITICAL, 0xff0000, RAINBOW, _("Critical Hit"));
    addColor(HIT_LOCAL_PLAYER_MONSTER, 0x00ff00, STATIC,
             _("Local Player Hits Monster"));
    addColor(HIT_LOCAL_PLAYER_CRITICAL, 0xff0000, RAINBOW,
             _("Local Player Critical Hit"));
    addColor(HIT_LOCAL_PLAYER_MISS, 0x00ffa6, STATIC,
             _("Local Player Miss"));
    addColor(MISS, 0xffff00, STATIC, _("Misses"));
    addColor(PORTAL_HIGHLIGHT, 0xC80000, STATIC, _("Portal Highlight"));
    addColor(COLLISION_HIGHLIGHT, 0x0000C8, STATIC,
             _("Default collision Highlight"), 64);
    addColor(AIR_COLLISION_HIGHLIGHT, 0xe0e0ff, STATIC,
             _("Air collision Highlight"), 64);
    addColor(WATER_COLLISION_HIGHLIGHT, 0x2050e0, STATIC,
             _("Water collision Highlight"), 64);
    addColor(WALKABLE_HIGHLIGHT, 0x00D000, STATIC,
             _("Walkable Highlight"), 255);
    addColor(ATTACK_RANGE, 0xffffff, STATIC,
             _("Local Player Attack Range"), 5);
    addColor(ATTACK_RANGE_BORDER, 0x0, STATIC,
             _("Local Player Attack Range Border"), 76);
    addColor(MONSTER_ATTACK_RANGE, 0xff0000, STATIC,
             _("Monster Attack Range"), 20);
    addColor(HOME_PLACE, 0xffffff, STATIC,
             _("Home Place"), 20);
    addColor(HOME_PLACE_BORDER, 0xffff00, STATIC,
             _("Home Place Border"), 200);
    addColor(ROAD_POINT, 0x000000, STATIC,
             _("Road Point"), 100);
    commit(true);
}

UserPalette::~UserPalette()
{
    for (Colors::const_iterator col = mColors.begin(),
         colEnd = mColors.end(); col != colEnd; ++col)
    {
        const std::string &configName = ColorTypeNames[col->type];
        config.setValue(configName + "Gradient",
            static_cast<int>(col->committedGrad));
        config.setValue(configName + "Delay", col->delay);

        if (col->grad == STATIC || col->grad == PULSE)
        {
            char buffer[20];
            sprintf(buffer, "0x%06x", col->getRGB());
            config.setValue(configName, std::string(buffer));
        }
    }
}

void UserPalette::setColor(int type, int r, int g, int b)
{
    gcn::Color &color = mColors[type].color;
    color.r = r;
    color.g = g;
    color.b = b;
}

void UserPalette::setGradient(int type, GradientType grad)
{
    ColorElem *elem = &mColors[type];
    if (!elem)
        return;

    if (elem->grad != STATIC && grad == STATIC)
    {
        for (size_t i = 0; i < mGradVector.size(); i++)
        {
            if (mGradVector[i] == elem)
            {
                mGradVector.erase(mGradVector.begin() + i);
                break;
            }
        }
    }
    else if (elem->grad == STATIC && grad != STATIC)
    {
        mGradVector.push_back(elem);
    }

    if (elem->grad != grad)
        elem->grad = grad;
}

std::string UserPalette::getElementAt(int i)
{
    if (i < 0 || i >= getNumberOfElements())
        return "";

    return mColors[i].text;
}

void UserPalette::commit(bool commitNonStatic)
{
    for (Colors::iterator i = mColors.begin(), iEnd = mColors.end();
         i != iEnd; ++i)
    {
        i->committedGrad = i->grad;
        i->committedDelay = i->delay;
        if (commitNonStatic || i->grad == STATIC)
            i->committedColor = i->color;
        else if (i->grad == PULSE)
            i->committedColor = i->testColor;
    }
}

void UserPalette::rollback()
{
    for (Colors::iterator i = mColors.begin(), iEnd = mColors.end();
         i != iEnd; ++i)
    {
        if (i->grad != i->committedGrad)
            setGradient(i->type, i->committedGrad);

        const gcn::Color &committedColor = i->committedColor;
        setGradientDelay(i->type, i->committedDelay);
        setColor(i->type, committedColor.r,
                 committedColor.g, committedColor.b);

        if (i->grad == PULSE)
        {
            gcn::Color &testColor = i->testColor;
            testColor.r = committedColor.r;
            testColor.g = committedColor.g;
            testColor.b = committedColor.b;
        }
    }
}

int UserPalette::getColorTypeAt(int i)
{
    if (i < 0 || i >= getNumberOfElements())
        return BEING;

    return mColors[i].type;
}

void UserPalette::addColor(unsigned type, unsigned rgb,
                           Palette::GradientType grad, const std::string &text,
                           int delay)
{
    const unsigned maxType = sizeof(ColorTypeNames)
        / sizeof(ColorTypeNames[0]);

    if (type >= maxType)
        return;

    const std::string &configName = ColorTypeNames[type];
    char buffer[20];
    sprintf(buffer, "0x%06x", rgb);

    const std::string rgbString = config.getValue(configName,
                                                  std::string(buffer));
    unsigned int rgbValue = 0;
    if (rgbString.length() == 8 && rgbString[0] == '0' && rgbString[1] == 'x')
        rgbValue = atox(rgbString);
    else
        rgbValue = atoi(rgbString.c_str());
    gcn::Color trueCol = rgbValue;
    grad = static_cast<GradientType>(config.getValue(configName + "Gradient",
                                     static_cast<int>(grad)));
    delay = config.getValueInt(configName + "Delay", delay);
    mColors[type].set(type, trueCol, grad, delay);
    mColors[type].text = text;

    if (grad != STATIC)
        mGradVector.push_back(&mColors[type]);
}
