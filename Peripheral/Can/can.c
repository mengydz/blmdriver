/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "canard.h"
#include "canardmain.h"

uint32_t hal_CAN_Plane_ID;
uint32_t hal_CAN_Gimbal_ID;

extern CanardInstance canard;
extern uint8_t Flag_Statue_Tx;

bool CANInit(const GIMBAL_CAN_CFG * cfg,uint32_t *can_id)
{
	HAL_RCC_CLK_ENABLE(cfg->hcan->Instance);
	HAL_RCC_CLK_ENABLE(cfg->gpioRXTX.gpio);

	HAL_GPIO_Init(cfg->gpioRXTX.gpio, (GPIO_InitTypeDef *)&cfg->gpioRXTX.initTypeDef);
	
	if (HAL_CAN_Init(cfg->hcan) != HAL_OK)
	{
		return false;
	}

	for(uint8_t i = 0;i<cfg->sFilterCount;i++)
	{
		if(HAL_CAN_ConfigFilter(cfg->hcan, (CAN_FilterConfTypeDef*)&cfg->sFilterConfig[i]) != HAL_OK)
		{
		/* Filter configuration Error */
			return false;
		}
	}
	HAL_NVIC_SetPriority(cfg->canTxIRQ.irq,cfg->canTxIRQ.nvic_preemptPriority,cfg->canTxIRQ.nvic_subPriority);
	HAL_NVIC_EnableIRQ(cfg->canTxIRQ.irq);
	HAL_NVIC_SetPriority(cfg->canRxIRQ.irq,cfg->canRxIRQ.nvic_preemptPriority,cfg->canRxIRQ.nvic_subPriority);
	HAL_NVIC_EnableIRQ(cfg->canRxIRQ.irq);
	
//	enable can it through FIFO0/1.
	cfg->hcan->pTxMsg = malloc(sizeof(CanTxMsgTypeDef));
	cfg->hcan->pRxMsg = malloc(sizeof(CanRxMsgTypeDef));
	cfg->hcan->pRx1Msg = malloc(sizeof(CanRxMsgTypeDef));
	__HAL_CAN_ENABLE_IT(cfg->hcan, CAN_IT_FMP0|CAN_IT_FMP1|CAN_IT_TME);

	*can_id = (uint32_t)cfg->hcan;

	return true;
}

void CAN1_TX_IRQHandler(void)
{
	CAN_HandleTypeDef *hcan;
	hcan = (CAN_HandleTypeDef *)hal_CAN_Plane_ID;
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_IRQHandler(hcan);
	}
	hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_IRQHandler(hcan);
	}
}
void CAN1_RX0_IRQHandler(void)
{
	CAN_HandleTypeDef *hcan;
	hcan = (CAN_HandleTypeDef *)hal_CAN_Plane_ID;
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_IRQHandler(hcan);
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP0);
	}
	hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_IRQHandler(hcan);
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP0);
	}
}

