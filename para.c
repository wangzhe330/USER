#include "para.h"
#include "spi_flash.h"


int AD654F = 0 ;

int NewSecurity = 0;
int NewSecurityAgain = 0;
//需要一个结构体，表明哪部分被修改了

//需要一个函数，用来检查设定值错误，每一块设定值进行逻辑上的检查

int ParaArray[70]={0};
int ParaArray_copy[70]={0};//开机从FLASH读取数据之后，在这里放一个副本，最后用来对比两个数组，确定是否有改变参数，询问时候保存修改


//核心参数
//电机控制参数
int	ValvePosValue = 0;		//三位数，最后一位是小数位 
//前一次的编码器值
int	ValvePosValueLast = 0;		//三位数，最后一位是小数位

int ValveSetExecute_ValveRec = 0 ; // 三位数，记录开始执行远程命令的初始阀位

extern int tempmax , tempmin ;
extern float weight ;
extern int ValveOpenError,ValveCloseError;
extern float set_k;	

int ssr0_750msflag = 0;
int ssr1_750msflag = 0;

int ValveOpenLedFlag = 0;
int ValveCloseLedFlag = 0;

int PCMode = 0;		
int PCHoldOpenSig = 0;	//维持式：开信号
int PCHoldCloseSig = 0;	//维持式：关信号
int PCPluseOpenSig = 0;	//脉冲式：开信号
int PCPluseCloseSig = 0;//脉冲式：关信号
int PCPluseStopSig = 0 ;//脉冲式：停信号
int PCAllOpenSig = 0;	//全开信号
int PCAllCLoseSig = 0;	//全关信号
int PCSetSig = 0;		//设定 模式
int PCSetValve = 0; 	//设定值

//参数初始化，应该是去flash读取所有参数	  
void ParaInit(void)
{
	int i=0;
	SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//在ParaArray_copy[]中做一个备份，用来对比
	for( i=0;i<ParaArrayLength ; i++ ){
		*(ParaArray_copy+i) = *(ParaArray+i);
	}
	if( ParaArray[SelfLearnRec] == NoHavedSelfLearn )
	{
		tempmax = ParaArray[SetValveMax];
		tempmin = ParaArray[SetValveMin];
		weight = 10000.0/((float)tempmax - (float)tempmin);
		//这里要的只是一个绝对值
		if( weight < 0 )
			weight = 0-weight;
		//error1 = (int)(17.0*weight);
		//error2 = (int)(18.0*weight);
		ValveOpenError = (int)(19.0*weight);
		ValveCloseError = 0-(int)(19.0*weight);
	}else if(ParaArray[SelfLearnRec] == HavedSelfLearn ){
		ValveOpenError = ParaArray[OpenError];
		ValveCloseError = ParaArray[CloseError];	//换了一下位置		是个负数
	}
	
	set_k = 1000.0/((float)ParaArray[Set20ma] - (float)ParaArray[Set4ma]);		
}

//两个int数组做比较，如果不相同则返回1，如果相同则返回0
int BufferCmp(int* pBuffer1, int* pBuffer2, int BufferLength)
{
	while(BufferLength--)
	{
		if(*pBuffer1 != *pBuffer2)
		{
		  return 1;
		}
		
		pBuffer1++;
		pBuffer2++;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------
//WZ 2015-3-19 重新修改了参数表以及参数存储flash的方式
//一下为原来的参数定义形式，现在修改为哈希形式
