/*
 *  Support for custom units
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "units.h"

#include "configuration.h"
#include "logger.h"

#include "utils/stringutils.h"
#include "utils/xml.h"

#include <climits>
#include <vector>

#include "debug.h"

struct UnitLevel final
{
    std::string symbol;
    int count;
    int round;
    std::string separator;
};

struct UnitDescription final
{
    std::vector<struct UnitLevel> levels;
    double conversion;
    bool mix;
};

enum UnitType
{
    UNIT_WEIGHT = 0,
    UNIT_CURRENCY = 1,
    UNIT_END
};

static std::string formatUnit(const int value, const int type);

static std::string splitNumber(std::string str, const std::string &separator);

struct UnitDescription units[UNIT_END];

void Units::loadUnits()
{
    { // Setup default weight
        UnitDescription ud;

        ud.conversion = 1.0;
        ud.mix = false;

        const UnitLevel bu =
        {
            "g",
            1,
            0,
            ""
        };
        ud.levels.push_back(bu);

        const UnitLevel ul =
        {
            "kg",
            1000,
            2,
            ""
        };
        ud.levels.push_back(ul);

        units[UNIT_WEIGHT] = ud;
    }

    { // Setup default currency
        UnitDescription ud;

        ud.conversion = 1.0;
        ud.mix = false;

        UnitLevel bu = {"¤", 1, 0, ""};
        ud.levels.push_back(bu);

        units[UNIT_CURRENCY] = ud;
    }

    XML::Document doc(paths.getStringValue("unitsFile"));
    const XmlNodePtr root = doc.rootNode();

    if (!root || !xmlNameEqual(root, "units"))
    {
        logger->log("Error loading unit definition file: "
            + paths.getStringValue("unitsFile"));
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "unit"))
        {
            UnitDescription ud;
            int level = 1;
            const std::string type = XML::getProperty(node, "type", "");
            ud.conversion = XML::getProperty(node, "conversion", 1);
            ud.mix = XML::getProperty(node, "mix", "no") == "yes";

            UnitLevel bu;
            bu.symbol = XML::getProperty(node, "base", "¤");
            bu.count = 1;
            bu.round = XML::getProperty(node, "round", 2);
            bu.separator = XML::getProperty(node, "separator", " ");

            ud.levels.push_back(bu);

            for_each_xml_child_node(uLevel, node)
            {
                if (xmlNameEqual(uLevel, "level"))
                {
                    const UnitLevel ul =
                    {
                        XML::getProperty(uLevel, "symbol",
                                         strprintf("¤%d", level)),
                        XML::getProperty(uLevel, "count", -1),
                        XML::getProperty(uLevel, "round", bu.round),
                        XML::getProperty(uLevel, "separator", bu.separator)
                    };

                    if (ul.count > 0)
                    {
                        ud.levels.push_back(ul);
                        level++;
                    }
                    else
                    {
                        logger->log("Error bad unit count: %d for %s in %s",
                                    ul.count, ul.symbol.c_str(),
                                    bu.symbol.c_str());
                    }
                }
            }

            // Add one more level for saftey
            const UnitLevel lev =
            {
                "",
                INT_MAX,
                0,
                ""
            };
            ud.levels.push_back(lev);

            if (type == "weight")
                units[UNIT_WEIGHT] = ud;
            else if (type == "currency")
                units[UNIT_CURRENCY] = ud;
            else
                logger->log("Error unknown unit type: %s", type.c_str());
        }
    }
}

static std::string formatUnit(const int value, const int type)
{
    const UnitDescription ud = units[type];
    UnitLevel ul;

    // Shortcut for 0; do the same for values less than 0  (for now)
    if (value <= 0)
    {
        ul = ud.levels[0];
        return strprintf("0%s", ul.symbol.c_str());
    }
    else
    {
        double amount = ud.conversion * value;
        const unsigned int sz = ud.levels.size();

        // If only the first level is needed, act like mix if false
        if (ud.mix && !ud.levels.empty() && ud.levels[1].count < amount)
        {
            std::string output;
            UnitLevel pl = ud.levels[0];
            ul = ud.levels[1];
            int levelAmount = static_cast<int>(amount);
            int nextAmount = 0;

            if (ul.count)
                levelAmount /= ul.count;

            amount -= levelAmount * ul.count;

            if (amount > 0)
            {
                output = splitNumber(strprintf("%.*f", pl.round,
                    amount), pl.separator).append(pl.symbol);
            }

            for (unsigned int i = 2; i < sz; i++)
            {
                pl = ul;
                ul = ud.levels[i];

                if (ul.count)
                {
                    nextAmount = levelAmount / ul.count;
                    levelAmount %= ul.count;
                }

                if (levelAmount > 0)
                {
                    output = splitNumber(strprintf("%d", levelAmount),
                        pl.separator).append(pl.symbol).append(output);
                }

                if (!nextAmount)
                    break;
                levelAmount = nextAmount;
            }

            return output;
        }
        else
        {
            ul.round = 0;
            for (unsigned int i = 0; i < sz; i++)
            {
                ul = ud.levels[i];
                if (amount < ul.count && ul.count > 0)
                {
                    ul = ud.levels[i - 1];
                    break;
                }
                if (ul.count)
                    amount /= ul.count;
            }

            return splitNumber(strprintf("%.*f", ul.round, amount),
                ul.separator).append(ul.symbol);
        }
    }
}

std::string Units::formatCurrency(const int value)
{
    return formatUnit(value, UNIT_CURRENCY);
}

std::string Units::formatWeight(const int value)
{
    return formatUnit(value, UNIT_WEIGHT);
}

static std::string splitNumber(std::string str, const std::string &separator)
{
    std::string lastPart;
    const size_t point = str.find(".");
    if (point != std::string::npos)
    {
        lastPart = str.substr(point);
        str = str.substr(0, point);
    }
    std::string result;

    if (!str.empty())
    {
        size_t sz = str.size();
        while (sz >= 3)
        {
            if (sz >= 6)
            {
                result = std::string(separator).append(str.substr(
                    sz - 3)).append(result);
            }
            else
            {
                result = str.substr(sz - 3).append(result);
            }
            str = str.substr(0, str.size() - 3);
            sz = str.size();
        }
        if (!str.empty())
        {
            if (!result.empty())
                result = std::string(str).append(separator).append(result);
            else
                result = str;
        }
    }

    return result + lastPart;
}
