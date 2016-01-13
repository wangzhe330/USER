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
	if(cmpArray(ModbusCommand+1,OpenValveaCmd,7))		//��һλ�ǵ�ַΪ���ڽ��յ�ʱ��ֻ�е�ַλ�Ž��գ��������ﲻ���ж��ˡ�
	{
		Serial3PutChar(0x50);		
		//������		
	}
	else if(cmpArray(ModbusCommand+1,CloseValveaCmd,7))		//��һλ�ǵ�ַΪ���ڽ��յ�ʱ��ֻ�е�ַλ�Ž��գ��������ﲻ���ж��ˡ�
	{
		Serial3PutChar(0x51);		
		//�ط���
	}
}