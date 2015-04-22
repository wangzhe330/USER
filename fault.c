/*
	故障检测模块
	对系统各种故障进行检测，并且计数、显示
	故障项：
	1.远方故障：
	2.电源缺相：这里是单相电源
	3.阀位故障：
	4.电机过热故障：温度传感器？？
	5.中间位置1：
	6.中间位置2：？？
	7.开向行程限位：超过行程限位之后故障报警。并且不能够再执行动作。
	8.关向行程限位：
	9.开向转矩限位：超过转矩限位之后故障报警。并且不能够再执行动作。
	10.关向转矩限位：
	11.设定值故障：设定值故障，a.针对设置的参数进行逻辑上的检测  b.对远程控制中的电流值进行检查

	**测试报告中重点提到的三个故障：
	信号中断
	信号超限
	力矩过载


	故障计数器
	//总故障  //远方故障  //电源缺相  //阀位故障   //电机过热故障  //中间位置1	//中间位置2	 
//开向行程限位	 //关向行程限位	//开向转矩限位	//关向转矩限位	 //设定值故障	  //未定义
	#define	 TotalFault				231 		
	#define	 FarawayFault			232		
	#define  LackPhaseFault			233
	#define	 ValveLocationFault 	234	
	#define	 MotorOverHeartFault	235
	#define	 MidPosition1Fault		236		
	#define	 MidPosition2Fault		237
	#define	 OpenDirJourneyLimit	238
	#define	 CloseDirJourneyLimit	239
	#define	 OpenDirTorqueLimit		240	
	#define	 CloseDirTorqueLimit	241
	#define	 SetValueFault			242		
	#define	 NoDefined				0
*/
#include "fault.h"
#include "led.h"
#include "oled.h"
#include "ssr.h"
#include "io.h"

//先做：信号中断、信号超限
int FaultTotal = 0;
int FaultShowFlag = 0;

extern int ValvePosValue;

//故障显示
void FaultShow(int x )
{
	//对每个故障的进行轮询检测 ,并且显示
	//信号中断
	/*
	static int cnt = 1;
	static int  sum = 0 ;
	int sumcnt = 0;
	int i = 0 , temp = 0 ;
	
		temp = FaultTotal ;
		sum = 0;
		for( i = 0 ; i < 16 ; i++ )
		{
			sum += (temp>>i) & 0x01;
		}
		
		if(sum != 0 ){
			for( i = 0 ; i < 16 ; i++ )
			{
				sumcnt += (temp>>i) & 0x01;
				if(sumcnt == cnt)
					break;
			}
			switch(i){
				case 1:
					hz_1616( 8 , 32 , chinese[29],0);
					hz_1616( 16 , 32 , chinese[30],0);
					hz_1616( 24 , 32 , chinese[27],0);
					hz_1616( 32 , 32 , chinese[152],0);
					hz_1616( 40 , 32 , chinese[17],0);
					hz_1616( 48 , 32 , chinese[18],0);
				case 2:
					hz_1616( 8 , 32 , chinese[29],0);
					hz_1616( 16 , 32 , chinese[30],0);
					hz_1616( 24 , 32 , chinese[153],0);
					hz_1616( 32 , 32 , chinese[154],0);			
					hz_1616( 40 , 32 , chinese[17],0);
					hz_1616( 48 , 32 , chinese[18],0);									
	
			}
			cnt ++;
		}
	
		if(cnt == sum + 1) cnt = 0;	  */
	
	if( x & FaultSigBreakMask ){
		hz_1616( 8 , 32 , chinese[29],0);
		hz_1616( 16 , 32 , chinese[30],0);
		hz_1616( 24 , 32 , chinese[27],0);
		hz_1616( 32 , 32 , chinese[152],0);
		hz_1616( 40 , 32 , chinese[17],0);
		hz_1616( 48 , 32 , chinese[18],0);
	}
	//信号超限
	if( x & FaultSigOverMask ){
		hz_1616( 8 , 32 , chinese[29],0);
		hz_1616( 16 , 32 , chinese[30],0);
		hz_1616( 24 , 32 , chinese[153],0);
		hz_1616( 32 , 32 , chinese[154],0);			
		hz_1616( 40 , 32 , chinese[17],0);
		hz_1616( 48 , 32 , chinese[18],0);
	}
	if( x & FaultJourneyOverMask ){
		hz_1616( 8 , 32 , chinese[76],0);
		hz_1616( 16 , 32 , chinese[127],0);
		hz_1616( 24 , 32 , chinese[153],0);
		hz_1616( 32 , 32 , chinese[154],0);			
		hz_1616( 40 , 32 , chinese[17],0);
		hz_1616( 48 , 32 , chinese[18],0);		
	}
	if(x == 0)
	{		
		hz_1616( 8 , 32 , chinese[92],0);
		hz_1616( 16 , 32 , chinese[92],0);
		hz_1616( 24 , 32 , chinese[92],0);
		hz_1616( 32 , 32 , chinese[92],0);
		hz_1616( 40 , 32 , chinese[92],0);			
		hz_1616( 48 , 32 , chinese[92],0);

	} 	
}



//故障检测函数
void FaultDetect(void)
{
	//有故障的情况下  	
	if( FaultTotal != 0 )
	{
		//黄灯亮
		LED2(ON);
		LED3(OFF);
		LED1(OFF);			
		//。SSR供电继电器断开
		
		//io输出是哪一个故障

		RelayOut3(CLOSE);
		/*
		if( FaultTotal & FaultSigBreakMask )
			RelayOut4(CLOSE);
		if( FaultTotal & FaultSigOverMask )				
			RelayOut5(CLOSE);
		if( FaultTotal & FaultJourneyOverMask )
			RelayOut0(CLOSE); */
		SSRPowerOff();

	}
	else
	{
		SSRPowerOn();
		//故障解除 
		LED2(OFF);
		//ssr上电
	   	if( ValvePosValue <= 0 )
		{
			LED3(ON);
			LED1(OFF);							
		}
		if(ValvePosValue >=1000)
		{
			LED3(OFF);
			LED1(ON);
		}

		//io输出清除

		RelayOut3(OPEN);

	}
}