#include "page.h"
#include "area.h"
#include "oled.h"
#include "atom.h"
#include "para.h"
#include "current.h"
#include "control.h"
#include "Time_test.h"
#include "pwm.h"
#include "fault.h"
#include "key.h"

int ad654rec[10] = {0};
int alram = 0;

extern float set_k;

void AD654Read(void){
	ad654rec[9] = ad654rec[8]; 
	ad654rec[8] = ad654rec[7]; 
	ad654rec[7] = ad654rec[6];		
	ad654rec[6] = ad654rec[5]; 
	ad654rec[5] = ad654rec[4]; 	
	ad654rec[4] = ad654rec[3]; 
	ad654rec[3] = ad654rec[2];		
	ad654rec[2] = ad654rec[1]; 
	ad654rec[1] = ad654rec[0]; 
	ad654rec[0] = AD654F;
	AD654F = 0;	
	/*
	if(ad654rec[0] - ad654rec[1] > 10 || ad654rec[0] - ad654rec[1] < -10)
	{
		ad654rec[9] = 0;
	} */
}

//wz����ôŪ��
//����4�ξ�ֵ�˲��������4*500ms=2s�������ͺ����4~20mA�ܾ�׼�Ļ�������ȡ�������ֵ�˲�
float AD654ToCurrent(void){
	float re = 0.0;
	int cnt = 0 , sum = 0;
	//re = ( (float)AD654F - 200.0 )/50.0 + 4.0;
	//re = ( (float)ad654rec[0] - 80.0 )/20.0 + 4.0;
	//ValveSetVal	= ((float)AD654F - 200.0 )*1.25 ;
	if( (ad654rec[0]-ad654rec[1]>5 && ad654rec[1]-ad654rec[2]>5) || (ad654rec[1]-ad654rec[0]>5 && ad654rec[2]-ad654rec[1]>5) )
		sum = ad654rec[0];
	else 
		sum = (ad654rec[0] + ad654rec[1] + ad654rec[2] + ad654rec[3])/4;
		/*
	for(cnt = 0 ; cnt < 4 ; cnt ++){
		sum = sum + ad654rec[cnt]/4 ;
	}	  */
	//ValveSetVal	= ((float)sum - ParaArray[Set8ma] )/()*500.0+250.0 ;
	ValveSetVal	=(int)( ((float)(sum - ParaArray[Set4ma] ))*set_k  );
	
	//

	if( ValveSetVal > 1000 && ValveSetVal < 1190 )
		ValveSetVal = 1000;
	if( ValveSetVal >= 1190 )
	{
		//guzhang  �źų���
		FaultTotal = FaultTotal | FaultSigOverMask;
	}
	if( ValveSetVal < 10 && ValveSetVal > -52 )
	{
		ValveSetVal = 0 ;
	}
	if( ValveSetVal <= -52 && ValveSetVal >= -135  )
	{
		//guzhang �źų��޹���
		FaultTotal = FaultTotal | FaultSigOverMask;
	}
	if( ValveSetVal < -135  )
	{
		//guzhang �ź��жϹ���
		FaultTotal = FaultTotal | FaultSigBreakMask;
		FaultTotal = FaultTotal & ~FaultSigOverMask;
	}

	//�������
	if( ValveSetVal >= -135  )
		FaultTotal = FaultTotal & ~FaultSigBreakMask;
	if(ValveSetVal >= -52 && ValveSetVal <= 1190){
		FaultTotal = FaultTotal & ~FaultSigOverMask;	
	}	
	/*
	ad654rec[9] = ad654rec[8];	
	ad654rec[8] = ad654rec[7]; 
	ad654rec[7] = ad654rec[6];
	ad654rec[6] = ad654rec[5]; 
	ad654rec[5] = ad654rec[4];	
	ad654rec[4] = ad654rec[3]; 
	ad654rec[3] = ad654rec[2];		
	ad654rec[2] = ad654rec[1]; 
	ad654rec[1] = ad654rec[0]; 
	ad654rec[0] = AD654F;
	if( ad654rec[2]-ad654rec[1] > 1 || ad654rec[2]-ad654rec[1] < -1)
	{
		alram = 1;	
	}

	AD654F = 0;	 */
	return re;
}
void ValveSetShow()
{
	//ֻ�е�δִ�е�ʱ�򣬲ſ�������Set���������ִ��֮��Set����������֣���ʾҲ����
	//if(ValveSetExecuteFlag == 0)
	{
		//ValveSetVal = (int)(sum / 8);	   //ƽ��ֵ	ԭ����23  ����ĳ�8 wz2015-3-13

		if(ValveSetVal>=0 && ValveSetVal<=1000){	
			if(ValveSetVal == 1000) {
			
				ascii_1608(40,16,english[ValveSetVal/1000+18],0);
				ascii_1608(44,16,english[ValveSetVal%1000/100+18],0);
				ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
				ascii_1608(56,16,english[ValveSetVal%10+18],0);
				ascii_1608(52,16,english[31],0);
				ascii_1608(60,16,english[32],0);	
			}else{
				ascii_1608(40,16,english[41],0); 
				ascii_1608(44,16,english[ValveSetVal/100+18],0);
				ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
				ascii_1608(56,16,english[ValveSetVal%10+18],0);
				ascii_1608(52,16,english[31],0);
				ascii_1608(60,16,english[32],0);				
			}
		}else{
			//guzhang  ����Ӧ�ô�������ģ��
				ascii_1608(40,16,english[41],0);
			
			ascii_1608(44,16,english[33],0);
			ascii_1608(48,16,english[34],0);
			ascii_1608(52,16,english[35],0);
			ascii_1608(56,16,english[36],0);
			ascii_1608(60,16,english[37],0);			
		}
	}	
}

