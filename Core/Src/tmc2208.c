/*
 * tmc2208.c
 *
 *  Created on: Jun 12, 2021
 *      Author: Vitech-UA
 */

#include "tmc2208.h"
#include "stm32f1xx_hal.h"
#include "main.h"


volatile uint32_t step = 0;
uint8_t divider;

void motorDisable(void) {
	HAL_GPIO_WritePin(En_GPIO_Port, En_Pin, GPIO_PIN_SET);
}
void motorEnable(void) {
	HAL_GPIO_WritePin(En_GPIO_Port, En_Pin, GPIO_PIN_RESET);
}
void motorSetDir(uint8_t dir) {
	if (dir == 0)
		HAL_GPIO_WritePin(Dir_GPIO_Port, Dir_Pin, GPIO_PIN_SET);
	if (dir >= 1)
		HAL_GPIO_WritePin(Dir_GPIO_Port, Dir_Pin, GPIO_PIN_RESET);
}
void motorSetDiv(STEP_DIV div) {
	switch (div) {
	case STEP_DIV_BY_2:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		divider = 2;
		break;
	case STEP_DIV_BY_4:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		divider = 4;
		break;
	case STEP_DIV_BY_8:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		divider = 8;
		break;
	case STEP_DIV_BY_16:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		divider = 16;
		break;
	default:
		break;
	}
}
void motorSetFreq(uint16_t freq){
	if(freq <=0)
		freq = 1000;
      TIM1->PSC = (int)((24000000/2) / freq) / 2;
}
