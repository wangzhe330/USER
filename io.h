#ifndef _IO_H_
#define _IO_H_


//四个IO输入
#define LINK 		0
#define NOLINK	 	1

#define IO0State() GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
#define IO1State() GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define IO2State() GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define IO3State() GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)


//六个继电器的输出  0~5	  
#define OPEN 		0 
#define CLOSE  		1

#define RelayOut0(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_0)

#define RelayOut1(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_1)

#define RelayOut2(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_2);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_2)

#define RelayOut3(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_3)

#define RelayOut4(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define RelayOut5(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5)


void IOConfig(void);
void IOInit(void);
void IOTest(void);

#endif
