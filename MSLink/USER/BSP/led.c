/**
  ******************************************************************************
  * @file    led.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file is led source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_RED_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(LED_GREEN_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_RED_GPIO_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = LED_GREEN_GPIO_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);
}

