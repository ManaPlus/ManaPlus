/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2021  Andrei Karas
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

#include "spellmanager.h"

#include "configuration.h"
#include "textcommand.h"

#include "being/localplayer.h"
#ifdef TMWA_SUPPORT
#include "being/playerinfo.h"
#endif  // TMWA_SUPPORT

#include "const/spells.h"

#include "gui/windows/chatwindow.h"

#include "net/playerhandler.h"

#include "resources/db/commandsdb.h"

#include "utils/dtor.h"
#include "utils/foreach.h"

#include <sstream>

#include "debug.h"

SpellManager *spellManager = nullptr;

SpellManager::SpellManager() :
    mSpells(),
    mSpellsVector()
{
    load();
}

SpellManager::~SpellManager()
{
    delete_all(mSpells);
    mSpells.clear();
    mSpellsVector.clear();
}

TextCommand* SpellManager::getSpell(const int spellId) const
{
    if (spellId < 0 || CAST_SIZE(spellId) >= mSpells.size())
        return nullptr;

    const std::map<unsigned int, TextCommand*>::const_iterator
        it = mSpells.find(spellId);

    return it != mSpells.end() ? (*it).second : nullptr;
}

const TextCommand* SpellManager::getSpellByItem(const int itemId) const
{
    return getSpell(itemId - SPELL_MIN_ID);
}

void SpellManager::fillSpells()
{
    CommandsDB::load();

    CommandsMap &commands = CommandsDB::getAll();
    FOR_EACH (CommandsMapIter, it, commands)
        addSpell((*it).second);

    for (unsigned f = 0; f < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; f++)
    {
        const std::map<unsigned int, TextCommand*>::const_iterator
            it = mSpells.find(f);
        if (it == mSpells.end())
            addSpell(new TextCommand(f));
    }
    CommandsDB::unload();
}

bool SpellManager::addSpell(TextCommand *const spell)
{
    if (spell == nullptr)
        return false;

    const int id = spell->getId();
    if (id < 0 || id >= CAST_S32(SPELL_SHORTCUT_ITEMS
        * SPELL_SHORTCUT_TABS))
    {
        delete spell;
        return false;
    }
    const std::map<unsigned int, TextCommand*>::const_iterator
        i = mSpells.find(spell->getId());
    if (i == mSpells.end())
    {
        mSpells[spell->getId()] = spell;
        mSpellsVector.push_back(spell);
        return true;
    }
    return false;
}

const STD_VECTOR<TextCommand*> &SpellManager::getAll() const
{
    return mSpellsVector;
}

void SpellManager::useItem(const int itemId) const
{
    invoke(itemId - SPELL_MIN_ID);
}

void SpellManager::invoke(const int spellId) const
{
    if (localPlayer == nullptr)
        return;

    const TextCommand *const spell = getSpell(spellId);
    if (spell == nullptr)
        return;

    if ((playerHandler == nullptr) || spell->getCommand().empty())
        return;

#ifdef TMWA_SUPPORT
    if (spell->getCommandType() == TextCommandType::Text ||
        (playerHandler->canUseMagic() &&
        PlayerInfo::getSkillLevel(CAST_S32(MagicSchool::SkillMagic))
        >= CAST_S32(spell->getBaseLvl()) &&
        PlayerInfo::getSkillLevel(CAST_S32(
        spell->getSchool())) >= CAST_S32(spell->getSchoolLvl())
        && PlayerInfo::getAttribute(Attributes::PLAYER_MP)
        >= CAST_S32(spell->getMana()))
        )
#endif  // TMWA_SUPPORT
    {
        const Being *const target = localPlayer->getTarget();
        if (spell->getTargetType() == CommandTarget::NoTarget)
        {
            invokeSpell(spell);
        }
#ifdef TMWA_SUPPORT
        if ((target != nullptr &&
            (target->getType() != ActorType::Monster ||
            spell->getCommandType() == TextCommandType::Text)) &&
            (spell->getTargetType() == CommandTarget::AllowTarget ||
            spell->getTargetType() == CommandTarget::NeedTarget))
#else  // TMWA_SUPPORT

        if (target != nullptr &&
            (spell->getTargetType() == CommandTarget::AllowTarget ||
            spell->getTargetType() == CommandTarget::NeedTarget))
#endif  // TMWA_SUPPORT
        {
            invokeSpell(spell, target);
        }
        else if (spell->getTargetType() == CommandTarget::AllowTarget)
        {
            invokeSpell(spell);
        }
    }
}