//Զ��ģʽ��4-20mA��λ�趨ֵ�˲�����ʾ
void ValveSetShow_old()
{
	int i = 0 , j = 0;
	int temp = 0;
	int max = 0 , min = 65535;//ȥ�����ֵ����Сֵ
	long int sum = 0;
	/*
	for(i = 0 ; i < 10 ;i ++)
	{
		if(ValveSetValTemp[i] > max) max = ValveSetValTemp[i];
		if(ValveSetValTemp[i] < min) min = ValveSetValTemp[i];
	}
	for(i = 0 ; i < 10 ;i ++)
	{
		sum += ValveSetValTemp[i];
	}
	sum =sum - max - min; //ȥ��������С��ֵ
	*/
	for(i = 0 ; i < 25 ; i ++){
		for(j = 0 ; j < 25 ; j ++){
			if(ValveSetValTemp[j] < ValveSetValTemp[i]){
				temp = ValveSetValTemp[i];
				ValveSetValTemp[i] = ValveSetValTemp[j];
				ValveSetValTemp[j] = temp; 	
			}
		}
	}
	for(i = 8 ; i < 16 ;i ++)
	{
		sum += ValveSetValTemp[i];
	}


	//ֻ�е�δִ�е�ʱ�򣬲ſ�������Set���������ִ��֮��Set����������֣���ʾҲ����
	if(ValveSetExecuteFlag == 0)
	{
		ValveSetVal = (int)(sum / 8);	   //ƽ��ֵ	ԭ����23  ����ĳ�8 wz2015-3-13

		if(ValveSetVal>=0 && ValveSetVal<=1000){
			ascii_1608(44,16,english[ValveSetVal/100+18],0);
			ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
			ascii_1608(56,16,english[ValveSetVal%10+18],0);
			ascii_1608(52,16,english[31],0);
			ascii_1608(60,16,english[32],0);	
		}else{
			ascii_1608(44,16,english[33],0);
			ascii_1608(48,16,english[34],0);
			ascii_1608(52,16,english[35],0);
			ascii_1608(56,16,english[36],0);
			ascii_1608(60,16,english[37],0);			
		}
	}
}

//Զ��ģʽ�� ���������ʾ
void ValveErrorShow()
{
	int temp = 0;
	if( ValveError >= 0){
		ascii_1608(40,32,english[39],0);
		ascii_1608(44,32,english[ValveError/100+18],0);
		ascii_1608(48,32,english[ValveError%100/10+18],0);
		ascii_1608(56,32,english[ValveError%10+18],0);
	}else{
		temp = 0-ValveError; 
		
		ascii_1608(40,32,english[38],0);
		ascii_1608(44,32,english[ValveError/100+18],0);
		ascii_1608(48,32,english[ValveError%100/10+18],0);
		ascii_1608(56,32,english[ValveError%10+18],0);		
	}
	ascii_1608(52,32,english[31],0);
	ascii_1608(60,32,english[32],0);	
}

