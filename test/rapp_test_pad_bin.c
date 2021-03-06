/*  Copyright (C) 2005-2010, Axis Communications AB, LUND, SWEDEN
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
 *  @file   rapp_test_pad_bin.c
 *  @brief  Correctness tests for binary image padding.
 */

#include <string.h>           /* memcpy()       */
#include "rapp.h"             /* RAPP API       */
#include "rapp_ref_pad_bin.h" /* Reference API  */
#include "rapp_test_util.h"   /* Test utilities */


/*
 * -------------------------------------------------------------
 *  Constants
 * -------------------------------------------------------------
 */

/**
 *  The number of test iterations.
 */
#define RAPP_TEST_ITER  512

/**
 *  Test image maximum width.
 */
#define RAPP_TEST_WIDTH  512

/**
 *  Test image maximum height.
 */
#define RAPP_TEST_HEIGHT 8

/**
 *  The maximum padding in pixels.
 */
#define RAPP_TEST_BORDER 128


/*
 * -------------------------------------------------------------
 *  Local functions fwd declare
 * -------------------------------------------------------------
 */

static bool
rapp_test_driver(int (*func)(), void (*ref)());

/*
 * -------------------------------------------------------------
 *  Test cases
 * -------------------------------------------------------------
 */

bool
rapp_test_pad_align_bin()
{
    int      dim     = rapp_align((RAPP_TEST_WIDTH + 7)/8) + rapp_alignment;
    int      len     = dim*RAPP_TEST_HEIGHT;
    uint8_t *src_buf = rapp_malloc(len, 0);
    uint8_t *dst_buf = rapp_malloc(len, 0);
    uint8_t *ref_buf = rapp_malloc(len, 0);
    int      k;

    /* Initialize the source buffer */
    rapp_test_init(src_buf, 0, len, 1, true);

    /* Run random tests */
    for (k = 0; k < RAPP_TEST_ITER; k++) {
        int width  = rapp_test_rand(1, RAPP_TEST_WIDTH);
        int height = rapp_test_rand(1, RAPP_TEST_HEIGHT);
        int align  = rapp_test_rand(0, rapp_alignment - 1);
        int offset = rapp_test_rand(0, 7);
        int full   = (8*align + offset + width + 8*rapp_alignment - 1) &
                     ~(8*rapp_alignment - 1);
        int value  = rapp_test_rand(0, 1);

        /* Set the buffers */
        memcpy(dst_buf, src_buf, len);
        memcpy(ref_buf, src_buf, len);

        /* Call the padding function */
        if (rapp_pad_align_bin(&dst_buf[align], dim, offset,
                               width, height, value) < 0)
        {
            DBG("Got FAIL return value\n");
            return false;
        }

        /* Call the reference function */
        rapp_ref_pad_align_bin(&ref_buf[align], dim, offset,
                               width, height, value);

        /* Compare the results */
        if (!rapp_test_compare_bin(ref_buf, dim, dst_buf, dim,
                                   0, full, height))
        {
            DBG("Failed\n");
            DBG("dst=\n");
            rapp_test_dump_bin(dst_buf, dim, 0, full, height);
            DBG("ref=\n");
            rapp_test_dump_bin(ref_buf, dim, 0, full, height);
            return false;
        }
    }

    rapp_free(src_buf);
    rapp_free(dst_buf);
    rapp_free(ref_buf);

    return true;
}

bool
rapp_test_pad_const_bin(void)
{
    return rapp_test_driver(&rapp_pad_const_bin,
                            &rapp_ref_pad_const_bin);
}

bool
rapp_test_pad_clamp_bin(void)
{
    return rapp_test_driver(&rapp_pad_clamp_bin,
                            &rapp_ref_pad_clamp_bin);
}

/*
 * -------------------------------------------------------------
 *  Local functions
 * -------------------------------------------------------------
 */

static bool
rapp_test_driver(int (*func)(), void (*ref)())
{
    int      dim     = rapp_align((RAPP_TEST_WIDTH +
                                   2*RAPP_TEST_BORDER + 7)/8) + rapp_alignment;
    int      len     = dim*(RAPP_TEST_HEIGHT + 2*RAPP_TEST_BORDER);
    uint8_t *src_buf = rapp_malloc(len, 0);
    uint8_t *dst_buf = rapp_malloc(len, 0);
    uint8_t *ref_buf = rapp_malloc(len, 0);
    int      k;

    /* Initialize the source buffer */
    rapp_test_init(src_buf, 0, len, 1, true);

    /* Run random tests */
    for (k = 0; k < RAPP_TEST_ITER; k++) {
        int width   = rapp_test_rand(1, RAPP_TEST_WIDTH);
        int height  = rapp_test_rand(1, RAPP_TEST_HEIGHT);
        int border  = rapp_test_rand(1, RAPP_TEST_BORDER);

        int value   = rapp_test_rand(0, 1);
        int pad_idx = rapp_test_rand(0, rapp_alignment - 1);
        int pad_off = rapp_test_rand(0, 7);

        int buf_idx = pad_idx + (pad_off + border) / 8 + border*dim;
        int buf_off = (pad_off + border) % 8;

        /* Set the buffers */
        memcpy(dst_buf, src_buf, len);
        memcpy(ref_buf, src_buf, len);

        /* Call the padding function */
        if ((*func)(&dst_buf[buf_idx], dim, buf_off,
                    width, height, border, value) < 0)
        {
            DBG("Got FAIL return value\n");
            return false;
        }

        /* Call the reference function */
        (*ref)(&ref_buf[buf_idx], dim, buf_off, width, height, border, value);

        /* Compare the results */
        if (!rapp_test_compare_bin(&ref_buf[pad_idx], dim,
                                   &dst_buf[pad_idx], dim, pad_off,
                                   width  + 2*border,
                                   height + 2*border))
        {
            DBG("Failed\n");
            DBG("dst=\n");
            rapp_test_dump_bin(&dst_buf[pad_idx], dim, pad_off,
                               width  + 2*border,
                               height + 2*border);
            DBG("ref=\n");
            rapp_test_dump_bin(&ref_buf[pad_idx], dim, pad_off,
                               width  + 2*border,
                               height + 2*border);
            return false;
        }
    }

    rapp_free(src_buf);
    rapp_free(dst_buf);
    rapp_free(ref_buf);

    return true;
}