void SpellManager::invokeSpell(const TextCommand *const spell)
{
    if ((chatWindow == nullptr) || (spell == nullptr))
        return;
    chatWindow->localChatInput(parseCommand(spell->getCommand(), nullptr));
}

void SpellManager::invokeSpell(const TextCommand *const spell,
                               const Being *const target)
{
    if (chatWindow == nullptr ||
        spell == nullptr ||
        target == nullptr)
    {
        return;
    }
    chatWindow->localChatInput(parseCommand(spell->getCommand(), target));
}

void SpellManager::invokeCommand(const std::string &command,
                                 const Being *const target)
{
    if (chatWindow == nullptr)
        return;
    chatWindow->localChatInput(parseCommand(command, target));
}

std::string SpellManager::parseCommand(std::string command,
                                       const Being *const target)
{
    if (localPlayer == nullptr)
        return command;

    std::string name;
    std::string id;
    std::string name2;

    if (target != nullptr)
    {
        name = target->getName();
        name2 = name;
        id = toString(toInt(target->getId(), int));
    }
    else
    {
        name2 = localPlayer->getName();
    }

    bool found = false;

    size_t idx = command.find("<TARGET>");
    if (idx != std::string::npos)
    {
        found = true;
        command = replaceAll(command, "<TARGET>", name);
    }
    idx = command.find("<TARGETID>");
    if (idx != std::string::npos)
    {
        found = true;
        command = replaceAll(command, "<TARGETID>", id);
    }
    idx = command.find("<TARGETORSELF>");
    if (idx != std::string::npos)
    {
        found = true;
        command = replaceAll(command, "<TARGETORSELF>", name2);
    }

    if (!found && !name.empty())
        command.append(" ").append(name);

    return command;
}

TextCommand *SpellManager::createNewSpell() const
{
    return new TextCommand(CAST_U32(mSpellsVector.size()));
}

