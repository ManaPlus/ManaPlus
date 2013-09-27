/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "utils/stringutils.h"

#include "gtest/gtest.h"

#include <list>
#include <string>
#include <vector>

#include "debug.h"

TEST(stringuntils, trim1)
{
    std::string str = "str";
    EXPECT_EQ("str", trim(str));

    str = " str";
    EXPECT_EQ("str", trim(str));
    EXPECT_EQ("str", trim(str));

    str = " str   this IS Long Стринг "
        "~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   ";
    EXPECT_EQ("str   this IS Long Стринг ~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?",
        trim(str));

    str = "";
    EXPECT_EQ("", trim(str));
}

TEST(stringuntils, toLower1)
{
    std::string str = "str";
    EXPECT_EQ("str", toLower(str));

    str = " StR";
    EXPECT_EQ(" str", toLower(str));

    str = " str   this IS Long  "
        "~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   ";

    EXPECT_EQ(" str   this is long  ~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?   ",
        toLower(str));

    str = "";
    EXPECT_EQ("", toLower(str));
}

TEST(stringuntils, toUpper1)
{
    std::string str = "str";
    EXPECT_EQ("STR", toUpper(str));

    str = " StR";
    EXPECT_EQ(" STR", toUpper(str));

    str = " str   this IS Long  "
        "~!@#$%^&*()_+`-=[]\\{}|;':,./<>?   ";

    EXPECT_EQ(" STR   THIS IS LONG  ~!@#$%^&*()_+`-=[]\\{}|;':,./<>?   ",
        toUpper(str));

    str = "";
    EXPECT_EQ("", toUpper(str));
}

TEST(stringuntils, atox1)
{
    std::string str = "0x10";
    EXPECT_EQ(16, atox(str));

    str = "0x0";
    EXPECT_EQ(0, atox(str));

    str = "0x1";
    EXPECT_EQ(1, atox(str));

    str = "0x0x0x0x0x0x0x0";
    EXPECT_EQ(0, atox(str));

    str = "0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    atox(str);

    str = "";
    int k = atox(str);

    str = "0";
    k = atox(str);

    str = "0x";
    k = atox(str);
}

TEST(stringuntils, ipToString1)
{
    EXPECT_EQ("0.0.0.0", std::string(ipToString(0)));
    EXPECT_EQ("219.255.210.73", std::string(ipToString(1238564827)));
}

TEST(stringuntils, removeColors1)
{
    EXPECT_EQ("", removeColors(""));
    EXPECT_EQ("#", removeColors("#"));
    EXPECT_EQ("##", removeColors("##"));
    EXPECT_EQ("", removeColors("##1"));
    EXPECT_EQ("2", removeColors("##12"));
    EXPECT_EQ("1##", removeColors("1##"));
    EXPECT_EQ("1", removeColors("1##2"));
    EXPECT_EQ("13", removeColors("1##23"));
    EXPECT_EQ("#1#2", removeColors("#1#2"));
    EXPECT_EQ("#1", removeColors("#1##2"));
}

TEST(stringuntils, compareStrI1)
{
    std::string str1 = "";
    std::string str2 = "";
    EXPECT_EQ(0, compareStrI(str1, str2));

    str1 = "test";
    str2 = "test";
    EXPECT_EQ(0, compareStrI(str1, str2));

    str1 = "test";
    str2 = "test1";
    EXPECT_GT(0, compareStrI(str1, str2));

    str1 = "test";
    str2 = "aest1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    EXPECT_LT(0, compareStrI(str1, str2));

    str1 = "testaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    str2 = "testaa";
    EXPECT_LT(0, compareStrI(str1, str2));
}

TEST(stringuntils, isWordSeparator1)
{
    EXPECT_TRUE(isWordSeparator(' '));
    EXPECT_TRUE(isWordSeparator(','));
    EXPECT_TRUE(isWordSeparator('.'));
    EXPECT_TRUE(isWordSeparator('\"'));
    EXPECT_TRUE(!isWordSeparator(0));
    EXPECT_TRUE(!isWordSeparator('a'));
    EXPECT_TRUE(!isWordSeparator('-'));
}

