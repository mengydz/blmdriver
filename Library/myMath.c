/*
 * myMath.c
 *
 *  Created on: Oct 28, 2016
 *      Author: baron
 */
#include "myMath.h"

/////////////////////////////////////////////////////////    
//功能：十进制转BCD码  //  
//输入：int Dec                      待转换的十进制数据  
//      int length                   BCD码数据长度    
//输出：unsigned char *Bcd           转换后的BCD码    
//返回：0  success   
//思路：原理同BCD码转十进制   
//////////////////////////////////////////////////////////  
//uint8_t DectoBCD(int32_t Dec, uint8_t *Bcd, uint8_t length)
//{
//	int8_t temp;
//	uint8_t symbol = 0;
//	if(Dec < 0) {symbol = 1;Dec = -Dec;}
//	for(int8_t i=0; i<length; i++)
//	{
//		temp = Dec%100;
//		if(symbol && i == (length-1))
//			Bcd[length -(1 + i)] = (((temp/10)<<4)|0x80) + ((temp%10) & 0x0F);
//		else
//			Bcd[length -(1 + i)] = ((temp/10)<<4) + ((temp%10) & 0x0F);
//		Dec /= 100;
//	}
//	return 0;
//}

void LSM_Plus(double X,double Y,double* SUMX,double* SUMX2,double* SUMY,double* SUMXY,double* SUMY2)
{
	*SUMX  += X;
	*SUMX2 += (X*X);
	*SUMY  += Y;
	*SUMXY += (X*Y);
	*SUMY2 += (Y*Y);
}

void LSM_Output(double N,double SUMX,double SUMX2,double SUMY,double SUMXY,double SUMY2,float* K,float* B,float* R )
{

	*K =(N*SUMXY-SUMX*SUMY)/(N*SUMX2-SUMX*SUMX);
	*B =(SUMX2*SUMY-SUMX*SUMXY)/(N*SUMX2-SUMX*SUMX);
	*R =(SUMXY/N-SUMY/N*SUMX/N)/sqrt((SUMX2/N-SUMX/N*SUMX/N)*(SUMY2/N-SUMY/N*SUMY/N));
}

float RCLowPass(float newDat,float oldDat,float f0,float Ts)
{
	float a;
	if(Ts==0)
	{
		return oldDat;
	}
	a = f0/(1/Ts+f0);
	LowPassSimple(newDat,oldDat,a);
	return oldDat;
}

unsigned char CalculateCheckSum(unsigned char *p,int len)
{
    unsigned char checksum = 0;
    for(unsigned char i = 0;i<len;i++)
    {
        checksum+=p[i];
    }
    return checksum;
}

DECLARE_SimpleRCLowPassFilter(1)
DECLARE_SimpleRCLowPassFilter(2)
DECLARE_SimpleRCLowPassFilter(10)
DECLARE_SimpleRCLowPassFilter(100)
DECLARE_SimpleRCLowPassFilter(200)