void SpellManager::load()
{
    const Configuration *cfg = &serverConfig;

    delete_all(mSpells);
    mSpells.clear();
    mSpellsVector.clear();

    if (cfg->getValue("commandShortcutFlags0", "").empty())
    {
        fillSpells();
        save();
        return;
    }

    for (unsigned i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
    {
        unsigned int targetType;
        unsigned int basicLvl;
        unsigned int school;
        unsigned int schoolLvl;
        unsigned int mana;
        unsigned int commandType;

        std::string flags =
            cfg->getValue("commandShortcutFlags" + toString(i), "");
        std::stringstream ss(flags);
        ss >> commandType;
        ss >> targetType;
        ss >> basicLvl;
        ss >> school;
        ss >> schoolLvl;
        ss >> mana;

        std::string cmd = cfg->getValue("commandShortcutCmd"
                                        + toString(i), "");
        std::string comment = cfg->getValue("commandShortcutComment"
                                        + toString(i), "");
        std::string symbol = cfg->getValue("commandShortcutSymbol"
                                           + toString(i), "");
        std::string icon = cfg->getValue("commandShortcutIcon"
                                         + toString(i), "");

#ifdef TMWA_SUPPORT
        if (static_cast<TextCommandTypeT>(commandType) ==
            TextCommandType::Magic)
        {
            addSpell(new TextCommand(i, symbol, cmd, comment,
                static_cast<CommandTargetT>(targetType), icon, basicLvl,
                static_cast<MagicSchoolT>(school), schoolLvl, mana));
        }
        else
#endif  // TMWA_SUPPORT
        {
            addSpell(new TextCommand(i, symbol, cmd, comment,
                static_cast<CommandTargetT>(targetType), icon));
        }
    }
}

#define setOrDel(str, method) \
    const std::string var##method = spell->method(); \
    if (!var##method.empty()) \
        serverConfig.setValue((str) + toString(i), var##method); \
    else \
        serverConfig.deleteKey((str) + toString(i))

void SpellManager::save() const
{
    for (unsigned i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
    {
        const TextCommand *const spell = mSpellsVector[i];
        if (spell != nullptr)
        {
            setOrDel("commandShortcutCmd", getCommand);
            setOrDel("commandShortcutComment", getComment);
            setOrDel("commandShortcutSymbol", getSymbol);
            setOrDel("commandShortcutIcon", getIcon);
            if (!spell->getCommand().empty() &&
                !spell->getSymbol().empty())
            {
#ifdef TMWA_SUPPORT
                serverConfig.setValue("commandShortcutFlags" + toString(i),
                    strprintf("%u %u %u %u %u %u",
                    CAST_U32(spell->getCommandType()),
                    CAST_U32(spell->getTargetType()),
                    spell->getBaseLvl(),
                    CAST_U32(spell->getSchool()),
                    spell->getSchoolLvl(),
                    CAST_U32(spell->getMana())));
#else  // TMWA_SUPPORT

                serverConfig.setValue("commandShortcutFlags" + toString(i),
                    strprintf("%u %u %u %u %u %u", 1U,
                    CAST_U32(spell->getTargetType()),
                    0U,
                    0U,
                    0U,
                    0U));
#endif  // TMWA_SUPPORT
            }
            else
            {
                serverConfig.deleteKey("commandShortcutFlags" + toString(i));
            }
        }
    }
}

#undef setOrDel

std::string SpellManager::autoComplete(const std::string &partName) const
{
    STD_VECTOR<TextCommand*>::const_iterator i = mSpellsVector.begin();
    const STD_VECTOR<TextCommand*>::const_iterator
        i_end = mSpellsVector.end();
    std::string newName;
    const TextCommand *newCommand = nullptr;

    while (i != i_end)
    {
        const TextCommand *const cmd = *i;
        const std::string line = cmd->getCommand();

        if (!line.empty())
        {
            const size_t pos = line.find(partName, 0);
            if (pos == 0)
            {
                if (!newName.empty())
                {
                    newName = findSameSubstring(line, newName);
                    newCommand = nullptr;
                }
                else
                {
                    newName = line;
                    newCommand = cmd;
                }
            }
        }
        ++i;
    }
    if (!newName.empty() &&
        (newCommand != nullptr) &&
        newCommand->getTargetType() == CommandTarget::NeedTarget)
    {
        return newName.append(" ");
    }
    return newName;
}

void SpellManager::swap(const int id1, const int id2)
{
    TextCommand *const spell1 = mSpells[id1];
    TextCommand *const spell2 = mSpells[id2];
    if ((spell1 == nullptr) || (spell2 == nullptr))
        return;

    // swap in map
    mSpells[id1] = spell2;
    mSpells[id2] = spell1;

    // swap id
    const int tmp = spell1->getId();
    spell1->setId(spell2->getId());
    spell2->setId(tmp);

    // swap in vector
    const size_t sz = SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS;
    for (size_t f = 0; f < sz; f++)
    {
        const TextCommand *const spellA = mSpellsVector[f];
        if (spellA == spell1)
        {
            for (size_t d = 0; d < sz; d++)
            {
                const TextCommand *const spellB = mSpellsVector[d];
                if (spellB == spell2)
                {
                    mSpellsVector[f] = spell2;
                    mSpellsVector[d] = spell1;
                    return;
                }
            }
        }
    }
}
