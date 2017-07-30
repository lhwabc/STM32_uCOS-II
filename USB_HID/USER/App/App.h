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
static  void  App_TaskStart (void *p_arg);
void usb_task(void *pdata);
void key_task(void *pdata);
void led_task(void *pdata);
void offline_download_task(void *pdata);


/**************** help *******************/
u16 get_firmware_length(void);
u8  get_used_block(void);
u8  get_offset_block(void);

#endif
