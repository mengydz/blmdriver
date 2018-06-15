/*
 * timer.h
 *
 *  Created on: Sep 27, 2016
 *      Author: baron
 */

#ifndef __TIMER_H_
#define __TIMER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "board_hw_defs.h"
#include "global.h"
extern uint32_t Hal_Timer_ID;
extern uint32_t Hal_Print_Tim_ID,Hal_motor_Tim_ID,Hal_can_Tim_ID;;

typedef struct{
	GIMBAL_TIM_TIMER_CFG	*TimerCfg;
	bool	IQRInited;
	bool    isInit;
}__attribute__((packed))GIMBAL_TIM_TIMER_DEV;


extern uint32_t Hal_Timer_4,Hal_Timer_5,Hal_Timer_6,Hal_Timer_7;

void SysTimerTimInit(uint32_t *timer_tim_id,const GIMBAL_TIM_TIMER_CFG *cfg);
uint64_t GetMicro(void);
uint32_t GetMillis(void);
#ifdef __cplusplus
 }
#endif
#endif /* TIM_PWM_OUTPUT_H_ */
