/*
 * uart.c
 *
 *  Created on: Sep 26, 2016
 *      Author: baron
 */
#include "uart.h"
#include "pios_com.h"
#include "string.h"
//#include "global.h"

/* Provide a COM driver */
static void PIOS_USART_ChangeBaud(uint32_t usart_id, uint32_t baud);
static void PIOS_USART_SetCtrlLine(uint32_t usart_id, uint32_t mask, uint32_t state);
static void PIOS_USART_RegisterRxCallback(uint32_t usart_id, pios_com_callback rx_in_cb, uint32_t context);
static void PIOS_USART_RegisterTxCallback(uint32_t usart_id, pios_com_callback tx_out_cb, uint32_t context);
static void PIOS_USART_TxStart(uint32_t usart_id, uint16_t tx_bytes_avail);
static void USART_txDmaStart(uint32_t usart_id,uint32_t BufferAddr,uint16_t size);
static void PIOS_USART_RxStart(uint32_t usart_id, uint16_t rx_bytes_avail);
static void USART_RxDmaStart(uint32_t usart_id,uint32_t BufferAddr,uint16_t size);
static bool PIOS_USART_available(uint32_t id);
static void UsartRegisterDmaRxCallback(uint32_t id,DmaTransferCallback dma_rx_cb, uint32_t context);
static void UsartRegisterDmaTxCallback(uint32_t id,DmaTransferCallback dma_tx_cb, uint32_t context);
static uint16_t UsartRxDMACount(uint32_t id);
static uint16_t UsartTxDMACount(uint32_t id);
static void UartDmaTxIrqHandler(uint32_t id);
static void UartDmaRxIrqHandler(uint32_t id);

const struct com_driver usart_driver = {
    .set_baud      		= PIOS_USART_ChangeBaud,
    .set_ctrl_line 		= PIOS_USART_SetCtrlLine,
    .tx_start      		= PIOS_USART_TxStart,
    .rx_start      		= PIOS_USART_RxStart,
    .rxDmaStart 		= USART_RxDmaStart,
    .txDmaStart 		= USART_txDmaStart,
    .bind_tx_cb    		= PIOS_USART_RegisterTxCallback,
    .bind_rx_cb    		= PIOS_USART_RegisterRxCallback,
    .available	  		= PIOS_USART_available,
    .bind_DmaRx_cb		= UsartRegisterDmaRxCallback,
    .bind_DmaTx_cb		= UsartRegisterDmaTxCallback,
    .rxDMACount  		= UsartRxDMACount,
    .txDMACount  		= UsartTxDMACount,
};

enum pios_usart_dev_magic {
    PIOS_USART_DEV_MAGIC = 0x55617274,
};

struct pios_usart_dev {
    enum pios_usart_dev_magic   magic;
    GIMBAL_UART_CFG *cfg;

    pios_com_callback rx_in_cb;
    uint32_t rx_in_context;
    pios_com_callback tx_out_cb;
    uint32_t tx_out_context;
    
    DmaTransferCallback dmaTxCb;
    DmaTransferCallback dmaRxCb;
    uint32_t DmaTx_context;
    uint32_t DmaRx_context;
};

static bool PIOS_USART_validate(struct pios_usart_dev *usart_dev)
{
    return usart_dev->magic == PIOS_USART_DEV_MAGIC;
}

static bool PIOS_USART_available(uint32_t id)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;
	return PIOS_USART_validate(usart_dev);
}

#if defined(PIOS_INCLUDE_FREERTOS)
static struct pios_usart_dev *PIOS_USART_alloc(void)
{
    struct pios_usart_dev *usart_dev;

    usart_dev = (struct pios_usart_dev *)pvPortMalloc(sizeof(struct pios_usart_dev));
    if (!usart_dev) {
        return NULL;
    }

    memset(usart_dev, 0, sizeof(struct pios_usart_dev));
    usart_dev->magic = PIOS_USART_DEV_MAGIC;
    return usart_dev;
}
#else
static struct pios_usart_dev pios_usart_devs[PIOS_USART_MAX_DEVS];
static uint8_t pios_usart_num_devs;
static struct pios_usart_dev *PIOS_USART_alloc(void)
{
    struct pios_usart_dev *usart_dev;

