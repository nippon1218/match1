#ifndef __ESP8266_H
#define __ESP8266_H	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	 	 	 	 	    
#include "string.h"   

#define SSID "Medical_Nursing_Bed"    //APģʽ�µ��ȵ�����
#define PASS "12345678"               //�ȵ������

//WiFi�ȵ�����
#define WIFI_STATION "mitsubishi"     //Ҫ�����WiFiվ������
#define PASSWORD "884869japan"        //��Ӧ������

#define ECN WPA_WPA2_PSK               
#define port 8086                     //�˿ں�
#define WPA_WPA2_PSK 4                //����ģʽ



void atk_8266_at_response(u8 mode);                           //�ô���2��ӡWiFi���յ������ݡ�
u8* atk_8266_check_cmd(u8 *str);                              //�ж�UART4_RX_BUF����������ָ���ַ��� 
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);           //����WiFiָ��
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);         //��������
u8 atk_8266_quit_trans(void);                                 //�˳�͸��
u8 atk_8266_SET_AP(u8 *ssid,u8 *password,u8 mode,u16 timeout);//esp8266����APģʽ
u8 atk_8266_SET_IPPORT(u16 timeout);                          //esp8266���ö�·����
u8 atk_8266_SET_STA(u8 *ssid,u8 *password,u16 timeout);       //esp8266����STAģʽ
u8 atk_8266_SET_PORT(u16 timeout);                            //���ö˿ں�

u8 stick_send_cmd(u8 *cmd,u8 *ack,u16 waittime);              //ͨ�����ڣ���PC��������Ϣ���ȴ�PC����Ӧ��
u8* stick_check_cmd(u8 *str);                                 //�жϴ��ڷ���ֵ���趨ֵ�Ƿ�һ��

u8 ESP8266_SET_IP(u8 *Ip,u16 timeout);                        //����IP��ַ
u8 ESP8266_SET_PORT(u16 Port,u16 timeout);                    //���ö˿ں�

void esp_8266_apsta_Init(u8 n);                 //��ʼ��esp8266��AP������-TCP������ģʽ

void atk_8266_get_wanip(u8* ipbuf);             //��ȡipbuf�У�����"\"��"\"�е�����
void esp8266_get_ip(u8* ipbuf);                 //��ipbuf�л�ȡ·���������STM��IP��ַ
void esp8266_AP_Init(void);                     //��ʼ��esp8266��APģʽ
void esp8266_STA_SERVER_Init(void);             //��ʼ��esp8266��STAģʽ
void esp8266_Get_IP(void);                      //��ȡIP��ַ
void esp8266_send_data(u8 *data);               //�������ݸ��豸0

void ESP8266_apsta_Init(u8 n);                  //��ʼ��esp8266��AP������-TCP������ģʽ
void ESP8266_AP_Init(u8 n);		                //��ʼ��esp8266��APģʽ
void ESP8266_Get_Controler_Type(u16 timeout);   //��WiFi�豸����ʱ��������Ӧ���豸��
void ESP8266_Close_Controler_Type(void);        //wifi�豸�˳�ʱ���ͷŷ�����豸��
void ESP8266_send_data(u8 id,u8 *data);         //�������ݸ��豸��Ϊid���豸
u8   ESP8266_Get_ID(void);                      //���յ���ǰ�豸���͵�ָ���������䷵��һ��Receivedָ��

void get_wifiname_wifipassword(u8 *ip1,u8 *ip2);//��ȡ�ַ����е�WiFi���ƺ�����
void get_apname_wifipassword(u8 *ip1,u8 *ip2);  //��ȡ�ַ����е�WiFi���ƺ�����
void get_ip_port(u8 *ip1,u16 data);             //��ȡ�ַ����е�WiFi��IP��ַ�Ͷ˿ں�

void Flash_Read_STA(void);                      //��Flash�ڶ�ȡ�����·�������ƺ�����
void Flash_Read_AP(void);                       //��Flash�ڶ�ȡ������ȵ����ƺͶ�Ӧ����
void Flash_Read_AP_IPPORT(void);                //��Flash�ڶ�ȡ�����IP��ַ�Ͷ˿ں�
void WIFIStateCheck(void);
#endif

