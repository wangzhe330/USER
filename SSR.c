#include "stm32f10x.h"
#include "ssr.h"
#include "SysTick.h"
#include "control.h"
#include "led.h"
#include "para.h"

extern u16 EncoderData;
extern u16 temp;
extern int	ValvePosValue ;		//��λ�������һλ��С��λ 
//ǰһ�εı�����ֵ
extern int	ValvePosValueLast ;		//��λ�������һλ��С��λ
extern int SPEED; 

void SSR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 );		//ȫ����1
	//PC0 PC1 ����ߵ�ƽ��̬�̵����ضϡ�PC0 PC1 ����͵�ƽ��ʱ��SSR��ͨ 
    //PC2 ����͵�ƽ�����ж�SSR�ĵ�Դ��
}

void SSRPowerOn(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);	
}

void SSRPowerOff(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	
	ValveOpenLedFlag = 0;
	ValveCloseLedFlag = 0;
}

void OpenValve(void)
{
//bug �����ǲ���Ҫ����SSRSTOP()
	SSRSTOP();
	SSR0RUN();
	ValveOpenLedFlag = 1;
	ValveCloseLedFlag = 0;
	LED3(ON);
}

void CloseValve(void)
{
	SSRSTOP();
	SSR1RUN();
	ValveCloseLedFlag = 1;
	ValveOpenLedFlag = 0;
	LED1(ON);	
}


void SSR0RUN()
{	
	GPIO_ResetBits(GPIOC, GPIO_Pin_0 );
	//Delay_ms(100);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	STOP_FLAG = 0;		
}

void SSR1RUN()
{
	//GPIO_ResetBits(GPIOC, GPIO_Pin_2 );
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	//Delay_ms(100);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	STOP_FLAG = 0;		
}

void SSRSTOP()
{
	//ULN2803�з����ܣ����Ը�λ���൱�������������Ǹ㶨ƽ�������ͨ
	//GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 );
	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);	

	ValveOpenLedFlag = 0;
	ValveCloseLedFlag = 0;
	ValveStopFlag = 0;
	if(ValvePosValue == 0) 
	{
		LED3(ON);
		LED1(OFF);
	}
	if( ValvePosValue == 1000 )
	{
		LED3(OFF);
		LED1(ON);
	}	
}

//����������
/*
void MotorParaAutoSet()
{
	int pos_now=0,pos_set=0,cnt=0;
	while( ValvePosValue > 100) SSR1RUN();
	SSRSTOP();
	for( cnt = 0 ; cnt < 8 ; cnt++){
		pos_now = ValvePosValue	 ;
	}
} */
