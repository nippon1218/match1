#include "ucosiitask.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "includes.h"

//�����ջ
__align(4) OS_STK	START_TASK_STK[START_STK_SIZE];
__align(4) OS_STK LED0_TASK_STK[LED0_STK_SIZE];
__align(8) OS_STK float_TASK_STK[float_STK_SIZE];
__align(8) OS_STK japan_TASK_STK[japan_STK_SIZE];



void start_task(void *pdata){
	OS_CPU_SR cpu_sr;

	OSStatInit();  //����ͳ������
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO); 		//LED0����
	OSTaskCreate(float_task,(void*)0,(OS_STK*)&float_TASK_STK[float_STK_SIZE-1],float_TASK_PRIO); 		//LED1����
	OSTaskCreate(japan_task,(void*)0,(OS_STK*)&japan_TASK_STK[japan_STK_SIZE-1],japan_TASK_PRIO); 
	OSTaskSuspend(OS_PRIO_SELF); //����start����
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}

void led0_task(void *pdata){
	while(1){	
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		OSTimeDly(500);//��ʱ500ms
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







