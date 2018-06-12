/**
 ******************************************************************************
 * @addtogroup PIOS PIOS Core hardware abstraction layer
 * @{
 * @addtogroup PIOS_COM COM layer functions
 * @brief Hardware communication layer
 * @{
 *
 * @file       pios_com.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 *             Parts by Thorsten Klose (tk@midibox.org)
 * @brief      COM layer functions header
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

#ifndef PIOS_COM_H
#define PIOS_COM_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h> /* uint*_t */
#include <stdbool.h> /* bool */

typedef uint16_t (*DmaTransferCallback)(uint32_t context,bool *task_woken);
 
typedef uint16_t (*pios_com_callback)(uint32_t context, uint8_t *buf, uint16_t buf_len, uint16_t *headroom, bool *task_woken);
typedef void (*pios_com_callback_ctrl_line)(uint32_t context, uint32_t mask, uint32_t state);

struct com_driver {
    void (*init)(uint32_t id);
    void (*set_baud)(uint32_t id, uint32_t baud);
    void (*set_ctrl_line)(uint32_t id, uint32_t mask, uint32_t state);
    void (*tx_start)(uint32_t id, uint16_t tx_bytes_avail);
    void (*txDmaStart)(uint32_t usart_id,uint32_t BufferAddr,uint16_t size);
    void (*rx_start)(uint32_t id, uint16_t rx_bytes_avail);
    void (*rxDmaStart)(uint32_t usart_id,uint32_t BufferAddr,uint16_t size);
    void (*bind_rx_cb)(uint32_t id, pios_com_callback rx_in_cb, uint32_t context);
    void (*bind_tx_cb)(uint32_t id, pios_com_callback tx_out_cb, uint32_t context);
    void (*bind_ctrl_line_cb)(uint32_t id, pios_com_callback_ctrl_line ctrl_line_cb, uint32_t context);
    bool (*available)(uint32_t id);
    void (*bind_DmaRx_cb)(uint32_t id,DmaTransferCallback dma_rx_cb, uint32_t context);
    void (*bind_DmaTx_cb)(uint32_t id,DmaTransferCallback dma_tx_cb, uint32_t context);
    uint16_t (*rxDMACount)(uint32_t id);
    uint16_t (*txDMACount)(uint32_t id);
};

/* Control line definitions */
#define COM_CTRL_LINE_DTR_MASK 0x01
#define COM_CTRL_LINE_RTS_MASK 0x02
#define COM_USART_CONSOLE_TX_BUF_LEN    128
#define COM_USART_CONSOLE_RX_BUF_LEN    128
extern uint8_t USART_CONSOLE_TX_BUF[ COM_USART_CONSOLE_TX_BUF_LEN];
extern uint8_t USART_CONSOLE_RX_BUF[ COM_USART_CONSOLE_RX_BUF_LEN];

extern void GimbalBoardCfgCom(uint32_t usart_port_init,uint8_t *rx_buffer, uint16_t rx_buffer_len, uint8_t *tx_buffer, uint16_t tx_buffer_len,
							const struct com_driver *com_driver,uint32_t *pios_com_id,bool isDmaUsed);
/* Public Functions */
extern int32_t PIOS_COM_Init(uint32_t *com_id, const struct com_driver *driver, uint32_t lower_id, uint8_t *rx_buffer, uint16_t rx_buffer_len, uint8_t *tx_buffer, uint16_t tx_buffer_len,bool isDmaUsed);
extern int32_t PIOS_COM_ChangeBaud(uint32_t com_id, uint32_t baud);
extern int32_t PIOS_COM_SetCtrlLine(uint32_t com_id, uint32_t mask, uint32_t state);
extern int32_t PIOS_COM_RegisterCtrlLineCallback(uint32_t usart_id, pios_com_callback_ctrl_line ctrl_line_cb, uint32_t context);
extern int32_t PIOS_COM_SendCharNonBlocking(uint32_t com_id, char c);
extern int32_t PIOS_COM_SendChar(uint32_t com_id, char c);
extern int32_t PIOS_COM_SendBufferNonBlocking(uint32_t com_id, const uint8_t *buffer, uint16_t len);
extern int32_t PIOS_COM_SendBuffer(uint32_t com_id, const uint8_t *buffer, uint16_t len);
extern int32_t PIOS_COM_SendStringNonBlocking(uint32_t com_id, const char *str);
extern int32_t PIOS_COM_SendString(uint32_t com_id, const char *str);
extern int32_t PIOS_COM_SendFormattedStringNonBlocking(uint32_t com_id, const char *format, ...);
extern int32_t PIOS_COM_SendFormattedString(uint32_t com_id, const char *format, ...);
extern uint16_t PIOS_COM_ReceiveBuffer(uint32_t com_id, uint8_t *buf, uint16_t buf_len, uint32_t timeout_ms);
extern uint16_t PIOS_COM_ReceiveByteLen(uint32_t com_id);
extern bool PIOS_COM_ReceiveBytePeek(uint32_t com_id,uint8_t *byte);
extern bool PIOS_COM_Available(uint32_t com_id);
extern void PIOS_COM_Flush_Rx(uint32_t com_id);
extern void PIOS_COM_Flush_Tx(uint32_t com_id);

extern uint32_t comDebugId ;
extern uint32_t comGimInnerId;
extern uint32_t comCameraId;

#ifdef __cplusplus
 }
#endif
#endif /* PIOS_COM_H */

/**
 * @}
 * @}
 */