    if (pios_usart_num_devs >= PIOS_USART_MAX_DEVS) {
        return NULL;
    }

    usart_dev = &pios_usart_devs[pios_usart_num_devs++];

    memset(usart_dev, 0, sizeof(struct pios_usart_dev));
    usart_dev->magic = PIOS_USART_DEV_MAGIC;

    return usart_dev;
}
#endif /* if defined(PIOS_INCLUDE_FREERTOS) */

/* Bind Interrupt Handlers
 *
 * Map all valid USART IRQs to the common interrupt handler
 * and provide storage for a 32-bit device id IRQ to map
 * each physical IRQ to a specific registered device instance.
 */
static void PIOS_USART_generic_irq_handler(uint32_t usart_id);

static uint32_t PIOS_USART_1_id;
void USART1_IRQHandler(void) __attribute__((alias("PIOS_USART_1_irq_handler")));
static void PIOS_USART_1_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_1_id);
}

static uint32_t PIOS_USART_2_id;
void USART2_IRQHandler(void) __attribute__((alias("PIOS_USART_2_irq_handler")));
static void PIOS_USART_2_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_2_id);
}

static uint32_t PIOS_USART_3_id;
void USART3_IRQHandler(void) __attribute__((alias("PIOS_USART_3_irq_handler")));
static void PIOS_USART_3_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_3_id);
}

static uint32_t PIOS_USART_4_id;
#if GIMBAL_SLAVE_UART_USE_DMA_IDLE_MODE != 1 || GIMBAL_SLAVECONTROLLER_COM_USE_UART4 != 1
void UART4_IRQHandler(void) __attribute__((alias("PIOS_USART_4_irq_handler")));
static void PIOS_USART_4_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_4_id);
}
#endif
static uint32_t PIOS_USART_5_id;
#if GIMBAL_SLAVE_UART_USE_DMA_IDLE_MODE != 1 || GIMBAL_SLAVECONTROLLER_COM_USE_UART5 != 1
void UART5_IRQHandler(void) __attribute__((alias("PIOS_USART_5_irq_handler")));
static void PIOS_USART_5_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_5_id);
}
#endif
static uint32_t PIOS_USART_6_id;
void USART6_IRQHandler(void) __attribute__((alias("PIOS_USART_6_irq_handler")));
static void PIOS_USART_6_irq_handler(void)
{
    PIOS_USART_generic_irq_handler(PIOS_USART_6_id);
}
void DMA1_Stream1_IRQHandler(void) __attribute__((alias("Usart3_RX_Dma_irq_handler")));
void DMA1_Stream3_IRQHandler(void) __attribute__((alias("Usart3_TX_Dma_irq_handler")));
void DMA2_Stream2_IRQHandler(void) __attribute__((alias("Usart1_RX_Dma_irq_handler")));
void DMA2_Stream7_IRQHandler(void) __attribute__((alias("Usart1_TX_Dma_irq_handler")));

static void Usart3_RX_Dma_irq_handler(void)
{
	UartDmaRxIrqHandler(PIOS_USART_3_id);
}

static void Usart3_TX_Dma_irq_handler(void)
{
	UartDmaTxIrqHandler(PIOS_USART_3_id);
}
static void Usart1_RX_Dma_irq_handler(void)
{
	UartDmaRxIrqHandler(PIOS_USART_1_id);
}

static void Usart1_TX_Dma_irq_handler(void)
{
	UartDmaTxIrqHandler(PIOS_USART_1_id);
}


static void UartDmaTxIrqHandler(uint32_t id)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	HAL_DMA_IRQHandler(usart_dev->cfg->hdmaTx);
}

static void UartDmaRxIrqHandler(uint32_t id)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	HAL_DMA_IRQHandler(usart_dev->cfg->hdmaRx);
	

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	struct pios_usart_dev *usart_dev;
	switch((uint32_t)UartHandle->Instance)
	{
		case (uint32_t)UART4:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_4_id;
			break;
		case (uint32_t)UART5:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_5_id;
			break;
		case (uint32_t)USART1:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_1_id;
			break;
		case (uint32_t)USART2:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_2_id;
			break;
		case (uint32_t)USART3:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_3_id;
			break;
		case (uint32_t)USART6:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_6_id;
			break; 
	}
	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
