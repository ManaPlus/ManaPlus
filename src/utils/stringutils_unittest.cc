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

#include "catch.hpp"
#include "configuration.h"
#include "logger.h"

#include "const/utils/utf8.h"

#include "fs/virtfs.h"

#include "utils/delete2.h"
#include "utils/dtor.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("stringuntils  trim 1")
{
    std::string str = "str";
    REQUIRE("str" == trim(str));

    str = " str";
    REQUIRE("str" == trim(str));
    REQUIRE("str" == trim(str));

    str = " str   this IS Long Стринг "
        "~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   ";
    REQUIRE("str   this IS Long Стринг ~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?" ==
        trim(str));

    str = "";
    REQUIRE("" == trim(str));
}

TEST_CASE("stringuntils toLower 1")
{
    std::string str = "str";
    REQUIRE("str" == toLower(str));

    str = " StR";
    REQUIRE(" str" == toLower(str));

    str = " str   this IS Long  "
        "~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   ";

    REQUIRE(" str   this is long  ~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   " ==
        toLower(str));

    str = "";
    REQUIRE("" == toLower(str));
}

TEST_CASE("stringuntils toUpper 1")
{
    std::string str = "str";
    REQUIRE("STR" == toUpper(str));

    str = " StR";
    REQUIRE(" STR" == toUpper(str));

    str = " str   this IS Long  "
        "~!@#$%^&*()_+`-=[]\\{}|;':,./<>?   ";

    REQUIRE(" STR   THIS IS LONG  ~!@#$%^&*()_+`-=[]\\{}|;':,./<>?   " ==
        toUpper(str));

    str = "";
    REQUIRE("" == toUpper(str));
}

TEST_CASE("stringuntils atox 1")
{
    std::string str = "0x10";
    REQUIRE(16 == atox(str));

    str = "0x0";
    REQUIRE(0 == atox(str));

    str = "0x1";
    REQUIRE(1 == atox(str));

    str = "0x0x0x0x0x0x0x0";
    REQUIRE(0 == atox(str));

    str = "0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    const int k = atox(str);
    printf("%d\n", k);  // for avoid warning

    str = "";
    REQUIRE(0 == atox(str));

    str = "0";
    REQUIRE(0 == atox(str));

    str = "0x";
    REQUIRE(0 == atox(str));

    str = "zzz";
    REQUIRE(0 == atox(str));
}

TEST_CASE("stringuntils ipToString 1")
{
    REQUIRE("0.0.0.0" == std::string(ipToString(0)));
    REQUIRE("219.255.210.73" == std::string(ipToString(1238564827)));
}

TEST_CASE("stringuntils toString 1")
{
    REQUIRE(strprintf("%d", 0) == toString(0));
    REQUIRE(strprintf("%d", -1) == toString(-1));
    REQUIRE(strprintf("%d", 30000000) == toString(30000000));
    REQUIRE(strprintf("%d", -10000000) == toString(-10000000));
    REQUIRE(strprintf("%d", 30000000) == toString(
        static_cast<signed int>(30000000)));
    REQUIRE(strprintf("%d", 3000) == toString(CAST_U16(3000)));
    REQUIRE(strprintf("%d", 123) == toString(CAST_U8(123)));
    REQUIRE(strprintf("%ld", static_cast<unsigned long>(30000000)) == toString(
        static_cast<unsigned long>(30000000)));
    REQUIRE(strprintf("%f", 3.1f) == toString(3.1f));
    REQUIRE(strprintf("%f", 3.1) == toString(3.1));
}

TEST_CASE("stringuntils toStringPrint 1")
{
    REQUIRE(toStringPrint(0) == "0 0x0");
    REQUIRE(toStringPrint(10) == "10 0xa");
    REQUIRE(toStringPrint(255) == "255 0xff");
}

TEST_CASE("stringuntils parse2Int 1")
{
    int a = -1;
    int b = -1;

    REQUIRE(parse2Int("", a, b) == false);
    REQUIRE(a == -1);
    REQUIRE(b == -1);

    a = -1;
    b = -1;
    REQUIRE(parse2Int(",", a, b) == false);
    REQUIRE(a == -1);
    REQUIRE(b == -1);

    a = -1;
    b = -1;
    REQUIRE(parse2Int("10,20", a, b) == true);
    REQUIRE(a == 10);
    REQUIRE(b == 20);

    a = -1;
    b = -1;
    REQUIRE(parse2Int("10 20", a, b) == true);
    REQUIRE(a == 10);
    REQUIRE(b == 20);

    a = -1;
    b = -1;
    REQUIRE(parse2Int("10 z20", a, b) == true);
    REQUIRE(a == 10);
    REQUIRE(b == 0);
}

TEST_CASE("stringuntils parse2Str 1")
{
    std::string str1 = "-";
    std::string str2 = "-";

    REQUIRE(parse2Str("", str1, str2) == false);
    REQUIRE(str1 == "-");
    REQUIRE(str2 == "-");

    REQUIRE(parse2Str(",", str1, str2) == false);
    REQUIRE(str1 == "-");
    REQUIRE(str2 == "-");

    str1 = "-";
    str2 = "-";
    REQUIRE(parse2Str("test line", str1, str2) == true);
    REQUIRE(str1 == "test");
    REQUIRE(str2 == "line");

    str1 = "-";
    str2 = "-";
    REQUIRE(parse2Str("test,line", str1, str2) == true);
    REQUIRE(str1 == "test");
    REQUIRE(str2 == "line");
}

TEST_CASE("stringuntils parseNumber 1")
{
    REQUIRE(parseNumber("") == 0);
    REQUIRE(parseNumber("0x") == 0);
    REQUIRE(parseNumber("10") == 10);
    REQUIRE(parseNumber("h10") == 16);
    REQUIRE(parseNumber("x100") == 256);
    REQUIRE(parseNumber("0x20") == 32);
}

