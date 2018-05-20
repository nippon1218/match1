#include "ucosiitask.h"
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "includes.h"

//任务堆栈
__align(4) OS_STK	START_TASK_STK[START_STK_SIZE];
__align(4) OS_STK LED0_TASK_STK[LED0_STK_SIZE];
__align(8) OS_STK float_TASK_STK[float_STK_SIZE];
__align(8) OS_STK japan_TASK_STK[japan_STK_SIZE];



void start_task(void *pdata){
	OS_CPU_SR cpu_sr;

	OSStatInit();  //开启统计任务
	OS_ENTER_CRITICAL();  //进入临界区,关闭中断
	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO); 		//LED0任务
	OSTaskCreate(float_task,(void*)0,(OS_STK*)&float_TASK_STK[float_STK_SIZE-1],float_TASK_PRIO); 		//LED1任务
	OSTaskCreate(japan_task,(void*)0,(OS_STK*)&japan_TASK_STK[japan_STK_SIZE-1],japan_TASK_PRIO); 
	OSTaskSuspend(OS_PRIO_SELF); //挂起start任务
	OS_EXIT_CRITICAL();  //退出临界区,开中断	
}

void led0_task(void *pdata){
	while(1){	
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		OSTimeDly(500);//延时500ms
	}
}

void float_task(void *pdata){
	uint8_t len=0; 
	float i=100.123456;
	while(1){
		i*=(float)0.98765;
		printf("浮点：%f\r\n",i);
		if(USART2_RX_LEN&0x8000)
		{
			len=USART2_RX_LEN&0x3fff;			//获取串口2接收数组字符长度			
			USART2_RX_BUF[len]=0;			    //串口2接收数组末尾添0,作为字符串的结束	
			if(strstr((const char *)USART2_RX_BUF,(const char *)"japan"))   //判断USART2_RX_BUF中是否存在"SZ"
				{			 				
					printf("japan：%f\r\n",i);	//串口设置功能模块  	
				}
			memset(USART2_RX_BUF,0,len);   //清除串口2
			USART2_RX_LEN=0; 
		}
		OSTimeDly(500);//延时500ms
	}
}

void japan_task(void *pdata){
	while(1){	
		printf("japan\r\n");
		OSTimeDly(600);//延时500ms
	}
}







