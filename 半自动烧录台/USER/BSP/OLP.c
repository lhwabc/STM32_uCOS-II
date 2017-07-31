/**
  ******************************************************************************
  * @file    OLP.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is Offline program source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "OLP.h"

void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}
}

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else
#define STM_SECTOR_SIZE	2048
#endif

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(uint32_t WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	uint32_t secpos;	   //扇区地址
	uint16_t secoff;	   //扇区内偏移地址(16位字计算)
	uint16_t secremain; //扇区内剩余地址(16位字计算)

	uint32_t offaddr;   //去掉0X08000000后的地址
	if(WriteAddr < STM32_FLASH_BASE||(WriteAddr >= (STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();                         //解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;     //实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;         //扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;     //在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;     //扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}
	};
	FLASH_Lock();//上锁
}

//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

void clear_memory(void)
{
	uint8_t i ;
	FLASH_Unlock();						//解锁
	for(i = 0;i<(STM32_FLASH_DATA_SIZE/STM_SECTOR_SIZE);i++)
	{
		if(FLASH_ErasePage(i*STM_SECTOR_SIZE+STM32_FLASH_DATA_BASE))//擦除32K 需要使用的区域
		{
			//printf("擦除block%2d失败",i);
		}
	}	
	FLASH_Lock();//上锁
}

//for mtp
uint16_t get_firmware_length(void)
{
    uint16_t i,length = 0;
    
    for(i = (STM32_FLASH_DATA_SIZE - 128); i > 0; i -= 2)
    {
        if(STMFLASH_ReadHalfWord(STM32_FLASH_DATA_BASE+i-2) != 0xffff)
        {
            length = i;
            break;
        }
    }
    
    return length;
}

uint8_t get_used_block(void)
{
    uint8_t i, u8_codeBuffer;
    for(i=32;i>0;i--)
    {
        ICP_read(0x7F9F+i, 1, &u8_codeBuffer);
        if((u8_codeBuffer != 0x00)&&(u8_codeBuffer != 0xFF ))
        {
            break;
        }
    }
    return i;
}

uint8_t get_offset_block(void)
{
    uint8_t i, u8_codeBuffer;
    for(i=32;i>0;i--)
    {
        ICP_read(0x7FBF+i, 1, &u8_codeBuffer);
        if((u8_codeBuffer != 0x00)&&(u8_codeBuffer != 0xFF ))
        {
            break;
        }
    }
    return i;
}

uint8_t  Get_ICP_mode(void)
{
    uint16_t u16_readBuffer;
    STMFLASH_Read(FLASH_ADDR_MSLink_KEY, &u16_readBuffer, 1);
    if(u16_readBuffer == MSLink_KEY)
    {
        STMFLASH_Read(FLASH_ADDR_ICP_MODE, &u16_readBuffer, 1);
        if((u16_readBuffer & FLASH_ADDR_ICP_MODE_BIT) == FLASH_ADDR_ICP_MODE_BIT)
        {
            return 0x01;    //MTP
        }
    }
    return 0x00;        //OTP
}

void  Set_ICP_mode(uint8_t u8_mtp)
{
    uint16_t u16_writeBuffer;
    
    FLASH_Unlock();                              //解锁
    if(FLASH_ErasePage(FLASH_ADDR_MSLink_KEY))   //擦除32K 需要使用的区域
    {
        //printf("FLASH_ADDR_MSLink_KEY ErasePage error!");
    }
    FLASH_Lock();//上锁
    
    u16_writeBuffer = MSLink_KEY;
    STMFLASH_Write(FLASH_ADDR_MSLink_KEY, &u16_writeBuffer,1);
    if(u8_mtp == 0x01)
    {
        u16_writeBuffer = 0x0001;
    }
    else
    {
        u16_writeBuffer = 0x0000;
    }
    STMFLASH_Write(FLASH_ADDR_ICP_MODE, &u16_writeBuffer, 1);
}

