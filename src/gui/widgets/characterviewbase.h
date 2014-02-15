/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_CHARACTERVIEWBASE_H
#define GUI_WIDGETS_CHARACTERVIEWBASE_H

#include "gui/windows/charselectdialog.h"

#include "gui/widgets/container.h"

#include "listeners/actionlistener.h"

#include "localconsts.h"

class CharacterViewBase : public Container,
                          public ActionListener
{
    public:
        A_DELETE_COPY(CharacterViewBase)

        virtual ~CharacterViewBase()
        { }

        virtual void show(const int i) = 0;

        virtual void resize() = 0;

        int getSelected() const
        {
            return mSelected;
        }

    protected:
        CharacterViewBase(CharSelectDialog *const widget, const int padding) :
            Container(widget),
            ActionListener(),
            mParent(widget),
            mPadding(padding),
            mSelected(0)
        {
        }

        CharSelectDialog *mParent;
        int mPadding;
        int mSelected;
};

#endif  // GUI_WIDGETS_CHARACTERVIEWBASE_H
