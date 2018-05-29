#ifndef __UCOSiiTASK_
#define __UCOSiiTASK_
#include "includes.h"

//������������ȼ�
#define START_TASK_PRIO				0
//�����ջ��С 
#define START_STK_SIZE			  256

//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO 				12
//�����ջ��С
#define LED0_STK_SIZE				128

//float_TASK
//�����������ȼ�
#define float_TASK_PRIO 				4
//�����ջ��С
#define float_STK_SIZE				128

//japan_task
//�����������ȼ�
#define japan_TASK_PRIO 				5
//�����ջ��С
#define japan_STK_SIZE				128

#define hello_TASK_PRIO 				3
#define hello_STK_SIZE 				128

#define tmp_TASK_PRIO 				6
#define tmp_STK_SIZE 				128

#define bma_TASK_PRIO 				7
#define bma_STK_SIZE 				128

#define adc_TASK_PRIO 				8
#define adc_STK_SIZE 				128

//�����ջ
extern __align(4) OS_STK	START_TASK_STK[START_STK_SIZE];


void start_task(void *pdata);
void led0_task(void *pdata);
void float_task(void *pdata);
void japan_task(void *pdata);
void hello_task(void *pdata);
void tmp_task(void *pdata);
void bma_task(void *pdata);
void adc_task(void *pdata);
#endif


