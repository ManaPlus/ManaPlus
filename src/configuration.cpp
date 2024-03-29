/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "configuration.h"

#include "variabledata.h"

#include "fs/files.h"
#include "fs/paths.h"

#include "fs/virtfs/fs.h"

#include "listeners/configlistener.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/foreach.h"
#ifdef DEBUG_CONFIG
#include "utils/stringmap.h"
#endif  // DEBUG_CONFIG

#include "debug.h"

#ifdef DEBUG_CONFIG
StringIntMap optionsCount;
#define GETLOG() if (logger) {logger->log("config get: " + key); \
    if (mIsMain) optionsCount[key] = 1; }
#else  // DEBUG_CONFIG
#define GETLOG()
#endif  // DEBUG_CONFIG

Configuration config;              // XML file configuration reader
Configuration serverConfig;        // XML file server configuration reader
Configuration features;            // XML file features
Configuration branding;            // XML branding information reader
Configuration paths;               // XML default paths information reader

const std::string unusedKeys[] =
{
    "BotCheckerWindowSticky",
    "afkmessage",
    "BotCheckerWindowVisible",
    "BotCheckerWindowWinX",
    "BotCheckerWindowWinY",
    "hideShield",
    "AttackRange",
    "emoteshortcut0",
    "screenshotDirectory2",
    "AttackRangeBorder",
    "AttackRangeBorderDelay",
    "AttackRangeBorderGradient",
    "AttackRangeDelay",
    "AttackRangeGradient",
    "Being",
    "BeingDelay",
    "BeingGradient",
    "BeingPopupSkin",
    "BotCheckerWindowSkin",
    "BuySellSkin",
    "BuySkin",
    "ChatSkin",
    "CollisionHighlight",
    "CollisionHighlightDelay",
    "CollisionHighlightGradient",
    "ColorCross",
    "ColorCrossDelay",
    "ColorCrossGradient",
    "ColorExperience",
    "ColorExperienceGradient",
    "ColorPickup",
    "ColorPickupGradient",
    "DebugSkin",
    "DropShortcutSkin",
    "EmoteShortcutSkin",
    "EquipmentSkin",
    "ExpInfo",
    "ExpInfoDelay",
    "ExpInfoGradient",
    "Experience",
    "ExperienceGradient",
    "GM",
    "GMDelay",
    "GMGradient",
    "Guild",
    "GuildDelay",
    "GuildGradient",
    "GuildSkin",
    "HelpSkin",
    "Hit CriticalDelay",
    "Hit CriticalGradient",
    "Hit Monster Player",
    "Hit Monster PlayerGradient",
    "Hit Player Monster",
    "Hit Player MonsterGradient",
    "HitCriticalDelay",
    "HitCriticalGradient",
    "HitLocalPlayerCriticalDelay",
    "HitLocalPlayerCriticalGradient",
    "HitLocalPlayerMiss",
    "HitLocalPlayerMissDelay",
    "HitLocalPlayerMissGradient",
    "HitLocalPlayerMonster",
    "HitLocalPlayerMonsterDelay",
    "HitLocalPlayerMonsterGradient",
    "HitMonsterPlayer",
    "HitMonsterPlayerDelay",
    "HitMonsterPlayerGradient",
    "HitPlayerMonster",
    "HitPlayerMonsterDelay",
    "HitPlayerMonsterGradient",
    "HomePlace",
    "HomePlaceBorder",
    "HomePlaceBorderDelay",
    "HomePlaceBorderGradient",
    "HomePlaceDelay",
    "HomePlaceGradient",
    "InventorySkin",
    "ItemPopupSkin",
    "ItemShortcutSkin",
    "Kill statsSkin",
    "MiniStatusSkin",
    "MinimapSkin",
    "Miss",
    "MissDelay",
    "MissGradient",
    "Monster",
    "MonsterAttackRange",
    "MonsterAttackRangeDelay",
    "MonsterAttackRangeGradient",
    "MonsterDelay",
    "MonsterGradient",
    "NPC",
    "NPCDelay",
    "NPCGradient",
    "NpcTextSkin",
    "OutfitsSkin",
    "Particle",
    "ParticleDelay",
    "ParticleGradient",
    "PartyDelay",
    "PartyGradient",
    "PartySkin",
    "Personal ShopSkin",
    "Pickup",
    "PickupGradient",
    "Player",
    "PlayerDelay",
    "PlayerGradient",
    "PopupMenuSkin",
    "PortalHighlight",
    "PortalHighlightDelay",
    "PortalHighlightGradient",
    "RecorderSkin",
    "RecorderWinX",
    "RecorderWinY",
    "RoadPoint",
    "RoadPointDelay",
    "RoadPointGradient",
    "Self",
    "SelfDelay",
    "SelfGradient",
    "SellSkin",
    "ServerDialogSkin",
    "ShopSkin",
    "SkillsSkin",
    "SocialCreatePopupSkin",
    "SocialSkin",
    "SpecialsSkin",
    "SpeechSkin",
    "SpellPopupSkin",
    "SpellShortcutSkin",
    "StatusPopupSkin",
    "StatusSkin",
    "StorageSkin",
    "TextCommandEditorSkin",
    "TextPopupSkin",
    "TradeSkin",
    "WhoIsOnlineSkin",
    "emoteshortcut1",
    "emoteshortcut2",
    "emoteshortcut3",
    "emoteshortcut4",
    "emoteshortcut5",
    "emoteshortcut6",
    "emoteshortcut7",
    "emoteshortcut8",
    "emoteshortcut9",
    "emoteshortcut10",
    "emoteshortcut11",
    "emoteshortcut12",
    "emoteshortcut13",
    "fastOpenGL",
    "keyAutoCompleteChat",
    "keyDeActivateChat",
    "keyTargetClosest",
    "keyWindowParty",
    "mapalpha",
    "port",
    "shopBuyList",
    "shopSellList",
    "OutfitAwayIndex",
    "playerHomes",
    "remember",
    "screenshotDirectory",
    ""
};

