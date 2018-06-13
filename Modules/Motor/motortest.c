#include "motortest.h"
#include "gpio.h"
#include "global.h"
#include "motordriver.h"
#include "timer.h"
#include "svpwm.h"
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

static MotorCfg		motorCfg={
	.encodePPR = 4096,
	.pole = 11,
	.PhasePulseMax = 2000,//MotorPhaseTimerPeriod,
	.isUplseReverse = MotorRotateReverse_ACB,//MOTOR_Y_ROTATE_REVERSE,
};


//Task任务
void MotoTestTask(void const * argument)
{
  portTickType xLastWakeTime;
  svpwmArrayQ12Init();
  svpwmDri.SetMotorConfig(svpwmID,(uint32_t)&motorCfg);
  SvpwmDriverPulseUpdateFunRegister(&svpwmID,Hal_Tim_pwmOut_ID,(uint32_t)MotorSvpwmTimPulseUpdate);
  MotorSwitchOn();
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
	vTaskDelayUntil(&xLastWakeTime,(10/portTICK_RATE_MS));
//	svpwmDri.setUse(svpwmID);
	svpwmDri.outPut(svpwmID,0.1,0,GetMillis(),false);
//	svpwmDri.releaseUse(svpwmID);
  }
}


