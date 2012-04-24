/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "configlistener.h"
#include "logger.h"

#include "utils/paths.h"
#include "utils/stringutils.h"

#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG_CONFIG
std::map<std::string, int> optionsCount;
#define GETLOG() if (logger) {logger->log("config get: " + key); \
    if (mIsMain) optionsCount[key] = 1; }
#else
#define GETLOG()
#endif

const std::string unusedKeys[] =
{
    "AttackRange",
    "emoteshortcut0",
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

void Configuration::setValue(const std::string &key, const std::string &value)
{
    ConfigurationObject::setValue(key, value);

    // Notify listeners
    ListenerMapIterator list = mListenerMap.find(key);
    if (list != mListenerMap.end())
    {
        Listeners listeners = list->second;
        for (ListenerIterator i = listeners.begin(), i_end = listeners.end();
             i != i_end; ++i)
        {
            (*i)->optionChanged(key);
        }
    }
}

void Configuration::setSilent(const std::string &key, const std::string &value)
{
    ConfigurationObject::setValue(key, value);
}

std::string ConfigurationObject::getValue(const std::string &key,
                                          const std::string &deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
    return ((iter != mOptions.end()) ? iter->second : deflt);
}

int ConfigurationObject::getValue(const std::string &key, int deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atoi(iter->second.c_str()) : deflt;
}

int ConfigurationObject::getValueInt(const std::string &key, int deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atoi(iter->second.c_str()) : deflt;
}

bool ConfigurationObject::getValueBool(const std::string &key,
                                       bool deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
    if (iter != mOptions.end())
        return atoi(iter->second.c_str()) != 0 ? true : false;
    else
        return deflt;
}

unsigned ConfigurationObject::getValue(const std::string &key,
                                       unsigned deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? static_cast<unsigned>(
            atol(iter->second.c_str())) : deflt;
}

double ConfigurationObject::getValue(const std::string &key,
                                     double deflt) const
{
    GETLOG();
    Options::const_iterator iter = mOptions.find(key);
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

ConfigurationObject::~ConfigurationObject()
{
    clear();
}

Configuration::Configuration() :
    mConfigPath(""),
    mDefaultsData(nullptr),
    mDirectory("")
{
#ifdef DEBUG_CONFIG
    mLogKeys = false;
    mIsMain = false;
#endif
}

void Configuration::cleanDefaults()
{
    if (mDefaultsData)
    {
        for (DefaultsData::const_iterator iter = mDefaultsData->begin();
             iter != mDefaultsData->end(); ++iter)
        {
            delete (iter->second);
        }
        mDefaultsData->clear();
        delete mDefaultsData;
        mDefaultsData = nullptr;
    }
}

Configuration::~Configuration()
{
    cleanDefaults();
}

void Configuration::setDefaultValues(DefaultsData *defaultsData)
{
    cleanDefaults();
    mDefaultsData = defaultsData;
}

int Configuration::getIntValue(const std::string &key) const
{
    GETLOG();
    int defaultValue = 0;
    Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        if (mDefaultsData)
        {
            DefaultsData::const_iterator itdef = mDefaultsData->find(key);

            if (itdef != mDefaultsData->end() && itdef->second)
            {
                if (itdef->second->getType() == VariableData::DATA_INT)
                {
                    defaultValue = (static_cast<IntData*>(
                        itdef->second))->getData();
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_STRING)
                {
                    defaultValue = atoi((static_cast<StringData*>(
                        itdef->second))->getData().c_str());
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_BOOL)
                {
                    if ((static_cast<BoolData*>(
                        itdef->second))->getData())
                    {
                        defaultValue = 1;
                    }
                    else
                    {
                        defaultValue = 0;
                    }
                }
                else if (itdef->second->getType() == VariableData::DATA_FLOAT)
                {
                    defaultValue = static_cast<int>(
                        (static_cast<FloatData*>(itdef->second))->getData());
                }
            }
            else
            {
                logger->log("%s: No integer value in registry for key %s",
                            mConfigPath.c_str(), key.c_str());
            }
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
    if (mDefaultsData)
    {
        DefaultsData::const_iterator itdef = mDefaultsData->find(key);

        if (itdef != mDefaultsData->end() && itdef->second
            && itdef->second->getType() == VariableData::DATA_INT)
        {
            defaultValue = (static_cast<IntData*>(
            itdef->second))->getData();
        }
        else
        {
            logger->log("%s: No integer value in registry for key %s",
                        mConfigPath.c_str(), key.c_str());
        }
    }
    setValue(key, defaultValue);
    return defaultValue;
}

std::string Configuration::getStringValue(const std::string &key) const
{
    GETLOG();
    std::string defaultValue("");
    Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        if (mDefaultsData)
        {
            DefaultsData::const_iterator itdef = mDefaultsData->find(key);

            if (itdef != mDefaultsData->end() && itdef->second)
            {
                if (itdef->second->getType()
                    == VariableData::DATA_STRING)
                {
                    defaultValue = (static_cast<StringData*>(
                        itdef->second))->getData();
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_BOOL)
                {
                    if ((static_cast<BoolData*>(
                        itdef->second))->getData())
                    {
                        defaultValue = "1";
                    }
                    else
                    {
                        defaultValue = "0";
                    }
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_INT)
                {
                    defaultValue = toString((static_cast<IntData*>(
                        itdef->second))->getData());
                }
            }
            else
            {
                logger->log("%s: No string value in registry for key %s",
                            mConfigPath.c_str(), key.c_str());
            }
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
    float defaultValue = 0.0f;
    Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        if (mDefaultsData)
        {
            DefaultsData::const_iterator itdef = mDefaultsData->find(key);

            if (itdef != mDefaultsData->end() && itdef->second)
            {
                if (itdef->second->getType() == VariableData::DATA_FLOAT)
                {
                    defaultValue = static_cast<float>(
                        (static_cast<FloatData*>(itdef->second))->getData());
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_STRING)
                {
                    defaultValue = atof((static_cast<StringData*>(
                        itdef->second))->getData().c_str());
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_BOOL)
                {
                    if ((static_cast<BoolData*>(
                        itdef->second))->getData())
                    {
                        defaultValue = 1;
                    }
                    else
                    {
                        defaultValue = 0;
                    }
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_INT)
                {
                    defaultValue = (static_cast<IntData*>(
                        itdef->second))->getData();
                }
            }
            else
            {
                logger->log("%s: No float value in registry for key %s",
                            mConfigPath.c_str(), key.c_str());
            }
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
    Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        if (mDefaultsData)
        {
            DefaultsData::const_iterator itdef = mDefaultsData->find(key);

            if (itdef != mDefaultsData->end() && itdef->second)
            {
                if (itdef->second->getType() == VariableData::DATA_BOOL)
                {
                    defaultValue = (static_cast<BoolData*>(
                        itdef->second))->getData();
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_INT)
                {
                    if ((static_cast<IntData*>(
                        itdef->second))->getData() != 0)
                    {
                        defaultValue = true;
                    }
                    else
                    {
                        defaultValue = false;
                    }
                }
                else if (itdef->second->getType()
                         == VariableData::DATA_STRING)
                {
                    if ((static_cast<StringData*>(
                        itdef->second))->getData() != "0")
                    {
                        defaultValue = true;
                    }
                    else
                    {
                        defaultValue = false;
                    }
                }
                if (itdef->second->getType() == VariableData::DATA_FLOAT)
                {
                    if (static_cast<int>((static_cast<FloatData*>(
                        itdef->second))->getData()) != 0)
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
                logger->log("%s: No boolean value in registry for key %s",
                            mConfigPath.c_str(), key.c_str());
            }
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
    if (mDefaultsData)
    {
        DefaultsData::const_iterator itdef = mDefaultsData->find(key);

        if (itdef != mDefaultsData->end() && itdef->second
            && itdef->second->getType() == VariableData::DATA_BOOL)
        {
            defaultValue = (static_cast<BoolData*>(
                itdef->second))->getData();
        }
        else
        {
            logger->log("%s: No boolean value in registry for key %s",
                        mConfigPath.c_str(), key.c_str());
        }
    }

    setValue(key, defaultValue);
    return defaultValue;
}


void ConfigurationObject::initFromXML(XmlNodePtr parent_node)
{
    clear();

    for_each_xml_child_node(node, parent_node)
    {
        if (xmlNameEqual(node, "list"))
        {
            // list option handling

            std::string name = XML::getProperty(node, "name", std::string());

            for_each_xml_child_node(subnode, node)
            {
                if (xmlNameEqual(subnode, name.c_str())
                    && subnode->type == XML_ELEMENT_NODE)
                {
                    ConfigurationObject *cobj = new ConfigurationObject;

                    cobj->initFromXML(subnode); // recurse

                    mContainerOptions[name].push_back(cobj);
                }
            }

        }
        else if (xmlNameEqual(node, "option"))
        {
            // single option handling

            std::string name = XML::getProperty(node, "name", std::string());
            std::string value = XML::getProperty(node, "value", std::string());

            if (!name.empty())
                mOptions[name] = value;
        } // otherwise ignore
    }
}

void Configuration::init(const std::string &filename, bool useResManager)
{
    mDefaultsData = nullptr;
    XML::Document doc(filename, useResManager);

    if (useResManager)
    {
        mConfigPath = "PhysFS://" + filename;
        mDirectory = "";
    }
    else
    {
        mConfigPath = filename;
        logger->log("init 1");
        mDirectory = getRealPath(getFileDir(filename));
    }
    logger->log("init 2");

    if (!doc.rootNode())
    {
        logger->log("Couldn't open configuration file: %s", filename.c_str());
        return;
    }

    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "configuration"))
    {
        logger->log("Warning: No configuration file (%s)", filename.c_str());
        return;
    }

    initFromXML(rootNode);
}

void ConfigurationObject::writeToXML(XmlTextWriterPtr writer)
{
    for (Options::const_iterator i = mOptions.begin(), i_end = mOptions.end();
         i != i_end; ++i)
    {
#ifdef DEBUG_CONFIG
        if (mLogKeys)
        {
            if (optionsCount.find(i->first) == optionsCount.end())
                logger->log("unused configuration option: " + i->first);
        }
#endif
        xmlTextWriterStartElement(writer, BAD_CAST "option");
        xmlTextWriterWriteAttribute(writer,
                BAD_CAST "name", BAD_CAST i->first.c_str());

        xmlTextWriterWriteAttribute(writer,
                BAD_CAST "value", BAD_CAST i->second.c_str());
        xmlTextWriterEndElement(writer);
    }

    for (std::map<std::string, ConfigurationList>::const_iterator
         it = mContainerOptions.begin(), it_end = mContainerOptions.end();
         it != it_end; ++ it)
    {
        const char *name = it->first.c_str();

        xmlTextWriterStartElement(writer, BAD_CAST "list");
        xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST name);

        // recurse on all elements
        for (ConfigurationList::const_iterator elt_it = it->second.begin(),
             elt_it_end = it->second.end(); elt_it != elt_it_end; ++elt_it)
        {
            xmlTextWriterStartElement(writer, BAD_CAST name);
            if (*elt_it)
                (*elt_it)->writeToXML(writer);
            xmlTextWriterEndElement(writer);
        }

        xmlTextWriterEndElement(writer);
    }
}

void Configuration::write()
{
    // Do not attempt to write to file that cannot be opened for writing
    FILE *testFile = fopen(mConfigPath.c_str(), "w");
    if (!testFile)
    {
        logger->log("Configuration::write() couldn't open %s for writing",
                    mConfigPath.c_str());
        return;
    }
    else
    {
        fclose(testFile);
    }

    XmlTextWriterPtr writer = xmlNewTextWriterFilename(mConfigPath.c_str(), 0);

    if (!writer)
    {
        logger->log1("Configuration::write() error while creating writer");
        return;
    }

    logger->log1("Configuration::write() writing configuration...");

    xmlTextWriterSetIndent(writer, 1);
    xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
//    xmlTextWriterStartDocument(writer, nullptr, "utf8", nullptr);
    xmlTextWriterStartElement(writer, BAD_CAST "configuration");

    writeToXML(writer);

    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}

void Configuration::addListener(const std::string &key,
                                ConfigListener *listener)
{
    mListenerMap[key].push_front(listener);
}

void Configuration::removeListener(const std::string &key,
                                   ConfigListener *listener)
{
    mListenerMap[key].remove(listener);
}

void Configuration::removeListeners(ConfigListener *listener)
{
    for (ListenerMapIterator it = mListenerMap.begin(),
         it_end = mListenerMap.end(); it != it_end; ++ it)
    {
        (it->second).remove(listener);
    }
}

void Configuration::removeOldKeys()
{
    if (mOptions.find(unusedKeys[0]) != mOptions.end()
        || mOptions.find(unusedKeys[1]) != mOptions.end())
    {
        int f = 0;
        while (unusedKeys[f] != "")
        {
            deleteKey(unusedKeys[f]);
            logger->log("remove unused key: " + unusedKeys[f]);
            f ++;
        }
        for (f = 0; f < 80; f ++)
        {
            deleteKey("Outfit" + toString(f));
            deleteKey("OutfitUnequip" + toString(f));
            deleteKey("commandShortcutCmd" + toString(f));
            deleteKey("commandShortcutFlags" + toString(f));
            deleteKey("commandShortcutSymbol" + toString(f));
            deleteKey("drop" + toString(f));
            deleteKey("shortcut" + toString(f));
        }
    }
}
