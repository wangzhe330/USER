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
		
	/* PA8����Ϊ���ܽ�(PWM) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/* PA11����Ϊ���ܽ�(���벶׽) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	/*PB13 ����ΪPWM�ķ��������	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/	
	/*��ʱ��PWM�� ��GPIO��*/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	

	/*����TIM1*/	
	TIM_DeInit(TIM1); //����Ϊȱʡֵ		   
	/*TIM1ʱ������*/
	
	TIM_TimeBaseStructure.TIM_Prescaler = 200;    //400  //Ԥ��Ƶ(ʱ�ӷ�Ƶ)72M/4000=18K
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 5760;//2880;//1440       //װ��ֵ 18k/144=125hz ����˵���ϼӵ�144������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //������ʱ�ӷָ� 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;    //���ڼ�����ֵ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);    //��ʼ��TIMx��ʱ�������λ
	/*
	TIM_TimeBaseStructure.TIM_Prescaler = 400;      //Ԥ��Ƶ(ʱ�ӷ�Ƶ)72M/4000=18K
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 65535;//1440       //װ��ֵ 18k/144=125hz ����˵���ϼӵ�144������
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //������ʱ�ӷָ� 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;    //���ڼ�����ֵ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);    //��ʼ��TIMx��ʱ�������λ
	*/
	/* Channel 1 Configuration in PWM mode ͨ��һ��PWM */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     //PWMģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //����ͨ����Ч  PA8 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //����ͨ��Ҳ��Ч PB13
	TIM_OCInitStructure.TIM_Pulse =60;        //ռ��ʱ��  144 ����40��ʱ��Ϊ�ߣ���������������෴
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;   //�������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;     //�����˵ļ���  
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  //����״̬�µķǹ���״̬ ����
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  //�Ȳ���
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);       //����ʼ������TIMxͨ��1����2.0��ΪTIM_OCInit

	
	
	//����TIM1_CH4Ϊ���벶��ģʽ���������AD654�����Ƶ���ź� wz
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //ѡ��ͨ��4
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //���������ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ͨ������ѡ��(��λ)
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //ÿ�μ�⵽��������ʹ���һ�β���
	//��Ԥ��Ƶϵ����	
	TIM_ICInitStructure.TIM_ICFilter = 0x0; //�˲�ϵ��
	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	
	//�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC4, ENABLE); //���ж�
																   
	/* TIM1 counter enable����ʱ�� */
	TIM_Cmd(TIM1,ENABLE);
	
	/* TIM1 Main Output Enable ʹ��TIM1����������*/
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}





