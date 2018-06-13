/*
 * motorConfig.h
 *
 *  Created on: May 28, 2017
 *      Author: baron
 */

#ifndef MOTORCONFIG_H_
#define MOTORCONFIG_H_
#include <stdint.h>
#include "stdbool.h"

typedef struct{
	uint16_t	encodePPR;
	uint16_t	pole;
	uint16_t	PhasePulseMax;	
	uint16_t 	encodeZeroPos;
	uint8_t		isUplseReverse;
	uint16_t    *GetEncoderAddr;
}MotorCfg;


typedef void (*OutPutFun)(uint32_t id,float out,uint16_t encoderPos,uint16_t vectorPos,bool isClosedLoop);
typedef void (*ReleaseSetFun)(uint32_t id);
typedef void (*ReleaseUseFun)(uint32_t id);
typedef bool (*ClaimUseFun)(uint32_t id,uint32_t claimUseTimeout_ms);
typedef bool (*SetMotorConfigFun)(uint32_t svpwmid,uint32_t cfg);

typedef struct{
    OutPutFun               outPut;
    ReleaseSetFun			setUse;
    ReleaseUseFun           releaseUse;
    ClaimUseFun             claimUse;
    SetMotorConfigFun       SetMotorConfig;
}MotorDriver;

#endif /* MOTORCONFIG_H_ */
