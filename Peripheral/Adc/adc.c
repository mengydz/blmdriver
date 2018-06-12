/*
 * adc.c
 *
 *  Created on: Oct 1, 2016
 *      Author: baron
 */ 
#include "adc.h"
#include "tim_PWM_Output.h"
#include "FreeRTOS.h"

uint32_t hal_ADC_pwmout_sample_id;
uint32_t hal_ADC_Vol_ID;

uint32_t DMA2_Stream0_id;
uint32_t DMA2_Stream1_id;

typedef struct{
	GIMBAL_ADC_CFG *cfg;
	uint32_t    FocDriverId;
	struct pios_mutex	 *ADCBusyMutex;
	uint16_t ADCSampleArr[3];
	uint32_t adcMagic;
}GimbalADCDev;

#define ADC_MAGIC       0x02958abf

void ADCSampleInit(uint32_t *adc_id,const GIMBAL_ADC_CFG *cfg,bool isDmaUsed)
{
    GimbalADCDev *dev = (GimbalADCDev *)pvPortMalloc(sizeof(GimbalADCDev));

	DEBUG_Assert(dev);

	dev->cfg = (GIMBAL_ADC_CFG *)cfg;

	dev->adcMagic = ADC_MAGIC;
	
	HAL_RCC_CLK_ENABLE(dev->cfg->hadc->Instance);
	
	if(HAL_ADC_Init(dev->cfg->hadc) != HAL_OK)
	{
		DEBUG_Assert(0);
	}

	for(uint8_t i= 0;i<dev->cfg->channelNum;i++)
	{
		HAL_RCC_CLK_ENABLE(dev->cfg->ADCGpio[i].gpio);
		HAL_GPIO_Init(dev->cfg->ADCGpio[i].gpio,&dev->cfg->ADCGpio[i].initTypeDef);

		if(HAL_ADC_ConfigChannel(dev->cfg->hadc,&dev->cfg->sConfig[i]) != HAL_OK )
		{
			DEBUG_Assert(0);
		}
	}

	if(dev->cfg->hdma->Instance == DMA2_Stream0)
	{
	    DMA2_Stream0_id = (uint32_t)dev;
	}else if(dev->cfg->hdma->Instance == DMA2_Stream1)
	{
	    DMA2_Stream1_id = (uint32_t)dev;
	}
	
	__HAL_ADC_ENABLE(dev->cfg->hadc);

	if(isDmaUsed)
	{
		HAL_RCC_CLK_ENABLE(dev->cfg->hdma->Instance);

		if(HAL_DMA_Init(dev->cfg->hdma) != HAL_OK)
		{
			DEBUG_Assert(1);
		}

		dev->cfg->hadc->DMA_Handle = dev->cfg->hdma;
		dev->cfg->hdma->Parent = (dev->cfg->hadc);

		if(dev->cfg->dmairq.irq_enabled)
		{
			STM32_IRQ_CFG *irq = &dev->cfg->dmairq;
			if(irq->irq_enabled == true)
			{
				HAL_NVIC_SetPriority(irq->irq_cfg.irq,irq->irq_cfg.nvic_preemptPriority,irq->irq_cfg.nvic_subPriority);
				HAL_NVIC_EnableIRQ(irq->irq_cfg.irq);
			}
		}
		HAL_ADC_Start_DMA(dev->cfg->hadc,(uint32_t*)dev->ADCSampleArr,dev->cfg->channelNum);
	}
	*adc_id = (uint32_t)dev;

}

static bool ADCValidate(GimbalADCDev *dev)
{
    if(dev == NULL)
        return false;
    if(dev->adcMagic != ADC_MAGIC)
        return false;
    return true;
}

void SetDMAADC_INT_FOCProcessId(uint32_t adcId,uint32_t id)
{
    GimbalADCDev *dev = (GimbalADCDev *)adcId;
    DEBUG_Assert(dev);  
    dev->FocDriverId = id;
}

void ADCSample(uint32_t adc_id,uint16_t sample[],uint8_t channelNum)
{

	GimbalADCDev *dev = (GimbalADCDev *)adc_id;
	DEBUG_Assert(dev);  

	for(uint8_t i = 0;i<dev->cfg->channelNum && i < channelNum;i++)
	{
		dev->cfg->hadc->Instance->SQR3 = dev->cfg->sConfig[i].Channel; 			 
		dev->cfg->hadc->Instance->CR2	|= (uint32_t)(ADC_CR2_SWSTART ); 
		while(!(dev->cfg->hadc->Instance->SR & ADC_FLAG_EOC)); 		 
		sample[i] = (uint16_t)((dev->cfg->hadc->Instance->DR) & 0x0FFF);
	}
}

uint32_t GetAdcSampleResoltuion(uint32_t adc_id)
{
	GimbalADCDev *dev = (GimbalADCDev *)adc_id;
	DEBUG_Assert(dev); 
	return dev->cfg->hadc->Init.Resolution;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
        GimbalADCDev *dev = (GimbalADCDev *)hal_ADC_pwmout_sample_id;
        ADCValidate(dev);
        if(hadc == dev->cfg->hadc)
        {
//            CurrentRunning(dev->FocDriverId,dev->ADCSampleArr);
//            break;
        }
}

void DMA2_Stream0_IRQHandler(void)
{
	GimbalADCDev *dev;
	dev = (GimbalADCDev *)DMA2_Stream0_id;
	HAL_DMA_IRQHandler(dev->cfg->hdma);
}

void DMA2_Stream1_IRQHandler(void)
{
    GimbalADCDev *dev;
    dev = (GimbalADCDev *)DMA2_Stream1_id;
    HAL_DMA_IRQHandler(dev->cfg->hdma);
}
