/*  Copyright (C) 2005-2016, Axis Communications AB, LUND, SWEDEN
 *
 *  This file is part of RAPP.
 *
 *  RAPP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  You can use the comments under either the terms of the GNU Lesser General
 *  Public License version 3 as published by the Free Software Foundation,
 *  either version 3 of the License or (at your option) any later version, or
 *  the GNU Free Documentation License version 1.3 or any later version
 *  published by the Free Software Foundation; with no Invariant Sections, no
 *  Front-Cover Texts, and no Back-Cover Texts.
 *  A copy of the license is included in the documentation section entitled
 *  "GNU Free Documentation License".
 *
 *  RAPP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License and a copy of the GNU Free Documentation License along
 *  with RAPP. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *  @file   rapp_ref_cond.c
 *  @brief  RAPP conditional operations, reference implementation.
 */

#include "rapp.h"          /* RAPP API            */
#include "rapp_ref_cond.h" /* Conditional ops API */

#undef  MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#undef  CLAMP
#define CLAMP(x, a, b) ((x) < (a) ? (a) : (x) > (b) ? (b) : (x))


/*
 * -------------------------------------------------------------
 *  Exported functions
 * -------------------------------------------------------------
 */

void
rapp_ref_cond_set_u8(uint8_t *dst, int dst_dim,
                     const uint8_t *map, int map_dim,
                     int width, int height, unsigned value)
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (rapp_pixel_get_bin(map, map_dim, 0, x, y)) {
                dst[y*dst_dim + x] = value;
            }
        }
    }
}

void
rapp_ref_cond_addc_u8(uint8_t *dst, int dst_dim,
                      const uint8_t *map, int map_dim,
                      int width, int height, int value)
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (rapp_pixel_get_bin(map, map_dim, 0, x, y)) {
                dst[y*dst_dim + x] = CLAMP(dst[y*dst_dim + x] + value, 0, 0xff);
            }
        }
    }
}

void
rapp_ref_cond_copy_u8(uint8_t *dst, int dst_dim,
                      const uint8_t *src, int src_dim,
                      const uint8_t *map, int map_dim,
                      int width, int height)
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (rapp_pixel_get_bin(map, map_dim, 0, x, y)) {
                dst[y*dst_dim + x] = src[y*src_dim + x];
            }
        }
    }
}

void
rapp_ref_cond_add_u8(uint8_t *dst, int dst_dim,
                     const uint8_t *src, int src_dim,
                     const uint8_t *map, int map_dim,
                     int width, int height)
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (rapp_pixel_get_bin(map, map_dim, 0, x, y)) {
                dst[y*dst_dim + x] = CLAMP(dst[y*dst_dim + x] +
                                           src[y*src_dim + x], 0, 0xff);
            }
        }
    }
}
