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
#include "protocolType.h"
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
	uint8_t type;		//FrameType_ObserveGroup_Console
	Console str;
	uint8_t checksum;
}__attribute__((packed))FrameTypeGroup_Console;

/*-----------------------------------------------------------------------*/
#define Length_FrameTypeHeartBeat			sizeof(FrameTypeHeartBeat)
#define Length_FrameTypeGroup_Console		sizeof(FrameTypeGroup_Console)

#define LengthOfFrame(protocoltype)			(	protocoltype ==	FrameType_HeartBeat					?	Length_FrameTypeHeartBeat			:\
											(	protocoltype == FrameType_ObserveGroup_Console		?	Length_FrameTypeGroup_Console		:0))

typedef union{
	FrameTypeHeartBeat				heartBeat;
	FrameTypeCmd					cmd;
	FrameTypeGroup_Console			console;
}GBProtocol;
/*-----------------------------------------------------------------------*/
extern t_fifo_buffer 	gbConsoleBuffer;	

extern void systemPrintfInit(void);
extern void gbProtocolHandle(const void * arg);



typedef struct{
	uint16_t heartBeat;
	uint16_t Console;
}TrigFrameType;

typedef union{
	TrigFrameType 	trigS;
	uint16_t		trigA[NumOfFrameType_Send];
}TrigCountUnion;

TrigCountUnion      gbSendTrig;

extern uint8_t InfoOfTask;

void HandleGBCtrCmd(uint8_t cmd);

void gbSendGroupConsole(uint32_t ExterBuffAddr);
#ifdef __cplusplus
}
#endif
#endif /* UPPERPROTOCOL_H_ */