TEST_CASE("stringuntils removeToken 1")
{
    std::string str = "";

    REQUIRE(removeToken(str, " ") == "");
    REQUIRE(str == "");

    str = "test";
    REQUIRE(removeToken(str, " ") == "");
    REQUIRE(str == "");

    str = "test line";
    REQUIRE(removeToken(str, " ") == "line");
    REQUIRE(str == "line");

    str = "test,line";
    REQUIRE(removeToken(str, ",") == "line");
    REQUIRE(str == "line");

    str = "test line";
    REQUIRE(removeToken(str, ",") == "");
    REQUIRE(str == "");

    str = ",line";
    REQUIRE(removeToken(str, ",") == "");
    REQUIRE(str == "");
}

TEST_CASE("stringuntils strprintf 1")
{
    REQUIRE(strprintf("%s very long string. 123456789012345678901234567890"
        "+++++++++++++++++++++++++++++++++++++++"
        "________________________________"
        "***********************************"
        "----------------------------------------"
        "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| %s",
        "test", "end of test") ==
        "test very long string. 123456789012345678901234567890"
        "+++++++++++++++++++++++++++++++++++++++"
        "________________________________"
        "***********************************"
        "----------------------------------------"
        "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| "
        "end of test");
}

TEST_CASE("stringuntils toString 2")
{
    for (int f = 0; f < 10000000; f += 123)
    {
        REQUIRE(strprintf("%d", f) == toString(f));
    }
}

TEST_CASE("stringuntils removeColors 1")
{
    REQUIRE("" == removeColors(""));
    REQUIRE("#" == removeColors("#"));
    REQUIRE("##" == removeColors("##"));
    REQUIRE("" == removeColors("##1"));
    REQUIRE("2" == removeColors("##12"));
    REQUIRE("1##" == removeColors("1##"));
    REQUIRE("1" == removeColors("1##2"));
    REQUIRE("13" == removeColors("1##23"));
    REQUIRE("#1#2" == removeColors("#1#2"));
    REQUIRE("#1" == removeColors("#1##2"));
}

TEST_CASE("stringuntils compareStrI 1")
{
    std::string str1 = "";
    std::string str2 = "";
    REQUIRE(0 == compareStrI(str1, str2));

    str1 = "test";
    str2 = "test";
    REQUIRE(0 == compareStrI(str1, str2));

    str1 = "test";
    str2 = "test1";
    REQUIRE(0 > compareStrI(str1, str2));

    str1 = "test";
    str2 = "aest1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    REQUIRE(0 < compareStrI(str1, str2));

    str1 = "testaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    str2 = "testaa";
    REQUIRE(0 < compareStrI(str1, str2));
}

TEST_CASE("stringuntils isWordSeparator 1")
{
    REQUIRE(isWordSeparator(' '));
    REQUIRE(isWordSeparator(','));
    REQUIRE(isWordSeparator('.'));
    REQUIRE(isWordSeparator('\"'));
    REQUIRE(!isWordSeparator(0));
    REQUIRE(!isWordSeparator('a'));
    REQUIRE(!isWordSeparator('-'));
}

TEST_CASE("stringuntils findSameSubstring")
{
    std::string str1 = "";
    std::string str2 = "";

    REQUIRE("" == findSameSubstring("", ""));

    str1 = "test line";
    str2 = "test line";
    REQUIRE("test line" == findSameSubstring(str1, str2));

    str1 = "test li";
    str2 = "test line";
    REQUIRE("test li" == findSameSubstring(str1, str2));

    str1 = "test li";
    str2 = "est li";
    REQUIRE("" == findSameSubstring(str1, str2));
}

TEST_CASE("stringuntils findSameSubstringI")
{
    std::string str1 = "";
    std::string str2 = "";

    REQUIRE("" == findSameSubstringI("", ""));

    str1 = "tEst line";
    str2 = "tesT line";
    REQUIRE("tEst line" == findSameSubstringI(str1, str2));

    str1 = "test Li";
    str2 = "test lINe";
    REQUIRE("test Li" == findSameSubstringI(str1, str2));

    str1 = "test lINe";
    str2 = "test Li";
    REQUIRE("test lI" == findSameSubstringI(str1, str2));

    str1 = "teSt li";
    str2 = "est li";
    REQUIRE("" == findSameSubstringI(str1, str2));
}

TEST_CASE("stringuntils findI 1")
{
    REQUIRE(0 == findI("", ""));
    REQUIRE(std::string::npos == findI("test", "line"));
    REQUIRE(0 == findI("test line", "t"));
    REQUIRE(0 == findI("test line", "te"));
    REQUIRE(3 == findI("test line", "t l"));
}

TEST_CASE("stringuntils findI 2")
{
    std::vector <std::string> vect1;
    vect1.push_back("test");
    vect1.push_back("line");
    vect1.push_back("qwe");

    REQUIRE(std::string::npos == findI("", vect1));
    REQUIRE(0 == findI("test", vect1));
    REQUIRE(0 == findI("tesT lIne", vect1));
    REQUIRE(5 == findI("teoT line", vect1));
    REQUIRE(std::string::npos == findI("zzz", vect1));
}

TEST_CASE("stringuntils encodeStr 1")
{
    std::string str = encodeStr(10, 1);
    REQUIRE(10 == decodeStr(str));

    str = "";
    REQUIRE(0 == decodeStr(str));

    str = encodeStr(10, 2);
    REQUIRE(10 == decodeStr(str));

    str = encodeStr(100, 3);
    REQUIRE(100 == decodeStr(str));

    str = encodeStr(1000, 4);
    REQUIRE(1000 == decodeStr(str));
}

