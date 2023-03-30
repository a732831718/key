#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "stdio.h"

void Uart_IT_Init(void);

void instruct_dat_printf(void);

#endif