//	if(usart_dev->dmaTxCb)
//	{
//		usart_dev->dmaTxCb(usart_dev->DmaTx_context);
//	}
	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	struct pios_usart_dev *usart_dev;
	switch((uint32_t)UartHandle->Instance)
	{
		case (uint32_t)UART4:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_4_id;
			break;
		case (uint32_t)UART5:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_5_id;
			break;
		case (uint32_t)USART1:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_1_id;
			break;
		case (uint32_t)USART2:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_2_id;
			break;
		case (uint32_t)USART3:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_3_id;
			break;
		case (uint32_t)USART6:
			usart_dev = (struct pios_usart_dev *) PIOS_USART_6_id;
			break; 
	}
	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
//	if(usart_dev->dmaRxCb)
//	{
//		usart_dev->dmaRxCb(usart_dev->DmaRx_context);
//	}
}
/**
 * Initialise a single USART device
 */
int32_t USART_Init(uint32_t *usart_id, GIMBAL_UART_CFG *cfg,bool isUmaUsed)
{
    DEBUG_Assert(usart_id);
    DEBUG_Assert(cfg);

    struct pios_usart_dev *usart_dev;

    usart_dev = (struct pios_usart_dev *)PIOS_USART_alloc();
    if (!usart_dev) {
        goto out_fail;
    }

    /* Bind the configuration to the device instance */
    usart_dev->cfg = cfg;
    HAL_RCC_CLK_ENABLE(usart_dev->cfg->gpioRx.gpio);
    HAL_RCC_CLK_ENABLE(usart_dev->cfg->gpioTx.gpio);
    /* Map pins to USART function */
    /* note __builtin_ctz() due to the difference between GPIO_PinX and GPIO_PinSourceX */
	HAL_GPIO_Init(usart_dev->cfg->gpioRx.gpio,&usart_dev->cfg->gpioRx.initTypeDef);
	HAL_GPIO_Init(usart_dev->cfg->gpioTx.gpio,&usart_dev->cfg->gpioTx.initTypeDef);
    /* Initialize the USART Rx and Tx pins */


    /* Configure the USART */
	
	HAL_RCC_CLK_ENABLE(usart_dev->cfg->uartHandle->Instance);
	if(HAL_UART_Init(usart_dev->cfg->uartHandle) != HAL_OK)
	{
		DEBUG_Assert(1);
	}


    *usart_id = (uint32_t)usart_dev;

    /* Configure USART Interrupts */

	switch((uint32_t)usart_dev->cfg->uartHandle->Instance)
	{
		case (uint32_t)UART4:
			PIOS_USART_4_id = (uint32_t)usart_dev;
			break;
		case (uint32_t)UART5:
			PIOS_USART_5_id = (uint32_t)usart_dev;
			break;
		case (uint32_t)USART1:
			PIOS_USART_1_id = (uint32_t)usart_dev;
			break;
		case (uint32_t)USART2:
			PIOS_USART_2_id = (uint32_t)usart_dev;
			break;
		case (uint32_t)USART3:
			PIOS_USART_3_id = (uint32_t)usart_dev;
			break;
		case (uint32_t)USART6:
			PIOS_USART_6_id = (uint32_t)usart_dev;
			break; 
	}

    

	
	if(isUmaUsed)
	{
		HAL_RCC_CLK_ENABLE(usart_dev->cfg->hdmaTx->Instance);
		HAL_RCC_CLK_ENABLE(usart_dev->cfg->hdmaRx->Instance);
		
		
		if(HAL_DMA_Init(usart_dev->cfg->hdmaTx) != HAL_OK)
		{
			DEBUG_Assert(1);
		}
		
		usart_dev->cfg->uartHandle->hdmatx = usart_dev->cfg->hdmaTx; 
		usart_dev->cfg->hdmaTx->Parent = (usart_dev->cfg->uartHandle);             
										 
		
		if(HAL_DMA_Init(usart_dev->cfg->hdmaRx) != HAL_OK)
		{
			DEBUG_Assert(1);
		}
				
		(usart_dev->cfg->uartHandle)->hdmarx = (usart_dev->cfg->hdmaRx); 
		(usart_dev->cfg->hdmaRx)->Parent = (usart_dev->cfg->uartHandle);
		

		if(usart_dev->cfg->txDmaIrq.irq_enabled)
		{
			STM32_IRQ_CFG *irq = &usart_dev->cfg->txDmaIrq;	
			if(irq->irq_enabled == true)
			{
				HAL_NVIC_SetPriority(irq->irq_cfg.irq,irq->irq_cfg.nvic_preemptPriority,irq->irq_cfg.nvic_subPriority);
				HAL_NVIC_EnableIRQ(irq->irq_cfg.irq);
			}
		}
		if(usart_dev->cfg->rxDmaIrq.irq_enabled)
		{
			STM32_IRQ_CFG *irq = &usart_dev->cfg->rxDmaIrq;	
			if(irq->irq_enabled == true)
			{
				HAL_NVIC_SetPriority(irq->irq_cfg.irq,irq->irq_cfg.nvic_preemptPriority,irq->irq_cfg.nvic_subPriority);
				HAL_NVIC_EnableIRQ(irq->irq_cfg.irq);
			}
		}
	}

	STM32_IRQ_CFG *irq = &usart_dev->cfg->uartIrq;
	if(irq->irq_enabled == true)
	{
		HAL_NVIC_SetPriority(irq->irq_cfg.irq,irq->irq_cfg.nvic_preemptPriority,irq->irq_cfg.nvic_subPriority);
		HAL_NVIC_EnableIRQ(irq->irq_cfg.irq);
	}

	
	__HAL_UART_ENABLE(usart_dev->cfg->uartHandle);
    return 0;

out_fail:
    return -1;
}

