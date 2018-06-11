#ifndef DRIVER_STM32_H_
#define DRIVER_STM32_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "chip.h"

#define DEBUG_Assert(test)  while (!(test))

typedef struct{
	GPIO_TypeDef 		*gpio;
	GPIO_InitTypeDef	initTypeDef;
}STM32_GPIO;

typedef struct{
	IRQn_Type			irq;
	uint32_t			nvic_preemptPriority;
	uint32_t			nvic_subPriority;
}STM32_IRQ;

typedef struct{
	bool		irq_enabled;
	STM32_IRQ	irq_cfg;
	uint32_t	irqFlag[5];
	uint8_t		irqFlagNum;
}STM32_IRQ_CFG;

//--------------------------------
//Interrupt Priorites
//--------------------------------

#define IRQ_PRIO_LOW				12		//lower than rtos
#define IRQ_PRIO_MID				8		//HIGHER THAN RTOS
#define IRQ_PRIO_HIGH				5		//for spi adc i2c etc
//#define IRQ_PRIO_HIGHEST			4		//for uart etc..

//void Error_Handler(void);

#define PIOS_I2C_MAX_DEVS         	3
#define PIOS_USART_MAX_DEVS			2
#define PIOS_COM_MAX_DEVS			2
#define ENCODER_DATA_CHANNEL_Max	2
#define MOTOR_OUTPUT_CHANNEL_Max	2

#define PlanStdId1 0x123                      //这里采用4个标准CAN ID
#define PlanStdId2 0x124                      //这里采用4个标准CAN ID
#define PlanStdId3 0x125                      //这里采用4个标准CAN ID
#define PlanStdId4 0x126                      //这里采用4个标准CAN ID

#define GimbalStdId1 0x123                      //这里采用4个标准CAN ID
#define GimbalStdId2 0x124                      //这里采用4个标准CAN ID
#define GimbalStdId3 0x125                      //这里采用4个标准CAN ID
#define GimbalStdId4 0x126                      //这里采用4个标准CAN ID

/*----------------------------------------------------------------------------------------*/
//Last section of flash , 128KB
#define FlashInterDataAddrBase                  (uint32_t)0x0800C000
 /*
  * 所有校准数据预设占用内存最大数量
  */
#define FlashInternCaliMemMax                   (uint32_t)1024*3

/*
 *  三相电机零电相位下的机械相位 uint16_t * 3 = 6 byte  + checksum(uint32_t) 4 + magic(uint32_t) 4 = 14byte
 *  设立内存32byte (0x20)
 */
#define FlashInterMotorZeroPosAddr              FlashInterDataAddrBase
#define MotorZeroPosMemSize                     0x20
/*
 * 编码器非线性校准 uint16_t *3 (三个轴) = 6 byte * 56 (预计最大存储56个点) = 336 + 4 = 340 byte
 * 设定内存为512byte
 */
#define EncoderLinearCorrectAddr                (FlashInterMotorZeroPosAddr + MotorZeroPosMemSize)
#define EncoderLinearCorrectMemSize             0x200

#define FlashInterUserDataAddrBase				FlashInterDataAddrBase
#define InternFlashAddrBias(x)					(x - FlashInterUserDataAddrBase)

extern uint8_t caliDataRAMMap[FlashInternCaliMemMax];
#define GetFlashMapAddr(x)						((uint32_t)caliDataRAMMap + InternFlashAddrBias(x))

#ifdef __cplusplus
 }
#endif
#endif

