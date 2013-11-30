/*
 *  Configurable text colors
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/userpalette.h"

#include "configuration.h"

#include "utils/gettext.h"

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
    "ColorCollisionGroundtopHighlight",
    "ColorWalkableTileHighlight",
    "ColorAttackRange",
    "ColorAttackRangeBorder",
    "ColorMonsterAttackRange",
    "ColorFloorItemText",
    "ColorHomePlace",
    "ColorHomePlaceBorder",
    "ColorRoadPoint",
    "ColorNet"
};

std::string UserPalette::getConfigName(const std::string &typeName)
{
    std::string res("Color" + typeName);
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
           res[pos] = static_cast<signed char>(tolower(typeName[i]));
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

    // TRANSLATORS: palette color
    addColor(BEING, 0xffffff, STATIC, _("Being"));
    // TRANSLATORS: palette color
    addColor(FRIEND, 0xb0ffb0, STATIC, _("Friend names"));
    // TRANSLATORS: palette color
    addColor(DISREGARDED, 0xa00000, STATIC, _("Disregarded names"));
    // TRANSLATORS: palette color
    addColor(IGNORED, 0xff0000, STATIC, _("Ignored names"));
    // TRANSLATORS: palette color
    addColor(ERASED, 0xff0000, STATIC, _("Erased names"));
    // TRANSLATORS: palette color
    addColor(PC, 0xffffff, STATIC, _("Other players names"));
    // TRANSLATORS: palette color
    addColor(SELF, 0xff8040, STATIC, _("Own name"));
    // TRANSLATORS: palette color
    addColor(GM, 0x00ff00, STATIC, _("GM names"));
    // TRANSLATORS: palette color
    addColor(NPC, 0xc8c8ff, STATIC, _("NPCs"));
    // TRANSLATORS: palette color
    addColor(MONSTER, 0xff4040, STATIC, _("Monsters"));
    // TRANSLATORS: palette color
    addColor(MONSTER_HP, 0x00ff00, STATIC, _("Monster HP bar"), 50);
    addColor(MONSTER_HP2, 0xff0000, STATIC,
        // TRANSLATORS: palette color
        _("Monster HP bar (second color)"), 50);
    // TRANSLATORS: palette color
    addColor(PARTY, 0xff00d8, STATIC, _("Party members"));
    // TRANSLATORS: palette color
    addColor(GUILD, 0xff00d8, STATIC, _("Guild members"));
    // TRANSLATORS: palette color
    addColor(PARTICLE, 0xffffff, STATIC, _("Particle effects"));
    // TRANSLATORS: palette color
    addColor(PICKUP_INFO, 0x28dc28, STATIC, _("Pickup notification"));
    // TRANSLATORS: palette color
    addColor(EXP_INFO, 0xffff00, STATIC, _("Exp notification"));
    // TRANSLATORS: palette color
    addColor(PLAYER_HP, 0x00ff00, STATIC, _("Player HP bar"), 50);
    // TRANSLATORS: palette color
    addColor(PLAYER_HP2, 0xff0000, STATIC,
        // TRANSLATORS: palette color
        _("Player HP bar (second color)"), 50);
    // TRANSLATORS: palette color
    addColor(HIT_PLAYER_MONSTER, 0x0064ff, STATIC, _("Player hits monster"));
    // TRANSLATORS: palette color
    addColor(HIT_MONSTER_PLAYER, 0xff3232, STATIC, _("Monster hits player"));
    // TRANSLATORS: palette color
    addColor(HIT_PLAYER_PLAYER, 0xff5050, STATIC,
       // TRANSLATORS: palette color
       _("Other player hits local player"));
    // TRANSLATORS: palette color
    addColor(HIT_CRITICAL, 0xff0000, RAINBOW, _("Critical Hit"));
    // TRANSLATORS: palette color
    addColor(HIT_LOCAL_PLAYER_MONSTER, 0x00ff00, STATIC,
        // TRANSLATORS: palette color
        _("Local player hits monster"));
    addColor(HIT_LOCAL_PLAYER_CRITICAL, 0xff0000, RAINBOW,
        // TRANSLATORS: palette color
        _("Local player critical hit"));
    addColor(HIT_LOCAL_PLAYER_MISS, 0x00ffa6, STATIC,
    // TRANSLATORS: palette color
        _("Local player miss"));
    // TRANSLATORS: palette color
    addColor(MISS, 0xffff00, STATIC, _("Misses"));
    // TRANSLATORS: palette color
    addColor(PORTAL_HIGHLIGHT, 0xC80000, STATIC, _("Portal highlight"));
    addColor(COLLISION_HIGHLIGHT, 0x0000C8, STATIC,
        // TRANSLATORS: palette color
        _("Default collision highlight"), 64);
    addColor(AIR_COLLISION_HIGHLIGHT, 0xe0e0ff, STATIC,
        // TRANSLATORS: palette color
        _("Air collision highlight"), 64);
    addColor(WATER_COLLISION_HIGHLIGHT, 0x2050e0, STATIC,
        // TRANSLATORS: palette color
        _("Water collision highlight"), 64);
    addColor(GROUNDTOP_COLLISION_HIGHLIGHT, 0xffff00, STATIC,
        // TRANSLATORS: palette color
        _("Special ground collision highlight"), 20);
    addColor(WALKABLE_HIGHLIGHT, 0x00D000, STATIC,
        // TRANSLATORS: palette color
        _("Walkable highlight"), 255);
    addColor(ATTACK_RANGE, 0xffffff, STATIC,
        // TRANSLATORS: palette color
        _("Local player attack range"), 5);
    addColor(ATTACK_RANGE_BORDER, 0x0, STATIC,
        // TRANSLATORS: palette color
        _("Local player attack range border"), 76);
    addColor(MONSTER_ATTACK_RANGE, 0xff0000, STATIC,
        // TRANSLATORS: palette color
        _("Monster attack range"), 20);

    addColor(FLOOR_ITEM_TEXT, 0xffffff, STATIC,
        // TRANSLATORS: palette color
        _("Floor item amount color"), 100);
    addColor(HOME_PLACE, 0xffffff, STATIC,
        // TRANSLATORS: palette color
        _("Home place"), 20);
    addColor(HOME_PLACE_BORDER, 0xffff00, STATIC,
        // TRANSLATORS: palette color
        _("Home place border"), 200);
    addColor(ROAD_POINT, 0x000000, STATIC,
        // TRANSLATORS: palette color
        _("Road point"), 100);
    addColor(NET, 0x000000, STATIC,
        // TRANSLATORS: palette color
        _("Tiles border"), 64);
    commit(true);
}

UserPalette::~UserPalette()
{
    FOR_EACH (Colors::const_iterator, col, mColors)
    {
        const std::string &configName = ColorTypeNames[col->type];
        config.setValue(configName + "Gradient",
            static_cast<int>(col->committedGrad));
        config.setValue(configName + "Delay", col->delay);

        if (col->grad == STATIC || col->grad == PULSE)
        {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "0x%06x", col->getRGB());
            buffer[19] = 0;
            config.setValue(configName, std::string(buffer));
        }
    }
}

void UserPalette::setColor(const int type, const int r,
                           const int g, const int b)
{
    gcn::Color &color = mColors[type].color;
    color.r = r;
    color.g = g;
    color.b = b;
}

void UserPalette::setGradient(const int type, const GradientType grad)
{
    ColorElem *const elem = &mColors[type];

    if (elem->grad != STATIC && grad == STATIC)
    {
        const size_t sz = mGradVector.size();
        for (size_t i = 0; i < sz; i++)
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

void UserPalette::commit(const bool commitNonStatic)
{
    FOR_EACH (Colors::iterator, i, mColors)
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
    FOR_EACH (Colors::iterator, i, mColors)
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

void UserPalette::addColor(const unsigned type, const unsigned rgb,
                           Palette::GradientType grad,
                           const std::string &text,
                           int delay)
{
    const unsigned maxType = sizeof(ColorTypeNames)
        / sizeof(ColorTypeNames[0]);

    if (type >= maxType)
        return;

    const std::string &configName = ColorTypeNames[type];
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "0x%06x", rgb);
    buffer[19] = 0;

    const std::string rgbString = config.getValue(
        configName, std::string(buffer));
    unsigned int rgbValue = 0;
    if (rgbString.length() == 8 && rgbString[0] == '0' && rgbString[1] == 'x')
        rgbValue = atox(rgbString);
    else
        rgbValue = atoi(rgbString.c_str());
    const gcn::Color &trueCol = gcn::Color(rgbValue);
    grad = static_cast<GradientType>(config.getValue(configName + "Gradient",
                                     static_cast<int>(grad)));
    delay = config.getValueInt(configName + "Delay", delay);
    mColors[type].set(type, trueCol, grad, delay);
    mColors[type].text = text;

    if (grad != STATIC)
        mGradVector.push_back(&mColors[type]);
}
