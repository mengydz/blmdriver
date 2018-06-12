/*
 * canardmain.h
 *
 *  Created on: Jan 22, 2017
 *      Author: baron
 */

#ifndef CANARDMAIN_H_
#define CANARDMAIN_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "canard.h"
#include "driver_stm32.h"
#include "global.h"
#include "gimbal_global_data.h"
 
extern uint8_t GetSysInfo;
extern ModulesStatusInfo SysStatusInfo;

void CanardMainInit(void);
void SetGimbalAngle(void);

#define CANARD_STM32_GIMBAL_NODE_ID 40

#define GIMBAL_REBOOT_ID1		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X180)
#define GIMBAL_REBOOT_ID2		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X280)
#define GIMBAL_REBOOT_ID3		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X380)
#define GIMBAL_REBOOT_ID4		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X480)

#define UAVCAN_AIRDATA_GIMBALCONTROLDATA_MESSAGE_SIZE           6
#define UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID           20554
#define UAVCAN_AIRDATA_GIMBALCONTROLDATA_TYPE_SIGNATURE         0x3B058FA5B150C5BEULL

#ifdef __cplusplus
 }
#endif
#endif /* CANARDMAIN_H_ */
