#include "includes.h"

OS_STK task_led2_stk[TASK_LED2_STK_SIZE];		  //����ջ
OS_STK task_led3_stk[TASK_LED3_STK_SIZE];		  //����ջ
OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //����ջ
  
int main(void)
{
  	BSP_Init();
	OSInit();
	OSTaskCreate(Task_Start,(void *)0,
	   &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSStart();
    return 0;
 }


void Task_Start(void *p_arg)
{
    (void)p_arg;                				// 'p_arg' ��û���õ�����ֹ��������ʾ����

	OSTaskCreate(Task_LED2,(void *)0,		  	//��������2
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

	OSTaskCreate(Task_LED3,(void *)0,		   	//��������3
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);

    while (1)
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED1( OFF);   
		OSTimeDlyHMSM(0, 0,0,100);     
    }
}

//����2
void Task_LED2(void *p_arg)
{
    (void)p_arg;                	
	
    while (1)
    {
        LED2( ON );
        OSTimeDlyHMSM(0, 0,0,200);
        LED2( OFF);
		OSTimeDlyHMSM(0, 0,0,200);	
    }
}

//����3
void Task_LED3(void *p_arg)
{
    (void)p_arg;      
	
    while (1)
    {
        LED3( ON );
        OSTimeDlyHMSM(0, 0,0,300);
        LED3( OFF);
		OSTimeDlyHMSM(0, 0,0,300);        
    }
}
