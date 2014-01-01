/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef TEXTCOMMAND_H
#define TEXTCOMMAND_H

#include <string>

#include "localconsts.h"

const unsigned int MAGIC_START_ID = 340;

class Image;

enum SpellTarget
{
    NOTARGET = 0,
    ALLOWTARGET = 1,
    NEEDTARGET = 2
};

enum MagicSchool
{
    SKILL_MAGIC = 340,
    SKILL_MAGIC_LIFE = 341,
    SKILL_MAGIC_WAR = 342,
    SKILL_MAGIC_TRANSMUTE = 343,
    SKILL_MAGIC_NATURE = 344,
    SKILL_MAGIC_ASTRAL = 345
};

enum TextCommandType
{
    TEXT_COMMAND_MAGIC = 0,
    TEXT_COMMAND_TEXT = 1
};

/**
 * Represents one or more instances of a certain item type.
 */
class TextCommand final
{
    public:
        /**
         * Constructor.
         */
        TextCommand(const unsigned int id, const std::string &symbol,
                    const std::string &command,
                    const std::string &comment, const SpellTarget type,
                    const std::string &icon, const unsigned int basicLvl,
                    const MagicSchool school = SKILL_MAGIC,
                    const unsigned int schoolLvl = 0, const int mana = 0);

        /**
         * Constructor.
         */
        TextCommand(const unsigned int id, const std::string &symbol,
                    const std::string &command, const std::string &comment,
                    const SpellTarget type, const std::string &icon);

        /**
         * Constructor.
         */
        explicit TextCommand(const unsigned int id);

        A_DELETE_COPY(TextCommand)

        /**
         * Destructor.
         */
        ~TextCommand();

        std::string getName() const A_WARN_UNUSED
        { return mCommand; }

        std::string getCommand() const A_WARN_UNUSED
        { return mCommand; }

        std::string getComment() const A_WARN_UNUSED
        { return mComment; }

        std::string getSymbol() const A_WARN_UNUSED
        { return mSymbol; }

        unsigned int getId() const A_WARN_UNUSED
        { return mId; }

        SpellTarget getTargetType() const A_WARN_UNUSED
        { return mTargetType; }

        std::string getIcon() const A_WARN_UNUSED
        { return mIcon; }

        int getMana() const A_WARN_UNUSED
        { return mMana; }

        MagicSchool getSchool() const A_WARN_UNUSED
        { return mSchool; }

        unsigned getBaseLvl() const A_WARN_UNUSED
        { return mBaseLvl; }

        unsigned getSchoolLvl() const A_WARN_UNUSED
        { return mSchoolLvl; }

        TextCommandType getCommandType() const A_WARN_UNUSED
        { return mCommandType; }

        void setCommand(std::string command)
        { mCommand = command; }

        void setComment(std::string comment)
        { mComment = comment; }

        void setSymbol(std::string symbol)
        { mSymbol = symbol; }

        void setId(const unsigned int id)
        { mId = id; }

        void setTargetType(const SpellTarget targetType)
        { mTargetType = targetType; }

        void setIcon(std::string icon)
        { mIcon = icon; loadImage(); }

        void setMana(const unsigned int mana)
        { mMana = mana; }

        void setSchool(const MagicSchool school)
        { mSchool = school; }

        void setBaseLvl(const unsigned int baseLvl)
        { mBaseLvl = baseLvl; }

        void setSchoolLvl(const unsigned int schoolLvl)
        { mSchoolLvl = schoolLvl; }

        void setCommandType(const TextCommandType commandType)
        { mCommandType = commandType; }

        bool isEmpty() const A_WARN_UNUSED
        { return mCommand.empty() && mSymbol.empty(); }

        Image *getImage() const A_WARN_UNUSED
        { return mImage; }

    private:
        void loadImage();

    protected:
        std::string mCommand;
        std::string mComment;
        std::string mSymbol;
        SpellTarget mTargetType;
        std::string mIcon;
        unsigned int mId;
        int mMana;
        MagicSchool mSchool;
        unsigned mBaseLvl;
        unsigned mSchoolLvl;
        TextCommandType mCommandType;
        Image *mImage;
};

#endif  // TEXTCOMMAND_H
