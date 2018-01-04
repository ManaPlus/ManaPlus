/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

UserPalette *userPalette = nullptr;

const std::string ColorTypeNames[CAST_SIZE(
    UserColorId::USER_COLOR_LAST)] =
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
    "ColorPet",
    "ColorMercenary",
    "ColorHomunculus",
    "ColorSkillUnit",
    "ColorParty",
    "ColorGuild",
    "ColorTeam1",
    "ColorTeam2",
    "ColorTeam3",
    "",
    "ColorParticle",
    "ColorPickupInfo",
    "ColorExpInfo",
    "",
    "ColorPlayerHp",
    "ColorPlayerHp2",
    "ColorMonsterHp",
    "ColorMonsterHp2",
    "ColorHomunHp",
    "ColorHomunHp2",
    "ColorMercHp",
    "ColorMercHp2",
    "ColorElementalHp",
    "ColorElementalHp2",
    "",
    "ColorHitPlayerMonster",
    "ColorHitMonsterPlayer",
    "ColorHitPlayerPlayer",
    "ColorHitCritical",
    "ColorHitLocalPlayerMonster",
    "ColorHitLocalPlayerCritical",
    "ColorHitLocalPlayerMiss",
    "ColorMiss",
    "",
    "ColorPortalHighlight",
    "ColorCollisionHighlight",
    "ColorCollisionAirHighlight",
    "ColorCollisionWaterHighlight",
    "ColorCollisionMonsterHighlight",
    "ColorCollisionGroundtopHighlight",
    "ColorWalkableTileHighlight",
    "ColorNet",
    "",
    "ColorAttackRange",
    "ColorAttackRangeBorder",
    "ColorMonsterAttackRange",
    "ColorSkillAttackRange",
    "",
    "ColorFloorItemText",
    "ColorHomePlace",
    "ColorHomePlaceBorder",
    "ColorRoadPoint",
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
           res[pos] = CAST_S8(tolower(typeName[i]));
        }
        pos ++;
    }
    res.erase(pos, res.length() - pos);
    return res;
}