static void PIOS_USART_RxStart(uint32_t usart_id, __attribute__((unused)) uint16_t rx_bytes_avail)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);
    if(usart_dev->cfg->uartIrq.irq_enabled)
    {

    	__HAL_UART_ENABLE_IT(usart_dev->cfg->uartHandle,UART_IT_RXNE);

    }
}

static void USART_RxDmaStart(uint32_t usart_id,uint32_t BufferAddr,uint16_t size)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);

	HAL_UART_Receive_DMA(usart_dev->cfg->uartHandle, (uint8_t *)BufferAddr, size);
	
	if(usart_dev->cfg->uartIrq.irq_enabled)
	{
		__HAL_UART_ENABLE_IT(usart_dev->cfg->uartHandle,UART_IT_IDLE);
	}
}

static void PIOS_USART_TxStart(uint32_t usart_id, __attribute__((unused)) uint16_t tx_bytes_avail)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);
    if(usart_dev->cfg->uartIrq.irq_enabled)
    {

		__HAL_UART_ENABLE_IT(usart_dev->cfg->uartHandle,UART_IT_TXE);

    }
}

static void USART_txDmaStart(uint32_t usart_id,uint32_t BufferAddr,uint16_t size)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	HAL_UART_Transmit_DMA(usart_dev->cfg->uartHandle,  (uint8_t *)BufferAddr, size);
}
/**
 * Changes the baud rate of the USART peripheral without re-initialising.
 * \param[in] usart_id USART name (GPS, TELEM, AUX)
 * \param[in] baud Requested baud rate
 */
static void PIOS_USART_ChangeBaud(uint32_t usart_id, uint32_t baud)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);

	usart_dev->cfg->uartHandle->Init.BaudRate = baud;
	HAL_UART_Init(usart_dev->cfg->uartHandle);
}

static uint16_t UsartRxDMACount(uint32_t id)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	return __HAL_DMA_GET_COUNTER(usart_dev->cfg->hdmaRx);
}

static uint16_t UsartTxDMACount(uint32_t id)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);
    
    return __HAL_DMA_GET_COUNTER(usart_dev->cfg->hdmaTx);
}

static void PIOS_USART_SetCtrlLine(uint32_t usart_id, uint32_t mask, uint32_t state)
{
#if 0
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);

    /* Only attempt to drive DTR if this USART has a GPIO line defined */
    if (usart_dev->cfg->dtr.gpio && (mask & COM_CTRL_LINE_DTR_MASK)) {
        GPIO_WriteBit(usart_dev->cfg->dtr.gpio,
                      usart_dev->cfg->dtr.init.GPIO_Pin,
                      state & COM_CTRL_LINE_DTR_MASK ? Bit_RESET : Bit_SET);
    }
