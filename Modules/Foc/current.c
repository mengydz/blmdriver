#include "current.h"
#include "serialplot.h"
#include <math.h>
#include "timer.h"

adc_result_type adc_result;
sysFbkVals motor_fbk;
sysEstimateVals motor_Estimate;

void MotorInit(void)
{
	adc_result.haszero = false;
	motor_fbk.I_fbk_factor = 0.0805664f;
	motor_Estimate.Klsf = 0.5f;
}

void adc_zero(void)
{
	static uint32_t mot_a_sum=0,mot_b_sum=0,cnt=0;
	if(cnt >= 100)//wait for system health
	{
		mot_a_sum += adc_result.adc_currnt_a;
		mot_b_sum += adc_result.adc_current_b;
	}
	cnt++;
	if(cnt >= 356)
	{
		adc_result.motor_a_zero = mot_a_sum/256;
		adc_result.motor_b_zero = mot_b_sum/256;
		adc_result.haszero = true;
	}
}

void motor_estimat_theta(void)
{
	motor_Estimate.Ialpha_estimate_pu = (motor_Estimate.Gctrl*(motor_Estimate.Ualpha_pll_compens - motor_Estimate.Ealpha_estimate_pu - motor_Estimate.Adjust_alpha_pu) + motor_Estimate.Fctrl * motor_Estimate.Ialpha_estimate_pu);
	motor_Estimate.Ibeta_estimate_pu = (motor_Estimate.Gctrl*(motor_Estimate.Ubeta_pll_compens - motor_Estimate.Ebeta_estimate_pu - motor_Estimate.Adjust_beta_pu) + motor_Estimate.Fctrl * motor_Estimate.Ibeta_estimate_pu);
	//计算电流的误差
	motor_Estimate.Ialpha_pu_err = motor_Estimate.Ialpha_estimate_pu - motor_fbk.Ialpha_fbk_pu;
	motor_Estimate.Ibeta_pu_err = motor_Estimate.Ibeta_estimate_pu - motor_fbk.Ibeta_fbk_pu;

	motor_Estimate.Ealpha_estimate_pu += motor_Estimate.Klsf * (motor_Estimate.Adjust_alpha_pu - motor_Estimate.Ealpha_estimate_pu);
	motor_Estimate.Ebeta_estimate_pu += motor_Estimate.Klsf * (motor_Estimate.Adjust_beta_pu - motor_Estimate.Ebeta_estimate_pu);

	motor_Estimate.Ealpha_estimate_pu_filt += motor_Estimate.Klsf * (motor_Estimate.Ealpha_estimate_pu - motor_Estimate.Ealpha_estimate_pu_filt);
	motor_Estimate.Ebeta_estimate_pu_filt += motor_Estimate.Klsf * (motor_Estimate.Ebeta_estimate_pu - motor_Estimate.Ebeta_estimate_pu_filt);

	motor_Estimate.Theta_estimate = atan2f(motor_Estimate.Ebeta_estimate_pu_filt,-motor_Estimate.Ealpha_estimate_pu_filt);
}

volatile uint64_t micro_start,micro_diff,micro_now;
void CurrentRunning(uint32_t focId,uint16_t *sample)
{
	static uint8_t state;
#if 0
	SerialPlotFrameInput(sample);
#endif
	adc_result.adc_currnt_a = sample[0];
	adc_result.adc_current_b = sample[1];
	if(!adc_result.haszero)
	{
		adc_zero();
		return;
	}

	motor_fbk.Ia_fbk_ad = adc_result.adc_currnt_a - adc_result.motor_a_zero;
	motor_fbk.Ib_fbk_ad = adc_result.adc_current_b - adc_result.motor_b_zero;

	motor_fbk.Ia_fbk_real = motor_fbk.Ia_fbk_ad * motor_fbk.I_fbk_factor;
	motor_fbk.Ib_fbk_real = motor_fbk.Ib_fbk_ad * motor_fbk.I_fbk_factor;

	motor_fbk.Ialpha_fbk_pu = motor_fbk.Ia_fbk_real;
	motor_fbk.Ibeta_fbk_pu = (2*motor_fbk.Ib_fbk_real + motor_fbk.Ia_fbk_real) / 1.7321f;
	motor_Estimate.Ualpha_pll_compens = motor_Estimate.Uan_pu;
	motor_Estimate.Ubeta_pll_compens = (2*motor_Estimate.Uan_pu + motor_Estimate.Ubn_pu) / 1.7321f;
	micro_now = GetMicro();
	micro_diff = micro_now - micro_start;
	micro_start = micro_now;

	#if 1
	frame.fdata[0] = micro_diff;
	frame.fdata[1] = 60;
	frame.fdata[2] = 0;
	frame.fdata[3] = 0;
	PIOS_COM_SendBufferNonBlocking(comDebugId, (uint8_t*)&frame, (uint16_t)sizeof(frame));
	#endif

}




