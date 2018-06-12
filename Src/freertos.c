/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "notify.h"
#include "string.h"
#include "cpu_utils.h"
#include "gpio.h"
#include "timer.h"
#include "board_hw_defs.h"
#include "canardmain.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/

/* USER CODE BEGIN Variables */

#define START_TASK_PRIO           1
#define START_STK_SIZE            64
TaskHandle_t xHandleTaskStart;
void StartTask(void const * argument);

#define Proto_TASK_PRIO            31
#define Proto_STK_SIZE             256
TaskHandle_t xHandleTaskProto;
void ProtocolTask(void const * argument);

#define Notify_TASK_PRIO            30
#define Notify_STK_SIZE             256
TaskHandle_t xHandleTaskNotify;
void NotifyTask(void const * argument);

#define LED3_TASK_PRIO            29
#define LED3_STK_SIZE             256
TaskHandle_t xHandleTaskLED3;
void TaskLED3(void const * argument);

#define LED4_TASK_PRIO            28
#define LED4_STK_SIZE             256
TaskHandle_t xHandleTaskLED4;
void TaskLED4(void const * argument);

void MX_FREERTOS_Init(void) {

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  //创建开始任务
  xTaskCreate((TaskFunction_t )StartTask,
        (const char *  )"Start Task",
        (uint16_t       )START_STK_SIZE,
        (void *         )NULL,
        (UBaseType_t    )START_TASK_PRIO,
        (TaskHandle_t * )&xHandleTaskStart);
  /* USER CODE END RTOS_QUEUES */
}

//开始任务，用于创建两个新的测试任务
void StartTask(void const * argument)
{
  taskENTER_CRITICAL();
  xTaskCreate((TaskFunction_t )ProtocolTask,
              (const char *  )"ProtocolTask",
              (uint16_t       )Proto_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Proto_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskProto);

  xTaskCreate((TaskFunction_t )NotifyTask,
              (const char *  )"NotifyTask",
              (uint16_t       )Notify_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Notify_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskNotify);

  xTaskCreate((TaskFunction_t )TaskLED3,
              (const char *  )"Task3",
              (uint16_t       )LED3_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED3_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED3);

  xTaskCreate((TaskFunction_t )TaskLED4,
              (const char *  )"Task4",
              (uint16_t       )LED4_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED4_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED4);

  vTaskDelete(xHandleTaskStart);
  taskEXIT_CRITICAL();
}

//LED3任务
void TaskLED3(void const * argument)
{
  portTickType xLastWakeTime;

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
//	vTaskGetRunTimeStats(NULL);
	printf("cpu:%d%%\n",osGetCPUUsage());
//    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
	osDelay(1000);
  }
}

//LED4任务
void TaskLED4(void const * argument)
{
  portTickType xLastWakeTime;

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
//	SetGimbalAngle();
	for(uint32_t k=0;k<45567;k++);
    vTaskDelayUntil(&xLastWakeTime,(10/portTICK_RATE_MS));
  }
}
/* USER CODE END Application */
