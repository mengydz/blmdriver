/*
 * gbProtocol.c
 *
 *  Created on: Apr 21, 2017
 *      Author: baron
 */
#include "gbProtocol.h"
#include "global.h"
#include "pios_com.h"
#include "defaultCtrPara.h"
#include <string.h>
#include "myMath.h"
#include "calibrate.h"
#include "timer.h"

static GBProtocol	gbSend,gbRecv;
static uint16_t 	gbSendDelay = 2000;
static uint32_t 	tick = 0;
static bool			response;
bool    isClintConnect = false;
uint32_t ComSendCaliStatus=0;
bool transparent_enable = false;

TrigCountUnion      gbSendTrig = {
    .trigS = {
        .heartBeat      = 1000, 
        .Sensor         = 30,
        .Att            = 30,
        .AngleCtrLoop   = 30,
        .GyroCtrLoop    = 10,
        .SysPara        = 20,
        .Console        = 10,
        .CurLoop        = 1000,
        .Reserve        = 1000,
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

void SystemReboot(bool hold_in_bootloader);
void HandleGBCtrCmd(uint8_t cmd)
{
    CalibrateInfo   *CaliInfo = GetCaliInfo();
//    ModuleTest  *ModuleTestFlag = GetModuleTestFlag();
    switch(cmd)
    {

        case CmdType_Cali_TC:
        {
//            CaliInfo->IMUTempCaliNow = true;
//        	CaliInfo->MotorDampDetecctNow = true;
        }break;
        case CmdType_Cali_TC_Clear:
        {
//            ClearCaliDataTemp();
//        	ClearCaliDataMotorDamp();
        }break;
        case CmdType_Cali_AE:
        {
//            CaliInfo->IMUAcceEllipseCaliNow = true;
        }break;
        case CmdType_Cali_AE_Clear:
        {
//            ClearCaliDataAcce();
        }break;
        case CmdType_Cali_MZ:
        {
            CaliInfo->MotorZeroPhaseCaliNow = true;
        }break;
        case CmdType_Cali_MZ_Clear:
        {
            ClearCaliDataMotorZero();
        }break;
        case CmdType_Cali_Fr:
        {
        	
        }break;
        case CmdType_Cali_Fr_Clear:
        {
        }break;
        case CmdType_Cali_EL:
        {
            CaliInfo->EncoderNoneLinearSmpNow = true;
        }break;
        case CmdType_Cali_EL_Clear:
        {
            ClearCaliDataEncoder();
        }break;
        case CmdType_Test_MR:
        {
//            ModuleTestFlag->MotorRotateDirect = true;
        }break;
        case CmdType_Test_MC:
        {

        }break;
        case CmdType_Test_Vib:
        {

        }break;
        
        case CmdType_OpenPower:
        {
//            MotorSwitchHandle   *motorSwitch = GetMotorSwitch();
            /*  Power On without any judeg  */
//            motorSwitch->SwitchON();
        }break;
        case CmdType_ClosePower:
        {
//            MotorSwitchHandle   *motorSwitch = GetMotorSwitch();
            /*  Power Off without any judeg */
//            motorSwitch->SwitchOff();
        }break;
       
        case CmdType_EraseStaticHis:
        {
//            ClearCaliDataStaticHis();
        }break;

        case CmdType_EraseCtrPara:
        {
//            ClearControlPara();
//            CtrParaInitDefault();
//            DefaultControlParaMap2Ram();
        }break;
        case CmdType_PrintVersion:
        {
            printf("--------------------\n");
            printf("Dat: %2d.%2d.%2d\n",COMPILE_YEAR,COMPILE_MONTH,COMPILE_DAY);
            printf("SW : %4.2f\n",0.01f*GIMBAL_INFO_SW_VER);
//            printf("Git: 0x%X\nMaster\n",(int)GitVersion);  
            printf("--------------------\n");
        }break;
        case CmdType_SystemReset_Hold_IN_Bootloader:
        {
//            MotorSwitchHandle   *motorSwitch = GetMotorSwitch();
            /*  Power Off without any judeg */
//            motorSwitch->SwitchOff();
        	SystemReboot(true);
        }break;
        case CmdType_SystemReset:
        {
//            MotorSwitchHandle   *motorSwitch = GetMotorSwitch();
//            /*  Power Off without any judeg */
//            motorSwitch->SwitchOff();
            SystemReboot(true);
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

//static void gbSendVarInfo(void)
//{
//	for(uint8_t i = 0;i<ControlParaIndex_SaveMax;i++)
//	{
//		gbSend.varInfo.headH = GT_PROTOCOL_HEAD_H;
//		gbSend.varInfo.headL = GT_PROTOCOL_HEAD_L;
//		gbSend.varInfo.type = FrameType_SyncVar_Rsp_All;
//		gbSend.varInfo.checksum = 0;
//		gbSend.varInfo.VarInfo.VarIndex = i;
//		gbSend.varInfo.VarInfo.type = controlPara[i].type;
//		memcpy(gbSend.varInfo.VarInfo.Name,controlPara[i].name,16);
//		if(controlPara[i].valueAddr != 0)
//		{
//		    gbSend.varInfo.VarInfo.Value = *(uint32_t *)controlPara[i].valueAddr;
//		    gbSend.varInfo.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_SyncVar_Rsp_All)-1);
//            SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_SyncVar_Rsp_All));
//            osDelay(10);
//		}
//	}
//}

//static void gbSendSetParaResponse(void)
//{
//	gbSend.setResponse.headH = GT_PROTOCOL_HEAD_H;
//	gbSend.setResponse.headL = GT_PROTOCOL_HEAD_L;
//	gbSend.setResponse.type = FrameType_Set_Para_Response;
//	gbSend.setResponse.checksum = 0;
//	gbSend.setResponse.setParaResopnse.flag = (uint8_t)response;
//	gbSend.setResponse.setParaResopnse.VarIndex = lastSetParaIndex;
//	gbSend.setResponse.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_Set_Para_Response)-1);
//	SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_Set_Para_Response));
//	response = false;
//}

//static void gbSendCmdResponse(void)
//{
//	gbSend.cmdResponse.headH = GT_PROTOCOL_HEAD_H;
//	gbSend.cmdResponse.headL = GT_PROTOCOL_HEAD_L;
//	gbSend.cmdResponse.type = FrameType_Cmd_Response;
//	gbSend.cmdResponse.checksum = 0;
//	gbSend.cmdResponse.response.flag= (uint8_t)response;
//	gbSend.cmdResponse.response.cmd = lastCmd;
//	gbSend.cmdResponse.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_Cmd_Response)-1);
//	SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_Cmd_Response));
//	response = false;
//}

static void gbSendHeartBeat(void)
{
	gbSend.heartBeat.headH = GT_PROTOCOL_HEAD_H;
	gbSend.heartBeat.headL = GT_PROTOCOL_HEAD_L;
	gbSend.heartBeat.type = FrameType_HeartBeat;
	gbSend.heartBeat.second = GetMillis()/1000;
	gbSend.heartBeat.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_HeartBeat)-1);
	SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_HeartBeat));
}

void gbSendCaliStatus(uint8_t* dat)
{
	gbSend.calistatus.headH = GT_PROTOCOL_HEAD_H;
	gbSend.calistatus.headL = GT_PROTOCOL_HEAD_L;
	gbSend.calistatus.type  = FrameType_CaliStatus;
	if(dat!=0)
    {
        memcpy((void *)&gbSend.calistatus.flag.dat,(void *)dat,sizeof(gbSend.calistatus.flag));
    }else
    {
        memcpy((void *)&gbSend.calistatus.flag.dat,(void *)0,sizeof(gbSend.calistatus.flag));
    }

    gbSend.calistatus.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_CaliStatus)-1);
    SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_CaliStatus));
}