void ConfigurationObject::setValue(const std::string &key,
                                   const std::string &value)
{
    mOptions[key] = value;
}

void ConfigurationObject::deleteKey(const std::string &key)
{
    mOptions.erase(key);
}

void Configuration::setValue(const std::string &key,
                             const std::string &value)
{
    ConfigurationObject::setValue(key, value);
    mUpdated = true;

    // Notify listeners
    const ListenerMapIterator list = mListenerMap.find(key);
    if (list != mListenerMap.end())
    {
        Listeners listeners = list->second;
        FOR_EACH (ListenerIterator, i, listeners)
            (*i)->optionChanged(key);
    }
}

void Configuration::incValue(const std::string &key)
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    setValue(key, (iter != mOptions.end())
        ? atoi(iter->second.c_str()) + 1 : 1);
}

void Configuration::setSilent(const std::string &key,
                              const std::string &value)
{
    ConfigurationObject::setValue(key, value);
}

std::string ConfigurationObject::getValue(const std::string &key,
                                          const std::string &deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    return ((iter != mOptions.end()) ? iter->second : deflt);
}

int ConfigurationObject::getValue(const std::string &key,
                                  const int deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atoi(iter->second.c_str()) : deflt;
}

int ConfigurationObject::getValueInt(const std::string &key,
                                     const int deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atoi(iter->second.c_str()) : deflt;
}

bool ConfigurationObject::getValueBool(const std::string &key,
                                       const bool deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    if (iter != mOptions.end())
        return atoi(iter->second.c_str()) != 0 ? true : false;
    return deflt;
}

unsigned ConfigurationObject::getValue(const std::string &key,
                                       const unsigned deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? CAST_U32(
            atol(iter->second.c_str())) : deflt;
}

double ConfigurationObject::getValue(const std::string &key,
                                     const double deflt) const
{
    GETLOG();
    const Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atof(iter->second.c_str()) : deflt;
}

