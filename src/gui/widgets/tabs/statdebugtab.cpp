/*
 *  The ManaPlus Client
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

#include "gui/widgets/tabs/statdebugtab.h"

#include "const/utils/timer.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "gui/windows/chatwindow.h"

#include "utils/gettext.h"
#include "utils/perfstat.h"
#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

StatDebugTab::StatDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    // TRANSLATORS: debug window label, logic per second
    mLPSLabel(new Label(this, strprintf(_("LPS: %d"), 0))),
    // TRANSLATORS: debug window stats reset button
    mResetButton(new Button(this, _("Reset"), "reset", BUTTON_SKIN, this)),
    // TRANSLATORS: debug window stats copy button
    mCopyButton(new Button(this, _("Copy"), "copy", BUTTON_SKIN, this)),
    mStatLabels(),
    mWorseStatLabels(),
    mDrawIndex(0)
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    mResetButton->adjustSize();
    mCopyButton->adjustSize();

    place(0, 0, mLPSLabel, 2, 1);
    place(0, 1, mResetButton, 1, 1);
    place(1, 1, mCopyButton, 1, 1);
    for (size_t f = 1; f < PERFSTAT_LAST_STAT; f ++)
    {
        mStatLabels[f - 1] = new Label(this,
            // TRANSLATORS: debug window stat label
            strprintf(_("stat%u: %d ms"), CAST_U32(f), 1000));
        mStatLabels[f - 1]->adjustSize();
        mWorseStatLabels[f - 1] = new Label(this,
            // TRANSLATORS: debug window stat label
            strprintf(_("%d ms"), 1000));
        place(0, CAST_S32(f + 1), mStatLabels[f - 1], 3, 1);
        place(3, CAST_S32(f + 1), mWorseStatLabels[f - 1], 1, 1);
    }

    setDimension(Rect(0, 0, 200, 300));
}

void StatDebugTab::logic()
{
    BLOCK_START("StatDebugTab::logic")
    // TRANSLATORS: debug window label, logic per second
    mLPSLabel->setCaption(strprintf(_("LPS: %d"), lps));

    for (size_t f = 1; f < PERFSTAT_LAST_STAT; f ++)
    {
        mStatLabels[f - 1]->setCaption(
            // TRANSLATORS: debug window stat label
            strprintf(_("stat%u: %d ms"),
            CAST_U32(f),
            Perf::getTime(prevPerfFrameId, f) * MILLISECONDS_IN_A_TICK));
        mWorseStatLabels[f - 1]->setCaption(
            // TRANSLATORS: debug window stat label
            strprintf(_("%d ms"),
            Perf::getWorstTime(f) * MILLISECONDS_IN_A_TICK));
    }
    mDrawIndex = prevPerfFrameId;
    BLOCK_END("StatDebugTab::logic")
}

void StatDebugTab::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "reset")
    {
        Perf::init();
    }
    else if (eventId == "copy")
    {
        std::string data("perf:");
        for (size_t f = 1; f < PERFSTAT_LAST_STAT; f ++)
        {
            data.append(strprintf(" %d",
                Perf::getTime(mDrawIndex, f) * MILLISECONDS_IN_A_TICK));
        }
        data.append(",");
        for (size_t f = 1; f < PERFSTAT_LAST_STAT; f ++)
        {
            data.append(strprintf(" %d",
                Perf::getWorstTime(f) * MILLISECONDS_IN_A_TICK));
        }
        chatWindow->addInputText(data,
            true);
    }
}
