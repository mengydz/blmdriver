/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTORDRIVER_H
#define __MOTORDRIVER_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "board_hw_defs.h"
#include "motorConfig.h"
#include "tim_PWM_Output.h"

 typedef struct{

 	float				out;
 	uint16_t			encoderPos;
 	uint16_t			vectorPos;

 	bool				isClosedLoop;

 	bool				isCurrentLoop;

 	MotorCfg	const 	*cfg;
 	uint32_t			svpwm_tim_id;
 #if defined(PIOS_INCLUDE_FREERTOS)
 	struct pios_mutex	*svpwmUseMutex;
 #else
 	bool				svpwmUseBusy;
 	void (*delay_ms)(uint16_t timeout);
 #endif
 	uint32_t			Magic;

 	PulseUpdate         updataFun;
 }SvpwmDrive;

#define PHASE1_MAX_RADVECTOR		2048
#define PHASE2_MAX_RADVECTOR		3413
#define PHASE3_MAX_RADVECTOR		683

#define MotorPwmFre                 (uint16_t)15000

#define MotorPhaseTimerPeriod       (uint16_t)(180000000/(MotorPwmFre*2)-1)


#if !defined(PIOS_INCLUDE_FREERTOS)
typedef void (*DelayMsFun)(uint16_t tick);
extern bool RegisterDelayMsFunction(uint32_t svpwmId,DelayMsFun fun);
#endif


extern uint32_t	svpwmID;
extern const MotorDriver svpwmDri;

/*
 *
 */
bool SvpwmDriverPulseUpdateFunRegister(uint32_t *svpwmid,uint32_t svpwm_tim_id,uint32_t Update);


#ifdef __cplusplus
}
#endif
#endif /*__ MOTOR_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
