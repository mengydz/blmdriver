#include "current.h"
#include "serialplot.h"
#include <math.h>
#include "timer.h"
#include "motordriver.h"

adc_result_type adc_result;
sysFbkVals motor_fbk;
sysEstimateVals motor_Estimate;
MotorParamVars motor;

void MotorInit(void)
{
	adc_result.haszero = false;
	motor_fbk.I_fbk_factor = 0.000805664f;
	motor_Estimate.Klsf = 0.01f;
	motor_Estimate.kctrl = 0.01f;
	motor.pwm_freq = PWM_FREQUENCE_VAL;
	motor.pwm_Ts = 1.0f/PWM_FREQUENCE_VAL;

	motor.Motor_Rs_pu = MOTOR_RS;// / R_base;
	motor.Motor_Ld_pu = MOTOR_LD;// / L_base;
	motor.Motor_Lq_pu = MOTOR_LQ;// / L_base;
	//Fctrl = 1 - R*Ts/L
	motor_Estimate.Fctrl = (1 - (motor.Motor_Rs_pu * motor.pwm_Ts) / motor.Motor_Ld_pu);
	//Gctrl = Ts/Ls
	motor_Estimate.Gctrl = motor.pwm_Ts / motor.Motor_Ld_pu;
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
	float I_error_abs;
	motor_Estimate.Ialpha_estimate_pu = (motor_Estimate.Gctrl*(motor_Estimate.Ualpha_pll_compens - motor_Estimate.Ealpha_estimate_pu - motor_Estimate.Adjust_alpha_pu) + motor_Estimate.Fctrl * motor_Estimate.Ialpha_estimate_pu);
	motor_Estimate.Ibeta_estimate_pu = (motor_Estimate.Gctrl*(motor_Estimate.Ubeta_pll_compens - motor_Estimate.Ebeta_estimate_pu - motor_Estimate.Adjust_beta_pu) + motor_Estimate.Fctrl * motor_Estimate.Ibeta_estimate_pu);
	//计算电流的误差
	motor_Estimate.Ialpha_pu_err = motor_Estimate.Ialpha_estimate_pu - motor_fbk.Ialpha_fbk_pu;
	motor_Estimate.Ibeta_pu_err = motor_Estimate.Ibeta_estimate_pu - motor_fbk.Ibeta_fbk_pu;

	I_error_abs = motor_Estimate.Ialpha_pu_err;
	if(I_error_abs < 0){
		I_error_abs = -I_error_abs;
	}
	if(motor_Estimate.Ialpha_pu_err > 0.1f )
		motor_Estimate.Ialpha_pu_err = 0.1f;
	else if(motor_Estimate.Ialpha_pu_err < -0.1f)
		motor_Estimate.Ialpha_pu_err = -0.1f;
	motor_Estimate.Adjust_alpha_pu = motor_Estimate.kctrl * motor_Estimate.Ialpha_pu_err;
	I_error_abs = motor_Estimate.Ibeta_pu_err;
	if(I_error_abs < 0){
		I_error_abs = -I_error_abs;
	}
	if(motor_Estimate.Ibeta_pu_err > 0.1f )
		motor_Estimate.Ibeta_pu_err = 0.1f;
	else if(motor_Estimate.Ibeta_pu_err < -0.1f)
		motor_Estimate.Ibeta_pu_err = -0.1f;
	motor_Estimate.Adjust_beta_pu = motor_Estimate.kctrl * motor_Estimate.Ibeta_pu_err;

	motor_Estimate.Ealpha_estimate_pu += motor_Estimate.Klsf * (motor_Estimate.Adjust_alpha_pu - motor_Estimate.Ealpha_estimate_pu);
	motor_Estimate.Ebeta_estimate_pu += motor_Estimate.Klsf * (motor_Estimate.Adjust_beta_pu - motor_Estimate.Ebeta_estimate_pu);

	motor_Estimate.Ealpha_estimate_pu_filt += motor_Estimate.Klsf * (motor_Estimate.Ealpha_estimate_pu - motor_Estimate.Ealpha_estimate_pu_filt);
	motor_Estimate.Ebeta_estimate_pu_filt += motor_Estimate.Klsf * (motor_Estimate.Ebeta_estimate_pu - motor_Estimate.Ebeta_estimate_pu_filt);

	motor_Estimate.Theta_estimate = atan2f(motor_Estimate.Ebeta_estimate_pu_filt,-motor_Estimate.Ealpha_estimate_pu_filt);
}

volatile uint64_t micro_start,micro_diff,micro_now;
uint64_t rotate=0;
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

	motor_estimat_theta();

	micro_now = GetMicro();
	micro_diff = micro_now - micro_start;
	micro_start = micro_now;

	#if 1
	frame.fdata[0] = motor_Estimate.Theta_estimate*57.3f;
	frame.fdata[1] = motor_Estimate.Adjust_alpha_pu*1000;
	frame.fdata[2] = motor_Estimate.Ealpha_estimate_pu*1000;
	frame.fdata[3] = motor_Estimate.Ualpha_pll_compens*100;
	frame.fdata[4] = motor_Estimate.Ialpha_estimate_pu*100;
	frame.fdata[5] = motor_fbk.Ialpha_fbk_pu*100;
	frame.fdata[6] = motor_Estimate.Ialpha_pu_err*100;
	frame.fdata[7] = motor_Estimate.Adjust_alpha_pu*100;
	PIOS_COM_SendBufferNonBlocking(comDebugId, (uint8_t*)&frame, (uint16_t)sizeof(frame));
	#endif
	svpwmDri.outPut(svpwmID,0.18,0,rotate,false);
	rotate+=2;
}




