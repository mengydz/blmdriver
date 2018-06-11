/*
 * gbProtocol.h
 *
 *  Created on: Nov 18, 2016
 *      Author: Kalman.wang
 */

#ifndef GBPROTOCOL_H_
#define GBPROTOCOL_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "gbProtocolType.h"
#include "fifo_buffer.h"


#define GT_PROTOCOL_HEAD_H		'G'
#define GT_PROTOCOL_HEAD_L		'B'
/*-----------------------------------------------------------------------*/
/*	每个数据/命令帧格式为
 * 		headl:			GT_PROTOCOL_HEAD_L
 * 		headH:			GT_PROTOCOL_HEAD_H
 * 		FrameType:		相应不同包类型填入数据
 *		buff			根据不同包内容有不同实体参数，可以为空
 *		checksum		以上所有byte之和的校验码
 */


typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_HeartBeat
	uint32_t second;
	uint8_t checksum;	
}__attribute__((packed))FrameTypeHeartBeat;
/*-----------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_Cmd
	uint8_t	cmd;
	uint8_t checksum;
}__attribute__((packed))FrameTypeCmd;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_Cmd_Response
	CmdResponse response;
	uint8_t checksum;
}__attribute__((packed))FrameTypeCmdResopense;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint16_t VarIndex;
	union{
		int8_t		i8_Value;
		uint8_t 	u8_Value;
		int16_t 	i16_Value;
		uint16_t 	u16_Value;
		int32_t 	i32_Value;
		uint32_t 	u32_Value;
		float		f_Value;
	}Value;
}__attribute__((packed))SetParaUnion;

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_Set_Para
	SetParaUnion	  setPara;	
	uint8_t checksum;
}__attribute__((packed))FrameTypeSetPara;
/*-----------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_Set_Para_Response
	SetParaResponse setParaResopnse;
	uint8_t checksum;
}__attribute__((packed))FrameTypeSetResonse;
/*-----------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_Save_Para
	uint8_t checksum;
}__attribute__((packed))FrameTypeSavePara;
/*------------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_SyncVar
	uint8_t checksum;
}__attribute__((packed))FrameTypeSyncVar;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;				//FrameType_SyncVar_Rsp_All
	SyncVariableInfo VarInfo;
	uint8_t checksum;
}__attribute__((packed))FrameTypeSyncVariableInfo;
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_ObserveGroup_Sensor
	GroupSensor sensor;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_Sensor;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//类型参见 enum FrameType
	Att att;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_Att;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//类型参见 enum FrameType
	FromCameraData dat;
	uint8_t checksum;
}__attribute__((packed))FrameTypeFromCamera;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//类型参见 enum FrameType
	CaliStatus flag;
	uint8_t checksum;
}__attribute__((packed))FrameTypeCaliStatus;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//类型参见 enum FrameType
	MotorDamper data;
	uint8_t checksum;
}__attribute__((packed))FrameTypeMotorDamp;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	AngleCtrLoop AL;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_AngleCtrLoop;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	GyroCtrLoop GL;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_GyroCtrLoop;
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	SysPara sys;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_SysPara;
/*-----------------------------------------------------------------------*/
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;		//FrameType_ObserveGroup_Console
	Console str;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_Console;
						
/*-----------------------------------------------------------------------*/

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	Reserved reserve;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_Reserved;
/*-----------------------------------------------------------------------*/
											//							uint8_t_ObserveGroup_CurLoop
typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	CurLoop	 curLoop;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_CurLoop;

typedef struct{
	uint8_t headL;
	uint8_t headH;
	uint8_t type;
	ToCameraData	 camdat;
	uint8_t checksum;
}__attribute__((packed))FrameTypeToCamera;

/*-----------------------------------------------------------------------*/
#define Length_FrameTypeHeartBeat			sizeof(FrameTypeHeartBeat)
#define Length_FrameTypeCmd					sizeof(FrameTypeCmd)
#define Length_FrameTypeCmdResopense		sizeof(FrameTypeCmdResopense)
#define Length_FrameTypeSetPara				sizeof(FrameTypeSetPara)
#define Length_FrameTypeSetResonse			sizeof(FrameTypeSetResonse)
#define Length_FrameTypeSavePara			sizeof(FrameTypeSavePara)
#define Length_FrameTypeSyncVar				sizeof(FrameTypeSyncVar)
#define Length_FrameTypeSyncVariableInfo	sizeof(FrameTypeSyncVariableInfo)
#define Length_FrameTypeGroup_Sensor		sizeof(FrameTypeGroup_Sensor)
#define Length_FrameTypeGroup_Att			sizeof(FrameTypeGroup_Att)
#define Length_FrameTypeGroup_AngleCtrLoop	sizeof(FrameTypeGroup_AngleCtrLoop)
#define Length_FrameTypeGroup_GyroCtrLoop	sizeof(FrameTypeGroup_GyroCtrLoop)
#define Length_FrameTypeGroup_SysPara		sizeof(FrameTypeGroup_SysPara)
#define Length_FrameTypeGroup_Console		sizeof(FrameTypeGroup_Console)
#define Length_FrameTypeGroup_Reserved		sizeof(FrameTypeGroup_Reserved)
#define Length_FrameTypeGrout_CurLoop		sizeof(FrameTypeGroup_CurLoop)
#define Length_FrameTypeToCamera			sizeof(FrameTypeToCamera)
#define Length_FrameTypeFromCamera			sizeof(FrameTypeFromCamera)
#define Length_FrameTypeMotorDamp			sizeof(FrameTypeMotorDamp)
#define Length_FrameTypeCaliStatus			sizeof(FrameTypeCaliStatus)

