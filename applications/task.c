/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "task.h"
#include  "main.h"
#include "BLE_Decode.h"
#include "string.h"

uint8_t BLERxData[BLE_RXDATABUFFER_LEN];
static rt_thread_t dynamic_thread = RT_NULL;

void led1_thread_entry(void* parameter);



TaskStruct TaskThreads[] = {
			{"ledThread", 					led1_thread_entry, 		RT_NULL, 512, 9, 10},
			{"usart2_recv_thread",          App_BLE_ThreadEntry, 	RT_NULL, 2048, 8, 10},
	
			{"",RT_NULL, RT_NULL,RT_NULL,RT_NULL,RT_NULL}
};
/* USER CODE BEGIN 0 */
void TaskInit(void)
{		
	uint8_t TaskThreadIndex = 0;
		while(1)
	 {
		 if(strcmp(TaskThreads[TaskThreadIndex].name,"") != 0)
		 {
			 
				dynamic_thread = rt_thread_create(TaskThreads[TaskThreadIndex].name,
                                TaskThreads[TaskThreadIndex].entry,
                                TaskThreads[TaskThreadIndex].parameter,
                                TaskThreads[TaskThreadIndex].stack_size,
                                TaskThreads[TaskThreadIndex].priority,
                                TaskThreads[TaskThreadIndex].tick
                   	            );
				if(dynamic_thread != RT_NULL)
				{
					rt_thread_startup(dynamic_thread);
				}
			  TaskThreadIndex ++;
		 }
		 else
			 break;
	}
}
/* USER CODE END 0 */
void led1_thread_entry(void* parameter)
{	
    while (1)
    {
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        rt_thread_delay(300);   /* ��ʱ500��tick */		 		
    }
}

/**=====================================================================*
 * @brief  	App_BLE_ThreadEntry
 * @param  	void *param
 * @retval 	None	
 **=====================================================================*/
void App_BLE_ThreadEntry(void *param)
{
    static uint32_t RxLen;
    while (1)
    {
        if(RxLen != BLEGetRxLen())
        {
            RxLen = BLEGetRxLen();
        }
        else
        {
            RxLen = BLEUartRead(BLERxData, BLE_RXDATABUFFER_LEN);
            if (RxLen)
            {
							rt_kprintf("************BLE->MasterUnit**************************** \n");
							rt_kprintf(" BLE->MasterUnit Len: %d \n", RxLen);
							RevceiveNewBLEMessage(BLERxData, RxLen);
            }
        }
#ifdef TEST_DATA
        rt_thread_delay(BLE_DECODE_DELAY__);
#endif
        rt_thread_delay(BLE_DECODE_DELAY__);
    }
}


/* SPI2 init function */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
