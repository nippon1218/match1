#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

#define IIC_SDA_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)		
#define IIC_SDA_0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)

#define IIC_SCL_1 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)
#define IIC_SCL_0 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)

#define READ_SDA	 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)		


//#define IIC_SDA_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)		
//#define IIC_SDA_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)

//#define IIC_SCL_1 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET)
//#define IIC_SCL_0 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET)

//#define READ_SDA	 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)	

/***********************IIC���в�������************************************/

void IIC_Init(void);                    //��ʼ��IIC��IO��				 
void IIC_Start(void);				    //����IIC��ʼ�ź�
void IIC_Stop(void);	  			    //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			    //IIC����һ���ֽ�
u8   IIC_Read_Byte(unsigned char ack);  //IIC��ȡһ���ֽ�
u8   IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					    //IIC����ACK�ź�
void IIC_NAck(void);				    //IIC������ACK�ź�	



#endif