void ConfigurationObject::deleteList(const std::string &name)
{
    for (ConfigurationList::const_iterator
         it = mContainerOptions[name].begin();
         it != mContainerOptions[name].end(); ++it)
    {
        delete *it;
    }

    mContainerOptions[name].clear();
}

void ConfigurationObject::clear()
{
    for (std::map<std::string, ConfigurationList>::const_iterator
         it = mContainerOptions.begin();
         it != mContainerOptions.end(); ++it)
    {
        deleteList(it->first);
    }
    mOptions.clear();
    mContainerOptions.clear();
}

ConfigurationObject::ConfigurationObject() :
    mOptions(),
#ifdef DEBUG_CONFIG
    mContainerOptions(),
    mLogKeys(false),
    mIsMain(false)
#else  // DEBUG_CONFIG

    mContainerOptions()
#endif  // DEBUG_CONFIG
{
}

ConfigurationObject::~ConfigurationObject()
{
    clear();
}

Configuration::Configuration() :
    ConfigurationObject(),
    mListenerMap(),
    mConfigPath(),
    mDefaultsData(),
    mDirectory(),
    mFilename(),
    mUseResManager(UseVirtFs_false),
    mUpdated(false)
{
#ifdef DEBUG_CONFIG
    mLogKeys = false;
    mIsMain = false;
#endif  // DEBUG_CONFIG
}

void Configuration::cleanDefaults()
{
    for (DefaultsData::const_iterator iter = mDefaultsData.begin();
         iter != mDefaultsData.end();
         ++iter)
    {
        delete iter->second;
    }
    mDefaultsData.clear();
}

Configuration::~Configuration()
{
    cleanDefaults();
}

void Configuration::unload()
{
    cleanDefaults();
    mConfigPath.clear();
    mDirectory.clear();
    mFilename.clear();
    mUseResManager = UseVirtFs_false;
    ConfigurationObject::clear();
}

