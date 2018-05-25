#include "ucosiitask.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
/* USER CODE BEGIN Includes */
#include "includes.h"
#include "24cxx.h"
#include "tmp006.h"



#define LedToggle HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

const u8 TEXT_Buffer[]={"sony*****IICTest"};
const u8 data1=11;
#define SIZE sizeof(TEXT_Buffer)


//�����ջ
__align(4) OS_STK	START_TASK_STK[START_STK_SIZE];
__align(4) OS_STK LED0_TASK_STK[LED0_STK_SIZE];
__align(8) OS_STK float_TASK_STK[float_STK_SIZE];
__align(8) OS_STK japan_TASK_STK[japan_STK_SIZE];
__align(8) OS_STK hello_TASK_STK[hello_STK_SIZE];
__align(8) OS_STK tmp_TASK_STK[tmp_STK_SIZE];

void start_task(void *pdata){
	OS_CPU_SR cpu_sr;
	OSStatInit();  //����ͳ������
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO); 		//LED0����
//	OSTaskCreate(float_task,(void*)0,(OS_STK*)&float_TASK_STK[float_STK_SIZE-1],float_TASK_PRIO); 		//LED1����
//	OSTaskCreate(japan_task,(void*)0,(OS_STK*)&japan_TASK_STK[japan_STK_SIZE-1],japan_TASK_PRIO); 
//	OSTaskCreate(hello_task,(void*)0,(OS_STK*)&hello_TASK_STK[hello_STK_SIZE-1],hello_TASK_PRIO); 
	OSTaskCreate(tmp_task,(void*)0,(OS_STK*)&tmp_TASK_STK[tmp_STK_SIZE-1],tmp_TASK_PRIO); 
	OSTaskSuspend(OS_PRIO_SELF); //����start����
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}

void led0_task(void *pdata){
	while(1){	
//		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
//		LedToggle;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
//		OSTimeDly(500);//��ʱ500ms
		delay_us(8333333);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
		delay_us(8333333);
	}
}

void float_task(void *pdata){
	uint8_t len=0; 
	float i=100.123456;
	while(1){
		i*=(float)0.98765;
		printf("���㣺%f\r\n",i);
		if(USART2_RX_LEN&0x8000)
		{
			len=USART2_RX_LEN&0x3fff;			//��ȡ����2���������ַ�����			
			USART2_RX_BUF[len]=0;			    //����2��������ĩβ��0,��Ϊ�ַ����Ľ���	
			if(strstr((const char *)USART2_RX_BUF,(const char *)"japan"))   //�ж�USART2_RX_BUF���Ƿ����"SZ"
				{			 				
					printf("japan��%f\r\n",i);	//�������ù���ģ��  	
				}
			memset(USART2_RX_BUF,0,len);   //�������2
			USART2_RX_LEN=0; 
		}
		OSTimeDly(500);//��ʱ500ms
	}
}

void japan_task(void *pdata){
	while(1){	
		printf("japan\r\n");
		OSTimeDly(600);//��ʱ500ms
	}
}

void hello_task(void *pdata)
	{
		u8 datatemp[SIZE];
		u8 num=0;		
		while(1)
		{	
			OS_CPU_SR cpu_sr;
			OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
			num++;
			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
			delay_ms(10);
			AT24CXX_Read(0,datatemp,SIZE);
			delay_ms(50); 
			u2_printf("�����%s\r\n\r\n",datatemp);
			OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�
//			delay_ms(800);//��ʱ500ms
			OSTimeDly(1200);//��ʱ500ms
		}
}

//�¶Ȼ�ȡ����
void tmp_task(void *pdata)
{	
	u16 Conf_Read=0;
	u16 Tdie_Temp=0;
	u16 Vobj_Read;  
	float Vobj;  
	float Tdie;  
	while(1)
	{	
		do  
		{  
				// TODO: enter the block content here  
				Conf_Read = tmp006_ReadTwoByte(0x02);  
		} while ((Conf_Read&0x0080) != 0x0080); 
		 Tdie_Temp = tmp006_ReadTwoByte(0x01) ;
		
		u2_printf("�����¶ȣ�%d\r\n",Tdie_Temp);
		//delay_ms(1000);
		OSTimeDly(1200);
	}
}

















