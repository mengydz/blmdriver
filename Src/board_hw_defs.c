#include "board_hw_defs.h"
#include "stm32f4xx_hal.h"
#include "canardmain.h"

void hal_rcc_clk_enable(uint32_t instance)
{
    switch((uint32_t)instance){                                                                                 
        case (uint32_t )TIM1:           __HAL_RCC_TIM1_CLK_ENABLE();    break;                  
        case (uint32_t )TIM2:           __HAL_RCC_TIM2_CLK_ENABLE();    break;                  
        case (uint32_t )TIM3:           __HAL_RCC_TIM3_CLK_ENABLE();    break;                  
        case (uint32_t )TIM4:           __HAL_RCC_TIM4_CLK_ENABLE();    break;                  
        case (uint32_t )TIM5:           __HAL_RCC_TIM5_CLK_ENABLE();    break;                  
        case (uint32_t )TIM6:           __HAL_RCC_TIM6_CLK_ENABLE();    break;                  
        case (uint32_t )TIM7:           __HAL_RCC_TIM7_CLK_ENABLE();    break;                  
        case (uint32_t )TIM8:           __HAL_RCC_TIM8_CLK_ENABLE();    break;                  
        case (uint32_t )TIM9:           __HAL_RCC_TIM9_CLK_ENABLE();    break;                  
        case (uint32_t )TIM10:          __HAL_RCC_TIM10_CLK_ENABLE();   break;                  
        case (uint32_t )TIM11:          __HAL_RCC_TIM11_CLK_ENABLE();   break;                  
        case (uint32_t )TIM12:          __HAL_RCC_TIM12_CLK_ENABLE();   break;                  
        case (uint32_t )TIM13:          __HAL_RCC_TIM13_CLK_ENABLE();   break;                  
        case (uint32_t )TIM14:          __HAL_RCC_TIM14_CLK_ENABLE();   break;                  
                                                                                        
        case (uint32_t)GPIOA:           __HAL_RCC_GPIOA_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOB:           __HAL_RCC_GPIOB_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOC:           __HAL_RCC_GPIOC_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOD:           __HAL_RCC_GPIOD_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOE:           __HAL_RCC_GPIOE_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOF:           __HAL_RCC_GPIOF_CLK_ENABLE();   break;                  
        case (uint32_t)GPIOH:           __HAL_RCC_GPIOH_CLK_ENABLE();   break;                  
        
        case (uint32_t)ADC1:            __HAL_RCC_ADC1_CLK_ENABLE();    break;                  
        case (uint32_t)ADC2:            __HAL_RCC_ADC2_CLK_ENABLE();    break;                  
        case (uint32_t)ADC3:            __HAL_RCC_ADC3_CLK_ENABLE();    break;
        
        case (uint32_t)DMA1_Stream0:
        case (uint32_t)DMA1_Stream1:
        case (uint32_t)DMA1_Stream2:
        case (uint32_t)DMA1_Stream3:
        case (uint32_t)DMA1_Stream4:
        case (uint32_t)DMA1_Stream5:
        case (uint32_t)DMA1_Stream6:
        case (uint32_t)DMA1_Stream7:
        case (uint32_t)DMA1:            __HAL_RCC_DMA1_CLK_ENABLE();    break;                  
        case (uint32_t)DMA2_Stream0:    __HAL_RCC_DMA2_CLK_ENABLE();    break;
        case (uint32_t)DMA2_Stream1:
        case (uint32_t)DMA2_Stream2:
        case (uint32_t)DMA2_Stream3:    __HAL_RCC_DMA2_CLK_ENABLE();    break;
        case (uint32_t)DMA2_Stream4:
        case (uint32_t)DMA2_Stream5:
        case (uint32_t)DMA2_Stream6:
        case (uint32_t)DMA2_Stream7:
        case (uint32_t)DMA2:            __HAL_RCC_DMA2_CLK_ENABLE();    break;                  
        
        case (uint32_t)USART1:          __HAL_RCC_USART1_CLK_ENABLE();  break;              
        case (uint32_t)USART2:          __HAL_RCC_USART2_CLK_ENABLE();  break;              
        case (uint32_t)USART3:          __HAL_RCC_USART3_CLK_ENABLE();  break;              
        case (uint32_t)UART4:           __HAL_RCC_UART4_CLK_ENABLE();   break;                  
        case (uint32_t)UART5:           __HAL_RCC_UART5_CLK_ENABLE();   break;                  
        case (uint32_t)USART6:          __HAL_RCC_USART6_CLK_ENABLE();  break;              
        
        case (uint32_t)I2C1:            __HAL_RCC_I2C1_CLK_ENABLE();    break;                  
        case (uint32_t)I2C2:            __HAL_RCC_I2C2_CLK_ENABLE();    break;                  
        
        case (uint32_t)SPI1:            __HAL_RCC_SPI1_CLK_ENABLE();    break;                  
        case (uint32_t)SPI2:            __HAL_RCC_SPI2_CLK_ENABLE();    break;                  
        
        case (uint32_t)CAN1:            __HAL_RCC_CAN1_CLK_ENABLE();    break;                  
        case (uint32_t)CAN2:            __HAL_RCC_CAN1_CLK_ENABLE();    
        								__HAL_RCC_CAN2_CLK_ENABLE();	break;                  
        
        default:while(1);break;
        
    }
}

