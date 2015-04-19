
#include "key.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

unsigned int KEY=0;
unsigned char KEY_Flag = 0;	//有按键按下的标志   1为有按键按下，0为无
unsigned char Main_KEY_State = 0;	//0表示设置，1表示就地，2表示远程
unsigned char Main_KEY_State_Last = 0;
unsigned char Main_State_Change = 0;//0表示状态没有切换，1表示状态切换了

void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

void Key_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  	/*开启按键端口（PD8）的时钟*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// GPIO_Mode_IPU; 

	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
		/*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	  {	   
	 	 /*延时消抖*/
	  	Delay(10000);		
   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{	 
			/*等待按键释放 */
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
		/*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON  || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON || \
		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == KEY_ON ||\
		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_ON ) {	   
	 	 /*延时消抖*/
	  	Delay(10000);		
   		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
			/*等待按键释放 */
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


//轮询PD8 、 PD9，最左边的两个按键，两个按键的三种状态，分别表示：就地、设置、远程
/*
int myKeyStateScan(void)
{			
	//检测是否有按键按下 
   	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON  || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON ) {	   
	 	 //延时消抖
	  	Delay(1000);		
   		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
			//等待按键释放 
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

//EXTI模式下的按键中断初始化
void KEY_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD , &GPIO_InitStructure);

	//使能AFIO时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE );

//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource8 ); 
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource9 ); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource10 ); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource11 );
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource12 );


	//PD8 PD9用来选择状态的，要上下降沿都检测 。
	/*
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line9  ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init( &EXTI_InitStructure );
	*/

	//PD10 PD11 PD12 作为普通按键，下降沿检测就行了。
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

