#include "includes.h"

/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
    SystemInit();
    SysTick_init();
    Signal_GPIO_Config();
}

/*
 * 函数名：SysTick_init
 * 描述  ：配置SysTick定时器
 * 输入  ：无
 * 输出  ：无
 */
void SysTick_init(void)
{
    SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//初始化并使能SysTick定时器
}

void usb_port_set(u8 enable)
{
    RCC->APB2ENR|=1<<2;
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));
    else
    {     
        _SetCNTR(_GetCNTR()|(1<<1));
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        GPIOA->ODR &=~(1<<12);
    }
}


