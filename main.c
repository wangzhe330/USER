#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "SysTick.h"
#include "led.h"
#include "oled.h"
#include "page.h"
#include "list.h"
#include "key.h"
#include "area.h"
#include "atom.h"
#include "para.h"
#include "usart.h"
#include "ssr.h"
#include "time_test.h"
#include "keyresponse.h"
#include "pwm.h"
#include "current.h"
#include "control.h"
#include "stm32f10x_usart.h"  
#include "spi_flash.h"
#include "io.h"
#include "fault.h"
//#include "timer.h"

//#define DEBUG_MODE 0

//全局变量
extern Menu_Option option[6];
extern Menu_Label ch_label[8];
extern Menu_InputOp input_op[5];		//用作输入参数
extern M_Page Page[PageSum];

extern int ad654rec[10];
	
volatile u32 time; // ms 计时变量
volatile u16 time_10ms;	 //10ms 计时器变量

volatile u32 time_5s;	 //5s 计时器变量
volatile u32 time_60s;	 //60s 计时器变量
u16 ButtonFlag = 0;		//如果这个值==1 说明有按键操作  那么就不屏保
u16 ScreenFlag = 0;		//开始屏保的标志位
u16 ScreenCnt = 0;		//屏保计数器

//编码器数据
u32 EncoderData = 0;
u32 temp = 0;

u32 SetValveMin32Bit = 0;
u32 SetValveMax32Bit = 0;


int auto_set_flag = 0;
int AutoSM = 0; //自学习状态机

int pos_now=0; //自学习算法中用来记录当前位置信息的变量
int pos_set=0;
int error_array[32];//自学习算法中，用来记录误差值的数组
int error_array_cnt = 0;

int motor_auto_run(int now , int set);
void para_auto_set(void);
int motor_auto_run_states = 0;
int motor_wait_cnt = 0;	

int tempmax = 0 , tempmin = 0;
float weight = 0.0;
int ValveOpenError = 0 ,ValveCloseError = 0;
		
float set_k = 0.0;

int ValveClosePluseFlag = 0,ValveOpenPluseFlag = 0;
float debug_t1 = 0.0, debug_t2 = 0.0 ,debug_t3 = 0.0;


//初始化函数，用来初始化系统的各个模块
void ValveSysInit()
{
	/* config the sysclock to 72mhz */
	SystemInit();
	/* config led gpio */
	LED_GPIO_Config();
	/* 配置SysTick 为1us中断一次 */
	SysTick_Init();

	OLED_GPIO_Init();		//OLED引脚初始化
	OLED_Init();
	SSR_GPIO_Config();

	IOConfig();

	//Key_GPIO_Config();
	KEY_EXTI_Config();	
	
	//TIM1 4-20mA接收与发送用
	TIM1_PWM2_Init();
	TIM_SetCompare1(TIM1, 2848);
	
	//while(1);

	//time2配置
	TIM2_NVIC_Configuration();
	TIM2_Configuration();

	//TIM3配置20ms中断
	TIM3_Int_Init(99,719);//100Khz的计数频率，计数到99为1ms	
	//START_TIME;
	//串口2配置 读取编码器数据
	USART2_Config();
	//串口1配置
	uart1_init(115200);

	u3_init(); //9600

	//SPI-FLASH初始化，驱动W25X16
	SPI_FLASH_Init();
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay_ms(50);
	FlashID = SPI_FLASH_ReadID();
	if (FlashID != sFLASH_ID)
	{
		//这里去掉while1  改成FLASH报错
		while(1);
	}	
	/*
	time=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	*/
	
	PageInit();
	AreaInit();

	//参数初始化，从FLASH中读取数据
	ParaInit();
	//继电器输出初始化
	IOInit();

	Delay_ms(300);
	Fill_RAM(0x00);

	Delay_ms(300);
}

