#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"
#include "SysTick.h"



 
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

extern u16 EncoderData;
extern u16 temp;
extern int	ValvePosValue ;		//三位数，最后一位是小数位 
//前一次的编码器值
extern int	ValvePosValueLast ;		//三位数，最后一位是小数位
extern int SPEED; 

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);  

#endif	/* TIME_TEST_H */
