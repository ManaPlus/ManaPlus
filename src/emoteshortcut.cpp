/*
 *  Extended support for activating emotes
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "emoteshortcut.h"

#include "configuration.h"

#include "being/localplayer.h"

#include "resources/db/emotedb.h"

#include "debug.h"

EmoteShortcut *emoteShortcut = nullptr;

EmoteShortcut::EmoteShortcut():
    mEmoteSelected(0)
{
    for (int i = 0; i < SHORTCUT_EMOTES; i++)
        mEmotes[i] = 0;
}

EmoteShortcut::~EmoteShortcut()
{
    save();
}

void EmoteShortcut::load()
{
    for (unsigned char i = 0, j = 0, sz = EmoteDB::getLast();
         i <= sz && j < SHORTCUT_EMOTES;
         i++)
    {
        const EmoteSprite *const sprite = EmoteDB::getSprite(i, true);
        if (sprite)
        {
            mEmotes[j] = static_cast<unsigned char>(i + 1);
            j ++;
        }
    }
}

void EmoteShortcut::save() const
{
    for (int i = 0; i < SHORTCUT_EMOTES; i++)
    {
        const unsigned char emoteId = mEmotes[i] ? mEmotes[i]
            : static_cast<unsigned char>(0);
        serverConfig.setValue("emoteshortcut" + toString(i),
            static_cast<unsigned int>(emoteId));
    }
}

void EmoteShortcut::useEmote(const int index) const
{
    if (!player_node)
        return;

    if (index > 0 && index <= SHORTCUT_EMOTES)
    {
       if (mEmotes[index - 1] > 0)
          player_node->emote(mEmotes[index - 1]);
    }
}
