/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "enums/commandtarget.h"
#ifdef TMWA_SUPPORT
#include "enums/magicschool.h"
#include "enums/textcommandtype.h"
#endif

#include <string>

#include "localconsts.h"

class Image;

/**
 * Represents one or more instances of a certain item type.
 */
class TextCommand final
{
    public:
#ifdef TMWA_SUPPORT
        /**
         * Constructor.
         */
        TextCommand(const int id,
                    const std::string &symbol,
                    const std::string &command,
                    const std::string &comment,
                    const CommandTargetT type,
                    const std::string &icon,
                    const unsigned int basicLvl,
                    const MagicSchoolT school,
                    const unsigned int schoolLvl,
                    const unsigned int mana);
#endif  // TMWA_SUPPORT

        /**
         * Constructor.
         */
        TextCommand(const int id,
                    const std::string &symbol,
                    const std::string &command,
                    const std::string &comment,
                    const CommandTargetT type,
                    const std::string &icon);

        /**
         * Constructor.
         */
        explicit TextCommand(const int id);

        A_DELETE_COPY(TextCommand)

        /**
         * Destructor.
         */
        ~TextCommand();

        std::string getName() const noexcept2 A_WARN_UNUSED
        { return mCommand; }

        std::string getCommand() const noexcept2 A_WARN_UNUSED
        { return mCommand; }

        std::string getComment() const noexcept2 A_WARN_UNUSED
        { return mComment; }

        std::string getSymbol() const noexcept2 A_WARN_UNUSED
        { return mSymbol; }

        int getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        CommandTargetT getTargetType() const noexcept2 A_WARN_UNUSED
        { return mTargetType; }

        std::string getIcon() const noexcept2 A_WARN_UNUSED
        { return mIcon; }

#ifdef TMWA_SUPPORT
        unsigned int getMana() const noexcept2 A_WARN_UNUSED
        { return mMana; }

        MagicSchoolT getSchool() const noexcept2 A_WARN_UNUSED
        { return mSchool; }

        unsigned getBaseLvl() const noexcept2 A_WARN_UNUSED
        { return mBaseLvl; }

        unsigned getSchoolLvl() const noexcept2 A_WARN_UNUSED
        { return mSchoolLvl; }

        void setMana(const unsigned int mana)
        { mMana = mana; }

        void setSchool(const MagicSchoolT school)
        { mSchool = school; }

        void setBaseLvl(const unsigned int baseLvl)
        { mBaseLvl = baseLvl; }

        void setSchoolLvl(const unsigned int schoolLvl)
        { mSchoolLvl = schoolLvl; }

        TextCommandTypeT getCommandType() const noexcept2 A_WARN_UNUSED
        { return mCommandType; }

        void setCommandType(const TextCommandTypeT commandType)
        { mCommandType = commandType; }
#endif  // TMWA_SUPPORT

        void setCommand(const std::string &command)
        { mCommand = command; }

        void setComment(const std::string &comment)
        { mComment = comment; }

        void setSymbol(const std::string &symbol)
        { mSymbol = symbol; }

        void setId(const int id)
        { mId = id; }

        void setTargetType(const CommandTargetT targetType)
        { mTargetType = targetType; }

        void setIcon(const std::string &icon)
        { mIcon = icon; loadImage(); }

        bool isEmpty() const noexcept2 A_WARN_UNUSED
        { return mCommand.empty() && mSymbol.empty(); }

        Image *getImage() const noexcept2 A_WARN_UNUSED
        { return mImage; }

    private:
        void loadImage();

    protected:
        std::string mCommand;
        std::string mComment;
        std::string mSymbol;
        CommandTargetT mTargetType;
        std::string mIcon;
        int mId;
#ifdef TMWA_SUPPORT
        unsigned int mMana;
        MagicSchoolT mSchool;
        unsigned mBaseLvl;
        unsigned mSchoolLvl;
        TextCommandTypeT mCommandType;
#endif  // TMWA_SUPPORT

        Image *mImage;
};

#endif  // TEXTCOMMAND_H
