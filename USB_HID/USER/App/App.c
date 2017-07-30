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


//�����ջ

//����USB ����/����
//enable:0,�Ͽ�
//       1,��������    
void usb_port_set(u8 enable)
{
    RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��       
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
    else
    {     
        _SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        PAout(12)=0;
    }
} 

int main(void)
{    
    SystemInit();
    
    delay_init(48);     //��ʱ��ʼ��
    uart_init(48,115200); //����1��ʼ��      
    delay_ms(10);
    
    usb_port_set(0);    //USB�ȶϿ�
    delay_ms(30);
    usb_port_set(1);    //USB�ٴ�����
    //USB����
    USB_Interrupts_Config();    
    Set_USBClock();   
    USB_Init();    
    while(1)
    {
        delay_ms(1000);
    }
    
}

