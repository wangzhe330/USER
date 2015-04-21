#ifndef PARA_H
#define PARA_H

#include "stm32f10x.h"
#include "stdio.h"

//��ʾ�������ֳ��ȣ���������crc�ģ�
//51�ǲ����ܸ�������SPI_FLASH_BufferRead��������ʱ�����4 ����Ϊÿ��������32bits��int����Ҫ��Ӧ8bits��flashд����������������
#define  ParaArrayLength  	(59)

//������ĵ�0λû���ã���32���ֽ�Ҳ���Դ洢CRC�ͳ���
//��������ʾ�޸��˶��ٴ�  
#define  HaveSetTimes       0

//��������*********************************************************
//������λ ������λ
//�г̡�ת��
#define  OpenDirLimit   	1
#define  CloseDirLimit  	2
#define  journey        	101 
#define  torque				102

////�������Ʒ�ʽ���趨�źŶ�ʧ
//��λ��ָ��λ��
#define  EmergencyMode  	3
#define  SetSigLose     	4
#define  hold				301
#define  assign				302

//�м�λ��1
//�м�λ��2
//��������ת��
//��������ת��
#define	 MidPosition1   	5
#define	 MidPosition2		6
#define	 OpenDirJour		7
#define	 CloseDirJour		8

//���Ʋ�������
//Զ�̿���ģʽ������ʽ��ά��ʽ��˫λʽ��ģ����
#define	 FarawayMode    	9
#define	 LocallyMode		10
#define	 pluse_mode     	901
#define  hold_mode      	902
#define  pc_mode    	903
#define  analog_mode		904

//�źű궨���� 
//����ֵ4ma����ʾ��λ��	 ����ֵ20ma����ʾ��λ��	 �趨ֵ4ma	�趨ֵ20ma	 
#define  Feedback4maPos    	11		
#define  Feedback20maPos 	12
#define  Set4ma 			13
#define  Set20ma 			14

//��λ�궨 0%  100%
#define  SetValveMin 		15
#define  SetValveMax 		16

//I\O�趨����*****************************************************
//��������ĳ�������״̬
#define  OutNormalState0	17
#define  OutNormalState1	18
#define  OutNormalState2	19
#define  OutNormalState3	20
#define  OutNormalState4	21
#define  OutNormalState5	22

#define  OutNormalState_open  171
#define  OutNormalState_close 172

//���������ź������ʾ��12��״̬
#define  OutExpState0		23
#define  OutExpState1		24
#define  OutExpState2		25
#define  OutExpState3		26
#define  OutExpState4		27
#define  OutExpState5		28
//���
//�ܹ���  //Զ������  //��Դȱ��  //��λ����   //������ȹ���  //�м�λ��1	//�м�λ��2	 
//�����г���λ	 //�����г���λ	//�����г���λ	//�����г���λ	 //�趨ֵ����	  //δ����
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

//��������
//�Ƿ�ָ��������� //�Ƿ��������
#define  Restore			29
#define  ClearAlarm 		30
//����ǡ���
#define  YorN_yes			291
#define  YorN_no			292

//���ϲ���

//���ϼ�����
//�ܹ���  //Զ������  //��Դȱ��  //��λ���� //������ȹ���	//�м�λ��1 //�м�λ��2	 
//�����г���λ	//�����г���λ	//�����г���λ //�����г���λ //�趨ֵ����
//�����г���λ	 //�����г���λ	 //�����г���λ	 //�����г���λ
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

//ת��ADֵ	 //��������	  //Ӳ���汾  //����汾
#define  JourneyADCalue 			47				
#define  FactoryCode 				48
#define  HardwareVersion 			49			
#define  SoftwareVersion 			50

//wz �������ӵ�
#define  Feedback8maDty 			51	
#define  Feedback16maDty			52

#define  CloseError					53
#define  OpenError					54
#define  SelfLearnRec 				55
#define  HavedSelfLearn				551
#define  NoHavedSelfLearn			552

//����
#define  Security 					56

//����CRCλ�ã�����У�����в����Ƿ��ȡ��ȷ	 �������Ƕ����8bbit����32bit������
#define  CRC_H						57
#define  CRC_L						58	

extern int ValvePosValue ;		//��λ�������һλ��С��λ
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

#define pc_mode_all   600
#define pc_mode_pluse 601
#define pc_mode_hold  602
#define pc_mode_set   603


extern int PCMode;
extern int PCHoldOpenSig ;	//ά��ʽ�����ź�
extern int PCHoldCloseSig ;	//ά��ʽ�����ź�
extern int PCPluseOpenSig ;	//����ʽ�����ź�
extern int PCPluseCloseSig ;//����ʽ�����ź�
extern int PCPluseStopSig ;//����ʽ��ͣ�ź�
extern int PCAllOpenSig ;	//ȫ���ź�
extern int PCAllCLoseSig ;	//ȫ���ź�
extern int PCSetSig ;		//�趨 ģʽ
extern int PCSetValve ; 	//�趨ֵ


void ParaInit(void);
void ParaWriteToFlash(void);
int BufferCmp(int* pBuffer1, int* pBuffer2, int BufferLength);




#endif
