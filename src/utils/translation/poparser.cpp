/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "utils/translation/poparser.h"

#include "fs/virtfs/fs.h"

#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include "logger.h"

#include "debug.h"

PoParser::PoParser() :
    mLang(),
    mFile(),
    mLine(),
    mMsgId(),
    mMsgStr(),
    mDict(nullptr),
    mReadingId(false),
    mReadingStr(false),
    mSkipId(false)
{
}

void PoParser::openFile(const std::string &name)
{
    int size;
    const char *buf = VirtFs::loadFile(getFileName(name), size);

    if (buf != nullptr)
    {
        mFile.str(std::string(buf, size));
        delete [] buf;
    }
    else
    {
        mFile.clear();
    }
}

PoDict *PoParser::load(const std::string &restrict lang,
                       const std::string &restrict fileName,
                       PoDict *restrict const dict)
{
    logger->log("loading lang: %s, file: %s", lang.c_str(), fileName.c_str());

    setLang(lang);
    if (dict == nullptr)
        mDict = getDict();
    else
        mDict = dict;

    if (fileName.empty())
        openFile(mLang);
    else
        openFile(fileName);

    mMsgId.clear();
    mMsgStr.clear();

    // cycle by msgid+msgstr
    while (readLine())
    {
        // reading msgid
        while (readMsgId())
        {
            if (!readLine())
                break;
        }

        if (!mMsgId.empty())
        {
            // if we got msgid then reading msgstr
            while (readMsgStr())
            {
                if (!readLine())
                    break;
            }
        }

        if (!mMsgId.empty() && !mMsgStr.empty())
        {
//            logger->log("add key: " + mMsgId);
//            logger->log("add value: " + mMsgStr);

            convertStr(mMsgId);
            convertStr(mMsgStr);
            // store key and value
            mDict->set(mMsgId, mMsgStr);
        }

        mMsgId.clear();
        mMsgStr.clear();
    }

    return mDict;
}

bool PoParser::readLine()
{
    char line[1001];
    if (!mFile.getline(line, 1000))
        return false;
    // skip plurals msgid if present
    if (strStartWith(line, "msgid_plural \""))
    {
        if (!mFile.getline(line, 1000))
            return false;
    }
    mLine = line;
    return true;
}

bool PoParser::readMsgId()
{
    // if we reading msgstr then stop here
    if (mReadingStr)
        return false;

    const std::string msgId1("msgid \"");

    // check if in reading process
    if (mReadingId)
    {
        // if we get empty line in file then stop reading
        if (mLine.empty())
        {
            mReadingId = false;
            return false;
        }
        else if (checkLine())
        {
            // reading text from: "text"
            mMsgId.append(mLine.substr(1, mLine.size() - 2));
            mLine.clear();
            return true;
        }
        // stop reading in other case
        mReadingId = false;
        return false;
    }
    else
    {
        // check line start from msgid "
        if (strStartWith(mLine, msgId1))
        {
            if (!mSkipId)
            {   // translation not fuzzed and can be processed
                mReadingId = true;
                const size_t msgId1Size = msgId1.size();
                // reading text from: msgid "text"
                mMsgId.append(mLine.substr(msgId1Size,
                    mLine.size() - 1 - msgId1Size));
            }
            else
            {   // skipped fuzzed translation. reset skip flag
                mSkipId = false;
            }
            mLine.clear();
            return true;
        }
        else if (mLine == "#, fuzzy")
        {   // check for fuzzy translation
            mSkipId = true;
            mLine.clear();
            return true;
        }
        // stop reading if we don't read msgid before
        return mMsgId.empty();
    }
}

bool PoParser::readMsgStr()
{
    // normal msgstr
    const std::string msgStr1("msgstr \"");
    // plurals first msgstr
    const std::string msgStr2("msgstr[0] \"");

    // check if in reading process
    if (mReadingStr)
    {
        // if we get empty line in file then stop reading
        if (mLine.empty())
        {
            mReadingStr = false;
            return false;
        }
        if (checkLine())
        {
            // reading text from: "text"
            mMsgStr.append(mLine.substr(1, mLine.size() - 2));
            mLine.clear();
            return true;
        }
        // stop reading in other case
        mReadingStr = false;
    }
    else
    {
        // check line start from msgstr "
        if (strStartWith(mLine, msgStr1))
        {
            mReadingStr = true;
            const size_t msgStr1Size = msgStr1.size();
            // reading text from: msgstr "text"
            mMsgStr.append(mLine.substr(msgStr1Size,
                mLine.size() - 1 - msgStr1Size));
            mLine.clear();
            return true;
        }
        // checl list start from msgstr[0] "
        else if (strStartWith(mLine, msgStr2))
        {
            mReadingStr = true;
            const size_t msgStr2Size = msgStr2.size();
            // reading text from: msgstr[0] "text"
            mMsgStr.append(mLine.substr(msgStr2Size,
                mLine.size() - 1 - msgStr2Size));
            mLine.clear();
            return true;
        }
    }

    // stop reading in other case
    return false;
}

bool PoParser::checkLine() const
{
    const size_t sz = mLine.size();
    // check is line in format: "text"
    return sz > 2 && mLine[0] == '\"' && mLine[sz - 1] == '\"';
}

PoDict *PoParser::getEmptyDict()
{
    return new PoDict("");
}

bool PoParser::checkLang(const std::string &lang)
{
    // check is po file exists
    return VirtFs::exists(getFileName(lang));
}

std::string PoParser::getFileName(const std::string &lang)
{
    // get po file name from lang name
//    logger->log("getFileName: translations/%s.po", lang.c_str());
    return strprintf("translations/%s.po", lang.c_str());
}

PoDict *PoParser::getDict() const
{
    return new PoDict(mLang);
}

void PoParser::convertStr(std::string &str)
{
    if (str.empty())
        return;

    replaceAll(str, "\\n", "\n");
    replaceAll(str, "\\\"", "\"");
    replaceAll(str, "\\\\", "\\");
}