void CAN2_TX_IRQHandler(void)
{
	CAN_HandleTypeDef *hcan;
	hcan = (CAN_HandleTypeDef *)hal_CAN_Plane_ID;
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_IRQHandler(hcan);
	}
	hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_IRQHandler(hcan);
	}
}
void CAN2_RX1_IRQHandler(void)
{
	CAN_HandleTypeDef *hcan;
	hcan = (CAN_HandleTypeDef *)hal_CAN_Plane_ID;
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_IRQHandler(hcan);
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP1);
	}
	hcan = (CAN_HandleTypeDef *)hal_CAN_Gimbal_ID;
	if(hcan->Instance == CAN2)
	{
		HAL_CAN_IRQHandler(hcan);
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP1);
	}
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
	const CanardCANFrame* txf = canardPeekTxQueue(&canard);
	if(txf != NULL)
	{
		Flag_Statue_Tx = CAN_SENDING;
		memcpy(hcan->pTxMsg,0,sizeof(CanTxMsgTypeDef));
		hcan->pTxMsg->StdId = 0x0000;
		hcan->pTxMsg->ExtId = txf->id;
		hcan->pTxMsg->IDE = CAN_ID_EXT;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = txf->data_len;
		memcpy(hcan->pTxMsg->Data, txf->data, hcan->pTxMsg->DLC);
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_TME);
		HAL_CAN_Transmit_IT(hcan);
		canardPopTxQueue(&canard);
	}else
	{
		Flag_Statue_Tx = CAN_IDLE;
	}
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    CanardCANFrame rx_frame;
	const uint64_t timestamp = GetMicro();
	if(hcan->Instance == CAN1)
	{
		if((hcan->pRxMsg->IDE == CAN_ID_STD ) && ((hcan->pRxMsg->StdId == GIMBAL_REBOOT_ID1 ) \
				|| (hcan->pRxMsg->StdId == GIMBAL_REBOOT_ID2 ) || (hcan->pRxMsg->StdId == GIMBAL_REBOOT_ID3 ) \
				|| (hcan->pRxMsg->StdId == GIMBAL_REBOOT_ID4 )) )
		{
			if((hcan->pRxMsg->Data[0] == 0xA2) && (hcan->pRxMsg->Data[1] == 0xB2)&& (hcan->pRxMsg->Data[2] == 0xC2)&& (hcan->pRxMsg->Data[3] == 0xD2))
			{
				if(hcan->pRxMsg->Data[4] == 0x01)
				{
					SystemReboot(true);
				}else if(hcan->pRxMsg->Data[4] == 0x02)
				{
					SystemReboot(false);
				}
			}
		}
		rx_frame.id = hcan->pRxMsg->ExtId;
		if (hcan->pRxMsg->IDE == CAN_ID_EXT)
		{
			// set the EFF bit for canard id.
			rx_frame.id |= CANARD_CAN_FRAME_EFF;
		}
		if (hcan->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			// set the RTR bit for canard id.
			rx_frame.id |= CANARD_CAN_FRAME_RTR;
		}

		rx_frame.data_len = (uint8_t) hcan->pRxMsg->DLC;
		memcpy(rx_frame.data, hcan->pRxMsg->Data, rx_frame.data_len);
		canardHandleRxFrame(&canard, &rx_frame, timestamp);		
	}else if(hcan->Instance == CAN2)
	{
		if((hcan->pRx1Msg->IDE == CAN_ID_STD ) && ((hcan->pRx1Msg->StdId == GIMBAL_REBOOT_ID1 ) \
				|| (hcan->pRx1Msg->StdId == GIMBAL_REBOOT_ID2 ) || (hcan->pRx1Msg->StdId == GIMBAL_REBOOT_ID3 ) \
				|| (hcan->pRx1Msg->StdId == GIMBAL_REBOOT_ID4 )) )
		{
			if((hcan->pRx1Msg->Data[0] == 0xA2) && (hcan->pRx1Msg->Data[1] == 0xB2)&& (hcan->pRx1Msg->Data[2] == 0xC2)&& (hcan->pRx1Msg->Data[3] == 0xD2))
			{
				if(hcan->pRx1Msg->Data[4] == 0x01)
				{
					SystemReboot(true);
				}else if(hcan->pRx1Msg->Data[4] == 0x02)
				{
					SystemReboot(false);
				}
			}
		}
		rx_frame.id = hcan->pRx1Msg->ExtId;
		if (hcan->pRx1Msg->IDE == CAN_ID_EXT)
		{
			// set the EFF bit for canard id.
			rx_frame.id |= CANARD_CAN_FRAME_EFF;
		}
		if (hcan->pRx1Msg->RTR == CAN_RTR_REMOTE)
		{
			// set the RTR bit for canard id.
			rx_frame.id |= CANARD_CAN_FRAME_RTR;
		}

		rx_frame.data_len = (uint8_t) hcan->pRx1Msg->DLC;
		memcpy(rx_frame.data, hcan->pRx1Msg->Data, rx_frame.data_len);
		canardHandleRxFrame(&canard, &rx_frame, timestamp);		
	}	
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
