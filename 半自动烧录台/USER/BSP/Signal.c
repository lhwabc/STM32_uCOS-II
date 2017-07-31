/**
  ******************************************************************************
  * @file    Signal.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is MSLink for machine Signal source code.
  * 
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "Signal.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void Signal_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(START_A_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(BusySignal_A_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(OKSignal_A_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(NGSignal_A_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = START_A_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(START_A_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BusySignal_A_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BusySignal_A_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = OKSignal_A_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OKSignal_A_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = NGSignal_A_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NGSignal_A_GPIO_PORT, &GPIO_InitStructure);
    
    BusySignal_A(EN);
    OKSignal_A(EN);
    NGSignal_A(EN);
}

uint8_t START_A_Scan(void)
{			
	if(GPIO_ReadInputDataBit(START_A_GPIO_PORT,START_A_GPIO_PIN) == EN)  
	{	 
		while(GPIO_ReadInputDataBit(START_A_GPIO_PORT,START_A_GPIO_PIN) == DIS);   
		return 	EN;
	}
	else
    {
		return DIS;
    }
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
