/**
  ******************************************************************************
  * @file    app.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file defines the app interface driver function.  
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef _APP_H_
#define _APP_H_

/**************** 用户任务声明 *******************/
void App_TaskStart(void *p_arg);
void App_usb_task(void *p_arg);

#endif  //_APP_H_
