/*
    This file is part of TinyVGM.

    Copyright (C) 2021 ReimuNotMoe <reimu@sudomaker.com>
    Copyright (C) 2021 Yukino Song <yukino@sudomaker.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	TinyVGM_OK = 0,
	TinyVGM_NO = -1,
} TinyVGMReturn;

typedef enum {
	TinyVGM_Event_HeaderParseDone = 1,
	TinyVGM_Event_PlaybackDone = 2
} TinyVGMEvent;

typedef enum {
	TinyVGM_CallBackType_Command = 0,
	TinyVGM_CallBackType_Event = 2
} TinyVGMCallbackType;

typedef struct tinyvgm_callback_info {
	uint8_t id;
	void *userp;

	int (*callback)(void *, uint8_t, const void *, uint32_t);
} TinyVGMCallbackInfo;

typedef struct tinyvgm_context {
	TinyVGMCallbackInfo *callbacks[3];
	uint8_t nr_callbacks[3];

	uint32_t read_bytes;

	uint8_t current_command;

	uint8_t buffer[8];
	uint8_t buffer_pos;
} TinyVGMContext;

/**
 * Calculate the length of a 16-bit, 0x0000-terminated string.
 *
 * @param strarg		The 16-bit string.
 *
 * @return			Length of the string.
 */
extern size_t tinyvgm_strlen16(const int16_t* strarg);

/**
 * Initialize a TinyVGM context.
 *
 * @param ctx			TinyVGM context pointer.
 *
 */
extern void tinyvgm_init(TinyVGMContext *ctx);

/**
 * Reset a TinyVGM context, to make it able to parse a new VGM stream.
 *
 * @param ctx			TinyVGM context pointer.
 *
 */
extern void tinyvgm_reset(TinyVGMContext *ctx);

/**
 * Destroy a TinyVGM context, frees all runtime allocated memory.
 *
 * @param ctx			TinyVGM context pointer.
 *
 */
extern void tinyvgm_destroy(TinyVGMContext *ctx);

/**
 * Parse a portion of VGM stream.
 *
 * @param ctx			TinyVGM context pointer.
 * @param buf			Buffer of the VGM stream.
 * @param len			Length of buffer.
 *
 * @return			Length of successfully processed bytes. If a callback fails, negated bytes drained from buffer will be returned. If the VGM stream is invalid, INT32_MIN will be returned.
 *
 * When a callback fails, you can get the command in TinyVGMContext::current_command and its params in TinyVGMContext::buffer, if needed.
 *
 */
extern int32_t tinyvgm_parse(TinyVGMContext *ctx, const void *buf, uint16_t len);

/**
 * Add an event callback.
 *
 * @param ctx			TinyVGM context pointer.
 * @param event			Event. See `TinyVGMEvent' type for available event types.
 * @param callback		Callback function pointer.
 * @param userp			User data pointer to supply as the first argument of the callback function. Can be a C++ class pointer or NULL.
 *
 */
extern void tinyvgm_add_event_callback(TinyVGMContext *ctx, TinyVGMEvent event, int (*callback)(void *, uint8_t, const void *, uint32_t), void *userp);

/**
 * Add a command callback.
 *
 * @param ctx			TinyVGM context pointer.
 * @param command		Command. e.g. 0x50 for `SN76489', 0x5e for `OPL3 Port0'.
 * @param callback		Callback function pointer.
 * @param userp			User data pointer to supply as the first argument of the callback function. Can be a C++ class pointer or NULL.
 *
 */
extern void tinyvgm_add_command_callback(TinyVGMContext *ctx, uint8_t command, int (*callback)(void *, uint8_t, const void *, uint32_t), void *userp);

#ifdef __cplusplus
};
#endif