int main(void)
{

	ValveSysInit();


	//TIM_SetCompare1(TIM1, 5700 );
	//while(1);
	//page_mode();
	//Fill_Block(0,40,0,32,0x00);  //参数：起点列号，重点列号，起点行号，终点行号，填充灰度值
	//while(1);

	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2 );
	//GPIO_SetBits(GPIOC, GPIO_Pin_2 );
	//GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
	//while(1);

	//读取最左边两个按键的状态，确定上电之后的状态
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
		Main_KEY_State = 1; 
	}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )  {	 
		Main_KEY_State = 2;   
	}else{
		Main_KEY_State = 0;
	}

	Main_State_Change = 1;	//第一次运行，用这个去驱动显示什么内容

	START_TIME;

	//ssr0_750msflag = 1;

	//while(1);

  	while(1){
	
		//Serial3PutChar(0x33);  //test u3 
		
		//按键触发事件
		if(KEY_Flag == 1)	 
		{
			if(Main_KEY_State == 0)	   //设置模式
			{
				//wz 11.18南通
				if(ScreenFlag == 1)
 				{
					//屏保模式下不响应动作
					page_mode();	
 				}
				else{
					if(AreaMode == 0)
					{
						if(InputMode == 0)
							PageKeyResponse(KEY);
						else if(InputMode == 1)
							InputKeyResponse(KEY);
					}
					else if(AreaMode == 1)		//在area模式下，每个按键动作有了新的意义
					{
						if(InputMode == 0)
							AreaKeyResponse(KEY);									
						else if(InputMode == 1)
							InputKeyResponse(KEY);	
					}
				}
				
				//bug？ 没有else了
				KEY_Flag = 0;
			}
			if( Main_KEY_State == 1)   //就地模式
			{
				LocalKeyResponse(KEY);
				KEY_Flag = 0;

			}		
			if( Main_KEY_State == 2)   //远程模式
			{
				FarawayKeyResponse(KEY);
				KEY_Flag = 0;					
			}
						//wz 11.18 屏保
			ButtonFlag = 1;	//有动作发生，切出屏保
			ScreenFlag = 0;
			time_60s =0;
		 }

		//定时器驱动的
		if ( time >= 1000 ) /* 1s 时间到 */
		{
			time = 0;
			/* LED1 取反 */      
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, 
					(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8))));
			Serial3PutChar(0x33);
				 
		} 

		//每20ms读取一次编码器的值，并且显示
		if( time % 20 == 0 )
		{
			//-----------------读取一次编码器的值、显示
			GetEncoderPara();
			Delay_ms(2);

			//wz 修改 11.18南通 针对编码器可能是24位的修改
			EncoderData = UartData[2];
			EncoderData = EncoderData<<8;
			EncoderData += UartData[1];				
			EncoderData = EncoderData<<8;
			EncoderData += UartData[0];

			temp = EncoderData;	
			EncoderCommEnd();
			 		
			ValvePosValueLast = ValvePosValue;
			
			//wzdebug 看一下下面四个变量的值 是否恢复成为32位的了
			//wz 编码器32位的算法
			SetValveMin32Bit = ParaArray[SetValveMin16High]<<16;
			SetValveMin32Bit = SetValveMin32Bit + ParaArray[SetValveMin16Low];
			SetValveMax32Bit = ParaArray[SetValveMax16High]<<16;
			SetValveMax32Bit = SetValveMax32Bit + ParaArray[SetValveMax16Low];
			/*			
			//ValvePosValue = (int)(((float) EncoderData - 46854.0 )/9810.0*1000.0);
			//0%的编码器值 是比 100%的编码器值 大的
			if(ParaArray[SetValveMin] > ParaArray[SetValveMax])
			{ 				
				ValvePosValue = (int)(((float) EncoderData - (float)ParaArray[SetValveMin] )/(float)(ParaArray[SetValveMax]-ParaArray[SetValveMin])*1000.0); 	
				//if(ValvePosValue>1000 && ValvePosValue<1002)
				//	ValvePosValue =1000;
			}
			else
			{
				//guzhang 这里应该给出一个设定值故障
				ValvePosValue = (int)(((float) EncoderData - 46854.0 )/9810.0*1000.0);
			} */
			debug_t1 = (float) EncoderData - (float)SetValveMin32Bit ;
			debug_t2 = (float)SetValveMax32Bit-(float)SetValveMin32Bit;
			debug_t3 = debug_t1/debug_t2*1000.0;
			ValvePosValue = (int)debug_t3;										
			

			//wz 0% 零位
			if( ValvePosValue < 0 && ValvePosValue > -3)
				ValvePosValue = 0;
			if( ValvePosValue > 1000 && ValvePosValue < 1003)
				ValvePosValue = 1000;
			//wz bug 改写这个个else 这里输入设定值故障
			
			SPEED =  ValvePosValue - ValvePosValueLast;		//求出速度
			if( SPEED<0 ) SPEED = 0-SPEED;

			//ValvePosShow();
			//wzdebug  设置终端位置的时候 在右上角 显示 编码器的值
			if(PageNum == 3 && AreaNum == 1){
				ascii_1608(36,0,english[EncoderData%10000000/1000000+18],0);
				ascii_1608(40,0,english[EncoderData%1000000/100000+18],0);
				ascii_1608(44,0,english[EncoderData%100000/10000+18],0);	
				ascii_1608(48,0,english[EncoderData%10000/1000+18],0);
				ascii_1608(52,0,english[EncoderData%1000/100+18],0);
				ascii_1608(56,0,english[(EncoderData%100)/10+18],0);
				ascii_1608(60,0,english[EncoderData%10+18],0);	
			}else{
				ValvePosShow();	
			}
			ValveFreedback(EncoderData);

			if(AreaMode == 1){	
				if(PageNum == 3 && AreaNum == 4 && (PageAreaRec[PageNum][AreaNum] ==0 || PageAreaRec[PageNum][AreaNum]==1 ))
				{
					ParaShow(EncoderData);
				}
				if(PageNum == 3 && AreaNum == 4 && (PageAreaRec[PageNum][AreaNum] ==2 || PageAreaRec[PageNum][AreaNum]==3 ))
				{
					ParaShow(ad654rec[0]);
				}

			}
			/*
			else
			{
				ParaShow( SPEED );
			} */
			//----------------------------------------------

			if(FarawaySM == 2)		//停止信号给出之后，判断是不是完全停下来
			{
				if( SPEED == 0)
				{
					//ValveError  = ValveError + ValvePosValue - ValveSetVal;		//求出这一次停止下来之后的静态误差
					ValveError  =  ValvePosValue - ValveSetVal;		//求出这一次停止下来之后的静态误差
					STOP_FLAG = 0;
					//FarawaySM = 3;	//状态机 = 3，表示完全停下来了，记录误差
					FarawaySM = 0;		//状态机=0  表示可以设置参数
				}
			}
			//远程模式去读取设定值，每20ms*10=200ms一次
			if(Main_KEY_State == 2)
			//if(1)
			{
				
				//wzdebug  11.18南通 调试
				
				//if(1)
				if( ParaArray[FarawayMode] == analog_mode  )
				{	
				
					ValveSetValTempCnt ++;
					if(ValveSetValTempCnt == 10)
					{ //wz 2015-3-23 20 //25
						
						AD654ToCurrent();
						ValveSetShow();	//设定值显示											
					
						ValveSetValTempCnt = 0;
	
						//wz 4-22 加入 FaultTotal == 0 会不会对别的造成影响？
						if( IO2State() == LINK && FaultTotal == 0)
						{
							if(ValveSetVal > 0 && ValveSetVal < 1000 ){  //&& ValveSetVal != 1000
								if(ValvePosValue < ValveSetVal - 10)
								{	
									
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;	//记录阀位开始动作的初始阀位是多少，用来比较是否停止运动
									OpenValve();
								}
								else if(ValvePosValue > ValveSetVal + 10)
								{							
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();	
								}
							}	 
							else if(ValveSetVal <= 0)
							{
								//全关情况要单独判断是否完全关闭
								//wz4-17
								if(ValvePosValue > 0 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();				
								}					
							} 	 
							else if(ValveSetVal >= 1000)
							{
								//全开情况要单独判断是否完全打开
								//wz4-17
								if(ValvePosValue < 1000 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									OpenValve();
								}								
							}					 
						}
		 	
						//}
					}
				}
				//wz 注意这里的    && FarawaySM == 0   将震荡 从三次降到了两次
				if(ParaArray[FarawayMode] == pc_mode && PCMode == pc_mode_set && FarawaySM == 0)
				{					
					if(PCSetValve >= 10 && PCSetValve <=995)
						ValveSetVal = PCSetValve;
					else if(PCSetValve < 10)	
						ValveSetVal = 0;
					else if(PCSetValve > 995)
						ValveSetVal = 1000;
					if( IO2State() == LINK )
					{	
						if( IO2State() == LINK && FaultTotal == 0)
						{
							if(ValveSetVal > 0 && ValveSetVal < 1000 ){  //&& ValveSetVal != 1000
								if(ValvePosValue < ValveSetVal - 10)
								{	
									
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;	//记录阀位开始动作的初始阀位是多少，用来比较是否停止运动
									OpenValve();
								}
								else if(ValvePosValue > ValveSetVal + 10)
								{							
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();	
								}
							}	 
							else if(ValveSetVal <= 0)
							{
								//全关情况要单独判断是否完全关闭
								//wz4-17
								if(ValvePosValue > 0 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();				
								}					
							} 	 
							else if(ValveSetVal >= 1000)
							{
								//全开情况要单独判断是否完全打开
								//wz4-17
								if(ValvePosValue < 1000 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//状态机=1  表示开始运行
									ValveSetExecute_ValveRec = ValvePosValue;
									OpenValve();
								}								
							}					 
						}
						/*if(ValveSetVal > 0  ){  //&& ValveSetVal != 1000
							if(ValvePosValue < ValveSetVal - 10)
							{	
								
								ValveSetExecuteFlag = 1;
								FarawaySM = 1;		//状态机=1  表示开始运行
								ValveSetExecute_ValveRec = ValvePosValue;	//记录阀位开始动作的初始阀位是多少，用来比较是否停止运动
								OpenValve();
							}
							else if(ValvePosValue > ValveSetVal + 10)
							{							
								ValveSetExecuteFlag = 1;
								FarawaySM = 1;		//状态机=1  表示开始运行
								ValveSetExecute_ValveRec = ValvePosValue;
								CloseValve();	
							}
						}	 
						else if(ValveSetVal <= 0)
						{
							//全关情况要单独判断是否完全关闭
							//wz4-17
							if(ValvePosValue > 0 ){
								ValveSetExecuteFlag = 1;
								FarawaySM = 1;		//状态机=1  表示开始运行
								ValveSetExecute_ValveRec = ValvePosValue;
								CloseValve();				
							}					
						}*/					
					}
				}
			}
			if(Main_KEY_State == 1 || Main_KEY_State == 2)
			{
				if(FaultShowFlag == 1)
					FaultShow(FaultTotal);
			} 
			//else{ ValveSetValTempCnt = 0; } //在别的模式情况下，把计数器清零，防止上一次的干扰带入 

			motor_wait_cnt++;
			if(motor_wait_cnt == 75 ) motor_wait_cnt = 0;
		}

		//每10ms读取一次最左边两个状态选择按键的状态，如果按键中断中抖动的问题完全解决了的话，可以把这里的查询去掉，继续用中断的方式
		if(time_10ms >= 10)
		{
			//这里读取之后可以紧接着判断远程模式是否接着运动。！！  .//WZ
			
			Main_KEY_State_Last	 = Main_KEY_State;
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON ){
				Delay_ms(1);
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )
					Main_KEY_State = 2; 
			}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )  {
				Delay_ms(1);
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )	 
					Main_KEY_State = 1;   
			}else{
				Delay_ms(1);
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_OFF || GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_OFF)
					Main_KEY_State = 0;
			}			
			if(	Main_KEY_State_Last != Main_KEY_State)
				Main_State_Change = 1;
			time_10ms = 0;
		}

		//左边两个主按键的状态变化
		if( Main_State_Change == 1)
		{
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 0 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 1)   //就地模式
				Main_KEY_State = 1;
			else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 1 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 0)   //进入远程  
				Main_KEY_State = 2;
			else 
				Main_KEY_State = 0;

			if(Main_KEY_State == 0)
			{
				//wz  从设置切出去之后 再切进来的时候应该要到初始界面  所以PageNum = 1;
				PageNum = 1;
				
				page_mode();
				FaultShowFlag = 0;				
				SSRSTOP();
			}
			if( Main_KEY_State == 1 )
			{
				local_mode();
				FaultShowFlag = 1;
				SSRSTOP();
			}
			if( Main_KEY_State == 2)
			{
				faraway_mode();	 
				FaultShowFlag = 1;
				SSRSTOP();		  
			}
					
			Main_State_Change = 0;
			//wz11.18 屏保
			ButtonFlag = 1;	//有动作发生，切出屏保
			ScreenFlag = 0;
			time_60s = 0;
		}  
	
		//故障检测 
		
		FaultDetect();

		if(Main_KEY_State == 2  )
		{
			if(ParaArray[FarawayMode] == analog_mode  || (ParaArray[FarawayMode] == pc_mode && PCMode == pc_mode_set ))
			{		
				if( FarawaySM == 1 )
				{
					if( IO2State() == NOLINK )
					{
						SSRSTOP();
						STOP_FLAG = 1;
						FarawaySM = 2;		//状态机=2  表示电机停转					
					}
					else if( IO2State() == LINK )
					{
						//if(( ValvePosValue < ValveSetVal && ValvePosValue > ValveSetVal - 17 ) || ( ValvePosValue>ValveSetVal && ValvePosValue <ValveSetVal + 19 )	)
						//if(( ValvePosValue < ValveSetVal && ValvePosValue > ValveSetVal - (int)(17.0*( 1000.0/((float)ParaArray[SetValveMax]-(float)ParaArray[SetValveMin])) ) ) || ( ValvePosValue>ValveSetVal && ValvePosValue <ValveSetVal + (int)(38.0*( 1000.0/((float)ParaArray[SetValveMax]-(float)ParaArray[SetValveMin]) ) ) )	)					
	
						if( ValveOpenLedFlag == 1 )//( ValveSetExecute_ValveRec < ValveSetVal - 10)  //这里是开阀门操作
						{
							if(ValveSetVal < 1000){	
								if( ValvePosValue > ValveSetVal - ValveOpenError )
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//状态机=2  表示电机停转
								}
							}
							else if( ValveSetVal == 1000 ){
								if( ValvePosValue >= 998 )//if( ValvePosValue > ValveSetVal - ValveOpenError  )
								{
									//if( ValvePosValue > ValveSetVal - 10 )
									{
										SSRSTOP();
										STOP_FLAG = 1;
										FarawaySM = 2;		//状态机=2  表示电机停转
									}
								}	
							}
						}
						
						if(ValveSetVal != 0){
							if(ValveSetExecute_ValveRec > ValveSetVal + 10)		//现在执行的是关阀门操作
							{
								if( ValvePosValue < ValveSetVal - ValveCloseError )
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//状态机=2  表示电机停转
								}
							}
							if( ValvePosValue < 0-ValveCloseError && ValvePosValue < ValvePosValueLast )
							{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//状态机=2  表示电机停转																					
							}
						}
						else if(ValveSetVal == 0)
						{
							if(ValveSetExecute_ValveRec > ValveSetVal)		//现在执行的是关阀门操作
							{
								if( ValvePosValue < ValveSetVal - ValveCloseError - 5)
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//状态机=2  表示电机停转
								}
								if( ValvePosValue < 0)
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//状态机=2  表示电机停转									
								}
							}								
						}
						/*
						if(ValveSetVal == 1000)
						{
							if(ValvePosValue == 1000 )
								SSRSTOP();	
						} */
					}
				}
			}
			if(ParaArray[FarawayMode] == pluse_mode )
			{
				if( IO2State() == NOLINK )
				{
						SSRSTOP();
						STOP_FLAG = 1;
						FarawaySM = 2;		//状态机=2  表示电机停转					
				}
				else if(IO2State() == LINK)
				{
					if( IO0State() == LINK && IO1State() == NOLINK  )
					{
						Delay_us(500);
						if( IO0State() == LINK && IO1State() == NOLINK)	
						{
							if( ValvePosValue > 0)
								CloseValve();	
						}
					}
					else if( IO0State() == NOLINK && IO1State() == LINK )
					{
						Delay_us(500);
						if( IO0State() == NOLINK && IO1State() == LINK )
						{
							if(ValvePosValue < 1000)
								OpenValve();	
						}
					}
					else if( IO0State() == NOLINK && IO1State() == NOLINK )
					{
						Delay_us(500);
						if( IO0State() == NOLINK && IO1State() == NOLINK )
							SSRSTOP();	
					}
					else if( IO0State() == LINK && IO1State() == LINK )
					{
						Delay_us(500);
						if( IO0State() == LINK && IO1State() == LINK )
						//guzhang  同时开和关
						SSRSTOP();	
					}
				}
				if( ValveSetVal == 1000 )
				{
					if( ValvePosValue>1000 - ValveOpenError - 8 &&  ValvePosValue>ValvePosValueLast)
						SSRSTOP();	
				}
				if( ValvePosValue>1000 - ValveOpenError &&  ValvePosValue>ValvePosValueLast)
						SSRSTOP();	
				if( ValvePosValue < 0-ValveCloseError && ValvePosValue<ValvePosValueLast )
						SSRSTOP();				
			}
			if( ParaArray[FarawayMode] == hold_mode )
			{
				if( IO2State() == NOLINK )
				{
						SSRSTOP();					
				}
				else if(IO2State() == LINK)
				{			
					//开阀门
					if(	IO1State() == LINK && IO0State() == NOLINK){
						if( ValvePosValue > 1000 - ValveOpenError )
							SSRSTOP();
						else
							OpenValve();//SSR1RUN();		
					}
					if( IO0State() == LINK && IO1State() == NOLINK){
						if( ValvePosValue > 0 )
							CloseValve();//SSR0RUN();			
					}							
	
					if( ( ValvePosValue>1000 - ValveOpenError - 5 ) &&  ValvePosValue>ValvePosValueLast)	  // + ValveOpenError/10
							SSRSTOP();	
					if( ( ValvePosValue < 0-ValveCloseError+ValveCloseError/5  ) && ValvePosValue<ValvePosValueLast )
							SSRSTOP();
				}				
			}
			if(ParaArray[FarawayMode] == pc_mode && PCMode == pc_mode_pluse )
			{
				if( IO2State() == NOLINK )
				{
						SSRSTOP();
						STOP_FLAG = 1;
						FarawaySM = 2;		//状态机=2  表示电机停转					
				}
				else if(IO2State() == LINK)
				{
					if( PCPluseCloseSig == 1 )
					{
						if( ValvePosValue > 0)
							CloseValve();
						PCPluseCloseSig	 = 0;
					}
					if( PCPluseOpenSig == 1 )
					{
						if(ValvePosValue < 1000)
							OpenValve();
						PCPluseOpenSig = 0;								
					}
					if( PCPluseStopSig == 1)
					{
						SSRSTOP();
						PCPluseStopSig = 0;	
					}
				}
				if( ValveSetVal == 1000 )
				{
					if( ValvePosValue>1000 - ValveOpenError - 8 &&  ValvePosValue>ValvePosValueLast)
						SSRSTOP();	
				}
				if( ValvePosValue>1000 - ValveOpenError &&  ValvePosValue>ValvePosValueLast)
						SSRSTOP();	
				if( ValvePosValue < 0-ValveCloseError && ValvePosValue<ValvePosValueLast )
						SSRSTOP();				
			}
		}

		
		if(Main_KEY_State == 1){
			if( ParaArray[LocallyMode]==pluse_mode )
			{
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_OFF)
				{
					if( ValvePosValue < 1000 )
						OpenValve();//SSR1RUN();
				}
				else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_OFF && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_OFF)
					SSRSTOP();
				if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_ON && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_OFF)
				{
					if( ValvePosValue >0 )
						CloseValve();//SSR0RUN();
				}
				else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_OFF && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == KEY_OFF)
					SSRSTOP();

				if( ValvePosValue>1000 - ValveOpenError &&  ValvePosValue>ValvePosValueLast)
						SSRSTOP();	
				if( ValvePosValue < 0-ValveCloseError && ValvePosValue<ValvePosValueLast )
						SSRSTOP();
			}
			else if( ParaArray[LocallyMode] == hold_mode )
			{
				if( ( ValvePosValue>1000 - ValveOpenError -3 ) &&  ValvePosValue>ValvePosValueLast)	 //+ ValveOpenError/10 
						SSRSTOP();	
				if( ( ValvePosValue < 0-ValveCloseError+ValveCloseError/5  ) && ValvePosValue<ValvePosValueLast )
						SSRSTOP();
			}
		} 	


		if(auto_set_flag == 1){
			para_auto_set();	//参数自整定
		}

		if(FaultTotal == 0){
			if(ValvePosValue <= 0 && ValvePosValue > -15) 
			{
				LED3(ON);
				LED1(OFF);
				RelayOut4(CLOSE);
			}
			if( ValvePosValue > 0 && ValvePosValue < 1000 )
			{
				RelayOut4(OPEN);
				RelayOut5(OPEN);	
			}
			if(ValvePosValue >= 1000 )//&& ValvePosValue < 1020)
			{
				LED3(OFF);
				LED1(ON);
				RelayOut5(CLOSE);
			}
		}


		//阀位保护 （就地模式）
		/*
		if(Main_KEY_State == 1){
			if(ValvePosValue <= 5 || ValvePosValue >= 995){
				SSRSTOP();
			}
		}						 */
		//wz 保护电机
		//if(ValvePosValue < 20 || ValvePosValue > 980 ) SSRSTOP(); 
		
		//60s 无按键动作的情况下，进入屏保模式	600000
		if(time_60s >= 50000)
		{
			if (ButtonFlag == 0){  //任何按键操作将会打断屏保 （问一下：远程操作是否启动屏保？）
				//没有按键按下的时候 执行屏保
				ScreenFlag = 1;
				ScreenCnt = 0;
			}
			else{
				//有按键动作 则退出屏保
				ScreenFlag = 0;
				time_60s=0;	
			}	
			ButtonFlag =0;
			time_60s =0 ;
		}
		if(ScreenFlag==1&&ButtonFlag ==0)
		{
			//开始显示屏保
			if(time_5s>=3000){
				time_5s = 0;
				
				if(ScreenCnt == 3){
					ScreenCnt=0; 
				}
				Fill_RAM(0x00);
				switch (ScreenCnt){
					case 0:
						   hz_1616(0,16,chinese[172],0);
					   	   hz_1616(8,16,chinese[173],0);
					   	   hz_1616(16,16,chinese[174],0);
					   	   hz_1616(24,16,chinese[175],0);
						ascii_1608(36,16,english[18],0);
						ascii_1608(40,16,english[18],0);
						ascii_1608(44,16,english[19],0);	
					break;
					case 1:
						   hz_1616(16,32,chinese[172],0);
					   	   hz_1616(24,32,chinese[173],0);
					   	   hz_1616(32,32,chinese[174],0);
					   	   hz_1616(40,32,chinese[175],0);
						ascii_1608(52,32,english[18],0);
						ascii_1608(56,32,english[18],0);
						ascii_1608(60,32,english[19],0);	
					break;	
					break;
					case 2:
						   hz_1616(0,48,chinese[172],0);
					   	   hz_1616(8,48,chinese[173],0);
					   	   hz_1616(16,48,chinese[174],0);
					   	   hz_1616(24,48,chinese[175],0);
						ascii_1608(36,48,english[18],0);
						ascii_1608(40,48,english[18],0);
						ascii_1608(44,48,english[19],0);	
					break;
				}
				ScreenCnt++;
			}
		}
	}
}


