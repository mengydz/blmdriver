/*
 * gpio.c
 *
 *  Created on: Sep 20, 2016
 *      Author: baron
 */
#include "gpio.h"
#include "myMath.h"

static const DriverLed driverLeds[] = {
        [LED_BLUE] = {
            .pin = {
                .gpio = GPIOC,
                .initTypeDef = {
                    .Pin   = GPIO_PIN_0,
                    .Speed = GPIO_SPEED_FREQ_HIGH,
                    .Mode  = GPIO_MODE_OUTPUT_PP,
                    .Pull  = GPIO_NOPULL,
                },
            },
            .active_high = false,
        },
        [LED_GREEN] = {
            .pin = {
                .gpio = GPIOC,
                .initTypeDef = {
                    .Pin   = GPIO_PIN_1,
                    .Speed = GPIO_SPEED_FREQ_HIGH,
                    .Mode  = GPIO_MODE_OUTPUT_PP,
                    .Pull  = GPIO_NOPULL
                },
            },
            .active_high = false,
        },
        [LED_RED] = {
            .pin = {
                .gpio = GPIOC,
                .initTypeDef = {
                    .Pin   = GPIO_PIN_2,
                    .Speed = GPIO_SPEED_FREQ_HIGH,
                    .Mode  = GPIO_MODE_OUTPUT_PP,
                    .Pull  = GPIO_NOPULL
                },
            },
            .active_high = false,
        },          
};

static const DriverGpio GimbalMotorSwitch = {
	.pin = {
		.gpio = GPIOC,
		.initTypeDef = {
			.Pin   = GPIO_PIN_5,
			.Speed = GPIO_SPEED_FREQ_HIGH,
			.Mode  = GPIO_MODE_OUTPUT_PP,
			.Pull  = GPIO_NOPULL
		},
	},
	.active_high = true,
};

static const DriverLedCfg driverLedCfg = {
    .leds     = driverLeds,
    .num_leds = NELEMENTS(driverLeds),
};

void LedOff(uint8_t ledID)
{
	if(ledID > driverLedCfg.num_leds)
		return;

	const DriverLed *led = &(driverLedCfg.leds[ledID]);

	if(led->active_high)
	{
		HAL_GPIO_WritePin(led->pin.gpio,led->pin.initTypeDef.Pin,GPIO_PIN_RESET);
	}else
	{
		HAL_GPIO_WritePin(led->pin.gpio,led->pin.initTypeDef.Pin,GPIO_PIN_SET);
	}
}

void LedOn(uint8_t ledID)
{
	if(ledID > driverLedCfg.num_leds)
		return;

	const DriverLed *led = &(driverLedCfg.leds[ledID]);

	if(led->active_high)
	{
		HAL_GPIO_WritePin(led->pin.gpio,led->pin.initTypeDef.Pin,GPIO_PIN_SET);
	}else
	{
		HAL_GPIO_WritePin(led->pin.gpio,led->pin.initTypeDef.Pin,GPIO_PIN_RESET);
	}
}

void LedToggle(int8_t ledID)
{
	if(ledID > driverLedCfg.num_leds)
		return;

	const DriverLed *led = &(driverLedCfg.leds[ledID]);
	HAL_GPIO_TogglePin(led->pin.gpio,led->pin.initTypeDef.Pin);
}

void BoardLedGpioInit(const DriverLedCfg *ledCfg)
{
    for(uint8_t i = 0;i<ledCfg->num_leds;i++)
    {
        const DriverLed *led = &(ledCfg->leds[i]);
        HAL_RCC_CLK_ENABLE((GPIO_TypeDef *)led->pin.gpio);
        HAL_GPIO_Init((GPIO_TypeDef *)led->pin.gpio,(GPIO_InitTypeDef *)&(led->pin.initTypeDef));
        LedOff(i);
    }
}

void GimbalMotorSwitchGpioInit(void)
{
	HAL_RCC_CLK_ENABLE((GPIO_TypeDef *)GimbalMotorSwitch.pin.gpio);
	HAL_GPIO_Init((GPIO_TypeDef *)GimbalMotorSwitch.pin.gpio,(GPIO_InitTypeDef *)&(GimbalMotorSwitch.pin.initTypeDef));
	MotorSwitchOff();
}

DriverLedCfg const*GetBoardLedGpioCfg(void)
{
    return &driverLedCfg;
}

void MotorSwitchOn(void)
{
	//printf("MotorSwitch On\r\n");
	if(GimbalMotorSwitch.active_high != false)
		HAL_GPIO_WritePin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin,GPIO_PIN_RESET);
}

void MotorSwitchOff(void)
{
	//printf("MotorSwitch Off\r\n");
	if(GimbalMotorSwitch.active_high != false)
		HAL_GPIO_WritePin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin,GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin,GPIO_PIN_SET);
}

bool CheckMotorSwitchStatus(void)
{
	if(GimbalMotorSwitch.active_high != false)
	{
		if(HAL_GPIO_ReadPin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin) == GPIO_PIN_SET)
			return true;
		else
			return false;
	}else
	{
		if(HAL_GPIO_ReadPin(GimbalMotorSwitch.pin.gpio,GimbalMotorSwitch.pin.initTypeDef.Pin) == GPIO_PIN_SET)
			return false;
		else
			return true;
	}
}


