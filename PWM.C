#include "PWM.h"
#include "stm32f10x_conf.h"

int CAPTURE = 0;
int TIM1_FREQ = 0;


void TIM1_PWM2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
		
	/* PA8设置为功能脚(PWM) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/* PA11设置为功能脚(输入捕捉) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	/*PB13 设置为PWM的反极性输出	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/	
	/*开时钟PWM的 与GPIO的*/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	

	/*配置TIM1*/	
	TIM_DeInit(TIM1); //重设为缺省值		   
	/*TIM1时钟配置*/
	
	TIM_TimeBaseStructure.TIM_Prescaler = 200;    //400  //预分频(时钟分频)72M/4000=18K
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseStructure.TIM_Period = 5760;//2880;//1440       //装载值 18k/144=125hz 就是说向上加的144便满了
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //设置了时钟分割 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;    //周期计数器值
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);    //初始化TIMx的时间基数单位
	/*
	TIM_TimeBaseStructure.TIM_Prescaler = 400;      //预分频(时钟分频)72M/4000=18K
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseStructure.TIM_Period = 65535;//1440       //装载值 18k/144=125hz 就是说向上加的144便满了
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //设置了时钟分割 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;    //周期计数器值
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);    //初始化TIMx的时间基数单位
	*/
	/* Channel 1 Configuration in PWM mode 通道一的PWM */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     //PWM模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //正向通道有效  PA8 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //反向通道也有效 PB13
	TIM_OCInitStructure.TIM_Pulse =60;        //占空时间  144 中有40的时间为高，互补的输出正好相反
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;   //输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;     //互补端的极性  
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  //空闲状态下的非工作状态 不管
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  //先不管
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);       //数初始化外设TIMx通道1这里2.0库为TIM_OCInit

	
	
	//配置TIM1_CH4为输入捕获模式，用来获得AD654输出的频率信号 wz
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //选择通道4
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //输入上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 通道方向选择(相位)
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //每次检测到捕获输入就触发一次捕获
	//（预分频系数）	
	TIM_ICInitStructure.TIM_ICFilter = 0x0; //滤波系数
	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC4, ENABLE); //打开中断
																   
	/* TIM1 counter enable开定时器 */
	TIM_Cmd(TIM1,ENABLE);
	
	/* TIM1 Main Output Enable 使能TIM1外设的主输出*/
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}






