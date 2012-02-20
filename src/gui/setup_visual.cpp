/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  Andrei Karas
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/setup_visual.h"

#include "gui/chatwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"
#include "gui/widgets/slider.h"

#include "resources/image.h"

#include "configuration.h"
#include "localplayer.h"
#include "logger.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Visual::Setup_Visual()
{
    setName(_("Visual"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemCheckBox(_("Show pickup notifications in chat"), "",
        "showpickupchat", this, "showpickupchatEvent");

    new SetupItemCheckBox(_("Show pickup notifications as particle effects"),
        "", "showpickupparticle", this, "showpickupparticleEvent");

    new SetupItemCheckBox(_("Grab mouse and keyboard input"),
        "", "grabinput", this, "grabinputEvent");

    new SetupItemSlider(_("Gui opacity"), "", "guialpha",
        this, "guialphaEvent", 0.1, 1.0);

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

void Setup_Visual::apply()
{
    SetupTabScroll::apply();
    Image::setEnableAlpha(config.getFloatValue("guialpha") != 1.0f);
    Client::applyGrabMode();
}

void Setup_Visual::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "guialphaEvent")
    {
        Slider *slider = static_cast<Slider*>(event.getSource());
        if (slider)
        {
            config.setValue("guialpha", slider->getValue());
            Image::setEnableAlpha(config.getFloatValue("guialpha") != 1.0f);
        }
    }
}