TEST(stringuntils, findSameSubstring)
{
    std::string str1 = "";
    std::string str2 = "";

    EXPECT_EQ("", findSameSubstring("", ""));

    str1 = "test line";
    str2 = "test line";
    EXPECT_EQ("test line", findSameSubstring(str1, str2));

    str1 = "test li";
    str2 = "test line";
    EXPECT_EQ("test li", findSameSubstring(str1, str2));

    str1 = "test li";
    str2 = "est li";
    EXPECT_EQ("", findSameSubstring(str1, str2));
}

TEST(stringuntils, findSameSubstringI)
{
    std::string str1 = "";
    std::string str2 = "";

    EXPECT_EQ("", findSameSubstringI("", ""));

    str1 = "tEst line";
    str2 = "tesT line";
    EXPECT_EQ("tEst line", findSameSubstringI(str1, str2));

    str1 = "test Li";
    str2 = "test lINe";
    EXPECT_EQ("test Li", findSameSubstringI(str1, str2));

    str1 = "teSt li";
    str2 = "est li";
    EXPECT_EQ("", findSameSubstringI(str1, str2));
}

TEST(stringuntils, findI1)
{
    EXPECT_EQ(0, findI("", ""));
    EXPECT_EQ(std::string::npos, findI("test", "line"));
    EXPECT_EQ(0, findI("test line", "t"));
    EXPECT_EQ(0, findI("test line", "te"));
    EXPECT_EQ(3, findI("test line", "t l"));
}

TEST(stringuntils, findI2)
{
    std::vector <std::string> vect1;
    vect1.push_back("test");
    vect1.push_back("line");
    vect1.push_back("qwe");

    EXPECT_EQ(std::string::npos, findI("", vect1));
    EXPECT_EQ(0, findI("test", vect1));
    EXPECT_EQ(0, findI("tesT lIne", vect1));
    EXPECT_EQ(5, findI("teoT line", vect1));
    EXPECT_EQ(std::string::npos, findI("zzz", vect1));
}

TEST(stringuntils, encodeStr1)
{
    std::string str = encodeStr(10, 1);
    EXPECT_EQ(10, decodeStr(str));

    str = encodeStr(10, 2);
    EXPECT_EQ(10, decodeStr(str));

    str = encodeStr(100, 3);
    EXPECT_EQ(100, decodeStr(str));

    str = encodeStr(1000, 4);
    EXPECT_EQ(1000, decodeStr(str));
}

TEST(stringuntils, extractNameFromSprite1)
{
    EXPECT_EQ("", extractNameFromSprite(""));
    EXPECT_EQ("test", extractNameFromSprite("test"));
    EXPECT_EQ("test", extractNameFromSprite("test.qwe"));
    EXPECT_EQ("line", extractNameFromSprite("test/line.zzz"));
}

TEST(stringuntils, removeSpriteIndex1)
{
    EXPECT_EQ("", removeSpriteIndex(""));
    EXPECT_EQ("test", removeSpriteIndex("test"));
    EXPECT_EQ("test", removeSpriteIndex("test[1]"));
    EXPECT_EQ("line", removeSpriteIndex("test/line[12]"));
}

TEST(stringuntils, getFileName1)
{
    EXPECT_EQ("", getFileName(""));
    EXPECT_EQ("file", getFileName("file"));
    EXPECT_EQ("file", getFileName("test/file1\\file"));
    EXPECT_EQ("file", getFileName("test\\file1/file"));
    EXPECT_EQ("", getFileName("file/"));
    EXPECT_EQ("file", getFileName("/file"));
}

TEST(stringuntils, getFileDir1)
{
    EXPECT_EQ("", getFileDir(""));
    EXPECT_EQ("file", getFileDir("file"));
    EXPECT_EQ("test/file1", getFileDir("test/file1\\file"));
    EXPECT_EQ("test\\file1", getFileDir("test\\file1/file"));
    EXPECT_EQ("file", getFileDir("file/"));
    EXPECT_EQ("", getFileDir("/file"));
}

