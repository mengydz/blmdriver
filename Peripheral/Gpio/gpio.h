/*
 * gpio.h
 *
 *  Created on: Sep 20, 2016
 *      Author: baron
 */

#ifndef GPIO_H_
#define GPIO_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "board_hw_defs.h"


void LedOn(uint8_t ledID);
void LedOff(uint8_t ledID);
void LedToggle(int8_t ledID);

void BoardLedGpioInit(const DriverLedCfg *ledCfg);
void GimbalMotorSwitchGpioInit(void);

DriverLedCfg const *GetBoardLedGpioCfg(void);

void MotorSwitchOn(void);
void MotorSwitchOff(void);
bool CheckMotorSwitchStatus(void);

#ifdef __cplusplus
 }
#endif
#endif /* GPIO_H_ */
