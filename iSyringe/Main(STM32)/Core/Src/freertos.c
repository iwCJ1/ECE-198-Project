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
#include "st7735.h"
#include "lzy.h"
#include "firstscreen.h"
#include "internet_connected.h"
#include "internet_error.h"
#include "KeyPad.h"
#include "tim.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for GUI_RENDER */
osThreadId_t GUI_RENDERHandle;
const osThreadAttr_t GUI_RENDER_attributes = {
  .name = "GUI_RENDER",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Buzzer */
osThreadId_t BuzzerHandle;
const osThreadAttr_t Buzzer_attributes = {
  .name = "Buzzer",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for alarmlight */
osThreadId_t alarmlightHandle;
const osThreadAttr_t alarmlight_attributes = {
  .name = "alarmlight",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for wifi */
osThreadId_t wifiHandle;
const osThreadAttr_t wifi_attributes = {
  .name = "wifi",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for emergencystop */
osThreadId_t emergencystopHandle;
const osThreadAttr_t emergencystop_attributes = {
  .name = "emergencystop",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void gui_render(void *argument);
void buzzer(void *argument);
void OrangeAlarmLight(void *argument);
void WiFi(void *argument);
void EmergencyStop(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of GUI_RENDER */
  GUI_RENDERHandle = osThreadNew(gui_render, NULL, &GUI_RENDER_attributes);

  /* creation of Buzzer */
  BuzzerHandle = osThreadNew(buzzer, NULL, &Buzzer_attributes);

  /* creation of alarmlight */
  alarmlightHandle = osThreadNew(OrangeAlarmLight, NULL, &alarmlight_attributes);

  /* creation of wifi */
  wifiHandle = osThreadNew(WiFi, NULL, &wifi_attributes);

  /* creation of emergencystop */
  emergencystopHandle = osThreadNew(EmergencyStop, NULL, &emergencystop_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_gui_render */
/**
  * @brief  Function implementing the GUI_RENDER thread.
  * @param  argument: Not used
  * @retval None
  */

// PUBLIC VARIABLES FOR STATUS
// Alarm
int orange_light = 0;
int buzzer_on = 0;
// TFT topbar
char time = 'Wait for WEB...'; // Time function should update it every 1 min (get time from web)
int icon_charged = 0; // charged -> 1
int icon_internet = 0; // no internet -> 0
int prev_internet = 0;
int battery_bad = 0; // battery ok -> 0
int message = 0; // Messagebox pop out -> 0 = No Error	1 = Done Injection	2 = Emergency Stop
int prevMessageBox = 0;
int screen_number = 0;
int screen_2_input_done = 0;
int injecting = 0;
char digit_1 = '_';
char digit_2 = '_';
int speed = 0;
int finish_inject = 0;
int emergency = 0;


int char_to_int(char a){
	if(a == '1'){
		return 1;
	}
	if(a == '2'){
		return 2;
	}
	if(a == '3'){
		return 3;
	}
	if(a == '4'){
		return 4;
	}
	if(a == '5'){
		return 5;
	}
	if(a == '6'){
		return 6;
	}
	if(a == '7'){
		return 7;
	}
	if(a == '8'){
		return 8;
	}
	if(a == '9'){
		return 9;
	}
	if(a == '0'){
		return 0;
	}
}

// DRAWING FUNCTIONS
void first_screen(){ // Initial screen
	ST7735_Init();
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawImage(0,160,128,160,gImage_firstscreen);
}

void top_bar(){ // Top bar drawing
	if(icon_internet == 1 && prev_internet != icon_internet){
		prev_internet = icon_internet;
		ST7735_DrawImage(113,15,15,15, gImage_internet_connected);
	}else{
		if(prev_internet != icon_internet){
			prev_internet = icon_internet;
			ST7735_DrawImage(113,15,15,15, gImage_internet_error);
		}
	}
}

void screen_0(){
	ST7735_DrawString(20, 40, "CID# 14", ST7735_WHITE, ST7735_BLUE, 2, 12);
	ST7735_DrawString(10, 60, "iSyringe", ST7735_WHITE, ST7735_BLUE, 2, 13);
	if(injecting == 1){
		ST7735_DrawString(5, 100, "Injecting...", ST7735_MAGENTA, ST7735_CYAN, 2, 11);
		ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
	}

}

void screen_1(){
	ST7735_DrawString(10, 22, "Injection", ST7735_WHITE, ST7735_ORANGE, 2, 12);
	ST7735_DrawString(10, 42, "Target Speed:", ST7735_WHITE, ST7735_BLACK, 1, 8);
	ST7735_DrawCharS(10, 54, digit_1, ST7735_WHITE, ST7735_BLACK, 2);
	ST7735_DrawCharS(25, 54, digit_2, ST7735_WHITE, ST7735_BLACK, 2);
	ST7735_DrawString(48, 54, "ml/min", ST7735_WHITE, ST7735_BLACK, 2, 12);
	ST7735_DrawString(10, 70, "Syringe Volume:", ST7735_WHITE, ST7735_BLACK, 1, 8);
	ST7735_DrawString(10, 82, "10 ml", ST7735_WHITE, ST7735_BLACK, 2, 12);
	ST7735_DrawString(10, 98, "Syringe Length:", ST7735_WHITE, ST7735_BLACK, 1, 8);
	ST7735_DrawString(10, 110, "10 cm", ST7735_WHITE, ST7735_BLACK, 2, 12);


	//HAL_Delay(100);
}

void bottom_bar(){ // Bottom bar drawing
	if(screen_number == 0 && message == 0 && injecting != 1){
		ST7735_DrawCharS(2, 142, '*', ST7735_RED, ST7735_WHITE, 2);
		ST7735_DrawString(15, 140, "=menu", ST7735_WHITE, ST7735_BLUE, 2, 13);
	}
	if(screen_number == 1 && screen_2_input_done == 0 && message == 0){
		ST7735_DrawString(18, 142, "-INPUT-", ST7735_WHITE, ST7735_BLUE, 2, 13);
	}
	if(screen_number == 1 && screen_2_input_done == 1 && message == 0){
		ST7735_DrawCharS(2, 142, '*', ST7735_RED, ST7735_WHITE, 2);
		ST7735_DrawString(15, 140, "=OK", ST7735_WHITE, ST7735_BLUE, 2, 13);
		ST7735_DrawCharS(60, 142, '#', ST7735_RED, ST7735_WHITE, 2);
		ST7735_DrawString(73, 140, "=X", ST7735_WHITE, ST7735_BLUE, 2, 13);
	}

}

void message_box(){ // Draw the box and the message
	if(message == 1 && prevMessageBox != 1){
		ST7735_FillRectangle( 14,  43,  100,  60,  ST7735_WHITE); // MessageBox Body
		ST7735_FillRectangle( 14,  25,  100,  18,  ST7735_GREEN); // MessageBox Top - Finish Injection
		ST7735_DrawString(40, 25, "INFO", ST7735_WHITE, ST7735_GREEN, 2, 12);
		ST7735_DrawString(16, 50, "Inject", ST7735_BLACK, ST7735_WHITE, 2, 12);
		ST7735_DrawString(16, 65, "finished", ST7735_BLACK, ST7735_WHITE, 2, 12);
		ST7735_DrawCharS(2, 142, '*', ST7735_RED, ST7735_WHITE, 2);
		ST7735_DrawString(15, 140, "=OK", ST7735_WHITE, ST7735_BLUE, 2, 13);
		prevMessageBox = message;
		if(KeyPad_WaitForKeyGetChar(0) == '*'){ // 用户确认操作关闭MessageBox
		  message = 0;
			prevMessageBox = 0;
		}
	} else {
		if(message == 2 ){
		ST7735_FillRectangle( 14,  43,  100,  60,  ST7735_WHITE); // MessageBox Body
		ST7735_FillRectangle( 14,  25,  100,  18,  ST7735_MAGENTA); // MessageBox Top - Emergency Stop
		ST7735_DrawString(35, 25, "ERROR", ST7735_WHITE, ST7735_MAGENTA, 2, 12);
		ST7735_DrawString(16, 50, "Emerge-", ST7735_BLACK, ST7735_WHITE, 2, 12);
		ST7735_DrawString(16, 65, "ncy", ST7735_BLACK, ST7735_WHITE, 2, 12);
		ST7735_DrawString(16, 80, "stopped", ST7735_BLACK, ST7735_WHITE, 2, 12);
		ST7735_DrawString(20, 140, "Restart to clear", ST7735_RED, ST7735_BLUE, 2, 12);
		prevMessageBox = message;
	} else {
		if(message == 3 && prevMessageBox != 3){
			ST7735_FillRectangle( 14,  43,  100,  60,  ST7735_WHITE); // MessageBox Body
					ST7735_FillRectangle( 14,  25,  100,  18,  ST7735_MAGENTA); // MessageBox Top - Emergency Stop
					ST7735_DrawString(35, 25, "Remind", ST7735_WHITE, ST7735_MAGENTA, 2, 12);
					ST7735_DrawString(16, 50, "Time to", ST7735_BLACK, ST7735_WHITE, 2, 12);
					ST7735_DrawString(16, 65, "eat drug", ST7735_BLACK, ST7735_WHITE, 2, 12);
					ST7735_DrawCharS(2, 142, '*', ST7735_RED, ST7735_WHITE, 2);
					ST7735_DrawString(15, 140, "=OK", ST7735_WHITE, ST7735_BLUE, 2, 13);
					prevMessageBox = message;
					if(KeyPad_WaitForKeyGetChar(0) == '*'){ // 用户确认操作关闭MessageBox
						  message = 0;
						  prevMessageBox = 0;
					}
		}
	}

	}
}

#define DIR_PIN GPIO_PIN_1
#define DIR_PORT GPIOA
#define STEP_PIN GPIO_PIN_4
#define STEP_PORT GPIOA

void microDelay (uint16_t delay)
{
	/*
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  while (__HAL_TIM_GET_COUNTER(&htim3) < delay);
  */
	HAL_Delay(delay);
}

void step (int steps, uint8_t direction, uint16_t delay)
{
  int x;
  if (direction == 0)
    HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
  for(x=0; x<steps && emergency != 1; x=x+1)
  {
    HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
    microDelay(delay);
    HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
    microDelay(delay);
  }
}

void StepperMotor(int speed){
	  int v = speed; // User Input
	  int V = 10;
	  int L = 10;
	  speed = (v*L)/(V*0.25);
	  ST7735_FillRectangle( 0,  19,  128,  122,  ST7735_BLACK); // Main Window
	  				  			ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
	  	  	  screen_0();
		       step(3200, 1, speed); // 推进
		        osDelay(100);
		        screen_number =0;
		        injecting = 0;
		        finish_inject = 1;
		        orange_light = 1;
		        message = 1;
		        buzzer_on = 1;

		        message_box();
		        //buzzer_on = 1;
		        ST7735_FillRectangle( 0,  19,  128,  122,  ST7735_BLACK); // Main Window
		        	  				  			ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
		        screen_0();
		        digit_1 = '_';
		        digit_2 = '_';
		      step(3200, 0, 1); // 回�??
		      bottom_bar();

}

void StepperInit(){
	step(1600, 1, 1); // 初始化：向前归位
	step(3200, 0, 1); // 初始化：向后归位
}
/* USER CODE END Header_gui_render */
void gui_render(void *argument)
{
  /* USER CODE BEGIN gui_render */
	first_screen();
	KeyPad_Init();
	StepperInit();
	//HAL_Delay(2000);
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_FillRectangle( 0,  0,  128,  18,  ST7735_BLUE); // Top
	ST7735_DrawImage(113,15,15,15, gImage_internet_error);
	ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom

  /* Infinite loop */

  for(;;)
  {
	  if(message != 0){ // MessageBox appears -> 阻止main window刷新
		  message_box();
	  }else{
		  if(screen_number == 0){ // Screen #0
			  screen_0();
			  if(KeyPad_WaitForKeyGetChar(500) == '*' && injecting != 1){ // 用户切换到第二屏
			    screen_number = 1;
				ST7735_FillRectangle( 0,  19,  128,  122,  ST7735_BLACK); // Main Window
				ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
			  }
		  }
		  if(screen_number == 1){ // Screen #1
			  screen_1();
			  bottom_bar();
			  if(screen_2_input_done != 1){
				  digit_1 = KeyPad_WaitForKeyGetChar(0);
				  screen_1();
				  digit_2 = KeyPad_WaitForKeyGetChar(0);
				  screen_1();
				  ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
				  screen_2_input_done = 1;
			  }
			  if(screen_2_input_done == 1){
				  if(KeyPad_WaitForKeyGetChar(0) == '*'){ // 用户确定�????????�????????

					  	  //ST7735_FillRectangle( 0,  19,  128,  122,  ST7735_BLACK); // Main Window
					  		//ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom

						  	  speed = char_to_int(digit_1)*10 + char_to_int(digit_2);
						  	  injecting = 1;
						  	screen_number = 0;
						  	StepperMotor(speed);
				  	}
				  if(KeyPad_WaitForKeyGetChar(0) == '#'){ // 用户取消
				  			digit_1 = '_';
				  			digit_2 = '_';
				  			screen_2_input_done = 0;
				  			ST7735_FillRectangle( 0,  19,  128,  122,  ST7735_BLACK); // Main Window
				  			ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
				  			screen_number = 0;

				  	}
			  }
		  }
	  }
	  top_bar();
	  bottom_bar();
	  //HAL_Delay(1000);
    osDelay(1);
  }
  /* USER CODE END gui_render */
}

/* USER CODE BEGIN Header_buzzer */
/**
* @brief Function implementing the Buzzer thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_buzzer */
void buzzer(void *argument)
{
  /* USER CODE BEGIN buzzer */
  /* Infinite loop */
  for(;;)
  {
	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0){ // MUTE // �?????????�?????????查：按下高电�????????? or 按下低电�?????????
		  buzzer_on =0;
	  }
	  if(buzzer_on == 1){
		  for(int i = 0; i <=10; i++){
			  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);;
			  HAL_Delay(500);
		  }
		  buzzer_on = 0;
	  }
	  else{
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  }
    osDelay(1);
  }
  /* USER CODE END buzzer */
}

/* USER CODE BEGIN Header_OrangeAlarmLight */
/**
* @brief Function implementing the alarmlight thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OrangeAlarmLight */
void OrangeAlarmLight(void *argument)
{
  /* USER CODE BEGIN OrangeAlarmLight */
  /* Infinite loop */
  for(;;)
  {
	  if(orange_light == 1){
	  		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);;
	  		  HAL_Delay(500);
	  	  }
	  if((orange_light == 1 && message == 1) || (orange_light == 1 && message == 3)){
		  	  for(int i = 0; i <= 10; i++){
		  		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);;
		  		  HAL_Delay(500);
		  	  }
		  	  orange_light = 0;
	  	  }
	  else{
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	  }
    osDelay(1);
  }
  /* USER CODE END OrangeAlarmLight */
}

/* USER CODE BEGIN Header_WiFi */
/**
* @brief Function implementing the wifi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WiFi */
void WiFi(void *argument)
{
  /* USER CODE BEGIN WiFi */
  /* Infinite loop */
  for(;;)
  {
	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1){ //WEB_OK
		  icon_internet = 1;
	  	  }
	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == 1 && emergency != 1){ //WEB_EAT_DRUG
		  orange_light = 1;
	  		  message = 3;
	  		buzzer_on = 1;
	  	  }
	  if(emergency == 1){ //Emergency
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
	  }
	  if(finish_inject == 1){
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 1);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, 0);
		  finish_inject = 0;
	  }
    osDelay(1);
  }
  /* USER CODE END WiFi */
}

/* USER CODE BEGIN Header_EmergencyStop */
/**
* @brief Function implementing the emergencystop thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EmergencyStop */
void EmergencyStop(void *argument)
{
  /* USER CODE BEGIN EmergencyStop */
  /* Infinite loop */
  for(;;)
  {
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1){
		  emergency = 1;
	  }
	  if(emergency == 1){ //Emergency Stop
	  		  buzzer_on =1;
	  		orange_light = 1;
	  		message = 2;
	  		//ST7735_FillRectangle( 0,  140,  128,  20,  ST7735_BLUE); // Bottom
	  		message_box();
	  		bottom_bar();
	  	  }
    osDelay(1);
  }
  /* USER CODE END EmergencyStop */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

