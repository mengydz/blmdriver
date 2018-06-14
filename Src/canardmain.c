#include "canardmain.h"
#include "can.h"
#include "myMath.h"
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

uint8_t Flag_Statue_Tx = CAN_IDLE;
uint8_t GetSysInfo = false;
uint8_t GetPitchEncoder = false;
ModulesStatusInfo SysStatusInfo;
CanardCANFrame CanRxQueueBuffer[CanRxQueueBufferSize];
t_fifo_buffercan CanRxBuffer;

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

void SetGimbalAngle(void)
{
	static uint8_t transfer_id;
	uint8_t buffer[UAVCAN_AIRDATA_GIMBALCONTROLDATA_MESSAGE_SIZE];
	for(uint16_t i=0;i<UAVCAN_AIRDATA_GIMBALCONTROLDATA_MESSAGE_SIZE;i++)
	{
		buffer[i] = i;
	}
	const int bc_res = canardBroadcast(&canard, UAVCAN_AIRDATA_GIMBALCONTROLDATA_TYPE_SIGNATURE,
			UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID, &transfer_id, CANARD_TRANSFER_PRIORITY_LOW,
									 buffer, UAVCAN_AIRDATA_GIMBALCONTROLDATA_MESSAGE_SIZE);
	
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
			case UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID:
			{
				if(transfer->payload_len <= 7)
				{//head 0-6
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
			case UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID:
			{
				*out_data_type_signature = UAVCAN_AIRDATA_GIMBALCONTROLDATA_TYPE_SIGNATURE;
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

void CanardRevBufferInit(void)
{
	CanfifoBuf_init(&CanRxBuffer,CanRxQueueBuffer,CanRxQueueBufferSize);
}
//CanardmainTask任务
void CanardmainTask(void const * argument)
{
  portTickType xLastWakeTime;
  uint16_t SendFreqCnt=0;

  CanardCANFrame*pPeekCanRx;
  //延时时间单元初始值记录
  xLastWakeTime = xTaskGetTickCount();
  while(1)
  {
    vTaskDelayUntil(&xLastWakeTime,portTICK_RATE_MS);
    /*******************receive********************************/
	while(CanfifoBuf_getUsed(&CanRxBuffer))
	{
		canardHandleRxFrame(&canard, CanfifoBuf_getByte(&CanRxBuffer), SendFreqCnt);
	}
	/**********send***************************************************/
	if(SendFreqCnt%5 == 0)
    {
//    	SetGimbalAngle();
    }
    if(SendFreqCnt%10 == 5)
    {

    }
    if(SendFreqCnt%100 == 0)
    {

    }
    SendFreqCnt++;
    SendFreqCnt %= 10000;
  }
}


/********************************************************************/


