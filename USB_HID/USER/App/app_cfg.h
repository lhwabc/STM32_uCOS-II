#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************设置任务优先级*******************/
#define  APP_CFG_TASK_START_PRIO            3
#define  USB_TASK_PRIO                      6 
#define  ODL_TASK_PRIO                      9
#define  KEY_TASK_PRIO                      8
#define  LED_TASK_PRIO                      7


/************设置任务堆栈大小(单位为 OS_STK )************/
#define  APP_CFG_TASK_START_STK_SIZE        128
#define  USB_STK_SIZE                        64
#define  ODL_STK_SIZE                        80
#define  KEY_STK_SIZE                        64
#define  LED_STK_SIZE                        64


#endif
