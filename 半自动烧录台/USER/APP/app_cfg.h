#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************设置任务优先级*******************/
#define STARTUP_TASK_PRIO       3      
#define USB_TASK_PRIO           5

/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE   80   
#define USB_STK_SIZE            80
#define TASK_LED3_STK_SIZE      80

#endif