TEST(stringuntils, replaceAll1)
{
    std::string str1 = "";
    std::string str2 = "";
    std::string str3 = "";

    EXPECT_EQ("", replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "";
    str3 = "";
    EXPECT_EQ("this is test line", replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "is ";
    str3 = "";
    EXPECT_EQ("thtest line", replaceAll(str1, str2, str3));

    str1 = "this is test line";
    str2 = "";
    str3 = "1";
    EXPECT_EQ("this is test line", replaceAll(str1, str2, str3));
}

TEST(stringuntils, getBoolFromString1)
{
    EXPECT_TRUE(getBoolFromString("true"));
    EXPECT_TRUE(!getBoolFromString("false"));
    EXPECT_TRUE(getBoolFromString("1"));
    EXPECT_TRUE(!getBoolFromString("0"));
}

TEST(stringuntils, replaceSpecialChars1)
{
    std::string str;

    str = "";
    replaceSpecialChars(str);
    EXPECT_EQ("", str);

    str = "test";
    replaceSpecialChars(str);
    EXPECT_EQ("test", str);

    str = "&";
    replaceSpecialChars(str);
    EXPECT_EQ("&", str);

    str = "&1";
    replaceSpecialChars(str);
    EXPECT_EQ("&1", str);

    str = "&33";
    replaceSpecialChars(str);
    EXPECT_EQ("&33", str);

    str = "&33;";
    replaceSpecialChars(str);
    EXPECT_EQ("!", str);

    str = "1&33;";
    replaceSpecialChars(str);
    EXPECT_EQ("1!", str);

    str = "&33;2";
    replaceSpecialChars(str);
    EXPECT_EQ("!2", str);

    str = "&33;&";
    replaceSpecialChars(str);
    EXPECT_EQ("!&", str);

    str = "test line&33;";
    replaceSpecialChars(str);
    EXPECT_EQ("test line!", str);
}

TEST(stringuntils, combineDye1)
{
    EXPECT_EQ("", combineDye("", ""));
    EXPECT_EQ("test", combineDye("test", ""));
    EXPECT_EQ("|line", combineDye("", "line"));
    EXPECT_EQ("test|line", combineDye("test", "line"));
}

TEST(stringuntils, combineDye2)
{
    EXPECT_EQ("", combineDye2("", ""));
    EXPECT_EQ("test", combineDye2("test", ""));
    EXPECT_EQ("", combineDye2("", "line"));
    EXPECT_EQ("test.xml", combineDye2("test.xml", "123"));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c",
        combineDye2("test.xml|#43413d,59544f,7a706c", ""));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c:W;",
        combineDye2("test.xml|#43413d,59544f,7a706c", "W"));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c:W;#123456:B;",
        combineDye2("test.xml|#43413d,59544f,7a706c;#123456", "W;B"));
}

TEST(stringuntils, combineDye3)
{
    EXPECT_EQ("", combineDye3("", ""));
    EXPECT_EQ("test", combineDye3("test", ""));
    EXPECT_EQ("", combineDye3("", "line"));
    EXPECT_EQ("test.xml|123", combineDye3("test.xml", "123"));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c",
        combineDye3("test.xml|#43413d,59544f,7a706c", ""));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c:W;",
        combineDye3("test.xml|#43413d,59544f,7a706c", "W"));
    EXPECT_EQ("test.xml|#43413d,59544f,7a706c:W;#123456:B;",
        combineDye3("test.xml|#43413d,59544f,7a706c;#123456", "W;B"));
}

TEST(stringuntils, packList1)
{
    std::list <std::string> list;
    EXPECT_EQ("", packList(list));

    list.push_back("");
    EXPECT_EQ("|", packList(list));

    list.clear();
    list.push_back("test");
    EXPECT_EQ("test", packList(list));

    list.push_back("line");
    EXPECT_EQ("test|line", packList(list));

    list.push_back("2");
    EXPECT_EQ("test|line|2", packList(list));
}

