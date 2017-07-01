/**
  ******************************************************************************
  * @file    bsp.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file is bsp source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "includes.h"

/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
    SystemInit();		/* ����ϵͳʱ��Ϊ72M */	
	SysTick_init();		/* ��ʼ����ʹ��SysTick��ʱ�� */
    
    LED_GPIO_Config();  /* LED �˿ڳ�ʼ�� */
    Key_GPIO_Config();
}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
    SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
}

