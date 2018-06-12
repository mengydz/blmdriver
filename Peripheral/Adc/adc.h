/*
 * adc.h
 *
 *  Created on: Oct 1, 2016
 *      Author: baron
 */

#ifndef ADC_H_
#define ADC_H_
#ifdef __cplusplus
 extern "C" {
#endif



#include "board_hw_defs.h"

 extern uint32_t hal_ADC_pwmout_sample_id;
 extern uint32_t hal_ADC_Vol_ID;

void ADCSampleInit(uint32_t *adcid,const GIMBAL_ADC_CFG *cfg,bool isDmaUsed);

void SetDMAADC_INT_FOCProcessId(uint32_t adcId,uint32_t id);

void ADCSample(uint32_t id,uint16_t sample[],uint8_t channelnum);

uint16_t ADCGetLastAndStartNewSample(uint32_t adcId,uint8_t channelNum);

uint32_t GetAdcSampleResoltuion(uint32_t adc_id);
#ifdef __cplusplus
 }
#endif
#endif /* ADC_H_ */