TEST(stringuntils, stringToHexPath1)
{
    std::string str;

    str = "";
    EXPECT_EQ("", stringToHexPath(str));

    str = "a";
    EXPECT_EQ("%61/", stringToHexPath(str));

    str = "ab";
    EXPECT_EQ("%61/%62", stringToHexPath(str));

    str = "abc";
    EXPECT_EQ("%61/%62%63", stringToHexPath(str));

    str = "abcd";
    EXPECT_EQ("%61/%62%63%64", stringToHexPath(str));
}

TEST(stringuntils, deleteCharLeft1)
{
    std::string str;
    unsigned int pos = 0;

    str = "";
    deleteCharLeft(str, nullptr);
    EXPECT_EQ("", str);

    str = "test line";
    pos = 4;
    deleteCharLeft(str, &pos);
    EXPECT_EQ("tes line", str);

    str = "тест line";
    pos = 8;
    deleteCharLeft(str, &pos);
    EXPECT_EQ("тес line", str);
}

TEST(stringuntils, findLast1)
{
    std::string str;

    str = "";
    EXPECT_TRUE(findLast(str, ""));

    str = "test line";
    EXPECT_TRUE(findLast(str, "line"));

    str = "test line";
    EXPECT_TRUE(!findLast(str, "lin"));
}

TEST(stringuntils, findFirst1)
{
    std::string str;

    str = "";
    EXPECT_TRUE(findFirst(str, ""));

    str = "test line";
    EXPECT_TRUE(findFirst(str, "test"));

    str = "test line";
    EXPECT_TRUE(!findFirst(str, "est"));
}

TEST(stringuntils, findCutLast1)
{
    std::string str;

    str = "";
    EXPECT_TRUE(findCutLast(str, ""));
    EXPECT_EQ("", str);

    str = "test line";
    EXPECT_TRUE(findCutLast(str, "line"));
    EXPECT_EQ("test ", str);

    str = "test line";
    EXPECT_TRUE(!findCutLast(str, "lin"));
    EXPECT_EQ("test line", str);
}

TEST(stringuntils, findCutFirst1)
{
    std::string str;

    str = "";
    EXPECT_TRUE(findCutFirst(str, ""));
    EXPECT_EQ("", str);

    str = "test line";
    EXPECT_TRUE(findCutFirst(str, "test"));
    EXPECT_EQ(" line", str);

    str = "test line";
    EXPECT_TRUE(!findCutFirst(str, "est"));
    EXPECT_EQ("test line", str);
}

TEST(stringuntils, removeProtocol1)
{
    std::string str;

    str = "";
    EXPECT_EQ("", removeProtocol(str));

    str = "http://";
    EXPECT_EQ("", removeProtocol(str));

    str = "http://test";
    EXPECT_EQ("test", removeProtocol(str));
}

TEST(stringuntils, strStartWith1)
{
    EXPECT_TRUE(strStartWith("", ""));
    EXPECT_TRUE(!strStartWith("", "1"));
    EXPECT_TRUE(strStartWith("test line", "test"));
    EXPECT_TRUE(strStartWith("test line", "test line"));
    EXPECT_TRUE(!strStartWith("test line", "est"));
}

TEST(stringuntils, encodeLinkText)
{
    std::string str;

    str = encodeLinkText("test line");
    EXPECT_EQ(str, "test line");
    str = encodeLinkText("test|line");
    EXPECT_EQ(str, "test\342\235\230line");
    str = encodeLinkText("test||line");
    EXPECT_EQ(str, "test\342\235\230\342\235\230line");
}

TEST(stringuntils, decodeLinkText)
{
    std::string str;

    str = encodeLinkText("test|line");
    EXPECT_EQ(str, "test\342\235\230line");
    str = decodeLinkText(str);
    EXPECT_EQ(str, "test|line");
}

TEST(stringuntils, isDigit)
{
    std::string str;

    EXPECT_FALSE(isDigit(""));
    EXPECT_TRUE(isDigit("1"));
    EXPECT_TRUE(isDigit("123"));
    EXPECT_FALSE(isDigit("+123"));
    EXPECT_FALSE(isDigit("-123"));
    EXPECT_FALSE(isDigit("1.23"));
    EXPECT_FALSE(isDigit("12-34"));
}
