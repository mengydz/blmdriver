/*
 * adc.c
 *
 *  Created on: Oct 1, 2016
 *      Author: baron
 */ 
#include "adc.h"
#include "tim_PWM_Output.h"
#include "FreeRTOS.h"
#include "current.h"

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
		__HAL_DMA_DISABLE_IT(dev->cfg->hdma,DMA_IT_HT);
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

typedef struct
{
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;
void DMA2_Stream0_IRQHandler(void)
{
	GimbalADCDev *dev = (GimbalADCDev *)DMA2_Stream0_id;
	DMA_Base_Registers *regs = (DMA_Base_Registers *)dev->cfg->hdma->StreamBaseAddress;
	if ((regs->ISR & (DMA_FLAG_TCIF0_4 << dev->cfg->hdma->StreamIndex)) != RESET)
	{
		if(__HAL_DMA_GET_IT_SOURCE(dev->cfg->hdma, DMA_IT_TC) != RESET)
		{
			/* Clear the transfer complete flag */
			regs->IFCR = DMA_FLAG_TCIF0_4 << dev->cfg->hdma->StreamIndex;
			CurrentRunning(dev->FocDriverId,dev->ADCSampleArr);
		}
	}
}
