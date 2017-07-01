/**
  ******************************************************************************
  * @file    led.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file defines the led interface driver function.  
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef __LED_H
#define	__LED_H
#include "stm32f10x.h"

/* 
   Define the GPIO port of the LED connection. 
   The user only needs to modify the following code to change the control LED pin
*/
#define LED_RED_GPIO_PORT       GPIOB
#define LED_RED_GPIO_CLK        RCC_APB2Periph_GPIOB
#define LED_RED_GPIO_PIN        GPIO_Pin_13

#define LED_GREEN_GPIO_PORT     GPIOB
#define LED_GREEN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define LED_GREEN_GPIO_PIN      GPIO_Pin_12

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  1
#define OFF 0

#define LED_RED(a)      if (a)	\
                        GPIO_SetBits(LED_RED_GPIO_PORT,LED_RED_GPIO_PIN);\
                        else		\
                        GPIO_ResetBits(LED_RED_GPIO_PORT,LED_RED_GPIO_PIN)

#define LED_GREEN(a)    if (a)	\
                        GPIO_SetBits(LED_GREEN_GPIO_PORT,LED_GREEN_GPIO_PIN);\
                        else		\
                        GPIO_ResetBits(LED_GREEN_GPIO_PORT,LED_GREEN_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
