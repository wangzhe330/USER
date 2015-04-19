#include "io.h"
#include "stm32f10x.h" 
#include "para.h"

/*
	有四个IO输入，PD0 PD1 PD2 PD3
	有六个继电器输出  PE0 PE1 PE2 PE3 PE4 PE5
*/


void IOConfig(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//IO输入部分	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// GPIO_Mode_IPU; 	  GPIO_Mode_IN_FLOATING	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//PC12 245cs
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// GPIO_Mode_IPU; 	  GPIO_Mode_IN_FLOATING	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	//初始化主控板上面的继电器
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RelayOut0(OPEN);
	RelayOut1(OPEN);
	RelayOut2(OPEN);
	RelayOut3(OPEN);
	RelayOut4(OPEN);
	RelayOut5(OPEN);	
		
}

//根据ParaArray[] 初始化继电器的输出
void IOInit(void)
{

	if(ParaArray[OutNormalState0] == OutNormalState_open )
		RelayOut0(OPEN);
	else if(ParaArray[OutNormalState0] == OutNormalState_close)
		RelayOut0(CLOSE);
	else
		RelayOut0(OPEN);

	if(ParaArray[OutNormalState1] == OutNormalState_open )
		RelayOut1(OPEN);
	else if(ParaArray[OutNormalState1] == OutNormalState_close)
		RelayOut1(CLOSE);
	else
		RelayOut1(OPEN);

	if(ParaArray[OutNormalState2] == OutNormalState_open )
		RelayOut2(OPEN);
	else if(ParaArray[OutNormalState2] == OutNormalState_close)
		RelayOut2(CLOSE);
	else
		RelayOut2(OPEN);

	if(ParaArray[OutNormalState3] == OutNormalState_open )
		RelayOut3(OPEN);
	else if(ParaArray[OutNormalState3] == OutNormalState_close)
		RelayOut3(CLOSE);
	else
		RelayOut3(OPEN);

	if(ParaArray[OutNormalState4] == OutNormalState_open )
		RelayOut4(OPEN);
	else if(ParaArray[OutNormalState4] == OutNormalState_close)
		RelayOut4(CLOSE);
	else
		RelayOut4(OPEN);

	if(ParaArray[OutNormalState5] == OutNormalState_open )
		RelayOut5(OPEN);
	else if(ParaArray[OutNormalState5] == OutNormalState_close)
		RelayOut5(CLOSE);
	else
		RelayOut5(OPEN);
			
}

void IOTest(void){
	  	if( IO0State() == LINK )
			RelayOut0(CLOSE);
		if(  IO0State() == NOLINK )
			RelayOut0(OPEN);
	  	if( IO1State() == LINK )
			RelayOut1(CLOSE);
		if(  IO1State() == NOLINK )
			RelayOut1(OPEN);
	  	if( IO2State() == LINK )
			RelayOut2(CLOSE);
		if(  IO2State() == NOLINK )
			RelayOut2(OPEN);
	  	if( IO3State() == LINK )
			RelayOut3(CLOSE);
		if(  IO3State() == NOLINK )
			RelayOut3(OPEN);
}