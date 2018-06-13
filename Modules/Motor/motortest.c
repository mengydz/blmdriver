#include "motortest.h"
#include "gpio.h"
#include "global.h"
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


//Task任务
void MotoTestTask(void const * argument)
{
  portTickType xLastWakeTime;
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
	vTaskDelayUntil(&xLastWakeTime,(10/portTICK_RATE_MS));

  }
}


