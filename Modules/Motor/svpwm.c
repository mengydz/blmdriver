/*
 * svpwm.c
 *
 *  Created on: Apr 4, 2017
 *      Author: baron
 */
#include "svpwm.h"
#include "myMath.h"

extern const float 			svpwmarray[2048];
#define VALUE_Q12			4096
#define VALUE_Q11			2048
									//（svpwmarray[] - 2048） 相对中心点的偏离
int16_t svpwmArrayQ12[4096];


void svpwmArrayQ12Init(void)
{
	for(uint16_t i = 0;i<2048;i++)
	{
		svpwmArrayQ12[i] = svpwmarray[i] * VALUE_Q12 - VALUE_Q11;
		svpwmArrayQ12[SvpwmDriverRad - 1 - i] = svpwmArrayQ12[i];
	}
}

/*
 *	uint16_t vector_Q12			0-4096	0-2*pi
 *	uint16_t *pulse				pulse[3]定时器三相比较值
 *	uint16_t abs_out_Q12		0-4096	0-1
 *	uint16_t PeriodMax			定时器最大周期值
 */
void svpwm(uint16_t vector_Q12,uint16_t *pulse,uint16_t abs_out_Q12,uint16_t PeriodMax)
{
#if 0
	/*940ns*/
	uint16_t Index[3];
	
	Constrain(abs_out_Q12,0,VALUE_Q12);
	/*	相位与算法生成一致	,加减左右 相位滞后用减法*/
	Index[0] = vector_Q12 & SvpwmDriverRad_mask;
	Index[1] = (SvpwmDriverRad + Index[0] - SvpwmDriverRad_120 ) & SvpwmDriverRad_mask;
	Index[2] = (SvpwmDriverRad + Index[0] - SvpwmDriverRad_240 ) & SvpwmDriverRad_mask;

	for(uint8_t i = 0 ;i<3 ; i++)
	{
		*pulse++ = (PeriodMax>>1) * (VALUE_Q12 + (((svpwmArrayQ12[Index[i]]-(VALUE_Q12>>1))*abs_out_Q12 )>> 11))>>12;
	}
#else
	/*600ns*/
	uint16_t index[3];
	uint16_t vector;
	
	Constrain(abs_out_Q12,0,VALUE_Q12);
	vector = vector_Q12 & SvpwmDriverRad_mask;

	index[0] = vector;
	pulse[0] = PeriodMax * (VALUE_Q12 + ((svpwmArrayQ12[index[0]]*abs_out_Q12 )>> 11))>>13;

	index[1] = (SvpwmDriverRad + vector - SvpwmDriverRad_120 ) & SvpwmDriverRad_mask;
	pulse[1] = PeriodMax * (VALUE_Q12 + ((svpwmArrayQ12[index[1]]*abs_out_Q12 )>> 11))>>13;

	index[2] = (SvpwmDriverRad + vector - SvpwmDriverRad_240 ) & SvpwmDriverRad_mask;
	pulse[2] = PeriodMax * (VALUE_Q12 + ((svpwmArrayQ12[index[2]]*abs_out_Q12 )>> 11))>>13;
#endif
}



void svpwm2(int32_t v_alpha,int32_t v_beta,uint16_t *pulse,uint16_t periodMax_DIV_SQRT3,uint16_t periodMax_div2)
{
	//SQRT3_Q15
	Q15 va,vb,vc,vmax,vmin,vcom;
	va = v_alpha;
	vb = ((v_beta*SQRT3_DIV2_Q15)>>15) - (v_alpha>>1);
	vc = -((v_beta*SQRT3_DIV2_Q15)>>15) - (v_alpha>>1);
	vmax = 0;
	vmin = 0;
	if (va > vb)
	{
		vmax = va;
		vmin = vb;
	}
	else
	{
		vmax = vb;
		vmin = va;
	}
	if(vc > vmax)
	{
		vmax = vc;
	}else if (vc < vmin)
	{
		vmin = vc;
	}
	vcom = (vmax+vmin)/2;
	pulse[0] = ((vcom - va)*periodMax_DIV_SQRT3>>15) + periodMax_div2;
	pulse[1] = ((vcom - vb)*periodMax_DIV_SQRT3>>15) + periodMax_div2;
	pulse[2] = ((vcom - vc)*periodMax_DIV_SQRT3>>15) + periodMax_div2;
}
