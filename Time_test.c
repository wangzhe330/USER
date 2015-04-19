#include "Time_test.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "usart.h"
#include "para.h"

extern int ValveClosePluseFlag,ValveOpenPluseFlag;
/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* ����ʱ�� */
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}

//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static int count1s = 0;
	static int count10ms = 0;
	static int count20ms = 0;
	static int count100ms = 0;
	static int count250ms = 0;
	static int count500ms = 0;
	static int count750ms = 0;
	static int count1500ms = 0;

	static int sig250ms = 0;
	static int sig750ms = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		count1s++;
		count100ms++;
		/*
		count10ms++;
		count20ms++;
		 */
		count250ms++; 
		count500ms++;
		count750ms++;
		count1500ms++;
		if(count1s == 1000){
			/* LED2 ȡ��      
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, 
					(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9)))); 
					count1s = 0;*/
			if(ValveOpenLedFlag == 1)
				GPIO_WriteBit(GPIOE, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_13))));
			if(ValveCloseLedFlag == 1)
				GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));	
				
			count1s = 0;
									 			
		}
		if(count100ms == 100){
			count100ms = 0;
			AD654Read();				
		}
		/*
		if(count10ms == 10){
			count10ms = 0;	
		}
		if(count20ms == 20){
			count20ms = 0;	
		}
*/
		if(count250ms == 250){
		/*
			if(ValveClosePluseFlag == 1){
				CloseValve();
				ValveClosePluseFlag =0;
				sig250ms = 1;	
			}
			if(sig250ms == 1)
			{
				SSRSTOP();
				sig250ms = 0;
			}				 */
			count250ms = 0;	
		}
		if(count500ms == 500){
			 /*
			if(ValveOpenLedFlag == 1)
				GPIO_WriteBit(GPIOE, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_13))));
			if(ValveCloseLedFlag == 1)
				GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));						
				*/
			count500ms = 0;	
		}
		if(count1500ms == 20){
		
			if(sig750ms == 1){
				SSRSTOP();
 
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, 
					(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9)))); 
					count1s = 0; 
				ssr0_750msflag = 0;
				sig750ms = 0;
			}
			if(ssr0_750msflag == 1){
				SSR0RUN();
  
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, 
					(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9)))); 
					count1s = 0; 
				sig750ms = 1;	
			}
			count1500ms = 0;	
			
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		 
	}
}
