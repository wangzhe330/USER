#include "modbus.h"
//#include "usart.h"
#include "para.h"
#include "stdio.h"
#include "stdlib.h"

int cmpArray(u8 *a,const u8 *b, int len)
{
	int i=0;
	if(a==NULL||b==NULL)
		return 0;

	for(;i<len;i++){
		if( *(a+i)!=*(b+i) )
			return 0;
	}	
	return 1;
}

const u8 OpenValveaCmd[] = {0x05,0x00,0x00,0xff,0x00,0x8c,0x3a}; 
const u8 CloseValveaCmd[] = {0x05,0x00,0x01,0xff,0x00,0xdd,0xfa}; 

void DealMessage(void)
{
	if(cmpArray(ModbusCommand+1,OpenValveaCmd,7))		//第一位是地址为，在接收的时候只有地址位才接收，所以这里不用判断了。
	{
		Serial3PutChar(0x50);		
		//开阀门		
	}
	else if(cmpArray(ModbusCommand+1,CloseValveaCmd,7))		//第一位是地址为，在接收的时候只有地址位才接收，所以这里不用判断了。
	{
		Serial3PutChar(0x51);		
		//关阀门
	}
}