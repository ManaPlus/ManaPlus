/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "gui/widgets/setuptouchitem.h"

#include "gui/setupactiondata.h"

#include "gui/widgets/dropdown.h"
#include "gui/widgets/horizontcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/vertcontainer.h"

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


SetupActionDropDown::SetupActionDropDown(const std::string &restrict text,
                                         const std::string &restrict
                                         description,
                                         const std::string &restrict keyName,
                                         SetupTabScroll *restrict const parent,
                                         const std::string &restrict eventName,
                                         TouchActionsModel *restrict
                                         const model,
                                         const int width,
                                         const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
{
    mValueType = VSTR;
    createControls();
}

SetupActionDropDown::SetupActionDropDown(const std::string &restrict text,
                                         const std::string &restrict
                                         description,
                                         const std::string &restrict keyName,
                                         SetupTabScroll *restrict const parent,
                                         const std::string &restrict eventName,
                                         TouchActionsModel *restrict
                                         const model,
                                         const int width,
                                         const std::string &restrict def,
                                         const bool mainConfig) :
    SetupItem(text, description, keyName, parent, eventName, def, mainConfig),
    mHorizont(nullptr),
    mLabel(nullptr),
    mModel(model),
    mDropDown(nullptr),
    mWidth(width)
{
    mValueType = VSTR;
    createControls();
}

SetupActionDropDown::~SetupActionDropDown()
{
    mHorizont = nullptr;
    mWidget = nullptr;
    mModel = nullptr;
    mDropDown = nullptr;
    mLabel = nullptr;
}

void SetupActionDropDown::createControls()
{
    if (!mModel)
        return;

    load();
    mHorizont = new HorizontContainer(this, 32, 2);

    mLabel = new Label(this, mText);
    mDropDown = new DropDown(this, mModel);
    mDropDown->setActionEventId(mEventName);
    mDropDown->addActionListener(mParent);
    mDropDown->setWidth(mWidth);
    mDropDown->setSelected(mModel->getSelectionFromAction(
        atoi(mValue.c_str())));

    mWidget = mDropDown;
    fixFirstItemSize(mLabel);
    mHorizont->add(mLabel);
    mHorizont->add(mDropDown);

    mParent->getContainer()->add2(mHorizont, true, 4);
    mParent->addControl(this);
    mParent->addActionListener(this);
    mWidget->addActionListener(this);
}

void SetupActionDropDown::fromWidget()
{
    if (!mDropDown || !mModel)
        return;

    mValue = toString(mModel->getActionFromSelection(
        mDropDown->getSelected()));
}

void SetupActionDropDown::toWidget()
{
    if (!mDropDown || !mModel)
        return;

    mDropDown->setSelected(mModel->getSelectionFromAction(
        atoi(mValue.c_str())));
}