UserPalette::UserPalette() :
    Palette(CAST_S32(UserColorId::USER_COLOR_LAST)),
    ListModel()
{
    mColors[CAST_SIZE(UserColorId::BEING)] = ColorElem();
    mColors[CAST_SIZE(UserColorId::PC)] = ColorElem();
    mColors[CAST_SIZE(UserColorId::SELF)] = ColorElem();
    mColors[CAST_SIZE(UserColorId::GM)] = ColorElem();
    mColors[CAST_SIZE(UserColorId::NPC)] = ColorElem();
    mColors[CAST_SIZE(UserColorId::MONSTER)] = ColorElem();

    addLabel(UserColorId::LABEL_BEING,
        // TRANSLATORS: palette label
        _("Beings"));
    addColor(UserColorId::BEING,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Being"),
        GRADIENT_DELAY);
    addColor(UserColorId::FRIEND,
        0xb0ffb0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Friend names"),
        GRADIENT_DELAY);
    addColor(UserColorId::DISREGARDED,
        0xa00000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Disregarded names"),
        GRADIENT_DELAY);
    addColor(UserColorId::IGNORED,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Ignored names"),
        GRADIENT_DELAY);
    addColor(UserColorId::ERASED,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Erased names"),
        GRADIENT_DELAY);
    addColor(UserColorId::ENEMY,
        0xff4040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Enemy"),
        GRADIENT_DELAY);
    addColor(UserColorId::PC,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Other players names"),
        GRADIENT_DELAY);
    addColor(UserColorId::SELF,
        0xff8040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Own name"),
        GRADIENT_DELAY);
    addColor(UserColorId::GM,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("GM names"),
        GRADIENT_DELAY);
    addColor(UserColorId::NPC,
        0xc8c8ff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("NPCs"),
        GRADIENT_DELAY);
    addColor(UserColorId::MONSTER,
        0xff4040,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monsters"),
        GRADIENT_DELAY);
    addColor(UserColorId::PET,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Pets"),
        GRADIENT_DELAY);
    addColor(UserColorId::MERCENARY,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Mercenary"),
        GRADIENT_DELAY);
    addColor(UserColorId::HOMUNCULUS,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Homunculus"),
        GRADIENT_DELAY);
    addColor(UserColorId::SKILLUNIT,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Skill unit"),
        GRADIENT_DELAY);
    addColor(UserColorId::PARTY,
        0xff00d8,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Party members"),
        GRADIENT_DELAY);
    addColor(UserColorId::GUILD,
        0xff00d8,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Guild members"),
        GRADIENT_DELAY);
    addColor(UserColorId::TEAM1,
        0x0000ff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        strprintf(_("Team %d"), 1),
        GRADIENT_DELAY);
    addColor(UserColorId::TEAM2,
        0x00a020,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        strprintf(_("Team %d"), 2),
        GRADIENT_DELAY);
    addColor(UserColorId::TEAM3,
        0xffff20,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        strprintf(_("Team %d"), 3),
        GRADIENT_DELAY);
    addLabel(UserColorId::LABEL_PARTICLES,
        // TRANSLATORS: palette label
        _("Particles"));
    addColor(UserColorId::PARTICLE,
        0xffffff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Particle effects"),
        GRADIENT_DELAY);
    addColor(UserColorId::PICKUP_INFO,
        0x28dc28,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Pickup notification"),
        GRADIENT_DELAY);
    addColor(UserColorId::EXP_INFO,
        0xffff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Exp notification"),
        GRADIENT_DELAY);
    addLabel(UserColorId::LABEL_HP,
        // TRANSLATORS: palette label
        _("Hp bars"));
    addColor(UserColorId::PLAYER_HP,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Player HP bar"), 50);
    addColor(UserColorId::PLAYER_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Player HP bar (second color)"),
        50);
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
    addColor(UserColorId::HOMUN_HP,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Homunculus HP bar"),
        50);
    addColor(UserColorId::HOMUN_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Homunculus HP bar (second color)"),
        50);
    addColor(UserColorId::MERC_HP,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Mercenary HP bar"),
        50);
    addColor(UserColorId::MERC_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Mercenary HP bar (second color)"),
        50);

    addColor(UserColorId::ELEMENTAL_HP,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Elemental HP bar"),
        50);
    addColor(UserColorId::ELEMENTAL_HP2,
        0xff0000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Elemental HP bar (second color)"),
        50);
    addLabel(UserColorId::LABEL_HITS,
        // TRANSLATORS: palette label
        _("Hits"));
    addColor(UserColorId::HIT_PLAYER_MONSTER,
        0x0064ff,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Player hits monster"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_MONSTER_PLAYER,
        0xff3232,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monster hits player"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_PLAYER_PLAYER,
        0xff5050,
        GradientType::STATIC,
       // TRANSLATORS: palette color
       _("Other player hits local player"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_CRITICAL,
        0xff0000,
        GradientType::RAINBOW,
        // TRANSLATORS: palette color
        _("Critical Hit"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_LOCAL_PLAYER_MONSTER,
        0x00ff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player hits monster"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_LOCAL_PLAYER_CRITICAL,
        0xff0000,
        GradientType::RAINBOW,
        // TRANSLATORS: palette color
        _("Local player critical hit"),
        GRADIENT_DELAY);
    addColor(UserColorId::HIT_LOCAL_PLAYER_MISS,
        0x00ffa6,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Local player miss"),
        GRADIENT_DELAY);
    addColor(UserColorId::MISS, 0xffff00,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Misses"),
        GRADIENT_DELAY);
    addLabel(UserColorId::LABEL_TILES,
        // TRANSLATORS: palette label
        _("Tiles"));
    addColor(UserColorId::PORTAL_HIGHLIGHT,
        0xC80000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Portal highlight"),
        GRADIENT_DELAY);
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
    addColor(UserColorId::MONSTER_COLLISION_HIGHLIGHT,
        0x2050e0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Monster collision highlight"),
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
    addColor(UserColorId::NET,
        0x000000,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Tiles border"), 64);
    addLabel(UserColorId::LABEL_RANGES,
        // TRANSLATORS: palette label
        _("Ranges"));
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
    addColor(UserColorId::SKILL_ATTACK_RANGE,
        0x0,
        GradientType::STATIC,
        // TRANSLATORS: palette color
        _("Skill attack range border"),
        76);
    addLabel(UserColorId::LABEL_OTHER,
        // TRANSLATORS: palette label
        _("Other"));
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
            CAST_S32(col->committedGrad));
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
    Color &color = mColors[CAST_SIZE(type)].color;
    color.r = r;
    color.g = g;
    color.b = b;
}

void UserPalette::setGradient(const UserColorIdT type,
                              const GradientTypeT grad)
{
    ColorElem *const elem = &mColors[CAST_SIZE(type)];

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

    if (CAST_U32(type) >= maxType)
        return;

    const std::string &configName = ColorTypeNames[CAST_SIZE(type)];
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
        CAST_S32(grad)));
    delay = config.getValueInt(configName + "Delay", delay);
    mColors[CAST_SIZE(type)].set(CAST_S32(type),
        trueCol, grad, delay);
    mColors[CAST_SIZE(type)].text = text;

    if (grad != GradientType::STATIC)
        mGradVector.push_back(&mColors[CAST_SIZE(type)]);
}

void UserPalette::addLabel(const UserColorIdT type,
                           const std::string &text)
{
    const unsigned maxType = sizeof(ColorTypeNames)
        / sizeof(ColorTypeNames[0]);

    if (CAST_U32(type) >= maxType)
        return;


    mColors[CAST_SIZE(type)] = ColorElem();
    const std::string str(" \342\200\225\342\200\225\342\200\225"
        "\342\200\225\342\200\225 ");
    mColors[CAST_SIZE(type)].grad = GradientType::LABEL;
    mColors[CAST_SIZE(type)].text =
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
