/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2018  The ManaPlus Developers
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

/*

SDL_gfxBlitFunc: custom blitters (part of SDL_gfx library)

LGPL (c) A. Schiffler

*/

#ifndef USE_SDL2
#include "resources/sdlgfxblitfunc.h"

#include "utils/cast.h"
#include "utils/checkutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#define GFX_DUFFS_LOOP4(pixel_copy_increment, width) \
    int n = ((width) + 3) / 4; \
    switch ((width) & 3) \
    { \
        case 0: do { \
            pixel_copy_increment; \
        case 3: pixel_copy_increment; \
        case 2: pixel_copy_increment; \
        case 1: pixel_copy_increment; \
        default: ; \
            } while (--n > 0 ); \
    }

namespace
{
    struct SDL_gfxBlitInfo final
    {
        A_DEFAULT_COPY(SDL_gfxBlitInfo)

        Uint8    *s_pixels;
        int       s_width;
        int       s_height;
        int       s_skip;
        Uint8    *d_pixels;
        int       d_width;
        int       d_height;
        int       d_skip;
        void     *aux_data;
        SDL_PixelFormat *src;
        Uint8    *table;
        SDL_PixelFormat *dst;
    };

    unsigned int GFX_ALPHA_ADJUST_ARRAY[256] =
    {
        0,  /* 0 */
        15,  /* 1 */
        22,  /* 2 */
        27,  /* 3 */
        31,  /* 4 */
        35,  /* 5 */
        39,  /* 6 */
        42,  /* 7 */
        45,  /* 8 */
        47,  /* 9 */
        50,  /* 10 */
        52,  /* 11 */
        55,  /* 12 */
        57,  /* 13 */
        59,  /* 14 */
        61,  /* 15 */
        63,  /* 16 */
        65,  /* 17 */
        67,  /* 18 */
        69,  /* 19 */
        71,  /* 20 */
        73,  /* 21 */
        74,  /* 22 */
        76,  /* 23 */
        78,  /* 24 */
        79,  /* 25 */
        81,  /* 26 */
        82,  /* 27 */
        84,  /* 28 */
        85,  /* 29 */
        87,  /* 30 */
        88,  /* 31 */
        90,  /* 32 */
        91,  /* 33 */
        93,  /* 34 */
        94,  /* 35 */
        95,  /* 36 */
        97,  /* 37 */
        98,  /* 38 */
        99,  /* 39 */
        100,  /* 40 */
        102,  /* 41 */
        103,  /* 42 */
        104,  /* 43 */
        105,  /* 44 */
        107,  /* 45 */
        108,  /* 46 */
        109,  /* 47 */
        110,  /* 48 */
        111,  /* 49 */
        112,  /* 50 */
        114,  /* 51 */
        115,  /* 52 */
        116,  /* 53 */
        117,  /* 54 */
        118,  /* 55 */
        119,  /* 56 */
        120,  /* 57 */
        121,  /* 58 */
        122,  /* 59 */
        123,  /* 60 */
        124,  /* 61 */
        125,  /* 62 */
        126,  /* 63 */
        127,  /* 64 */
        128,  /* 65 */
        129,  /* 66 */
        130,  /* 67 */
        131,  /* 68 */
        132,  /* 69 */
        133,  /* 70 */
        134,  /* 71 */
        135,  /* 72 */
        136,  /* 73 */
        137,  /* 74 */
        138,  /* 75 */
        139,  /* 76 */
        140,  /* 77 */
        141,  /* 78 */
        141,  /* 79 */
        142,  /* 80 */
        143,  /* 81 */
        144,  /* 82 */
        145,  /* 83 */
        146,  /* 84 */
        147,  /* 85 */
        148,  /* 86 */
        148,  /* 87 */
        149,  /* 88 */
        150,  /* 89 */
        151,  /* 90 */
        152,  /* 91 */
        153,  /* 92 */
        153,  /* 93 */
        154,  /* 94 */
        155,  /* 95 */
        156,  /* 96 */
        157,  /* 97 */
        158,  /* 98 */
        158,  /* 99 */
        159,  /* 100 */
        160,  /* 101 */
        161,  /* 102 */
        162,  /* 103 */
        162,  /* 104 */
        163,  /* 105 */
        164,  /* 106 */
        165,  /* 107 */
        165,  /* 108 */
        166,  /* 109 */
        167,  /* 110 */
        168,  /* 111 */
        168,  /* 112 */
        169,  /* 113 */
        170,  /* 114 */
        171,  /* 115 */
        171,  /* 116 */
        172,  /* 117 */
        173,  /* 118 */
        174,  /* 119 */
        174,  /* 120 */
        175,  /* 121 */
        176,  /* 122 */
        177,  /* 123 */
        177,  /* 124 */
        178,  /* 125 */
        179,  /* 126 */
        179,  /* 127 */
        180,  /* 128 */
        181,  /* 129 */
        182,  /* 130 */
        182,  /* 131 */
        183,  /* 132 */
        184,  /* 133 */
        184,  /* 134 */
        185,  /* 135 */
        186,  /* 136 */
        186,  /* 137 */
        187,  /* 138 */
        188,  /* 139 */
        188,  /* 140 */
        189,  /* 141 */
        190,  /* 142 */
        190,  /* 143 */
        191,  /* 144 */
        192,  /* 145 */
        192,  /* 146 */
        193,  /* 147 */
        194,  /* 148 */
        194,  /* 149 */
        195,  /* 150 */
        196,  /* 151 */
        196,  /* 152 */
        197,  /* 153 */
        198,  /* 154 */
        198,  /* 155 */
        199,  /* 156 */
        200,  /* 157 */
        200,  /* 158 */
        201,  /* 159 */
        201,  /* 160 */
        202,  /* 161 */
        203,  /* 162 */
        203,  /* 163 */
        204,  /* 164 */
        205,  /* 165 */
        205,  /* 166 */
        206,  /* 167 */
        206,  /* 168 */
        207,  /* 169 */
        208,  /* 170 */
        208,  /* 171 */
        209,  /* 172 */
        210,  /* 173 */
        210,  /* 174 */
        211,  /* 175 */
        211,  /* 176 */
        212,  /* 177 */
        213,  /* 178 */
        213,  /* 179 */
        214,  /* 180 */
        214,  /* 181 */
        215,  /* 182 */
        216,  /* 183 */
        216,  /* 184 */
        217,  /* 185 */
        217,  /* 186 */
        218,  /* 187 */
        218,  /* 188 */
        219,  /* 189 */
        220,  /* 190 */
        220,  /* 191 */
        221,  /* 192 */
        221,  /* 193 */
        222,  /* 194 */
        222,  /* 195 */
        223,  /* 196 */
        224,  /* 197 */
        224,  /* 198 */
        225,  /* 199 */
        225,  /* 200 */
        226,  /* 201 */
        226,  /* 202 */
        227,  /* 203 */
        228,  /* 204 */
        228,  /* 205 */
        229,  /* 206 */
        229,  /* 207 */
        230,  /* 208 */
        230,  /* 209 */
        231,  /* 210 */
        231,  /* 211 */
        232,  /* 212 */
        233,  /* 213 */
        233,  /* 214 */
        234,  /* 215 */
        234,  /* 216 */
        235,  /* 217 */
        235,  /* 218 */
        236,  /* 219 */
        236,  /* 220 */
        237,  /* 221 */
        237,  /* 222 */
        238,  /* 223 */
        238,  /* 224 */
        239,  /* 225 */
        240,  /* 226 */
        240,  /* 227 */
        241,  /* 228 */
        241,  /* 229 */
        242,  /* 230 */
        242,  /* 231 */
        243,  /* 232 */
        243,  /* 233 */
        244,  /* 234 */
        244,  /* 235 */
        245,  /* 236 */
        245,  /* 237 */
        246,  /* 238 */
        246,  /* 239 */
        247,  /* 240 */
        247,  /* 241 */
        248,  /* 242 */
        248,  /* 243 */
        249,  /* 244 */
        249,  /* 245 */
        250,  /* 246 */
        250,  /* 247 */
        251,  /* 248 */
        251,  /* 249 */
        252,  /* 250 */
        252,  /* 251 */
        253,  /* 252 */
        253,  /* 253 */
        254,  /* 254 */
        255   /* 255 */
    };
}  // namespace

