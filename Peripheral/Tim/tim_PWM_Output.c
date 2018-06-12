/*
 * tim_PWM_Output.c
 *
 *  Created on: Sep 27, 2016
 *      Author: baron
 */
#include "tim_PWM_Output.h"
#include "driver_stm32.h"
#include "FreeRTOS.h"

uint32_t Hal_Tim_pwmOut_ID;

uint32_t Hal_Tim_1,Hal_Tim_8;
//static void MotorSvpwmTimIRQEnable(uint32_t svpwm_tim_id,CurLoopConfig	*cfg);
//static void MotorSvpwmCurrentLoopSetIq(uint32_t svpwm_tim_id,float iq,uint16_t encode,bool enable,float dt);
//static uint32_t GetCurLoopHandle(uint32_t tim_id);

void MotorSvpwmTimInit(uint32_t *svpwm_tim_id,const GIMBAL_TIM_PWMOUT_CFG *cfg,uint32_t sampleADC_id)
{
	GIMBAL_TIM_PWUOUT_DEV *pwmout_dev = (GIMBAL_TIM_PWUOUT_DEV *)pvPortMalloc(sizeof(GIMBAL_TIM_PWUOUT_DEV));
	DEBUG_Assert(pwmout_dev);

	pwmout_dev->MotorCfg = (GIMBAL_TIM_PWMOUT_CFG	*)cfg;
	
	HAL_RCC_CLK_ENABLE(pwmout_dev->MotorCfg->tim->Instance);
	
	HAL_TIM_Base_DeInit(pwmout_dev->MotorCfg->tim);
	
	if(pwmout_dev->MotorCfg->tim->Instance == TIM1)
	{
		Hal_Tim_1 = (uint32_t)pwmout_dev;
	}else if(pwmout_dev->MotorCfg->tim->Instance == TIM8)
	{
		Hal_Tim_8 = (uint32_t)pwmout_dev;
	}
	pwmout_dev->sampleADC_ID = sampleADC_id;

	if(HAL_TIM_Base_Init(pwmout_dev->MotorCfg->tim)!=HAL_OK \
			|| (HAL_TIMEx_MasterConfigSynchronization(pwmout_dev->MotorCfg->tim, &pwmout_dev->MotorCfg->sMC) != HAL_OK))
	{
		DEBUG_Assert(0);
	}

	if((HAL_TIM_PWM_ConfigChannel(pwmout_dev->MotorCfg->tim,&pwmout_dev->MotorCfg->oc,pwmout_dev->MotorCfg->CCTimChannel) != HAL_OK) \
		|| (HAL_TIM_PWM_Start(pwmout_dev->MotorCfg->tim,pwmout_dev->MotorCfg->CCTimChannel) != HAL_OK) )
	{
		DEBUG_Assert(0);
	}

	for(uint8_t j = 0 ; j < MotorPhase_Num ; j++)
	{
		HAL_RCC_CLK_ENABLE(pwmout_dev->MotorCfg->Gpio[j].gpio);
		HAL_GPIO_Init(pwmout_dev->MotorCfg->Gpio[j].gpio,&pwmout_dev->MotorCfg->Gpio[j].initTypeDef);

		if((HAL_TIM_PWM_ConfigChannel(pwmout_dev->MotorCfg->tim,&pwmout_dev->MotorCfg->oc,pwmout_dev->MotorCfg->TimChannel[j]) != HAL_OK) \
			|| (HAL_TIM_PWM_Start(pwmout_dev->MotorCfg->tim,pwmout_dev->MotorCfg->TimChannel[j]) != HAL_OK) )
		{
			DEBUG_Assert(0);
		}
	}

	pwmout_dev->IQRInited = false;
	pwmout_dev->isInit = true;
	*svpwm_tim_id = (uint32_t)pwmout_dev;
}

static void MotorSvpwmTimPulseSet(TIM_TypeDef *Timx,uint32_t timChan,uint16_t pulse)
{
	switch(timChan)
	{
		case TIM_CHANNEL_1	:	Timx->CCR1 = pulse;	break;
		case TIM_CHANNEL_2	:	Timx->CCR2 = pulse;	break;
		case TIM_CHANNEL_3	:	Timx->CCR3 = pulse;	break;
		case TIM_CHANNEL_4	:	Timx->CCR4 = pulse;	break;
		default:break;
	}
}

void MotorSvpwmTimPulseUpdate(uint32_t svpwm_tim_id,uint16_t *pulse)
{
	GIMBAL_TIM_PWUOUT_DEV *pwmout_dev = (GIMBAL_TIM_PWUOUT_DEV *)svpwm_tim_id;
	DEBUG_Assert(pwmout_dev);

	for(uint8_t j = 0;j<MotorPhase_Num;j++)
	{
	    if(pwmout_dev->MotorCfg->oc.OCMode == TIM_OCMODE_PWM2)
	    {
	        if(pulse[j] < pwmout_dev->MotorCfg->tim->Init.Period)
	        {
	            pulse[j] = pwmout_dev->MotorCfg->tim->Init.Period - pulse[j];
	        }else
	        {
	            pulse[j] = 0;
	        }
	        
	    }
		MotorSvpwmTimPulseSet(pwmout_dev->MotorCfg->tim->Instance,pwmout_dev->MotorCfg->TimChannel[j],pulse[j]);
	}
}


void TIM1_UP_TIM10_IRQHandler(void)
{
	//tim_irq_handler(Hal_Tim_1);
}

