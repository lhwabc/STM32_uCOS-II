/**
  ******************************************************************************
  * @file    ICP.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is ICP source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "ICP.h"

#define ICP_Delay   delay_25ns

const u8 start_key1[8] = {0x5a,0xe7,0xa5,0x96,0x3c,0x7e,0xc3,0xe7};
const u8 start_key2[2] = {0x5a,0x3c};

void delay_25ns()
{
    uint16_t i;
    
    for(i = 0;i<5;i++);
}

void ICP_CMD(u8 cmd)
{
    uint8_t i;
    
    PMODE_LO;
    PALE_LO;
    PDLE_LO;
    PCLE_HI;
    PDQS_HI;
    PDATAOUT();
    for(i=8;i>0;i--)
    {
        PDQS_LO;
        if((cmd>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay();
        PDQS_HI;
        ICP_Delay();
    }   
    PDQS_HI;   
    PCLE_LO;
    ICP_Delay();    
}

void ICP_ADDR(u16 addr)
{
    uint8_t i;
    
    PMODE_LO;
    PALE_LO;
    PCLE_LO;
    PDQS_HI;

    ICP_Delay();
    PALE_HI;
    ICP_Delay();
    PDATAOUT();
    for(i=16;i>0;i--)
    {
        PDQS_LO;
        if((addr>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay();
        PDQS_HI;
        ICP_Delay();
    }
    PALE_LO; 
}

u8 _ICP_START()
{
    u8  i ;
    u8  temp = 0;
    PCLE_LO;
    PDLE_LO;
    PALE_LO;
    PDQS_HI;
    ICP_Delay(); 
    PCLE_HI;
    PDQS_LO;
    PDATAOUT();
    ICP_Delay();
    PDQS_HI;
    ICP_Delay(); 

    PCLE_HI;
    for(i=8;i>0;i--)  // send 0x5a
    {
        PDQS_LO;
        if((start_key1[0]>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay(); 
        PDQS_HI;
        ICP_Delay(); 
    }
    for(i=8;i>0;i--) //dend 0xe7
    {
        PDQS_LO;
        if((start_key1[1]>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay(); 
        PDQS_HI;
        ICP_Delay(); 
    }

    PDATAIN();
    PCLE_LO;   
    ICP_Delay(); 
    for(i=0;i<8;i++)   // receive 0xa5
    {
        PDQS_LO;
        ICP_Delay();
        PDQS_HI;
        temp <<= 1;
        temp |=  PDATA_READ;
        ICP_Delay();
    }

    PCLE_HI;
    PDQS_HI;
    //  ICP_Delay(); 
    PDATAOUT();

    for(i=8;i>0;i--)  // send  
    {
        PDQS_LO;
        if((start_key2[0]>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay(); 
        PDQS_HI;
        ICP_Delay(); 
    }
    for(i=8;i>0;i--) //send  
    {
        PDQS_LO;
        if((start_key2[1]>>(i-1))&0x01)
        {
            PDATA_HI;
        }
        else
        {
            PDATA_LO;
        }
        ICP_Delay(); 
        PDQS_HI;
        ICP_Delay(); 
    }
    PCLE_LO;
    if(temp!=0xa5)
    {
        ICP_deinit();
        return 0xa5;
    }
    else    return 0;
}

u8 ICP_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( NRST_GPIO_CLK | \
                            VPP_GPIO_CLK | \
                            PMODE_GPIO_CLK | \
                            PDQS_GPIO_CLK | \
                            PCLE_GPIO_CLK | \
                            PALE_GPIO_CLK | \
                            PDLE_GPIO_CLK | \
                            PDATA_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = NRST_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRST_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = VPP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VPP_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PMODE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PMODE_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PDQS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PDQS_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PCLE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PCLE_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PALE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PALE_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PDLE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PDLE_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PDATA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PDATA_GPIO_PORT, &GPIO_InitStructure);

    NRST_LO;
    VPP_LO;
    PCLE_LO;
    PALE_LO;
    PDQS_HI;
    PMODE_LO;
    PDLE_LO;
    ICP_Delay();
    
    return _ICP_START();
}

void ICP_deinit(void)
{
    GPIOC->CRL = 0x0;
    GPIOC->CRH = 0x0;
    GPIOA->CRL = 0x0;
    NRST_HI;
}

void ICP_read(u16 start_addr,u8 length,u8 *readbuffer)
{
    u8 i,j;
    ICP_CMD(0xF0);
    ICP_CMD(0x00);              // PTM = 0 , read mode
    ICP_ADDR(start_addr);       // PTM = 0 , read mode
    PDATAIN();
    ICP_Delay();
    for(i=0;i<length;i++)
    {
        PDQS_HI;               // 硬件读数据阶段
        PMODE_HI;
        PALE_LO;
        PCLE_LO;
        ICP_Delay();
        PDQS_LO;
        ICP_Delay();
        PDQS_HI;
        ICP_Delay();
        PALE_HI;
        PDLE_HI;
        *readbuffer = 0;
        for(j = 0;j<8;j++)      // 数据传送阶段
        {
            PDQS_LO;
            ICP_Delay();
            *readbuffer <<=1;
            PDQS_HI;
            *readbuffer |= PDATA_READ;
            ICP_Delay();
        }
        PDLE_LO;
        PALE_LO;
        PMODE_LO;
        readbuffer++;
        ICP_Delay();
        PCLE_HI;
        PALE_HI;
        ICP_Delay();
        PDQS_LO;               // 地址加1 阶段
        ICP_Delay();
        PDQS_HI;
        ICP_Delay();
    }
    PDQS_HI;
    PMODE_LO;
    PCLE_LO;
    PALE_LO;
    ICP_CMD(0xff);
}

void ICP_write(u16 start_addr,u8 length,u8 *writebuffer)
{
    u8 i,j;
    
    VPP_HI;                     // 打开高压控制
    OSTimeDly(20);
    PDATAOUT();
    ICP_CMD(0xF0);
    ICP_CMD(0x02);              // PTM = 010 , Program mode
    ICP_ADDR(start_addr);       // PTM = 0 , read mode
    
    //delay_us(4);
    for(i=0;i<length;i++)
    {
        PDQS_HI;
        PMODE_LO;
        PCLE_HI;
        PALE_LO;
        PDLE_HI;
        for(j=8;j>0;j--)        // 数据发送阶段
        {
            PDQS_LO;
            if(((*writebuffer)>>(j-1))&0x01)
            {
                PDATA_HI;
            }
            else
            {
                PDATA_LO;
            }
            ICP_Delay();
            PDQS_HI;
            ICP_Delay();
        }     
        writebuffer++;
        PDLE_LO;
        PCLE_LO;
        PMODE_HI; 
        PDQS_HI;
        ICP_Delay();
        PDQS_LO;
        //delay_us(102);          //  写阶段
        PDQS_HI;
        //delay_us(3);
        
        PMODE_LO;
        PALE_HI;
        PCLE_HI;
        PDQS_HI;
        ICP_Delay();
        PDQS_LO;               // 地址加1 阶段
        ICP_Delay();
    }   
    PDQS_HI;
    PMODE_LO;
    PCLE_LO;
    PALE_LO; 
    VPP_LO;                    // 关闭高压控制
    ICP_CMD(0xff); 
}

