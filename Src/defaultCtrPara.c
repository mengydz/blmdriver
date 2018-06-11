#include "defaultCtrPara.h"
#include "motorConfig.h"
#include "motorZeroPosCalibration.h"
#include "motordriver.h"
#include "global.h"

static MotorCfg		motorCfg={
	.encodePPR = EncoderPPR,
	.pole = MotorPole,
	.PhasePulseMax = 2000,//MotorPhaseTimerPeriod,
	.isUplseReverse = MotorRotateReverse_ACB,//MOTOR_Y_ROTATE_REVERSE,
};


void SetMotorDriverCfg(void)
{
	MotorDriver			   *ourDriver = (MotorDriver*)&svpwmDri;
	MotorZeroPosSaveStruct *saveDat = (MotorZeroPosSaveStruct *)GetFlashMapAddr(FlashInterMotorZeroPosAddr);//(MotorZeroPosSaveStruct *)gimbal.caliDataAddr->motorZero;

    motorCfg.encodeZeroPos = saveDat->MotorZero;//2805;

    ourDriver->SetMotorConfig(svpwmID,(uint32_t)&motorCfg);
}

uint32_t GetCaliAddr_MotorZero(void)
{
	return GetFlashMapAddr(FlashInterMotorZeroPosAddr);
}

uint32_t GetCaliAddr_EncodeLine(void)
{
	return GetFlashMapAddr(EncoderLinearCorrectAddr);
}














