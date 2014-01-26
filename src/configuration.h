/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "utils/stringutils.h"
#include "utils/xml.h"

#include "defaults.h"
#include "localconsts.h"

#include <list>
#include <map>
#include <string>

class ConfigListener;
class ConfigurationObject;

/**
 * Configuration list manager interface; responsible for
 * serializing/deserializing configuration choices in containers.
 *
 * \param T Type of the container elements to serialise
 * \param CONT Type of the container we (de)serialise
 */
template <class T, class CONT>
class ConfigurationListManager
{
    public:
        /**
         * Writes a value into a configuration object
         *
         * \param value The value to write out
         * \param obj The configuation object to write to
         * \return obj, or otherwise NULL to indicate that this option should
         *         be skipped
         */
        virtual ConfigurationObject *writeConfigItem(const T &value,
                                                     ConfigurationObject
                                                     *const obj) const = 0;

        /**
         * Reads a value from a configuration object
         *
         * \param obj The configuration object to read from
         * \param container The container to insert the object to
         */
        virtual CONT readConfigItem(const ConfigurationObject *const obj,
                                    CONT container)
                                    const A_WARN_UNUSED = 0;

        virtual ~ConfigurationListManager() {}
};

/**
 * Configuration object, mapping values to names and possibly containing
 * lists of further configuration objects
 *
 * \ingroup CORE
 */
class ConfigurationObject
{
    friend class Configuration;

    public:
        virtual ~ConfigurationObject();

        /**
         * Sets an option using a string value.
         *
         * \param key Option identifier.
         * \param value Value.
         */
        virtual void setValue(const std::string &key,
                              const std::string &value);

        void deleteKey(const std::string &key);

        /**
         * Gets a value as string.
         *
         * \param key Option identifier.
         * \param deflt Default option if not there or error.
         */
        std::string getValue(const std::string &key,
                             const std::string &deflt) const A_WARN_UNUSED;

        int getValue(const std::string &key,
                     const int deflt) const A_WARN_UNUSED;

        int getValueInt(const std::string &key,
                        const int deflt) const A_WARN_UNUSED;

        bool getValueBool(const std::string &key,
                          const bool deflt) const A_WARN_UNUSED;

        unsigned getValue(const std::string &key,
                          const unsigned deflt) const A_WARN_UNUSED;

        double getValue(const std::string &key,
                        const double deflt) const A_WARN_UNUSED;

        /**
         * Re-sets all data in the configuration
         */
        void clear();

        /**
         * Serialises a container into a list of configuration options
         *
         * \param IT Iterator type over CONT
         * \param T Elements that IT iterates over
         * \param CONT The associated container type
         *
         * \param name Name of the list the elements should be stored under
         * \param begin Iterator start
         * \param end Iterator end
         * \param manager An object capable of serialising T items
         */
        template <class IT, class T, class CONT>
        void setList(const std::string &name, IT begin, IT end,
                     ConfigurationListManager<T, CONT> *manager)
        {
            if (!manager)
                return;

            ConfigurationObject *nextobj = new ConfigurationObject;
            deleteList(name);
            ConfigurationList *list = &(mContainerOptions[name]);

            for (IT it = begin; it != end; it++)
            {
                ConfigurationObject *const wrobj
                    = manager->writeConfigItem(*it, nextobj);
                if (wrobj)
                { // wrote something
//                    assert (wrobj == nextobj);
                    nextobj = new ConfigurationObject;
                    list->push_back(wrobj);
                }
                else
                {
                    nextobj->clear();  // you never know...
                }
            }

            delete nextobj;
        }

       /**
        * Serialises a container into a list of configuration options
        *
        * \param IT Iterator type over CONT
        * \param T Elements that IT iterates over
        * \param CONT The associated container type
        *
        * \param name Name of the list the elements should be read from under
        * \param empty Initial (empty) container to write to
        * \param manager An object capable of deserialising items into CONT
        */
        template<class T, class CONT>
        CONT getList(const std::string &name, CONT empty,
                     ConfigurationListManager<T, CONT> *manager)
        {
            ConfigurationList *const list = &(mContainerOptions[name]);
            CONT container = empty;

            if (!manager)
                return container;

            for (ConfigurationList::const_iterator it = list->begin();
                 it != list->end(); ++ it)
            {
                container = manager->readConfigItem(*it, container);
            }

            return container;
        }

#ifdef DEBUG_CONFIG
        void enableKeyLogging()
        { mLogKeys = true; }

