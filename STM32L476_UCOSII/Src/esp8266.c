#include "esp8266.h"
#include "usart.h"
#include "timer.h"


u8 wifi_ssid[20]="Medical_Bed";	            //�ȵ�����
u8 wifi_ssid_password[20]="12345678";	    //�ȵ�����  

//WiFi����-·��-Ĭ��ֵ
u8 wifi_station[20]="tmqs";					//·��������
u8 wifi_station_password[20]="daizhiwen";	//·��������
u8 wifi_ap_ip[15]="192.168.1.115";          //IP��ַ
u16 wifi_ap_port=8086;                      //�˿ں�

/***********************************************************************
 ������      ��atk_8266_at_response(u8 mode)  
 ��������    ���ô���2��ӡWiFi���յ������ݡ�
 ����        ��0��USART3_RX_LEN�����㣬1������
 ���        ����  
************************************************************************/
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_LEN&0X8000)			//�ȴ�����ֵ
	{ 
		USART3_RX_BUF[USART3_RX_LEN&0X7FFF]=0;	//����4��������ĩβ��0
		u2_printf("%s",USART3_RX_BUF);					//����2�������4������������
		if(mode)USART3_RX_LEN=0;								//0��USART3_RX_LEN�����㣬1������
	} 
}


/***********************************************************************
 ������      ��atk_8266_check_cmd(u8 *str)  
 ��������    ���ж�USART3_RX_BUF����������ָ���ַ���
 ����        ��0��USART3_RX_LEN�����㣬     1������
 ���        ��ָ���ַ�����һ���ַ��ĵ�ַ
************************************************************************/
u8* atk_8266_check_cmd(u8 *str)
{	
	char *strx=0;
	if(USART3_RX_LEN&0X8000)	//�ȴ�����ֵ
	{ 
		USART3_RX_BUF[USART3_RX_LEN&0X7FFF]=0;	//����4��������ĩβ��0
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str); 		//��ȡ�����ַ���str��һ���ַ��ĵ�ַ
	} 
	return (u8*)strx;					//�����ַ����׵�ַ
}

/***********************************************************************
 ������      ��atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
 ��������    ������WiFiָ��
 ����        ��cmd:ָ��,ack:����ֵ,waittime:��ʱʱ��
 ���        ��1: ָ��ͳɹ�������ֵ��ȷ  0��ʧ��  

************************************************************************/
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u16 time=0;
	u8 res=0; 
	USART3_RX_LEN=0;												//��մ���4״̬���λ
	u3_printf("%s\r\n",cmd);              //����CMDָ��
	time = 0;
	while(time<waittime)									//����ظ����д���
	{
		delay_ms(130);		
		if(atk_8266_check_cmd(ack))       //�жϷ���ֵ
		{
			break;
		}	
		time++;				
	}
	if(time<waittime)res=1;                   //����ڳ�ʱʱ����break��������res=1
//	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);	
	return res;
} 

/***********************************************************************
 ������      ��atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
 ��������    ����������
 ����        ��
 ���        ��

************************************************************************/
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_LEN=0;				//��մ���4״̬���λ
	u3_printf("%s",data);	//��������data
	if(ack&&waittime)			//��֤ack�͵ȴ�ʱ��ǿ�
	{
		while(--waittime)	
		{
			delay_ms(5);			//��ʱ
			if(USART3_RX_LEN&0X8000)			//�ȴ�����ֵack
			{
				if(atk_8266_check_cmd(ack))break;			//�ж�ģ�鷵��ֵ
				USART3_RX_LEN=0;						//��մ���4״̬��־λ
			} 
		}
		if(waittime==0)res=1; 		//��waittimeΪ0������1
	}
	return res;
}

/***********************************************************************
 ������      ��atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
 ��������    ���������ݸ��豸0
 ����        ���ַ���
 ���        ���� 

************************************************************************/
void esp8266_send_data(u8 *data)
{
	u16 i;
	i=strlen((const char*)data);          //��ȡ���ݳ���
	u3_printf("AT+CIPSEND=0,%d\r\n",i);   //����AT+CIPSENDָ��
	delay_ms(5);       		//��ʱ                  
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);		//�������4���ջ�����	
	USART3_RX_LEN=0;                           	//��մ���4״̬��־λ    
	u3_printf("%s",data);                         //���������ַ�����data
}