void gbSendMotorDamp(uint8_t axis,int16_t index,int16_t dat)
{
	gbSend.Mdamper.headH = GT_PROTOCOL_HEAD_H;
	gbSend.Mdamper.headL = GT_PROTOCOL_HEAD_L;
	gbSend.Mdamper.type = FrameType_MotorDampData;

	gbSend.Mdamper.data.axis = axis;
    gbSend.Mdamper.data.index = index;
    gbSend.Mdamper.data.dat = dat;

    gbSend.Mdamper.checksum = CalculateCheckSum((uint8_t *)&gbSend,LengthOfFrame(FrameType_MotorDampData)-1);
    SendingBuffer((uint8_t *)&gbSend,LengthOfFrame(FrameType_MotorDampData));
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
		case FrameType_Cmd_Response:
		{
//			gbSendCmdResponse();
		}break;
		case FrameType_Set_Para_Response:
		{
//			gbSendSetParaResponse();
		}break;
		case FrameType_SyncVar_Rsp_All:
		{
//			gbSendVarInfo();
		}break;
		
		case FrameType_ObserveGroup_Sensor:
		{
//			gbSendGroupSensor(0);
		}break;
		case FrameType_ObserveGroup_Att:
		{
//			gbSendGroupAtt(0);
		}break;
		case FrameType_ObserveGroup_AngleCtrLoop:
		{
//			gbSendGroupAngleCtrLoop(0);
		}break;
		case FrameType_ObserveGroup_GyroCtrLoop:
		{
//			gbSendGroupGyroCtrLoop(0);
		}break;
		case FrameType_ObserveGroup_SysPara:
		{
//			gbSendGroupSysPara();
		}break;
		case FrameType_ObserveGroup_Console:
		{
			gbSendGroupConsole(0);
		}break;
		case FrameType_ObserveGroup_Reserve:
		{
//			gbSendGroupReserved();
		}break;
		case FrameType_ObserveGroup_CurLoop:
		{
//			gbSendGroupCurLoop();
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
//			if(!externCtrInstrcution_PC_isInit)
//            {
//                externCtrRegister(externCtrInstrcution_PC);
//                externCtrInstrcution_PC_isInit = true;
                isClintConnect = true;
//            }
//			GetGimbalCalResponseValue(&ComSendCaliStatus);
//			gbSendCaliStatus((uint8_t*)&ComSendCaliStatus);
		}break;
		case FrameType_Set_Para:
		{
			response = true;
//			ParaSet(gbRecv.setPara.setPara.VarIndex,(void *)&gbRecv.setPara.setPara.Value);
//			gbTxType(FrameType_Set_Para_Response,1);
		}break;
		case FrameType_Cmd:
		{
		    HandleGBCtrCmd(gbRecv.cmd.cmd);
//			gbTxType(FrameType_Cmd_Response,1);
		}break;
		case FrameType_SyncVar:
		{
			//controlParaCCMRAMMapInit();
//			gbTxType(FrameType_SyncVar_Rsp_All,1000);
		}break;
		case FrameType_Save_Para:
		{
//			ControlParaSaveData();
		}break;
		case FrameType_ToCamera:
		{
//			while(!SendInnerFrame(GimbalProType_To_Camera,(uint32_t)&gbRecv.tocameradat.camdat.data,1));
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

void gbProtocolHandle(const void * arg)
{
	if(!transparent_enable)
	{
		if(gbReceiveFrame())
		{
			gbRxHandle();
		}
	}else
	{

	}
	tick++;
	gbTxTrig(tick);
	if(gbSendDelay > 0)
		gbSendDelay--;
}

void gbProtocolSendDisable(void)
{
    memset(&gbSendTrig,0,sizeof(gbSendTrig));
}
