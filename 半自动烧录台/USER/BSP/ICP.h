/**
  ******************************************************************************
  * @file    ICP.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file defines the ICP interface driver function.  
  * @Hardware：------------------
  *          |   PB12 - LED1     |
  *          |   PB13 - LED2     |
  *          |   PB12 - LED3     |
  *          |   PB13 - LED3     |
  *           -------------------
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef __ICP_H
#define __ICP_H
#include "includes.h"

#define NRST_GPIO_PORT          GPIOC
#define NRST_GPIO_CLK           RCC_APB2Periph_GPIOC
#define NRST_GPIO_PIN           GPIO_Pin_15

#define VPP_GPIO_PORT           GPIOB
#define VPP_GPIO_CLK            RCC_APB2Periph_GPIOB
#define VPP_GPIO_PIN            GPIO_Pin_15

#define PMODE_GPIO_PORT         GPIOC
#define PMODE_GPIO_CLK          RCC_APB2Periph_GPIOC
#define PMODE_GPIO_PIN          GPIO_Pin_0

#define PDQS_GPIO_PORT          GPIOC
#define PDQS_GPIO_CLK           RCC_APB2Periph_GPIOC
#define PDQS_GPIO_PIN           GPIO_Pin_1

#define PCLE_GPIO_PORT          GPIOC
#define PCLE_GPIO_CLK           RCC_APB2Periph_GPIOC
#define PCLE_GPIO_PIN           GPIO_Pin_2

#define PALE_GPIO_PORT          GPIOC
#define PALE_GPIO_CLK           RCC_APB2Periph_GPIOC
#define PALE_GPIO_PIN           GPIO_Pin_3

#define PDLE_GPIO_PORT          GPIOA
#define PDLE_GPIO_CLK           RCC_APB2Periph_GPIOA
#define PDLE_GPIO_PIN           GPIO_Pin_0

#define PDATA_GPIO_PORT         GPIOA
#define PDATA_GPIO_CLK          RCC_APB2Periph_GPIOA
#define PDATA_GPIO_PIN          GPIO_Pin_1

/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)          {p->BSRR=i;}            //设置为高电平      
#define digitalLo(p,i)          {p->BRR=i;}             //输出低电平
#define digitalToggle(p,i)      {p->ODR ^=i;}           //输出反转状态

/* 定义控制IO的宏 */
#define NRST_HI         digitalHi(NRST_GPIO_PORT, NRST_GPIO_PIN)
#define NRST_LO         digitalLo(NRST_GPIO_PORT, NRST_GPIO_PIN)

#define VPP_HI          digitalHi(VPP_GPIO_PORT, VPP_GPIO_PIN)
#define VPP_LO          digitalLo(VPP_GPIO_PORT, VPP_GPIO_PIN)

#define PMODE_HI        digitalHi(PMODE_GPIO_PORT, PMODE_GPIO_PIN)
#define PMODE_LO        digitalLo(PMODE_GPIO_PORT, PMODE_GPIO_PIN)

#define PDQS_HI         digitalHi(PDQS_GPIO_PORT, PDQS_GPIO_PIN)
#define PDQS_LO         digitalLo(PDQS_GPIO_PORT, PDQS_GPIO_PIN)

#define PCLE_HI         digitalHi(PCLE_GPIO_PORT, PCLE_GPIO_PIN)
#define PCLE_LO         digitalLo(PCLE_GPIO_PORT, PCLE_GPIO_PIN)

#define PALE_HI         digitalHi(PALE_GPIO_PORT, PALE_GPIO_PIN)
#define PALE_LO         digitalLo(PALE_GPIO_PORT, PALE_GPIO_PIN)

#define PDLE_HI         digitalHi(PDLE_GPIO_PORT, PDLE_GPIO_PIN)
#define PDLE_LO         digitalLo(PDLE_GPIO_PORT, PDLE_GPIO_PIN)

#define PDATA_HI        digitalHi(PDATA_GPIO_PORT, PDATA_GPIO_PIN)
#define PDATA_LO        digitalLo(PDATA_GPIO_PORT, PDATA_GPIO_PIN)
#define PDATA_READ      ((PDATA_GPIO_PORT->IDR & PDATA_GPIO_PIN) != 0)

//推挽输出模式10M
#define PDATAOUT()      {GPIOA->CRL &= 0xFFFFFF0F; GPIOA->CRL |= 0x00000010;}

//浮空输入模式
#define PDATAIN()       {GPIOA->CRL &= 0xFFFFFF0F; GPIOA->CRL |= 0x00000040;}

u8   ICP_init(void);
void ICP_deinit(void);
void ICP_read(u16 start_addr,u8 length,u8 *readbuffer);
void ICP_write(u16 start_addr,u8 length,u8 *writebuffer);

#endif





