//���Ͻǵ�ʵʱ��ʾ��λ����
void ValvePosShow()
{
	int temp = 0;
	int ValvePosValueShow = 0;		//��ʾ��ֵ
	int ValvePosValueShowBits = 0; //λ��
	int PlusOrMinus = 0;
	/*
	if(Main_KEY_State == 0)	   //����ģʽ ���������������Ƿ񳬹�100% ����ʾ����
	{
		if(ValvePosValue > 999)
		{
			ValvePosValueShow  =  ValvePosValue;
			ValvePosValueShowBits = 4;
			PlusOrMinus = 1;
		}
		else if(ValvePosValue >= 0)
		{
			ValvePosValueShow  =  ValvePosValue;
			ValvePosValueShowBits = 3;
			PlusOrMinus = 1;		
		}
		else if( ValvePosValue < 0 )
		{
			ValvePosValueShow  =  -ValvePosValue;
			ValvePosValueShowBits = 3;
			PlusOrMinus = -1;

		}

		if(ValvePosValueShowBits == 3 )
		{
			if(PlusOrMinus == 1)
				ascii_1608(40,0,english[41],0);	
			else if(PlusOrMinus == -1)
				ascii_1608(40,0,english[38],0);
		}
		else if(ValvePosValueShowBits == 4 )
		{
		   ascii_1608(40,0,english[ValvePosValueShow/1000+18],0);
		}
		ascii_1608(44,0,english[ValvePosValueShow%1000/100+18],0);
		ascii_1608(48,0,english[ValvePosValueShow%100/10+18],0);
		ascii_1608(56,0,english[ValvePosValueShow%10+18],0);
		ascii_1608(52,0,english[31],0);
		ascii_1608(60,0,english[32],0);	
	}
	
	if(Main_KEY_State == 1 || Main_KEY_State == 2 )
	{ */
	
	/*	
		if( ValvePosValue > 1030 )
		{
			//���ޱ���
			//guzhang
			//FaultTotal  = FaultTotal | FaultJourneyOverMask;
		}
		else */
		if(  ValvePosValue > 995 )
		{
			temp = 1000;
		}
		else if( ValvePosValue <= 995 && ValvePosValue >= 0)
		{
			temp = ValvePosValue;
			//FaultTotal  = FaultTotal & ~FaultJourneyOverMask;
		}
		else if( ValvePosValue <0 &&  ValvePosValue >= -25)
		{
			temp = 0 ;	
			FaultTotal  = FaultTotal & ~FaultJourneyOverMask;
		}
		else if( ValvePosValue < -15 )
		{
			//guzhang
			//��λ���ޣ�С��0%�Ĳ�������
			//FaultTotal  = FaultTotal | FaultJourneyOverMask;			
		}

		if( (FaultTotal & FaultJourneyOverMask) == 0 )
		{
			if(temp > 999)
			{
			   	ascii_1608(40,0,english[temp/1000+18],0);
				ascii_1608(44,0,english[temp%1000/100+18],0);
				ascii_1608(48,0,english[temp%100/10+18],0);
				ascii_1608(56,0,english[temp%10+18],0);
				ascii_1608(52,0,english[31],0);
				ascii_1608(60,0,english[32],0);	
			}else if(temp >= 0){
				ascii_1608(40,0,english[41],0);
				ascii_1608(44,0,english[temp%1000/100+18],0);
				ascii_1608(48,0,english[temp%100/10+18],0);
				ascii_1608(56,0,english[temp%10+18],0);
				ascii_1608(52,0,english[31],0);
				ascii_1608(60,0,english[32],0);	
			}
		}
		else
		{
			ascii_1608(40,0,english[41],0);
			ascii_1608(44,0,english[33],0);
			ascii_1608(48,0,english[34],0);
			ascii_1608(52,0,english[35],0);
			ascii_1608(56,0,english[36],0);
			ascii_1608(60,0,english[37],0);
		}
	//}  

}

//��ʾ����  �ڵ�����
void ParaShow(int x)
{
	//ascii_1608(16,48,english[x/100000000+18],0);
	//ascii_1608(20,48,english[x%100000000/10000000+18],0);
	//ascii_1608(24,48,english[x%10000000/10000+18],0);
	ascii_1608(24,48,english[x%100000/10000+18],0);	
	ascii_1608(28,48,english[x%10000/1000+18],0);
	ascii_1608(32,48,english[x%1000/100+18],0);
	ascii_1608(36,48,english[x%100/10+18],0);
	ascii_1608(40,48,english[x%10+18],0);	
}

//ҳ��3area0 ������������  ������λ���г�
void page3area0atom0op0(){
	ParaArray[OpenDirLimit] = journey;
}
//ҳ��3area0 ������������  ������λ��ת��
void page3area0atom0op1(){
	ParaArray[OpenDirLimit] = torque;
}

