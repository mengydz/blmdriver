#include "canardmain.h"
#include "svpwm.h"
#include "motordriver.h"
#include "can.h"
#include "myMath.h"

uint8_t Flag_Statue_Tx = CAN_IDLE;
uint8_t GetSysInfo = false;
uint8_t GetPitchEncoder = false;
ModulesStatusInfo SysStatusInfo;
/********************************************************************/
/*
 * Application constants
 */
#define APP_VERSION_MAJOR                                           1
#define APP_VERSION_MINOR                                           0
#define APP_NODE_NAME                                               "org.uavcan.libcanard.demo"

#define REV_MAX_LENGTH_BYTES                                      	32

/*
 * Library instance.
 * In simple applications it makes sense to make it static, but it is not necessary.
 */
CanardInstance canard;                       ///< The library instance
static uint8_t canard_memory_pool[2048];            ///< Arena for memory allocation, used by the library

void SetSpeed(void)
{
	static uint8_t transfer_id;
	uint8_t buffer[UAVCAN_GIMBAL_SET_SPEED_MESSAGE_SIZE];
	for(uint16_t i=0;i<UAVCAN_GIMBAL_SET_SPEED_MESSAGE_SIZE;i++)
	{
		buffer[i] = i;
	}
	const int bc_res = canardBroadcast(&canard, UAVCAN_GIMBAL_SET_SPEED_TYPE_SIGNATURE,
			UAVCAN_GIMBAL_SET_SPEED_DATA_TYPE_ID, &transfer_id, CANARD_TRANSFER_PRIORITY_LOW,
									 buffer, UAVCAN_GIMBAL_SET_SPEED_MESSAGE_SIZE);
	
	if(Flag_Statue_Tx == CAN_IDLE)
	{
		CAN_HandleTypeDef *hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
		const CanardCANFrame* txf = canardPeekTxQueue(&canard);
		Flag_Statue_Tx = CAN_SENDING;
		memcpy(hcan->pTxMsg,0,sizeof(CanTxMsgTypeDef));
		hcan->pTxMsg->StdId = 0x0000;
		hcan->pTxMsg->ExtId = txf->id;
		hcan->pTxMsg->IDE = CAN_ID_EXT;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = txf->data_len;
		memcpy(hcan->pTxMsg->Data, txf->data, hcan->pTxMsg->DLC);
		HAL_CAN_Transmit_IT(hcan);
		canardPopTxQueue(&canard);
	}
}

void PitchResponse(uint16_t value)
{
	static uint8_t transfer_id;
	uint8_t buffer[UAVCAN_GIMBAL_PIT_RESPONSE_MESSAGE_SIZE];

	buffer[0] = value/256;buffer[1] = value%256;
	
	const int bc_res = canardBroadcast(&canard, UAVCAN_GIMBAL_PIT_RESPONSE_TYPE_SIGNATURE,
			UAVCAN_GIMBAL_PIT_RESPONSE_DATA_TYPE_ID, &transfer_id, CANARD_TRANSFER_PRIORITY_LOW,
									 buffer, UAVCAN_GIMBAL_PIT_RESPONSE_MESSAGE_SIZE);
	
	if(Flag_Statue_Tx == CAN_IDLE)
	{
		CAN_HandleTypeDef *hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
		const CanardCANFrame* txf = canardPeekTxQueue(&canard);
		Flag_Statue_Tx = CAN_SENDING;
		memcpy(hcan->pTxMsg,0,sizeof(CanTxMsgTypeDef));
		hcan->pTxMsg->StdId = 0x0000;
		hcan->pTxMsg->ExtId = txf->id;
		hcan->pTxMsg->IDE = CAN_ID_EXT;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = txf->data_len;
		memcpy(hcan->pTxMsg->Data, txf->data, hcan->pTxMsg->DLC);
		HAL_CAN_Transmit_IT(hcan);
		canardPopTxQueue(&canard);
	}
}

void GimbalRequest(void)
{
	static uint8_t transfer_id;
	uint8_t buffer[UAVCAN_GIMBAL_CMD_CONTROL_MESSAGE_SIZE];
	for(uint16_t i=0;i<UAVCAN_GIMBAL_CMD_CONTROL_MESSAGE_SIZE;i++)
	{
		buffer[i] = i;
	}
	const int bc_res = canardBroadcast(&canard, UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE,
			UAVCAN_GIMBAL_CMD_CONTROL_DATA_TYPE_ID, &transfer_id, CANARD_TRANSFER_PRIORITY_LOW,
									 buffer, UAVCAN_GIMBAL_CMD_CONTROL_MESSAGE_SIZE);
	
	if(Flag_Statue_Tx == CAN_IDLE)
	{
		CAN_HandleTypeDef *hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
		const CanardCANFrame* txf = canardPeekTxQueue(&canard);
		Flag_Statue_Tx = CAN_SENDING;
		memcpy(hcan->pTxMsg,0,sizeof(CanTxMsgTypeDef));
		hcan->pTxMsg->StdId = 0x0000;
		hcan->pTxMsg->ExtId = txf->id;
		hcan->pTxMsg->IDE = CAN_ID_EXT;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = txf->data_len;
		memcpy(hcan->pTxMsg->Data, txf->data, hcan->pTxMsg->DLC);
		HAL_CAN_Transmit_IT(hcan);
		canardPopTxQueue(&canard);
	}
}

