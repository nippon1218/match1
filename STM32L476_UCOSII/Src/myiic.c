#include "myiic.h"
#include "sys.h"
#include "includes.h"
#include "delay.h"

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //使能GPIOC时钟
    
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_8;
	
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

		IIC_SDA_1;		//IIC数据线，默认拉高
		IIC_SCL_1;		//IIC时钟线，默认拉高
}


void SDA_OUT()
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //使能GPIOC时钟
    GPIO_Initure.Pin=GPIO_PIN_6;		

    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}

void SDA_IN()
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //使能GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6;		
    GPIO_Initure.Mode=GPIO_MODE_INPUT;   //输入浮空
    GPIO_Initure.Pull=GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}


void IIC_Start(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //进入临界区,关闭中断
	SDA_OUT();           //SDA线输出

	IIC_SDA_1;	         //数据线高电平  	  
	IIC_SCL_1;	         //时钟线高电平 
	delay_us(34);				//延时
	IIC_SDA_0;						//START信号:当时钟线为高电平时，数据线从高到低变化
	delay_us(34);
	IIC_SCL_0;					//钳住I2C总线，准备发送或接收数据           
	OS_EXIT_CRITICAL();  //退出临界区,开中断	
}	  


void IIC_Stop(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //进入临界区,关闭中断
	SDA_OUT();           //SDA线输出
	IIC_SCL_0;	         //时钟线高电平 
	IIC_SDA_0;	         //数据线高电平  	  
	delay_us(34);				//延时
	IIC_SCL_1;           //STOP信号:当时钟线为高电平，数据线从低到高变化
	IIC_SDA_1;           //发送I2C总线结束信号
	delay_us(34);
	OS_EXIT_CRITICAL();  //退出临界区,开中断	
}	 


u8 IIC_Wait_Ack(void)
{
	OS_CPU_SR cpu_sr;
	u8 ucErrTime=0;
	OS_ENTER_CRITICAL();  //进入临界区,关闭中断
	
	SDA_IN();                 //SDA设置为输入  
	IIC_SDA_1;delay_us(34);	   
	IIC_SCL_1;delay_us(34);	 
	while(READ_SDA==GPIO_PIN_SET)           //如果在规定时间内未收到应答信号，则接收应答失败
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0;                //时钟输出0 	   
	OS_EXIT_CRITICAL();  //退出临界区,开中断	
	return 0;  
	
} 


void IIC_Ack(void)
{
	IIC_SCL_0;	         //时钟线低电平
	SDA_OUT();
	IIC_SDA_0;	         //数据线低电平
	delay_us(17);
	IIC_SCL_1;	         //时钟线高电平
	delay_us(17);
	IIC_SCL_0;	         //时钟线低电平
}

void IIC_NAck(void)
{
	IIC_SCL_0;	         //时钟线低电平
	SDA_OUT();           //SDA设置为输出
	IIC_SDA_1;	         //数据线高电平
	delay_us(17);
	IIC_SCL_1;	         //时钟线高电平
	delay_us(17);
	IIC_SCL_0;	         //时钟线低电平
}		

void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	           //SDA设置为输出    
    IIC_SCL_0;             //拉低时钟线，开始数据传输
    for(t=0;t<8;t++)
    {
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,(txd&0x80)>>7);			
			txd<<=1; 	  
			delay_us(17);       //对TEA5767这三个延时都是必须的
			IIC_SCL_1;
			delay_us(17); 
			IIC_SCL_0;	
			delay_us(17);
    }	 
} 	

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();                   //SDA设置为输入
  for(i=0;i<8;i++ )
	{
		IIC_SCL_0; 
		delay_us(17);
		IIC_SCL_1;
		receive<<=1;
		if(READ_SDA==GPIO_PIN_SET)receive++;   
		delay_us(9); 
  }					 
	if (!ack)
			IIC_NAck();            //发送nACK
	else
			IIC_Ack();             //发送ACK   
	return receive;
}




