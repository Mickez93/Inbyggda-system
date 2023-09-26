/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile uint32_t Blink_delay = 0;
volatile uint8_t chars_read = 0;
char btn1_txt[255];
uint32_t std_task = 0;
uint32_t btn1_task = 0;
uint32_t btn2_task = 0;
osStatus_t os_status;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for btn1task */
osThreadId_t btn1taskHandle;
const osThreadAttr_t btn1task_attributes = {
  .name = "btn1task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for btn2task */
osThreadId_t btn2taskHandle;
const osThreadAttr_t btn2task_attributes = {
  .name = "btn2task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MUTEX_UART_2 */
osMutexId_t MUTEX_UART_2Handle;
osStaticMutexDef_t MUTEX_UART_2ControlBlock;
const osMutexAttr_t MUTEX_UART_2_attributes = {
  .name = "MUTEX_UART_2",
  .cb_mem = &MUTEX_UART_2ControlBlock,
  .cb_size = sizeof(MUTEX_UART_2ControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Startbtn1task(void *argument);
void Startbtn2task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of MUTEX_UART_2 */
  MUTEX_UART_2Handle = osMutexNew(&MUTEX_UART_2_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of btn1task */
  btn1taskHandle = osThreadNew(Startbtn1task, NULL, &btn1task_attributes);

  /* creation of btn2task */
  btn2taskHandle = osThreadNew(Startbtn2task, NULL, &btn2task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  
  /* Infinite loop */
  for(;;)
  {
    std_task++;
    if(Blink_delay == 0)
    {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,RESET);
    }
    else
    {
      if(osKernelGetTickCount()-tick > Blink_delay)
      {
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
        tick = osKernelGetTickCount();
      }
    }
     
     if(read_to_buf_1)
    {
      for(int i = 0; i < myDMA_buffer_length/2 ; i++)
      {
        input_buffer[i] = ((float32_t)adc_buffer_1[i]-2048.0);
    
      }
    }
    //Buffer 2 ready to process
    if(read_to_buf_2)
    {
      for(int i = 0; i < myDMA_buffer_length/2 ; i++)
      {
        input_buffer[myDMA_buffer_length/2 + i] = ((float32_t)adc_buffer_2[i]-2048.0);
      }
      
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(ready_to_process)
    { 
      arm_rms_f32(input_buffer,myDMA_buffer_length,&RMS_VLU);
   
      uint32_t print = (uint32_t) RMS_VLU;
      chars_read = sprintf(btn1_txt, " RMS Value = %d\n\r",print);

      os_status = osMutexAcquire(MUTEX_UART_2Handle,1000);
      if(os_status==osOK)
      {
        HAL_UART_Transmit(&huart2,btn1_txt,chars_read,1000);
        osMutexRelease(MUTEX_UART_2Handle);
      }
      else
      {
        printf("fail");
      }
      
      ready_to_process = 0;

      if(print > 999)
      {
        chars_read = sprintf(btn1_txt, " Warning sound level too high!\n\r",print);


        os_status = osMutexAcquire(MUTEX_UART_2Handle,1000);
        if(os_status==osOK)
        {
          HAL_UART_Transmit(&huart2,btn1_txt,chars_read,1000);
          osMutexRelease(MUTEX_UART_2Handle);
        }
        else
        {
          printf("fail");
        }
        
        
      }
    }
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Startbtn1task */
/**
* @brief Function implementing the btn1task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startbtn1task */
void Startbtn1task(void *argument)
{
  /* USER CODE BEGIN Startbtn1task */
  /* Infinite loop */
  for(;;)
  { 
    btn1_task++;
    if(debounce1 == 0)
    {
      if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == RESET)
      {
        
        debounce1 = 2;
        btn1_flg = 1;
        Blink_delay += 75;
        chars_read = sprintf(btn1_txt, " User_PushButton-2 was pressed! Current Blink_Delay =%d ",Blink_delay);
        //SEND WITH MUTEX
        os_status = osMutexAcquire(MUTEX_UART_2Handle,1000);
        if(os_status==osOK)
        {
          HAL_UART_Transmit(&huart2,btn1_txt,chars_read,1000);
          osMutexRelease(MUTEX_UART_2Handle);
        }
        else
        {
          printf("fail");
        }
        

        HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_4);
      }
    }
    
    osDelay(1);
  }
  /* USER CODE END Startbtn1task */
}

/* USER CODE BEGIN Header_Startbtn2task */
/**
* @brief Function implementing the btn2task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startbtn2task */
void Startbtn2task(void *argument)
{
  /* USER CODE BEGIN Startbtn2task */
  /* Infinite loop */
  for(;;)
  { 
    btn2_task++;
    if(debounce2 == 0)
    {
      if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) == RESET)
      {
      
        debounce2 = 2;
        btn2_flg = 1;
        HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_4);

         if(Blink_delay <= 75)
         {
            /////Slå av
            Blink_delay = 0;
            chars_read = sprintf(btn1_txt, " User_PushButton-1 was pressed! Current Blink_Delay =%d ",Blink_delay);
            
            os_status = osMutexAcquire(MUTEX_UART_2Handle,1000);
            if(os_status==osOK)
            {
              HAL_UART_Transmit(&huart2,btn1_txt,chars_read,1000);
              osMutexRelease(MUTEX_UART_2Handle);
            }
            else
            {
              printf("fail");
            }

          }
        //Öka delay
        else
        {
          ////250 ~= 75 ms toggle speed
          Blink_delay -= 75;
          chars_read = sprintf(btn1_txt, " User_PushButton-1 was pressed! Current Blink_Delay =%d ",Blink_delay);
          
          os_status = osMutexAcquire(MUTEX_UART_2Handle,1000);
          if(os_status==osOK)
          {
            HAL_UART_Transmit(&huart2,btn1_txt,chars_read,1000);
            osMutexRelease(MUTEX_UART_2Handle);
          }
          else
          {
            printf("fail");
          }
      

        } 
      }
    }
    osDelay(1);
  }
  /* USER CODE END Startbtn2task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

