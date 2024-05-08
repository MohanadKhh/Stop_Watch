/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE BEGIN PV */
Matrix_Keypad_t keypad = {
		.Columns = 4,
		.IntputPort = GPIOB,
		.InputStartingPin = 6,
		.Rows = 4,
		.OutputPort = GPIOB,
		.OutputStartingPin = 12
};

Alcd_t lcd = {
		.Data_GPIO = GPIOA ,
		.Data_GPIO_Start_Pin = 0 ,
		.RS_GPIO = GPIOA,
		.RS_GPIO_Pin = GPIO_PIN_4,
		.EN_GPIO = GPIOA,
		.EN_GPIO_Pin = GPIO_PIN_5
};

char str [16];
uint8_t HH,MM,SS,mS = 0;
uint8_t len;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  __HAL_RCC_GPIOB_CLK_ENABLE();

  Keypad_Init(&keypad);
  Alcd_Init(&lcd, 2, 16);
  Alcd_PutAt(&lcd, 0, 0, "Hi, Stop Watch..");
  HAL_Delay(2000);
  Alcd_PutAt(&lcd, 1, 0, "let's begin...");
  HAL_Delay(2000);
  Alcd_PutAt(&lcd, 0, 0, "_____Rules_____");
  Alcd_PutAt(&lcd, 1, 0, "Press B to Stop");
  HAL_Delay(3000);
  Alcd_PutAt(&lcd, 1, 0, "Press C to Reset");
  HAL_Delay(3000);
  Alcd_PutAt(&lcd, 1, 0, "Press A to Start");
  HAL_Delay(3000);
  Alcd_PutAt(&lcd, 0, 0, "let's start.....");

  uint8_t flag_start = 0;
  uint8_t flag_stop = 0;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* USER CODE END WHILE */

      // Refresh the keypad to get the latest input
      Keypad_Refresh(&keypad);

      // Check if the 'Start' button (Button A) is pressed
      if (Keypad_Get_Key(&keypad, 3))
      {
          // If the stopwatch is not running and not stopped, clear the display and reset time variables
          if (flag_start == 0 && flag_stop == 0)
          {
              Alcd_Clear(&lcd);
              HH = MM = SS = mS = 0;
          }
          // Set the flag indicating the stopwatch has started
          flag_start = 1;

          // Format the time string and display it on the LCD
          len = sprintf(str, "%02d:%02d:%02d.%02d", HH, MM, SS, mS);
          Alcd_PutAt_n(&lcd, 0, 0, str, len);
          Alcd_PutAt(&lcd, 1, 0, "B Stop - C Reset");

          // Update time variables
          mS++;
          if (mS == 100)	//update every 1 sec
          {
              SS++;
              mS = 0;
          }
          if (SS == 60)		//update every 1 min
          {
              MM++;
              SS = 0;
          }
          if (MM == 60)		//update every 1 hr
          {
              HH++;
              MM = 0;
          }
          if (HH == 24)		//update every 1 day
          {
              MM = 0;
              HH = 0;
              SS = 0;
          }
      }

      // Check if the stopwatch is already running
      if (flag_start == 1)
      {
          // Format the time string and display it on the LCD
          len = sprintf(str, "%02d:%02d:%02d.%02d", HH, MM, SS, mS);
          Alcd_PutAt_n(&lcd, 0, 0, str, len);

          // Update time variables
          mS++;
          if (mS == 100)	//update every 1 sec
          {
              SS++;
              mS = 0;
          }
          if (SS == 60)		//update every 1 min
          {
              MM++;
              SS = 0;
          }
          if (MM == 60)		//update every 1 hr
          {
              HH++;
              MM = 0;
          }
          if (HH == 24)		//update every 1 day
          {
              MM = 0;
              HH = 0;
              SS = 0;
          }
      }

      // Check if the 'Stop' button (Button B) is pressed
      if (Keypad_Get_Key(&keypad, 7))
      {
          // Format the time string and display it on the LCD
          len = sprintf(str, "%02d:%02d:%02d.%02d", HH, MM, SS, mS);
          Alcd_PutAt_n(&lcd, 0, 0, str, len);

          // Display a message indicating the user can start the stopwatch again or reset it
          Alcd_PutAt(&lcd, 1, 0, "A Play - C Reset");

          // Reset the 'Start' flag and set the 'Stop' flag
          flag_start = 0;
          flag_stop = 1;
      }

      // Check if the 'Reset' button (Button C) is pressed
      if (Keypad_Get_Key(&keypad, 11))
      {
          // Reset all time variables to zero
          HH = MM = SS = mS = 0;

          // Format the time string and display it on the LCD
          len = sprintf(str, "%02d:%02d:%02d.%02d", HH, MM, SS, mS);
          Alcd_PutAt_n(&lcd, 0, 0, str, len);

          // Display a message indicating the user can start the stopwatch or stop it
          Alcd_PutAt(&lcd, 1, 0, "A Start - B Stop");

          // Reset both 'Start' and 'Stop' flags
          flag_start = flag_stop = 0;
      }

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