TEST_CASE("stringuntils extractNameFromSprite 1")
{
    REQUIRE("" == extractNameFromSprite(""));
    REQUIRE("test" == extractNameFromSprite("test"));
    REQUIRE("test" == extractNameFromSprite("test.qwe"));
    REQUIRE("line" == extractNameFromSprite("test/line.zzz"));
    REQUIRE("line" == extractNameFromSprite("test\\line.zzz"));
    REQUIRE("line" == extractNameFromSprite("test/test2\\line.zzz"));
    REQUIRE("line" == extractNameFromSprite("test\\test2/line.zzz"));
}

TEST_CASE("stringuntils removeSpriteIndex 1")
{
    REQUIRE("" == removeSpriteIndex(""));
    REQUIRE("test" == removeSpriteIndex("test"));
    REQUIRE("test" == removeSpriteIndex("test[1]"));
    REQUIRE("line" == removeSpriteIndex("test/line[12]"));
    REQUIRE("line" == removeSpriteIndex("test\\line[12]"));
    REQUIRE("line" == removeSpriteIndex("test/test2\\line[12]"));
    REQUIRE("line" == removeSpriteIndex("test\\test2/line[1]"));
}

TEST_CASE("stringutils getSafeUtf8String 1")
{
    const char *str;
    str = getSafeUtf8String("");
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("", str) == 0);
    REQUIRE(str[0] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;

    str = getSafeUtf8String("test line");
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("test line", str) == 0);
    REQUIRE(str[strlen("test line")] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;

    str = getSafeUtf8String("1");
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("1", str) == 0);
    REQUIRE(str[1] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;
}

TEST_CASE("stringutils getSafeUtf8String 2")
{
    char *str;

    getSafeUtf8String("test", nullptr);

    str = new char[65535];
    getSafeUtf8String("", str);
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("", str) == 0);
    REQUIRE(str[0] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;

    str = new char[65535];
    getSafeUtf8String("test line", str);
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("test line", str) == 0);
    REQUIRE(str[strlen("test line")] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;

    str = new char[65535];
    getSafeUtf8String("1", str);
    REQUIRE(str != nullptr);
    REQUIRE(strcmp("1", str) == 0);
    REQUIRE(str[1] == '\0');
    REQUIRE(str[UTF8_MAX_SIZE - 1] == '\0');
    delete [] str;

    str = new char[65535];
    char *data1 = new char[65510];
    memset(data1, 'a', 65510);
    data1[65509] = '\0';
    char *data2 = new char[65510];
    memset(data2, 'a', 65500);
    data2[65500] = '\0';
    getSafeUtf8String(data1, str);
    REQUIRE(str != nullptr);
    REQUIRE(strcmp(data2, str) == 0);
    REQUIRE(str[65500] == '\0');
    delete [] data1;
    delete [] data2;
    delete [] str;
}

TEST_CASE("stringuntils getFileName 1")
{
    REQUIRE("" == getFileName(""));
    REQUIRE("file" == getFileName("file"));
    REQUIRE("file" == getFileName("test/file1\\file"));
    REQUIRE("file" == getFileName("test\\file1/file"));
    REQUIRE("" == getFileName("file/"));
    REQUIRE("file" == getFileName("/file"));
}

TEST_CASE("stringuntils getFileDir 1")
{
    REQUIRE("" == getFileDir(""));
    REQUIRE("file" == getFileDir("file"));
    REQUIRE("test/file1" == getFileDir("test/file1\\file"));
    REQUIRE("test\\file1" == getFileDir("test\\file1/file"));
    REQUIRE("file" == getFileDir("file/"));
    REQUIRE("" == getFileDir("/file"));
}

