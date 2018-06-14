#include "string.h"
#include "stdio.h"
#include "stdbool.h"

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

	float Klsf;//低通滤波器系数

	float Theta_estimate;
}sysEstimateVals;

extern adc_result_type adc_result;
extern sysFbkVals motor_fbk;
extern sysEstimateVals motor_Estimate;

void MotorInit(void);
void adc_zero(void);
void CurrentRunning(uint32_t focId,uint16_t *sample);



