/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ringbuffer.h"
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;


/* USER CODE BEGIN PV */

#if UART1_HW_AVAILABLE
UART_HandleTypeDef              huart1;
DMA_HandleTypeDef               hdma_uart1_rx;
uint8_t                         aDMABuf_uart1_rx[UART_DMA_BUF_SIZE]    = {0};
ring_buf_t                      tRingBuf_uart1_rx;
ring_buf_t                      tRingBuf_uart1_tx;
uint8_t                         aRxBuf_Uart1[UART_RINGBUF_SIZE]    = {0};
uint8_t                         aTxBuf_Uart1[UART_RINGBUF_SIZE]    = {0};
struct                          rt_semaphore rtSemUart1Rx;
#endif


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
__WEAK __WEAK int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
    static uint8_t uart1_init_clpt_flag = 0;
    if(uart1_init_clpt_flag == 0)
    {
            huart1.Instance = USART1;
            huart1.Init.BaudRate = 115200;
            huart1.Init.WordLength = UART_WORDLENGTH_8B;
            huart1.Init.StopBits = UART_STOPBITS_1;
            huart1.Init.Parity = UART_PARITY_NONE;
            huart1.Init.Mode = UART_MODE_TX_RX;
            huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
            huart1.Init.OverSampling = UART_OVERSAMPLING_16;
            if (HAL_UART_Init(&huart1) != HAL_OK)
            {
                Error_Handler();
            }
            ring_buf_create(&tRingBuf_uart1_rx, aRxBuf_Uart1, sizeof(aRxBuf_Uart1));
            ring_buf_create(&tRingBuf_uart1_tx, aTxBuf_Uart1, sizeof(aTxBuf_Uart1));

            rt_sem_init(&rtSemUart1Rx, "uart1 rx",0,RT_IPC_FLAG_FIFO);
            #if (UART1_USE_DMA)
            __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
            HAL_UART_Receive_DMA(&huart1,aDMABuf_uart1_rx,UART_DMA_BUF_SIZE);
            #else
            __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
            __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
            #endif
            uart1_init_clpt_flag = 1;
    }
}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}


/* USER CODE BEGIN 1 */
void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&huart3);

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&huart3, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&huart3, (uint8_t *)(str + i), 1, 1);
    }
}

char rt_hw_console_getchar(void)
{
        int ch = -1;
        if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET)
        {
                //notice stm32f429 and stm32f475 is different
                //stm32l475：ch = huart1.Instance->RDR
                //stm32f429：ch = huart1.Instance->DR
                ch = huart3.Instance->DR & 0xff;
        }
        else
        {
                if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_ORE) != RESET)
                {
                        __HAL_UART_CLEAR_OREFLAG(&huart3);
                }
                rt_thread_mdelay(10);
        }
        return ch;
}

//static void atcmd(int argc, char**argv)
//{
//    if (argc < 2)
//    {
//        rt_kprintf("Please input'atcmd <server|client>'\n");
//        return;
//    }

//    if (!rt_strcmp(argv[1], "server"))
//    {
//        rt_kprintf("AT server!\n");
//    }
//    else if (!rt_strcmp(argv[1], "client"))
//    {
//        rt_kprintf("AT client!\n");
//    }
//    else
//    {
//        rt_kprintf("Please input'atcmd <server|client>'\n");
//    }
//}

//MSH_CMD_EXPORT(atcmd, atcmd sample: atcmd <server|client>);



#if UART1_HW_AVAILABLE
void USART1_IRQHandler(void)
{
    rt_interrupt_enter();

    uint8_t     u8Data = 0;

#if (UART1_USE_DMA)
    uint16_t rx_len = 0;
    if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!= RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_DMAStop(&huart1);

        rx_len = __HAL_DMA_GET_COUNTER(&hdma_uart1_rx);
        rx_len = UART_DMA_BUF_SIZE - rx_len;

        ring_buf_put(&tRingBuf_uart1_rx, aDMABuf_uart1_rx, rx_len);

        HAL_UART_Receive_DMA(&huart1,aDMABuf_uart1_rx,UART_DMA_BUF_SIZE);

        rt_sem_release(&rtSemUart1Rx);
    }
#else
    if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!= RESET)
    {
        u8Data = USART1->DR & 0xff;
        ring_buf_put(&tRingBuf_uart1_rx, &u8Data, 1);
        //rt_sem_release(&rtSemUart1Rx);

    }
    if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!= RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        rt_sem_release(&rtSemUart1Rx);
    }
#endif
    if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)!= RESET)
    {
        if(ring_buf_get(&tRingBuf_uart1_tx, &u8Data, 1))
        {
            USART1->DR = u8Data;
        }
        else
        {
            __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
        }
    }

    HAL_UART_IRQHandler(&huart1);

    rt_interrupt_leave();
}
#endif

#if (UART1_USE_DMA)
void DMA1_Channel5_IRQHandler(void)
{
    rt_interrupt_enter();

  HAL_DMA_IRQHandler(&hdma_uart1_rx);

    rt_interrupt_leave();
}
#endif
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
#if (UART1_USE_DMA)
        ring_buf_put(&tRingBuf_uart1_rx, aDMABuf_uart1_rx, UART_DMA_BUF_SIZE);
        rt_sem_release(&rtSemUart1Rx);
#endif
    }
    else if(huart->Instance==USART2)
    {
#if (UART2_USE_DMA)
                ring_buf_put(&tRingBuf_uart2_rx, aDMABuf_uart2_rx, UART_DMA_BUF_SIZE);
        rt_sem_release(&rtSemUart2Rx);
#endif
    }
    else if(huart->Instance==USART3)
    {
#if (UART3_USE_DMA)
                ring_buf_put(&tRingBuf_uart3_rx, aDMABuf_uart3_rx, UART_DMA_BUF_SIZE);
        rt_sem_release(&rtSemUart3Rx);
#endif
    }
}
#if UART1_HW_AVAILABLE

uint32_t uart1_send(const void *buf ,uint32_t len)
{
    uint32_t ret;

    ret = ring_buf_put(&tRingBuf_uart1_tx, (uint8_t *)buf, len);

        if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE) != RESET)
        {
            __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
        }

    return ret;
}

void uart1_putstr(const char *str)
{
    uint32_t  u32Len = 0;

    u32Len = strlen(str);
    char cAA = '\r';

    uart1_send((void *)str, u32Len - 1);
    if(*(str+u32Len - 1) == '\n')
    {
        uart1_send(&cAA, 1);
    }
    uart1_send((void *)(str + u32Len - 1), 1);
}
uint32_t uart1_buflen(void)
{
    return ring_buf_len(&tRingBuf_uart1_rx);
}
uint32_t uart1_read(void *buf, unsigned int len)
{
    return ring_buf_get(&tRingBuf_uart1_rx, (uint8_t *)buf, len);
}

uint32_t uart1_blocking_send(void *buf ,uint32_t len)
{
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)== RESET);
    HAL_UART_Transmit(&huart1,(uint8_t *)buf,len,0xffffffff);
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)== RESET);
    return 0;
}
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

}

#endif
/* USER CODE BEGIN 4 */
void Sys_init(void)
{
    HAL_Init();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
