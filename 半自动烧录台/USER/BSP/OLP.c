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
	    WriteAddr+=2;//��ַ����2.
	}
}

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else
#define STM_SECTOR_SIZE	2048
#endif

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(uint32_t WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	uint32_t secpos;	   //������ַ
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)

	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr < STM32_FLASH_BASE||(WriteAddr >= (STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();                         //����
	offaddr=WriteAddr-STM32_FLASH_BASE;     //ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;         //������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;     //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;     //����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}
	};
	FLASH_Lock();//����
}

//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

void clear_memory(void)
{
	uint8_t i ;
	FLASH_Unlock();						//����
	for(i = 0;i<(STM32_FLASH_DATA_SIZE/STM_SECTOR_SIZE);i++)
	{
		if(FLASH_ErasePage(i*STM_SECTOR_SIZE+STM32_FLASH_DATA_BASE))//����32K ��Ҫʹ�õ�����
		{
			//printf("����block%2dʧ��",i);
		}
	}	
	FLASH_Lock();//����
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
    
    FLASH_Unlock();                              //����
    if(FLASH_ErasePage(FLASH_ADDR_MSLink_KEY))   //����32K ��Ҫʹ�õ�����
    {
        //printf("FLASH_ADDR_MSLink_KEY ErasePage error!");
    }
    FLASH_Lock();//����
    
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

