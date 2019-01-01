/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#define RENDER_OPENGLGRAPHICSDEFADVANCED_HPP

public:
    inline void drawVertexes(const OpenGLGraphicsVertexes &restrict ogl)
                             restrict2 A_INLINE;

    void initArrays(const int vertCount) restrict2 override final;

#ifdef DEBUG_DRAW_CALLS
    unsigned int getDrawCalls() restrict2 const noexcept2
    { return mLastDrawCalls; }

    static unsigned int mDrawCalls;

    static unsigned int mLastDrawCalls;
#endif

protected:
    void debugBindTexture(const Image *restrict const image) restrict2;

    void deleteArraysInternal() restrict2;
