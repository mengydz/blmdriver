#include "notify.h"
#include "gpio.h"
#include "global.h"
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

uint8_t LED_blink_tpye=0;
uint16_t out_led_idx_1=0;
uint16_t next_ms_1=0;
uint8_t show_step_1=0;


void Init_led_buz_state(void)
{
	Set_led_blink_type(11);
}

void SetLedColor(uint8_t color)
{
	switch(color)
	{
		case LED_GREEN:LedOff(LED_RED);LedOn(LED_GREEN);LedOff(LED_BLUE);break;//绿
		case LED_RED:LedOn(LED_RED);LedOff(LED_GREEN);LedOff(LED_BLUE);break;//红
		case LED_BLUE:LedOff(LED_RED);LedOff(LED_GREEN);LedOn(LED_BLUE);break;//蓝
		case LED_YELLOW:LedOn(LED_RED);LedOn(LED_GREEN);LedOff(LED_BLUE);break;//黄
		case LED_PINK:LedOn(LED_RED);LedOff(LED_GREEN);LedOn(LED_BLUE);break;//粉
		case LED_CYAN:LedOff(LED_RED);LedOn(LED_GREEN);LedOn(LED_BLUE);break;//青
		case LED_WHITE:LedOn(LED_RED);LedOn(LED_GREEN);LedOn(LED_BLUE);break;//白
		case LED_DARK:LedOff(LED_RED);LedOff(LED_GREEN);LedOff(LED_BLUE);break;//白
		default:LedOff(LED_RED);LedOff(LED_GREEN);LedOff(LED_BLUE);break;//黑
	}
}

void LED_control(void)  //  in 1ms loop
{
    switch(LED_blink_tpye)
    {
		case 0:
		{
			SetLedColor(LED_DARK);
		}break;
		case 1:
		{
			SetLedColor(LED_WHITE);
		}break;
		case 2://Green_Green_Green
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 3://Blue_Blue_Blue
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 4://Red_Red_Red
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_RED);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_RED);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_RED);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 5://Green_Green_Blue
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 6://Green_Green_Red
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_RED);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 7://B_B_G
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 8://BBR
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_RED);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 9://RRB
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_RED);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_RED);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_BLUE);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 10://RRG
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=50; SetLedColor(LED_RED);break;
					case 1:show_step_1=2;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 2:show_step_1=3;next_ms_1=50; SetLedColor(LED_RED);break;
					case 3:show_step_1=4;next_ms_1=450;SetLedColor(LED_DARK);break;
					case 4:show_step_1=5;next_ms_1=50; SetLedColor(LED_GREEN);break;
					case 5:show_step_1=0;next_ms_1=950;SetLedColor(LED_DARK);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		case 11://RGB
		{
			out_led_idx_1++;
			if(out_led_idx_1>=next_ms_1)
			{
				switch(show_step_1)
				{
					case 0:show_step_1=1;next_ms_1=500;SetLedColor(LED_RED);break;
					case 1:show_step_1=2;next_ms_1=500;SetLedColor(LED_GREEN);break;
					case 2:show_step_1=0;next_ms_1=500;SetLedColor(LED_BLUE);break;
					default:break;
				}
				out_led_idx_1=0;
			}
		}break;
		default:break;
    }

}

uint8_t last_tp=0;
void Set_led_blink_type(uint8_t tp)
{
//    if(Bat_arm_level!=0) return;
//    if(SetMode==true) return ;
    LED_blink_tpye=tp;
    if(LED_blink_tpye!=last_tp)
    {
        out_led_idx_1=0;
        next_ms_1=50;
        show_step_1=0;
    }
    last_tp=LED_blink_tpye;
}

//Task任务
void NotifyTask(void const * argument)
{
	static portTickType xLastWakeTime;
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
#if LEDNotify == 1
    LED_control();
#endif
    vTaskDelayUntil(&xLastWakeTime,portTICK_RATE_MS);
  }
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


