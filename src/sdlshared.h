/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef SDLSHARED_H
#define SDLSHARED_H

#ifdef USE_SDL2

#define SDL_GetKeyState SDL_GetKeyboardState
#define SDLKey SDL_Scancode
#define SDL_keysym SDL_Keysym

#define SDL_ANYFORMAT 0
#define SDL_HWSURFACE 0
#define SDL_NOFRAME SDL_WINDOW_BORDERLESS
#define SDL_RESIZABLE SDL_WINDOW_RESIZABLE
#define SDL_FULLSCREEN SDL_WINDOW_FULLSCREEN
#define SDL_OPENGL SDL_WINDOW_OPENGL

#define KMOD_META KMOD_GUI
#define SDLK_RMETA SDLK_RGUI
#define SDLK_LMETA SDLK_LGUI
#define SDLK_KP0 SDLK_KP_0
#define SDLK_KP1 SDLK_KP_1
#define SDLK_KP2 SDLK_KP_2
#define SDLK_KP3 SDLK_KP_3
#define SDLK_KP4 SDLK_KP_4
#define SDLK_KP5 SDLK_KP_5
#define SDLK_KP6 SDLK_KP_6
#define SDLK_KP7 SDLK_KP_7
#define SDLK_KP8 SDLK_KP_8
#define SDLK_KP9 SDLK_KP_9
#define SDLK_PRINT SDLK_PRINTSCREEN
#define SDLK_NUMLOCK SDLK_NUMLOCKCLEAR
#define SDLK_SCROLLOCK SDLK_SCROLLLOCK
#define SDLK_LAST SDL_NUM_SCANCODES

#define SDL_DEFAULT_REPEAT_DELAY 500

#else  // USE_SDL2

#define SDL_Window SDL_Surface
#define SDL_JoystickNameForIndex SDL_JoystickName
#define SurfaceImageHelper SDLImageHelper

#endif  // USE_SDL2

#endif  // SDLSHARED_H
