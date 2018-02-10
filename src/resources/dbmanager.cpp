/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2018  The ManaPlus Developers
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

#include "resources/dbmanager.h"

#include "being/being.h"

#include "net/loginhandler.h"
#include "net/net.h"

#include "resources/db/avatardb.h"
#include "resources/db/badgesdb.h"
#include "resources/db/chardb.h"
#include "resources/db/clandb.h"
#include "resources/db/colordb.h"
#include "resources/db/deaddb.h"
#include "resources/db/elementaldb.h"
#include "resources/db/emotedb.h"
#include "resources/db/groupdb.h"
#include "resources/db/homunculusdb.h"
#include "resources/db/horsedb.h"
#include "resources/db/itemdb.h"
#include "resources/db/itemfielddb.h"
#include "resources/db/itemoptiondb.h"
#include "resources/db/languagedb.h"
#include "resources/db/sounddb.h"
#include "resources/db/mapdb.h"
#include "resources/db/mercenarydb.h"
#include "resources/db/moddb.h"
#include "resources/db/monsterdb.h"
#include "resources/db/networkdb.h"
#include "resources/db/npcdb.h"
#include "resources/db/npcdialogdb.h"
#include "resources/db/palettedb.h"
#include "resources/db/petdb.h"
#include "resources/db/skillunitdb.h"
#include "resources/db/statdb.h"
#include "resources/db/statuseffectdb.h"
#include "resources/db/textdb.h"
#include "resources/db/unitsdb.h"
#include "resources/db/weaponsdb.h"

#include "debug.h"

void DbManager::loadDb()
{
    CharDB::load();
    GroupDb::load();
    StatDb::load();
    DeadDB::load();
    PaletteDB::load();
    ColorDB::load();
    SoundDB::load();
    LanguageDb::load();
    TextDb::load();
    MapDB::load();
    ItemFieldDb::load();
    ItemOptionDb::load();
    ItemDB::load();
    Being::load();
    const ServerTypeT type = Net::getNetworkType();
    if (type == ServerType::EATHENA ||
        type == ServerType::EVOL2)
    {
        NetworkDb::load();
        if (loginHandler != nullptr)
            loginHandler->updatePacketVersion();
        MercenaryDB::load();
        HomunculusDB::load();
        ElementalDb::load();
        SkillUnitDb::load();
        HorseDB::load();
        ClanDb::load();
    }
    MonsterDB::load();
    AvatarDB::load();
    BadgesDB::load();
    WeaponsDB::load();
    UnitsDb::load();
    NPCDB::load();
    NpcDialogDB::load();
    PETDB::load();
    EmoteDB::load();
//    ModDB::load();
    StatusEffectDB::load();
}

void DbManager::unloadDb()
{
    CharDB::unload();
    GroupDb::unload();
    StatDb::unload();
    DeadDB::unload();
    ColorDB::unload();
    SoundDB::unload();
    LanguageDb::unload();
    TextDb::unload();
    EmoteDB::unload();
    ItemDB::unload();
    ItemOptionDb::unload();
    ItemFieldDb::unload();
    const ServerTypeT type = Net::getNetworkType();
    if (type == ServerType::EATHENA ||
        type == ServerType::EVOL2)
    {
        MercenaryDB::unload();
        HomunculusDB::unload();
        ClanDb::unload();
        ElementalDb::unload();
        SkillUnitDb::unload();
        HorseDB::unload();
        NetworkDb::unload();
    }
    MonsterDB::unload();
    NPCDB::unload();
    NpcDialogDB::unload();
    AvatarDB::unload();
    BadgesDB::unload();
    WeaponsDB::unload();
    UnitsDb::unload();
    PaletteDB::unload();
    PETDB::unload();
    StatusEffectDB::unload();
    ModDB::unload();
}
