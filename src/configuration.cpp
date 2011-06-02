/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "log.h"

#include "utils/stringutils.h"
#include "utils/xml.h"

#include <libxml/encoding.h>

#include "debug.h"

#ifdef DEBUG_CONFIG
#define GETLOG() if (logger) {logger->log("config get: " + key); }
#else
#define GETLOG()
#endif

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
        for (ListenerIterator i = listeners.begin(); i != listeners.end(); ++i)
            (*i)->optionChanged(key);
    }
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

void Configuration::cleanDefaults()
{
    if (mDefaultsData)
    {
        for (DefaultsData::const_iterator iter = mDefaultsData->begin();
             iter != mDefaultsData->end(); ++iter)
        {
            delete(iter->second);
        }
        mDefaultsData->clear();
        delete mDefaultsData;
        mDefaultsData = 0;
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

            if (itdef != mDefaultsData->end() && itdef->second
                && itdef->second->getType() == Mana::VariableData::DATA_INT)
            {
                defaultValue = (static_cast<Mana::IntData*>(
                    itdef->second))->getData();
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
            && itdef->second->getType() == Mana::VariableData::DATA_INT)
        {
            defaultValue = (static_cast<Mana::IntData*>(
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
    std::string defaultValue = "";
    Options::const_iterator iter = mOptions.find(key);
    if (iter == mOptions.end())
    {
        if (mDefaultsData)
        {
            DefaultsData::const_iterator itdef = mDefaultsData->find(key);

            if (itdef != mDefaultsData->end() && itdef->second
                && itdef->second->getType() == Mana::VariableData::DATA_STRING)
            {
                defaultValue = (static_cast<Mana::StringData*>(
                    itdef->second))->getData();
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

            if (itdef != mDefaultsData->end() && itdef->second
                && itdef->second->getType() == Mana::VariableData::DATA_FLOAT)
            {
                defaultValue = static_cast<float>(
                    (static_cast<Mana::FloatData*>(itdef->second))->getData());
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

            if (itdef != mDefaultsData->end() && itdef->second
                && itdef->second->getType() == Mana::VariableData::DATA_BOOL)
            {
                defaultValue = (static_cast<Mana::BoolData*>(
                    itdef->second))->getData();
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
            && itdef->second->getType() == Mana::VariableData::DATA_BOOL)
        {
            defaultValue = (static_cast<Mana::BoolData*>(
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

void ConfigurationObject::initFromXML(xmlNodePtr parent_node)
{
    clear();

    for_each_xml_child_node(node, parent_node)
    {
        if (xmlStrEqual(node->name, BAD_CAST "list"))
        {
            // list option handling

            std::string name = XML::getProperty(node, "name", std::string());

            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST name.c_str())
                    && subnode->type == XML_ELEMENT_NODE)
                {
                    ConfigurationObject *cobj = new ConfigurationObject;

                    cobj->initFromXML(subnode); // recurse

                    mContainerOptions[name].push_back(cobj);
                }
            }

        }
        else if (xmlStrEqual(node->name, BAD_CAST "option"))
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
    mDefaultsData = 0;
    XML::Document doc(filename, useResManager);

    if (useResManager)
        mConfigPath = "PhysFS://" + filename;
    else
        mConfigPath = filename;

    if (!doc.rootNode())
    {
        logger->log("Couldn't open configuration file: %s", filename.c_str());
        return;
    }

    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "configuration"))
    {
        logger->log("Warning: No configuration file (%s)", filename.c_str());
        return;
    }

    initFromXML(rootNode);
}

void ConfigurationObject::writeToXML(xmlTextWriterPtr writer)
{
    for (Options::const_iterator i = mOptions.begin(), i_end = mOptions.end();
         i != i_end; ++i)
    {
        xmlTextWriterStartElement(writer, BAD_CAST "option");
        xmlTextWriterWriteAttribute(writer,
                BAD_CAST "name", BAD_CAST i->first.c_str());

        xmlTextWriterWriteAttribute(writer,
                BAD_CAST "value", BAD_CAST i->second.c_str());
        xmlTextWriterEndElement(writer);
    }

    for (std::map<std::string, ConfigurationList>::const_iterator
         it = mContainerOptions.begin();
         it != mContainerOptions.end(); ++it)
    {
        const char *name = it->first.c_str();

        xmlTextWriterStartElement(writer, BAD_CAST "list");
        xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST name);

        // recurse on all elements
        for (ConfigurationList::const_iterator elt_it = it->second.begin();
             elt_it != it->second.end(); ++elt_it)
        {
            xmlTextWriterStartElement(writer, BAD_CAST name);
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

    xmlTextWriterPtr writer = xmlNewTextWriterFilename(mConfigPath.c_str(), 0);

    if (!writer)
    {
        logger->log1("Configuration::write() error while creating writer");
        return;
    }

    logger->log1("Configuration::write() writing configuration...");

    xmlTextWriterSetIndent(writer, 1);
    xmlTextWriterStartDocument(writer, NULL, NULL, NULL);
//    xmlTextWriterStartDocument(writer, NULL, "utf8", NULL);
    xmlTextWriterStartElement(writer, BAD_CAST "configuration");

    writeToXML(writer);

    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}

void Configuration::addListener(
        const std::string &key, ConfigListener *listener)
{
    mListenerMap[key].push_front(listener);
}

void Configuration::removeListener(
        const std::string &key, ConfigListener *listener)
{
    mListenerMap[key].remove(listener);
}