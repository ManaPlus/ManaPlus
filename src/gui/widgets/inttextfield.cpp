/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/inttextfield.h"

#include "keydata.h"
#include "keyevent.h"

#include "gui/sdlinput.h"

#include "utils/stringutils.h"

#include "debug.h"

IntTextField::IntTextField(const int def, const int min, const int max,
                           const bool enabled, const int width) :
    TextField(toString(def)),
    mMin(0),
    mMax(0),
    mDefault(def),
    mValue(def)
{
    if (min != 0 || max != 0)
        setRange(min, max);

    setEnabled(enabled);
    if (width != 0)
        setWidth(width);
}

void IntTextField::keyPressed(gcn::KeyEvent &event)
{
    const int action = static_cast<KeyEvent*>(&event)->getActionId();

    if (action == Input::KEY_GUI_DELETE || action == Input::KEY_GUI_BACKSPACE)
    {
        setText(std::string());
        if (mSendAlwaysEvents)
            distributeActionEvent();

        event.consume();
    }

    if (!event.getKey().isNumber())
        return;

    TextField::keyPressed(event);

    std::istringstream s(getText());
    int i;
    s >> i;
    setValue(i);
    if (mSendAlwaysEvents)
        distributeActionEvent();
}

void IntTextField::setRange(const int min, const int max)
{
    mMin = min;
    mMax = max;

    if (mValue < mMin)
        mValue = mMin;
    else if (mValue > mMax)
        mValue = mMax;

    if (mDefault < mMin)
        mDefault = mMin;
    else if (mDefault > mMax)
        mDefault = mMax;
}

int IntTextField::getValue() const
{
    return getText().empty() ? mMin : mValue;
}

void IntTextField::setValue(const int i)
{
    if (i < mMin)
        mValue = mMin;
    else if (i > mMax)
        mValue = mMax;
    else
        mValue = i;

    const std::string valStr = toString(mValue);
    setText(valStr);
    setCaretPosition(static_cast<unsigned>(valStr.length()) + 1);
}

void IntTextField::setDefaultValue(const int value)
{
    if (value < mMin)
        mDefault = mMin;
    else if (value > mMax)
        mDefault = mMax;
    else
        mDefault = value;
}

void IntTextField::reset()
{
    setValue(mDefault);
}
