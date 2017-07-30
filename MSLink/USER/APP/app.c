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

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //����ջ
static OS_STK KEY_TASK_STK[TASK_KEY_STK_SIZE];

int main(void)
{
  	BSP_Init();
	OSInit();
	OSTaskCreate(Task_Start,(void *)0, &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);
    
	OSStart();
    return 0;
 }


 void usb_port_set(u8 enable)
{
    RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��
    GPIOE->CRL&=0XFFFF0FFF;
    GPIOE->CRL|=0X00003000;
    GPIOE->ODR |= (1<<3);          //PEout(3)=1;
    
    RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��       
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
    else
    {     
        _SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        GPIOA->ODR &=~(1<<12);          //PAout(12)=0;
    }
} 
 
void Task_Start(void *p_arg)
{
    (void)p_arg;
    
    OSTimeDlyHMSM(0, 0,0,10);
    usb_port_set(0);    //USB�ȶϿ�
    OSTimeDlyHMSM(0, 0,0,30);
    usb_port_set(1);    //USB�ٴ�����
    
     //USB����
    USB_Interrupts_Config();    
    Set_USBClock();   
    USB_Init(); 
    
    OSTaskCreate(Task_key,(void *)0,(OS_STK*)&KEY_TASK_STK[TASK_KEY_STK_SIZE-1],TASK_KEY_PRIO);
    while (1)
    {
        //LED_RED(ON);
        OSTimeDlyHMSM(0, 0,0,500);
        //LED_RED( OFF);   
        OSTimeDlyHMSM(0, 0,0,500);     
    }
}

void Task_LED(void *p_arg)
{
    (void)p_arg;
    
    while (1)
    {
        LED_RED(ON);
        OSTimeDlyHMSM(0, 0,0,100);
        LED_RED(OFF);
    }
}

void Task_key(void *p_arg)
{
    (void)p_arg;
    
    while (1)
    {
        LED_GREEN(ON);
        OSTimeDlyHMSM(0, 0,0,1000);
        LED_GREEN(OFF);
        OSTimeDlyHMSM(0, 0,0,1000);
    }
}
