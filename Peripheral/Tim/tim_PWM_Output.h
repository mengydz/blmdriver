/*
 * tim_PWM_Output.h
 *
 *  Created on: Sep 27, 2016
 *      Author: baron
 */

#ifndef TIM_PWM_OUTPUT_H_
#define TIM_PWM_OUTPUT_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "board_hw_defs.h"
//#include "currentLoop.h"

extern uint32_t Hal_Tim_pwmOut_ID;

typedef struct{
	GIMBAL_TIM_PWMOUT_CFG	*MotorCfg;
	uint32_t CurrentLoopId;
	uint32_t sampleADC_ID;
	uint8_t SampleChannel;
	bool	IQRInited;
	bool    isInit;
}__attribute__((packed))GIMBAL_TIM_PWUOUT_DEV;


typedef void (*PulseUpdate)(uint32_t svpwm_tim_id ,uint16_t *pulse);

void MotorSvpwmTimInit(uint32_t *svpwm_tim_id,const GIMBAL_TIM_PWMOUT_CFG *cfg,uint32_t sampleADC_id);


void MotorSvpwmTimPulseUpdate(uint32_t svpwm_tim_id ,uint16_t *pulse);
uint16_t CurLoopADCSampleChannal(uint32_t svpwm_tim_id,uint8_t chan);

extern uint32_t Hal_Tim_1,Hal_Tim_8;

#ifdef __cplusplus
 }
#endif
#endif /* TIM_PWM_OUTPUT_H_ */
