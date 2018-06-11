/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gpio.h"
#include "string.h"
#include "usart.h"
#include "cpu_utils.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/

/* USER CODE BEGIN Variables */

#define START_TASK_PRIO           1
#define START_STK_SIZE            64
TaskHandle_t xHandleTaskStart;
void StartTask(void const * argument);

#define LED1_TASK_PRIO            31
#define LED1_STK_SIZE             256
TaskHandle_t xHandleTaskLED1;
void TaskLED1(void const * argument);

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
  xTaskCreate((TaskFunction_t )TaskLED1,
              (const char *  )"Task1",
              (uint16_t       )LED1_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED1_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED1);

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
void vTaskGetRunTimeStats(char *pcWriteBuffer1)
{
   TaskStatus_t*pxTaskStatusArray;
   volatile UBaseType_t uxArraySize, x;
   unsigned long ulTotalRunTime, ulStatsAsPercentage;
   char pcWriteBuffertt[400]={0};
   char *pcWriteBuffer = pcWriteBuffertt;

   /* 防御性代码，确保字符串有合理的结束*/


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

		   sprintf( pcWriteBuffer, "%s\t%u\t%u\t%u\t%u%%\r\n",
							 pxTaskStatusArray[ x ].pcTaskName,
							 pxTaskStatusArray[ x ].uxCurrentPriority,
							 pxTaskStatusArray[ x ].usStackHighWaterMark,
							 pxTaskStatusArray[ x ].xTaskNumber,
							 ulStatsAsPercentage );
		   HAL_UART_Transmit(&huart1,(uint8_t *)pcWriteBuffer,strlen( ( char* ) pcWriteBuffer ),0xFFFF);
		   while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);
         }
		   HAL_UART_Transmit(&huart1,(uint8_t *)"\r\n",sizeof("\r\n"),0xFFFF);
		   while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);
      }

      /* 释放之前申请的内存*/
     vPortFree( pxTaskStatusArray );
   }
}
//LED1任务
void TaskLED1(void const * argument)
{
  portTickType xLastWakeTime;
  UBaseType_t uxHighWaterMark;
  static uint16_t usage = 0;
  volatile UBaseType_t uxArraySize, x;
  uint32_t num  = 0;
  char buffer[100];
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  uxHighWaterMark =uxTaskGetStackHighWaterMark( xHandleTaskLED1 );
  uxArraySize = uxTaskGetNumberOfTasks ();
  while(1)
  {
    //获取CPU使用率并串口打印
    usage = osGetCPUUsage();
    uxHighWaterMark =uxTaskGetStackHighWaterMark( xHandleTaskLED1 );
    uxArraySize = uxTaskGetNumberOfTasks ();
    num = sprintf(buffer,"cpu:%d%%,Water:%d,Num:%d\r\n",usage,uxHighWaterMark,uxArraySize);

    HAL_UART_Transmit(&huart1,(uint8_t *)buffer,num,0xFFFF);
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);
    vTaskGetRunTimeStats(NULL);
    vTaskDelayUntil(&xLastWakeTime,(1000/portTICK_RATE_MS));
  }
}

//LED2任务
void TaskLED2(void const * argument)
{
  portTickType xLastWakeTime;

  static  uint16_t usage = 0;
  uint32_t num  = 0;
  char buffer[100];

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);
	HAL_Delay(50);
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
  }
}

//LED3任务
void TaskLED3(void const * argument)
{
  portTickType xLastWakeTime;

  static  uint16_t usage = 0;
  uint32_t num  = 0;
  char buffer[100];

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);
	HAL_Delay(100);
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
  }
}

//LED4任务
void TaskLED4(void const * argument)
{
  portTickType xLastWakeTime;

  static  uint16_t usage = 0;
  uint32_t num  = 0;
  char buffer[100];

  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //LED闪烁
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_2);
	HAL_Delay(150);
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
  }
}
/* USER CODE END Application */
