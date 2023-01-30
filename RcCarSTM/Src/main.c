/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
TIM_HandleTypeDef htim2;
/* USER CODE BEGIN PV */
uint8_t leftDriveConfig = 0x00;
uint8_t rightDriveConfig = 0x80;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  // инициализация UART1
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // включаем тактирование UART1

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // разрешаем тактирование порта GPIOA

  // настройка вывода PA9 (TX1) на режим альтернативной функции с активным выходом
  // Биты CNF = 10, ,биты MODE = X1
  GPIOA->CRH &= (~GPIO_CRH_CNF9_0);
  GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9);

  // настройка вывода PA10 (RX1) на режим входа с подтягивающим резистором
  // Биты CNF = 10, ,биты MODE = 00, ODR = 1
  GPIOA->CRH &= (~GPIO_CRH_CNF10_0);
  GPIOA->CRH |= GPIO_CRH_CNF10_1;
  GPIOA->CRH &= (~(GPIO_CRH_MODE10));
  GPIOA->BSRR |= GPIO_ODR_ODR10;

  // конфигурация UART1
  USART1->CR1 = USART_CR1_UE; // разрешаем USART1, сбрасываем остальные биты

  USART1->BRR = 1875; // скорость 38400 бод

  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // разрешаем приемник, передатчик и прерывание по приему
  USART1->CR2 = 0;
  USART1->CR3 = 0;

  NVIC_EnableIRQ (USART1_IRQn);

  // конфигурация таймера 1
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);

  LL_TIM_EnableCounter(TIM1);  // разрешить счетчик 1
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);  // прямой счет
  LL_TIM_EnableUpdateEvent(TIM1);  // разрешить перезагрузку

  LL_TIM_SetPrescaler(TIM1, 719);  // предделитель = 719 - 10 мкс частота импульсов
  LL_TIM_SetAutoReload(TIM1, 9999);  // перезагрузка при 9999 - т.е. 10 раз в секунду
  LL_TIM_SetRepetitionCounter(TIM1, 0);  // без повтора

  LL_TIM_EnableIT_UPDATE(TIM1);  // разрешение прерывания

  NVIC_EnableIRQ (TIM1_UP_IRQn);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  uint8_t direction = 0;
  uint8_t motor_direction = 0;
  int32_t CH1_DC = 0;

  while (1)
  {
    /* USER CODE END WHILE */
	  TIM2->CCR1 = CH1_DC;
	  TIM2->CCR2 = CH1_DC;

	  if (motor_direction == 0)
	  {
		  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
		  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
		  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);
		  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	  }
	  else
	  {
		  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
		  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
		  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
		  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
	  }

	  //65535 = 3 * 5 * 17 * 257

	  if(direction == 0)
	  {
		  if (CH1_DC < 65535)
		  {
			  CH1_DC += 85; //5 * 17
		  }
		  else
		  {
			  direction = 1;
		  }
	  }
	  else
	  {
		  if (CH1_DC > 0)
		  {
			  CH1_DC -= 85; //5 * 17
		  }
		  else
		  {
			  direction = 0;

			  if(motor_direction == 0)
			  {
				  motor_direction = 1;
			  }
			  else
			  {
				  motor_direction = 0;
			  }
		  }
	  }

	  HAL_Delay(1);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);

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
