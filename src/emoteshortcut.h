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

#ifndef EMOTESHORTCUT_H
#define EMOTESHORTCUT_H

#include "localconsts.h"

static const int SHORTCUT_EMOTES = 48;

/**
 * The class which keeps track of the emote shortcuts.
 */
class EmoteShortcut final
{
    public:
        /**
         * Constructor.
         */
        EmoteShortcut();

        A_DELETE_COPY(EmoteShortcut)

        /**
         * Destructor.
         */
        ~EmoteShortcut();

        /**
         * Load the configuration information.
         */
        void load();

        /**
         * Returns the shortcut Emote ID specified by the index.
         *
         * @param index Index of the shortcut Emote.
         */
        unsigned char getEmote(const int index) const A_WARN_UNUSED
        { return mEmotes[index]; }

        /**
         * Returns the amount of shortcut Emotes.
         */
        static unsigned int getEmoteCount() A_WARN_UNUSED
        { return SHORTCUT_EMOTES; }

        /**
         * Returns the emote ID that is currently selected.
         */
        unsigned char getEmoteSelected() const A_WARN_UNUSED
        { return mEmoteSelected; }

        /**
         * Adds the selected emote ID to the emotes specified by the index.
         *
         * @param index Index of the emotes.
         */
        void setEmote(const int index)
        { mEmotes[index] = mEmoteSelected; }

        /**
         * Adds a emoticon to the emotes store specified by the index.
         *
         * @param index Index of the emote.
         * @param emoteId ID of the emote.
         */
        void setEmotes(const int index, const unsigned char emoteId)
        { mEmotes[index] = emoteId; }

        /**
         * Set the Emote that is selected.
         *
         * @param emoteId The ID of the emote that is to be assigned.
         */
        void setEmoteSelected(const unsigned char emoteId)
        { mEmoteSelected = emoteId; }

        /**
         * A flag to check if the Emote is selected.
         */
        bool isEmoteSelected() const A_WARN_UNUSED
        { return mEmoteSelected; }

        /**
         * Remove a Emote from the shortcut.
         */
        void removeEmote(const int index)
        { if (index >= 0 && index < SHORTCUT_EMOTES) mEmotes[index] = 0; }

        /**
         * Try to use the Emote specified by the index.
         *
         * @param index Index of the emote shortcut.
         */
        void useEmote(const int index) const;

    private:
        /**
         * Save the configuration information.
         */
        void save() const;

        unsigned char mEmotes[SHORTCUT_EMOTES];  // The emote stored.
        unsigned char mEmoteSelected;            // The emote held by cursor.
};

extern EmoteShortcut *emoteShortcut;

#endif  // EMOTESHORTCUT_H
