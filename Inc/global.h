#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "string.h"



#define MAXMOTORPOWER		1.0f
#define MINMOTORPOWER		-1.0f

#define COMPILE_YEAR		2018
#define COMPILE_MONTH		5
#define COMPILE_DAY			28
#define GIMBAL_INFO_SW_VER	100

#define MotorPole			11
#define EncoderPPR			4096

#define ENCODER_REVERSE		false
#define ENCODER_PLOT		0	
#define GPPROTOCOL			1
#define LEDNotify			1
#define CANARDUSEDCRC		0