static void _SDL_gfxBlitBlitterRGBA(const SDL_gfxBlitInfo *const info)
{
    const int width = info->d_width;
    int height = info->d_height;
    Uint8 *src = info->s_pixels;
    const int srcskip = info->s_skip;
    Uint8 *dst = info->d_pixels;
    const int dstskip = info->d_skip;
    const SDL_PixelFormat *const srcfmt = info->src;

    while ((height--) != 0)
    {
        GFX_DUFFS_LOOP4(
        {
            Uint32 pixel;
            uint32_t sR;
            uint32_t sG;
            uint32_t sB;
            uint32_t sA;
            uint32_t dR;
            uint32_t dG;
            uint32_t dB;
            uint32_t dA;
            uint32_t sAA;

            pixel = *(reinterpret_cast<uint32_t *>(src));
            sR = ((pixel & srcfmt->Rmask) >> srcfmt->Rshift);
            sG = ((pixel & srcfmt->Gmask) >> srcfmt->Gshift);
            sB = ((pixel & srcfmt->Bmask) >> srcfmt->Bshift);
            sA = ((pixel & srcfmt->Amask) >> srcfmt->Ashift);

            pixel = *(reinterpret_cast<uint32_t *>(dst));
            dR = pixel & 0xffU;
            dG = ((pixel & 0xff00U) >> 8);
            dB = ((pixel & 0xff0000U) >> 16);
            dA = ((pixel & 0xff000000U) >> 24);

            sAA = GFX_ALPHA_ADJUST_ARRAY[sA & 255];
            dR = (((sR - dR) * (sAA)) / 255) + dR;
            dG = (((sG - dG) * (sAA)) / 255) + dG;
            dB = (((sB - dB) * (sAA)) / 255) + dB;
            dA |= sAA;

            *(reinterpret_cast<uint32_t *>(dst)) = dR |
                (dG << 8) |
                (dB << 16) |
                (dA << 24);

            src += 4;
            dst += 4;
        }, width)
        src += srcskip;
        dst += dstskip;
    }
}

