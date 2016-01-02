/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_POPUPS_CREATEPARTYPOPUP_H
#define GUI_POPUPS_CREATEPARTYPOPUP_H

#include "gui/widgets/browserbox.h"
#include "gui/widgets/linkhandler.h"
#include "gui/widgets/popup.h"
#include "gui/windows/socialwindow.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "localconsts.h"

class CreatePartyPopup final : public Popup,
                               public LinkHandler
{
    public:
        CreatePartyPopup() :
            Popup("SocialCreatePartyPopup"),
            LinkHandler(),
            mBrowserBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
                "popupbrowserbox.xml"))
        {
            mBrowserBox->setPosition(4, 4);
            mBrowserBox->setOpaque(false);
            mBrowserBox->setLinkHandler(this);

            // TRANSLATORS: party popup item
            mBrowserBox->addRow(strprintf("@@party|%s@@", _("Create Party")));
            mBrowserBox->addRow("##3---");
            // TRANSLATORS: party popup item
            mBrowserBox->addRow(strprintf("@@cancel|%s@@", _("Cancel")));
        }

        void postInit() override final
        {
            Popup::postInit();
            add(mBrowserBox);
            setContentSize(mBrowserBox->getWidth() + 8,
                mBrowserBox->getHeight() + 8);
        }

        A_DELETE_COPY(CreatePartyPopup)

        void handleLink(const std::string &link,
                        MouseEvent *event A_UNUSED) override final
        {
            if (link == "guild" && socialWindow)
            {
                socialWindow->showGuildCreate();
            }
            else if (link == "party" && socialWindow)
            {
                socialWindow->showPartyCreate();
            }

            setVisible(Visible_false);
        }

        void show(Widget *parent)
        {
            if (!parent)
                return;

            int x, y;
            parent->getAbsolutePosition(x, y);
            y += parent->getHeight();
            setPosition(x, y);
            setVisible(Visible_true);
            requestMoveToTop();
        }

    private:
        BrowserBox* mBrowserBox;
};

#endif  // GUI_POPUPS_CREATEPARTYPOPUP_H
