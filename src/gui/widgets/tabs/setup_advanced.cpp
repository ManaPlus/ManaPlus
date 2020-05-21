/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/tabs/setup_advanced.h"

#include "being/localplayer.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"


Setup_Advanced::Setup_Advanced(const Widget2 *const widget) :
    SetupTabScroll(widget)
{
    // TRANSLATORS: settings tab name
    setName(_("Advanced"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Advanced Section (Change only " \
        "in case u know what you are doing!)"),
        "", this,
        Separator_true);

    // FIXME: if(debugCMD[/enabledebug] || gmlvl >= advancedgmsettings@groups.xml) : show
    // TRANSLATORS: settings option
    new SetupItemLabel(_("Debug Section (for debugging only)"),
        "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Disable logging (Default : unchecked)"),
            _("Disables inGame logging (carefull with this!)"),
            "disableLoggingInGame", this, "disableLoggingInGameEvent",
            MainConfig_true);
    // } //debugCMD || gmlvl/groupid

    // FIXME: if(gmlvl >= advancedgmsettings@groups.xml) : show
    // TRANSLATORS: settings option
    new SetupItemLabel(_("Developer Section (you need gmlvl>=5 " \
        "to access this options)"),
        "", this,
        Separator_true);
    // } //gmlvl/groupid


    setDimension(Rect(0, 0, 550, 350));
}
