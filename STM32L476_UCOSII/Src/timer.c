#include "timer.h"
#include "usart.h"


extern u16 USART3_RX_LEN;
TIM_HandleTypeDef TIM7_Handler; 


void TIM7_Init(u16 arr,u16 psc)
{
		__HAL_RCC_TIM7_CLK_ENABLE();			                  //ʹ�ܶ�ʱ��7

		TIM7_Handler.Instance=TIM7;                               //ͨ�ö�ʱ��7
    TIM7_Handler.Init.Prescaler=psc;                          //��Ƶϵ��
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //���ϼ�����
    TIM7_Handler.Init.Period=arr;                             //�Զ�װ��ֵ
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;   //ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM7_Handler);    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��7�Ͷ�ʱ��7�����жϣ�TIM_IT_UPDATE									 
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
		HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //����TIM7�ж�   
	}
}


void TIM7_IRQHandler(void)
{ 	    		    
	USART3_RX_LEN|=1<<15;	                                       //��ǽ�����ɱ�־λ
	__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );   //���TIM7�����жϱ�־  
	TIM7->CR1&=~(1<<0);     			                           //�رն�ʱ��7 
} 


