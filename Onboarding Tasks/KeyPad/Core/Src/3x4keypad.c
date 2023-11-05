/*
 * 3x4keypad.c
 *
 *  Created on: Oct 31, 2023
 *      Author: hedge
 */
/*
#include "3x4keypad.h"

char Get_key_char(){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 1){
		return '1';
	}
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 1){
		return '3';
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
}
*/
