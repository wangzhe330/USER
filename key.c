
#include "key.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

unsigned int KEY=0;
unsigned char KEY_Flag = 0;	//�а������µı�־   1Ϊ�а������£�0Ϊ��
unsigned char Main_KEY_State = 0;	//0��ʾ���ã�1��ʾ�͵أ�2��ʾԶ��
unsigned char Main_KEY_State_Last = 0;
unsigned char Main_State_Change = 0;//0��ʾ״̬û���л���1��ʾ״̬�л���

void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

void Key_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  	/*���������˿ڣ�PD8����ʱ��*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// GPIO_Mode_IPU; 

	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
		/*����Ƿ��а������� */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	  {	   
	 	 /*��ʱ����*/
	  	Delay(10000);		
   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{	 
			/*�ȴ������ͷ� */
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
 			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
		}
	else
		return KEY_OFF;
}

int myKey_Scan(void)
{			
		/*����Ƿ��а������� */
   	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON  || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON || \
		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == KEY_ON ||\
		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_ON ) {	   
	 	 /*��ʱ����*/
	  	Delay(10000);		
   		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
			/*�ȴ������ͷ� */
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON);   
 			return 	KEY_0;	 
		}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )  {	 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON);   
 			return 	KEY_1;	 
		}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON )  {	 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON);   
 			return 	KEY_2;	 
		}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == KEY_ON )  {	 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == KEY_ON);   
 			return 	KEY_3;	 
		}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_ON )  {	 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_ON);   
 			return 	KEY_4;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}


//��ѯPD8 �� PD9������ߵ�������������������������״̬���ֱ��ʾ���͵ء����á�Զ��
/*
int myKeyStateScan(void)
{			
	//����Ƿ��а������� 
   	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON  || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON ) {	   
	 	 //��ʱ����
	  	Delay(1000);		
   		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
			//�ȴ������ͷ� 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON);   
 			return 	KEY_0;	 
		}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )  {	 
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON);   
 			return 	KEY_1;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}
*/

//EXTIģʽ�µİ����жϳ�ʼ��
void KEY_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	//ʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE );

//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource8 ); 
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource9 ); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource10 ); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12 );


	//PD8 PD9����ѡ��״̬�ģ�Ҫ���½��ض���� ��
	/*
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line9  ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init( &EXTI_InitStructure );
	*/

	//PD10 PD11 PD12 ��Ϊ��ͨ�������½��ؼ������ˡ�
	EXTI_InitStructure.EXTI_Line = EXTI_Line10 | EXTI_Line11 | EXTI_Line12 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init( &EXTI_InitStructure );

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );	

	/*
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure) ;		
}