static int _SDL_gfxBlitRGBACall(const SDL_Surface *const src,
                                const SDL_Rect *const srcrect,
                                const SDL_Surface *const dst,
                                const SDL_Rect *const dstrect)
{
    /*
    * Set up source and destination buffer pointers, then blit
    */
    if ((srcrect->w != 0U) && (srcrect->h != 0U))
    {
        SDL_gfxBlitInfo info;

        /*
        * Set up the blit information
        */
        info.s_pixels = static_cast<Uint8 *>(src->pixels) + src->offset +
            static_cast<Uint16>(srcrect->y) * src->pitch +
            static_cast<Uint16>(srcrect->x) * src->format->BytesPerPixel;
        info.s_width = srcrect->w;
        info.s_height = srcrect->h;
        info.s_skip = CAST_S32(src->pitch - info.s_width *
            src->format->BytesPerPixel);
        info.d_pixels = static_cast<Uint8 *>(dst->pixels) + dst->offset +
            static_cast<Uint16>(dstrect->y) * dst->pitch +
            static_cast<Uint16>(dstrect->x) * dst->format->BytesPerPixel;
        info.d_width = dstrect->w;
        info.d_height = dstrect->h;
        info.d_skip = CAST_S32(dst->pitch - info.d_width *
            dst->format->BytesPerPixel);
        info.aux_data = nullptr;
        info.src = src->format;
        info.table = nullptr;
        info.dst = dst->format;

        /*
        * Run the actual software blitter
        */
        _SDL_gfxBlitBlitterRGBA(&info);
        return 1;
    }

    return 0;
}

int SDLgfxBlitRGBA(SDL_Surface *const src,
                   const SDL_Rect *const srcrect,
                   SDL_Surface *const dst,
                   const SDL_Rect *const dstrect)
{
    SDL_Rect sr;
    SDL_Rect dr;
    int srcx;
    int srcy;
    int w;
    int h;

    /*
    * Make sure the surfaces aren't locked
    */
    if (src == nullptr ||
        dst == nullptr)
    {
        reportAlways("SDLgfxBlitRGBA: passed a NULL surface")
        return -1;
    }

    /*
    * If the destination rectangle is NULL, use the entire dest surface
    */
    if (dstrect == nullptr)
    {
        dr.x = 0;
        dr.y = 0;
        dr.w = CAST_U16(dst->w);
        dr.h = CAST_U16(dst->h);
    }
    else
    {
        dr = *dstrect;
    }

    /*
    * Clip the source rectangle to the source surface
    */
    if (srcrect != nullptr)
    {
        int maxw;
        int maxh;

        srcx = srcrect->x;
        w = srcrect->w;
        maxw = src->w - srcx;
        if (maxw < w)
            w = maxw;

        srcy = srcrect->y;
        h = srcrect->h;
        maxh = src->h - srcy;
        if (maxh < h)
            h = maxh;
    }
    else
    {
        srcx = 0;
        srcy = 0;
        w = src->w;
        h = src->h;
    }

    /*
    * Clip the destination rectangle against the clip rectangle
    */
    const SDL_Rect *const clip = &dst->clip_rect;
    int dx;
    int dy;

    dx = clip->x - dr.x;
    if (dx > 0)
    {
        w -= dx;
        dr.x += dx;
        srcx += CAST_S16(dx);
    }
    dx = dr.x + w - clip->x - clip->w;
    if (dx > 0)
        w -= dx;

    dy = clip->y - dr.y;
    if (dy > 0)
    {
        h -= dy;
        dr.y += dy;
        srcy += CAST_S16(dy);
    }
    dy = dr.y + h - clip->y - clip->h;
    if (dy > 0)
        h -= dy;

    if (w > 0 && h > 0)
    {
        sr.x = CAST_S16(srcx);
        sr.y = CAST_S16(srcy);
        sr.w = dr.w = CAST_U16(w);
        sr.h = dr.h = CAST_U16(h);
        return _SDL_gfxBlitRGBACall(src, &sr, dst, &dr);
    }

    return 0;
}

#endif  // USE_SDL2
