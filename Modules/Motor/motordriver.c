/*
 * svpwmGenerate.c
 *
 *  Created on: Oct 10, 2016
 *      Author: baron
 */

#include "motordriver.h"
#include "svpwm.h"
#include <stdlib.h>
#include "myMath.h"
#include "string.h"

#define SVMPWMAGIC		(((uint32_t)'S'<<24)|(uint32_t)'p'<<16|(uint32_t)'w'<<8|(uint32_t)'m'<<24)

uint32_t	svpwmID;

static bool SvpwmDriverClaimUse(uint32_t svpwmid,uint32_t claimUseTimeout_ms);
static void SvpwmDriverSetUse(uint32_t svpwmid);
static void SvpwmDriverReleaseUse(uint32_t svpwmid);
static void SvpwmOutSet(uint32_t svpwmid,float	out,uint16_t encoderPos,uint16_t vectorPos,bool isClosedLoop);
static bool SvpwmDriverSetMotorConfig(uint32_t svpwmid,uint32_t cfg);
static void SvpwmGenerate(uint32_t svpwmid,float dt);

const MotorDriver svpwmDri = {
	.outPut = SvpwmOutSet,
	.setUse = SvpwmDriverSetUse,
	.releaseUse = SvpwmDriverReleaseUse,
	.claimUse = SvpwmDriverClaimUse,
	.SetMotorConfig	= SvpwmDriverSetMotorConfig,
};

static bool SvpwmValidate(SvpwmDrive	*svpwmDri)
{
	if(svpwmDri == NULL)
		return false;
	if(svpwmDri->Magic != SVMPWMAGIC)
		return false;
	return true;
}

#if !defined(PIOS_INCLUDE_FREERTOS)

bool RegisterDelayMsFunction(uint32_t svpwmId,DelayMsFun fun)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmId;
	if(!SvpwmValidate(svpwmDrive))
		return false;
	svpwmDrive->delay_ms = fun;
	return true;
}
#endif

bool SvpwmDriverPulseUpdateFunRegister(uint32_t *svpwmid,uint32_t svpwm_tim_id , uint32_t Update)
{

	SvpwmDrive	*svpwmDri = (SvpwmDrive	*)malloc(sizeof(SvpwmDrive));
	DEBUG_Assert(svpwmDri);
	memset(svpwmDri,0,sizeof(SvpwmDrive));
	svpwmDri->cfg = NULL;//
	svpwmDri->svpwm_tim_id = svpwm_tim_id;
	svpwmDri->updataFun = (PulseUpdate)Update;

	svpwmDri->Magic = SVMPWMAGIC;

	svpwmDri->isCurrentLoop = false;

	*svpwmid = (uint32_t)svpwmDri;

	return true;
}

static bool SvpwmDriverSetMotorConfig(uint32_t svpwmid,uint32_t cfg)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	if(!SvpwmValidate(svpwmDrive))
		return false;

	svpwmDrive->cfg = (MotorCfg *)cfg;

	return true;
}

static bool SvpwmDriverClaimUse(uint32_t svpwmid,uint32_t claimUseTimeout_ms)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	if(!SvpwmValidate(svpwmDrive))
		return false;

	return svpwmDrive->svpwmUseBusy;
}

static void SvpwmDriverSetUse(uint32_t svpwmid)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	if(SvpwmValidate(svpwmDrive))
#if defined(PIOS_INCLUDE_FREERTOS)
	    PIOS_Mutex_Unlock(svpwmDrive->svpwmUseMutex);
#else
	svpwmDrive->svpwmUseBusy = true;
#endif
}

static void SvpwmDriverReleaseUse(uint32_t svpwmid)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	if(SvpwmValidate(svpwmDrive))
#if defined(PIOS_INCLUDE_FREERTOS)
	    PIOS_Mutex_Unlock(svpwmDrive->svpwmUseMutex);
#else
	svpwmDrive->svpwmUseBusy = false;
#endif
}

static void SvpwmOutSet(uint32_t svpwmid,float	out,uint16_t encoderPos,uint16_t vectorPos,bool isClosedLoop)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	float dt = 0;
	if(!SvpwmValidate(svpwmDrive))
		return;

	if(svpwmDrive->cfg == NULL)
		return;

	if(isClosedLoop == false)
	{
		svpwmDrive->isCurrentLoop = false;
	}

    svpwmDrive->out = out;
    svpwmDrive->encoderPos = encoderPos;
    svpwmDrive->vectorPos = vectorPos;
    svpwmDrive->isClosedLoop = isClosedLoop;
    SvpwmGenerate(svpwmid,dt);
}

static void SvpwmGenerate(uint32_t svpwmid ,float dt)
{
	SvpwmDrive	*svpwmDrive = (SvpwmDrive *)svpwmid;
	DEBUG_Assert(svpwmDrive);
	uint16_t pulse[MotorPhase_Num];

	if(svpwmDrive->isClosedLoop != false)
	{
		uint16_t encodePPRperPole = svpwmDrive->cfg->encodePPR/svpwmDrive->cfg->pole;

		svpwmDrive->vectorPos = svpwmDrive->cfg->encodePPR + svpwmDrive->encoderPos + svpwmDrive->cfg->encodeZeroPos;
		svpwmDrive->vectorPos =	svpwmDrive->vectorPos%(encodePPRperPole);
		svpwmDrive->vectorPos = (uint64_t)SvpwmDriverRad * svpwmDrive->vectorPos/encodePPRperPole;
		svpwmDrive->vectorPos += SIGN(svpwmDrive->out)*SvpwmDriverRad_Quart;
		svpwmDrive->vectorPos = svpwmDrive->vectorPos&SvpwmDriverRad_mask;
	}else
	{
		svpwmDrive->vectorPos = svpwmDrive->vectorPos&SvpwmDriverRad_mask;
	}
	svpwm(svpwmDrive->vectorPos,pulse,fabs(svpwmDrive->out)*4096,svpwmDrive->cfg->PhasePulseMax);
	svpwmPhaseReverse(svpwmDrive->cfg->isUplseReverse,pulse);
	if(svpwmDrive->updataFun!=NULL)
	{
		svpwmDrive->updataFun(svpwmDrive->svpwm_tim_id,pulse);
	}
}




