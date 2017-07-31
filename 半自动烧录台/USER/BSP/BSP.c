#include "includes.h"

/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
    SystemInit();
    SysTick_init();
    Signal_GPIO_Config();
}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
    SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
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