TEST_CASE("stringuntils replaceAll 1")
{
    std::string str1 = "";
    std::string str2 = "";
    std::string str3 = "";

    REQUIRE("" == replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "";
    str3 = "";
    REQUIRE("this is test line" == replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "is ";
    str3 = "";
    REQUIRE("thtest line" == replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "";
    str3 = "1";
    REQUIRE("this is test line" == replaceAll(str1, str2, str3));
}

TEST_CASE("stringuntils replaceRecursiveAll 1")
{
    std::string str;
    str = "";
    replaceRecursiveAll(str, "line", '.');
    REQUIRE(str == "");
    str = "test line";
    replaceRecursiveAll(str, "line", '.');
    REQUIRE(str == "test .");
    str = "11112222";
    replaceRecursiveAll(str, "11", '1');
    REQUIRE(str == "12222");
    str = "122221";
    replaceRecursiveAll(str, "11", '1');
    REQUIRE(str == "122221");
    str = "1222211";
    replaceRecursiveAll(str, "11", '1');
    REQUIRE(str == "122221");
    str = "11112222";
    replaceRecursiveAll(str, "112", '1');
    REQUIRE(str == "111222");
    str = "111122224";
    replaceRecursiveAll(str, "112", '1');
    REQUIRE(str == "1112224");
    str = "3111122224";
    replaceRecursiveAll(str, "112", '1');
    REQUIRE(str == "31112224");
    str = "121212";
    replaceRecursiveAll(str, "12", '1');
    REQUIRE(str == "111");
    str = "1121212";
    replaceRecursiveAll(str, "12", '1');
    REQUIRE(str == "1111");
    str = "11212122";
    replaceRecursiveAll(str, "12", '1');
    REQUIRE(str == "1111");
    str = "112121222";
    replaceRecursiveAll(str, "12", '1');
    REQUIRE(str == "1111");
    str = "112211221122";
    replaceRecursiveAll(str, "12", '1');
    REQUIRE(str == "111111");
}

TEST_CASE("stringuntils getBoolFromString 1")
{
    REQUIRE(getBoolFromString("true"));
    REQUIRE(getBoolFromString("yes"));
    REQUIRE(getBoolFromString("on"));
    REQUIRE(!getBoolFromString("false"));
    REQUIRE(!getBoolFromString("no"));
    REQUIRE(!getBoolFromString("off"));
    REQUIRE(getBoolFromString("1"));
    REQUIRE(!getBoolFromString("0"));
    REQUIRE(getBoolFromString("2"));

    REQUIRE(getBoolFromString(" true"));
    REQUIRE(getBoolFromString("yes "));
    REQUIRE(getBoolFromString(" on"));
    REQUIRE(!getBoolFromString("false "));
    REQUIRE(!getBoolFromString(" no"));
    REQUIRE(!getBoolFromString("off "));
    REQUIRE(getBoolFromString(" 1"));
    REQUIRE(!getBoolFromString("0 "));
    REQUIRE(getBoolFromString(" 2"));

    REQUIRE(getBoolFromString("tRue "));
    REQUIRE(getBoolFromString(" Yes"));
    REQUIRE(getBoolFromString("ON "));
    REQUIRE(!getBoolFromString(" fALse"));
    REQUIRE(!getBoolFromString("nO "));
    REQUIRE(!getBoolFromString(" oFF"));
}

TEST_CASE("stringuntils parseBoolean 1")
{
    REQUIRE(parseBoolean("true") == 1);
    REQUIRE(parseBoolean("yes") == 1);
    REQUIRE(parseBoolean("on") == 1);
    REQUIRE(parseBoolean("false") == 0);
    REQUIRE(parseBoolean("no") == 0);
    REQUIRE(parseBoolean("off") == 0);
    REQUIRE(parseBoolean("1") == 1);
    REQUIRE(parseBoolean("0") == 0);
    REQUIRE(parseBoolean("2") == -1);
    REQUIRE(parseBoolean("test") == -1);
    REQUIRE(parseBoolean("") == -1);

    REQUIRE(parseBoolean("true ") == 1);
    REQUIRE(parseBoolean(" yes") == 1);
    REQUIRE(parseBoolean("on ") == 1);
    REQUIRE(parseBoolean(" false") == 0);
    REQUIRE(parseBoolean("no ") == 0);
    REQUIRE(parseBoolean(" off") == 0);
    REQUIRE(parseBoolean("1 ") == 1);
    REQUIRE(parseBoolean(" 0") == 0);
    REQUIRE(parseBoolean("2 ") == -1);
    REQUIRE(parseBoolean(" test") == -1);
    REQUIRE(parseBoolean(" ") == -1);

    REQUIRE(parseBoolean(" tRue") == 1);
    REQUIRE(parseBoolean("Yes ") == 1);
    REQUIRE(parseBoolean(" ON") == 1);
    REQUIRE(parseBoolean("FaLse ") == 0);
    REQUIRE(parseBoolean(" nO") == 0);
    REQUIRE(parseBoolean("oFf ") == 0);
    REQUIRE(parseBoolean(" tEst") == -1);
}

TEST_CASE("stringuntils splitToIntSet 1")
{
    std::set<int> tokens;
    splitToIntSet(tokens, "", ',');
    REQUIRE(tokens.empty() == true);

    tokens.clear();
    splitToIntSet(tokens, "10z,aa,-1", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find(10) != tokens.end());
    REQUIRE(tokens.find(0) != tokens.end());
    REQUIRE(tokens.find(-1) != tokens.end());

    tokens.clear();
    splitToIntSet(tokens, "10,2,30", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find(10) != tokens.end());
    REQUIRE(tokens.find(2) != tokens.end());
    REQUIRE(tokens.find(30) != tokens.end());

    tokens.clear();
    splitToIntSet(tokens, "10,2,30,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find(10) != tokens.end());
    REQUIRE(tokens.find(2) != tokens.end());
    REQUIRE(tokens.find(30) != tokens.end());

    tokens.clear();
    splitToIntSet(tokens, "10,2;30", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens.find(10) != tokens.end());
    REQUIRE(tokens.find(2) != tokens.end());

    tokens.clear();
    splitToIntSet(tokens, "10;20;30", ';');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find(10) != tokens.end());
    REQUIRE(tokens.find(20) != tokens.end());
    REQUIRE(tokens.find(30) != tokens.end());
}

TEST_CASE("stringuntils splitToIntList 1")
{
    std::list<int> tokens;
    tokens = splitToIntList("", ',');
    REQUIRE(tokens.empty() == true);

    tokens.clear();
    tokens = splitToIntList("10z,a,-1", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.front() == 10);
    tokens.pop_front();
    REQUIRE(tokens.front() == 0);
    tokens.pop_front();
    REQUIRE(tokens.front() == -1);
    tokens.pop_front();

    tokens.clear();
    tokens = splitToIntList("10,2,30", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.front() == 10);
    tokens.pop_front();
    REQUIRE(tokens.front() == 2);
    tokens.pop_front();
    REQUIRE(tokens.front() == 30);
    tokens.pop_front();

    tokens.clear();
    tokens = splitToIntList("10,2,30,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.front() == 10);
    tokens.pop_front();
    REQUIRE(tokens.front() == 2);
    tokens.pop_front();
    REQUIRE(tokens.front() == 30);
    tokens.pop_front();

    tokens.clear();
    tokens = splitToIntList("10,2;30", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens.front() == 10);
    tokens.pop_front();
    REQUIRE(tokens.front() == 2);
    tokens.pop_front();

    tokens.clear();
    tokens = splitToIntList("10;20;30", ';');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.front() == 10);
    tokens.pop_front();
    REQUIRE(tokens.front() == 20);
    tokens.pop_front();
    REQUIRE(tokens.front() == 30);
    tokens.pop_front();
}

TEST_CASE("stringuntils splitToStringSet 1")
{
    std::set<std::string> tokens;
    splitToStringSet(tokens, "", ',');
    REQUIRE(tokens.empty() == true);

    tokens.clear();
    splitToStringSet(tokens, "10q,2w,30e", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find("10q") != tokens.end());
    REQUIRE(tokens.find("2w") != tokens.end());
    REQUIRE(tokens.find("30e") != tokens.end());

    tokens.clear();
    splitToStringSet(tokens, "10q,2w,30e,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find("10q") != tokens.end());
    REQUIRE(tokens.find("2w") != tokens.end());
    REQUIRE(tokens.find("30e") != tokens.end());

    tokens.clear();
    splitToStringSet(tokens, "10q,,30e", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens.find("10q") != tokens.end());
    REQUIRE(tokens.find("30e") != tokens.end());

    tokens.clear();
    splitToStringSet(tokens, "10q,2w,30e,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find("10q") != tokens.end());
    REQUIRE(tokens.find("2w") != tokens.end());
    REQUIRE(tokens.find("30e") != tokens.end());

    tokens.clear();
    splitToStringSet(tokens, "10w,2w;30e", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens.find("10w") != tokens.end());
    REQUIRE(tokens.find("2w;30e") != tokens.end());

    tokens.clear();
    splitToStringSet(tokens, "10q;20w;30e", ';');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens.find("10q") != tokens.end());
    REQUIRE(tokens.find("20w") != tokens.end());
    REQUIRE(tokens.find("30e") != tokens.end());
}

TEST_CASE("stringuntils splitToIntVector 1")
{
    std::vector<int> tokens;
    splitToIntVector(tokens, "", ',');
    REQUIRE(tokens.empty() == true);

    tokens.clear();
    splitToIntVector(tokens, "10,2,30", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 2);
    REQUIRE(tokens[2] == 30);

    tokens.clear();
    splitToIntVector(tokens, "10,2a,z30", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 2);
    REQUIRE(tokens[2] == 0);

    tokens.clear();
    splitToIntVector(tokens, "10,2,30,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 2);
    REQUIRE(tokens[2] == 30);

    tokens.clear();
    splitToIntVector(tokens, "10,,30", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 30);

    tokens.clear();
    splitToIntVector(tokens, "10,2;30", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 2);

    tokens.clear();
    splitToIntVector(tokens, "10;20;30", ';');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == 10);
    REQUIRE(tokens[1] == 20);
    REQUIRE(tokens[2] == 30);
}

TEST_CASE("stringuntils splitToStringVector 1")
{
    std::vector<std::string> tokens;
    splitToStringVector(tokens, "", ',');
    REQUIRE(tokens.empty() == true);

    tokens.clear();
    splitToStringVector(tokens, "t,line,zz", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "t");
    REQUIRE(tokens[1] == "line");
    REQUIRE(tokens[2] == "zz");

    tokens.clear();
    splitToStringVector(tokens, "t,line,zz,", ',');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "t");
    REQUIRE(tokens[1] == "line");
    REQUIRE(tokens[2] == "zz");

    tokens.clear();
    splitToStringVector(tokens, "t,,zz", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "t");
    REQUIRE(tokens[1] == "zz");

    tokens.clear();
    splitToStringVector(tokens, "10,a2;30", ',');
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "10");
    REQUIRE(tokens[1] == "a2;30");

    tokens.clear();
    splitToStringVector(tokens, "a10;b;3line", ';');
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "a10");
    REQUIRE(tokens[1] == "b");
    REQUIRE(tokens[2] == "3line");
}

TEST_CASE("stringuntils replaceSpecialChars 1")
{
    std::string str;

    str = "";
    replaceSpecialChars(str);
    REQUIRE("" == str);

    str = "test";
    replaceSpecialChars(str);
    REQUIRE("test" == str);

    str = "&";
    replaceSpecialChars(str);
    REQUIRE("&" == str);

    str = "&1";
    replaceSpecialChars(str);
    REQUIRE("&1" == str);

    str = "&33";
    replaceSpecialChars(str);
    REQUIRE("&33" == str);

    str = "&33;";
    replaceSpecialChars(str);
    REQUIRE("!" == str);

    str = "&33z;";
    replaceSpecialChars(str);
    REQUIRE("&33z;" == str);

    str = "1&33;";
    replaceSpecialChars(str);
    REQUIRE("1!" == str);

    str = "&33;2";
    replaceSpecialChars(str);
    REQUIRE("!2" == str);

    str = "&33;&";
    replaceSpecialChars(str);
    REQUIRE("!&" == str);

    str = "test line&33;";
    replaceSpecialChars(str);
    REQUIRE("test line!" == str);
}

TEST_CASE("stringuntils normalize 1")
{
    REQUIRE(normalize("") == "");
    REQUIRE(normalize("test") == "test");
    REQUIRE(normalize("Test") == "test");
    REQUIRE(normalize(" test line") == "test line");
    REQUIRE(normalize("test line ") == "test line");
    REQUIRE(normalize(" tEst line") == "test line");
    REQUIRE(normalize("test lIne ") == "test line");
}

TEST_CASE("stringuntils combineDye 1")
{
    REQUIRE("" == combineDye("", ""));
    REQUIRE("test" == combineDye("test", ""));
    REQUIRE("|line" == combineDye("", "line"));
    REQUIRE("test|line" == combineDye("test", "line"));
    REQUIRE("|line" == combineDye("|w", "line"));
    REQUIRE("aaa|line" == combineDye("aaa|w", "line"));
    REQUIRE("test|line" == combineDye("test|w", "line"));
}

TEST_CASE("stringuntils combineDye 2")
{
    REQUIRE("" == combineDye2("", ""));
    REQUIRE("test" == combineDye2("test", ""));
    REQUIRE("test" == combineDye2("test", "W"));
    REQUIRE("" == combineDye2("", "line"));
    REQUIRE("test.xml" == combineDye2("test.xml", "123"));
    REQUIRE("test.xml|#43413d,59544f,7a706c" ==
        combineDye2("test.xml|#43413d,59544f,7a706c", ""));
    REQUIRE("test.xml|#43413d,59544f,7a706c:W;" ==
        combineDye2("test.xml|#43413d,59544f,7a706c", "W"));
    REQUIRE("test.xml|#43413d,59544f,7a706c:W;#123456:B;" ==
        combineDye2("test.xml|#43413d,59544f,7a706c;#123456", "W;B"));
}

TEST_CASE("stringuntils combineDye 3")
{
    REQUIRE("" == combineDye3("", ""));
    REQUIRE("test" == combineDye3("test", ""));
    REQUIRE("" == combineDye3("", "line"));
    REQUIRE("test.xml|123" == combineDye3("test.xml", "123"));
    REQUIRE("test.xml|#43413d,59544f,7a706c" ==
        combineDye3("test.xml|#43413d,59544f,7a706c", ""));
    REQUIRE("test.xml|#43413d,59544f,7a706c:W;" ==
        combineDye3("test.xml|#43413d,59544f,7a706c", "W"));
    REQUIRE("test.xml|#43413d,59544f,7a706c:W;#123456:B;" ==
        combineDye3("test.xml|#43413d,59544f,7a706c;#123456", "W;B"));
}

TEST_CASE("stringuntils packList 1")
{
    std::list <std::string> list;
    REQUIRE("" == packList(list));

    list.push_back("");
    REQUIRE("|" == packList(list));

    list.clear();
    list.push_back("test");
    REQUIRE("test" == packList(list));

    list.push_back("line");
    REQUIRE("test|line" == packList(list));

    list.push_back("2");
    REQUIRE("test|line|2" == packList(list));

    list.clear();
    list.push_back("|test");
    list.push_back("line");
    REQUIRE("|test|line" == packList(list));
}

TEST_CASE("stringuntils stringToHexPath 1")
{
    std::string str;

    str = "";
    REQUIRE("" == stringToHexPath(str));

    str = "a";
    REQUIRE("%61/" == stringToHexPath(str));

    str = "ab";
    REQUIRE("%61/%62" == stringToHexPath(str));

    str = "abc";
    REQUIRE("%61/%62%63" == stringToHexPath(str));

    str = "abcd";
    REQUIRE("%61/%62%63%64" == stringToHexPath(str));
}

TEST_CASE("stringuntils deleteCharLeft 1")
{
    std::string str;
    unsigned int pos = 0;

    str = "";
    deleteCharLeft(str, nullptr);
    REQUIRE("" == str);

    str = "test line";
    pos = 4;
    deleteCharLeft(str, &pos);
    REQUIRE("tes line" == str);

    str = "тест line";
    pos = 8;
    deleteCharLeft(str, &pos);
    REQUIRE("тес line" == str);

    str = "test line\x0";
    pos = 4;
    deleteCharLeft(str, &pos);
    REQUIRE("tes line\x0" == str);
}

TEST_CASE("stringuntils findLast 1")
{
    std::string str;

    str = "";
    REQUIRE(findLast(str, ""));

    str = "test line";
    REQUIRE(findLast(str, "line"));

    str = "test line";
    REQUIRE(!findLast(str, "lin"));
}

TEST_CASE("stringuntils findFirst 1")
{
    std::string str;

    str = "";
    REQUIRE(findFirst(str, ""));

    str = "test line";
    REQUIRE(findFirst(str, "test"));

    str = "test";
    REQUIRE(findFirst(str, "test line") == false);

    str = "test line";
    REQUIRE(findFirst(str, "est") == false);
}

TEST_CASE("stringuntils findCutLast 1")
{
    std::string str;

    str = "";
    REQUIRE(findCutLast(str, ""));
    REQUIRE("" == str);

    str = "test line";
    REQUIRE(findCutLast(str, "line"));
    REQUIRE("test " == str);

    str = "test line";
    REQUIRE(findCutLast(str, "lin") == false);
    REQUIRE("test line" == str);

    str = "test";
    REQUIRE(findCutLast(str, "test line") == false);
    REQUIRE("test" == str);
}

TEST_CASE("stringuntils CutLast 1")
{
    std::string str;

    str = "";
    cutLast(str, "");
    REQUIRE("" == str);

    str = "test line";
    cutLast(str, "line");
    REQUIRE("test " == str);

    str = "test line";
    cutLast(str, "lin");
    REQUIRE("test line" == str);

    str = "test";
    cutLast(str, "test line");
    REQUIRE("test" == str);
}

TEST_CASE("stringuntils findCutFirst 1")
{
    std::string str;

    str = "";
    REQUIRE(findCutFirst(str, ""));
    REQUIRE("" == str);

    str = "test line";
    REQUIRE(findCutFirst(str, "test"));
    REQUIRE(" line" == str);

    str = "test line";
    REQUIRE(findCutFirst(str, "est") == false);
    REQUIRE("test line" == str);

    str = "test";
    REQUIRE(findCutFirst(str, "test line") == false);
    REQUIRE("test" == str);
}

TEST_CASE("stringuntils cutFirst 1")
{
    std::string str;

    str = "";
    cutFirst(str, "");
    REQUIRE("" == str);

    str = "test line";
    cutFirst(str, "test");
    REQUIRE(" line" == str);

    str = "test line";
    cutFirst(str, "est");
    REQUIRE("test line" == str);

    str = "test";
    cutFirst(str, "test line");
    REQUIRE("test" == str);
}

TEST_CASE("stringuntils removeProtocol 1")
{
    std::string str;

    str = "";
    REQUIRE("" == removeProtocol(str));

    str = "http://";
    REQUIRE("" == removeProtocol(str));

    str = "http://test";
    REQUIRE("test" == removeProtocol(str));
}

TEST_CASE("stringuntils strStartWith 1")
{
    REQUIRE(strStartWith("", ""));
    REQUIRE(!strStartWith("", "1"));
    REQUIRE(strStartWith("test line", "test"));
    REQUIRE(strStartWith("test line", "test line"));
    REQUIRE(!strStartWith("test line", "est"));
}

TEST_CASE("stringuntils encodeLinkText")
{
    std::string str;

    str = encodeLinkText("test line");
    REQUIRE(str == "test line");
    str = encodeLinkText("test|line");
    REQUIRE(str == "test\342\235\230line");
    str = encodeLinkText("test||line");
    REQUIRE(str == "test\342\235\230\342\235\230line");
}

TEST_CASE("stringuntils decodeLinkText")
{
    std::string str;

    str = encodeLinkText("test|line");
    REQUIRE(str == "test\342\235\230line");
    str = decodeLinkText(str);
    REQUIRE(str == "test|line");
}

TEST_CASE("stringuntils isDigit")
{
    std::string str;

    REQUIRE_FALSE(isDigit(""));
    REQUIRE(isDigit("1"));
    REQUIRE(isDigit("123"));
    REQUIRE_FALSE(isDigit("+123"));
    REQUIRE_FALSE(isDigit("-123"));
    REQUIRE_FALSE(isDigit("1.23"));
    REQUIRE_FALSE(isDigit("12-34"));
}

TEST_CASE("stringuntils findAny")
{
    std::string str;

    REQUIRE(findAny("test line", ",", 0) == std::string::npos);
    REQUIRE(findAny("test line", " ", 0) == 4U);
    REQUIRE(findAny("test, line", ", ", 0) == 4U);
    REQUIRE(findAny("test ,line", ", ", 0) == 4U);
    REQUIRE(findAny("test, line", " ,", 2) == 4U);
    REQUIRE(findAny("test ,line", " ,", 3) == 4U);
    REQUIRE(findAny("\"one\",,two, tree", ",", 5) == 5U);
}

TEST_CASE("stringuntils escapeString")
{
    REQUIRE(escapeString("") == "\"\"");
    REQUIRE(escapeString("1") == "\"1\"");
    REQUIRE(escapeString(" ") == "\" \"");
    REQUIRE(escapeString("\"") == "\"\\\"\"");
    REQUIRE(escapeString("123") == "\"123\"");
    REQUIRE(escapeString("12\"3") == "\"12\\\"3\"");
    REQUIRE(escapeString("12\"\"3") == "\"12\\\"\\\"3\"");
    REQUIRE(escapeString("\"123\"") == "\"\\\"123\\\"\"");
    REQUIRE(escapeString("\\") == "\"\\\"");
    REQUIRE(escapeString("12\\3") == "\"12\\3\"");
}

TEST_CASE("stringuntils sanitizePath")
{
    std::string path;
    path = "";
    sanitizePath(path);
    REQUIRE(path == "");
    path = "/";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "/\\";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "\\/";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "//";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "///";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "//\\/";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "///\\";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "\\";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "\\\\";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "\\/\\";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "\\\\/";
    sanitizePath(path);
    REQUIRE(path == "/");
    path = "test";
    sanitizePath(path);
    REQUIRE(path == "test");
    path = "./test";
    sanitizePath(path);
    REQUIRE(path == "./test");
    path = "test line";
    sanitizePath(path);
    REQUIRE(path == "test line");
    path = "dir/test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "/dir/test";
    sanitizePath(path);
    REQUIRE(path == "/dir/test");
    path = "dir//test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "dir///test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "dir///\\test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "dir/\\//test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "dir\\test";
    sanitizePath(path);
    REQUIRE(path == "dir/test");
    path = "dir/test/";
    sanitizePath(path);
    REQUIRE(path == "dir/test/");
    path = "dir/test\\";
    sanitizePath(path);
    REQUIRE(path == "dir/test/");
    path = "/very\\long/dir\\with\\sepa/ra/tors";
    sanitizePath(path);
    REQUIRE(path == "/very/long/dir/with/sepa/ra/tors");
    path = "/very\\long/dir\\\\with\\sepa//ra/tors";
    sanitizePath(path);
    REQUIRE(path == "/very/long/dir/with/sepa/ra/tors");
}

TEST_CASE("stringuntils secureChatCommand")
{
    std::string str;
    secureChatCommand(str);
    REQUIRE(str == "");
    str = "test";
    secureChatCommand(str);
    REQUIRE(str == "test");
    str = "test line";
    secureChatCommand(str);
    REQUIRE(str == "test line");
    str = "/test";
    secureChatCommand(str);
    REQUIRE(str == "_/test");
    str = "@test";
    secureChatCommand(str);
    REQUIRE(str == "_@test");
    str = "#test";
    secureChatCommand(str);
    REQUIRE(str == "_#test");
}

TEST_CASE("stringuntils timeDiffToString")
{
    REQUIRE(timeDiffToString(60 * 60 * 24 * 7) == "1 week");
    REQUIRE(timeDiffToString(60 * 60 * 24 * 7 * 2 +
        60 * 60 * 24 * 3
        ) == "2 weeks, 3 days");
    REQUIRE(timeDiffToString(60 * 60 * 24 * 7 * 2 +
        60 * 60 * 24 * 3 +
        60 * 60 * 4
        ) == "2 weeks, 3 days, 4 hours");
    REQUIRE(timeDiffToString(60 * 60 * 24 * 7 * 2 +
        60 * 60 * 24 * 3 +
        60 * 60 * 4 +
        60 * 7
        ) == "2 weeks, 3 days, 4 hours, 7 minutes");
    REQUIRE(timeDiffToString(60 * 60 * 24 * 7 * 2 +
        60 * 60 * 24 * 3 +
        60 * 60 * 4 +
        60 * 7 +
        10
        ) == "2 weeks, 3 days, 4 hours, 7 minutes, 10 seconds");
    REQUIRE(timeDiffToString(5) == "5 seconds");
    REQUIRE(timeDiffToString(0) == "0 seconds");
    REQUIRE(timeDiffToString(60 * 60 * 24 * 3
        ) == "3 days");
    REQUIRE(timeDiffToString(60 * 60 * 4
        ) == "4 hours");
    REQUIRE(timeDiffToString(60 * 7
        ) == "7 minutes");
}

TEST_CASE("stringuntils replaceItemLinks")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);
    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    ItemDB::NamedItemInfos &namedInfos = ItemDB::getNamedItemInfosTest();
    ItemDB::ItemInfos &infos = ItemDB::getItemInfosTest();
    paths.setDefaultValues(getPathsDefaults());
    ItemInfo *info = new ItemInfo;
    info->setId(123456);
    info->setName("test name 1");
    namedInfos["test name 1"] = info;
    infos[123456] = info;

    info = new ItemInfo;
    info->setId(123);
    info->setName("test name 2");
    namedInfos["test name 2"] = info;
    namedInfos["qqq"] = info;
    infos[123] = info;

    std::string str;

    SECTION("empty")
    {
        str = "test line";
        replaceItemLinks(str);
        REQUIRE(str == "test line");

        str = "";
        replaceItemLinks(str);
        REQUIRE(str == "");

        str = "[]";
        replaceItemLinks(str);
        REQUIRE(str == "[]");

        str = "[qqq]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123|qqq@@]");

        str = "[,]";
        replaceItemLinks(str);
        REQUIRE(str == "[,]");

        str = "[, ]";
        replaceItemLinks(str);
        REQUIRE(str == "[, ]");
    }

    SECTION("simple")
    {
        str = "[test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123456|test name 1@@]");

        str = "text1 [test name 1] text2";
        replaceItemLinks(str);
        REQUIRE(str == "text1 [@@123456|test name 1@@] text2");

        str = "[test name 1][test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123456|test name 1@@][@@123456|test name 1@@]");

        str = "[test name 1] [test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123456|test name 1@@] [@@123456|test name 1@@]");

        str = "test1 [test name 1]test2[test name 1] test3";
        replaceItemLinks(str);
        REQUIRE(str == "test1 [@@123456|test name 1@@]test2"
            "[@@123456|test name 1@@] test3");