DMA_HandleTypeDef _com1DmaRx = {
	.Instance = DMA2_Stream2,
	.Init = {
		.Mode = DMA_CIRCULAR,
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_PERIPH_TO_MEMORY,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Priority = DMA_PRIORITY_MEDIUM,
		.FIFOMode = DMA_FIFOMODE_DISABLE,
		.FIFOThreshold = DMA_FIFOMODE_DISABLE,
		.MemBurst = DMA_MBURST_SINGLE,
		.PeriphBurst = DMA_PBURST_SINGLE
	}
};
DMA_HandleTypeDef _com1DmaTx = {
	.Instance = DMA2_Stream7,
	.Init = {
		.Mode = DMA_NORMAL,
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_MEMORY_TO_PERIPH,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Priority = DMA_PRIORITY_HIGH,
		.FIFOMode = DMA_FIFOMODE_ENABLE,
		.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
		.MemBurst = DMA_MBURST_SINGLE,
		.PeriphBurst = DMA_PBURST_SINGLE,
	}
};
DMA_HandleTypeDef _com2DmaRx = {
	.Instance = DMA1_Stream1,
	.Init = {
		.Mode = DMA_CIRCULAR,
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_PERIPH_TO_MEMORY,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Priority = DMA_PRIORITY_HIGH,
		.FIFOMode = DMA_FIFOMODE_DISABLE,
		.FIFOThreshold = DMA_FIFOMODE_DISABLE,
		.MemBurst = DMA_MBURST_SINGLE,
		.PeriphBurst = DMA_PBURST_SINGLE
	}
};
DMA_HandleTypeDef _com2DmaTx = {
	.Instance = DMA1_Stream3,
	.Init = {
		.Mode = DMA_NORMAL,
		.Channel = DMA_CHANNEL_4,
		.Direction = DMA_MEMORY_TO_PERIPH,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Priority = DMA_PRIORITY_MEDIUM,
		.FIFOMode = DMA_FIFOMODE_ENABLE,
		.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
		.MemBurst = DMA_MBURST_SINGLE,
		.PeriphBurst = DMA_PBURST_SINGLE,
	}
};

UART_HandleTypeDef _com1Uart = {
	.Instance = USART1,
	.Init = {
		.BaudRate = 115200,
		.WordLength = UART_WORDLENGTH_8B,
		.StopBits = UART_STOPBITS_1,
		.Parity = UART_PARITY_NONE,
		.Mode = UART_MODE_TX_RX,
	},
};

UART_HandleTypeDef _com2Uart = {
	.Instance = USART3,
	.Init = {
        .BaudRate = 115200,
		.WordLength = UART_WORDLENGTH_8B,
		.StopBits = UART_STOPBITS_1,
		.Parity = UART_PARITY_NONE,
		.Mode = UART_MODE_TX_RX,
	},
};

