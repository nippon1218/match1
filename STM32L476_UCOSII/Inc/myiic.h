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

/***********************IIC所有操作函数************************************/

void IIC_Init(void);                    //初始化IIC的IO口				 
void IIC_Start(void);				    //发送IIC开始信号
void IIC_Stop(void);	  			    //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			    //IIC发送一个字节
u8   IIC_Read_Byte(unsigned char ack);  //IIC读取一个字节
u8   IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					    //IIC发送ACK信号
void IIC_NAck(void);				    //IIC不发送ACK信号	



#endif

