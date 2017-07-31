/**
  ******************************************************************************
  * @file    OLP.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file defines the Offline program interface driver function.  
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.   
***********************************************************/
#ifndef __OLP_H__
#define __OLP_H__
#include "includes.h"

//用户根据自己的需要设置
#define STM32_FLASH_SIZE                256                 //所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN                1                   //使能FLASH写入(0，不使能; 1，使能)

//Offline frimware 保存地址
#define STM32_FLASH_BASE                0x08000000          //STM32 FLASH的起始地址
#define STM32_FLASH_DATA_BASE           0x08008000          //STM32 FLASH的起始地址
#define STM32_FLASH_DATA_SIZE           0x00008000          //STM32 FLASH的程序操作大小

#define FLASH_ADDR_MSLink_KEY           0x08007C00          
#define MSLink_KEY                      0x8005 
#define FLASH_ADDR_ICP_MODE             0x08007C04          //离线编程模式配置
#define FLASH_ADDR_ICP_MODE_BIT         0x0001              //离线编程模式配置位

void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);
void clear_memory(void);

uint16_t get_firmware_length(void);
uint8_t  get_used_block(void);
uint8_t  get_offset_block(void);

//0x00:Disable MTP; 0x01: enable MTP
uint8_t Get_ICP_mode(void);
void    Set_ICP_mode(uint8_t u8_mtp);

#endif