const GIMBAL_UART_CFG gimbalUartCfg[UartType_Num] = {
	[UartChannel1] = {
        .uartHandle = &_com1Uart,
        .uartIrq = {
            .irq_enabled = true,
            .irq_cfg = {
                .irq = USART1_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_HIGH,
                .nvic_subPriority = 0,
            },
            .irqFlagNum = 2,
            .irqFlag[0] = UART_IT_RXNE,
            .irqFlag[1] = UART_IT_TXE,
        },

        .hdmaTx		 = &_com1DmaTx,
        .txDmaIrq = {
            .irq_enabled = true,
            .irq_cfg = {
                .irq = DMA2_Stream7_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_HIGH,
                .nvic_subPriority = 0,
            },
            .irqFlagNum = 1,
            .irqFlag[0] = DMA_IT_TC,
        },


        .hdmaRx		 = &_com1DmaRx,
        .rxDmaIrq = {
            .irq_enabled = false,
            .irq_cfg = {
                .irq = DMA2_Stream2_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_HIGH,
                .nvic_subPriority = 0,
            },
            .irqFlagNum = 1,
            .irqFlag[0] = DMA_IT_TC,
        },
        .gpioTx = {
            .gpio = GPIOB,
            .initTypeDef = {
                .Pin = GPIO_PIN_6,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
                .Alternate = GPIO_AF7_USART1,
            }

        },

        .gpioRx = {
            .gpio = GPIOB,
            .initTypeDef = {
                .Pin = GPIO_PIN_7,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
                .Alternate = GPIO_AF7_USART1,
            }
        }
    },
    [UartChannel2] = {
        .uartHandle = &_com2Uart,
        .uartIrq = {
            .irq_enabled = true,
            .irq_cfg = {
                .irq = USART3_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_MID,
                .nvic_subPriority = 0,
            },
        },

        .hdmaTx		 = &_com2DmaTx,
        .txDmaIrq = {
            .irq_enabled = true,
            .irq_cfg = {
                .irq = DMA1_Stream3_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_MID,
                .nvic_subPriority = 0,
            },
            .irqFlagNum = 1,
            .irqFlag[0] = DMA_IT_TC,
        },


        .hdmaRx		 = &_com2DmaRx,
        .rxDmaIrq = {
            .irq_enabled = false,
            .irq_cfg = {
                .irq = DMA1_Stream1_IRQn,
                .nvic_preemptPriority = IRQ_PRIO_MID,
                .nvic_subPriority = 0,
            },
            .irqFlagNum = 1,
            .irqFlag[0] = DMA_IT_TC,
        },
        .gpioTx = {
            .gpio = GPIOC,
            .initTypeDef = {
                .Pin = GPIO_PIN_10,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
                .Alternate = GPIO_AF7_USART3,
            }

        },

        .gpioRx = {
            .gpio = GPIOC,
            .initTypeDef = {
                .Pin = GPIO_PIN_11,
                .Mode = GPIO_MODE_AF_OD,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
                .Alternate = GPIO_AF7_USART3,
            }
        }
    },
};

const GIMBAL_UART_CFG * GetUsartCfg(uint32_t usart_type)
{

    switch(usart_type)
    {
        case UartChannel1:	return &gimbalUartCfg[UartChannel1];

        case UartChannel2:	return &gimbalUartCfg[UartChannel2];

        default:return NULL;
    }
}
/*------------------------------------------------------*/
TIM_HandleTypeDef gimbalPwmInTimer[] = {
	[EncoderDatChannel1] = {
		.Instance = TIM2,
		.Init = {
			.Prescaler = 0,
			.Period = 0xffff,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1,//center-aligned mode selection
		},
	},
	[EncoderDatChannel2] = {
		.Instance = TIM3,
		.Init = {
			.Prescaler = 0,
			.Period = 0xffff,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1,//center-aligned mode selection
		},
	},
};