#endif
}
static void UsartRegisterDmaRxCallback(uint32_t id,DmaTransferCallback dma_rx_cb, uint32_t context)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	usart_dev->DmaRx_context = context;
	usart_dev->dmaRxCb = dma_rx_cb;
}

static void UsartRegisterDmaTxCallback(uint32_t id,DmaTransferCallback dma_tx_cb, uint32_t context)
{
	struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)id;

	bool valid = PIOS_USART_validate(usart_dev);

	DEBUG_Assert(valid);
	
	usart_dev->DmaTx_context = context;
	usart_dev->dmaTxCb = dma_tx_cb;
}

static void PIOS_USART_RegisterRxCallback(uint32_t usart_id, pios_com_callback rx_in_cb, uint32_t context)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);

    /*
     * Order is important in these assignments since ISR uses _cb
     * field to determine if it's ok to dereference _cb and _context
     */
    usart_dev->rx_in_context = context;
    usart_dev->rx_in_cb = rx_in_cb;
}

static void PIOS_USART_RegisterTxCallback(uint32_t usart_id, pios_com_callback tx_out_cb, uint32_t context)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);

    /*
     * Order is important in these assignments since ISR uses _cb
     * field to determine if it's ok to dereference _cb and _context
     */
    usart_dev->tx_out_context = context;
    usart_dev->tx_out_cb = tx_out_cb;
}

static void PIOS_USART_generic_irq_handler(uint32_t usart_id)
{
    struct pios_usart_dev *usart_dev = (struct pios_usart_dev *)usart_id;

    bool valid = PIOS_USART_validate(usart_dev);

    DEBUG_Assert(valid);

    /* Force read of dr after sr to make sure to clear error flags */
    volatile uint32_t sr;
	volatile uint32_t dr;
	

	sr = usart_dev->cfg->uartHandle->Instance->SR;
	dr = usart_dev->cfg->uartHandle->Instance->DR;

	 /*------------------------------------------------------------------------------------------------------------
	 * nomal interrupt mode
	 *
	 * --------------*/
	/* Check if RXNE flag is set */
    bool rx_need_yield   = false;
    if (sr & USART_SR_RXNE) {
    	uint8_t byte = dr;
        if (usart_dev->rx_in_cb) {
            (void)(usart_dev->rx_in_cb)(usart_dev->rx_in_context, &byte, 1, NULL, &rx_need_yield);
        }
    }

    /* Check if TXE flag is set */
    bool tx_need_yield = false;
    if (sr & USART_SR_TXE) {
        if (usart_dev->tx_out_cb) {
            uint8_t b;
            uint16_t bytes_to_send;

            bytes_to_send = (usart_dev->tx_out_cb)(usart_dev->tx_out_context, &b, 1, NULL, &tx_need_yield);

            if (bytes_to_send > 0) {
                /* Send the byte we've been given */

				usart_dev->cfg->uartHandle->Instance->DR = b;
				
            } else {
                /* No bytes to send, disable TXE interrupt */

				__HAL_UART_DISABLE_IT(usart_dev->cfg->uartHandle,UART_IT_TXE);
				
            }
        } else {
            /* No bytes to send, disable TXE interrupt */
			__HAL_UART_DISABLE_IT(usart_dev->cfg->uartHandle,UART_IT_TXE);
			
        }
    }

    /*------------------------------------------------------------------------------------------------------------
     * DMA mode
     *
     * --------------*/
    if(sr & USART_SR_IDLE)
    {
    	//idle flag has been cleared by read sr and dr in start
    	if(usart_dev->dmaRxCb)
		{
			usart_dev->dmaRxCb(usart_dev->DmaRx_context,&rx_need_yield);
		}
    }

    if(sr & USART_SR_TC)
    {
    	__HAL_UART_DISABLE_IT(usart_dev->cfg->uartHandle,UART_IT_TC);
    	//uart_it_tc bit will be enabled in dma tranmit success process
    	if(usart_dev->dmaTxCb)
		{
			usart_dev->dmaTxCb(usart_dev->DmaTx_context,&tx_need_yield);
		}
    }
#if defined(PIOS_INCLUDE_FREERTOS)
    if (rx_need_yield || tx_need_yield) {
        vPortYield();
    }
#endif /* PIOS_INCLUDE_FREERTOS */
}