void GimbalResponse(uint8_t* data)
{
	static uint8_t transfer_id;
	uint8_t buffer[UAVCAN_GIMBAL_INFO_RESPONSE_MESSAGE_SIZE];
	for(uint16_t i=0;i<UAVCAN_GIMBAL_INFO_RESPONSE_MESSAGE_SIZE;i++)
	{
		buffer[i] = *(data+i);
	}
	const int bc_res = canardBroadcast(&canard, UAVCAN_GIMBAL_INFO_RESPONSE_TYPE_SIGNATURE,
			UAVCAN_GIMBAL_INFO_RESPONSE_DATA_TYPE_ID, &transfer_id, CANARD_TRANSFER_PRIORITY_LOW,
									 buffer, UAVCAN_GIMBAL_INFO_RESPONSE_MESSAGE_SIZE);
	
	if(Flag_Statue_Tx == CAN_IDLE)
	{
		CAN_HandleTypeDef *hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
		const CanardCANFrame* txf = canardPeekTxQueue(&canard);
		Flag_Statue_Tx = CAN_SENDING;
		memcpy(hcan->pTxMsg,0,sizeof(CanTxMsgTypeDef));
		hcan->pTxMsg->StdId = 0x0000;
		hcan->pTxMsg->ExtId = txf->id;
		hcan->pTxMsg->IDE = CAN_ID_EXT;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = txf->data_len;
		memcpy(hcan->pTxMsg->Data, txf->data, hcan->pTxMsg->DLC);
		HAL_CAN_Transmit_IT(hcan);
		canardPopTxQueue(&canard);
	}
}

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
static void onTransferReceived(CanardInstance* ins,
                               CanardRxTransfer* transfer)
{
    if (transfer->transfer_type == CanardTransferTypeBroadcast)
    {
    	switch (transfer->data_type_id)
    	{
			case UAVCAN_GIMBAL_SET_SPEED_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					DataConvert _motorpower;
					SvpwmDrive	*svpwmDrive = (SvpwmDrive*)svpwmID;
					_motorpower.bytes[0] = transfer->payload_head[0];
					_motorpower.bytes[1] = transfer->payload_head[1];
					_motorpower.bytes[2] = transfer->payload_head[2];
					_motorpower.bytes[3] = transfer->payload_head[3];
					Constrain(_motorpower.fdata,MINMOTORPOWER,MAXMOTORPOWER);
					svpwmDrive->out = _motorpower.fdata;
					svpwmDrive->isClosedLoop = true;
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_SET_VECTOR_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					DataConvert _motorpower;
					SvpwmDrive	*svpwmDrive = (SvpwmDrive*)svpwmID;
					_motorpower.bytes[0] = transfer->payload_head[0];
					_motorpower.bytes[1] = transfer->payload_head[1];
					_motorpower.bytes[2] = transfer->payload_head[2];
					_motorpower.bytes[3] = transfer->payload_head[3];
					Constrain(_motorpower.fdata,MINMOTORPOWER,MAXMOTORPOWER);
					svpwmDrive->out = _motorpower.fdata;
					svpwmDrive->isClosedLoop = false;
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_SET_CURRENT_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_CMD_CONTROL_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					switch(transfer->payload_head[0])
					{
						case 1:
							GetSysInfo = true;
							break;
						case 2:
							GetPitchEncoder = true;
							break;
						case 3:
							
							break;
						default:break;
					}
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_PIT_RESPONSE_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_ROL_RESPONSE_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_YAW_RESPONSE_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
			case UAVCAN_GIMBAL_SET_MOTORSWITCH_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
					if(transfer->payload_head[0])
						MotorSwitchOn();
					else
						MotorSwitchOff();
				}else if(transfer->payload_len <= 12)
				{//head 0-5 tail 0-5
					
				}else
				{//head 0-5 middle ...
					
				}
			}break;
    		default:break;
    	}
    }
    
}


/**
 * This callback is invoked by the library when it detects beginning of a new transfer on the bus that can be received
 * by the local node.
 * If the callback returns true, the library will receive the transfer.
 * If the callback returns false, the library will ignore the transfer.
 * All transfers that are addressed to other nodes are always ignored.
 */
static bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{
    (void)source_node_id;

    if (transfer_type == CanardTransferTypeBroadcast)
	{
		switch (data_type_id)
		{
			case UAVCAN_GIMBAL_SET_SPEED_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_SET_SPEED_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_SET_VECTOR_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_SET_VECTOR_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_SET_CURRENT_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_SET_CURRENT_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_CMD_CONTROL_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_PIT_RESPONSE_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_ROL_RESPONSE_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_YAW_RESPONSE_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_CMD_CONTROL_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_SET_MOTORSWITCH_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_SET_MOTORSWITCH_TYPE_SIGNATURE;
				return true;
			}
			case UAVCAN_GIMBAL_INFO_RESPONSE_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_GIMBAL_INFO_RESPONSE_TYPE_SIGNATURE;
				return true;
			}
			default:break;
		}
	}
    return false;
}

void CanardMainInit(void)
{
	canardInit(&canard, canard_memory_pool, sizeof(canard_memory_pool), onTransferReceived, shouldAcceptTransfer, NULL);
	canard.node_id = CANARD_STM32_GIMBAL_NODE_ID;
}


/********************************************************************/


