/*
 * gbProtocol.c
 *
 *  Created on: Apr 21, 2017
 *      Author: baron
 */
#include "protocol.h"
#include "global.h"
#include "pios_com.h"
//#include "defaultCtrPara.h"
#include <string.h>
#include "myMath.h"
#include "timer.h"
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

static GBProtocol	gbSend,gbRecv;
static uint16_t 	gbSendDelay = 2000;
bool    isClintConnect = false;
uint32_t ComSendCaliStatus=0;
bool transparent_enable = false;

TrigCountUnion      gbSendTrig = {
    .trigS = {
        .heartBeat      = 1000, 
        .Console        = 10,
    },
};

uint8_t InfoOfTask      = 0;

t_fifo_buffer 	gbConsoleBuffer;
static uint8_t 	SendFrameQueueConsoleBuff[256];


static bool gbReceiveFrame(void)
{
	static uint8_t syncStep = 0;
	static uint8_t frameReceivedLen = 0;
	while(1)
	{
		uint16_t byteNum = PIOS_COM_ReceiveByteLen(comDebugId);
		if(byteNum == 0)
			return false;
		switch(syncStep)
		{
			case 0:
			{
				PIOS_COM_ReceiveBuffer(comDebugId,(uint8_t *)&gbRecv.heartBeat.headL,1,0);
				if(gbRecv.heartBeat.headL == GT_PROTOCOL_HEAD_L)
				{
					frameReceivedLen++;
					syncStep++;
				}
			}
			break;
			
			case 1:
			{
				if(PIOS_COM_ReceiveBytePeek(comDebugId,(uint8_t *)&gbRecv.heartBeat.headH) == true)
				{
					if(gbRecv.heartBeat.headH == GT_PROTOCOL_HEAD_H)
					{
						PIOS_COM_ReceiveBuffer(comDebugId,(uint8_t *)&gbRecv.heartBeat.headH,1,0);
						frameReceivedLen = 0;
						syncStep++;
					}else
					{
						syncStep = 0;
						frameReceivedLen = 0;
						continue;
					}
				}
			}
			break;
			
			case 2:
			{
				PIOS_COM_ReceiveBuffer(comDebugId,(uint8_t *)&gbRecv.heartBeat.type,1,0);
			
				if(LengthOfFrame(gbRecv.heartBeat.type) == 0)
				{
					syncStep = 0;
					frameReceivedLen = 0;
					continue;
				}else
				{
					frameReceivedLen = 0;
					syncStep++;
				}
			}break;
			
			case 3:
			{
				uint8_t lastbytes = (LengthOfFrame(gbRecv.heartBeat.type) - 3) - frameReceivedLen;
				if(byteNum >= lastbytes)
				{
					PIOS_COM_ReceiveBuffer(comDebugId,(uint8_t *)&gbRecv.heartBeat.second,lastbytes,0);
					frameReceivedLen += lastbytes;
					syncStep++;
				}else
				{
					PIOS_COM_ReceiveBuffer(comDebugId,(uint8_t *)&gbRecv.heartBeat.second,byteNum,0);
					frameReceivedLen += byteNum;
				}
			}
			break;	
			
			case 4:
			{
				uint16_t len = LengthOfFrame(gbRecv.heartBeat.type);
				if(CalculateCheckSum((uint8_t *)&gbRecv,len-1) == ((uint8_t *)&gbRecv)[len-1])
				{
					syncStep = 0;
					frameReceivedLen = 0;
					return true;
				}
				else
				{
					syncStep = 0;
					frameReceivedLen = 0;
					continue;
				}
			}
			break;	
			
			default:
			{
				syncStep = 0;
				frameReceivedLen = 0;
			}
			break;
		}
	}
	return false;
}

void HandleGBCtrCmd(uint8_t cmd)
{
    switch(cmd)
    {
        case CmdType_PrintVersion:
        {
//            printf("--------------------\n");
//            printf("Dat: %2d.%2d.%2d\n",COMPILE_YEAR,COMPILE_MONTH,COMPILE_DAY);
//            printf("SW : %4.2f\n",0.01f*GIMBAL_INFO_SW_VER);
//            printf("Git: 0x%X\nMaster\n",(int)GitVersion);  
//            printf("--------------------\n");
        }break;
        case CmdType_SystemReset:
        {

        }break;
        default:break;
    }
}

