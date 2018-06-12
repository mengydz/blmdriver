#ifndef GBPROTOCOLTYPE_H
#define GBPROTOCOLTYPE_H

typedef enum{
    FrameType_HeartBeat = 0,
    FrameType_ObserveGroup_Console,
    NumOfFrameType_Send,
    FrameType_Cmd = 50,
    FrameType_Cmd_Response = 51,
    FrameType_Set_Para = 52,
    FrameType_Set_Para_Response = 53,
    FrameType_Save_Para = 54,
    FrameType_SyncVar = 55,
    FrameType_SyncVar_Rsp_All = 56,
    FrameType_ToCamera,
    FrameType_FromCamera,
    FrameType_CaliStatus,
    FrameType_MotorDampData = 80,
}FrameType;

typedef enum{
    CmdType_Cali_TC = 0,
    CmdType_Cali_TC_Clear = 1,
    CmdType_Cali_AE = 2,
    CmdType_Cali_AE_Clear = 3,
    CmdType_Cali_MZ = 4,
    CmdType_Cali_MZ_Clear = 5,
    CmdType_Cali_Fr = 6,
    CmdType_Cali_Fr_Clear = 7,
    CmdType_Cali_EL = 8,
    CmdType_Cali_EL_Clear = 9,
    CmdType_Test_MR = 10,
    CmdType_Test_MC = 11,
    CmdType_Test_Vib = 12,
    CmdType_OpenPower = 13,
    CmdType_ClosePower = 14,
    CmdType_PrintVersion = 15,
    CmdType_EraseCtrPara = 16,
    CmdType_EraseStaticHis = 17,
    CmdType_ObserveEnable = 18,
    
    CmdType_SystemReset = 50,
    CmdType_SystemReset_Hold_IN_Bootloader= 51,
}GBCmdType;

typedef struct{
    uint32_t second;
}HeartBeat;

typedef struct{
    char text1[30];
}Console;

typedef struct{
    int16_t Millis;
    int16_t gyrox;
    int16_t gyroy;
    int16_t gyroz;
    int16_t accex;
    int16_t accey;
    int16_t accez;
    int16_t temperature;
    int16_t encoderPitch;
    int16_t encoderRoll;
    int16_t encoderYaw;
}GroupSensor;

typedef struct{
    int16_t Millis;
    int16_t eulerx;
    int16_t eulery;
    int16_t eulerz;
    int16_t encoderAnglex;
    int16_t encoderAngley;
    int16_t encoderAnglez;
    int16_t filtKp;
    int16_t filtKi;
    int16_t filtBeta;
}Att;

typedef struct{
    int16_t Millis;
    int16_t Desirex;
    int16_t Outputx;
    int16_t Desirey;
    int16_t OutPuty;
    int16_t Desirez;
    int16_t outPutz;
}AngleCtrLoop;

typedef struct{
    int16_t Millis;
    int16_t Desirex;
    int16_t Measurex;
    int16_t Outputx;
    int16_t Desirey;
    int16_t Measurey;
    int16_t OutPuty;
    int16_t Desirez;
    int16_t Measurez;
    int16_t OutPutz;
}GyroCtrLoop;

typedef struct{
    int16_t Millis;
    int16_t Voltage;
    int16_t Current;
    int16_t StaticSamplex;
    int16_t StaticSampley;
    int16_t StaticSamplez;
    int16_t GyroIntergx;
    int16_t GyroIntergy;
    int16_t GyroIntergz;
}SysPara;

typedef struct{
    int16_t Millis;
    int16_t sampleIa;
    int16_t sampleIb;
    int16_t sampleIalpha;
    int16_t sampleIbeta;
    int16_t sampleId;
    int16_t sampleIq;
    int16_t FiltId;
    int16_t FiltIq;
    int16_t Theta;
    int16_t IqSet;
    int16_t IdPidOut;
    int16_t IqPidOut;
    int16_t UAlpha;
    int16_t UBeta;
}CurLoop;

typedef struct{
    int16_t Millis;
    int16_t reserve[8];
}Reserved;

typedef struct{
    int8_t cmd;
}Cmd;

typedef struct{
    int8_t cmd;
    int8_t flag;
}CmdResponse;

typedef struct{
    uint16_t VarIndex;
    int32_t Value;
}SetPara;

typedef struct{
    uint16_t VarIndex;
    int8_t flag;
}SetParaResponse;

typedef struct{
    uint16_t VarIndex;
    int8_t type;
    char Name[16];
    uint32_t Value;
}SyncVariableInfo;

typedef struct{
    uint8_t data[32];
}ToCameraData;

typedef struct{
    uint8_t data[32];
}FromCameraData;

typedef struct{
    uint8_t dat[4];
}CaliStatus;

typedef struct{
	uint8_t axis;
	int16_t index;
    int16_t dat;
}MotorDamper;

#endif
