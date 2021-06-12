/*
 * tmc2208.h
 *
 *  Created on: Jun 12, 2021
 *      Author: Vitech-UA
 */

#include "stdint.h"

#ifndef TMC2208_H_
#define TMC2208_H_

typedef enum{
	STEP_DIV_BY_2,
	STEP_DIV_BY_4,
	STEP_DIV_BY_8,
	STEP_DIV_BY_16

}STEP_DIV;

void motorDisable(void);
void motorEnable(void);
void motorSetDir(uint8_t dir);
void motorSetDiv(STEP_DIV div);
void motorSetFreq(uint16_t freq);


#endif /* TMC2208_H_ */
