/*
 * tmc2208.c
 *
 *  Created on: Jun 12, 2021
 *      Author: Vitech-UA
 */

#include "tmc2208.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "main.h"

extern TIM_HandleTypeDef MOTOR_STEP_TIMER;
extern TIM_HandleTypeDef htim1;

volatile uint32_t step = 0;
uint8_t divider;

void motorDisable(void) {
	HAL_GPIO_WritePin(En_GPIO_Port, En_Pin, GPIO_PIN_SET);
}
void motorEnableTMC(void) {
	HAL_GPIO_WritePin(En_GPIO_Port, En_Pin, GPIO_PIN_RESET);
}
void motorSetDir(uint8_t dir) {
	if (dir == 0)
		HAL_GPIO_WritePin(Dir_GPIO_Port, Dir_Pin, GPIO_PIN_SET);
	if (dir >= 1)
		HAL_GPIO_WritePin(Dir_GPIO_Port, Dir_Pin, GPIO_PIN_RESET);
}
void motorSetDiv(STEP_DIV_t div) {
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
void motorSetFreq(uint16_t freq) {
	TIM1->ARR = 1;
	if (freq <= 0)
		freq = 1000;
	TIM1->PSC = (int) ((24000000 / 2) / freq) / 2;
}

void motorState(MOTOR_STATE_t state) {

	switch (state) {
	case MOTOR_RUN:
        TIM1->CR1 |= TIM_CR1_CEN;
		break;
	case MOTOR_BREAK:
		TIM1->CR1 &= ~TIM_CR1_CEN;
		break;
	default:
		break;
	}
}

