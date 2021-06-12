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
uint8_t _motorDevider;
MOTOR_STATUS_t _motorStatus;
uint16_t _motorFrequency = 100;

void delayInit(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void delayMs(volatile uint32_t delay) {
	TIM2->PSC = CURRENT_FREQ / 1000 - 1;
	TIM2->ARR = delay;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_OPM;
	while ((TIM2->CR1) & (TIM_CR1_CEN != 0))
		;
}

void delayUs(volatile uint32_t delay) {
	TIM2->PSC = CURRENT_FREQ / 1000000 - 1;
	TIM2->ARR = delay;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_OPM;
	while ((TIM2->CR1) & (TIM_CR1_CEN != 0))
		;
}
void motorDisableTMC(void) {
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
		_motorDevider = 2;
		break;
	case STEP_DIV_BY_4:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		_motorDevider = 4;
		break;
	case STEP_DIV_BY_8:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		_motorDevider = 8;
		break;
	case STEP_DIV_BY_16:
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		_motorDevider = 16;
		break;
	default:
		break;
	}
}
void motorSetFreq(uint16_t freq) {
	_motorFrequency = freq;
	if (freq <= 100)
		freq = 100;
	TIM1->ARR = 1;
	if (freq <= 0) {
		freq = 1000;
	}

	TIM1->PSC = (int) ((24000000 / 2) / freq) / 2;
}

void motorSetState(MOTOR_STATE_t state) {

	switch (state) {
	case MOTOR_RUN:
		_motorStatus = MOTOR_RUNNING;
		TIM1->CR1 |= TIM_CR1_CEN;
		break;
	case MOTOR_BREAK:
		_motorStatus = MOTOR_STOPED;
		TIM1->CR1 &= ~TIM_CR1_CEN;
		break;
	default:
		break;
	}
}

MOTOR_STATUS_t motorGetStatus(void) {
	return _motorStatus;
}

uint16_t motorGetFrequency(void) {
	return _motorFrequency;
}
uint8_t motorGetStepDevider(void) {
	return _motorDevider;
}

void motorRotateByStep(uint32_t stepcount) {

	uint16_t steps = (2 * (stepcount * motorGetStepDevider()));

	if (motorGetStatus() == MOTOR_RUNNING) {
		step = 0;

		while (step < steps) {
		}
		motorSetState(MOTOR_BREAK);
	}
	if (motorGetStatus() == MOTOR_STOPED) {
		step = 0;
		motorSetState(MOTOR_RUN);
		while (step < steps) {
		}
		motorSetState(MOTOR_BREAK);

	}

}

void motorRun(uint16_t freq, uint8_t dir, uint32_t steps) {

	uint32_t startSteps = (int) (steps * 2 * motorGetStepDevider()) / 4; // К-ть кроків для плавного розгону
	uint32_t stopSteps = (int) (steps * 2 * motorGetStepDevider()) / 4; // К-ть кроків для плавного гальмування
	uint32_t runSteps = (int) (steps * 2 * motorGetStepDevider()) / 2; // К-ть кроків для робочого ходу.
	uint16_t startStopDelay = (8000 / freq) * 250;

	motorSetDir(dir);
	motorSetFreq(100);
	motorEnableTMC();
	motorSetState(MOTOR_RUN);

	step = 0;
	uint16_t i = motorGetFrequency();
	while (startSteps >= step) {
		while (motorGetFrequency() <= freq) {
			motorSetFreq(i);
			delayUs(startStopDelay);
			i++;
			if (startSteps <= step)
				break;
		}
	}

	step = 0;
	while (runSteps >= step) {
	}

	step = 0;
	while (stopSteps >= step) {
		while (motorGetFrequency() >= 100) {
			motorSetFreq(i);
			delayUs(startStopDelay);
			i--;
			if (stopSteps <= step)
				break;
		}
	}

	motorSetState(MOTOR_BREAK);
}
