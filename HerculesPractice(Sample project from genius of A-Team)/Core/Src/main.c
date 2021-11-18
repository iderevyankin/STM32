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
#include "string.h"
#include "stdio.h"
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

/* USER CODE BEGIN PV */
uint8_t pc_data[3];
uint8_t command_from_pc = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)	
{
	if((pc_data[0] == 0x7E) && (pc_data[2] == 0xAB))
		command_from_pc = pc_data[1];
	HAL_UART_Receive_IT(&huart1, pc_data, 3);
}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	char buf[50];
	strcpy(buf, "Ready to work!\n");
	HAL_UART_Receive_IT(&huart1, pc_data, 3);


	uint16_t time = 0;
	uint8_t is_on = 0;
	uint8_t push_counter = 0;
	uint8_t button_val = 0;
	uint8_t num = 0;
	
	uint16_t LightOn[100];
	uint16_t LightOff[100];
	
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100); 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(command_from_pc == 0xA0)
		{
			num = 0;
			strcpy(buf, "Select mode.\n");
			HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100); 
			push_counter = 0;
		}	
		
		if(command_from_pc == 0xA1)
		{
			push_counter += 1;
			time = 0;
		}
	

		command_from_pc = 0;
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		button_val = HAL_GPIO_ReadPin(BigBlueButton_GPIO_Port, BigBlueButton_Pin);
		

		if(num == 0)
		{		
			
			if(button_val == 1)
			{
				if(!is_on)
				{
					push_counter += 1;
					is_on = 1;
				}
				time = 0;
			}
			else
			{
				is_on = 0;
				time += 1;
			}
		}
		
		if(time == 500 && num == 0)
		{
			if(push_counter == 1)
			{
				num = 1;
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
				
				strcpy(buf, "Button mode is selected.\n");
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100); 
			}
			if(push_counter == 2)
			{
				num = 2;
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				
				strcpy(buf, "PushCounter mode is selected.\n");
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
			}
			if(push_counter > 2)
			{
				num = 3;
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
				
				strcpy(buf, "SequenceRepeater mode is selected.\n");
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
			}
			time = 0;
			push_counter = 0;
			is_on = 0;
		}
		
		
		if(num == 1) 
		{
			
			if(button_val == 1)
			{
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
				if(is_on == 0)
				{
					strcpy(buf, "On.");
					HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
				}					
				is_on = 1;
			}
			if(button_val == 0)
			{
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
				if(is_on == 1)
				{
					strcpy(buf, " Off.\n");
					HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
				}	
				is_on = 0;
			}
		}
		
		if(num == 2)
		{			
			if(button_val == 1 || command_from_pc == 0xA1)
			{
				
				if(!is_on && button_val == 1)
				{
					push_counter += 1;
					is_on = 1;
				}
				
				if(command_from_pc == 0xA1)
					push_counter += 1;
				
				time = 0;
			}
			else
			{
				is_on = 0;
				time += 1;
			}
		
			if(time == 1000 && push_counter != 0)
			{
				sprintf(buf, "There were %d pushes.\n", push_counter);
				HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100); 
				
				for(int i = 0; i < push_counter; i = i + 1)
				{
					HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
					HAL_Delay(100);
					HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
					HAL_Delay(150);
				}
				push_counter = 0;
				time = 0;
			}
		}
		
		if(num == 3) 
		{
			if(button_val == 1)
			{
				if(!is_on)
				{
					push_counter+= 1;
					is_on = 1;
				}
				LightOn[push_counter - 1] += 2;
				time = 0;
			}
			else
			{
				is_on = 0;
				if(push_counter > 0)
				LightOff[push_counter - 1] += 2;
				time += 1;
			}
		
			if(time == 1000)
			{
			
				for(int i = 0; i < push_counter; i = i + 1)
				{
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
					HAL_Delay(LightOn[i]);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
					if(i != push_counter - 1)
					HAL_Delay(LightOff[i]);
				
					LightOn[i] = 0;
					LightOff[i] = 0;
				}		
				push_counter = 0;
				time = 0;
				}	
			}		
		HAL_Delay(1);
		
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BigBlueButton_Pin */
  GPIO_InitStruct.Pin = BigBlueButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BigBlueButton_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
