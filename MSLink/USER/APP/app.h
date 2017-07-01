/**
  ******************************************************************************
  * @file    app.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    01-July-2017
  * @brief   This file defines the app interface driver function.  
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef	__APP_H
#define	__APP_H

/**************** 用户任务声明 *******************/
void Task_Start(void *p_arg);
void Task_LED(void *p_arg);
void Task_key(void *p_arg);

#endif