//以下为参数自学习程序

//返回1表示阀位到位，返回0表示还没到位
int motor_auto_run(int now , int set)
{
	static int SM = 0;
	if(SM==0)
	{
		if(ValvePosValue < set - 10)
		{	
			OpenValve();
			SM = 1;		//状态机=1  表示开始运行
		}
		else if(ValvePosValue > set + 10)
		{
			CloseValve();		
			SM = 1;		//状态机=1  表示开始运行	
		}
		else		//已经在1%误差之内，电机不动
		{
			SM=0;
			return 1;
		}		
		return 0;
	}
	if( SM==1 )		//电机运动之后判断是否到达设定值
	{
		if(( now < set && ValvePosValue > set ) || ( now > set && ValvePosValue < set )	)
		{
			SSRSTOP();
			SM = 0 ;
			return 1;
		}
	}		
}

void para_auto_set(void)
{
	if(AutoSM == 0)	//状态0读取当前编码器值
	{
		pos_now = ValvePosValue;
		pos_set = 100;
		AutoSM = 1;
	}
	if(AutoSM == 1)//状态1 将阀位调整到100（10%）
	{
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			//pos_now = ValvePosValue;
			AutoSM = 2;
		}
	}
	if(AutoSM == 2 )  //wait for motor stop
	{
		if(SPEED == 0 && motor_wait_cnt == 73)
		{
			pos_now = ValvePosValue;
			AutoSM = 3;
		}
	}
	if(AutoSM == 3 )	//记录 8%升8次  依次记录  
	{
		pos_set = pos_now + 50;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 4;
		}	
	}
	if( AutoSM == 4 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 8) AutoSM = 5;
			else AutoSM = 3;
		}		
	}
	if(AutoSM == 5 )	//记录 8%降8次  依次记录  
	{
		pos_set = pos_now - 50;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 6;
		}	
	}
	if( AutoSM == 6 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 16 ) AutoSM = 7;
			else AutoSM = 5;
		}		
	}
	if(AutoSM == 7 )	//记录 15%升4次  依次记录  
	{
		pos_set = pos_now + 150;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 8;
		}	
	}
	if( AutoSM == 8 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 20) AutoSM = 9;
			else AutoSM = 7;
		}		
	}
	if(AutoSM == 9 )	//记录 15%降4次  依次记录  
	{
		pos_set = pos_now - 150;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 10;
		}	
	}
	if( AutoSM == 10 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 24 ) AutoSM = 11;
			else AutoSM = 9;
		}		
	}
	if(AutoSM == 11 )	//记录 35%升2次  依次记录  
	{
		pos_set = pos_now + 350;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 12;
		}	
	}
	if( AutoSM == 12 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 26) AutoSM = 13;
			else AutoSM = 11;
		}		
	}
	if(AutoSM == 13 )	//记录 35%降2次  依次记录  
	{
		pos_set = pos_now - 350;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 14;
		}	
	}
	if( AutoSM == 14 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 28 ) AutoSM = 15;
			else AutoSM = 13;
		}		
	}
	if(AutoSM == 15 )	//记录 70%升1次  依次记录  
	{
		pos_set = pos_now + 700;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 16;
		}	
	}
	if( AutoSM == 16 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 29) AutoSM = 17;
			else AutoSM = 15;
		}		
	}
	if(AutoSM == 17 )	//记录 70%降1次  依次记录  
	{
		pos_set = pos_now - 700;
		motor_auto_run_states = motor_auto_run( pos_now , pos_set);
		if( motor_auto_run_states == 1 )
		{
			AutoSM = 18;
		}	
	}
	if( AutoSM == 18 )
	{
		if(SPEED == 0 && motor_wait_cnt == 73 )
		{
			pos_now = ValvePosValue;
			error_array[error_array_cnt] = pos_now - pos_set;
			error_array_cnt ++;
			if( error_array_cnt == 30 ) AutoSM = 19;
			else AutoSM = 17;
		}		
	}

	if( AutoSM == 19 ) 
	{
		auto_set_flag = 2;//2表示自整定完成	   3     
		
		ParaArray[CloseError] = (error_array[8]+error_array[9]+error_array[10]+error_array[11]+error_array[12]+error_array[13]+error_array[14]+error_array[15]+error_array[20]+error_array[21]+error_array[22]+error_array[23]+ error_array[26]+error_array[27]+error_array[29])/15;
		ParaArray[OpenError] = (error_array[0]+error_array[1]+error_array[2]+error_array[3]+error_array[4]+error_array[5]+error_array[6]+error_array[7]+error_array[16]+error_array[17]+error_array[18]+error_array[19]+ error_array[24]+error_array[25]+error_array[28])/15;
		ParaArray[SelfLearnRec] = HavedSelfLearn ;	//参数用来表示自学习过了 ,每次设置了行程之后，就复位这个标志
		auto_set_exit();
	}
}

//wz  参数自学习程序结束
