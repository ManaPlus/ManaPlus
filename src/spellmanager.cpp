/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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

#include "being.h"
#include "configuration.h"
#include "logger.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/chatwindow.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/dtor.h"
#include "utils/stringutils.h"

#include "debug.h"

SpellManager::SpellManager()
{
    load();
}

SpellManager::~SpellManager()
{
    delete_all(mSpells);
    mSpells.clear();
    mSpellsVector.clear();
}

TextCommand* SpellManager::getSpell(int spellId)
{
    if (spellId < 0 || static_cast<unsigned int>(spellId) >= mSpells.size())
        return nullptr;

    return mSpells[spellId];
}

TextCommand* SpellManager::getSpellByItem(int itemId)
{
    return getSpell(itemId - SPELL_MIN_ID);
}

void SpellManager::fillSpells()
{
//id, std::string name, std::string symbol, ST type, int basicLvl,
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
    for (int f = 12; f < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; f++)
        addSpell(new TextCommand(f));
}

bool SpellManager::addSpell(TextCommand *spell)
{
    if (!spell)
        return false;

    std::map<unsigned int, TextCommand*>::const_iterator
        i = mSpells.find(spell->getId());
    if (i == mSpells.end())
    {
        mSpells[spell->getId()] = spell;
        mSpellsVector.push_back(spell);
        return true;
    }
    return false;
}

std::vector<TextCommand*> SpellManager::getAll()
{
    //logger->log(("mSpellsVector = " + toString(mSpellsVector.size())).c_str());
    return mSpellsVector;
}

void SpellManager::useItem(int itemId)
{
    invoke(itemId - SPELL_MIN_ID);
}

void SpellManager::invoke(int spellId)
{
    if (!player_node)
        return;

    TextCommand* spell = getSpell(spellId);
    if (!spell)
        return;

    if (spell->getCommand() == "")
        return;

    if (spell->getCommandType() == TEXT_COMMAND_TEXT
        || (Net::getPlayerHandler()->canUseMagic()
        && PlayerInfo::getStatEffective(SKILL_MAGIC) >= spell->getBaseLvl()
        && PlayerInfo::getStatEffective(
        spell->getSchool()) >= spell->getSchoolLvl()
        && PlayerInfo::getAttribute(MP) >= spell->getMana()))
    {
        Being* target = player_node->getTarget();
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

void SpellManager::invokeSpell(TextCommand* spell) const
{
    if (!chatWindow || !spell)
        return;
    chatWindow->localChatInput(parseCommand(spell->getCommand(), nullptr));
}

void SpellManager::invokeSpell(TextCommand* spell, Being* target) const
{
    if (!chatWindow || !spell || !target)
        return;
    chatWindow->localChatInput(parseCommand(spell->getCommand(), target));
}

std::string SpellManager::parseCommand(std::string command,
                                       Being *target) const
{
    if (!player_node)
        return command;

    std::string name("");
    std::string id("");
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

    int idx = static_cast<int>(command.find("<TARGET>"));
    if (idx >= 0)
    {
        found = true;
        command = replaceAll(command, "<TARGET>", name);
    }
    idx = static_cast<int>(command.find("<TARGETID>"));
    if (idx >= 0)
    {
        found = true;
        command = replaceAll(command, "<TARGETID>", id);
    }
    idx = static_cast<int>(command.find("<TARGETORSELF>"));
    if (idx >= 0)
    {
        found = true;
        command = replaceAll(command, "<TARGETORSELF>", name2);
    }

    if (!found && !name.empty())
        command += " " + name;

    return command;
}

TextCommand *SpellManager::createNewSpell()
{
    return new TextCommand(static_cast<unsigned>(mSpellsVector.size()));
}

void SpellManager::load(bool oldConfig)
{
    Configuration *cfg;
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

    for (int i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
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

void SpellManager::save()
{
    for (int i = 0; i < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; i++)
    {
        TextCommand *spell = mSpellsVector[i];
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
                    strprintf("%u %u %u %u %u %u", spell->getCommandType(),
                    spell->getTargetType(), spell->getBaseLvl(),
                    spell->getSchool(), spell->getSchoolLvl(),
                    spell->getMana()));
            }
            else
            {
                serverConfig.deleteKey("commandShortcutFlags" + toString(i));
            }
        }
    }
}

std::string SpellManager::autoComplete(std::string partName)
{
    std::vector<TextCommand*>::const_iterator i = mSpellsVector.begin();
    std::string newName = "";
    TextCommand *newCommand = nullptr;

    while (i != mSpellsVector.end())
    {
        TextCommand *cmd = *i;
        std::string line = cmd->getCommand();

        if (line != "")
        {
            std::string::size_type pos = line.find(partName, 0);
            if (pos == 0)
            {
                if (newName != "")
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
    if (newName != "" && newCommand
        && newCommand->getTargetType() == NEEDTARGET)
    {
        return newName + " ";
    }
    return newName;
}
