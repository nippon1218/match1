#ifndef _DELAY_H
#define _DELAY_H
#include <sys.h>	  

void delay_init(u8 SYSCLK);   //��ʼ������
void delay_ms(u16 nms);       //��ʱms����
void delay_us(u32 nus);       //��ʱus����
#endif

