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

#include "gui/widgets/tabs/chat/langtab.h"

#include "utils/gettext.h"

#include "debug.h"

LangTab *langChatTab = nullptr;

LangTab::LangTab(const Widget2 *const widget,
                 const std::string &lang) :
    // TRANSLATORS: lang chat tab name
    ChatTab(widget, _("Lang"), lang + " ", "#Lang", ChatTabType::LANG)
{
}

LangTab::~LangTab()
{
}
