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

//ȫ�ֱ���
extern Menu_Option option[6];
extern Menu_Label ch_label[8];
extern Menu_InputOp input_op[5];		//�����������
extern M_Page Page[PageSum];

extern int ad654rec[10];
	
volatile u32 time; // ms ��ʱ����
volatile u16 time_10ms;	 //10ms ��ʱ������

//����������
u16 EncoderData = 0;
u16 temp = 0;

int auto_set_flag = 0;
int AutoSM = 0; //��ѧϰ״̬��

int pos_now=0; //��ѧϰ�㷨��������¼��ǰλ����Ϣ�ı���
int pos_set=0;
int error_array[32];//��ѧϰ�㷨�У�������¼���ֵ������
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

//��ʼ��������������ʼ��ϵͳ�ĸ���ģ��
void ValveSysInit()
{
	/* config the sysclock to 72mhz */
	SystemInit();
	/* config led gpio */
	LED_GPIO_Config();
	/* ����SysTick Ϊ1us�ж�һ�� */
	SysTick_Init();

	OLED_GPIO_Init();		//OLED���ų�ʼ��
	OLED_Init();
	SSR_GPIO_Config();

	IOConfig();

	//Key_GPIO_Config();
	KEY_EXTI_Config();	
	
	//TIM1 4-20mA�����뷢����
	TIM1_PWM2_Init();
	TIM_SetCompare1(TIM1, 2848);
	
	//while(1);

	//time2����
	TIM2_NVIC_Configuration();
	TIM2_Configuration();

	//TIM3����20ms�ж�
	TIM3_Int_Init(99,719);//100Khz�ļ���Ƶ�ʣ�������99Ϊ1ms	
	//START_TIME;
	//����2���� ��ȡ����������
	USART2_Config();
	//����1����
	uart1_init(115200);

	u3_init();

	//SPI-FLASH��ʼ��������W25X16
	SPI_FLASH_Init();
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay_ms(50);
	FlashID = SPI_FLASH_ReadID();
	if (FlashID != sFLASH_ID)
	{
		//����ȥ��while1  �ĳ�FLASH����
		while(1);
	}	
	/*
	time=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	*/
	
	PageInit();
	AreaInit();

	//������ʼ������FLASH�ж�ȡ����
	ParaInit();
	//�̵��������ʼ��
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
	//Fill_Block(0,40,0,32,0x00);  //����������кţ��ص��кţ�����кţ��յ��кţ����Ҷ�ֵ
	//while(1);

	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_2 );
	//GPIO_SetBits(GPIOC, GPIO_Pin_2 );
	//GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
	//while(1);

	//��ȡ���������������״̬��ȷ���ϵ�֮���״̬
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == KEY_ON )  {	 
		Main_KEY_State = 1; 
	}else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == KEY_ON )  {	 
		Main_KEY_State = 2;   
	}else{
		Main_KEY_State = 0;
	}

	Main_State_Change = 1;	//��һ�����У������ȥ������ʾʲô����

	START_TIME;

	//ssr0_750msflag = 1;

	//while(1);

  	while(1){
	

		//���������������״̬�仯
		/*
		if( Main_State_Change == 1)
		{
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 0 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 1)   //�͵�ģʽ
				Main_KEY_State = 1;
			else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 1 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 0)   //����Զ��  
				Main_KEY_State = 2;
			else 
				Main_KEY_State = 0;

			if(Main_KEY_State == 0)
			{
				page_mode();				
				SSRSTOP();
			}
			if( Main_KEY_State == 1 )
			{
				local_mode();
				FaultShow(FaultTotal);
				SSRSTOP();
			}
			if( Main_KEY_State == 2)
			{
				faraway_mode();	 
				FaultShow(FaultTotal);
				SSRSTOP();		  
			}
					
			Main_State_Change = 0;
		}  
		*/
		
		//���������¼�
		if(KEY_Flag == 1)	 
		{
			if(Main_KEY_State == 0)	   //����ģʽ
			{
				//wz ��������һ��С���� 181~191
				if(AreaMode == 0)
				{
					if(InputMode == 0)
						PageKeyResponse(KEY);
					else if(InputMode == 1)
						InputKeyResponse(KEY);
				}
				else if(AreaMode == 1)		//��areaģʽ�£�ÿ���������������µ�����
				{
					if(InputMode == 0)
						AreaKeyResponse(KEY);									
					else if(InputMode == 1)
						InputKeyResponse(KEY);	
				}
				
				//bug�� û��else��
				KEY_Flag = 0;
			}
			if( Main_KEY_State == 1)   //�͵�ģʽ
			{
				LocalKeyResponse(KEY);
				KEY_Flag = 0;

			}		
			if( Main_KEY_State == 2)   //Զ��ģʽ
			{
				FarawayKeyResponse(KEY);
				KEY_Flag = 0;	
			}
		 }

		//��ʱ��������
		if ( time >= 1000 ) /* 1s ʱ�䵽 */
		{
			time = 0;
			/* LED1 ȡ�� */      
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, 
					(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8))));
				 
		} 

		//ÿ20ms��ȡһ�α�������ֵ��������ʾ
		if( time % 20 == 0 )
		{
			//-----------------��ȡһ�α�������ֵ����ʾ
			GetEncoderPara();
			Delay_ms(2);

			EncoderData = UartData[1];
			EncoderData = EncoderData<<8;
			EncoderData += UartData[0];
			temp = EncoderData;	
			EncoderCommEnd();
			 		
			ValvePosValueLast = ValvePosValue;

			//ValvePosValue = (int)(((float) EncoderData - 46854.0 )/9810.0*1000.0);
			//0%�ı�����ֵ �Ǳ� 100%�ı�����ֵ ���
			if(ParaArray[SetValveMin] > ParaArray[SetValveMax])
			{ 				
				ValvePosValue = (int)(((float) EncoderData - (float)ParaArray[SetValveMin] )/(float)(ParaArray[SetValveMax]-ParaArray[SetValveMin])*1000.0); 	
				//if(ValvePosValue>1000 && ValvePosValue<1002)
				//	ValvePosValue =1000;
			}
			else
			{
				//guzhang ����Ӧ�ø���һ���趨ֵ����
				ValvePosValue = (int)(((float) EncoderData - 46854.0 )/9810.0*1000.0);
			}

			//wz 0% ��λ
			if( ValvePosValue < 0 && ValvePosValue > -3)
				ValvePosValue = 0;
			if( ValvePosValue > 1000 && ValvePosValue < 1003)
				ValvePosValue = 1000;
			//wz bug ��д�����else ���������趨ֵ����
			
			SPEED =  ValvePosValue - ValvePosValueLast;		//����ٶ�
			if( SPEED<0 ) SPEED = 0-SPEED;

			ValvePosShow();

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

			if(FarawaySM == 2)		//ֹͣ�źŸ���֮���ж��ǲ�����ȫͣ����
			{
				if( SPEED == 0)
				{
					//ValveError  = ValveError + ValvePosValue - ValveSetVal;		//�����һ��ֹͣ����֮��ľ�̬���
					ValveError  =  ValvePosValue - ValveSetVal;		//�����һ��ֹͣ����֮��ľ�̬���
					STOP_FLAG = 0;
					//FarawaySM = 3;	//״̬�� = 3����ʾ��ȫͣ�����ˣ���¼���
					FarawaySM = 0;		//״̬��=0  ��ʾ�������ò���
				}
			}
			//Զ��ģʽȥ��ȡ�趨ֵ��ÿ20ms*20=400msһ��
			if(Main_KEY_State == 2)
			{
				if( ParaArray[FarawayMode] == analog_mode)
				{	
					//ValveSetValTemp[ValveSetValTempCnt] = current_to_valve();
					ValveSetValTempCnt ++;
					if(ValveSetValTempCnt == 10)
					{ //wz 2015-3-23 20 //25
						AD654ToCurrent();
						ValveSetShow();	//�趨ֵ��ʾ				
						ValveSetValTempCnt = 0;
						//wz
						//printf("%d \n        ",ValvePosValue);
						//�ж��Ƿ����Ĵ����
						//if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) == KEY_ON){
	//wz11
						if( IO2State() == LINK )
						{
							if(ValveSetVal > 0  ){  //&& ValveSetVal != 1000
								if(ValvePosValue < ValveSetVal - 10)
								{	
									
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//״̬��=1  ��ʾ��ʼ����
									ValveSetExecute_ValveRec = ValvePosValue;	//��¼��λ��ʼ�����ĳ�ʼ��λ�Ƕ��٣������Ƚ��Ƿ�ֹͣ�˶�
									OpenValve();
								}
								else if(ValvePosValue > ValveSetVal + 10)
								{							
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//״̬��=1  ��ʾ��ʼ����
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();	
								}
							}	 
							else if(ValveSetVal <= 0)
							{
								//ȫ�����Ҫ�����ж��Ƿ���ȫ�ر�
								//wz4-17
								if(ValvePosValue > 0 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//״̬��=1  ��ʾ��ʼ����
									ValveSetExecute_ValveRec = ValvePosValue;
									CloseValve();				
								}					
							} 	 /*
							else if(ValveSetVal >= 1000)
							{
								//ȫ�����Ҫ�����ж��Ƿ���ȫ��
								//wz4-17
								if(ValvePosValue < 1000 ){
									ValveSetExecuteFlag = 1;
									FarawaySM = 1;		//״̬��=1  ��ʾ��ʼ����
									ValveSetExecute_ValveRec = ValvePosValue;
									OpenValve();
								}								
							}	*/				 
						}
		 	
						//}
					}
				}
			}
			if(Main_KEY_State == 1 || Main_KEY_State == 2)
			{
				if(FaultShowFlag == 1)
					FaultShow(FaultTotal);
			} 
			//else{ ValveSetValTempCnt = 0; } //�ڱ��ģʽ����£��Ѽ��������㣬��ֹ��һ�εĸ��Ŵ��� 

			motor_wait_cnt++;
			if(motor_wait_cnt == 75 ) motor_wait_cnt = 0;
		}

		//ÿ10ms��ȡһ�����������״̬ѡ�񰴼���״̬����������ж��ж�����������ȫ����˵Ļ������԰�����Ĳ�ѯȥ�����������жϵķ�ʽ
		if(time_10ms >= 10)
		{
			//�����ȡ֮����Խ������ж�Զ��ģʽ�Ƿ�����˶�������  .//WZ
			
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

		//���������������״̬�仯
		if( Main_State_Change == 1)
		{
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 0 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 1)   //�͵�ģʽ
				Main_KEY_State = 1;
			else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == 1 && GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) == 0)   //����Զ��  
				Main_KEY_State = 2;
			else 
				Main_KEY_State = 0;

			if(Main_KEY_State == 0)
			{
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
		}  
	
		//���ϼ�� 
		
		FaultDetect();

		if(Main_KEY_State == 2  )
		{
			if(ParaArray[FarawayMode] == analog_mode)
			{		
				if( FarawaySM == 1 )
				{
					if( IO2State() == NOLINK )
					{
						SSRSTOP();
						STOP_FLAG = 1;
						FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת					
					}
					else if( IO2State() == LINK )
					{
						//if(( ValvePosValue < ValveSetVal && ValvePosValue > ValveSetVal - 17 ) || ( ValvePosValue>ValveSetVal && ValvePosValue <ValveSetVal + 19 )	)
						//if(( ValvePosValue < ValveSetVal && ValvePosValue > ValveSetVal - (int)(17.0*( 1000.0/((float)ParaArray[SetValveMax]-(float)ParaArray[SetValveMin])) ) ) || ( ValvePosValue>ValveSetVal && ValvePosValue <ValveSetVal + (int)(38.0*( 1000.0/((float)ParaArray[SetValveMax]-(float)ParaArray[SetValveMin]) ) ) )	)					
	
						if( ValveSetExecute_ValveRec < ValveSetVal - 10)  //�����ǿ����Ų���
						{
							if(ValveSetVal < 1000){	
								if( ValvePosValue > ValveSetVal - ValveOpenError )
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת
								}
							}
							else if( ValveSetVal == 1000 ){
								if( ValvePosValue > ValveSetVal - ValveOpenError - 5 )
								{
									//if( ValvePosValue > ValveSetVal - 10 )
									{
										SSRSTOP();
										STOP_FLAG = 1;
										FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת
									}
								}	
							}
						}
						if(ValveSetVal != 0){
							if(ValveSetExecute_ValveRec > ValveSetVal + 10)		//����ִ�е��ǹط��Ų���
							{
								if( ValvePosValue < ValveSetVal - ValveCloseError )
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת
								}
							}
							if( ValvePosValue < 0-ValveCloseError && ValvePosValue < ValvePosValueLast )
							{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת																					
							}
						}
						else if(ValveSetVal == 0)
						{
							if(ValveSetExecute_ValveRec > ValveSetVal)		//����ִ�е��ǹط��Ų���
							{
								if( ValvePosValue < ValveSetVal - ValveCloseError - 5)
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת
								}
								if( ValvePosValue < 0)
								{
									SSRSTOP();
									STOP_FLAG = 1;
									FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת									
								}
							}								
						}
					}
				}
			}
			if(ParaArray[FarawayMode] == pluse_mode)
			{
				if( IO2State() == NOLINK )
				{
						SSRSTOP();
						STOP_FLAG = 1;
						FarawaySM = 2;		//״̬��=2  ��ʾ���ͣת					
				}
				else if(IO2State() == LINK)
				{
					if( IO0State() == LINK && IO1State() == NOLINK)
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
						//guzhang  ͬʱ���͹�
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
					//������
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
			para_auto_set();	//����������
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
			if(ValvePosValue >= 1000 && ValvePosValue < 1020)
			{
				LED3(OFF);
				LED1(ON);
				RelayOut5(CLOSE);
			}
		}


		//��λ���� ���͵�ģʽ��
		/*
		if(Main_KEY_State == 1){
			if(ValvePosValue <= 5 || ValvePosValue >= 995){
				SSRSTOP();
			}
		}						 */
		//wz �������
		//if(ValvePosValue < 20 || ValvePosValue > 980 ) SSRSTOP(); 
	}
}


