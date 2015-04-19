#ifndef PARA_H
#define PARA_H

#include "stm32f10x.h"
#include "stdio.h"

//表示参数部分长度（包括两个crc的）
//51是参数总个数，在SPI_FLASH_BufferRead做参数的时候乘以4 是因为每个参数是32bits的int，而要适应8bits的flash写函数，故这样操作
#define  ParaArrayLength  	(59)

//参数表的第0位没有用，有32个字节也可以存储CRC和长度
//先用来表示修改了多少次  
#define  HaveSetTimes       0

//基本参数*********************************************************
//开向限位 关向限位
//行程、转矩
#define  OpenDirLimit   	1
#define  CloseDirLimit  	2
#define  journey        	101 
#define  torque				102

////紧急控制方式、设定信号丢失
//保位、指定位置
#define  EmergencyMode  	3
#define  SetSigLose     	4
#define  hold				301
#define  assign				302

//中间位置1
//中间位置2
//开向设置转矩
//关向设置转矩
#define	 MidPosition1   	5
#define	 MidPosition2		6
#define	 OpenDirJour		7
#define	 CloseDirJour		8

//控制参数部分
//远程控制模式：脉冲式、维持式、双位式、模拟量
#define	 FarawayMode    	9
#define	 LocallyMode		10
#define	 pluse_mode     	901
#define  hold_mode      	902
#define  pc_mode    	903
#define  analog_mode		904

//信号标定部分 
//反馈值4ma所表示的位置	 反馈值20ma所表示的位置	 设定值4ma	设定值20ma	 
#define  Feedback4maPos    	11		
#define  Feedback20maPos 	12
#define  Set4ma 			13
#define  Set20ma 			14

//阀位标定 0%  100%
#define  SetValveMin 		15
#define  SetValveMax 		16

//I\O设定部分*****************************************************
//六个输出的常开常闭状态
#define  OutNormalState0	17
#define  OutNormalState1	18
#define  OutNormalState2	19
#define  OutNormalState3	20
#define  OutNormalState4	21
#define  OutNormalState5	22

#define  OutNormalState_open  171
#define  OutNormalState_close 172

//六个开关信号输出表示的12种状态
#define  OutExpState0		23
#define  OutExpState1		24
#define  OutExpState2		25
#define  OutExpState3		26
#define  OutExpState4		27
#define  OutExpState5		28
//子项：
//总故障  //远方故障  //电源缺相  //阀位故障   //电机过热故障  //中间位置1	//中间位置2	 
//开向行程限位	 //关向行程限位	//开向行程限位	//关向行程限位	 //设定值故障	  //未定义
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

//其他部分
//是否恢复出厂设置 //是否清除警报
#define  Restore			29
#define  ClearAlarm 		30
//子项：是、否
#define  YorN_yes			291
#define  YorN_no			292

//故障部分

//故障计数器
//总故障  //远方故障  //电源缺相  //阀位故障 //电机过热故障	//中间位置1 //中间位置2	 
//开向行程限位	//关向行程限位	//开向行程限位 //关向行程限位 //设定值故障
//开向行程限位	 //关向行程限位	 //开向行程限位	 //关向行程限位
#define  TotalFaultCount 			31		
#define  FarawayFaultCout 			32		
#define  LackPhaseFaultCount 		33		
#define  ValveLocationFaultCount 	34	
#define  MotorOverHeartFaultCount 	35
#define  MidPosition1FaultCount 	36		
#define  MidPosition2FaultCount 	37
	
#define  OpenDirJourneyLimitCount 	38
#define  CloseDirJourneyLimitCount 	39
#define  OpenDirTorqueLimitCount 	40	
#define  CloseDirTorqueLimitCount 	41
#define  SetValueFaultCount 		42		

#define  OpenDirJourneyLimitFaultCount 	43
#define  CloseDirJourneyLimitFaultCount 44
#define  OpenDirTorqueLimitFaultCount 	45	
#define  CloseDirTorqueLimitFaultCount 	46	

//转矩AD值	 //出厂编码	  //硬件版本  //软件版本
#define  JourneyADCalue 			47				
#define  FactoryCode 				48
#define  HardwareVersion 			49			
#define  SoftwareVersion 			50

//wz 后来增加的
#define  Feedback8maDty 			51	
#define  Feedback16maDty			52

#define  CloseError					53
#define  OpenError					54
#define  SelfLearnRec 				55
#define  HavedSelfLearn				551
#define  NoHavedSelfLearn			552

//密码
#define  Security 					56

//定义CRC位置，用来校验所有参数是否读取正确	 （这里是定义成8bbit还是32bit？？）
#define  CRC_H						57
#define  CRC_L						58	

