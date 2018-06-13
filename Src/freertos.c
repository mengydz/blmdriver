/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "notify.h"
#include "string.h"
#include "gpio.h"
#include "timer.h"
#include "board_hw_defs.h"
#include "canardmain.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/

/* USER CODE BEGIN Variables */

#define START_TASK_PRIO				1
#define START_STK_SIZE				64
TaskHandle_t xHandleTaskStart;
void StartTask(void const * argument);

#define Proto_TASK_PRIO				31
#define Proto_STK_SIZE				128
TaskHandle_t xHandleTaskProto;
void ProtocolTask(void const * argument);

#define Notify_TASK_PRIO			30
#define Notify_STK_SIZE        		128
TaskHandle_t xHandleTaskNotify;
void NotifyTask(void const * argument);

#define Canard_TASK_PRIO           	29
#define Canard_STK_SIZE            	256
TaskHandle_t xHandleTaskCanad;
void CanardmainTask(void const * argument);

#define Status_TASK_PRIO           	28
#define Status_STK_SIZE            	256
TaskHandle_t xHandleTaskStatus;
void StatusTask(void const * argument);

#define MotoTest_TASK_PRIO           	27
#define MotoTest_STK_SIZE            	128
TaskHandle_t xHandleTaskMotoTest;
void MotoTestTask(void const * argument);

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
              (const char *  )"Protocol",
              (uint16_t       )Proto_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Proto_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskProto);

  xTaskCreate((TaskFunction_t )NotifyTask,
              (const char *  )"Notify",
              (uint16_t       )Notify_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Notify_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskNotify);

  xTaskCreate((TaskFunction_t )CanardmainTask,
              (const char *  )"Canard",
              (uint16_t       )Canard_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Canard_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskCanad);

  xTaskCreate((TaskFunction_t )StatusTask,
              (const char *  )"Status",
              (uint16_t       )Status_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )Status_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskStatus);

  xTaskCreate((TaskFunction_t )MotoTestTask,
              (const char *  )"MotoTest",
              (uint16_t       )MotoTest_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )MotoTest_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskMotoTest);

  vTaskDelete(xHandleTaskStart);
  taskEXIT_CRITICAL();
}

//StatusTask任务
void StatusTask(void const * argument)
{
  portTickType xLastWakeTime;

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
	vTaskGetRunTimeStats(NULL);
//	xTaskNotifyGive( xHandleTaskStatus );
//	ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
  }
}

/* USER CODE END Application */