TIM_HandleTypeDef	PwmOutTim[] = {
	[MotorOutPutChannel1] = {
		.Instance = TIM8,
		.Init = {
			.Prescaler = 83,
			.Period = 2000,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2,//center-aligned mode selection
		},
	},
	[MotorOutPutChannel2] = {
		.Instance = TIM1,
		.Init = {
			.Prescaler = 3-1,
			.Period = 2000,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2,//center-aligned mode selection
		},
	},
};

TIM_HandleTypeDef	TimerTim[] = {
	[TimerChannel1] = {
		.Instance = TIM7,
		.Init = {
			.Prescaler = 84-1,
			.Period = 50000,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_UP,//center-aligned mode selection
		},
	},
	[TimerChannel2] = {
		.Instance = TIM4,
		.Init = {
			.Prescaler = 84-1,
			.Period = 1000,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_UP,//center-aligned mode selection
		},
	},
	[TimerChannel3] = {
		.Instance = TIM5,
		.Init = {
			.Prescaler = 84-1,
			.Period = 1000,
			.ClockDivision = TIM_CLOCKDIVISION_DIV1,
			.CounterMode = TIM_COUNTERMODE_UP,//center-aligned mode selection
		},
	},
};

const GIMBAL_TIM_PWMIN_CFG	gimbalPwmInputCfg[] = {
	[EncoderDatChannel1] = {
		.timerHandle = &gimbalPwmInTimer[EncoderDatChannel1],
		.periodIC = {
			.ICPolarity = TIM_ICPOLARITY_RISING,
			.ICSelection = TIM_ICSELECTION_DIRECTTI,
			.ICPrescaler = TIM_ICPSC_DIV1,
			.ICFilter = 0,
		},
		.periodChannel = TIM_CHANNEL_1,
		.pulseIC = {
			.ICPolarity = TIM_ICPOLARITY_FALLING,
			.ICSelection = TIM_ICSELECTION_INDIRECTTI,
			.ICPrescaler = TIM_ICPSC_DIV1,
			.ICFilter = 0,
		},
		.pulseChannel = TIM_CHANNEL_2,
		.sSlaveConfig = {
			.SlaveMode = TIM_SLAVEMODE_RESET,
			.InputTrigger = TIM_TS_TI1FP1,
			.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED,
			.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1,
			.TriggerFilter = 0,
		},
		.gpioCapture = {
			.gpio = GPIOB,
			.initTypeDef = {
				.Pin = GPIO_PIN_8,
				.Mode = GPIO_MODE_AF_OD,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF1_TIM2,
			},
		},
	},
	[EncoderDatChannel2] = {
		.timerHandle = &gimbalPwmInTimer[EncoderDatChannel2],
		.periodIC = {
			.ICPolarity = TIM_ICPOLARITY_RISING,
			.ICSelection = TIM_ICSELECTION_DIRECTTI,
			.ICPrescaler = TIM_ICPSC_DIV1,
			.ICFilter = 0,
		},
		.periodChannel = TIM_CHANNEL_1,
		.pulseIC = {
			.ICPolarity = TIM_ICPOLARITY_FALLING,
			.ICSelection = TIM_ICSELECTION_INDIRECTTI,
			.ICPrescaler = TIM_ICPSC_DIV1,
			.ICFilter = 0,
		},
		.pulseChannel = TIM_CHANNEL_2,
		.sSlaveConfig = {
			.SlaveMode = TIM_SLAVEMODE_RESET,
			.InputTrigger = TIM_TS_TI1FP1,
			.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED,
			.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1,
			.TriggerFilter = 0,
		},
		.gpioCapture = {
			.gpio = GPIOA,
			.initTypeDef = {
				.Pin = GPIO_PIN_6,
				.Mode = GPIO_MODE_AF_OD,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF2_TIM3,
			},
		},
	},
};