//ҳ��3area0 ������������  ������λ���г�
void page3area0atom1op0(){	
	ParaArray[CloseDirLimit] = journey;
}
//ҳ��3area0 ������������  ������λ��ת��
void page3area0atom1op1(){	
	ParaArray[CloseDirLimit] = torque;
}
//ҳ��3area0 ������������   �м�λ�ã�������û��ѡ��ģ�Ӧ����һ���������뺯��   3λ��
//void page3area0atom2op0(){}

void page3area0atom2inop(){
	ParaArray[MidPosition1] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

void page3area0atom2op1(){	}
//ҳ��3area3 ������������   ��������ת�أ�������û��ѡ��ģ�Ӧ����һ���������뺯��
void page3area0atom3inop(){
	ParaArray[OpenDirJour] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

//ҳ��3area4 ������������   ��������ת�أ�������û��ѡ��ģ�Ӧ����һ���������뺯��
void page3area0atom4inop(){
	ParaArray[CloseDirJour] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

//ҳ��3area5 ������������   �������Ʒ�ʽ����λ
void page3area0atom5op0(){
	ParaArray[EmergencyMode] = hold;	
}
//ҳ��3area5 ������������   �������Ʒ�ʽ��ת��λ��
void page3area0atom5op1(){
	ParaArray[EmergencyMode] = assign;
}


//���Ʋ���
//wz ��Ϊarea1���ڸ���Ϊ �նˣ���ԭ���ĺ����ҹ�û�иı䣬��������ĺ������Ȳ���
//ҳ��3area1 ���Ʋ������� ���͵�  ������Ӧ��Ҫ����subarea
void page3area1atom0op0(){	
	SubAreaMode = 1;
	SubAreaNum = 0;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;
}

//ҳ��3area1 ���Ʋ�������   ��subarea
void page3area1atom0op1(){	
	SubAreaMode = 1;
	SubAreaNum = 1;
	AreaChageFlag = 0;  //������ʾ����subarea
	SubAreaChangeFlag = 1;
}
//�͵ؿ��Ʋ������ã�ѡΪ����ʽ��������ʽ��ά��ʽ��ʵ�ֻ�һ�£�ֱ�ӻ������
void page3area1atom0sub0op0(){
	ParaArray[LocallyMode] = hold_mode;//pluse_mode ;	
}
//�͵ؿ��Ʋ������ã�ѡΪά��ʽ ��������ʽ��ά��ʽ��ʵ�ֻ�һ�£�ֱ�ӻ����//�������ﻻ��һ�£���ʾ�ĵط�ҲҪ����
void page3area1atom0sub0op1(){
	ParaArray[LocallyMode] = pluse_mode;
}

//Զ�̿��Ʋ������ã�ѡΪ����ʽ	 ��������ʽ��ά��ʽ��ʵ�ֻ�һ�£�ֱ�ӻ������
void page3area1atom0sub1op0(){
	ParaArray[FarawayMode] = hold_mode;
}
//Զ�̿��Ʋ���ѡ��ѡΪά��ʽ	��������ʽ��ά��ʽ��ʵ�ֻ�һ�£�ֱ�ӻ������
void page3area1atom0sub1op1(){
	ParaArray[FarawayMode] = pluse_mode;	
}
//Զ�̿��Ʋ���ѡ��ѡΪ�����ʽ
void page3area1atom0sub1op2(){
	ParaArray[FarawayMode] = pc_mode;
}
//Զ�̿��Ʋ���ѡ��ѡΪģ����ʽ
void page3area1atom0sub1op3(){
	ParaArray[FarawayMode] = analog_mode;
}


//ҳ��3area2 I/O��������    �����ź�IOSetRec������
void page3area2atom0op0(){	
	ParaArray[OutNormalState0 + IOSetRec] = OutNormalState_open;
}
//ҳ��3area2 I/O��������    �����ź�IOSetRec������
void page3area2atom0op1(){	
	ParaArray[OutNormalState0 + IOSetRec] = OutNormalState_close;
}

//ҳ��3area2 I/O��������    �����ź�1����ʾ  ����Ӧ��Ҫ����6��subarea��ѡ��

//�����ź�IOSetRec���ܹ���
void page3area2atom1op(){	
	ParaArray[OutExpState0 + IOSetRec] = TotalFault;
}
//Զ������
void page3area2atom2op(){	
	ParaArray[OutExpState0 + IOSetRec] = FarawayFault;
}		  
//ȱ�����
void page3area2atom3op(){	
	ParaArray[OutExpState0 + IOSetRec] = LackPhaseFault;
}
//��λ����
void page3area2atom4op(){	
	ParaArray[OutExpState0 + IOSetRec] = ValveLocationFault;
}
//�������
void page3area2atom5op(){	
	ParaArray[OutExpState0 + IOSetRec] = MotorOverHeartFault;
}
//�м�λ��1
void page3area2atom6op(){	
	ParaArray[OutExpState0 + IOSetRec] = MidPosition1Fault;
}
//�м�λ��2
void page3area2atom7op(){	
	ParaArray[OutExpState0 + IOSetRec] = MidPosition2Fault;
}
//�����г���λ
void page3area2atom8op(){	
	ParaArray[OutExpState0 + IOSetRec] = OpenDirJourneyLimit;
}
//�����г���λ
void page3area2atom9op(){	
	ParaArray[OutExpState0 + IOSetRec] = CloseDirJourneyLimit;
}
//����ת����λ
void page3area2atom10op(){	
	ParaArray[OutExpState0 + IOSetRec] = OpenDirTorqueLimit;
}
//����ת����λ
void page3area2atom11op(){	
	ParaArray[OutExpState0 + IOSetRec] = CloseDirTorqueLimit;
}
//�趨ֵ����
void page3area2atom12op(){	
	ParaArray[OutExpState0 + IOSetRec] = SetValueFault;
}
/*
//ҳ��3area4 �궨����       �궨���� ����ֵ8mA  
void page3area4atom0inop(){
	ParaArray[Feedback8ma] = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;		
}

//ҳ��3area4 �궨����       �궨���� ����ֵ16mA   
void page3area4atom1inop(){
	ParaArray[Feedback16ma] = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;		
} */
//ҳ��3area4 �궨����       �궨���� ����4mA  ��¼�·���4ma��Ӧ�ı�����ֵ 
void page3area4atom0op(void){								   
	ParaArray[Feedback4maPos] = EncoderData;			
}
//ҳ��3area4 �궨����       �궨���� ����20mA  ��¼�·���4ma��Ӧ�ı�����ֵ 
void page3area4atom1op(void){
	ParaArray[Feedback20maPos] = EncoderData;
}
//ҳ��3area4 �궨����       �궨���� �趨ֵ4mA    �趨��ǰֵΪ4ma
void page3area4atom2op(void){
	int max = 0 ;
	int min = 65535;
	int cnt = 0;
	/*
	for(cnt = 0 ; cnt < 10 ; cnt ++){
		if()
	} */

	//bug ����Ӧ��ҲҪ���˲�
	int sum = 0;
	for(cnt = 0 ; cnt < 4 ; cnt++)
		sum = sum + ad654rec[cnt]/4;
	ParaArray[Set4ma] = sum;//TIM1_FREQ;		
}
//ҳ��3area4 �궨����       �궨���� �趨ֵ20mA   �趨��ǰֵΪ20ma
void page3area4atom3op(void){
	int cnt = 0;
	int sum = 0;
	for(cnt = 0 ; cnt < 4 ; cnt++)
		sum = sum + ad654rec[cnt]/4;
	ParaArray[Set20ma] = sum;	
}

//�ն�
//ҳ��3area3atom4op �궨����  �趨��ǰλ��Ϊ��λ0%
void page3area3atom0op(){

	//��ȡ��������ǰֵ   ������ֱ����ÿ20ms��ȡһ�εı�����ֵEncoderData
	ParaArray[SetValveMin] = EncoderData;
	//��Ӧ������ֱ�Ӳ�����Ӧ�û��ǽ� EncoderData ����FLASH�е� SetValveMin_ , Ȼ��SetValveMin��ÿ�ο��������������֮���FLASH������¹���

}
//ҳ��3area3op5 �궨����  �趨��ǰλ��Ϊ��λ100%  
void page3area3atom1op(){	
	ParaArray[SetValveMax] = EncoderData;

	ParaArray[SelfLearnRec] = NoHavedSelfLearn ;	
}  


//ҳ��3area4 ��������       �ָ��������ã����ָ�
void page3area4atom0op0(){	}
//ҳ��3area4 ��������       �ָ��������ã��ָ� 
void page3area4atom0op1(){	}
//ҳ��3area4 ��������       ��������������
void page3area4atom1op0(){	}
//ҳ��3area4 ��������       ������������ 
void page3area4atom1op1(){
	FaultTotal = 0;
}

//ҳ��3area5 ��������       �޸����룺���޸� 
void page3area5atom2op0(){}
//ҳ��3area5 ��������       �޸����룺�޸� 
void page3area5atom2op1(){
	SubAreaMode = 1;
	SubAreaNum = 1;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;	
}

//������������Ľ�����ѧϰģʽ
void page3area5atom3op(){
	SubAreaMode = 1;
	SubAreaNum = 0;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;		
}

void page3area5atom0op0(){ } //�������˳�



