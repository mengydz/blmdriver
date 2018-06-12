/**
 ******************************************************************************
 * @addtogroup TauLabsLibraries Tau Labs Libraries
 * @{
 *
 * @file       fifo_buffer.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      GPIO functions header.
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"

// *********************
typedef struct
{
	uint32_t id;
	uint8_t data[8];
	uint8_t data_len;
}canbufferType;

typedef struct
{
	canbufferType *buf_ptr;
    volatile uint16_t rd;
    volatile uint16_t wr;
    uint16_t buf_size;
} t_fifo_buffercan;

// *********************

//uint16_t CanCanfifoBuf_getSize(t_fifo_buffercan *buf);
//
//uint16_t CanfifoBuf_getUsed(t_fifo_buffercan *buf);
//uint16_t CanfifoBuf_getFree(t_fifo_buffercan *buf);
//
//void CanfifoBuf_clearData(t_fifo_buffercan *buf);
//void CanfifoBuf_removeData(t_fifo_buffercan *buf, uint16_t len);
//
//int16_t CanfifoBuf_getBytePeek(t_fifo_buffercan *buf);
//int16_t CanfifoBuf_getByte(t_fifo_buffercan *buf);
//
//uint16_t CanfifoBuf_getDataPeek(t_fifo_buffercan *buf, void *data, uint16_t len);
//uint16_t CanfifoBuf_getData(t_fifo_buffercan *buf, void *data, uint16_t len);
//
//uint16_t CanfifoBuf_putByte(t_fifo_buffercan *buf, const uint8_t b);
//
//uint16_t CanfifoBuf_putData(t_fifo_buffercan *buf, const void *data, uint16_t len);
//
//void CanfifoBuf_init(t_fifo_buffercan *buf, const void *buffer, const uint16_t buffer_size);
//
//void CanfifoBuf_flush(t_fifo_buffercan *buf);

#ifdef __cplusplus
 }
#endif
#endif /* _FIFO_BUFFER_H_ */

/**
 * @}
 */
