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

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //∂®“Â’ª
static OS_STK KEY_TASK_STK[TASK_KEY_STK_SIZE];

int main(void)
{
  	BSP_Init();
	OSInit();
	OSTaskCreate(Task_Start,(void *)0, &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);
    OSTaskCreate(Task_key,(void *)0,(OS_STK*)&KEY_TASK_STK[TASK_KEY_STK_SIZE-1],TASK_KEY_PRIO);
	OSStart();
    return 0;
 }


void Task_Start(void *p_arg)
{
    (void)p_arg;

    
    while (1)
    {
        LED_RED(ON);
        OSTimeDlyHMSM(0, 0,0,100);
        LED_RED( OFF);   
        OSTimeDlyHMSM(0, 0,0,100);     
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
        OSTimeDlyHMSM(0, 0,0,300);
        LED_GREEN(OFF);
        OSTimeDlyHMSM(0, 0,0,300);
    }
}
