/**
  ******************************************************************************
  * @file    app.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is MSLink for machine app source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "includes.h"


static  OS_STK          STARTUP_TASK_STK[STARTUP_TASK_STK_SIZE];       //定义栈
static  OS_STK          USB_TASK_STK[USB_STK_SIZE]; 
int main(void)
{
    BSP_Init();
    OSInit();
    OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                    */
                    (void           *) 0,
                    (OS_STK         *)&STARTUP_TASK_STK[STARTUP_TASK_STK_SIZE - 1],
                    (INT8U           ) STARTUP_TASK_PRIO,
                    (INT16U          ) STARTUP_TASK_PRIO,
                    (OS_STK         *)&STARTUP_TASK_STK[0],
                    (INT32U          ) STARTUP_TASK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();
    return 0;
 }


void App_TaskStart(void *p_arg)
{
    (void)p_arg;                                // 'p_arg' 并没有用到，防止编译器提示警告

    OSTaskCreate(App_usb_task,(void *)0, &USB_TASK_STK[USB_STK_SIZE-1], USB_TASK_PRIO);

    while (1)
    {
        
        OSTimeDlyHMSM(0, 0,0,100);
          
        OSTimeDlyHMSM(0, 0,0,100);     
    }
}

void App_usb_task(void *p_arg)
{
    (void)p_arg;                    
    OSTimeDlyHMSM(0, 0,0,10);
    usb_port_set(0);
    OSTimeDlyHMSM(0, 0,0,30);
    usb_port_set(1);
    
    USB_Interrupts_Config();    
    Set_USBClock();
    USB_Init();    
    while(1)
    {
        OSTimeDlyHMSM(0, 0,0,1000);
    }
}