/***********************************************************************
 ������      ��void ESP8266_send_data(u8 id,u8 *data)
 ��������    ���������ݸ��豸��Ϊid���豸
 ����        ��id���豸�ţ�data��Ҫ���͵�����
 ���        ���� 

************************************************************************/
void ESP8266_send_data(u8 id,u8 *data)
{
	u16 i;
	i=strlen((const char*)data);                //��ȡ���ݳ���
	u3_printf("AT+CIPSEND=%d,%d\r\n",id,i);     //����AT+CIPSENDָ��
	delay_ms(5);                     		//��ʱ    
	memset(USART3_RX_BUF,0,50);	                //�������4���ջ�����	
	USART3_RX_LEN=0;                            	//��մ���4״̬��־λ   
	u3_printf("%s",data);                       //���������ַ�����data	
	delay_ms(200);
	memset(USART3_RX_BUF,0,50);	                //�������4���ջ�����	
	USART3_RX_LEN=0; 														//��մ���4״̬��־λ
}

/***********************************************************************
 ������      ��atk_8266_quit_trans(void)
 ��������    ���˳�͸��
 ����        ����
 ���        ��1:�ɹ� 0:ʧ��

************************************************************************/
u8 atk_8266_quit_trans(void)
{
	while((UART4->ISR&0X40)==0);   //ѭ������"+",3��
	UART4->TDR='+';      
	delay_ms(15);				
	while((UART4->ISR&0X40)==0);	
	UART4->TDR='+';      
	delay_ms(15);	
	while((UART4->ISR&0X40)==0);	
	UART4->TDR='+';      
	delay_ms(500);	
	return atk_8266_send_cmd("AT","OK",20);			
}


/***********************************************************************
 ������      ��atk_8266_get_wanip(u8* ipbuf)
 ��������    ����ȡipbuf�У�����"\"��"\"�е�����
 ����        ����
 ���        ����

************************************************************************/
void atk_8266_get_wanip(u8* ipbuf)
{
	  u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))	//����AT+CIFSRָ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");		//��ȡ��1��"\"�ĵ�ַ
		p1=(u8*)strstr((const char*)(p+1),"\"");	//��ȡ��1��"\"�Ժ󣬵ڶ���"\"
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);			//д���ַ���
}

/***********************************************************************
 ������      ��esp8266_get_ip(u8* ipbuf)
 ��������    ����ipbuf�л�ȡ·���������STM��IP��ַ������"\"��"\"�е�����
 ����        ��ipbuf����ȡIP������
 ���        ����

************************************************************************/
void esp8266_get_ip(u8* ipbuf)
{
	  u8 *p,*p1;	
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//����AT+CIFSRָ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");								//��ȡ��1��"\"�ĵ�ַ
		p1=(u8*)strstr((const char*)(p+1),"\"");	//��ȡ��1��"\"�Ժ󣬵ڶ���"\"
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);		//д���ַ���
		u2_printf("\r\n%s\r\n",ipbuf);		//����2����
}

