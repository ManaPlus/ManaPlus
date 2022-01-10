/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef GUI_WIDGETS_TABS_SOCIALTAB_H
#define GUI_WIDGETS_TABS_SOCIALTAB_H

#include "gui/windows/socialwindow.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/textdialog.h"

#include "gui/widgets/avatarlistbox.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/scrollarea.h"

#include "gui/widgets/tabs/tab.h"

#include "localconsts.h"

class AvatarListModel;

class SocialTab notfinal : public Tab
{
    public:
        A_DELETE_COPY(SocialTab)

        virtual void invite()
        {
        }

        virtual void leave()
        {
        }

        virtual void updateList()
        {
        }

        virtual void updateAvatar(const std::string &name A_UNUSED)
        {
        }

        virtual void resetDamage(const std::string &name A_UNUSED)
        {
        }

        virtual void selectIndex(const unsigned num A_UNUSED)
        {
        }

        virtual void buildCounter(const int online A_UNUSED,
                                  const int total A_UNUSED)
        {
        }

    protected:
        friend class SocialWindow;

        explicit SocialTab(const Widget2 *const widget) :
            Tab(widget),
            mInviteDialog(nullptr),
            mConfirmDialog(nullptr),
            mScroll(nullptr),
            mList(nullptr),
            mCounterString(),
            mMenuAction("menu")
        {
        }

        ~SocialTab() override
        {
            // Cleanup dialogs
            if (mInviteDialog != nullptr)
            {
                mInviteDialog->close();
                mInviteDialog->scheduleDelete();
                mInviteDialog = nullptr;
            }

            if (mConfirmDialog != nullptr)
            {
                mConfirmDialog->close();
                mConfirmDialog->scheduleDelete();
                mConfirmDialog = nullptr;
            }
        }

        void createControls(AvatarListModel *const listModel,
                            const Opaque showBackground)
        {
            CREATEWIDGETV(mList, AvatarListBox, this, listModel);
            mScroll = new ScrollArea(this, mList, showBackground,
                "social_background.xml");

            mScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_AUTO);
            mScroll->setVerticalScrollPolicy(ScrollArea::SHOW_ALWAYS);
        }

        void setCurrent() override final
        {
            updateCounter();
            updateMenu();
        }

        void updateCounter() const
        {
            if (socialWindow != nullptr)
                socialWindow->updateCounter(this, mCounterString);
        }

        void updateMenu() const
        {
            if (socialWindow != nullptr)
                socialWindow->updateMenu(this, mMenuAction);
        }

        TextDialog *mInviteDialog;
        ConfirmDialog *mConfirmDialog;
        ScrollArea *mScroll;
        AvatarListBox *mList;
        std::string mCounterString;
        std::string mMenuAction;
};

#endif  // GUI_WIDGETS_TABS_SOCIALTAB_H
