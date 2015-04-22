/*
	���ϼ��ģ��
	��ϵͳ���ֹ��Ͻ��м�⣬���Ҽ�������ʾ
	�����
	1.Զ�����ϣ�
	2.��Դȱ�ࣺ�����ǵ����Դ
	3.��λ���ϣ�
	4.������ȹ��ϣ��¶ȴ���������
	5.�м�λ��1��
	6.�м�λ��2������
	7.�����г���λ�������г���λ֮����ϱ��������Ҳ��ܹ���ִ�ж�����
	8.�����г���λ��
	9.����ת����λ������ת����λ֮����ϱ��������Ҳ��ܹ���ִ�ж�����
	10.����ת����λ��
	11.�趨ֵ���ϣ��趨ֵ���ϣ�a.������õĲ��������߼��ϵļ��  b.��Զ�̿����еĵ���ֵ���м��

	**���Ա������ص��ᵽ���������ϣ�
	�ź��ж�
	�źų���
	���ع���


	���ϼ�����
	//�ܹ���  //Զ������  //��Դȱ��  //��λ����   //������ȹ���  //�м�λ��1	//�м�λ��2	 
//�����г���λ	 //�����г���λ	//����ת����λ	//����ת����λ	 //�趨ֵ����	  //δ����
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

//�������ź��жϡ��źų���
int FaultTotal = 0;
int FaultShowFlag = 0;

extern int ValvePosValue;

//������ʾ
void FaultShow(int x )
{
	//��ÿ�����ϵĽ�����ѯ��� ,������ʾ
	//�ź��ж�
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
	//�źų���
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



//���ϼ�⺯��
void FaultDetect(void)
{
	//�й��ϵ������  	
	if( FaultTotal != 0 )
	{
		//�Ƶ���
		LED2(ON);
		LED3(OFF);
		LED1(OFF);			
		//��SSR����̵����Ͽ�
		
		//io�������һ������

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
		//���Ͻ�� 
		LED2(OFF);
		//ssr�ϵ�
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

		//io������

		RelayOut3(OPEN);

	}
}