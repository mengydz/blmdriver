#include "string.h"
#include "stdio.h"
#include "stdbool.h"

#define PI	3.1415926f

#define MOTOR_RS			4.2f
#define MOTOR_LD			0.0025f
#define MOTOR_LQ			0.0025f
#define MOTOR_POLES_VAL		11
#define BASE_RPM_CNTRL_VAL	9500
#define VDC_BUS 			12.0f						//12V的直流电压
#define BASE_I_VALUE		100.0f


#define MOTOR_BASE_OMEGA	PI*2*BASE_RPM_CNTRL_VAL/60*MOTOR_POLES_VAL//电机的最大角速度，包括弱磁阶段
#define V_base			(float)(VDC_BUS*0.57735f)   			//电压基值
#define I_base			(float)(BASE_I_VALUE)   	    		//电流基值
#define T_base			(float)(1.0f/MOTOR_BASE_OMEGA)    		//时间基值
#define R_base			(float)(V_base/BASE_I_VALUE)        	//阻抗基值
#define L_base			(float)(R_base*T_base)	        		//电感基值

#define PWM_FREQUENCE_VAL	20000

typedef struct{
	uint16_t adc_currnt_a;
	uint16_t adc_current_b;

	uint16_t motor_a_zero;
	uint16_t motor_b_zero;

	bool	haszero;

}adc_result_type;

typedef struct{
	float Ia_fbk_ad;
	float Ia_fbk_real;
	float Ib_fbk_ad;
	float Ib_fbk_real;
	float I_fbk_factor;

	float Ialpha_fbk_pu;
	float Ibeta_fbk_pu;
}sysFbkVals;

typedef struct{
	float Ialpha_estimate_pu;
	float Ibeta_estimate_pu;

	float Ialpha_pu_err;
	float Ibeta_pu_err;

	float MaxErr;

	//估算的反电动势
	float Ealpha_estimate_pu;
	float Ebeta_estimate_pu;
	float Ealpha_estimate_pu_filt;
	float Ebeta_estimate_pu_filt;
	float Adjust_alpha_pu;
	float Adjust_beta_pu;
	//SMO控制器系数
	float Gctrl;
	float Fctrl;
	float kctrl;
	//电压输出
	float Ualpha_pu;
	float Ubeta_pu;
	float Ualpha_pll_compens;
	float Ubeta_pll_compens;

	float Klsf;//低通滤波器系数

	float Theta_estimate;

	float Uan_pu;
	float Ubn_pu;

	float Ua_pu;
	float Ub_pu;
	float Uc_pu;
}sysEstimateVals;

typedef struct{
	float Motor_Rs_pu;
	float Motor_Ld_pu;
	float Motor_Lq_pu;

	float pwm_freq;
	float pwm_Ts;
}MotorParamVars;

extern adc_result_type adc_result;
extern sysFbkVals motor_fbk;
extern sysEstimateVals motor_Estimate;
extern MotorParamVars motor;

void MotorInit(void);
void adc_zero(void);
void CurrentRunning(uint32_t focId,uint16_t *sample);



