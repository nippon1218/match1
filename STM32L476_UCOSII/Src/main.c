/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "myiic.h"

/* USER CODE BEGIN Includes */
#include "includes.h"
#include "ucosiitask.h"
#include "delay.h"
#include "24cxx.h"
#include "tmp006.h"
#include "esp8266.h"
#include "timer.h"

int main(void)
{
//	OS_CPU_SR cpu_sr;
//	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	
  HAL_Init();
	delay_init(80);
	IIC_Init();
		
  SystemClock_Config();
	
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();	

	
//	ESP8266_AP_Init(4);
	
//	OS_EXIT_CRITICAL();  //�˳��ٽ���,����
	printf("ʱ����ƵΪ��%dMhz\r\n",HAL_RCC_GetHCLKFreq()/1000000);
	OSInit();  //��ʼ��UCOS
	OSTaskCreate(start_task,  									//start_task����
					(void*)0,														//����
					(OS_STK*)&START_TASK_STK[START_STK_SIZE-1], 	//�����ջջ��
					START_TASK_PRIO);  								//�������ȼ�
	OSStart(); 

  while (1)
  {

  }
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
