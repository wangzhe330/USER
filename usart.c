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

//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
#if EN_USART1_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(  UartDataState == 0 ){
			if (Res == 0xd1) 
				UartDataState =1;
			else 
				UartDataState = 0; //��λ
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
				UartDataState = 0;	//��λ
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
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�	 
  	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		if(  UartDataState == 0 ){
			if (Res == 0xd1) 
				UartDataState =1;
			else 
				UartDataState = 0; //��λ
		}else if(UartDataState == 1){
			if (Res == 0x30)
				UartDataState =2;
			else
				UartDataState =0;  
		}else if(UartDataState == 2){
			UartData[UartDataRec] = Res;
			UartDataRec ++;
			//wz �޸�����2Ϊ3 11.18��ͨ��Ϊ�˶��һ���ֽڵı��������ݣ���������24λ��
			if(UartDataRec == 3)
			{
				UartDataState = 0;	//��λ
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

//����3�����ж�
void USART3_IRQHandler_old(void)
{
	char Res;
	static int U3DataState = 0; //״̬��
	static char temp_8_high = 0 , temp_8_low = 0;
	char high_8 = 0 , low_8 = 0 ;
	int temp_16 = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//if(Serial3GetChar()==0x01)
		//	Serial3PutString("\r\nUSART3 receive is successfull!\r\n");
  		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		Serial3PutChar(Res);		
		
		if(  U3DataState == 0 ){
			if (Res == 0x01) 		//��ַ��  ��Ϊ#define ���� 
				U3DataState = 1;
			else 
				U3DataState = 0; //��λ
		}else if(U3DataState == 1){
		
			if (Res == 0x01) 	//��ȡ״̬
			{
				//U3DataState =2;
				//���ص�ǰ��״̬
				//��λ��������Ϣ
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

				U3DataState = 0; //��λ
			}
			else if(Res == 0x09)	//�趨ֵ
			{
				U3DataState = 2;				
			}
			else
				U3DataState = 0;	//��λ  
		}else if(U3DataState == 2){			
			if(Res == 0x00) 		//ȫ����ȫ��
			{
				PCMode = pc_mode_set;
				U3DataState = 3;
			}
			else if(Res == 0x01){ 	//�趨һ����λֵ
				PCMode = pc_mode_set;
				U3DataState = 4;
			}
			else if(Res == 0x02){	//ά��ʽ
				PCMode = pc_mode_hold;
				U3DataState = 6;
			}			
			else if(Res == 0x03){	//����ʽ
				PCMode = pc_mode_pluse;			
				U3DataState = 7;			
			}	
		}else if(U3DataState == 3)
		{
			if(Res == 0xff)	 {
				Serial3PutString("\r\n Open!\r\n");//ȫ��
				PCSetValve = 1000;
				U3DataState = 0; //��λ
			}
			else if(Res == 0x00){
				Serial3PutString("\r\n Close!\r\n");;//ȫ�� 
				PCSetValve = 0;
				U3DataState = 0; //��λ			
			}
			else
				U3DataState = 0;	
		}else if(U3DataState == 4)
		{
			//�趨һ����λֵ  ��ȡ����char����Ϊint			
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
		}else if(U3DataState == 6)	//ά��ʽ
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
		}else if(U3DataState == 7)	//����ʽ
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

//����3�����ж�	   new modbus	  ���ж������ָ��ȫ�����浽�������棬���ж������жϸ��ظ�ָ��
void USART3_IRQHandler(void)
{
	char Res;
	static int U3DataState = 0; //״̬��
	static char temp_8_high = 0 , temp_8_low = 0;
	char high_8 = 0 , low_8 = 0 ;
	int temp_16 = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//if(Serial3GetChar()==0x01)
		//	Serial3PutString("\r\nUSART3 receive is successfull!\r\n");
  		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		//Serial3PutChar(Res);		
		
		if(  U3DataState == 0 ){
			if (Res == 0x01) 		//��ַ��  ��Ϊ#define ���� 
			{
				U3DataState = 1;
				ModbusCommand[ModbusCommandCnt++]=Res;
			}
			else 
				U3DataState = 0; //��λ
		}else if(U3DataState == 1){
			if (Res == 0x05 || Res == 0x01) //д/���̵���
			{
				U3DataState = 2;
				ModbusCommand[ModbusCommandCnt++]=Res;
			}	
		}
		else if( U3DataState == 2)   //������0x05 д�̵���
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
	USART_SendData(USART2, 0xD4);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	USART_SendData(USART2, 0xAA);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0x30);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0xcc);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0x08);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0x67);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

void EncoderCommEnd(void)
{
	USART_SendData(USART2, 0xD5);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	USART_SendData(USART2, 0xAA);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0x00);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0xFF);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0xAD);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USART2, 0x64);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}


