#ifndef BOARD_HW_DEFS_H_
#define BOARD_HW_DEFS_H_
#ifdef __cplusplus
 extern "C" {
#endif


#include "driver_stm32.h"

#define HAL_RCC_CLK_ENABLE(instance)    hal_rcc_clk_enable((uint32_t)instance)

 
void hal_rcc_clk_enable(uint32_t instance);

enum{
	LED_GREEN=0,
    LED_RED,
    LED_BLUE,
    LED_YELLOW,
    LED_PINK,
    LED_CYAN,
    LED_WHITE,
    LED_DARK,
};

enum{
    RES0=0,
    RES1,
};

enum{
    UartChannel1 = 0,
    UartChannel2,
    UartType_Num,
};

enum{
    TimerChannel1 = 0,
    TimerChannel2,
    TimerChannel3,
    TimerChannel4,
    Timer_Num,
};

enum{
	MotorOutPutChannel1 = 0,
	MotorOutPutChannel2,
	MotorOutPut_Num,
};

enum{
	MotorPhase1 = 0,
	MotorPhase2,
	MotorPhase3,
    MotorPhase_Num,
};

enum{
	EncoderDatChannel1 = 0,
	EncoderDatChannel2,
	EncoderDat_Num,
};

enum{
	MotorRotateReverse_ABC = 0,
	MotorRotateReverse_ACB,
	MotorRotateReverse_BAC,
	MotorRotateReverse_BCA,
	MotorRotateReverse_CAB,
	MotorRotateReverse_CBA,
};
/****************************************************
*   Gimbal Led Hal Driver Struct Defs
*
****************************************************/
 typedef struct {
     STM32_GPIO pin;
     bool active_high;
 }DriverLed;

 typedef struct  {
     const DriverLed * leds;
     uint8_t num_leds;
 }DriverLedCfg;

 typedef struct {
     STM32_GPIO pin;
 }DriverIDRes;

 typedef struct  {
     const DriverIDRes * res;
     uint8_t num_res;
 }DriverIDResCfg;


#define DriverGpio  DriverLed
/****************************************************
*   Gimbal SPI Hal Driver Struct Defs
*
****************************************************/
//typedef struct{
//    SPI_HandleTypeDef   *hspi;
//
//    DMA_HandleTypeDef   *hdma_spi_rx;
//    DMA_HandleTypeDef   *hdma_spi_tx;
//
//    bool                dma_spi_tx_enabled;
//    bool                dma_spi_rx_enabled;
//
//    STM32_IRQ_CFG       spi_irq_cfg;
//    STM32_IRQ_CFG       dma_spi_tx_irq_cfg;
//    STM32_IRQ_CFG       dma_spi_rx_irq_cfg;
//
//    STM32_GPIO  clk;
//    STM32_GPIO  miso;
//    STM32_GPIO  mosi;
//    uint8_t     slave_count;
//    STM32_GPIO  ssel[];
//}GIMBAL_SPI_CFG;


//typedef struct  {
//    GIMBAL_SPI_CFG *cfg;
//    Bus_PeripheralType  busPeripType;
//    void    (*callback)(uint8_t, uint8_t);
//    uint8_t tx_dummy_byte;
//    uint8_t rx_dummy_byte;
//
//    xSemaphoreHandle busy;
//
//}GIMBAL_SPI_DEV;

/****************************************************
*   Gimbal USART Hal Driver Struct Defs
*
****************************************************/

typedef struct{
	UART_HandleTypeDef  *uartHandle;

    DMA_HandleTypeDef   *hdmaTx;
    DMA_HandleTypeDef   *hdmaRx;

    STM32_IRQ_CFG       uartIrq;
    STM32_IRQ_CFG       txDmaIrq;
    STM32_IRQ_CFG       rxDmaIrq;
    STM32_GPIO          gpioTx;
    STM32_GPIO          gpioRx;
}GIMBAL_UART_CFG;


/****************************************************
*   Gimbal TIMER Hal Driver Struct Defs
*
****************************************************/

typedef struct{
    TIM_HandleTypeDef   *tim;
    STM32_IRQ_CFG       tim_irq;
}GIMBAL_TIM_TIMER_CFG;


/****************************************************
*   Gimbal TIMER_PWM_OUTPUT Hal Driver Struct Defs
*
****************************************************/

typedef struct{
    TIM_HandleTypeDef   *tim;
    STM32_IRQ_CFG       tim_irq;
    TIM_OC_InitTypeDef  oc;
    TIM_MasterConfigTypeDef sMC;
    TIM_ClockConfigTypeDef sCSC;
    uint32_t            CCTimChannel;
    uint32_t            TimChannel[3];
    STM32_GPIO          Gpio[3];
}GIMBAL_TIM_PWMOUT_CFG;


/****************************************************
*   Gimbal TIMER_PWM_INPUT Hal Driver Struct Defs
*
****************************************************/

typedef struct{
    TIM_HandleTypeDef   *timerHandle;
    TIM_IC_InitTypeDef  periodIC;
    uint32_t            periodChannel;
    TIM_IC_InitTypeDef  pulseIC;
    uint32_t            pulseChannel;
    TIM_SlaveConfigTypeDef  sSlaveConfig;
    STM32_GPIO              gpioCapture;
}GIMBAL_TIM_PWMIN_CFG;


/****************************************************
*   Gimbal ADC Hal Driver Struct Defs
*
****************************************************/

typedef struct{
    ADC_HandleTypeDef       *hadc;
    DMA_HandleTypeDef       *hdma;
    bool                    irq_enabled;
    STM32_IRQ_CFG           dmairq;
    uint8_t                 channelNum;
    ADC_ChannelConfTypeDef  sConfig[3];
    STM32_GPIO              ADCGpio[3];
}GIMBAL_ADC_CFG;


/****************************************************
*   Gimbal IIC Hal Driver Struct Defs
*
****************************************************/

//typedef struct{
//    I2C_HandleTypeDef   *iicHandle;
//    STM32_IRQ           irq_ev;
//    STM32_IRQ           irq_er;
//    STM32_GPIO          gpioSDA;
//    STM32_GPIO          gpioSCL;
//}GIMBAL_IIC_CFG;


/****************************************************
*   Gimbal CAN Hal Driver Struct Defs
*
****************************************************/
typedef struct{
    CAN_HandleTypeDef *hcan;
    uint8_t sFilterCount;
    CAN_FilterConfTypeDef  sFilterConfig[15];
    STM32_IRQ         canTxIRQ;
    STM32_IRQ         canRxIRQ;
    STM32_GPIO        gpioRXTX;
}GIMBAL_CAN_CFG;

typedef struct{
    GIMBAL_UART_CFG 		const *usart0;
    GIMBAL_UART_CFG 		const *usart1;
    GIMBAL_TIM_TIMER_CFG	const *timer0;
    GIMBAL_TIM_TIMER_CFG	const *timer1;
    GIMBAL_TIM_TIMER_CFG	const *timer2;
    GIMBAL_TIM_TIMER_CFG	const *timer3;
    GIMBAL_TIM_PWMOUT_CFG 	const *pwmout0;
    GIMBAL_TIM_PWMOUT_CFG 	const *pwmout1;
    GIMBAL_TIM_PWMOUT_CFG 	const *pwmout2;
    GIMBAL_TIM_PWMIN_CFG 	const *pwmin0;
    GIMBAL_TIM_PWMIN_CFG 	const *pwmin1;
    GIMBAL_TIM_PWMIN_CFG 	const *pwmin2;
    GIMBAL_ADC_CFG 			const *adc0;
    GIMBAL_ADC_CFG 			const *adc1;
    GIMBAL_ADC_CFG 			const *adc2;
    GIMBAL_CAN_CFG 			const *can0;
    GIMBAL_CAN_CFG 			const *can1;
}HAL;

extern const HAL hal;


/***************************************************
*   Define ends
****************************************************/
#ifdef __cplusplus
 }
#endif
#endif
