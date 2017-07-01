/**
  ******************************************************************************
  * @file    key.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file defines the key interface driver function.  
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef __KEY_H
#define	__KEY_H
#include "stm32f10x.h"

#define             KEY1_GPIO_CLK                   RCC_APB2Periph_GPIOB
#define             KEY1_GPIO_PORT                  GPIOB			   
#define             KEY1_GPIO_PIN                   GPIO_Pin_14


#define KEY_ON	1
#define KEY_OFF	0


void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif
