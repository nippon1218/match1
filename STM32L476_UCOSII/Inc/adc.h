#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#include "stm32l4xx_hal.h"



void MX_ADC3_Init(void);
void _Error_Handler(char *, int);


u16  Get_Adc(u32 ch); 
u16 Get_Adc_Average(u32 ch,u8 times);
#endif 