/***********************************************************************
 ������      ��atk_8266_SET_AP(u8 *ssid,u8 *password,u8 mode,u16 timeout)
 ��������    ��esp8266����APģʽ
 ����        ��ssid,password,ģʽ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 atk_8266_SET_AP(u8 *ssid,u8 *password,u8 mode,u16 timeout)
{
	u8 res=0;
	u16 time =0;	
	USART3_RX_LEN=0;	//��մ���4״̬��־λ
	u3_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n",ssid,password,mode);  //����ָ��
	time=0;
	while(time<timeout)			//����ظ����д���
	{
		if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
		break;
		delay_ms(10);
		time++;
	}
	if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;
}

/***********************************************************************
 ������      ��atk_8266_SET_STA(u8 *ssid,u8 *password,u16 timeout)
 ��������    ��esp8266����STAģʽ
 ����        ��ssid,password,ģʽ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 atk_8266_SET_STA(u8 *ssid,u8 *password,u16 timeout)
{	
	 u8 res=0;
	 u16 time =0;
	 USART3_RX_LEN=0;	//��մ���4״̬��־λ
	u3_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);  //����ָ��
	time=0;
	while(time<timeout)			//����ظ����д���
	{
		if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
	    break;
	    delay_ms(40);
		time++;
	}
	if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;	 
}

/***********************************************************************
 ������      ��atk_8266_SET_IPPORT(u16 timeout)
 ��������    ��esp8266���ö�·���ӣ�������IP�˿�Ϊ192.168.1.115
 ����        ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 atk_8266_SET_IPPORT(u16 timeout)
{
	u16 time =0;
	u8 res = 0;
	USART3_RX_LEN=0;	//��մ���4״̬��־λ

	if(atk_8266_send_cmd("AT+CIPMUX=1","OK",50))		//����AT+CIPMUX=1ָ��
	{
		u2_printf("\r\n������·���ӳɹ�\r\n");		
	}
	else
	{
		u2_printf("\r\n������·����ʧ��\r\n");			
	}
		
	if(atk_8266_send_cmd("AT+CIPAP=\"192.168.1.115\"","OK",100))   //����IP��ַ��192.168.1.115
	{
		u2_printf("����IP�ɹ�");	
	}
	else
	{
		u2_printf("����IPʧ��");	
	}
	
	u3_printf("AT+CIPSERVER=1,8086\r\n");    //���ö˿ں�Ϊ8086
	time=0;
	while(time<timeout)			//����ظ����д���
	{
		if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
		break;
		delay_ms(10);
		time++;
	}
	if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;
}

/***********************************************************************
 ������      ��atk_8266_SET_IPPORT(u16 timeout)
 ��������    �����ö˿ں�
 ����        ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 atk_8266_SET_PORT(u16 timeout)
{
	u8 res = 0;
		u16 time =0;
	USART3_RX_LEN=0;	//��մ���4״̬��־λ

//  if(atk_8266_send_cmd("AT+CIPMUX=1","OK",450))
//	{
//		 u2_printf("\r\n������·���ӳɹ�\r\n");
//	}
//	  else
//	{
//	  u2_printf("\r\n������·����ʧ��\r\n");		
//	}
		
	if(atk_8266_send_cmd("AT+CIPSERVER=1,8080","OK",50))//����"AT+CIPSERVER=1,8080"ָ��
	{
		delay_ms(1000); u2_printf("\r\n�˿����óɹ�\r\n");res=1;
	}
	else
	{
		u2_printf("\r\n�˿�����ʧ��\r\n");	res=0;
	}
//	time=0;
//   while(time<timeout)			//����ظ����д���
//	 {
//	    if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
//	    break;
//	    delay_ms(20);
//			time++;
//	 }
//	 if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;
}


/***********************************************************************
 ������      ��ESP8266_SET_IP(u8 *Ip,u16 timeout)
 ��������    �����ö˿ں�
 ����        ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 ESP8266_SET_IP(u8 *Ip,u16 timeout)
{
	u8 res = 0;
	u16 time =0;
	USART3_RX_LEN=0;	//��մ���4״̬��־λ

	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);	//�������4���ջ�����	
	
  u3_printf("AT+CIPAP=\"%s\"\r\n",Ip);  //����ָ��	
	delay_ms(100);
	time=0;
	while(time<timeout)			//����ظ����д���
	{
		if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
	    break;
	    delay_ms(40);
		time++;
	}
	if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;	 
}

/***********************************************************************
 ������      ��ESP8266_SET_PORT(u8 *Ip,u16 timeout)
 ��������    �����ö˿ں�
 ����        ����ʱʱ��
 ���        ��0��ʧ��  1���ɹ�

************************************************************************/
u8 ESP8266_SET_PORT(u16 Port,u16 timeout)
{
	u8 res = 0;
	u16 time =0;	
	USART3_RX_LEN=0;	//��մ���4״̬��־λ

	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);	//�������4���ջ�����	
	
  u3_printf("AT+CIPSERVER=1,%d\r\n",Port);  //����ָ��	
	time=0;
	while(time<timeout)			//����ظ����д���
	{
		if(strstr((const char *)USART3_RX_BUF,(const char *)"OK"))		//�������ֵΪ"OK"
	    break;
	    delay_ms(40);
		time++;
	}
	if(time<timeout) res =1;		//����ǰ���д���С��������������1�����򷵻�0
	return res;	 
}