const GIMBAL_TIM_PWMOUT_CFG gimbalPWMOutCfg[] = {
		[MotorOutPutChannel1] = {
			.tim = &PwmOutTim[MotorOutPutChannel1],
			.tim_irq = {
				.irq_enabled = false,
				.irq_cfg = {
					.irq = TIM8_UP_TIM13_IRQn,
					.nvic_preemptPriority = IRQ_PRIO_HIGH,
					.nvic_subPriority = 0,
				},
				.irqFlagNum = 1,
				.irqFlag[0] = TIM_IT_UPDATE,
			},
			.sMC = {
				.MasterOutputTrigger = TIM_TRGO_OC1REF,
				.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE,
			},
			.oc	= {
				.OCMode = TIM_OCMODE_PWM1,
				.OCPolarity		= TIM_OCPOLARITY_HIGH,
				.OCFastMode		= TIM_OCFAST_DISABLE,
				.OCNPolarity	= TIM_OCNPOLARITY_HIGH,
				.OCNIdleState	= TIM_OCNIDLESTATE_RESET,
				.OCIdleState	= TIM_OCIDLESTATE_RESET,
				.Pulse 			= 1000,
			},
			.CCTimChannel = TIM_CHANNEL_1,
			.TimChannel[MotorPhase1] = TIM_CHANNEL_2,
			.Gpio[MotorPhase1] = {
				.gpio = GPIOC,
				.initTypeDef = {
					.Pin = GPIO_PIN_7,
					.Mode = GPIO_MODE_AF_PP,
					.Pull = GPIO_PULLUP,
					.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
					.Alternate = GPIO_AF3_TIM8,
				}
			},
			.TimChannel[MotorPhase2] = TIM_CHANNEL_3,
			.Gpio[MotorPhase2] = {
				.gpio = GPIOC,
				.initTypeDef = {
					.Pin = GPIO_PIN_8,
					.Mode = GPIO_MODE_AF_PP,
					.Pull = GPIO_PULLUP,
					.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
					.Alternate = GPIO_AF3_TIM8,
				}
			},
			.TimChannel[MotorPhase3] = TIM_CHANNEL_4,
			.Gpio[MotorPhase3] = {
				.gpio = GPIOC,
				.initTypeDef = {
					.Pin = GPIO_PIN_9,
					.Mode = GPIO_MODE_AF_PP,
					.Pull = GPIO_PULLUP,
					.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
					.Alternate = GPIO_AF3_TIM8,
				}
			},
		},
	[MotorOutPutChannel2] = {
		.tim = &PwmOutTim[MotorOutPutChannel2],
		.tim_irq = {
			.irq_enabled = false,
			.irq_cfg = {
				.irq = TIM1_UP_TIM10_IRQn,
				.nvic_preemptPriority = IRQ_PRIO_HIGH,
				.nvic_subPriority = 0,
			},
			.irqFlagNum = 1,
			.irqFlag[0] = TIM_IT_UPDATE,
		},
		.sMC = {
			.MasterOutputTrigger = TIM_TRGO_OC1REF,
			.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
		},
		.oc	= {
			.OCMode			= TIM_OCMODE_PWM1,
			.OCPolarity		= TIM_OCPOLARITY_HIGH,
			.OCFastMode		= TIM_OCFAST_DISABLE,
			.OCNPolarity	= TIM_OCNPOLARITY_HIGH,
			.OCNIdleState	= TIM_OCNIDLESTATE_RESET,
			.OCIdleState	= TIM_OCIDLESTATE_RESET,
			.Pulse 			= 1000,
		},
		.CCTimChannel = TIM_CHANNEL_1,
		.TimChannel[MotorPhase1] = TIM_CHANNEL_1,
		.Gpio[MotorPhase1] = {
			.gpio = GPIOA,
			.initTypeDef = {
				.Pin = GPIO_PIN_8,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF1_TIM1,
			}
		},
		.TimChannel[MotorPhase2] = TIM_CHANNEL_2,
		.Gpio[MotorPhase2] = {
			.gpio = GPIOA,
			.initTypeDef = {
				.Pin = GPIO_PIN_9,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF1_TIM1,
			}
		},
		.TimChannel[MotorPhase3] = TIM_CHANNEL_3,
		.Gpio[MotorPhase3] = {
			.gpio = GPIOA,
			.initTypeDef = {
				.Pin = GPIO_PIN_10,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF1_TIM1,
			}
		},
	},
};

const GIMBAL_TIM_TIMER_CFG gimbalTimerCfg[] = {
		[TimerChannel1] = {
			.tim = &TimerTim[TimerChannel1],
			.tim_irq = {
				.irq_enabled = true,
				.irq_cfg = {
					.irq = TIM7_IRQn,
					.nvic_preemptPriority = IRQ_PRIO_LOW,
					.nvic_subPriority = 0,
				},
				.irqFlagNum = 1,
				.irqFlag[0] = TIM_IT_UPDATE,
			},
		},
		[TimerChannel2] = {
			.tim = &TimerTim[TimerChannel2],
			.tim_irq = {
				.irq_enabled = true,
				.irq_cfg = {
					.irq = TIM4_IRQn,
					.nvic_preemptPriority = IRQ_PRIO_MID,
					.nvic_subPriority = 0,
				},
				.irqFlagNum = 1,
				.irqFlag[0] = TIM_IT_UPDATE,
			},
		},
		[TimerChannel3] = {
			.tim = &TimerTim[TimerChannel3],
			.tim_irq = {
				.irq_enabled = true,
				.irq_cfg = {
					.irq = TIM5_IRQn,
					.nvic_preemptPriority = IRQ_PRIO_MID,
					.nvic_subPriority = 0,
				},
				.irqFlagNum = 1,
				.irqFlag[0] = TIM_IT_UPDATE,
			},
		},
};
/*-----------------------------------------------------------------------*/
const GIMBAL_TIM_TIMER_CFG *GetTimerTimCfg(uint8_t i)
{
    if(i<(sizeof(gimbalTimerCfg)/sizeof(gimbalTimerCfg[0])))
        return &gimbalTimerCfg[i];
    else
        return NULL;
}

const GIMBAL_TIM_PWMIN_CFG *GetPWMInCfg(uint8_t i)
{
    if(i<(sizeof(gimbalPwmInputCfg)/sizeof(gimbalPwmInputCfg[0])))
        return &gimbalPwmInputCfg[i];
    else
        return NULL;
}

GIMBAL_TIM_PWMOUT_CFG const *GetPwmOutCfg(uint8_t i)
{
	if(i<(sizeof(gimbalPWMOutCfg)/sizeof(gimbalPWMOutCfg[0])))
		return &gimbalPWMOutCfg[i];
	else
		return NULL;
}
/*-----------------------------------------------------------------------*/

ADC_HandleTypeDef		PwmoutChan0CurrentSampleADCHandle = {
	.Instance = ADC1,
	.Init = {
		.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4,
		.Resolution = ADC_RESOLUTION_12B,
		.ScanConvMode = ENABLE,
		.ContinuousConvMode = DISABLE,
		.DiscontinuousConvMode = DISABLE,
		.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING,
		.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO,
		.DataAlign = ADC_DATAALIGN_RIGHT,
		.NbrOfConversion = 2,
		.DMAContinuousRequests = ENABLE,
		.EOCSelection = ADC_EOC_SINGLE_CONV,
	},
};
DMA_HandleTypeDef _adc1Dma = {
    .Instance = DMA2_Stream0,
    .Init = {
    	.Channel = DMA_CHANNEL_0,
    	.Direction = DMA_PERIPH_TO_MEMORY,
    	.PeriphInc = DMA_PINC_DISABLE,
    	.MemInc = DMA_MINC_ENABLE,
    	.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD,
    	.MemDataAlignment = DMA_PDATAALIGN_HALFWORD,
    	.Mode = DMA_CIRCULAR,
    	.Priority = DMA_PRIORITY_LOW,
    	.FIFOMode = DMA_FIFOMODE_DISABLE,
        .PeriphBurst = DMA_PBURST_SINGLE,
    }
};

ADC_HandleTypeDef		VoltageSampleADCHandle = {
	.Instance = ADC3,
	.Init = {
		.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2,
		.Resolution		= ADC_RESOLUTION12b,
		.DataAlign		= ADC_DATAALIGN_RIGHT,
		.ScanConvMode	= DISABLE,
		.ContinuousConvMode		= ENABLE,
		.DiscontinuousConvMode	= DISABLE,
		.NbrOfConversion		= 1,
		.ExternalTrigConv		= ADC_SOFTWARE_START,
	},
};

const GIMBAL_ADC_CFG gimbalPwmout0CurSmpADCCfg = {
	.hadc = &PwmoutChan0CurrentSampleADCHandle,
	.irq_enabled = false,
	.channelNum = 2,
	.hdma = &_adc1Dma,
	.dmairq = {
		.irq_enabled = true,
		.irq_cfg = {
			.irq = DMA2_Stream0_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_HIGH,
			.nvic_subPriority = 0,
    	},
        .irqFlagNum = 1,
        .irqFlag[0] = DMA_IT_TC,
	},
	.sConfig[0] = {
		.Channel      = ADC_CHANNEL_3,
		.Rank         = 1,
		.SamplingTime = ADC_SAMPLETIME_3CYCLES,
	},
	.ADCGpio[0] = {
		.gpio = GPIOA,
		.initTypeDef = {
			.Pin = GPIO_PIN_3,
			.Mode = GPIO_MODE_ANALOG,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_MEDIUM,
		},
	},
	.sConfig[1] = {
		.Channel      = ADC_CHANNEL_4,
		.Rank         = 2,
		.SamplingTime = ADC_SAMPLETIME_3CYCLES,
	},
	.ADCGpio[1] = {
		.gpio = GPIOA,
		.initTypeDef = {
			.Pin = GPIO_PIN_4,
			.Mode = GPIO_MODE_ANALOG,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_MEDIUM,
		},
	},
};

const GIMBAL_ADC_CFG gimbalVoltageSmpADCCfg = {
	.hadc = &VoltageSampleADCHandle,
	.irq_enabled = false,
	.channelNum = 1,
	.hdma = NULL,//不使能
	.dmairq = {
		.irq_enabled = false,
		.irq_cfg = {
			.irq = DMA2_Stream1_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_HIGH,
			.nvic_subPriority = 0,
    	},
        .irqFlagNum = 1,
        .irqFlag[0] = DMA_IT_TC,
	},
	.sConfig[0] = {
		.Channel      = ADC_CHANNEL_0,
		.Rank         = 1,
		.SamplingTime = ADC_SAMPLETIME_3CYCLES,
	},
	.ADCGpio[0] = {
		.gpio = GPIOC,
		.initTypeDef = {
			.Pin = GPIO_PIN_4,
			.Mode = GPIO_MODE_ANALOG,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_MEDIUM,
		},
	},

};

/*-----------------------------------------------------------------------
**************************************************************************
-------------------------------------------------------------------------*/
CAN_HandleTypeDef hcan1 = {
	.Instance = CAN1,
	.Init = {
		.Prescaler = 3,
		.Mode = CAN_MODE_NORMAL,
		.SJW  = CAN_SJW_1TQ,
		.BS1  = CAN_BS1_10TQ,
		.BS2  = CAN_BS2_3TQ,
		.TTCM = DISABLE,
		.ABOM = ENABLE,
		.AWUM = DISABLE,
		.NART = DISABLE,
		.RFLM = DISABLE,
		.TXFP = ENABLE,
	}
};

CAN_HandleTypeDef hcan2 = {
	.Instance = CAN2,
	.Init = {
		.Prescaler = 3,
		.Mode = CAN_MODE_NORMAL,
		.SJW  = CAN_SJW_1TQ,
		.BS1  = CAN_BS1_10TQ,
		.BS2  = CAN_BS2_3TQ,
		.TTCM = DISABLE,
		.ABOM = ENABLE,
		.AWUM = DISABLE,
		.NART = DISABLE,
		.RFLM = DISABLE,
		.TXFP = ENABLE,
	}
};

