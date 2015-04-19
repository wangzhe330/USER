/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"	
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "key.h"
#include "SysTick.h"
#include "pwm.h"
#include "para.h"

extern void TimingDelay_Decrement(void);
extern u32 time;
extern u16 time_10ms;

extern 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}


void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
  		time++;
		time_10ms++;
		/*
		if( time%100 == 0 )	   //wz 原来是500
		{
			AD654Read();
		} */
	}		 	
}

void EXTI9_5_IRQHandler(void)//pd8 pd9所接按键中断服务程序
{
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		/*翻转D3*/
#ifdef DEBUG_MODE
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOE , GPIO_Pin_15)));
#endif
		KEY = KEY_0;
		//判断是上升沿还是下降沿
		/*
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 0 )   //PD8下降沿，进入远程模式  
			Main_KEY_State = 2;
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 1 )   //PD8上升沿，进入设置模式  
			Main_KEY_State = 0;
			*/
		//KEY_Flag = 1;
		Main_State_Change = 1;
		/*Clear the EXTI line8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
		EXTI_ClearFlag(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
#ifdef DEBUG_MODE
		/*翻转D3*/
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOE , GPIO_Pin_15)));
#endif
		KEY = KEY_1;
		//判断是上升沿还是下降沿
		/*
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 0 )   //PD9下降沿，进入	就地模式
			Main_KEY_State = 1;
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 1 )   //PD9上升沿，进入设置模式  
			Main_KEY_State = 0;
			*/
		//KEY_Flag = 1;
		Main_State_Change = 1;
		/*Clear the EXTI line8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
		EXTI_ClearFlag(EXTI_Line9);
	}

}

void EXTI15_10_IRQHandler(void)//P10和PD11 pd12所接按键中断服务程序
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
	{
#ifdef DEBUG_MODE
		/*翻转D2*/
#endif
		Delay(500);
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == 0)
		{
#ifdef DEBUG_MODE
			GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOE , GPIO_Pin_14)));
#endif
			KEY = KEY_2;
			KEY_Flag = 1;
		}
		/*Clear the EXTI line8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line10);
		EXTI_ClearFlag(EXTI_Line10);		
	}
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{
		Delay(500);
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0)
		{
#ifdef DEBUG_MODE
			/*翻转D2*/
			GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOE , GPIO_Pin_14)));
#endif
			KEY = KEY_3;
			KEY_Flag = 1;
		}
		/*Clear the EXTI line8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearFlag(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		Delay(300);
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == 0)
		{
#ifdef DEBUG_MODE
			/*翻转D2*/
			GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOE , GPIO_Pin_14)));
#endif
			KEY = KEY_4;
			KEY_Flag = 1;
		}
		/*Clear the EXTI line8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
		EXTI_ClearFlag(EXTI_Line12);
	}
}


//TIM1 CH4 输入捕捉中断  用来计频率
void TIM1_CC_IRQHandler(void)// 用相应的定时器中断 
{
	static int Ic1_readvalue1;
	static int Ic1_readvalue2;
	static int capture_number;

	AD654F ++;
	/*
	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)//是TIM1CH4
	{
		
		if(capture_number==0 )//第一次捕获
		{
			Ic1_readvalue1 = TIM_GetCapture4(TIM1);//保留捕获值	
			capture_number=1;//设置标志	
		}	
		else if(capture_number==1)//第二次捕获	
		{	
			Ic1_readvalue2 = TIM_GetCapture4(TIM1);//保留捕获值	
			
			
			if(Ic1_readvalue1 < Ic1_readvalue2)//考虑是否溢出，计算两次的差值	
			{	
				CAPTURE=Ic1_readvalue2-Ic1_readvalue1-1;	
			}	
			else	
			{	
				CAPTURE=Ic1_readvalue2+5760-Ic1_readvalue1;	 //2880
			}
			capture_number=0; 
			TIM1_FREQ =360000/ CAPTURE;  	   //180000
		}
		//
	}
	*/
	
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC4 | TIM_IT_Update);//清空捕获标志
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
