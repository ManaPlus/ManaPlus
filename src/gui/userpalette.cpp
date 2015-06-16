/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

UserPalette *userPalette = nullptr;

const std::string ColorTypeNames[] =
{
    "",
    "ColorBeing",
    "ColorFriend",
    "ColorDisregarded",
    "ColorIgnored",
    "ColorErased",
    "ColorEnemy",
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
    "ColorNet",
    "ColorPet",
    "ColorMercenary",
    "ColorHomunculus"
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

UserPalette::UserPalette() :
    Palette(static_cast<int>(UserColorId::USER_COLOR_LAST))
{
    mColors[static_cast<size_t>(UserColorId::BEING)] = ColorElem();
    mColors[static_cast<size_t>(UserColorId::PC)] = ColorElem();
    mColors[static_cast<size_t>(UserColorId::SELF)] = ColorElem();
    mColors[static_cast<size_t>(UserColorId::GM)] = ColorElem();
    mColors[static_cast<size_t>(UserColorId::NPC)] = ColorElem();
    mColors[static_cast<size_t>(UserColorId::MONSTER)] = ColorElem();

    addLabel(UserColorId::LABEL_BEING,
        // TRANSLATORS: palette label
        _("Being colors"));
    addColor(UserColorId::BEING,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Being"));
    addColor(UserColorId::FRIEND,
        0xb0ffb0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Friend names"));
    addColor(UserColorId::DISREGARDED,
        0xa00000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Disregarded names"));
    addColor(UserColorId::IGNORED,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Ignored names"));
    addColor(UserColorId::ERASED,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Erased names"));
    addColor(UserColorId::PC,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Other players names"));
    addColor(UserColorId::SELF,
        0xff8040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Own name"));
    addColor(UserColorId::GM,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("GM names"));
    addColor(UserColorId::NPC,
        0xc8c8ff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("NPCs"));
    addColor(UserColorId::MONSTER,
        0xff4040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monsters"));
    addColor(UserColorId::MONSTER_HP,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monster HP bar"),
        50);
    addColor(UserColorId::MONSTER_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monster HP bar (second color)"),
        50);
    addColor(UserColorId::PARTY,
        0xff00d8,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Party members"));
    addColor(UserColorId::GUILD,
        0xff00d8,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Guild members"));
    addColor(UserColorId::ENEMY,
        0xff4040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Enemy"));
    addColor(UserColorId::PARTICLE,
        0xffffff,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Particle effects"));
    addColor(UserColorId::PICKUP_INFO,
        0x28dc28,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Pickup notification"));
    addColor(UserColorId::EXP_INFO,
        0xffff00,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Exp notification"));
    addColor(UserColorId::PLAYER_HP,
        0x00ff00,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Player HP bar"), 50);
    addColor(UserColorId::PLAYER_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Player HP bar (second color)"),
        50);
    addColor(UserColorId::HIT_PLAYER_MONSTER,
        0x0064ff,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Player hits monster"));
    addColor(UserColorId::HIT_MONSTER_PLAYER,
        0xff3232,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Monster hits player"));
    addColor(UserColorId::HIT_PLAYER_PLAYER,
        0xff5050,
        GradientType::STATIC,
       // TRANSLATORS: palette color
       _("Other player hits local player"));
    addColor(UserColorId::HIT_CRITICAL,
        0xff0000,
        // TRANSLATORS: palette color
        GradientType::RAINBOW,
        _("Critical Hit"));
    addColor(UserColorId::HIT_LOCAL_PLAYER_MONSTER,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player hits monster"));
    addColor(UserColorId::HIT_LOCAL_PLAYER_CRITICAL,
        0xff0000,
        GradientType::RAINBOW,
        // TRANSLATORS: palette color
        _("Local player critical hit"));
    addColor(UserColorId::HIT_LOCAL_PLAYER_MISS,
        0x00ffa6,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player miss"));
    addColor(UserColorId::MISS, 0xffff00,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Misses"));
    addColor(UserColorId::PORTAL_HIGHLIGHT,
        0xC80000,
        // TRANSLATORS: palette color
        GradientType::STATIC,
        _("Portal highlight"));
    addColor(UserColorId::COLLISION_HIGHLIGHT,
        0x0000C8,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Default collision highlight"),
        64);
    addColor(UserColorId::AIR_COLLISION_HIGHLIGHT,
        0xe0e0ff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Air collision highlight"),
        64);
    addColor(UserColorId::WATER_COLLISION_HIGHLIGHT,
        0x2050e0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Water collision highlight"),
        64);
    addColor(UserColorId::GROUNDTOP_COLLISION_HIGHLIGHT,
        0xffff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Special ground collision highlight"),
        20);
    addColor(UserColorId::WALKABLE_HIGHLIGHT,
        0x00D000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Walkable highlight"),
        255);
    addColor(UserColorId::ATTACK_RANGE,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player attack range"),
        5);
    addColor(UserColorId::ATTACK_RANGE_BORDER,
        0x0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player attack range border"),
        76);
    addColor(UserColorId::MONSTER_ATTACK_RANGE,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monster attack range"),
        20);
    addColor(UserColorId::FLOOR_ITEM_TEXT,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Floor item amount color"),
        100);
    addColor(UserColorId::HOME_PLACE,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Home place"),
        20);
    addColor(UserColorId::HOME_PLACE_BORDER,
        0xffff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Home place border"),
        200);
    addColor(UserColorId::ROAD_POINT,
        0x000000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Road point"), 100);
    addColor(UserColorId::NET,
        0x000000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Tiles border"), 64);
    addColor(UserColorId::PET,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Pets"));
    addColor(UserColorId::MERCENARY,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Mercenary"));
    addColor(UserColorId::HOMUNCULUS,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Homunculus"));
    commit(true);
}

UserPalette::~UserPalette()
{
    FOR_EACH (Colors::const_iterator, col, mColors)
    {
        if (col->grad == GradientType::LABEL)
            continue;
        const std::string &configName = ColorTypeNames[col->type];
        config.setValue(configName + "Gradient",
            static_cast<int>(col->committedGrad));
        config.setValue(configName + "Delay", col->delay);

        if (col->grad == GradientType::STATIC ||
            col->grad == GradientType::PULSE)
        {
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "0x%06x", col->getRGB());
            buffer[19] = 0;
            config.setValue(configName, std::string(buffer));
        }
    }
}

void UserPalette::setColor(const UserColorIdT type,
                           const int r,
                           const int g,
                           const int b)
{
    Color &color = mColors[static_cast<size_t>(type)].color;
    color.r = r;
    color.g = g;
    color.b = b;
}

void UserPalette::setGradient(const UserColorIdT type,
                              const GradientTypeT grad)
{
    ColorElem *const elem = &mColors[static_cast<size_t>(type)];

    if (elem->grad != GradientType::STATIC && grad == GradientType::STATIC)
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
    else if (elem->grad == GradientType::STATIC &&
             grad != GradientType::STATIC)
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
        if (commitNonStatic || i->grad == GradientType::STATIC)
            i->committedColor = i->color;
        else if (i->grad == GradientType::PULSE)
            i->committedColor = i->testColor;
    }
}

void UserPalette::rollback()
{
    FOR_EACH (Colors::iterator, i, mColors)
    {
        if (i->grad != i->committedGrad)
            setGradient(static_cast<UserColorIdT>(i->type), i->committedGrad);

        const Color &committedColor = i->committedColor;
        setGradientDelay(static_cast<UserColorIdT>(i->type),
            i->committedDelay);
        setColor(static_cast<UserColorIdT>(i->type),
            committedColor.r,
            committedColor.g,
            committedColor.b);

        if (i->grad == GradientType::PULSE)
        {
            Color &testColor = i->testColor;
            testColor.r = committedColor.r;
            testColor.g = committedColor.g;
            testColor.b = committedColor.b;
        }
    }
}

int UserPalette::getColorTypeAt(const int i)
{
    if (i < 0 || i >= getNumberOfElements())
        return 0;

    return mColors[i].type;
}

void UserPalette::addColor(const UserColorIdT type,
                           const unsigned rgb,
                           GradientTypeT grad,
                           const std::string &text,
                           int delay)
{
    const unsigned maxType = sizeof(ColorTypeNames)
        / sizeof(ColorTypeNames[0]);

    if (static_cast<unsigned>(type) >= maxType)
        return;

    const std::string &configName = ColorTypeNames[static_cast<size_t>(type)];
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
    const Color &trueCol = Color(rgbValue);
    grad = static_cast<GradientTypeT>(config.getValue(
        configName + "Gradient",
        static_cast<int>(grad)));
    delay = config.getValueInt(configName + "Delay", delay);
    mColors[static_cast<size_t>(type)].set(static_cast<int>(type),
        trueCol, grad, delay);
    mColors[static_cast<size_t>(type)].text = text;

    if (grad != GradientType::STATIC)
        mGradVector.push_back(&mColors[static_cast<size_t>(type)]);
}

void UserPalette::addLabel(const UserColorIdT type,
                           const std::string &text)
{
    const unsigned maxType = sizeof(ColorTypeNames)
        / sizeof(ColorTypeNames[0]);

    if (static_cast<unsigned>(type) >= maxType)
        return;


    mColors[static_cast<size_t>(type)] = ColorElem();
    const std::string str(" \342\200\225\342\200\225\342\200\225"
        "\342\200\225\342\200\225 ");
    mColors[static_cast<size_t>(type)].grad = GradientTypeT::LABEL;
    mColors[static_cast<size_t>(type)].text =
        std::string(str).append(text).append(str);
}

int UserPalette::getIdByChar(const signed char c, bool &valid) const
{
    const CharColors::const_iterator it = mCharColors.find(c);
    if (it != mCharColors.end())
    {
        valid = true;
        return (*it).second;
    }

    valid = false;
    return 0;
}
