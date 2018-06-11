/*
 * myMath.h
 *
 *  Created on: Oct 28, 2016
 *      Author: baron
 */

#ifndef MYMATH_H_
#define MYMATH_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include <math.h>
#include <stdint.h>
//#include "arm_math.h"
 
#define min(a,b)	(a>b?b:a)

#define max(a,b) (a>b?a:b)

#define SIGN(a)		((a>0)?1:-1)

#define ABS(x)		x<0?(-x):x

#define NELEMENTS(x) (sizeof(x) / sizeof(*(x)))

#define Constrain(x,min,max)	do{					\
								if((x) > (max))		\
									(x) = (max);	\
								if((x) < (min))		\
									(x) = (min);	\
							}while(0)

#define AngleRange180_180(x)	do{					\
									while(x>180)	\
										{x -= 360;}	\
									while(x<-180)	\
										{x += 360;}	\
								}while(0)

#ifndef PI
#define PI		M_PI
#endif

#ifndef EARTH_G
#define EARTH_G 9.80665f
#endif

#define KP_ANGLE2RAD			0.01745329f
#define KP_RAD2ANGLE			57.2957795f


 typedef int32_t		Q30;
 typedef int32_t		Q24;
 typedef int32_t		Q20;
 typedef int32_t		Q16;
 typedef int32_t		Q15;
 typedef int32_t		Q10;
 typedef int32_t		Q5;
 typedef int32_t		Q4;
 typedef int32_t		Q0;

 #define VALUE_Q31			2147483648
 #define VALUE_Q30			1073741824
 #define VALUE_Q24			16777216
 #define VALUE_Q20			1048576
 #define VALUE_Q17			131072
 #define VALUE_Q16			65536
 #define VALUE_Q15			32768
 #define VALUE_Q14			16384
 #define VALUE_Q13			8192
 #define VALUE_Q12			4096
 #define VALUE_Q10			1024
 #define VALUE_Q5			32
 #define VALUE_Q4			16

#define SQRT3_Q15						56756
#define SQRT3_DIV2_Q15					28378
#define INV_SQRT3_Q15 					18918
#define I2INV_SQRT3_Q15 				37836

#define LowPassSimple(in,old,a)		((old) += ((in) - (old))*(a))							//((old) = (old)*(1-(a))+(in)*(a))

#define DEC2BCD(Dec,Bcd,pos)	{uint8_t temp; temp = (uint32_t)(Dec / pow(100,pos))%100;Bcd = ((temp/10)<<4) | ((temp%10) & 0x0F); }

float RCLowPass(float newDat,float oldDat,float f0,float Ts);

void LSM_Plus(double X,double Y,double* SUMX,double* SUMX2,double* SUMY,double* SUMXY,double* SUMY2);

void LSM_Output(double N,double SUMX,double SUMX2,double SUMY,double SUMXY,double SUMY2,float* K,float* B,float* R );

#define DECLARE_SimpleRCLowPassFilter(frequency) 	void SimpleRCLowPassFilter_##frequency##hz(float * in,float *out,float dt)		\
													{																				\
														*out = RCLowPass(*in,*out,frequency,dt);									\
													}

extern unsigned char CalculateCheckSum(unsigned char *p,int len);

extern void SimpleRCLowPassFilter_1hz(float * in,float *out,float dt);
extern void SimpleRCLowPassFilter_2hz(float * in,float *out,float dt);
extern void SimpleRCLowPassFilter_10hz(float * in,float *out,float dt);
extern void SimpleRCLowPassFilter_100hz(float * in,float *out,float dt);
extern void SimpleRCLowPassFilter_200hz(float * in,float *out,float dt);
//uint8_t DectoBCD(int32_t Dec, uint8_t *Bcd, uint8_t length);

#ifdef __cplusplus
 }
#endif
#endif /* MYMATH_H_ */