// failing because assert
//        str = "[test name 1] [no link]";
//        replaceItemLinks(str);
//        REQUIRE(str == "[@@123456|test name 1@@] [no link]");

        str = "[test name 1,test name 2]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123456,123|@@]");

        str = "[test name 1, test name 2 ]";
        replaceItemLinks(str);
        REQUIRE(str == "[@@123456,123|@@]");
    }

    SECTION("broken")
    {
        str = "[";
        replaceItemLinks(str);
        REQUIRE(str == "[");

        str = "]";
        replaceItemLinks(str);
        REQUIRE(str == "]");

        str = "][";
        replaceItemLinks(str);
        REQUIRE(str == "][");

        str = "]]";
        replaceItemLinks(str);
        REQUIRE(str == "]]");

        str = "]t";
        replaceItemLinks(str);
        REQUIRE(str == "]t");

        str = "t[";
        replaceItemLinks(str);
        REQUIRE(str == "t[");

        str = "t]";
        replaceItemLinks(str);
        REQUIRE(str == "t]");

        str = "[[[";
        replaceItemLinks(str);
        REQUIRE(str == "[[[");

        str = "[[]";
        replaceItemLinks(str);
        REQUIRE(str == "[[]");

        str = "[[t";
        replaceItemLinks(str);
        REQUIRE(str == "[[t");

        str = "[][";
        replaceItemLinks(str);
        REQUIRE(str == "[][");

        str = "[]]";
        replaceItemLinks(str);
        REQUIRE(str == "[]]");

        str = "[]t";
        replaceItemLinks(str);
        REQUIRE(str == "[]t");

        str = "[t[";
        replaceItemLinks(str);
        REQUIRE(str == "[t[");

// failing because assert
//        str = "[t]";
//        replaceItemLinks(str);
//        REQUIRE(str == "[t]");

        str = "t[[";
        replaceItemLinks(str);
        REQUIRE(str == "t[[");

        str = "t[]";
        replaceItemLinks(str);
        REQUIRE(str == "t[]");

        str = "t[[";
        replaceItemLinks(str);
        REQUIRE(str == "t[[");

        str = "]]]";
        replaceItemLinks(str);
        REQUIRE(str == "]]]");
    }

    SECTION("broken2")
    {
        str = "[][]";
        replaceItemLinks(str);
        REQUIRE(str == "[][]");

        str = "[[]]";
        replaceItemLinks(str);
        REQUIRE(str == "[[]]");

        str = "][[]";
        replaceItemLinks(str);
        REQUIRE(str == "][[]");
    }

    SECTION("broken3")
    {
        str = "[[test name 1]]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123456|test name 1@@]]");

        str = "[[test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123456|test name 1@@]");

        str = "[[qqq] name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123|qqq@@] name 1]");

        str = "[[test name 1]test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123456|test name 1@@]test name 1]");

        str = "[[test name 1[]test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[test name 1[]test name 1]");

        str = "[[test name 1],test name2,test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123456|test name 1@@],test name2,test name 1]");

        str = "[[ test name 1], test name2,test name 1 ]";
        replaceItemLinks(str);
        REQUIRE(str == "[[@@123456|test name 1@@], test name2,test name 1 ]");

        str = "[[test name 1,test name2[]test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[test name 1,test name2[]test name 1]");

        str = "[[test name 1 ,test name2[] test name 1]";
        replaceItemLinks(str);
        REQUIRE(str == "[[test name 1 ,test name2[] test name 1]");
    }
    ResourceManager::deleteInstance();
    delete2(logger);
    delete_all(infos);
    infos.clear();
    namedInfos.clear();
//    VirtFs::deinit();
}
