#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

extern int CAPTURE;
extern int TIM1_FREQ;

void TIM1_PWM2_Init(void);

#endif
