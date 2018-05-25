#include "bma222.h"

u8 bmadata[3];


void bma222_Init(void)
{
	IIC_Init();			//IIC��ʼ��
}


void bma222_ReadAcc(void)
{
	u8 x_acc=0;
	u8 y_acc=0;	
	u8 z_acc=0;

	IIC_Start(); 
	IIC_Send_Byte(0X30);       //����д����  
	IIC_Wait_Ack();  
	IIC_Send_Byte(0x02);    //���Ͷ���ַָ��  
	IIC_Wait_Ack();             
		
	IIC_Start();             
	IIC_Send_Byte(0X31);           //���Ͷ�����������ģʽ                
	IIC_Wait_Ack(); 
	
	IIC_Read_Byte(1); 
	bmadata[0]=IIC_Read_Byte(1); 
	
	IIC_Read_Byte(1); 
	bmadata[1]=IIC_Read_Byte(1); 	
	
	IIC_Read_Byte(1); 
	bmadata[2]=IIC_Read_Byte(0); 		
	IIC_Stop();                     //����һ��ֹͣ���� 	
	
}