int Configuration::getIntValue(const std::string &key) const
{
    GETLOG();
    int defaultValue = 0;
    const Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        const DefaultsData::const_iterator itdef
            = mDefaultsData.find(key);

        if (itdef != mDefaultsData.end() && (itdef->second != nullptr))
        {
            const VariableData *const data = itdef->second;
            const VariableData::DataType type = static_cast<
                VariableData::DataType>(data->getType());
            if (type == VariableData::DATA_INT)
            {
                defaultValue = (static_cast<const IntData*>(
                    data))->getData();
            }
            else if (type == VariableData::DATA_STRING)
            {
                defaultValue = atoi((static_cast<const StringData*>(
                    data))->getData().c_str());
            }
            else if (type == VariableData::DATA_BOOL)
            {
                if ((static_cast<const BoolData*>(data))->getData())
                    defaultValue = 1;
                else
                    defaultValue = 0;
            }
            else if (type == VariableData::DATA_FLOAT)
            {
                defaultValue = CAST_S32(
                    (static_cast<const FloatData*>(data))->getData());
            }
        }
        else
        {
            reportAlways(
                "%s: No integer value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }
    else
    {
        defaultValue = atoi(iter->second.c_str());
    }
    return defaultValue;
}

int Configuration::resetIntValue(const std::string &key)
{
    GETLOG();
    int defaultValue = 0;
    const DefaultsData::const_iterator itdef = mDefaultsData.find(key);
    if (itdef == mDefaultsData.end())
    {
        reportAlways("%s: No integer value in registry for key %s",
            mConfigPath.c_str(),
            key.c_str())
    }
    else
    {
        const VariableData *const data = itdef->second;
        if (data != nullptr &&
            data->getType() == VariableData::DATA_INT)
        {
            defaultValue = (static_cast<const IntData*>(
                data))->getData();
        }
        else
        {
            reportAlways("%s: No integer value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }
    setValue(key, defaultValue);
    return defaultValue;
}

std::string Configuration::getStringValue(const std::string &key) const
{
    GETLOG();
    std::string defaultValue;
    const Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        const DefaultsData::const_iterator
            itdef = mDefaultsData.find(key);

        if (itdef != mDefaultsData.end() &&
            (itdef->second != nullptr))
        {
            const VariableData *const data = itdef->second;
            const VariableData::DataType type = static_cast<
                VariableData::DataType>(data->getType());
            if (type == VariableData::DATA_STRING)
            {
                defaultValue = (static_cast<const StringData*>(
                    data))->getData();
            }
            else if (type == VariableData::DATA_BOOL)
            {
                if ((static_cast<const BoolData*>(data))->getData())
                    defaultValue = "1";
                else
                    defaultValue = "0";
            }
            else if (type == VariableData::DATA_INT)
            {
                defaultValue = toString((static_cast<const IntData*>(
                    data))->getData());
            }
            else if (type == VariableData::DATA_FLOAT)
            {
                defaultValue = toString((static_cast<const FloatData*>(
                    data))->getData());
            }
        }
        else
        {
            reportAlways("%s: No string value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }
    else
    {
        defaultValue = iter->second;
    }
    return defaultValue;
}


float Configuration::getFloatValue(const std::string &key) const
{
    GETLOG();
    float defaultValue = 0.0F;
    const Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        const DefaultsData::const_iterator itdef
            = mDefaultsData.find(key);

        if (itdef != mDefaultsData.end() &&
            (itdef->second != nullptr))
        {
            const VariableData *const data = itdef->second;
            const VariableData::DataType type = static_cast<
                VariableData::DataType>(data->getType());
            if (type == VariableData::DATA_FLOAT)
            {
                defaultValue = static_cast<float>(
                    (static_cast<const FloatData*>(data))->getData());
            }
            else if (type == VariableData::DATA_STRING)
            {
                defaultValue = static_cast<float>(atof((
                    static_cast<const StringData*>(
                    data))->getData().c_str()));
            }
            else if (type == VariableData::DATA_BOOL)
            {
                if ((static_cast<const BoolData*>(data))->getData())
                    defaultValue = 1;
                else
                    defaultValue = 0;
            }
            else if (type == VariableData::DATA_INT)
            {
                defaultValue = static_cast<float>((
                    static_cast<const IntData*>(
                    data))->getData());
            }
        }
        else
        {
            reportAlways("%s: No float value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }
    else
    {
        defaultValue = static_cast<float>(atof(iter->second.c_str()));
    }
    return defaultValue;
}

bool Configuration::getBoolValue(const std::string &key) const
{
    GETLOG();
    bool defaultValue = false;
    const Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        const DefaultsData::const_iterator itdef
            = mDefaultsData.find(key);

        if (itdef != mDefaultsData.end() &&
            (itdef->second != nullptr))
        {
            const VariableData *const data = itdef->second;
            const VariableData::DataType type = static_cast<
                VariableData::DataType>(data->getType());
            if (type == VariableData::DATA_BOOL)
            {
                defaultValue = (static_cast<const BoolData*>(
                    data))->getData();
            }
            else if (type == VariableData::DATA_INT)
            {
                if ((static_cast<const IntData*>(data))->getData() != 0)
                    defaultValue = true;
                else
                    defaultValue = false;
            }
            else if (type == VariableData::DATA_STRING)
            {
                if ((static_cast<const StringData*>(
                    data))->getData() != "0")
                {
                    defaultValue = true;
                }
                else
                {
                    defaultValue = false;
                }
            }
            if (type == VariableData::DATA_FLOAT)
            {
                if (CAST_S32((static_cast<const FloatData*>(
                    data))->getData()) != 0)
                {
                    defaultValue = true;
                }
                else
                {
                    defaultValue = false;
                }
            }
        }
        else
        {
            reportAlways(
                "%s: No boolean value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }
    else
    {
        defaultValue = getBoolFromString(iter->second);
    }

    return defaultValue;
}

bool Configuration::resetBoolValue(const std::string &key)
{
    GETLOG();
    bool defaultValue = false;
    const DefaultsData::const_iterator itdef = mDefaultsData.find(key);

    if (itdef == mDefaultsData.end())
    {
        reportAlways("%s: No boolean value in registry for key %s",
            mConfigPath.c_str(),
            key.c_str())
    }
    else
    {
        const VariableData *const data = itdef->second;
        if (data != nullptr &&
            data->getType() == VariableData::DATA_BOOL)
        {
            defaultValue = (static_cast<const BoolData*>(data))->getData();
        }
        else
        {
            reportAlways("%s: No boolean value in registry for key %s",
                mConfigPath.c_str(),
                key.c_str())
        }
    }

    setValue(key, defaultValue);
    return defaultValue;
}


void ConfigurationObject::initFromXML(XmlNodeConstPtrConst parentNode)
{
    clear();

    if (parentNode == nullptr)
        return;

    for_each_xml_child_node(node, parentNode)
    {
        if (xmlNameEqual(node, "list"))
        {
            // list option handling
            const std::string name = XML::getProperty(node,
                "name", std::string());

            for_each_xml_child_node(subnode, node)
            {
                if (xmlNameEqual(subnode, name.c_str()) &&
                    xmlTypeEqual(subnode, XML_ELEMENT_NODE))
                {
                    ConfigurationObject *const cobj = new ConfigurationObject;
                    cobj->initFromXML(subnode);  // recurse
                    mContainerOptions[name].push_back(cobj);
                }
            }
        }
        else if (xmlNameEqual(node, "option"))
        {
            // single option handling
            const std::string name = XML::getProperty(node,
                "name", std::string());
            if (!name.empty())
            {
                mOptions[name] = XML::getProperty(node,
                    "value", std::string());
            }
        }  // otherwise ignore
    }
}

void Configuration::init(const std::string &filename,
                         const UseVirtFs useResManager,
                         const SkipError skipError)
{
    cleanDefaults();
    clear();
    mFilename = filename;
    mUseResManager = useResManager;

    if (useResManager == UseVirtFs_true)
    {
        mConfigPath = "virtfs://" + filename;
        mDirectory.clear();
        if (VirtFs::exists(filename) == false)
        {
            logger->log("Warning: No configuration file (%s)",
                filename.c_str());
            return;
        }
    }
    else
    {
        mConfigPath = filename;
        logger->log1("init 1");
        mDirectory = getRealPath(getFileDir(filename));
        if (Files::existsLocal(filename) == false)
        {
            logger->log("Warning: No configuration file (%s)",
                filename.c_str());
            return;
        }
    }

    XML::Document doc(filename,
        useResManager,
        skipError);
    logger->log1("init 2");
    if (doc.rootNode() == nullptr)
    {
        logger->log("Couldn't open configuration file: %s", filename.c_str());
        return;
    }

    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "configuration"))
    {
        logger->log("Warning: No configuration file (%s)", filename.c_str());
        return;
    }

    initFromXML(rootNode);
}

void Configuration::reInit()
{
    XML::Document doc(mFilename, mUseResManager, SkipError_false);
    if (doc.rootNode() == nullptr)
    {
        logger->log("Couldn't open configuration file: %s", mFilename.c_str());
        return;
    }

    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "configuration"))
    {
        logger->log("Warning: No configuration file (%s)", mFilename.c_str());
        return;
    }

    initFromXML(rootNode);
}

void ConfigurationObject::writeToXML(XmlTextWriterPtr writer)
{
    FOR_EACH (Options::const_iterator, i, mOptions)
    {
#ifdef DEBUG_CONFIG
        if (mLogKeys)
        {
            if (optionsCount.find(i->first) == optionsCount.end())
                logger->log("unused configuration option: " + i->first);
        }
#endif  // DEBUG_CONFIG

        XmlTextWriterStartElement(writer, "option");
        XmlTextWriterWriteAttribute(writer, "name", i->first.c_str());
        XmlTextWriterWriteAttribute(writer, "value", i->second.c_str());
        XmlTextWriterEndElement(writer);
    }

    for (std::map<std::string, ConfigurationList>::const_iterator
         it = mContainerOptions.begin(), it_fend = mContainerOptions.end();
         it != it_fend; ++ it)
    {
        const char *const name = it->first.c_str();

        XmlTextWriterStartElement(writer, "list");
        XmlTextWriterWriteAttribute(writer, "name", name);

        // recurse on all elements
        FOR_EACH (ConfigurationList::const_iterator, elt_it, it->second)
        {
            XmlTextWriterStartElement(writer, name);
            if (*elt_it != nullptr)
                (*elt_it)->writeToXML(writer);
            XmlTextWriterEndElement(writer);
        }

        XmlTextWriterEndElement(writer);
    }
}

void Configuration::writeUpdated()
{
    if (mUpdated)
        write();
}

void Configuration::write()
{
    BLOCK_START("Configuration::write")
    if (mConfigPath.empty())
    {
        BLOCK_END("Configuration::write")
        return;
    }

    mUpdated = false;
    // Do not attempt to write to file that cannot be opened for writing
    FILE *const testFile = fopen(mConfigPath.c_str(), "w");
    if (testFile == nullptr)
    {
        reportAlways("Configuration::write() couldn't open %s for writing",
            mConfigPath.c_str())
        BLOCK_END("Configuration::write")
        return;
    }
    fclose(testFile);

    XmlTextWriterPtr writer = XmlNewTextWriterFilename(
        mConfigPath.c_str(), 0);

    if (writer == nullptr)
    {
        logger->log1("Configuration::write() error while creating writer");
        BLOCK_END("Configuration::write")
        return;
    }

    logger->log1("Configuration::write() writing configuration...");

    XmlTextWriterSetIndent(writer, 1);
    XmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
//    xmlTextWriterStartDocument(writer, nullptr, "utf8", nullptr);
    XmlTextWriterStartRootElement(writer, "configuration");

    writeToXML(writer);

    XmlTextWriterEndDocument(writer);
    XmlSaveTextWriterFilename(writer,
        mConfigPath.c_str());
    XmlFreeTextWriter(writer);
    BLOCK_END("Configuration::write")
}

void Configuration::addListener(const std::string &key,
                                ConfigListener *const listener)
{
    mListenerMap[key].push_front(listener);
}

void Configuration::removeListener(const std::string &key,
                                   ConfigListener *const listener)
{
    mListenerMap[key].remove(listener);
}

#ifdef ENABLE_CHECKS
void Configuration::checkListeners(ConfigListener *const listener,
                                   const char *const file,
                                   const unsigned line)
{
    FOR_EACH (ListenerMapIterator, it, mListenerMap)
    {
        Listeners listeners = it->second;
        FOR_EACH (ListenerIterator, it2, listeners)
        {
            if (*it2 == listener)
            {
                logger->log("detected not cleaned listener: %p, %s:%u",
                    static_cast<void*>(listener), file, line);
                exit(1);
            }
        }
    }
}
#endif  // ENABLE_CHECKS

void Configuration::removeListeners(ConfigListener *const listener)
{
    FOR_EACH (ListenerMapIterator, it, mListenerMap)
        (it->second).remove(listener);
}

void Configuration::removeOldKeys()
{
    if (mOptions.find(unusedKeys[0]) != mOptions.end() ||
        mOptions.find(unusedKeys[1]) != mOptions.end() ||
        mOptions.find(unusedKeys[2]) != mOptions.end())
    {
        int f = 0;
        while (!unusedKeys[f].empty())
        {
            deleteKey(unusedKeys[f]);
            logger->log("remove unused key: " + unusedKeys[f]);
            f ++;
        }
        for (f = 0; f < 80; f ++)
        {
            const std::string str = toString(f);
            deleteKey("Outfit" + str);
            deleteKey("OutfitUnequip" + str);
            deleteKey("commandShortcutCmd" + str);
            deleteKey("commandShortcutFlags" + str);
            deleteKey("commandShortcutSymbol" + str);
            deleteKey("drop" + str);
            deleteKey("shortcut" + str);
        }
    }
}
