/*
 * chip.h
 *
 *  Created on: Oct 9, 2016
 *      Author: tony
 */

#ifndef CHIP_H_
#define CHIP_H_

#include "stm32f4xx_hal.h"

#define STM32_SYSCLK_FREQUENCY  180000000ul

/* AHB clock (HCLK) is SYSCLK (168MHz) */

#define STM32_RCC_CFGR_HPRE     RCC_CFGR_HPRE_SYSCLK  /* HCLK  = SYSCLK / 1 */
#define STM32_HCLK_FREQUENCY    STM32_SYSCLK_FREQUENCY
#define STM32_BOARD_HCLK        STM32_HCLK_FREQUENCY  /* same as above, to satisfy compiler */

/* APB1 clock (PCLK1) is HCLK/4 (42MHz) */

#define STM32_PCLK1_FREQUENCY		(STM32_HCLK_FREQUENCY/4)
#define STM32_PCLK2_FREQUENCY		(STM32_HCLK_FREQUENCY/2)
/* Timers driven from APB1 will be twice PCLK1 */

#define STM32_APB1_TIM2_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM3_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM4_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM5_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM6_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM7_CLKIN   (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM12_CLKIN  (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM13_CLKIN  (2*STM32_PCLK1_FREQUENCY)
#define STM32_APB1_TIM14_CLKIN  (2*STM32_PCLK1_FREQUENCY)

#define STM32_APB2_TIM1_CLKIN		(2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM8_CLKIN		(2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM9_CLKIN		(2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM10_CLKIN		(2*STM32_PCLK2_FREQUENCY)
#define STM32_APB2_TIM11_CLKIN		(2*STM32_PCLK2_FREQUENCY)


#define STM32_TIMCLK1           STM32_APB1_TIM2_CLKIN

#define STM32_APB_TIM_FRE_GET(Instance)		(( (uint32_t)Instance == (uint32_t)TIM2 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM3 ) 	\
										||	( (uint32_t)Instance == (uint32_t)TIM4 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM5 ) 	\
										||	( (uint32_t)Instance == (uint32_t)TIM6 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM7 ) 	\
										||	( (uint32_t)Instance == (uint32_t)TIM12 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM13 ) 	\
										||	( (uint32_t)Instance == (uint32_t)TIM14 ))	\
										? (2*STM32_PCLK1_FREQUENCY):(					\
											(( (uint32_t)Instance == (uint32_t)TIM1 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM8 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM9 ) 	\
										||	( (uint32_t)Instance == (uint32_t)TIM10 )	\
										||	( (uint32_t)Instance == (uint32_t)TIM11 )) 	\
										? (2*STM32_PCLK2_FREQUENCY):0)





#endif /* CHIP_H_ */