static void SendingBuffer(uint8_t * str,uint16_t len)
{
    if(gbSendDelay)
    {
        PIOS_COM_SendBufferNonBlocking(comDebugId,str,len);
    }
}

static void gbSendHeartBeat(void)
{
	gbSend.heartBeat.headH = GT_PROTOCOL_HEAD_H;
	gbSend.heartBeat.headL = GT_PROTOCOL_HEAD_L;
	gbSend.heartBeat.type = FrameType_HeartBeat;
	gbSend.heartBeat.second = GetMillis()/1000;
	gbSend.heartBeat.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_HeartBeat)-1);
	SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_HeartBeat));
}

void gbSendGroupConsole(uint32_t ExterBuffAddr)
{
	uint16_t len = fifoBuf_getUsed(&gbConsoleBuffer);
	char *string = &gbSend.console.str.text1[0];
	gbSend.console.headH = GT_PROTOCOL_HEAD_H;
	gbSend.console.headL = GT_PROTOCOL_HEAD_L;
	gbSend.console.type = FrameType_ObserveGroup_Console;
	memset((char *)&gbSend.console.str,0,sizeof(gbSend.console.str));
	if(ExterBuffAddr!=0)
    {
        memcpy((void *)string,(void *)ExterBuffAddr,sizeof(gbSend.console.str));
    }else
    {
        if(len==0)
            return;

        if(len > sizeof(gbSend.console.str))
        {
            for(uint8_t i = 0 ;i < sizeof(gbSend.console.str) ;i++)
            {
                string[i] = fifoBuf_getByte(&gbConsoleBuffer);
                if(string[i] == 0 || string[i] == '\n' || string[i] == '\r')
                {
                    break;
                }
            }
        }else
        {
            for(uint8_t i = 0;i<len;i++)
            {
                string[i] = fifoBuf_getByte(&gbConsoleBuffer);
                if(string[i] == 0 || string[i] == '\n'|| string[i] == '\r')
                {
                    break;
                }
            }
        }
    }

	gbSend.console.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_ObserveGroup_Console)-1);
	SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_ObserveGroup_Console));

}

static void gbTxType(uint8_t type,uint16_t timeout)
{
	switch(type)
	{
		case FrameType_HeartBeat:
		{
			gbSendHeartBeat();
		}break;
		case FrameType_ObserveGroup_Console:
		{
			gbSendGroupConsole(0);
		}break;
		default :break;
	}
}

static void gbRxHandle(void)
{
    static bool externCtrInstrcution_PC_isInit = false;
	switch(gbRecv.heartBeat.type)
	{
		case FrameType_HeartBeat:
		{
			gbSendDelay = 2000;
            isClintConnect = true;
		}break;
		case FrameType_Cmd:
		{
		    HandleGBCtrCmd(gbRecv.cmd.cmd);
		}break;
		default:break;
	}
}

static void gbTxTrig(uint32_t tick)
{
	for(uint8_t i = 0;i<NumOfFrameType_Send;i++)	//NumOfFrameType_Send
	{
		if(gbSendTrig.trigA[i] == 0)
		{
			continue;
		}else{
			if((tick+i)%gbSendTrig.trigA[i] == 0)
			{
				gbTxType(i,0);
			}
		}
	}
}

void systemPrintfInit(void)
{
	fifoBuf_init(&gbConsoleBuffer,(const void *)&SendFrameQueueConsoleBuff[0],sizeof(SendFrameQueueConsoleBuff));
}

//Task任务
void ProtocolTask(void const * argument)
{
  portTickType xLastWakeTime;
  uint32_t tick = 0;
  xLastWakeTime = xTaskGetTickCount();
  gbSendDelay = 2000;
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime,(portTICK_RATE_MS));
	tick++;
	if(!transparent_enable)
	{
        if(gbReceiveFrame())
        {
            gbRxHandle();
        }
	}else
	{

	}

	gbTxTrig(tick);
	if(gbSendDelay > 0)
	    gbSendDelay--;
  }
}

void gbProtocolSendDisable(void)
{
    memset(&gbSendTrig,0,sizeof(gbSendTrig));
}
