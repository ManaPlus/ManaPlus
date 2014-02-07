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

#include "spellmanager.h"

#include "configuration.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/dtor.h"

#include "debug.h"

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
    if (spellId < 0 || static_cast<unsigned int>(spellId) >= mSpells.size())
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
// id, std::string name, std::string symbol, ST type, int basicLvl,
//    MagicSchool school, int schoolLvl, int mana)

    addSpell(new TextCommand(0, "lum", "#lum", "heal with lifestones",
             ALLOWTARGET, "", 1, SKILL_MAGIC_LIFE, 0, 6));
    addSpell(new TextCommand(1, "inm", "#inma", "heal", NEEDTARGET,
             "", 2, SKILL_MAGIC_LIFE, 2, 10));
    addSpell(new TextCommand(2, "fla", "#flar", "", NOTARGET,
             "", 1, SKILL_MAGIC_WAR, 0, 10));
    addSpell(new TextCommand(3, "chi", "#chiza", "", NOTARGET,
             "", 1, SKILL_MAGIC_WAR, 0, 9));
    addSpell(new TextCommand(4, "ing", "#ingrav", "", NOTARGET,
             "", 2, SKILL_MAGIC_WAR, 2, 20));
    addSpell(new TextCommand(5, "fri", "#frillyar", "", NOTARGET,
             "", 2, SKILL_MAGIC_WAR, 2, 25));
    addSpell(new TextCommand(6, "upm", "#upmarmu", "", NOTARGET,
             "", 2, SKILL_MAGIC_WAR, 2, 20));
    addSpell(new TextCommand(7, "ite", "#itenplz", "", NOTARGET,
             "", 1, SKILL_MAGIC_NATURE, 0, 3));
    addSpell(new TextCommand(8, "bet", "#betsanc", "", ALLOWTARGET,
             "", 2, SKILL_MAGIC_NATURE, 2, 14));
    addSpell(new TextCommand(9, "abi", "#abizit", "", NOTARGET,
             "", 1, SKILL_MAGIC, 0, 1));
    addSpell(new TextCommand(10, "inw", "#inwilt", "", NOTARGET,
             "", 2, SKILL_MAGIC, 2, 7));
    addSpell(new TextCommand(11, "hi", "hi", "", NOTARGET, ""));
    addSpell(new TextCommand(12, "hea", "heal", "", NOTARGET, ""));
    addSpell(new TextCommand(13, "@sp", "@spawn maggot 10", "", NOTARGET, ""));
    for (unsigned f = 12; f < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; f++)
        addSpell(new TextCommand(f));
}

