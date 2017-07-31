#ifndef __BSP_H
#define __BSP_H

void SysTick_init(void);
void BSP_Init(void);

void usb_port_set(u8 enable);

#endif // __BSP_H
