/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_CHARDELETECONFIRM_H
#define GUI_WINDOWS_CHARDELETECONFIRM_H

#include "gui/windows/confirmdialog.h"

#include "const/sound.h"

#include "gui/windows/charselectdialog.h"

#include "utils/gettext.h"

#include "localconsts.h"

/**
 * Listener for confirming character deletion.
 */
class CharDeleteConfirm final : public ConfirmDialog
{
    public:
        CharDeleteConfirm(CharSelectDialog *const m, const int index) :
            // TRANSLATORS: char deletion message
            ConfirmDialog(_("Confirm Character Delete"),
                          // TRANSLATORS: char deletion message
                          _("Are you sure you want to delete this character?"),
                          SOUND_REQUEST, false, Modal_false, m),
            mMaster(m),
            mIndex(index)
        {
        }

        A_DELETE_COPY(CharDeleteConfirm)

        void action(const ActionEvent &event) override final
        {
            if (event.getId() == "yes" && (mMaster != nullptr))
                mMaster->askPasswordForDeletion(mIndex);

            ConfirmDialog::action(event);
        }

    private:
        CharSelectDialog *mMaster;
        int mIndex;
};

#endif  // GUI_WINDOWS_CHARDELETECONFIRM_H
