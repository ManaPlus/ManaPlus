/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/tabs/targetdebugtab.h"

#include "being/localplayer.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

TargetDebugTab::TargetDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    // TRANSLATORS: debug window label
    mTargetLabel(new Label(this, strprintf("%s ?", _("Target:")))),
    // TRANSLATORS: debug window label
    mTargetIdLabel(new Label(this, strprintf("%s ?     ", _("Target Id:")))),
    mTargetTypeLabel(new Label(this, strprintf(
        // TRANSLATORS: debug window label
        "%s ?     ", _("Target type:")))),
    // TRANSLATORS: debug window label
    mTargetLevelLabel(new Label(this, strprintf("%s ?", _("Target level:")))),
    // TRANSLATORS: debug window label
    mTargetRaceLabel(new Label(this, strprintf("%s ?", _("Target race:")))),
    // TRANSLATORS: debug window label
    mTargetPartyLabel(new Label(this, strprintf("%s ?", _("Target party:")))),
    // TRANSLATORS: debug window label
    mTargetGuildLabel(new Label(this, strprintf("%s ?", _("Target guild:")))),
    // TRANSLATORS: debug window label
    mAttackDelayLabel(new Label(this, strprintf("%s ?", _("Attack delay:")))),
    // TRANSLATORS: debug window label
    mMinHitLabel(new Label(this, strprintf("%s ?", _("Minimal hit:")))),
    // TRANSLATORS: debug window label
    mMaxHitLabel(new Label(this, strprintf("%s ?", _("Maximum hit:")))),
    // TRANSLATORS: debug window label
    mCriticalHitLabel(new Label(this, strprintf("%s ?", _("Critical hit:")))),
    // TRANSLATORS: debug window label
    mKarmaLabel(new Label(this, strprintf("%s ?", _("Karma:")))),
    // TRANSLATORS: debug window label
    mMannerLabel(new Label(this, strprintf("%s ?", _("Manner:")))),
    // TRANSLATORS: debug window label
    mEffectsLabel(new Label(this, strprintf("%s ?", _("Effects:"))))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mTargetLabel, 2, 1);
    place(0, 1, mTargetIdLabel, 2, 1);
    place(0, 2, mTargetTypeLabel, 2, 1);
    place(0, 3, mTargetLevelLabel, 2, 1);
    place(0, 4, mTargetRaceLabel, 2, 1);
    place(0, 5, mAttackDelayLabel, 2, 1);
    place(0, 6, mTargetPartyLabel, 2, 1);
    place(0, 7, mTargetGuildLabel, 2, 1);
    place(0, 8, mMinHitLabel, 2, 1);
    place(0, 9, mMaxHitLabel, 2, 1);
    place(0, 10, mCriticalHitLabel, 2, 1);
    place(0, 11, mKarmaLabel, 2, 1);
    place(0, 12, mMannerLabel, 2, 1);
    place(0, 13, mEffectsLabel, 2, 1);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(Rect(0, 0, 600, 300));
}

