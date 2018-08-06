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

#ifndef GUI_WINDOWS_PINCODEDIALOG_H
#define GUI_WINDOWS_PINCODEDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class PasswordField;
class Pincode;

class PincodeDialog final : public Window,
                            public ActionListener
{
    public:
        PincodeDialog(const std::string &restrict title,
                      const std::string &restrict msg,
                      uint32_t seed,
                      Window *const parent);

        A_DELETE_COPY(PincodeDialog)

        ~PincodeDialog() override final;

        void postInit() override final;

        void action(const ActionEvent &event) override final;

        const std::string &getMsg() const A_WARN_UNUSED;

        static bool isActive() noexcept2 A_WARN_UNUSED
        { return instances != 0; }

        void close() override final;

        void updateButtons();

    private:
        static int instances;

        PasswordField *mPasswordField;
        Pincode *mPincode;
        Button *mOkButton;
};

#endif  // GUI_WINDOWS_PINCODEDIALOG_H
