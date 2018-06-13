/*
 * svpwm.h
 *
 *  Created on: Apr 4, 2017
 *      Author: baron
 */

#ifndef __SVPWM_H_
#define __SVPWM_H_

#include <stdint.h>


#define SvpwmDriverRad			(uint16_t)4096
#define SvpwmDriverRad_half		(SvpwmDriverRad>>1)
#define SvpwmDriverRad_Quart	(SvpwmDriverRad>>2)
#define SvpwmDriverRad_mask		(SvpwmDriverRad-1)
#define SvpwmDriverRad_max		(SvpwmDriverRad-1)
#define SvpwmDriverRad_120		(SvpwmDriverRad_max/3)
#define SvpwmDriverRad_240		((SvpwmDriverRad_max<<1)/3)

#define svpwmPhaseReverse(reverse,pulse)		\
do{												\
	uint16_t tempPulse[3];						\
	tempPulse[0] = pulse[0];					\
	tempPulse[1] = pulse[1];					\
	tempPulse[2] = pulse[2];					\
	switch(reverse)								\
	{											\
		case MotorRotateReverse_ACB:			\
			pulse[0] = tempPulse[0];			\
			pulse[1] = tempPulse[2];			\
			pulse[2] = tempPulse[1];			\
			break;								\
		case MotorRotateReverse_BAC:			\
			pulse[0] = tempPulse[1];			\
			pulse[1] = tempPulse[0];			\
			pulse[2] = tempPulse[2];			\
			break;								\
		case MotorRotateReverse_BCA:			\
			pulse[0] = tempPulse[1];			\
			pulse[1] = tempPulse[2];			\
			pulse[2] = tempPulse[0];			\
			break;								\
		case MotorRotateReverse_CAB:			\
			pulse[0] = tempPulse[2];			\
			pulse[1] = tempPulse[0];			\
			pulse[2] = tempPulse[1];			\
			break;								\
		case MotorRotateReverse_CBA:			\
			pulse[0] = tempPulse[2];			\
			pulse[1] = tempPulse[1];			\
			pulse[2] = tempPulse[0];			\
			break;								\
		default:								\
			break;								\
	}											\
}while(0)										

void svpwmArrayQ12Init(void);
void svpwm(uint16_t vector_Q12,uint16_t *pulse,uint16_t abs_out_Q12,uint16_t PeriodMax);
void svpwm2(int32_t v_alpha,int32_t v_beta,uint16_t *pulse,uint16_t periodMax_DIV_SQRT3,uint16_t periodMax_div2);
#endif /* SVPWM_H_ */
