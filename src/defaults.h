/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <map>
#include <string>

#include "localconsts.h"

class VariableData;

typedef std::map<std::string, VariableData*> DefaultsData;

VariableData* createData(const int defData) A_WARN_UNUSED;
VariableData* createData(const double defData) A_WARN_UNUSED;
VariableData* createData(const float defData) A_WARN_UNUSED;
VariableData* createData(const std::string &defData) A_WARN_UNUSED;
VariableData* createData(const char *const defData) A_WARN_UNUSED;
VariableData* createData(const bool defData) A_WARN_UNUSED;
DefaultsData* getConfigDefaults() A_WARN_UNUSED;
void getConfigDefaults2(DefaultsData *const configData);
DefaultsData* getBrandingDefaults() A_WARN_UNUSED;
DefaultsData* getPathsDefaults() A_WARN_UNUSED;
DefaultsData* getFeaturesDefaults() A_WARN_UNUSED;

#endif  // DEFAULTS_H
