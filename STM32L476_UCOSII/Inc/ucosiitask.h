#ifndef __UCOSiiTASK_
#define __UCOSiiTASK_
#include "includes.h"

//设置任务的优先级
#define START_TASK_PRIO				0
//任务堆栈大小 
#define START_STK_SIZE			  256

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 				12
//任务堆栈大小
#define LED0_STK_SIZE				128

//float_TASK
//设置任务优先级
#define float_TASK_PRIO 				4
//任务堆栈大小
#define float_STK_SIZE				128

//japan_task
//设置任务优先级
#define japan_TASK_PRIO 				5
//任务堆栈大小
#define japan_STK_SIZE				128

#define hello_TASK_PRIO 				3
#define hello_STK_SIZE 				128

#define tmp_TASK_PRIO 				6
#define tmp_STK_SIZE 				128

#define bma_TASK_PRIO 				7
#define bma_STK_SIZE 				128

#define adc_TASK_PRIO 				8
#define adc_STK_SIZE 				128

//任务堆栈
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