const GIMBAL_CAN_CFG GimbalCanCfg[2] = {
	[0] = {
		.hcan = &hcan1,
		.sFilterCount = 1,
		.sFilterConfig = {
			[0] = {
				.FilterNumber = 0,
				.FilterMode = CAN_FILTERMODE_IDMASK,
				.FilterScale = CAN_FILTERSCALE_32BIT,
				.FilterIdHigh = (((((uint32_t)UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID<<8) & 0xFFFF00)<<3)>>16) & 0xFFFF,
				.FilterIdLow = (uint16_t)(((UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID<<8) & 0xFFFF00)<<3)|CAN_ID_EXT,			//0x28 is can nod ids
				.FilterMaskIdHigh = ((0XFFFF00<<3)>>16) & 0xFFFF,
				.FilterMaskIdLow = (uint16_t)(0XFFFF00<<3)|CAN_ID_EXT,
				.FilterFIFOAssignment = CAN_FILTER_FIFO0,
				.FilterActivation = ENABLE,
				.BankNumber = 14,
			},
		},
		.canTxIRQ = {
			.irq = CAN1_TX_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_MID,
			.nvic_subPriority = 0,
		},
		.canRxIRQ = {
			.irq = CAN1_RX0_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_MID,
			.nvic_subPriority = 0,
		},
		.gpioRXTX = {
			.gpio = GPIOA,
			.initTypeDef = {
				.Pin = GPIO_PIN_11|GPIO_PIN_12,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF9_CAN1,
			},
		},
	},
	[1] = {
		.hcan = &hcan2,
		.sFilterCount = 1,
		.sFilterConfig = {
			[0] = {
				.FilterNumber = 14,
				.FilterMode = CAN_FILTERMODE_IDMASK,
				.FilterScale = CAN_FILTERSCALE_32BIT,
				.FilterIdHigh = (((((uint32_t)UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID<<8) & 0xFFFF00)<<3)>>16) & 0xFFFF,
				.FilterIdLow = (uint16_t)(((UAVCAN_AIRDATA_GIMBALCONTROLDATA_DATA_TYPE_ID<<8) & 0xFFFF00)<<3)|CAN_ID_EXT,			//0x28 is can nod ids
				.FilterMaskIdHigh = ((0XFFFF00<<3)>>16) & 0xFFFF,
				.FilterMaskIdLow = (uint16_t)(0XFFFF00<<3)|CAN_ID_EXT,
				.FilterFIFOAssignment = CAN_FILTER_FIFO1,
				.FilterActivation = ENABLE,
				.BankNumber = 14,
			},
		},
		.canTxIRQ = {
			.irq = CAN2_TX_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_MID,
			.nvic_subPriority = 0,
		},
		.canRxIRQ = {
			.irq = CAN2_RX1_IRQn,
			.nvic_preemptPriority = IRQ_PRIO_MID,
			.nvic_subPriority = 0,
		},
		.gpioRXTX = {
			.gpio = GPIOB,
			.initTypeDef = {
				.Pin = GPIO_PIN_12|GPIO_PIN_13,
				.Mode = GPIO_MODE_AF_PP,
				.Pull = GPIO_PULLUP,
				.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
				.Alternate = GPIO_AF9_CAN2,
			},
		},
	},
};

const HAL hal = {
    .usart0  = &gimbalUartCfg[0],
    .usart1	 = &gimbalUartCfg[1],
    .timer0	 = &gimbalTimerCfg[0],
    .timer1	 = &gimbalTimerCfg[1],
    .timer2	 = &gimbalTimerCfg[2],
    .pwmout0 = &gimbalPWMOutCfg[0],
    .pwmout1 = &gimbalPWMOutCfg[1],
    .pwmin0  = &gimbalPwmInputCfg[0],
    .pwmin1  = &gimbalPwmInputCfg[1],
    .adc0 	 = &gimbalPwmout0CurSmpADCCfg,
    .adc1 	 = &gimbalVoltageSmpADCCfg,
    .can0 	 = &GimbalCanCfg[0],
    .can1 	 = &GimbalCanCfg[1],
};
/*------------------------------------------------------*/
