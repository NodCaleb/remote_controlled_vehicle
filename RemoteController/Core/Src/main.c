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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "fonts.h"
#include "test.h"
#include "bitmap.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define read_adc_period 100
#define update_screen_period 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
//Counters for timed actions, initial values - time offsets
uint16_t read_adc_counter = 0;
uint16_t update_screen_counter = 50;

uint8_t str[32];
uint32_t adc_value[2];

uint16_t left_throttle = 0;
uint16_t right_throttle = 0;
uint8_t left_transmit = 0;
uint8_t right_transmit = 0;
uint8_t transmit_switch = 0; //Left or right throttle
SSD1306_COLOR_t draw_color = SSD1306_COLOR_BLACK;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

void Read_ADC(void);
void Calculate_Throttle(void);
void Update_Screen(void);
void Transmit_Throttle(uint8_t throttle);
void Select_ADC_Channel(uint8_t channel);

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  USART1->DR = 0x2E;

  HAL_TIM_Base_Start_IT(&htim1); // start timer
  HAL_ADCEx_Calibration_Start(&hadc1); //Calibrate ADC

  SSD1306_Init (); // initialize the display

  SSD1306_DrawBitmap(43, 17 , car, 48, 35, 1);
  
  SSD1306_UpdateScreen();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
//  sConfig.Channel = ADC_CHANNEL_0;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }

  /** Configure Regular Channel
  */
//  sConfig.Channel = ADC_CHANNEL_1;
//  sConfig.Rank = ADC_REGULAR_RANK_2;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 719;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  huart1.Init.BaudRate = 38400;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

//Process timer interrupts
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim == &htim1){ //Check if it's TIM1
		
    read_adc_counter++;
    update_screen_counter++;
		
    if (read_adc_counter == read_adc_period){
			read_adc_counter = 0;
			Read_ADC();
      Calculate_Throttle();
		}
		
    if (update_screen_counter == update_screen_period){
			update_screen_counter = 0;
			Update_Screen();
      //Transmit values alternately
      if (transmit_switch == 0)
      {
        transmit_switch = 1;
        Transmit_Throttle(left_transmit);
      }
      else{
        transmit_switch = 0;
        Transmit_Throttle(right_transmit);
      }
		}
	}
}

//Ticks once per second
void Read_ADC(void){

//	USART1->DR = 0x2E; //Send char '.' to UART (to test that UART and timer work)

  for (size_t i = 0; i < 2; i++)
  {
    Select_ADC_Channel(i);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    adc_value[i] = HAL_ADC_GetValue(&hadc1);
  }

}

//Selecting ADC channel
void Select_ADC_Channel(uint8_t channel){

	ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;

  switch (channel)
  {
    case 0:
      sConfig.Channel = ADC_CHANNEL_0;
      break;
    case 1:
      sConfig.Channel = ADC_CHANNEL_1;
      break;
    
    default:
      break;
  }

	
	
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void Calculate_Throttle(void){

  //Calculate values for transmission
  if (adc_value[0] < 2048) //Left forward
  {
    left_transmit = ((2047 - adc_value[0]) / 8) >> 2; //Reduce definition by 2 digits to contain motor index and direction
    if (left_transmit < 16) //Reduce noise
    {
      left_transmit = 0;
    }
    
    left_transmit = left_transmit & 0x7F; //Set motor index
    left_transmit = left_transmit & 0xBF; //Set direction bit
  }
  else{ //Left reverse
    left_transmit = ((adc_value[0] - 2048) / 8) >> 2;
    if (left_transmit < 16) //Reduce noise
    {
      left_transmit = 0;
    }

    left_transmit = left_transmit & 0x7F; //Set motor index
    left_transmit = left_transmit | 0x40; //Set direction bit
  }

  if (adc_value[1] < 2048) //Right forward
  {
    right_transmit = ((2047 - adc_value[1]) / 8) >> 2; //Reduce definition by 2 digits to contain motor index and direction
    if (right_transmit < 16) //Reduce noise
    {
      right_transmit = 0;
    }

    right_transmit = right_transmit | 0x80; //Set motor index
    right_transmit = right_transmit & 0xBF; //Set direction bit
  }
  else{ //Right reverse
    right_transmit = ((adc_value[1] - 2048) / 8) >> 2;
    if (right_transmit < 16) //Reduce noise
    {
      right_transmit = 0;
    }

    right_transmit = right_transmit | 0x80; //Set motor index
    right_transmit = right_transmit | 0x40; //Set direction bit
  }

  //Calculate values to show on screen
  left_throttle = adc_value[0] / 64;
  right_throttle = adc_value[1] / 64;
  // left_throttle = left_transmit ^ 1 << 6; //Toggle 6th bit cause displayed values should be more for forward direction
  // left_throttle = left_throttle >> 1; //Shift right to reduce scale from 128 to 64 for full throttle
  // right_throttle = right_transmit ^ 1 << 6;
  // right_throttle = right_throttle & 0x7F; //Set most significant bit to 0 cause motor indes is not being used on the display
  // right_throttle = right_throttle >> 1;
  
}

void Update_Screen(void){
  
  //Display numeric values (for debug)
  // for (size_t i = 0; i < 2; i++)
  // {
  //   sprintf(&str, "%4d", adc_value[i]);
  //   SSD1306_GotoXY(2, i * 10);
  //   SSD1306_Puts (str, &Font_7x10, 1);
  // }

  // sprintf(&str, "%4d", left_transmit);
  // SSD1306_GotoXY(2, 30);
  // SSD1306_Puts (str, &Font_7x10, 1);

  // sprintf(&str, "%4d", right_transmit);
  // SSD1306_GotoXY(2, 40);
  // SSD1306_Puts (str, &Font_7x10, 1);
  
  //Draw left and right throttle bars (for fancy look)
  for (size_t i = 0; i < 64; i++)
  {
    if (left_throttle < 32)
    {
      if (left_throttle <= i && i <= 32)
      {
        draw_color = SSD1306_COLOR_WHITE;
      }
      else{
        draw_color = SSD1306_COLOR_BLACK;
      }
    }
    else if (left_throttle > 32)
    {
      if (left_throttle >= i && i > 32)
      {
        draw_color = SSD1306_COLOR_WHITE;
      }
      else{
        draw_color = SSD1306_COLOR_BLACK;
      }
    }
    else
    {
      draw_color = SSD1306_COLOR_BLACK;
    }    

    SSD1306_DrawLine(2, i, 16, i, draw_color);

    if (right_throttle < 32)
    {
      if (right_throttle <= i && i <= 32)
      {
        draw_color = SSD1306_COLOR_WHITE;
      }
      else{
        draw_color = SSD1306_COLOR_BLACK;
      }
    }
    else if (right_throttle > 32)
    {
      if (right_throttle >= i && i > 32)
      {
        draw_color = SSD1306_COLOR_WHITE;
      }
      else{
        draw_color = SSD1306_COLOR_BLACK;
      }
    }
    else
    {
      draw_color = SSD1306_COLOR_BLACK;
    }    

    SSD1306_DrawLine(113, i, 129, i, draw_color);
  }
  
  SSD1306_UpdateScreen();

}

void Transmit_Throttle(uint8_t throttle){
  USART1->DR = throttle;
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
