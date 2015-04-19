#include "Time_test.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "usart.h"
#include "para.h"

extern int ValveClosePluseFlag,ValveOpenPluseFlag;
/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
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


/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* 开启时钟 */
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/    
}

//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
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
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
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
			/* LED2 取反      
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		 
	}
}
