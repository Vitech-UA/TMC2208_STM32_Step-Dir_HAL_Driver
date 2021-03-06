/*
 * tmc2208.h
 *
 *  Created on: Jun 12, 2021
 *      Author: Vitech-UA
 */

#include "stdint.h"
#include "stm32f1xx_hal.h"

#ifndef TMC2208_H_
#define TMC2208_H_


#define MOTOR_STEP_TIMER htim1
#define CURRENT_FREQ 24000000

typedef enum{
	STEP_DIV_BY_2,
	STEP_DIV_BY_4,
	STEP_DIV_BY_8,
	STEP_DIV_BY_16

}STEP_DIV_t;

typedef enum{
	MOTOR_RUN,
	MOTOR_BREAK
}MOTOR_STATE_t;

typedef enum{
	MOTOR_RUNNING,
	MOTOR_STOPED,
	MOTOR_INDLE
}MOTOR_STATUS_t;

void motorDisableTMC(void);
void motorEnableTMC(void);
void motorSetDir(uint8_t dir);
void motorSetDiv(STEP_DIV_t div);
void motorSetFreq(uint16_t freq);
void motorSetState(MOTOR_STATE_t state);
MOTOR_STATUS_t motorGetStatus(void);
void motorRotateByStep(uint32_t stepcount);
void motorRun(uint16_t freq, uint8_t dir, uint32_t steps);
#endif /* TMC2208_H_ */
