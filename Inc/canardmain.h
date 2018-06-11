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
extern uint8_t GetPitchEncoder;
extern ModulesStatusInfo SysStatusInfo;

void CanardMainInit(void);
void GimbalResponse(uint8_t* data);
void PitchResponse(uint16_t value);

#define CANARD_STM32_GIMBAL_NODE_ID 40

#define GIMBAL_REBOOT_ID1		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X180)
#define GIMBAL_REBOOT_ID2		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X280)
#define GIMBAL_REBOOT_ID3		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X380)
#define GIMBAL_REBOOT_ID4		(uint16_t)(CANARD_STM32_GIMBAL_NODE_ID + 0X480)

#define UAVCAN_GIMBAL_SET_SPEED_MESSAGE_SIZE						12
#define UAVCAN_GIMBAL_SET_SPEED_DATA_TYPE_ID						2035	
#define UAVCAN_GIMBAL_SET_SPEED_TYPE_SIGNATURE						0xB50F21998097AA6E

#define UAVCAN_GIMBAL_SET_VECTOR_MESSAGE_SIZE						12
#define UAVCAN_GIMBAL_SET_VECTOR_DATA_TYPE_ID						2034	
#define UAVCAN_GIMBAL_SET_VECTOR_TYPE_SIGNATURE						0x717BFF07C7592C1A

#define UAVCAN_GIMBAL_SET_CURRENT_MESSAGE_SIZE						12
#define UAVCAN_GIMBAL_SET_CURRENT_DATA_TYPE_ID						2033	
#define UAVCAN_GIMBAL_SET_CURRENT_TYPE_SIGNATURE					0xE3B2F7D326579DE6

#define UAVCAN_GIMBAL_CMD_CONTROL_MESSAGE_SIZE						1
#define UAVCAN_GIMBAL_CMD_CONTROL_DATA_TYPE_ID						2032	
#define UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE					0xBCAF79031A14541

#define UAVCAN_GIMBAL_PIT_RESPONSE_MESSAGE_SIZE						2
#define UAVCAN_GIMBAL_PIT_RESPONSE_DATA_TYPE_ID						2031	
#define UAVCAN_GIMBAL_PIT_RESPONSE_TYPE_SIGNATURE					0xAB7AC799B3B7074E

#define UAVCAN_GIMBAL_ROL_RESPONSE_MESSAGE_SIZE						2
#define UAVCAN_GIMBAL_ROL_RESPONSE_DATA_TYPE_ID						2030	
#define UAVCAN_GIMBAL_ROL_RESPONSE_TYPE_SIGNATURE					0xA5872868E1C79B73

#define UAVCAN_GIMBAL_YAW_RESPONSE_MESSAGE_SIZE						2
#define UAVCAN_GIMBAL_YAW_RESPONSE_DATA_TYPE_ID						2029	
#define UAVCAN_GIMBAL_YAW_RESPONSE_TYPE_SIGNATURE					0x7D5588158A20EE9E

#define UAVCAN_GIMBAL_SET_MOTORSWITCH_MESSAGE_SIZE					3
#define UAVCAN_GIMBAL_SET_MOTORSWITCH_DATA_TYPE_ID					2028	
#define UAVCAN_GIMBAL_SET_MOTORSWITCH_TYPE_SIGNATURE				0x170427160B10D8EF

#define UAVCAN_GIMBAL_INFO_RESPONSE_MESSAGE_SIZE					7
#define UAVCAN_GIMBAL_INFO_RESPONSE_DATA_TYPE_ID					2027	
#define UAVCAN_GIMBAL_INFO_RESPONSE_TYPE_SIGNATURE					0x6662729ECF404E87

#ifdef __cplusplus
 }
#endif
#endif /* CANARDMAIN_H_ */
