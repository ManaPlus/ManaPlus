#! /usr/bin/env python
# -*- coding: utf8 -*-
#
# Copyright (C) 2017  Andrei Karas

import os
import re

testCaseRe = re.compile("^TEST_CASE[(]\"(?P<name1>([\w ()_:.,]*))\",[ ]\"(?P<name2>([\w ()_:.,]*))\"[)]$")
sectionRe = re.compile("^([ ]*)SECTION[(]\"(?P<name>([\w ()_:.,]*))\"[)]$")


def printTestCase(testCase):
#    print("func --test-case=\"{0}\"".format(testCase))
#    print("func \"{0}\" \"\"".format(testCase))
    print("\"{0}\" \"\"".format(testCase))


def printSection(testCase, section):
#    print("func --test-case=\"{0}\" --subcase=\"{1}\"".format(testCase, section))
#    print("func \"{0}\" \"{1}\"".format(testCase, section))
    print("\"{0}\" \"{1}\"".format(testCase, section))


def parseCC(srcFile):
    with open(srcFile, "r") as r:
        testCase = ""
        sectionsCount = 0
        for line in r:
            m = testCaseRe.search(line)
            if m != None:
                if testCase != "" and sectionsCount == 0:
                    printTestCase(testCase)
                testCase = m.group("name1") + " " + m.group("name2")
                sectionsCount = 0
            elif line.find("TEST_CASE(\"") >= 0:
                print("Error: test case regexp failed for: " + line)
                exit(1)
            m = sectionRe.search(line)
            if m != None:
                sectionsCount = sectionsCount + 1
                printSection(testCase, m.group("name"))
            elif line.find("SECTION(\"") >= 0:
                print("Error: section regexp failed for: " + line)
                exit(1)
        if testCase != "" and sectionsCount == 0:
            printTestCase(testCase)


def enumFiles(srcDir):
    files = os.listdir(srcDir)
    for file1 in files:
        if file1[0] == ".":
            continue
        srcPath = os.path.abspath(srcDir + os.path.sep + file1)
        if os.path.isdir(srcPath):
            enumFiles(srcPath)
        elif file1[-3:] == ".cc":
            parseCC(srcPath)


enumFiles("src/unittests")
