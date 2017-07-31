/**
  ******************************************************************************
  * @file    app.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    31-July-2017
  * @brief   This file is MSLink for machine app source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "includes.h"

#define     ICP_INIT        0x01
#define     ICP_DEINIT      0x02
#define     ICP_START       0x03
#define     ICP_DONE        0x04
#define     ICP_ERROR       0x05

u8 usedBlock[] = 
{
    0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 
    0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D,
    0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D,
    0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D
};

u8 offsetBlock[] = 
{
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57,
    0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57
};

static  OS_STK          STARTUP_TASK_STK[STARTUP_TASK_STK_SIZE];       //定义栈
static  OS_STK          USB_TASK_STK[USB_STK_SIZE];
static  OS_STK          OLP_TASK_STK[OLP_STK_SIZE];
int main(void)
{
    BSP_Init();
    OSInit();
    OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                    */
                    (void           *) 0,
                    (OS_STK         *)&STARTUP_TASK_STK[STARTUP_TASK_STK_SIZE - 1],
                    (INT8U           ) STARTUP_TASK_PRIO,
                    (INT16U          ) STARTUP_TASK_PRIO,
                    (OS_STK         *)&STARTUP_TASK_STK[0],
                    (INT32U          ) STARTUP_TASK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSStart();
    return 0;
 }


void App_TaskStart(void *p_arg)
{
    (void)p_arg;                                // 'p_arg' 并没有用到，防止编译器提示警告

    OSTaskCreate(App_usb_task,(void *)0, &USB_TASK_STK[USB_STK_SIZE-1], USB_TASK_PRIO);
    OSTaskCreate(offline_download_task,(void *)0, &OLP_TASK_STK[OLP_STK_SIZE-1], OLP_TASK_PRIO);
    OSTaskSuspend(STARTUP_TASK_PRIO);
    while (1)
    {
        OSTimeDlyHMSM(0, 0,0,100); 
    }
}

void App_usb_task(void *p_arg)
{
    (void)p_arg;
    
    OSTimeDlyHMSM(0, 0,0,10);
    usb_port_set(0);
    OSTimeDlyHMSM(0, 0,0,30);
    usb_port_set(1);
    
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
        
    while(1)
    {
        OSTimeDlyHMSM(0, 0,0,5000);
    }
}

