/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "gui/setupactiondata.h"

#include "gui/models/touchactionmodel.h"

#include <algorithm>

#include "debug.h"

static class SortTouchActionFunctor final
{
    public:
        bool operator() (const SetupActionData *const data1,
                         const SetupActionData *const data2) const
        {
            if (!data1 || !data2)
                return false;
            return data1->name < data2->name;
        }
} touchActionSorter;

TouchActionsModel::TouchActionsModel() :
    NamesModel(),
    mActionId(),
    mActionToSelection()
{
    std::vector<SetupActionData*> data;

    for (int f = 0, sz = touchActionDataSize; f < sz; f ++)
    {
        int k = 0;
        while (!touchActionData[f][k].name.empty())
        {
            data.push_back(&touchActionData[f][k]);
            k ++;
        }
    }

    std::sort(data.begin(), data.end(), touchActionSorter);
    int cnt = 0;
    FOR_EACH (std::vector<SetupActionData*>::iterator, it, data)
    {
        const SetupActionData *const data1 = *it;
        mNames.push_back(data1->name);
        mActionId.push_back(data1->actionId);
        mActionToSelection[data1->actionId] = cnt;
        cnt ++;
    }
}

int TouchActionsModel::getActionFromSelection(const int sel) const
{
    if (sel < 0 || sel > static_cast<signed int>(mActionId.size()))
        return -1;
    return mActionId[sel];
}

int TouchActionsModel::getSelectionFromAction(const int action) const
{
    const std::map<int, int>::const_iterator it
        = mActionToSelection.find(action);
    if (it == mActionToSelection.end())
        return 0;
    return (*it).second;
}