extern int ValvePosValue ;		//三位数，最后一位是小数位
extern int ValvePosValueLast; 
extern int ValveSetExecute_ValveRec;
extern int ParaArray[];
extern int ParaArray_copy[];

extern int AD654F;

extern int NewSecurity ;
extern int NewSecurityAgain ;

extern int ssr0_750msflag ;
extern int ssr1_750msflag ;

extern int ValveOpenLedFlag ;
extern int ValveCloseLedFlag ;


void ParaInit(void);
void ParaWriteToFlash(void);
int BufferCmp(int* pBuffer1, int* pBuffer2, int BufferLength);

/*

//基本参数*********************************************************
//开向限位 关向限位
typedef enum DirLimit_def{
	journey,	//行程
	torque		//转矩
}DirLimit;

//紧急控制方式、设定信号丢失
typedef enum Danger_def{
	hold,  		//保位
	assign,		//指定位置
}Danger;

//远程控制模式：脉冲式、维持式、双位式、模拟量
typedef enum ControlMode_def{
	pluse_mode,
	hold_mode,
	double_mode,
	analog_mode
}ControlMode;

extern int ValvePosValue ;		//三位数，最后一位是小数位
extern int ValvePosValueLast; 
extern int MidPosition1 ;		//中间位置1
extern int MidPosition2 ;		//中间位置2
extern int OpenDirJour ;		//开向设置转矩
extern int CloseDirJour ;		//关向设置转矩
*/

//控制参数部分****************************************************
//就地控制模式：脉冲式、维持式
/*
enum LocallyControlMode{
	pluse_mode,
	hold_mode,
}LocallyMode;	*/

						 /*
//I\O设定部分*****************************************************
//六个输出的常开常闭状态
typedef enum Normal_def{
	close,
	open
}Normal;

extern int Feedback8ma ;	//反馈值8ma		
extern int Feedback16ma ;	//反馈值16ma
extern int Set8ma ;			//设定值8ma
extern int Set16ma ;		//设定值16ma

extern int SetValveMin ;	//阀位0%
extern int SetValveMax ;	//阀位100%

//开关信号输出表示的12种状态
typedef enum FaultType_def{
	TotalFault, 		//总故障
	FarawayFault,		//远方故障
	LackPhaseFault,		//电源缺相
	ValveLocationFault,	//阀位故障
	MotorOverHeartFault,//电机过热故障
	MidPosition1Fault,		//中间位置1
	MidPosition2Fault,		//中间位置2
	OpenDirJourneyLimit,//开向行程限位
	CloseDirJourneyLimit,//关向行程限位
	OpenDirTorqueLimit,	//开向行程限位
	CloseDirTorqueLimit,//关向行程限位
	SetValueFault,		//设定值故障
	NoDefined			//未定义
}FaultType;

//故障计数器
extern int	TotalFaultCount; 		//总故障
extern int	FarawayFaultCout;		//远方故障
extern int	LackPhaseFaultCount;		//电源缺相
extern int	ValveLocationFaultCount;	//阀位故障
extern int	MotorOverHeartFaultCount;//电机过热故障
extern int	MidPosition1FaultCount;		//中间位置1
extern int	MidPosition2FaultCount;		//中间位置2
extern int	OpenDirJourneyLimitCount;//开向行程限位
extern int	CloseDirJourneyLimitCount;//关向行程限位
extern int	OpenDirTorqueLimitCount;	//开向行程限位
extern int	CloseDirTorqueLimitCount;//关向行程限位
extern int	SetValueFaultCount;		//设定值故障

extern int	OpenDirJourneyLimitFaultCount ;//开向行程限位
extern int	CloseDirJourneyLimitFaultCount ;//关向行程限位
extern int	OpenDirTorqueLimitFaultCount ;	//开向行程限位
extern int	CloseDirTorqueLimitFaultCount ;	//关向行程限位

extern int JourneyADCalue;				//转矩AD值
extern int FactoryCode ;				//出厂编码
extern int HardwareVersion;			//硬件版本
extern int SoftwareVersion ;			//软件版本

//其他设置部分****************************************************
//是否恢复出厂设置、是否清除报警
typedef enum YorN_def{
	yes,
	no
}YorN;

extern DirLimit OpenDirLimit,CloseDirLimit;
extern Danger EmergencyMode,SetSigLose;
extern ControlMode FarawayMode,LocallyMode;
extern Normal OutNormalState[6];
extern FaultType OutExpState[6];
extern YorN Restore,ClearAlarm;

//void IntTo4Char(int x,unsigned char* y);
int ParaToBuffer(uint8_t* pBuffer,uint16_t BufferLength);
//从缓冲区读取一批参数
int ReadParaFromBuffer( uint8_t* pBuffer);

void WriteParaToFlash(void);
void ReadParaFromFlash(void);	 */


#endif
