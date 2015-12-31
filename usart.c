#include "stm32f10x.h"
#include "usart.h"	
#include "stm32f10x_usart.h" 
#include "misc.h" 
#include "para.h"
#include "fault.h"
 
unsigned char UartData[20];
unsigned char UartDataLen = 0;
unsigned char UartDataRec = 0;
unsigned char UartDataState = 0; 

//初始化IO 串口1 
//bound:波特率
void uart1_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if(  UartDataState == 0 ){
			if (Res == 0xd1) 
				UartDataState =1;
			else 
				UartDataState = 0; //复位
		}else if(UartDataState == 1){
			if (Res == 0x30)
				UartDataState =2;
			else
				UartDataState =0;  
		}else if(UartDataState == 2){
			UartData[UartDataRec] = Res;
			UartDataRec ++;
			if(UartDataRec == 3)
			{
				UartDataState = 0;	//复位
				UartDataRec = 0;	
			}
		}
		 
     } 

}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断	 
  	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		if(  UartDataState == 0 ){
			if (Res == 0xd1) 
				UartDataState =1;
			else 
				UartDataState = 0; //复位
		}else if(UartDataState == 1){
			if (Res == 0x30)
				UartDataState =2;
			else
				UartDataState =0;  
		}else if(UartDataState == 2){
			UartData[UartDataRec] = Res;
			UartDataRec ++;
			//wz 修改下面2为3 11.18南通，为了多读一个字节的编码器数据，编码器是24位的
			if(UartDataRec == 3)
			{
				UartDataState = 0;	//复位
				UartDataRec = 0;	
			}
		}
		 
     } 

}

void u3_init()
{
	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

   /* Enable/Disable USART and GPIO clock                                            */
  	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5, ENABLE);	

	   /* Configure USART3 Rx (PB11) as input floating                             */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure USART3 Tx (PB10) as alternate function push-pull                */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate            = 9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              =  USART_Parity_No ;//USART_Parity_Odd	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	//	if you want to set the parity bit ,you must select 9bit data

	USART_Init(USART3, &USART_InitStructure);

	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART3, ENABLE);
}


void Serial3PutChar(u8 c)
{
  USART_SendData(USART3, c);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
} 

void Serial3PutString(u8 *s)
{
  while (*s != '\0')
  {
    Serial3PutChar(*s);
    s ++;
  }
}

