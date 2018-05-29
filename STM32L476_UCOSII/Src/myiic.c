#include "myiic.h"
#include "sys.h"
#include "includes.h"
#include "delay.h"



void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //ʹ��GPIOCʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_8;
	
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //�������
    GPIO_Initure.Pull=GPIO_PULLUP;           //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

		IIC_SDA_1;		//IIC�����ߣ�Ĭ������
		IIC_SCL_1;		//IICʱ���ߣ�Ĭ������
}


void SDA_OUT()
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //ʹ��GPIOCʱ��
    GPIO_Initure.Pin=GPIO_PIN_6;		

    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //�������
    GPIO_Initure.Pull=GPIO_PULLUP;           //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}

void SDA_IN()
{
    GPIO_InitTypeDef GPIO_Initure;    
    __GPIOC_CLK_ENABLE();            //ʹ��GPIOCʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6;		
    GPIO_Initure.Mode=GPIO_MODE_INPUT;   //���븡��
    GPIO_Initure.Pull=GPIO_PULLUP;           //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}


//void IIC_Init(void)
//{
//    GPIO_InitTypeDef GPIO_Initure;    
//    __GPIOB_CLK_ENABLE();            //ʹ��GPIOCʱ��
//    
//    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
//	
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;           //����
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

//		IIC_SDA_1;		//IIC�����ߣ�Ĭ������
//		IIC_SCL_1;		//IICʱ���ߣ�Ĭ������
//}

//void SDA_OUT()
//{
//    GPIO_InitTypeDef GPIO_Initure;    
//    __GPIOB_CLK_ENABLE();            //ʹ��GPIOCʱ��
//    GPIO_Initure.Pin=GPIO_PIN_9;		

//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;           //����
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
//}

//void SDA_IN()
//{
//    GPIO_InitTypeDef GPIO_Initure;    
//    __GPIOB_CLK_ENABLE();            //ʹ��GPIOCʱ��
//	
//    GPIO_Initure.Pin=GPIO_PIN_9;		
//    GPIO_Initure.Mode=GPIO_MODE_INPUT;   //���븡��
//    GPIO_Initure.Pull=GPIO_PULLUP;           //����
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;      //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
//}

void IIC_Start(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	SDA_OUT();           //SDA�����

	IIC_SDA_1;	         //�����߸ߵ�ƽ  	  
	IIC_SCL_1;	         //ʱ���߸ߵ�ƽ 
	delay_us(34);				//��ʱ
	IIC_SDA_0;						//START�ź�:��ʱ����Ϊ�ߵ�ƽʱ�������ߴӸߵ��ͱ仯
	delay_us(34);
	IIC_SCL_0;					//ǯסI2C���ߣ�׼�����ͻ��������           
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}	  


void IIC_Stop(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	SDA_OUT();           //SDA�����
	IIC_SCL_0;	         //ʱ���߸ߵ�ƽ 
	IIC_SDA_0;	         //�����߸ߵ�ƽ  	  
	delay_us(34);				//��ʱ
	IIC_SCL_1;           //STOP�ź�:��ʱ����Ϊ�ߵ�ƽ�������ߴӵ͵��߱仯
	IIC_SDA_1;           //����I2C���߽����ź�
	delay_us(34);
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}	 

/***********************************************************************
 ������      ��IIC_Wait_Ack(void) 
 ��������    ���ȴ�Ӧ���źŵ���
 ����        ����
 ���        ��1������Ӧ��ʧ��
               0������Ӧ��ɹ�                 
************************************************************************/
u8 IIC_Wait_Ack(void)
{
	OS_CPU_SR cpu_sr;
	u8 ucErrTime=0;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	
	SDA_IN();                 //SDA����Ϊ����  
	IIC_SDA_1;delay_us(34);	   
	IIC_SCL_1;delay_us(34);	 
	while(READ_SDA==GPIO_PIN_SET)           //����ڹ涨ʱ����δ�յ�Ӧ���źţ������Ӧ��ʧ��
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
			return 1;
		}
	}
	IIC_SCL_0;                //ʱ�����0 	   
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
	return 0;  
} 

/***********************************************************************
 ������      ��IIC_Ack(void) 
 ��������    ������ACKӦ��
 ����        ����
 ���        ����
************************************************************************/
void IIC_Ack(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	IIC_SCL_0;	         //ʱ���ߵ͵�ƽ
	SDA_OUT();
	IIC_SDA_0;	         //�����ߵ͵�ƽ
	delay_us(17);
	IIC_SCL_1;	         //ʱ���߸ߵ�ƽ
	delay_us(17);
	IIC_SCL_0;	         //ʱ���ߵ͵�ƽ
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}

/***********************************************************************
 ������      ��IIC_NAck(void) 
 ��������    ��������ACKӦ��
 ����        ����
 ���        ����                    
************************************************************************/
void IIC_NAck(void)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	IIC_SCL_0;	         //ʱ���ߵ͵�ƽ
	SDA_OUT();           //SDA����Ϊ���
	IIC_SDA_1;	         //�����߸ߵ�ƽ
	delay_us(17);
	IIC_SCL_1;	         //ʱ���߸ߵ�ƽ
	delay_us(17);
	IIC_SCL_0;	         //ʱ���ߵ͵�ƽ
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
}		

void IIC_Send_Byte(u8 txd)
{                        
	u8 t; 
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�
	SDA_OUT(); 	           //SDA����Ϊ���    
	IIC_SCL_0;             //����ʱ���ߣ���ʼ���ݴ���
	for(t=0;t<8;t++)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,(txd&0x80)>>7);			
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,(txd&0x80)>>7);	
		txd<<=1; 	  
		delay_us(17);       //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
		delay_us(17); 
		IIC_SCL_0;	
		delay_us(17);
	}	 
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�	
} 	

/***********************************************************************
 ������      ��IIC_Read_Byte(unsigned char ack)
 ��������    ��IIC��ȡһ���ֽ�
 ����        ��ack=1ʱ������ACK��ack=0������nACK 
 ���        ��receive                    
************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
	OS_CPU_SR cpu_sr;
	unsigned char i,receive=0;
	OS_ENTER_CRITICAL();  //�����ٽ���,�ر��ж�	
	SDA_IN();                   //SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
		IIC_SCL_0; 
		delay_us(17);
		IIC_SCL_1;
		receive<<=1;
		if(READ_SDA==GPIO_PIN_SET)receive++;   
		delay_us(9); 
  }
	OS_EXIT_CRITICAL();  //�˳��ٽ���,���ж�		
	if (!ack)
			IIC_NAck();            //����nACK
	else
			IIC_Ack();             //����ACK   
	return receive;
}




