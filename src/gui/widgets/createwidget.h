/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2018  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_CREATEWIDGET_H
#define GUI_WIDGETS_CREATEWIDGET_H

#define CREATEWIDGETV(var, type, ...) \
    var = new type(__VA_ARGS__); \
    var->postInit()

#define CREATEWIDGET(type, ...) \
    (new type(__VA_ARGS__))->postInit()

#define CREATEWIDGETV0(var, type) \
    var = new type; \
    var->postInit()

#define CREATEWIDGETR(type, ...) \
    static_cast<type*>(Widget::callPostInit(new type(__VA_ARGS__)))

#define CREATEWIDGETR0(type) \
    static_cast<type*>(Widget::callPostInit(new type))

#endif  // GUI_WIDGETS_CREATEWIDGET_H
