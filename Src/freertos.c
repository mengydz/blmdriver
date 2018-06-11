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
osThreadId defaultTaskHandle;

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
#define LED2_STK_SIZE             256
TaskHandle_t xHandleTaskLED2;
void TaskLED2(void const * argument);

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
  xTaskCreate((TaskFunction_t )TaskLED1,
              (const char *  )"LED1 Task",
              (uint16_t       )LED1_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED1_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED1);

  xTaskCreate((TaskFunction_t )TaskLED2,
              (const char *  )"LED2 Task",
              (uint16_t       )LED2_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED2_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED2);

  xTaskCreate((TaskFunction_t )TaskLED3,
              (const char *  )"LED3 Task",
              (uint16_t       )LED3_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED3_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED3);

  xTaskCreate((TaskFunction_t )TaskLED4,
              (const char *  )"LED4 Task",
              (uint16_t       )LED4_STK_SIZE,
              (void *         )NULL,
              (UBaseType_t    )LED4_TASK_PRIO,
              (TaskHandle_t * )&xHandleTaskLED4);

  vTaskDelete(xHandleTaskStart);
  taskEXIT_CRITICAL();
}
uint16_t usage = 0;
//LED1任务
void TaskLED1(void const * argument)
{
  portTickType xLastWakeTime;
//  static uint16_t usage = 0;
  uint32_t num  = 0;
  char buffer[100];
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    //获取CPU使用率并串口打印
    usage = osGetCPUUsage();
    num = sprintf(buffer,"cpu usage:%d%%\r\n",usage);

    HAL_UART_Transmit(&huart1,(uint8_t *)buffer,num,0xFFFF);
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);
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
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
    HAL_Delay(100);
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
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
    HAL_Delay(100);
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
    vTaskDelayUntil(&xLastWakeTime,(500/portTICK_RATE_MS));
    HAL_Delay(200);
  }
}
/* USER CODE END Application */