//����Ϊ������ѧϰ����

//����1��ʾ��λ��λ������0��ʾ��û��λ
int motor_auto_run(int now , int set)
{
	static int SM = 0;
	if(SM==0)
	{
		if(ValvePosValue < set - 10)
		{	
			OpenValve();
			SM = 1;		//״̬��=1  ��ʾ��ʼ����
		}
		else if(ValvePosValue > set + 10)
		{
			CloseValve();		
			SM = 1;		//״̬��=1  ��ʾ��ʼ����	
		}
		else		//�Ѿ���1%���֮�ڣ��������
		{
			SM=0;
			return 1;
		}		
		return 0;
	}
	if( SM==1 )		//����˶�֮���ж��Ƿ񵽴��趨ֵ
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
	if(AutoSM == 0)	//״̬0��ȡ��ǰ������ֵ
	{
		pos_now = ValvePosValue;
		pos_set = 100;
		AutoSM = 1;
	}
	if(AutoSM == 1)//״̬1 ����λ������100��10%��
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
	if(AutoSM == 3 )	//��¼ 8%��8��  ���μ�¼  
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
	if(AutoSM == 5 )	//��¼ 8%��8��  ���μ�¼  
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
	if(AutoSM == 7 )	//��¼ 15%��4��  ���μ�¼  
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
	if(AutoSM == 9 )	//��¼ 15%��4��  ���μ�¼  
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
	if(AutoSM == 11 )	//��¼ 35%��2��  ���μ�¼  
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
	if(AutoSM == 13 )	//��¼ 35%��2��  ���μ�¼  
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
	if(AutoSM == 15 )	//��¼ 70%��1��  ���μ�¼  
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
	if(AutoSM == 17 )	//��¼ 70%��1��  ���μ�¼  
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
		auto_set_flag = 2;//2��ʾ���������	   3     
		
		ParaArray[CloseError] = (error_array[8]+error_array[9]+error_array[10]+error_array[11]+error_array[12]+error_array[13]+error_array[14]+error_array[15]+error_array[20]+error_array[21]+error_array[22]+error_array[23]+ error_array[26]+error_array[27]+error_array[29])/15;
		ParaArray[OpenError] = (error_array[0]+error_array[1]+error_array[2]+error_array[3]+error_array[4]+error_array[5]+error_array[6]+error_array[7]+error_array[16]+error_array[17]+error_array[18]+error_array[19]+ error_array[24]+error_array[25]+error_array[28])/15;
		ParaArray[SelfLearnRec] = HavedSelfLearn ;	//����������ʾ��ѧϰ���� ,ÿ���������г�֮�󣬾͸�λ�����־
		auto_set_exit();
	}
}

//wz  ������ѧϰ�������
