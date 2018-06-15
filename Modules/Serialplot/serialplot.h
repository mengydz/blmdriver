/*
 * serialplotif.h
 *
 *  Created on: Sep 30, 2016
 *      Author: baron
 */

#ifndef SERIALPLOTIF_H_
#define SERIALPLOTIF_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "global.h"
#include "pios_com.h"

#define FRAME_LEN		6

typedef  struct {
	uint8_t		frameHeader1;
	uint8_t		frameHeader2;
	int16_t		fdata[2];
	uint8_t 	checksum;
}__attribute__((packed))SerialPlotFrame;

typedef struct{
	SerialPlotFrame frame[64];
	uint8_t 	mask;
	uint32_t 	W;
	uint32_t 	R;
}CurrentLoopPlotBuff;

typedef struct{
	uint8_t		frameHeader1;
	uint8_t		frameHeader2;
	int16_t		fdata[8];
}__attribute__((packed))SerialPlotFrameSingle;

void SerialPlotFrameInput(int16_t fdata[8]);

void SerialPlotFramePlotHalfWord2(int16_t fdata,int16_t fdata2);
extern CurrentLoopPlotBuff clPlotBuff;
extern SerialPlotFrameSingle frame;
extern SerialPlotFrame freamcrc;
//extern volatile  SerialPlotFrame	frame[30];

#ifdef __cplusplus
 }
#endif
#endif /* SERIALPLOTIF_H_ */











