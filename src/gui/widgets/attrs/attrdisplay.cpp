/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/attrs/attrdisplay.h"

#include "being/playerinfo.h"

#include "gui/widgets/layouthelper.h"

#include "utils/delete2.h"
#include "utils/stringutils.h"

#include "debug.h"

AttrDisplay::AttrDisplay(const Widget2 *const widget,
                         const AttributesT id,
                         const std::string &restrict name,
                         const std::string &restrict shortName) :
    Container(widget),
    mId(id),
    mName(name),
    mShortName(shortName),
    mLayout(new LayoutHelper(this)),
    mLabel(new Label(this, name)),
    mValue(new Label(this, "1 "))
{
    setSize(100, 32);

    setSelectable(false);
    mLabel->setAlignment(Graphics::CENTER);
    mValue->setAlignment(Graphics::CENTER);
}

AttrDisplay::~AttrDisplay()
{
    delete2(mLayout)
}

std::string AttrDisplay::update()
{
    const int base = PlayerInfo::getStatBase(mId);
    const int bonus = PlayerInfo::getStatMod(mId);
    std::string value = toString(base + bonus);
    if (bonus != 0)
        value.append(strprintf("=%d%+d", base, bonus));
    mValue->setCaption(value);
    return mName;
}
