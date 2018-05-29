#include "adc.h"
#include "delay.h"
#include "usart.h"



ADC_HandleTypeDef hadc3;

void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;
    /**Common config 
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//2分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;			  //12位模式
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;					//右对齐
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE; 				//非扫描模式
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  hadc3.Init.EOCSelection = DISABLE;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;							//关闭连续转换
  hadc3.Init.NbrOfConversion = 1; 											//1个转换在规则序列中 也就是只转换规则序列1 
  hadc3.Init.DiscontinuousConvMode = DISABLE;						//禁止不连续采样模式
//	hadc3.Init.NbrOfDiscConversion = 1;	
  hadc3.Init.NbrOfDiscConversion = 0;										//不连续采样通道数为0
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;			//软件触发
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;		//使用软件触发
  hadc3.Init.DMAContinuousRequests = DISABLE;						//关闭DMA请求
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)										 //初始化 
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
//  sConfig.Channel = ADC_CHANNEL_1;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//	
//  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLE_5;
//  sConfig.SingleDiff = ADC_SINGLE_ENDED;
//  sConfig.OffsetNumber = ADC_OFFSET_NONE;
//  sConfig.Offset = 0;
//  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }

}


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
    /**ADC3 GPIO Configuration    
    PC0     ------> ADC3_IN1
    PC1     ------> ADC3_IN2
    PC2     ------> ADC3_IN3
    PC3     ------> ADC3_IN4 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;		//PC0,1,2,3
//    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();
  
    /**ADC3 GPIO Configuration    
    PC0     ------> ADC3_IN1
    PC1     ------> ADC3_IN2
    PC2     ------> ADC3_IN3
    PC3     ------> ADC3_IN4 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }

}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}





u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC3_ChanConf;
    
    ADC3_ChanConf.Channel=ch;                                   //通道
    ADC3_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_2CYCLE_5;        //采样时间
    ADC3_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc3,&ADC3_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc3);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc3,10);                //轮询转换

 
	return (u16)HAL_ADC_GetValue(&hadc3);	        //返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
		__HAL_ADC_CLEAR_FLAG(&hadc3, ADC_FLAG_EOS);
	}
	return temp_val/times;
} 