#define LengthOfFrame(protocoltype)			(	protocoltype ==	FrameType_HeartBeat					?	Length_FrameTypeHeartBeat			:\
											(	protocoltype == FrameType_Cmd						?	Length_FrameTypeCmd					:\
											(	protocoltype == FrameType_Cmd_Response				?	Length_FrameTypeCmdResopense		:\
											(	protocoltype == FrameType_Set_Para					?	Length_FrameTypeSetPara				:\
											(	protocoltype == FrameType_Set_Para_Response			?	Length_FrameTypeSetResonse			:\
											(	protocoltype == FrameType_Save_Para					?	Length_FrameTypeSavePara			:\
											(	protocoltype == FrameType_SyncVar					?	Length_FrameTypeSyncVar				:\
											(	protocoltype == FrameType_SyncVar_Rsp_All			?	Length_FrameTypeSyncVariableInfo	:\
											(	protocoltype == FrameType_ObserveGroup_Sensor		?	Length_FrameTypeGroup_Sensor		:\
											(	protocoltype == FrameType_ObserveGroup_Att			?	Length_FrameTypeGroup_Att			:\
											(	protocoltype == FrameType_ObserveGroup_AngleCtrLoop	?	Length_FrameTypeGroup_AngleCtrLoop	:\
											(	protocoltype == FrameType_ObserveGroup_GyroCtrLoop	?	Length_FrameTypeGroup_GyroCtrLoop	:\
											(	protocoltype == FrameType_ObserveGroup_SysPara		?	Length_FrameTypeGroup_SysPara		:\
											(	protocoltype == FrameType_ObserveGroup_Console		?	Length_FrameTypeGroup_Console		:\
											(	protocoltype ==	FrameType_ObserveGroup_CurLoop		?	Length_FrameTypeGrout_CurLoop		:\
											(	protocoltype ==	FrameType_ToCamera					?	Length_FrameTypeToCamera			:\
											(	protocoltype ==	FrameType_FromCamera				?	Length_FrameTypeFromCamera			:\
											(	protocoltype ==	FrameType_MotorDampData				?	Length_FrameTypeMotorDamp			:\
											(	protocoltype == FrameType_ObserveGroup_Reserve		?	Length_FrameTypeGroup_Reserved		: 0)))))))))))))))))))

typedef union{
	FrameTypeHeartBeat				heartBeat;
	FrameTypeCmd					cmd;
	FrameTypeCmdResopense			cmdResponse;
	FrameTypeSetPara				setPara;
	FrameTypeSetResonse				setResponse;
	FrameTypeSavePara				savePara;
	FrameTypeSyncVar				syncVar;
	FrameTypeSyncVariableInfo		varInfo;
	FrameTypeGroup_Sensor			sensor;
	FrameTypeGroup_Att				att;
	FrameTypeGroup_AngleCtrLoop		ACLoop;
	FrameTypeGroup_GyroCtrLoop		GCLoop;
	FrameTypeGroup_SysPara			sysPara;
	FrameTypeGroup_Console			console;
	FrameTypeGroup_CurLoop			CLoop;
	FrameTypeGroup_Reserved			reserved;
	FrameTypeToCamera				tocameradat;
	FrameTypeFromCamera				fromcameradat;
	FrameTypeCaliStatus				calistatus;
	FrameTypeMotorDamp				Mdamper;
}GBProtocol;
/*-----------------------------------------------------------------------*/
extern t_fifo_buffer 	gbConsoleBuffer;	

extern void systemPrintfInit(void);
extern void gbProtocolHandle(const void * arg);



typedef struct{
	uint16_t heartBeat;
	uint16_t Console;
	uint16_t Sensor;
	uint16_t Att;
	uint16_t AngleCtrLoop;
	uint16_t GyroCtrLoop;
	uint16_t SysPara;
	uint16_t CurLoop;
	uint16_t Reserve;
}TrigFrameType;

typedef union{
	TrigFrameType 	trigS;
	uint16_t		trigA[NumOfFrameType_Send];
}TrigCountUnion;

TrigCountUnion      gbSendTrig;

extern uint8_t InfoOfTask;

void HandleGBCtrCmd(uint8_t cmd);

void gbSendGroupSensor(uint32_t ExterBuffAddr);
void gbSendGroupAtt(uint32_t ExterBuffAddr);
void gbSendGroupAngleCtrLoop(uint32_t ExterBuffAddr);
void gbSendGroupGyroCtrLoop(uint32_t ExterBuffAddr);
void gbSendGroupConsole(uint32_t ExterBuffAddr);
void gbProtocolSendDisable(void);
void gbSendMotorDamp(uint8_t axis,int16_t index,int16_t dat);
#ifdef __cplusplus
}
#endif
#endif /* UPPERPROTOCOL_H_ */
