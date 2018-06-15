/*
 * serialplotif.c
 *
 *  Created on: Sep 30, 2016
 *      Author: baron
 */
#include "serialplot.h"
#include "pios_com.h"

//volatile SerialPlotFrame	frame[30] = {
//		[0] = {
//		.frameHeader1 = 0XAA,
//		.frameHeader2 = 0XBB,
//		}
//};
CurrentLoopPlotBuff clPlotBuff = {
		.mask = 63,
		.W = 0,
		.R = 0,
};

SerialPlotFrameSingle frame = {
		.frameHeader1 = 0XAA,
		.frameHeader2 = 0XBB,
};

SerialPlotFrame	freamcrc = {
		.frameHeader1 = 0xAA,
		.frameHeader2 = 0xBB,
};
void SerialPlotFrameInput(int16_t fdata[8])
{

#if 1
	for(uint8_t i = 0;i<8;i++)
	{
		frame.fdata[i] = *(fdata+i);
	}
	PIOS_COM_SendBufferNonBlocking(comDebugId, (uint8_t*)&frame, (uint16_t)sizeof(frame));
#endif

}

SerialPlotFrame frameHalfWord2 = {
        .frameHeader1 = 0XAA,
        .frameHeader2 = 0XBB,
};
void SerialPlotFramePlotHalfWord2(int16_t fdata,int16_t fdata2)
{
    frameHalfWord2.fdata[0] = fdata;
    frameHalfWord2.fdata[1] = fdata2;
    frameHalfWord2.checksum = 0;
    for(uint8_t j=0;j<sizeof(SerialPlotFrame)-1;j++)
    {
        if(j>1)
        {
            frameHalfWord2.checksum += *(((uint8_t *)&frameHalfWord2)+j);
        }
    }
    PIOS_COM_SendBufferNonBlocking(comDebugId,(uint8_t *)&frameHalfWord2,sizeof(SerialPlotFrame));
}















