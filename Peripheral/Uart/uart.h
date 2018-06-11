/*
 * uart.h
 *
 *  Created on: Sep 26, 2016
 *      Author: baron
 */

#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "board_hw_defs.h"


extern const struct com_driver usart_driver ;
extern int32_t USART_Init(uint32_t *usart_id, GIMBAL_UART_CFG *cfg,bool isUmaUsed);
extern GIMBAL_UART_CFG * GetUsartCfg(uint32_t usart_id);




#ifdef __cplusplus
 }
#endif
#endif /* UART_H_ */
