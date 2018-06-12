/*
 * timer.c
 *
 *  Created on: Sep 27, 2016
 *      Author: baron
 */
#include "timer.h"
#include "driver_stm32.h"
#include "stdlib.h"

uint32_t Hal_Timer_ID;
uint32_t Hal_Print_Tim_ID,Hal_motor_Tim_ID,Hal_can_Tim_ID;
uint64_t halSysTickCount = 0;

uint32_t Hal_Timer_4,Hal_Timer_5,Hal_Timer_6,Hal_Timer_7;


void SysTimerTimInit(uint32_t *timer_tim_id,const GIMBAL_TIM_TIMER_CFG *cfg)
{
	GIMBAL_TIM_TIMER_DEV *timer_dev = malloc(sizeof(GIMBAL_TIM_TIMER_DEV));
	DEBUG_Assert(timer_dev);

	timer_dev->TimerCfg = (GIMBAL_TIM_TIMER_CFG	*)cfg;
	
	HAL_RCC_CLK_ENABLE(timer_dev->TimerCfg->tim->Instance);
	
	HAL_TIM_Base_DeInit(timer_dev->TimerCfg->tim);
	
	if(timer_dev->TimerCfg->tim->Instance == TIM4){
		Hal_Timer_4 = (uint32_t)timer_dev;
	}else if(timer_dev->TimerCfg->tim->Instance == TIM5){	
		Hal_Timer_5 = (uint32_t)timer_dev;
	}else if(timer_dev->TimerCfg->tim->Instance == TIM6){
		Hal_Timer_6 = (uint32_t)timer_dev;
	}else if(timer_dev->TimerCfg->tim->Instance == TIM7){
		Hal_Timer_7 = (uint32_t)timer_dev;
	}

	if(HAL_TIM_Base_Init(timer_dev->TimerCfg->tim)!=HAL_OK)
	{
		DEBUG_Assert(0);
	}

	if(timer_dev->TimerCfg->tim_irq.irq_enabled)
	{
		STM32_IRQ_CFG *irq = &timer_dev->TimerCfg->tim_irq;
		if(irq->irq_enabled == true)
		{
			HAL_NVIC_SetPriority(irq->irq_cfg.irq,irq->irq_cfg.nvic_preemptPriority,irq->irq_cfg.nvic_subPriority);
			HAL_NVIC_EnableIRQ(irq->irq_cfg.irq);
		}
		timer_dev->IQRInited = false;
		timer_dev->isInit = true;
		*timer_tim_id = (uint32_t)timer_dev;
		HAL_TIM_Base_Start_IT(timer_dev->TimerCfg->tim);
	}else
	{
		timer_dev->IQRInited = false;
		timer_dev->isInit = true;
		*timer_tim_id = (uint32_t)timer_dev;
		HAL_TIM_Base_Start(timer_dev->TimerCfg->tim);
	}

}

uint8_t Wait_processing(uint16_t TMS)///////////////////////////////////////
{
	if(TIM6->CNT<TMS)
	{
		return 1;
	}
	else
	{
		TIM6->CNT=0;
		return 0;
	}
}

void TIM6_DAC_IRQnHandler(void)
{

}

/**********************************************************************************/
//定时器4中断
/**********************************************************************************/
void TIM4_IRQHandler(void)
{
	GIMBAL_TIM_TIMER_DEV *timer_dev = (GIMBAL_TIM_TIMER_DEV	*)Hal_motor_Tim_ID;
	DEBUG_Assert(timer_dev);

	if((timer_dev->TimerCfg->tim->Instance->DIER & TIM_IT_UPDATE) == TIM_IT_UPDATE)
	{
		timer_dev->TimerCfg->tim->Instance->SR &= ~TIM_IT_UPDATE;

	}
}
/**********************************************************************************/
//定时器5中断
/**********************************************************************************/
void TIM5_IRQHandler(void)
{
	GIMBAL_TIM_TIMER_DEV *timer_dev = (GIMBAL_TIM_TIMER_DEV	*)Hal_can_Tim_ID;
	DEBUG_Assert(timer_dev);

	if((timer_dev->TimerCfg->tim->Instance->DIER & TIM_IT_UPDATE) == TIM_IT_UPDATE)
	{
		timer_dev->TimerCfg->tim->Instance->SR &= ~TIM_IT_UPDATE;

	}
}
	
/**********************************************************************************/
//定时器7中断
/**********************************************************************************/
void TIM7_IRQHandler(void)
{
	GIMBAL_TIM_TIMER_DEV *timer_dev = (GIMBAL_TIM_TIMER_DEV	*)Hal_Print_Tim_ID;
	DEBUG_Assert(timer_dev);

	if((timer_dev->TimerCfg->tim->Instance->DIER & TIM_IT_UPDATE) == TIM_IT_UPDATE)
	{
		timer_dev->TimerCfg->tim->Instance->SR &= ~TIM_IT_UPDATE;

	}
}

#define SystickUsTick   (HAL_RCC_GetHCLKFreq()/1000000)
uint64_t GetMicro(void)
{
    uint64_t micro ;
    micro = halSysTickCount * 1000;
    micro += (SysTick->LOAD - SysTick->VAL)/SystickUsTick;
    return micro;
}

uint32_t GetMillis(void)
{
    return HAL_GetTick();
}

void delay_us(float us) //us no more than 1000ms
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;
    ticks= us*HAL_RCC_GetHCLKFreq()/1000000;        //HAL_RCC_GetHCLKFreq()/1000000 be the ticks when 1 us passed
    told=SysTick->VAL;
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=(told-tnow);
            else tcnt+=(reload-tnow+told);
            told=tnow;
            if(tcnt>=ticks)break;
        }
    }
}

void delay_ms(uint16_t ms)
{
	HAL_Delay(ms);
}

