/**
  ******************************************************************************
  * @file    app.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file is app source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "includes.h"


//任务堆栈

//设置USB 连接/断线
//enable:0,断开
//       1,允许连接    
void usb_port_set(u8 enable)
{
    RCC->APB2ENR|=1<<2;    //使能PORTA时钟       
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
    else
    {     
        _SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        PAout(12)=0;
    }
} 

int main(void)
{    
    SystemInit();
    
    delay_init(48);     //延时初始化
    uart_init(48,115200); //串口1初始化      
    delay_ms(10);
    
    usb_port_set(0);    //USB先断开
    delay_ms(30);
    usb_port_set(1);    //USB再次连接
    //USB配置
    USB_Interrupts_Config();    
    Set_USBClock();   
    USB_Init();    
    while(1)
    {
        delay_ms(1000);
    }
    
}