        void setIsMain(bool b)
        { mIsMain = b; }
#endif

    protected:
        ConfigurationObject();

        virtual void initFromXML(const XmlNodePtrConst parent_node);
        virtual void writeToXML(const XmlTextWriterPtr writer);

        void deleteList(const std::string &name);

        typedef std::map<std::string, std::string> Options;
        Options mOptions;

        typedef std::list<ConfigurationObject *> ConfigurationList;
        std::map<std::string, ConfigurationList> mContainerOptions;

#ifdef DEBUG_CONFIG
        bool mLogKeys;
        bool mIsMain;
#endif
};

/**
 * Configuration handler for reading (and writing).
 *
 * \ingroup CORE
 */
class Configuration final : public ConfigurationObject
{
    public:
        Configuration();

        A_DELETE_COPY(Configuration)

        ~Configuration();

        /**
         * Reads config file and parse all options into memory.
         *
         * @param filename path to config file
         * @param useResManager Make use of the resource manager.
         */
        void init(const std::string &filename,
                  const bool useResManager = false);

        void reInit();

        void unload();

        /**
         * Set the default values for each keys.
         *
         * @param defaultsData data used as defaults.
         */
        void setDefaultValues(DefaultsData *const defaultsData);

        DefaultsData *getDefaultValues()
        { return mDefaultsData; }

        /**
         * Writes the current settings back to the config file.
         */
        void write();

        /**
         * Adds a listener to the listen list of the specified config option.
         */
        void addListener(const std::string &key,
                         ConfigListener *const listener);

        /**
         * Removes a listener from the listen list of the specified config
         * option.
         */
        void removeListener(const std::string &key,
                            ConfigListener *const listener);

        void removeListeners(ConfigListener *const listener);

#ifdef ENABLE_CHECKS
        void checkListeners(ConfigListener *const listener,
                            const char *const file,
                            const unsigned line);
#endif

        void setValue(const std::string &key, const std::string &value);

        void incValue(const std::string &key);

        void setSilent(const std::string &key, const std::string &value);

        inline void setValue(const std::string &key, const char *const value)
        { if (value) setValue(key, std::string(value)); }

        inline void setSilent(const std::string &key, const char *const value)
        { if (value) setSilent(key, std::string(value)); }

        inline void setValue(const std::string &key, const float value)
        { setValue(key, toString(value)); }

        inline void setValue(const std::string &key, const double value)
        { setValue(key, toString(value)); }

        inline void setValue(const std::string &key, const int value)
        { setValue(key, toString(value)); }

        inline void setValueInt(const std::string &key, const int value)
        { setValue(key, toString(value)); }

        inline void setValue(const std::string &key, const unsigned value)
        { setValue(key, toString(value)); }

        inline void setValue(const std::string &key, const bool value)
        { setValue(key, value ? "1" : "0"); }

        inline void setSilent(const std::string &key, const bool value)
        { setSilent(key, value ? "1" : "0"); }

        int resetIntValue(const std::string &key);

        bool resetBoolValue(const std::string &key);

        const std::string &getConfigPath() const A_WARN_UNUSED
        { return mConfigPath; }

        /**
         * returns a value corresponding to the given key.
         * The default value returned in based on fallbacks registry.
         * @see defaults.h
         */
        int getIntValue(const std::string &key) const A_WARN_UNUSED;
        float getFloatValue(const std::string &key) const A_WARN_UNUSED;
        std::string getStringValue(const std::string &key) const A_WARN_UNUSED;
        bool getBoolValue(const std::string &key) const A_WARN_UNUSED;

        std::string getDirectory() const A_WARN_UNUSED
        { return mDirectory; }

        void removeOldKeys();

        std::string getFileName() const A_WARN_UNUSED
        { return mFilename; }

    private:
        /**
         * Clean up the default values member.
         */
        void cleanDefaults();

        typedef std::list<ConfigListener*> Listeners;
        typedef Listeners::iterator ListenerIterator;
        typedef std::map<std::string, Listeners> ListenerMap;
        typedef ListenerMap::iterator ListenerMapIterator;
        ListenerMap mListenerMap;

        // Location of config file
        std::string mConfigPath;
        /// Defaults of value for a given key
        DefaultsData *mDefaultsData;
        std::string mDirectory;
        std::string mFilename;
        bool mUseResManager;
};

extern Configuration branding;
extern Configuration config;
extern Configuration serverConfig;
extern Configuration paths;
extern Configuration features;

#endif  // CONFIGURATION_H
