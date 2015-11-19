#include "pwm.h"
#include "current.h"
#include "para.h"

float current_in_val = 0;
int current_set_valve_val = 0;
int ValveSetVal = 0;
int ValveSetValRec[4] = {0};
int ValveSetValRecFlag = 0;
int ValveSetValCmpCnt = 0;
int ValveSetValTemp[25];
int ValveSetValTempCnt = 0;
int ValveSetExecuteFlag = 0;	//阀位设定执行标志位 0 为未执行，1 为已执行

int ad654cnt = 0 ;

//阀位反馈 输出电流
/*
void ValveFreedback(int v)
{
	float current = 0.0;

	current = (float)v/1000.0*16.0+4.0;
	current_out( current );	
} */
//根据编码器的值来输出反馈电流
void ValveFreedback(int e)
{
	float current = 0.0;

	current = ((float)(e - ParaArray[Feedback4maPos]))/((float)(ParaArray[Feedback20maPos] - ParaArray[Feedback4maPos]))*16.0 + 4.0;

	//current = (float)v/1000.0*16.0+4.0;
	current_out( current );	
}

int current_out(float x)
{
	float dty = 0;
	//wz  //bug	  这里全开
	if(x<20.0&&x>4.0)
	{
		//wz这里应该加入对信号模块是否设置过的检查，如果设置过，就用flash中的参数去计算，如果没有设置过，还是要用一个固定的式子
		//dty = 2880.0 - 8.0*x;
		//dty = 5768.0 - 16.0*x;
		//dty = ( x*100.0 - (float)ParaArray[Feedback8ma])/(  (float)ParaArray[Feedback16ma] - (float)ParaArray[Feedback8ma])*( (float)ParaArray[Feedback16maDty]-(float)ParaArray[Feedback8maDty])+ (float)ParaArray[Feedback8maDty];
		if ( x < 4.43)
			dty = 5720 - (int)(13.79*(x-2.98));
		else 
			dty  = (int)( 5757.0 - 11.8*x -0.1672*x*x );

	}
	else if( x <= 4.0 )
	{
		//guzhang  这里是不是要报警？但是阀位超行程应该已经报警	了
		dty = 5706;
	}
	else if(x >= 20.0)
	{
		dty = 5454;
	}
	TIM_SetCompare1(TIM1, (int)dty);//y=2880-8*x (x为电流，y为占空比),2880~0mA,2832~4mA,2720~20mA 
	return dty;
}

//
//TIM1_FREQ :180 对应100mV 对应4mA					   
//TIM1_FREQ :920 对应500mV 对应20mA

float get_current_in(void)
{					   	//return ((float)TIM1_FREQ - 180.0)/740.0*16.0 + 4.0;
	float re = 0.0;
	re = 4.0+16.0*( (float)TIM1_FREQ - (float)ParaArray[Set4ma] )/( (float)ParaArray[Set20ma] - (float)ParaArray[Set4ma] );	
	return re;
}

//阀位在系统内部都是以1000为满度的
int current_to_valve(void)
{
	float temp = 0.0;
	//temp = ((float)TIM1_FREQ - 180.0)/740.0 * 1000.0;// + 4000.0;
	temp = ( (float)TIM1_FREQ - (float)ParaArray[Set4ma] )/( (float)ParaArray[Set20ma] - (float)ParaArray[Set4ma] )*1000.0;	
	return (int)temp;		
}