bool SpellManager::addSpell(TextCommand *const spell)
{
    if (!spell)
        return false;

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

const std::vector<TextCommand*> &SpellManager::getAll() const
{
    return mSpellsVector;
}

void SpellManager::useItem(const int itemId) const
{
    invoke(itemId - SPELL_MIN_ID);
}

void SpellManager::invoke(const int spellId) const
{
    if (!player_node)
        return;

    const TextCommand *const spell = getSpell(spellId);
    if (!spell)
        return;

    if (spell->getCommand() == "")
        return;

    if (spell->getCommandType() == TEXT_COMMAND_TEXT
        || (Net::getPlayerHandler()->canUseMagic()
        && PlayerInfo::getSkillLevel(SKILL_MAGIC)
        >= static_cast<signed>(spell->getBaseLvl())
        && PlayerInfo::getSkillLevel(
        spell->getSchool()) >= static_cast<signed>(spell->getSchoolLvl())
        && PlayerInfo::getAttribute(PlayerInfo::MP) >= spell->getMana()))
    {
        const Being *const target = player_node->getTarget();
        if (spell->getTargetType() == NOTARGET)
        {
            invokeSpell(spell);
        }
        if ((target && (target->getType() != Being::MONSTER
            || spell->getCommandType() == TEXT_COMMAND_TEXT))
            && (spell->getTargetType() == ALLOWTARGET
            || spell->getTargetType() == NEEDTARGET))
        {
            invokeSpell(spell, target);
        }
        else if (spell->getTargetType() == ALLOWTARGET)
        {
            invokeSpell(spell);
        }
    }
}

void SpellManager::invokeSpell(const TextCommand *const spell)
{
    if (!chatWindow || !spell)
        return;
    chatWindow->localChatInput(parseCommand(spell->getCommand(), nullptr));
}

void SpellManager::invokeSpell(const TextCommand *const spell,
                               const Being *const target)
{
    if (!chatWindow || !spell || !target)
        return;
    chatWindow->localChatInput(parseCommand(spell->getCommand(), target));
}

std::string SpellManager::parseCommand(std::string command,
                                       const Being *const target)
{
    if (!player_node)
        return command;

    std::string name;
    std::string id;
    std::string name2;

    if (target)
    {
        name = target->getName();
        name2 = target->getName();
        id = toString(target->getId());
    }
    else
    {
        name2 = player_node->getName();
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
    return new TextCommand(static_cast<unsigned>(mSpellsVector.size()));
}

void SpellManager::load(const bool oldConfig)
{
    const Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    delete_all(mSpells);
    mSpells.clear();
    mSpellsVector.clear();

    if (cfg->getValue("commandShortcutFlags0", "") == "")
    {
        fillSpells();
        save();
        return;
    }

    unsigned int targetType;
    unsigned int basicLvl;
    unsigned int school;
    unsigned int schoolLvl;
    unsigned int mana;
    unsigned int commandType;

    for (unsigned i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
    {
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

        if (static_cast<TextCommandType>(commandType) == TEXT_COMMAND_MAGIC)
        {
            addSpell(new TextCommand(i, symbol, cmd, comment,
                static_cast<SpellTarget>(targetType), icon, basicLvl,
                static_cast<MagicSchool>(school), schoolLvl, mana));
        }
        else
        {
            addSpell(new TextCommand(i, symbol, cmd, comment,
                static_cast<SpellTarget>(targetType), icon));
        }
    }
}

void SpellManager::save() const
{
    for (unsigned i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
    {
        const TextCommand *const spell = mSpellsVector[i];
        if (spell)
        {
            if (spell->getCommand() != "")
            {
                serverConfig.setValue("commandShortcutCmd" + toString(i),
                                      spell->getCommand());
            }
            else
            {
                serverConfig.deleteKey("commandShortcutCmd" + toString(i));
            }
            if (spell->getComment() != "")
            {
                serverConfig.setValue("commandShortcutComment" + toString(i),
                                      spell->getComment());
            }
            else
            {
                serverConfig.deleteKey("commandShortcutComment" + toString(i));
            }
            if (spell->getSymbol() != "")
            {
                serverConfig.setValue("commandShortcutSymbol" + toString(i),
                                      spell->getSymbol());
            }
            else
            {
                serverConfig.deleteKey("commandShortcutSymbol" + toString(i));
            }
            if (spell->getIcon() != "")
            {
                serverConfig.setValue("commandShortcutIcon" + toString(i),
                                      spell->getIcon());
            }
            else
            {
                serverConfig.deleteKey("commandShortcutIcon" + toString(i));
            }
            if (spell->getCommand() != "" && spell->getSymbol() != "")
            {
                serverConfig.setValue("commandShortcutFlags" + toString(i),
                    strprintf("%u %u %u %u %u %u", static_cast<unsigned>(
                    spell->getCommandType()), static_cast<unsigned>(
                    spell->getTargetType()), spell->getBaseLvl(),
                    static_cast<unsigned>(spell->getSchool()),
                    spell->getSchoolLvl(), static_cast<unsigned>(
                    spell->getMana())));
            }
            else
            {
                serverConfig.deleteKey("commandShortcutFlags" + toString(i));
            }
        }
    }
}

std::string SpellManager::autoComplete(const std::string &partName) const
{
    std::vector<TextCommand*>::const_iterator i = mSpellsVector.begin();
    const std::vector<TextCommand*>::const_iterator
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
    if (!newName.empty() && newCommand
        && newCommand->getTargetType() == NEEDTARGET)
    {
        return newName.append(" ");
    }
    return newName;
}

void SpellManager::swap(const int id1, const int id2)
{
    TextCommand *const spell1 = mSpells[id1];
    TextCommand *const spell2 = mSpells[id2];
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
