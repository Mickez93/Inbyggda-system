/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DMA_buffer 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  uint8_t readBuf[DMA_buffer] = {0};
  uint8_t testBuf[4096] = {0,0,0,0,0,0,0,0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  FATFS FatFs; 	//Fatfs handle
  FIL fil; 		//File handle
  FRESULT fres; //Result after operations
  uint8_t bufferState = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SD_CARD_INIT(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
DWORD free_clusters, free_sectors, total_sectors;
FATFS* getFreeFs;
UINT read_chars = 1;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_FATFS_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t asendDMA[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
  
  
   //HAL_SPI_Transmit_DMA(&hspi1,(uint32_t *)&asendDMA, 2);
  SD_CARD_INIT();
  f_read (&fil, readBuf,DMA_buffer, &read_chars);
  HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_4);
  HAL_DAC_Start_DMA(&hdac1,DAC1_CHANNEL_1,(uint32_t *)readBuf, DMA_buffer,DAC_ALIGN_8B_R);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    if(bufferState == 1)
	{
		
                f_read (&fil, &readBuf[0],DMA_buffer/2, &read_chars);
		bufferState = 0;
	}
	if(bufferState == 2)
	{
		
                f_read (&fil, &readBuf[DMA_buffer/2],DMA_buffer/2, &read_chars);
		bufferState = 0;
	}

 
  

  //Be a tidy kiwi - don't forget to close your file!
  //f_close(&fil);
    
    //HAL_SPI_Transmit(&hspi1,&test,1,HAL_MAX_DELAY);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 9;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void SD_CARD_INIT(void)
{
    
    fres = f_mount(&FatFs, "", 1); //1=mount now
    
    if (fres != FR_OK) {
	printf("f_mount error (%i)\r\n", fres);
    }

    DWORD free_clusters, free_sectors, total_sectors;

    FATFS* getFreeFs;

    fres = f_getfree("", &free_clusters, &getFreeFs);
    if (fres != FR_OK) {
	printf("f_getfree error (%i)\r\n", fres);
      }

  //Formula comes from ChaN's documentation
    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors = free_clusters * getFreeFs->csize;

    printf("SD card stats:\r\n %lu KiB total drive space.\r\n %lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);
    //textfile.txt eller snd.wav
    fres = f_open(&fil, "snd.wav", FA_READ);
    if (fres != FR_OK) {
  	printf("f_open error (%i)\r\n");
  	while(1);
    }
    printf("I was able to open file for reading!\r\n");

  

}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
 bufferState = 1;
}


void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
 bufferState = 2;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