//串口3接收中断
void USART3_IRQHandler_old(void)
{
	char Res;
	static int U3DataState = 0; //状态机
	static char temp_8_high = 0 , temp_8_low = 0;
	char high_8 = 0 , low_8 = 0 ;
	int temp_16 = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//if(Serial3GetChar()==0x01)
		//	Serial3PutString("\r\nUSART3 receive is successfull!\r\n");
  		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		Serial3PutChar(Res);		
		
		if(  U3DataState == 0 ){
			if (Res == 0x01) 		//地址码  改为#define 更好 
				U3DataState = 1;
			else 
				U3DataState = 0; //复位
		}else if(U3DataState == 1){
		
			if (Res == 0x01) 	//读取状态
			{
				//U3DataState =2;
				//返回当前的状态
				//阀位、报警信息
				//Serial3PutString("\r\n return state now!\r\n");
				Serial3PutChar(0x99);
				Serial3PutChar(0x01);
				high_8 = ValvePosValue >> 8;
				low_8 = ValvePosValue;
				Serial3PutChar(high_8);
				Serial3PutChar(low_8);
				high_8 = FaultTotal >> 8;
				low_8 = FaultTotal;
				Serial3PutChar(high_8);
				Serial3PutChar(low_8);

				U3DataState = 0; //复位
			}
			else if(Res == 0x09)	//设定值
			{
				U3DataState = 2;				
			}
			else
				U3DataState = 0;	//复位  
		}else if(U3DataState == 2){			
			if(Res == 0x00) 		//全开或全关
			{
				PCMode = pc_mode_set;
				U3DataState = 3;
			}
			else if(Res == 0x01){ 	//设定一个阀位值
				PCMode = pc_mode_set;
				U3DataState = 4;
			}
			else if(Res == 0x02){	//维持式
				PCMode = pc_mode_hold;
				U3DataState = 6;
			}			
			else if(Res == 0x03){	//脉冲式
				PCMode = pc_mode_pluse;			
				U3DataState = 7;			
			}	
		}else if(U3DataState == 3)
		{
			if(Res == 0xff)	 {
				Serial3PutString("\r\n Open!\r\n");//全开
				PCSetValve = 1000;
				U3DataState = 0; //复位
			}
			else if(Res == 0x00){
				Serial3PutString("\r\n Close!\r\n");;//全关 
				PCSetValve = 0;
				U3DataState = 0; //复位			
			}
			else
				U3DataState = 0;	
		}else if(U3DataState == 4)
		{
			//设定一个阀位值  读取两个char保存为int			
			temp_8_high = Res;
			U3DataState = 5;
		}else if(U3DataState == 5)
		{
			temp_8_low = Res;
			temp_16 = temp_8_high<<8;
			temp_16 +=temp_8_low;
			Serial3PutString("\r\n set a valve value! \r\n");
			temp_8_high = 0;
			temp_8_low = 0;
			PCSetValve = temp_16;
			temp_16 = 0;
			U3DataState = 0;	
		}else if(U3DataState == 6)	//维持式
		{
		   	if(Res == 0x01){
				PCHoldOpenSig = 1;
				Serial3PutString("\r\n PCHoldOpenSig \r\n");
			}
			if(Res == 0x02){
				PCHoldCloseSig = 1;
			   	Serial3PutString("\r\n PCHoldCloseSig \r\n");
			}
				
			U3DataState = 0;	
		}else if(U3DataState == 7)	//脉冲式
		{
		   	if(Res == 0x01){
				PCPluseOpenSig = 1;
				Serial3PutString("\r\n PCPluseOpenSig \r\n");
			}
			if(Res == 0x02){
				PCPluseCloseSig = 1;
			   	Serial3PutString("\r\n PCPluseCloseSig \r\n");
			}
			if(Res == 0x03){
				PCPluseStopSig = 1;
			   	Serial3PutString("\r\n PCPluseStopSig \r\n");
			}
					
			U3DataState = 0;	
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

//串口3接收中断	   new modbus	  在中断里面把指令全部保存到数组里面，在中断外面判断跟回复指令
void USART3_IRQHandler(void)
{
	char Res;
	static int U3DataState = 0; //状态机
	static char temp_8_high = 0 , temp_8_low = 0;
	char high_8 = 0 , low_8 = 0 ;
	int temp_16 = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//if(Serial3GetChar()==0x01)
		//	Serial3PutString("\r\nUSART3 receive is successfull!\r\n");
  		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		//Serial3PutChar(Res);		
		
		if(  U3DataState == 0 ){
			if (Res == 0x01) 		//地址码  改为#define 更好 
			{
				U3DataState = 1;
				ModbusCommand[ModbusCommandCnt++]=Res;
			}
			else 
				U3DataState = 0; //复位
		}else if(U3DataState == 1){
			if (Res == 0x05 || Res == 0x01) //写/读继电器
			{
				U3DataState = 2;
				ModbusCommand[ModbusCommandCnt++]=Res;
			}	
		}
		else if( U3DataState == 2)   //功能码0x05 写继电器
		{
			 ModbusCommand[ModbusCommandCnt]=Res;
			 ModbusCommandCnt++;
			 if(ModbusCommandCnt==8){
			 	 U3DataState = 0;
				 ModbusCommandCnt = 0;
			 }
		}
		
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}


int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART3, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}


void GetEncoderPara(void)
{
	USART_SendData(USART2, 0xD4);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART2, 0xAA);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0x30);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0xcc);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0x08);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0x67);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
}

void EncoderCommEnd(void)
{
	USART_SendData(USART2, 0xD5);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART2, 0xAA);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0x00);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0xFF);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0xAD);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USART2, 0x64);//向串口1发送数据
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
}


