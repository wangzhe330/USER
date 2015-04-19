#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"
 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

#define KEY_0 20
#define KEY_1 21
#define KEY_2 22
#define KEY_3 23
#define KEY_4 24

#define KEY_LOCAL 30
#define KEY_SET 31
#define KEY_REMOTE 32
#define KEY_ESC 33
#define KEY_OK 34
#define KEY_NEXT 35

extern unsigned int KEY;
extern unsigned char KEY_Flag ;
extern unsigned char Main_KEY_State ;
extern unsigned char Main_KEY_State_Last;
extern unsigned char Main_State_Change;



void Key_GPIO_Config(void);
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);
int myKey_Scan(void);

void KEY_EXTI_Config(void);

#endif /* __LED_H */

