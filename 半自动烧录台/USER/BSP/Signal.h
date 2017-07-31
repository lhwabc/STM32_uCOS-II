/**
  ******************************************************************************
  * @file    Signal.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is MSLink for machine Signal source code.
  * @Hardware£º---------------------
  *          |   PB14 - START_A     |
  *          |   PA10 - usySignal_A |
  *          |   PA09 - OKSignal_A  |
  *          |   PB13 - NGSignal_A  |
  *           ----------------------
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#ifndef __SIGNAL_H
#define	__SIGNAL_H
#include "stm32f10x.h"

#define START_A_GPIO_PORT           GPIOB
#define START_A_GPIO_CLK            RCC_APB2Periph_GPIOB
#define START_A_GPIO_PIN            GPIO_Pin_14

#define BusySignal_A_GPIO_PORT      GPIOA
#define BusySignal_A_GPIO_CLK       RCC_APB2Periph_GPIOA
#define BusySignal_A_GPIO_PIN       GPIO_Pin_10

#define OKSignal_A_GPIO_PORT        GPIOA
#define OKSignal_A_GPIO_CLK         RCC_APB2Periph_GPIOA
#define OKSignal_A_GPIO_PIN         GPIO_Pin_9

#define NGSignal_A_GPIO_PORT        GPIOB
#define NGSignal_A_GPIO_CLK         RCC_APB2Periph_GPIOB
#define NGSignal_A_GPIO_PIN         GPIO_Pin_13

/* the macro definition to trigger the Signal enable or disable */
#define EN  0
#define DIS 1

#define BusySignal_A(a)	if (a)	\
					GPIO_SetBits(BusySignal_A_GPIO_PORT,BusySignal_A_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BusySignal_A_GPIO_PORT,BusySignal_A_GPIO_PIN)

#define OKSignal_A(a)	if (a)	\
					GPIO_SetBits(OKSignal_A_GPIO_PORT,OKSignal_A_GPIO_PIN);\
					else		\
					GPIO_ResetBits(OKSignal_A_GPIO_PORT,OKSignal_A_GPIO_PIN)
                    
#define NGSignal_A(a)	if (a)	\
					GPIO_SetBits(NGSignal_A_GPIO_PORT,NGSignal_A_GPIO_PIN);\
					else		\
					GPIO_ResetBits(NGSignal_A_GPIO_PORT,NGSignal_A_GPIO_PIN)

void Signal_GPIO_Config(void);
uint8_t START_A_Scan(void);

#endif /* __LED_H */
