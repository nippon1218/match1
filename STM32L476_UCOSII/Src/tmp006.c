#include "tmp006.h"


u8 tmp006_Init(void)
{	
	 IIC_Init();			//IIC��ʼ�� 	
}

u16 tmp006_ReadTwoByte(u8 daddr)
{
	u16 temp;  
	IIC_Start();    
	IIC_Send_Byte(0X82);       //����д����  
	IIC_Wait_Ack();  
	IIC_Send_Byte(daddr);    //���Ͷ���ַָ��  
	IIC_Wait_Ack();             
		
	IIC_Start();             
	IIC_Send_Byte(0X83);           //���Ͷ�����������ģʽ                
	IIC_Wait_Ack();    
	temp = IIC_Read_Byte(1);    //���߰�λ����  
	temp = temp<<8;  
	temp += IIC_Read_Byte(0);   //���Ͱ�λ����  
	IIC_Stop();                     //����һ��ֹͣ����  
	return temp; 	
}









