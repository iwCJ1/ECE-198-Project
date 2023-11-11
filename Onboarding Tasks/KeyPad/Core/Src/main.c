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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "fonts.h"
#include "lzy.h"
//#include "3x4keypad.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define R1_PORT GPIOC
#define R1_PIN GPIO_PIN_1

#define R2_PORT GPIOA
#define R2_PIN GPIO_PIN_9

#define R3_PORT GPIOA
#define R3_PIN GPIO_PIN_8

#define R4_PORT GPIOA
#define R4_PIN GPIO_PIN_12

#define C1_PORT GPIOC
#define C1_PIN GPIO_PIN_2

#define C2_PORT GPIOC
#define C2_PIN GPIO_PIN_0

#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_11

#include "KeyPad.h"

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
char read_key_char(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1); // Wire 2 //
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3 //
		return '1';
  	}
  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
  		return '3';
  	}
  	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1 //
  	  	return '2';
  	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1); // Wire 7
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3 //
			return '4';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
	  		return '6';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1 //
	  	  	return '5';
	  	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1); // Wire 6
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3 //
			return '7';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
	  		return '9';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1 //
	  	  	return '8';
	  	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1); // Wire 4
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3 //
			return '*';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
	  		return '3';
	  	}
	  	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1 //
	  	  	return '2';
	  	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
}


int read_key_int(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1); // Wire 2
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3
		return 1;
  	}
  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
  		return 3;
  	}
  	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1
  	  	return 2;
  	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1); // Wire 7
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3
		return 4;
	}
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1
		return 5;
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
		return 6;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1); // Wire 6
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3
		return 7;
	}
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1
		return 8;
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
		return 9;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1); // Wire 4
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 1){ // Wire 3
		return 12; // '*' -> Cancel
	}
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 1){ // Wire 1
		return 0;
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){ // Wire 5
		return 14; // '#' -> Enter
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
}
*/
uint8_t key;

char read_keypad (void)
{
	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '1';
	}

	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '2';
	}

	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '3';
	}


	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '4';
	}

	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '5';
	}

	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '6';
	}


	/* Make ROW 3 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '7';
	}

	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '8';
	}

	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '9';
	}



	/* Make ROW 4 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_RESET);  // Pull the R4 High

	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '*';
	}

	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '0';
	}

	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '#';
	}


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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  	  ST7735_FillScreen(ST7735_BLACK);
  	  ST7735_DrawImage(0,160,120,160,gImage_lzy);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  	KeyPad_Init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		//ST7735_DrawCharS(10, 10, read_keypad(), ST7735_BLACK, ST7735_GREEN,4);
		//a *= 10;
		//a += read_key_int();
		//char A;
		//sprintf(A, "%d", a);
		//ST7735_DrawCharS(10, 40, A, ST7735_BLACK, ST7735_GREEN,3);

	  /*
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(1);

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
			ST7735_DrawCharS(10, 10, '1', ST7735_BLACK, ST7735_GREEN,4);

		}
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){

			ST7735_DrawCharS(10, 10, '2', ST7735_BLACK, ST7735_GREEN,4);

		}
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)){

			ST7735_DrawCharS(10, 10, '3', ST7735_BLACK, ST7735_GREEN,4);

		}
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(1);

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
					ST7735_DrawCharS(10, 10, '4', ST7735_BLACK, ST7735_GREEN,4);

				}
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){

					ST7735_DrawCharS(10, 10, '5', ST7735_BLACK, ST7735_GREEN,4);

				}
				if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)){

					ST7735_DrawCharS(10, 10, '6', ST7735_BLACK, ST7735_GREEN,4);

				}

				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);



				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(1);

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
					ST7735_DrawCharS(10, 10, '7', ST7735_BLACK, ST7735_GREEN,4);

				}
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){

					ST7735_DrawCharS(10, 10, '8', ST7735_BLACK, ST7735_GREEN,4);

				}
				if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)){

					ST7735_DrawCharS(10, 10, '9', ST7735_BLACK, ST7735_GREEN,4);

				}

				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(1);

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
							ST7735_DrawCharS(10, 10, '*', ST7735_BLACK, ST7735_GREEN,4);

						}
						if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)){

							ST7735_DrawCharS(10, 10, '0', ST7735_BLACK, ST7735_GREEN,4);

						}
						if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)){

							ST7735_DrawCharS(10, 10, '#', ST7735_BLACK, ST7735_GREEN,4);

						}

						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
										HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
										HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
*/
	  ST7735_DrawCharS(10, 10, KeyPad_WaitForKeyGetChar(500), ST7735_BLACK, ST7735_GREEN,4);


}

  }
  /* USER CODE END 3 */


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
