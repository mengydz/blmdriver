/*
 * gimbal_global_data.h
 *
 *  Created on: Oct 10, 2016
 *      Author: baron
 */

#ifndef GIMBAL_GLOBAL_DATA_H_
#define GIMBAL_GLOBAL_DATA_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "vectors.h"
#include "global.h"

//定义数据类型转换
typedef union 
{
	float fdata;
	unsigned char bytes[4];
	unsigned int idata[2];
}DataConvert;

typedef struct{
	bool	Upload_Flag_ZeroInfo;
	bool	Upload_Flag_FrontInfo;
	bool	Upload_Flag_EncodInfo;
	bool	Upload_Flag_EncodLiner;
	bool	Upload_version;
}CanReportMessage;

typedef union {
	struct {
	uint8_t Flag_ZeroInfo:1,
			Flag_FrontInfo:1,
			Flag_EncodInfo:1,
			Flag_EncodLiner:1,
			Flag_PowerInfo:1,
			spare:3;
		};
	uint8_t cbytes;
}ModulesStatusInfo;

enum{
	CAN_IDLE = 0,
	CAN_SENDING = 1,
};

#ifdef __cplusplus
 }
#endif
#endif /* GIMBAL_GLOBAL_DATA_H_ */
