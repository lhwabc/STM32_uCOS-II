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

void delay_us(uint16_t us)
{
    uint16_t i;
    while(us--)
    {
        for(i = 0;i<5;i++);
    }
}

void ICP_CMD(u8 cmd)
{
    uint8_t i;
    
    PMODE = 0;
    PALE = 0;
    PDLE = 0;
    PCLE = 1;
    PDQS = 1;
    PDATAOUT();
    for(i=8;i>0;i--)
    {
        PDQS = 0;
        PDATA_O = cmd>>(i-1);
        ICP_Delay();
        PDQS = 1;
        ICP_Delay();
    }	
    PDQS = 1;	
    PCLE = 0;
    ICP_Delay();	
}

void ICP_ADDR(u16 addr)
{
    uint8_t i;
    
    PMODE = 0;
    PALE = 0;
    PCLE = 0;
    PDQS = 1;

    ICP_Delay();
    PALE = 1;
    ICP_Delay();
    PDATAOUT();
    for(i = 16;i>0;i--)
    {
        PDQS = 0;
        PDATA_O = addr>>(i-1);
        ICP_Delay();
        PDQS = 1;
        ICP_Delay();
    }
    PALE = 0; 
}

u8 _ICP_START()
{
    u8 i ;
    u8  temp = 0;
    PCLE = 0;
    PDLE = 0;
    PALE = 0;
    PDQS = 1;
    ICP_Delay(); 
    PCLE = 1;
    PDQS = 0;
    PDATAOUT();
    ICP_Delay();
    PDQS = 1;
    ICP_Delay(); 

    PCLE = 1;
    for(i=8;i>0;i--)  // send 0x5a
    {
        PDQS = 0;
        PDATA_O = start_key1[0]>>(i-1);
        ICP_Delay(); 
        PDQS = 1;
        ICP_Delay(); 
    }
    for(i=8;i>0;i--) //dend 0xe7
    {
        PDQS = 0;
        PDATA_O = start_key1[1]>>(i-1);
        ICP_Delay(); 
        PDQS = 1;
        ICP_Delay(); 
    }

    PDATAIN();
    PCLE = 0;	
    ICP_Delay(); 
    for(i=0;i<8;i++)   // receive 0xa5
    {
        PDQS = 0;
        ICP_Delay();
        PDQS = 1;
        temp <<= 1;
        temp |=  PDATA_I;
        ICP_Delay();
    }

    PCLE = 1;
    PDQS = 1;
    //	ICP_Delay(); 
    PDATAOUT();

    for(i=8;i>0;i--)  // send  
    {
        PDQS = 0;
        PDATA_O = start_key2[0]>>(i-1);
        ICP_Delay(); 
        PDQS = 1;
        ICP_Delay(); 
    }
    for(i=8;i>0;i--) //send  
    {
        PDQS = 0;
        PDATA_O = start_key2[1]>>(i-1);
        ICP_Delay(); 
        PDQS = 1;
        ICP_Delay(); 
    }
    PCLE = 0;
    if(temp!=0xa5)
    {
        ICP_deinit();
        return 0xa5;
    }
    else	return 0;
}

u8 ICP_init(void)
{
    RCC->APB2ENR|=1<<2;             //使能PORTA时钟
    RCC->APB2ENR|=1<<3;             //使能PORTB时钟
    RCC->APB2ENR|=1<<4;             //使能PORTC时钟

    GPIOC->CRH &= 0x0FFFFFFF;       //PC15 推挽输出  最大速度10MHz
    GPIOC->CRH |= 0x10000000;
    GPIOC->ODR &=~(1<<15);          //PC15 输出低//NRST = 0;
    
    GPIOC->CRL &= 0xFFFF0000;       //PC0, PC1, PC2, PC3 推挽输出  最大速度10MHz
    GPIOC->CRL |= 0x00001111;
    
    GPIOA->CRL &= 0xFFFFFF00;       //PA0, PA1 推挽输出  最大速度10MHz
    GPIOA->CRL |= 0x00000011;
    
    GPIOB->CRH &= 0X0FFFFFFF;
    GPIOB->CRH |= 0X10000000;       //PB15 推挽输出  最大速度10MHz
    GPIOB->ODR &=~(1<<15);          //PB15 输出低

    NRST = 0;
    VPP = 0;
    PCLE = 0;
    PALE = 0;
    PDQS = 1;
    PMODE  = 0;
    PDLE  = 0;
    ICP_Delay();
    
    return _ICP_START();
}

void ICP_deinit(void)
{
    GPIOC->CRL = 0x0;
    GPIOC->CRH = 0x0;
    GPIOA->CRL = 0x0;
    NRST = 1;
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
        PDQS = 1;               // 硬件读数据阶段
        PMODE  = 1;
        PALE = 0;
        PCLE =0;
        ICP_Delay();
        PDQS = 0;
        ICP_Delay();
        PDQS = 1;
        ICP_Delay();
        PALE = 1;
        PDLE = 1;
        *readbuffer = 0;
        for(j = 0;j<8;j++)      // 数据传送阶段
        {
            PDQS = 0;
            ICP_Delay();
            *readbuffer <<=1;
            PDQS = 1;
            *readbuffer |= PDATA_I;
            ICP_Delay();
        }
        PDLE = 0;
        PALE = 0;
        PMODE  = 0;
        readbuffer++;
        ICP_Delay();
        PCLE = 1;
        PALE = 1;
        ICP_Delay();
        PDQS = 0;               // 地址加1 阶段
        ICP_Delay();
        PDQS = 1;
        ICP_Delay();
    }
    PDQS = 1;
    PMODE  = 0;
    PCLE  = 0;
    PALE  = 0;
    ICP_CMD(0xff);
}

void ICP_write(u16 start_addr,u8 length,u8 *writebuffer)
{
    u8 i,j;
    
    VPP =1;                     // 打开高压控制
    delay_us(20);
    PDATAOUT();
    ICP_CMD(0xF0);
    ICP_CMD(0x02);              // PTM = 010 , Program mode
    ICP_ADDR(start_addr);       // PTM = 0 , read mode
    
    delay_us(4);
    for(i=0;i<length;i++)
    {
        PDQS = 1;
        PMODE  = 0;
        PCLE  = 1;
        PALE  = 0;
        PDLE  = 1;
        for(j=8;j>0;j--)        // 数据发送阶段
        {
            PDQS = 0;
            PDATA_O = (*writebuffer)>>(j-1);
            ICP_Delay();
            PDQS = 1;
            ICP_Delay();
        }	  
        writebuffer++;
        PDLE  = 0;
        PCLE = 0;
        PMODE  = 1; 
        PDQS = 1;
        ICP_Delay();
        PDQS = 0;
        delay_us(102);          //  写阶段
        PDQS = 1;
        delay_us(3);
        
        PMODE = 0;
        PALE = 1;
        PCLE = 1;
        PDQS = 1;
        ICP_Delay();
        PDQS = 0;               // 地址加1 阶段
        ICP_Delay();
    }	
    PDQS = 1;
    PMODE  = 0;
    PCLE  = 0;
    PALE  = 0; 
    VPP = 0;                    // 关闭高压控制
    ICP_CMD(0xff); 
}