void TargetDebugTab::logic()
{
    BLOCK_START("TargetDebugTab::logic")
    if ((localPlayer != nullptr) && (localPlayer->getTarget() != nullptr))
    {
        const Being *const target = localPlayer->getTarget();

        // TRANSLATORS: debug window label
        mTargetLabel->setCaption(strprintf("%s %s (%d, %d)", _("Target:"),
            target->getName().c_str(), target->getTileX(),
            target->getTileY()));

        mTargetIdLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Target Id:"), toInt(target->getId(), int)));
        mTargetTypeLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Target type:"), toInt(target->getSubType(), int)));
        if (target->getLevel() != 0)
        {
            mTargetLevelLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Target Level:"), target->getLevel()));
        }
        else
        {
            mTargetLevelLabel->setCaption(strprintf("%s ?",
                // TRANSLATORS: debug window label
                _("Target Level:")));
        }

        mTargetRaceLabel->setCaption(strprintf("%s %s",
            // TRANSLATORS: debug window label
            _("Target race:"), target->getRaceName().c_str()));

        // TRANSLATORS: debug window label
        mTargetPartyLabel->setCaption(strprintf("%s %s", _("Target Party:"),
            target->getPartyName().c_str()));

        // TRANSLATORS: debug window label
        mTargetGuildLabel->setCaption(strprintf("%s %s", _("Target Guild:"),
            target->getGuildName().c_str()));

        mMinHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Minimal hit:"), target->getMinHit()));
        mMaxHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Maximum hit:"), target->getMaxHit()));
        mCriticalHitLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Critical hit:"), target->getCriticalHit()));
        mKarmaLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Karma:"), target->getKarma()));
        mMannerLabel->setCaption(strprintf("%s %d",
            // TRANSLATORS: debug window label
            _("Manner:"), target->getManner()));
        mEffectsLabel->setCaption(strprintf("%s %s",
            // TRANSLATORS: debug window label
            _("Effects:"), target->getStatusEffectsString().c_str()));

        const int delay = target->getAttackDelay();
        if (delay != 0)
        {
            mAttackDelayLabel->setCaption(strprintf("%s %d",
                // TRANSLATORS: debug window label
                _("Attack delay:"), delay));
        }
        else
        {
            mAttackDelayLabel->setCaption(strprintf(
                // TRANSLATORS: debug window label
                "%s ?", _("Attack delay:")));
        }
    }
    else
    {
        // TRANSLATORS: debug window label
        mTargetLabel->setCaption(strprintf("%s ?", _("Target:")));
        // TRANSLATORS: debug window label
        mTargetIdLabel->setCaption(strprintf("%s ?", _("Target Id:")));
        // TRANSLATORS: debug window label
        mTargetTypeLabel->setCaption(strprintf("%s ?", _("Target type:")));
        // TRANSLATORS: debug window label
        mTargetLevelLabel->setCaption(strprintf("%s ?", _("Target Level:")));
        // TRANSLATORS: debug window label
        mTargetPartyLabel->setCaption(strprintf("%s ?", _("Target Party:")));
        // TRANSLATORS: debug window label
        mTargetGuildLabel->setCaption(strprintf("%s ?", _("Target Guild:")));
        // TRANSLATORS: debug window label
        mAttackDelayLabel->setCaption(strprintf("%s ?", _("Attack delay:")));
        // TRANSLATORS: debug window label
        mMinHitLabel->setCaption(strprintf("%s ?", _("Minimal hit:")));
        // TRANSLATORS: debug window label
        mMaxHitLabel->setCaption(strprintf("%s ?", _("Maximum hit:")));
        // TRANSLATORS: debug window label
        mCriticalHitLabel->setCaption(strprintf("%s ?", _("Critical hit:")));
        // TRANSLATORS: debug window label
        mKarmaLabel->setCaption(strprintf("%s ?", _("Karma:")));
        // TRANSLATORS: debug window label
        mMannerLabel->setCaption(strprintf("%s ?", _("Manner:")));
        // TRANSLATORS: debug window label
        mEffectsLabel->setCaption(strprintf("%s ?", _("Effects:")));
    }

    mTargetLabel->adjustSize();
    mTargetIdLabel->adjustSize();
    mTargetTypeLabel->adjustSize();
    mTargetLevelLabel->adjustSize();
    mTargetPartyLabel->adjustSize();
    mTargetGuildLabel->adjustSize();
    mAttackDelayLabel->adjustSize();
    mMinHitLabel->adjustSize();
    mMaxHitLabel->adjustSize();
    mCriticalHitLabel->adjustSize();
    mKarmaLabel->adjustSize();
    mMannerLabel->adjustSize();
    mEffectsLabel->adjustSize();
    BLOCK_END("TargetDebugTab::logic")
}
