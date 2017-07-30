#include "includes.h"

OS_STK task_led2_stk[TASK_LED2_STK_SIZE];		  //定义栈
OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //定义栈
  
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
    (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告

	OSTaskCreate(Task_LED2,(void *)0,		  	//创建任务2
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

    while (1)
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,100);
        LED1( OFF);   
		OSTimeDlyHMSM(0, 0,0,100);     
    }
}

//任务2
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
