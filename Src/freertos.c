/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "string.h"
#include "cpu_utils.h"
#include "gpio.h"
#include "board_hw_defs.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/

/* USER CODE BEGIN Variables */

#define START_TASK_PRIO           1
#define START_STK_SIZE            64
TaskHandle_t xHandleTaskStart;
void StartTask(void const * argument);

#define Proto_TASK_PRIO            31
#define Proto_STK_SIZE             128
TaskHandle_t xHandleTaskProto;
void ProtocolTask(void const * argument);

#define LED2_TASK_PRIO            30
#define LED2_STK_SIZE             128
TaskHandle_t xHandleTaskLED2;
void TaskLED2(void const * argument);

#define LED3_TASK_PRIO            29
#define LED3_STK_SIZE             128
TaskHandle_t xHandleTaskLED3;
void TaskLED3(void const * argument);

#define LED4_TASK_PRIO            28
#define LED4_STK_SIZE             128
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

  xTaskCreate((TaskFunction_t )TaskLED2,
              (const char *  )"Task2",
              (uint16_t       )LED2_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED2_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED2);

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

/*本例演示如是使用uxTaskGetSystemState()函数来获取运行时间信息，并将其转化为程序员更易识别的字符格式，这些转化后的字符保存到pcWriteBuffer中。*/
void vTaskGetRunTimeStats(char *pcWriteBuffer)
{
	TaskStatus_t*pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;
	/* 获取任务总数目*/
	uxArraySize = uxTaskGetNumberOfTasks ();
	/*为每个任务的TaskStatus_t结构体分配内存，也可以静态的分配一个足够大的数组 */
	pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ));
	if(pxTaskStatusArray != NULL )
	{
		/*获取每个任务的状态信息 */
		uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize,&ulTotalRunTime );
		/* 百分比计算 */
		ulTotalRunTime /= 100UL;
		/* 避免除零错误 */
		if(ulTotalRunTime > 0 )
		{
			/* 将获得的每一个任务状态信息部分的转化为程序员容易识别的字符串格式*/
			for( x = 0; x < uxArraySize; x++ )
			{
				/* 计算任务运行时间与总运行时间的百分比。*/
				ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter /ulTotalRunTime;
				printf("%s  %u  %u  %u  %u%%\n",
						pxTaskStatusArray[ x ].pcTaskName,
						pxTaskStatusArray[ x ].uxCurrentPriority,
						pxTaskStatusArray[ x ].usStackHighWaterMark,
						pxTaskStatusArray[ x ].xTaskNumber,
						ulStatsAsPercentage );
			}
		}
		/* 释放之前申请的内存*/
		vPortFree( pxTaskStatusArray );
	}
}
//LED2任务
void TaskLED2(void const * argument)
{
  portTickType xLastWakeTime;
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
	LedToggle(LED_RED);
	HAL_Delay(1);
    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
  }
}

//LED3任务
void TaskLED3(void const * argument)
{
  portTickType xLastWakeTime;

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
	printf("cpu:%d%%\n",osGetCPUUsage());
	LedToggle(LED_GREEN);
    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
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
	LedToggle(LED_BLUE);
    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
  }
}
/* USER CODE END Application */