#define ICP_BLOCK_SIZE   60     //master less than 255
void offline_download_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
    
    u8  u8_used_block = 0;
    u8  u8_offset_block = 0;
    u16 i,length = 0;
    u16 u16_blockCount = 0;
    u8  u8_ICPFlag = ICP_DEINIT;
    
    u16 u16_readBuffer[ICP_BLOCK_SIZE],u16_checkBuffer;
    u8  u8_codeBuffer[2*ICP_BLOCK_SIZE];
    u32 u32_flash_start_address = 0;
    
    (void)pdata;
    
    while(1)
    {
        OSTimeDlyHMSM(0, 0,0,25);
        if(START_A_Scan())
        {
            BusySignal_A(EN);
            OKSignal_A(DIS);
            NGSignal_A(DIS);
            OS_ENTER_CRITICAL();            //进入临界区(无法被中断打断)
            if(ICP_init() != 0)             // 进入ICP失败
            {
                u8_ICPFlag = ICP_ERROR;
                //printf("ICP connect Fail!\r\n");
            }
            else
            {
                u8_ICPFlag = ICP_DEINIT;
                //printf("ICP Connected!!!\r\n");
                length = get_firmware_length();
                if(Get_ICP_mode() == 0x01)//MTP
                {
                    u8_used_block = get_used_block();
                    //printf("used block = %d \r\n", u8_used_block);
                    
                    u8_offset_block = get_offset_block();
                    //printf("offset block = %d \r\n", u8_offset_block);
                    
                    u32_flash_start_address = u8_used_block*1024;
                    //printf("flash start address = 0x%X \r\n", u32_flash_start_address);
                                
                    if(0x7F80 < length + u32_flash_start_address)
                    {
                        u8_ICPFlag = ICP_ERROR;
                        //printf("MCU: Insufficient code space\r\n");
                    }
                    else
                    {
                        u8_offset_block = u8_used_block;
                        u8_used_block = u8_used_block + length/1024+1;
                    }
                }
                
                //printf("Firmware length: %d (0x%X) \r\n", length, length);
                
                u16_blockCount = 0;
                while((length >= ICP_BLOCK_SIZE)&&(u8_ICPFlag != ICP_ERROR))
                {
                    STMFLASH_Read((STM32_FLASH_DATA_BASE + (u16_blockCount*ICP_BLOCK_SIZE)), u16_readBuffer, ICP_BLOCK_SIZE);
                    for(i = 0; i<ICP_BLOCK_SIZE; i+=1)
                    {
                        u8_codeBuffer[i*2]   = (u8)(u16_readBuffer[i]);
                        u8_codeBuffer[i*2+1] = (u8)(u16_readBuffer[i]>>8);
                    }

                    ICP_write(u32_flash_start_address+(u16_blockCount*ICP_BLOCK_SIZE),(u8)(ICP_BLOCK_SIZE*2), u8_codeBuffer);
                    ICP_read(u32_flash_start_address+(u16_blockCount*ICP_BLOCK_SIZE),(u8)(ICP_BLOCK_SIZE*2),u8_codeBuffer);
                    if(u16_blockCount > 10)
                    {
                        //LED_RED_TOGGLE;
                        //LED_GREEN_TOGGLE;
                    }
                    for(i = 0; i<ICP_BLOCK_SIZE; i+=1)
                    {
                        u16_checkBuffer = u8_codeBuffer[i*2];
                        u16_checkBuffer |= (u8_codeBuffer[i*2+1]<<8);
                        if(u16_checkBuffer != u16_readBuffer[i])
                        {
                            u8_ICPFlag = ICP_ERROR;
                            i = ICP_BLOCK_SIZE;
                            
                            //printf("Error!!!\r\n");
                        }
                    }
                    
                    length = length - ICP_BLOCK_SIZE;
                    u16_blockCount ++;
                }

                if((length != 0)&&(u8_ICPFlag != ICP_ERROR))
                {
                    STMFLASH_Read((STM32_FLASH_DATA_BASE + (u16_blockCount*ICP_BLOCK_SIZE)), u16_readBuffer, length);
                    for(i = 0; i<length; i+=1)
                    {
                        u8_codeBuffer[i*2]   = (u8)(u16_readBuffer[i]);
                        u8_codeBuffer[i*2+1] = (u8)(u16_readBuffer[i]>>8);
                    }
                    ICP_write(u32_flash_start_address+(u16_blockCount*ICP_BLOCK_SIZE),(u8)(length*2), u8_codeBuffer);
                    ICP_read(u32_flash_start_address+(u16_blockCount*ICP_BLOCK_SIZE),(u8)(length*2),u8_codeBuffer);
                    if(u16_blockCount > 10)
                    {
                        //LED_RED_TOGGLE;
                        //LED_GREEN_TOGGLE;
                    }
                    for(i = 0; i<length; i+=1)
                    {
                        u16_checkBuffer = u8_codeBuffer[i*2];
                        u16_checkBuffer |= (u8_codeBuffer[i*2+1]<<8);
                        if(u16_checkBuffer != u16_readBuffer[i])
                        {
                            u8_ICPFlag = ICP_ERROR;
                            i = length;
                            //printf("Error!!! \r\n");
                        }
                    }
                }
                if(Get_ICP_mode() == 0x01)
                {
                    if(u8_ICPFlag != ICP_ERROR)
                    {
                        ICP_write(0x7FA0, u8_used_block, usedBlock);
                        ICP_write(0x7FC0, u8_offset_block, offsetBlock);
                    }
                }
                ICP_deinit();
                //printf("ICP deinit!!!\r\n");
            }
            OS_EXIT_CRITICAL();           //退出临界区(可以被中断打断)
            BusySignal_A(DIS);
            if(u8_ICPFlag == ICP_ERROR)
            {
                OKSignal_A(DIS);
                NGSignal_A(EN);
            }
            else
            {
                OKSignal_A(EN);
                NGSignal_A(DIS);
            }
        }
    }
}