/***********************************************************************
 ������      ��esp8266_Get_IP(u16 timeout)
 ��������    ��
 ����        ����
 ���        ����

************************************************************************/
void esp8266_Get_IP(void)
{
	if(atk_8266_send_cmd("AT+CIFSR","OK",500))//����AT+CIFSRָ��
		{u2_printf("��ȷ������IP��ַ");}
	else
		{u2_printf("��ȡ����ֵʧ��");}
	atk_8266_at_response(1);	//���ʹ���ģ�鷵��ֵ
}



/***********************************************************************
 ��������ESP8266_AP_Init
 ���ܣ�  ��ʼ��esp8266��APģʽ
 ������  ��
 ����ֵ����
 ����: �ȵ�����ATK_ESP8266;���룺12345678;�˿ںţ�8086

***********************************************************************/
void ESP8266_AP_Init(u8 n)
{
	 u8 i;
	 for(i=0;i<n;i++)	  //���ѭ����������Ϊ����n
	 {
		 if(atk_8266_send_cmd("AT+CWMODE=2","OK",60))	//����ָ�AT+CWMODE=2
		 {
			u2_printf("\r\nģʽ���óɹ�\r\n");
			break;
		 }
		 else
		 {
			u2_printf("\r\n��%d��ģʽ���ó���ʧ�ܣ�׼��������������\r\n",(i+1));
		 }
		delay_ms(300);			 
	 }

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_send_cmd("AT+CIPMUX=1","OK",60))	//����ָ�AT+CIPMUX=1
		 {
			u2_printf("\r\n�����ӿ���\r\n");
			break;
		 }
		 else
		 {	u2_printf("\r\n��%d�ζ����ӿ���ʧ�ܣ�׼��������������\r\n",(i+1)); }
		 delay_ms(300);			 
	 }	 

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
			if(atk_8266_SET_AP(wifi_ssid,wifi_ssid_password,WPA_WPA2_PSK,500)) //�����������ƣ����룬���ܷ�ʽ
			{
				delay_ms(300);	
				u2_printf("\r\nAP���óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d��AP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}
			delay_ms(400);	
	 }	 

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
			if(ESP8266_SET_IP(wifi_ap_ip,60))			//����IP
			{
				u2_printf("\r\nIP���óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d��IP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}		
			delay_ms(600);			
	 }

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {		 
			if(ESP8266_SET_PORT(wifi_ap_port,50))		//���ö˿ں�
			{
				u2_printf("\r\n�˿����óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d�ζ˿����ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}
			delay_ms(1200);					
	 }
	 	 	for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {		 
		 if(atk_8266_send_cmd("AT+CIPSTO=150","OK",60))	//����ָ�AT+CWMODE=2
		 {
			u2_printf("\r\n��ʱʱ�����óɹ�\r\n");
			break;
		 }
		 else
		 {
			u2_printf("\r\n��%d�γ�ʱʱ�����ó���ʧ�ܣ�׼��������������\r\n",(i+1));
		 }
		delay_ms(200);				
	 }
}




/***********************************************************************
 ������    ��esp8266_STA_SERVER_Init
 ����      ����ʼ��esp8266��STAģʽ
 ����      ����
 ����ֵ    ����
 ����: 

***********************************************************************/
void esp8266_STA_SERVER_Init(void)
{
  u8 i;
	for(i=0;i<2;i++)
	{
		delay_ms(100);		
		if(atk_8266_send_cmd("AT+CWMODE=1","OK",500))		//����ָ�AT+CWMODE=1
			{u2_printf("\r\nģʽ���óɹ�\r\n");}
		else
			{u2_printf("\r\nģʽ����ʧ��\r\n");}	
			
		if(atk_8266_send_cmd("AT+RST","OK",3000))		//����ָ�AT+RST
			{u2_printf("\r\n���óɹ�\r\n");}
		else
			{u2_printf("\r\n����ʧ��\r\n");}
			
		delay_ms(5000);
					
		if(atk_8266_SET_STA(WIFI_STATION,PASSWORD,600)) 	//����·�������ƺ�����
			{delay_ms(5000);u2_printf("\r\nWiFi���ӳɹ�\r\n");}
		else
			{u2_printf("\r\nWiFi����ʧ��\r\n");}
			
		delay_ms(3000);

		if(atk_8266_SET_PORT(600)) 								//��������˿ں�
			{ u2_printf("\r\n �˿����óɹ� \r\n"); }		  		
		else	
			{ u2_printf("\r\n �˿����óɹ� \r\n"); }		
	}
}

/***********************************************************************
 ������    ��esp_8266_apsta_Init
 ����      ����ʼ��esp8266��AP������-TCP������ģʽ
 ����      ��n,ʧ����������
 ����ֵ    ����
 ����: 

***********************************************************************/
void esp_8266_apsta_Init(u8 n)
{
   u8 i;
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_send_cmd("AT+CWMODE=3","OK",100))	//����ָ�AT+CWMODE=3
		 {
			u2_printf("\r\nģʽ���óɹ�\r\n");
			break;
		 }
		 else
		 {
			u2_printf("\r\n��%d��ģʽ���ó���ʧ�ܣ�׼��������������\r\n",(i+1));
		 }
	 }
	for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	{
		if(atk_8266_send_cmd("AT+RST","OK",100))	//����ָ�AT+RST
		{
			u2_printf("\r\n�������óɹ�\r\n");
			delay_ms(2000);	
			break;
		}
		else
		{
			u2_printf("\r\n��%d����������ʧ�ܣ�׼��������������\r\n",i+1);
		}
//			 delay_ms(1000);		 
	}
	for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	{
		if(atk_8266_SET_AP(wifi_ssid,wifi_ssid_password,WPA_WPA2_PSK,500))		//�����������ƣ����룬���ܷ�ʽ
		{
			u2_printf("\r\nAP���óɹ�\r\n");
			break;
		}
		else
		{
			u2_printf("\r\n��%d��AP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
		}
			delay_ms(100);
	 }

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
			if(ESP8266_SET_IP(wifi_ap_ip,60))							//��������IP��ַ
			{
				u2_printf("\r\nIP���óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d��IP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}		
			delay_ms(500);			
	 }

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {		 
			if(ESP8266_SET_PORT(wifi_ap_port,50))			//��������˿ں�
			{
				u2_printf("\r\n�˿����óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d�ζ˿����ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}
			delay_ms(1000);					
	 }

	
	for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	{
		if(atk_8266_SET_STA(wifi_station,wifi_station_password,700))			//����·�������ƺ�����
		{
			delay_ms(4000);
			u2_printf("\r\nWIFI-STATION���óɹ�\r\n");
			break;
		}
		else
		{
			u2_printf("\r\n��%d��WIFI-STATION���ó���ʧ�ܣ�׼��������������\r\n",i+1);
		}
	 }
	
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
//	   delay_ms(50);
		if(atk_8266_SET_PORT(200))					//���ö˿ں�
		{
			u2_printf("\r\n�������˿����óɹ�\r\n");
			break;
		}
		else
		{
			u2_printf("\r\n��%d�η������˿�����ʧ�ܣ�׼��������������\r\n",i+1);
		}
	 }	 
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		delay_ms(20);
		if(atk_8266_send_cmd("AT+CIPSTO=2200","OK",500))			//AT+CIPSTO=2200�����öϿ�����ʱ��
		{
			u2_printf("\r\n��ʱʱ�����óɹ�\r\n");
			break;
		}
		else
		{
			u2_printf("\r\n��%d�γ�ʱʱ�����ó���ʧ�ܣ�׼��������������\r\n",i+1);
		}
	 } 
//	 esp8266_get_ip(wifi_ip_address);
	 delay_ms(500);
	 esp8266_Get_IP();		//��ȡ�������IP��ַ
	 u2_printf("\r\nready\r\n");
}




/***********************************************************************
 ������    ��ESP8266_apsta_Init
 ����      ����ʼ��esp8266��AP������-TCP������ģʽ
 ����      ��n,ʧ����������
 ����ֵ    ����
 ����      ��
***********************************************************************/
void ESP8266_apsta_Init(u8 n)
{
   u8 i;
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_send_cmd("AT+CWMODE=3","OK",100))		//����ָ�AT+CWMODE=3
		 {
		   u2_printf("\r\nģʽ���óɹ�\r\n");
			 break;
		 }
		 else
		 {
		   u2_printf("\r\n��%d��ģʽ���ó���ʧ�ܣ�׼��������������\r\n",(i+1));
		 }
	 }
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_send_cmd("AT+RST","OK",100))			//����ָ�AT+RST
		 {
		   u2_printf("\r\n�������óɹ�\r\n");
			 break;
		 }
		 else
		 {
		   u2_printf("\r\n��%d����������ʧ�ܣ�׼��������������\r\n",i+1);
		 }	 
	 }
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_SET_AP(wifi_ssid,wifi_ssid_password,WPA_WPA2_PSK,500))				//�����������ƣ����룬���ܷ�ʽ
		 {
		   u2_printf("\r\nAP���óɹ�\r\n");
			 break;
		 }
		 else
		 {
		   u2_printf("\r\n��%d��AP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
		 }
		 delay_ms(100);
	 }
	 
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
			if(ESP8266_SET_IP(wifi_ap_ip,60))		//����IP
			{
				u2_printf("\r\nIP���óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d��IP���ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}		
			delay_ms(1000);			
	 }
	 
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
		 if(atk_8266_SET_STA(wifi_station,wifi_station_password,3000))		//����·�������ƺ�����
		 {
		   u2_printf("\r\nWIFI-STATION���óɹ�\r\n");
			 delay_ms(9000);
			 break;
		 }
		 else
		 {
		   u2_printf("\r\n��%d��WIFI-STATION���ó���ʧ�ܣ�׼��������������\r\n",i+1);
		 }
		 memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);	          //�������4���ջ�����	
		 delay_ms(8000);
	 }

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
			if(atk_8266_send_cmd("AT+CIPMUX=1","OK",450))
			{
				 u2_printf("\r\n������·���ӳɹ�\r\n");
				 break;
			}
				else
			{
				u2_printf("\r\n��%d�ζ�·����ʧ�ܣ�׼��������������\r\n",i+1);		
			}
		 memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);			          //�������4���ջ�����	
		 delay_ms(1000);
	 }	

	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {		 
			if(ESP8266_SET_PORT(wifi_ap_port,50))			//���ö˿ں�
			{
				u2_printf("\r\n�˿����óɹ�\r\n");
				break;
			}
			else
			{
				u2_printf("\r\n��%d�ζ˿����ó���ʧ�ܣ�׼��������������\r\n",i+1);
			}
			delay_ms(1000);					
	 }	 

	 
	 for(i=0;i<n;i++)		//���ѭ����������Ϊ����n
	 {
	   delay_ms(20);
		 if(atk_8266_send_cmd("AT+CIPSTO=2200","OK",500))
		 {
		   u2_printf("\r\n��ʱʱ�����óɹ�\r\n");
			 break;
		 }
		 else
		 {
		   u2_printf("\r\n��%d�γ�ʱʱ�����ó���ʧ�ܣ�׼��������������\r\n",i+1);
		 }
		 memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);			          //�������4���ջ�����	
		 delay_ms(1000);
	 } 
	 esp8266_Get_IP();		//��ȡIP
	 u2_printf("\r\nready\r\n");
}



