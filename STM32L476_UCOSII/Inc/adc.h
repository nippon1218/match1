#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"

void MY_ADC_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u32 ch); 		        //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ
#endif 

