#include "page.h"
#include "area.h"
#include "oled.h"
#include "atom.h"
#include "para.h"
#include "current.h"

extern int auto_set_flag;
extern int AutoSM ;

//area������ʼ��
//��¼ÿ��ҳ���ÿ��area�ĳ���
//��ά����ĵ�һ��ά�ȱ�ʾpage�ţ��ڶ���ά�ȱ�ʾarea�ţ������д�ŵ���area�ĳ���
void AreaInit(void)
{
	PageAreaLen[2][0] = 6;//����������������λ��������λ
	PageAreaLen[2][1] = 1;
	PageAreaLen[2][2] = 6;//ʵ����6�� �Ȳ���1��
	PageAreaLen[2][3] = 6;
	PageAreaLen[2][4] = 2;	
	PageAreaLen[3][0] = 6;
	PageAreaLen[3][1] = 4; //�ն�����	 4��ҳ��
	PageAreaLen[3][2] = 78; //io����  ÿ��iO13��ҳ�� ��6��IO����  �������78 
	PageAreaLen[3][3] = 2;// ����  һ��area ����subarea
	PageAreaLen[3][4] = 4;//�ź�
	PageAreaLen[3][5] = 4;//����
	//PageAreaLen[3][6] = 4;

	PageAreaRec[2][0] = 0;//ָ���ʼ��Ϊ0		
}

//***********************************************************************************************************
//						
//											ҳ��2  �������
//
//***********************************************************************************************************


//ҳ��2  ������� area0 ����
void page2area0()
{
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У��������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	��������
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 7;
			ch_label[1].code[1] = 8;
			ch_label[1].code[2] = 13;
			ch_label[1].code[3] = 14;
			ch_label[1].type = 'c';
			//�����е�һ�� ������λ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';	  		
			//�����е�һ��  
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].type = 'c';

			if(ParaArray[OpenDirLimit] == journey){  //�г�
				ch_label[3].code[0] = 38;
				ch_label[3].code[1] = 39;
			}else if(ParaArray[OpenDirLimit] == torque)	//  ת��
			{	  
				ch_label[3].code[0] = 40;
				ch_label[3].code[1] = 41;
			}
			else
			{
				//����������ó������
			}			
			LabelLen = 4;
			LabelLenE = 0;
	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ������λ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��  �г�
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].type = 'c';
			if( ParaArray[CloseDirLimit] == journey){  //�г�
				ch_label[3].code[0] = 38;
				ch_label[3].code[1] = 39;
			}else if( ParaArray[CloseDirLimit] == torque ) //  ת��
			{	  
				ch_label[3].code[0] = 40;
				ch_label[3].code[1] = 41;
			}
			
			LabelLen = 4;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 2:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �м�λ��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 27;
			ch_label[2].code[1] = 42;
			ch_label[2].code[2] = 43;
			ch_label[2].code[3] = 10;
			ch_label[2].type = 'c';		  		
						
			LabelLen = 3;
			LabelLenE = 0;
			NumShow(24,48, ParaArray[MidPosition1] );		//�����ʾ����Ų����showlabel���棬��Ȼnum���ȳ���
	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 3:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ��������ת��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 9;
			ch_label[2].code[3] = 10;
			ch_label[2].code[4] = 40;
			ch_label[2].code[5] = 41;
			ch_label[2].type = 'c';
		  					
			LabelLen = 3;
			LabelLenE = 0;
	
			NumShow(24,48, ParaArray[OpenDirJour]);

			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 4:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ��������ת��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 9;
			ch_label[2].code[3] = 10;
			ch_label[2].code[4] = 40;
			ch_label[2].code[5] = 41;
			ch_label[2].type = 'c';
		  		
			LabelLen = 3;
			LabelLenE = 0;
	
			NumShow(24,48,ParaArray[CloseDirJour]);

			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 5:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �������Ʒ�ʽ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 44;
			ch_label[2].code[1] = 45;
			ch_label[2].code[2] = 15;
			ch_label[2].code[3] = 16;
			ch_label[2].code[4] = 63;
			ch_label[2].code[5] = 6;
			ch_label[2].type = 'c';
		  		
			//�����е�һ�� 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].type = 'c';
			if( ParaArray[EmergencyMode] == hold){	 // ��λ
				ch_label[3].length = 2;
				ch_label[3].code[0] = 66;
				ch_label[3].code[1] = 37;
			}else if(ParaArray[EmergencyMode] == assign ){		   //ָ��λ��
				ch_label[3].length = 4;
				ch_label[3].code[0] = 64;
				ch_label[3].code[1] = 65;
				ch_label[3].code[2] = 37;
				ch_label[3].code[3] = 10;
			}
			
			LabelLen = 4;
			LabelLenE = 0;
			OptionLen = 0;
			OptionLoc = 0;
			break;											
		}	
}

void page2area1(void){
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У��������
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	���Ʋ���
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 15;
			ch_label[1].code[1] = 16;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//�����е�һ��   �͵�
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 0;
			ch_label[2].code[1] = 1;
			ch_label[2].type = 'c';
			//�����еڶ���   �͵�ģʽ������ʽ��ά��ʽ
			ch_label[3].loc.x = 40;
			ch_label[3].loc.y = 32;
			ch_label[3].length = 3;
			ch_label[3].type = 'c';
			switch(  ParaArray[LocallyMode] ){
				case pluse_mode:
					ch_label[3].code[0] = 4;
					ch_label[3].code[1] = 5;
					ch_label[3].code[2] = 6;	
					break;
				case hold_mode:
					ch_label[3].code[0] = 86;
					ch_label[3].code[1] = 87;
					ch_label[3].code[2] = 6;
					break;
			}
		  		
			//�����е�һ��   Զ�� 
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 2;
			ch_label[4].code[1] = 3;
			ch_label[4].type = 'c';
			//�����еڶ���   Զ��ģʽ������ʽ��ά��ʽ��˫λʽ��ģ����
			ch_label[5].loc.x = 40;
			ch_label[5].loc.y = 48;
			ch_label[5].length = 3;
			ch_label[5].type = 'c';
			switch( ParaArray[FarawayMode] ){
				case pluse_mode:
					ch_label[5].code[0] = 4;
					ch_label[5].code[1] = 5;
					ch_label[5].code[2] = 6;	
					break;
				case hold_mode:
					ch_label[5].code[0] = 86;
					ch_label[5].code[1] = 87;
					ch_label[5].code[2] = 6;
					break;
				case double_mode:
					ch_label[5].code[0] = 91;
					ch_label[5].code[1] = 43;
					ch_label[5].code[2] = 6;
					break;
				case analog_mode:
					ch_label[5].code[0] = 88;
					ch_label[5].code[1] = 89;
					ch_label[5].code[2] = 90;
					break;
			}
	
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 6;
			LabelLenE = 0;
	
			option[0].loc.x = 16;
			option[0].loc.y = 32;
			option[0].Func = func_null;
			option[1].loc.x = 16;
			option[1].loc.y = 48;
			option[1].Func = func_null;
	
			OptionLen = 0;
			OptionLoc = 0;
			break;											
		}
				
}

//I/O����
void page2area2(void){
		
		//switch(PageAreaRec[PageNum][AreaNum]){
			//case 0:
				//��һ�У��������	 
				ch_label[0].loc.x = 0;
				ch_label[0].loc.y = 0;
				ch_label[0].length = 4;
				ch_label[0].code[0] = 7;
				ch_label[0].code[1] = 8;
				ch_label[0].code[2] = 11;
				ch_label[0].code[3] = 12;
				ch_label[0].type = 'c';
				//�ڶ��е�һ��	I/O����
				en_label[0].loc.x = 8;
				en_label[0].loc.y = 16;
				en_label[0].length = 3;
				en_label[0].code[0] = 15;
				en_label[0].code[1] = 16;
				en_label[0].code[2] = 17;
				en_label[0].type = 'e';
				ch_label[1].loc.x = 20;
				ch_label[1].loc.y = 16;
				ch_label[1].length = 2;
				ch_label[1].code[0] = 7;
				ch_label[1].code[1] = 8;
				ch_label[1].type = 'c';
				//�����е�һ�� �����ź�n
				ch_label[2].loc.x = 8;
				ch_label[2].loc.y = 32;
				ch_label[2].length = 4;
				ch_label[2].code[0] = 33;
				ch_label[2].code[1] = 34;
				ch_label[2].code[2] = 29;
				ch_label[2].code[3] = 30;
				ch_label[2].type = 'c';
				en_label[1].loc.x = 40;
				en_label[1].loc.y = 32;
				en_label[1].length = 1;
				en_label[1].code[0] = 19 + PageAreaRec[PageNum][AreaNum];
				en_label[1].type = 'e';
			  		
				//�����е�һ��  ����\����
				ch_label[3].loc.x = 0;
				ch_label[3].loc.y = 48;
				ch_label[3].length = 2;
				ch_label[3].type = 'c';
				switch(  ParaArray[OutNormalState0+PageAreaRec[PageNum][AreaNum]]){
					case OutNormalState_open:
						ch_label[3].code[0] = 67;
						ch_label[3].code[1] = 68;
						break;
					case OutNormalState_close:
						ch_label[3].code[0] = 67;
						ch_label[3].code[1] = 69;	
						break;
					default:
						ch_label[3].length = 2;
						ch_label[3].code[0] = 109;
						ch_label[3].code[1] = 115;
						ch_label[3].code[2] = 110;
					//bug ȱ��Ĭ���� 
				}				
				
				LabelLenE = 2;
				//�����еڶ���  ��ʾ�ĺ���
				ch_label[4].type = 'c';
				switch( ParaArray[OutExpState0 + PageAreaRec[PageNum][AreaNum]] )
				{
					case NoDefined:			//δ����
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;
						ch_label[4].length = 3;
						ch_label[4].code[0] = 109;
						ch_label[4].code[1] = 93;
						ch_label[4].code[2] = 110;
						break;					
					case TotalFault:		//�ܹ���
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 3;
						ch_label[4].code[0] = 75;
						ch_label[4].code[1] = 17;
						ch_label[4].code[2] = 18;
						break;
					case FarawayFault:		//Զ������	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 74;
						ch_label[4].code[1] = 63;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;
					case LackPhaseFault:		//��Դȱ��   �����Դ  �������������! !!!	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;
						ch_label[4].length = 4;
						ch_label[4].code[0] = 77;
						ch_label[4].code[1] = 79;
						ch_label[4].code[2] = 82;
						ch_label[4].code[3] = 83;
						break;
					case ValveLocationFault:		//��λ����	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 76;
						ch_label[4].code[1] = 43;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;	
					case MotorOverHeartFault:		//�������	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 77;
						ch_label[4].code[1] = 78;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;
					case MidPosition1Fault:			//�м�λ��1����
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 27;
						ch_label[4].code[1] = 42;
						ch_label[4].code[2] = 43;
						ch_label[4].code[3] = 10;
						en_label[2].loc.x = 56;
						en_label[2].loc.y = 32;
						en_label[2].length = 1;
						en_label[2].code[0] = 19;
						en_label[2].type = 'e';
						LabelLenE = 3;
						break;
					case MidPosition2Fault:			//�м�λ��2����
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 27;
						ch_label[4].code[1] = 42;
						ch_label[4].code[2] = 43;
						ch_label[4].code[3] = 10;
						en_label[2].loc.x = 56;
						en_label[2].loc.y = 32;
						en_label[2].length = 1;
						en_label[2].code[0] = 20;
						en_label[2].type = 'e';
						LabelLenE = 3;
						break;
					case OpenDirJourneyLimit:  		//�����г���λ
						ch_label[4].loc.x = 16;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 6;
						ch_label[4].code[0] = 33;
						ch_label[4].code[1] = 35;
						ch_label[4].code[2] = 38;
						ch_label[4].code[3] = 39;
						ch_label[4].code[4] = 36;
						ch_label[4].code[5] = 37;
						break;
					case CloseDirJourneyLimit:
						ch_label[4].loc.x = 16;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 6;
						ch_label[4].code[0] = 34;
						ch_label[4].code[1] = 35;
						ch_label[4].code[2] = 38;
						ch_label[4].code[3] = 39;
						ch_label[4].code[4] = 36;
						ch_label[4].code[5] = 37;
						break;
					case OpenDirTorqueLimit:
						ch_label[4].loc.x = 16;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 6;
						ch_label[4].code[0] = 33;
						ch_label[4].code[1] = 35;
						ch_label[4].code[2] = 40;
						ch_label[4].code[3] = 41;
						ch_label[4].code[4] = 36;
						ch_label[4].code[5] = 37;
						break;
					case CloseDirTorqueLimit:
						ch_label[4].loc.x = 16;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 6;
						ch_label[4].code[0] = 34;
						ch_label[4].code[1] = 35;
						ch_label[4].code[2] = 40;
						ch_label[4].code[3] = 41;
						ch_label[4].code[4] = 36;
						ch_label[4].code[5] = 37;
						break;
					case SetValueFault:
						ch_label[4].loc.x = 16;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 5;
						ch_label[4].code[0] = 9;
						ch_label[4].code[1] = 32;
						ch_label[4].code[2] = 48;
						ch_label[4].code[3] = 70;
						ch_label[4].code[4] = 71;
						break;
					//bug ȱ��Ĭ���� 									
				}
				LabelLen = 5;
				
				OptionLen = 0;
				OptionLoc = 0;
				//break;
			
	//	}
}

//�������  ���ϲ���
void page2area3(void){
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У��������
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	����
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 70;
			ch_label[1].code[1] = 71;
			/*
			ch_label[1].code[4] = 92;
			ch_label[1].code[5] = 0;
			ch_label[1].code[6] = 1;
			ch_label[1].code[7] = 7;
			ch_label[1].code[8] = 8;*/
			ch_label[1].type = 'c';
			//�����е�һ��   ������ȹ��ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 7;
			ch_label[2].code[0] = 77;
			ch_label[2].code[1] = 78;
			ch_label[2].code[2] = 84;
			ch_label[2].code[3] = 85;
			ch_label[2].code[4] = 70;
			ch_label[2].code[5] = 71;
			ch_label[2].code[6] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
			
			NumShow(24,48,ParaArray[MotorOverHeartFaultCount]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �ڶ���ͬ case0

			//�����е�һ��   ��λ���ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 76;
			ch_label[2].code[1] = 43;
			ch_label[2].code[2] = 70;
			ch_label[2].code[3] = 71;
			ch_label[2].code[4] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
			
			NumShow(24,48, ParaArray[ValveLocationFaultCount]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;														
		case 2:
			//��һ�� �ڶ���ͬ case0
			//�����е�һ��   ��ת�ع��ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 40;
			ch_label[2].code[2] = 41;
			ch_label[2].code[3] = 70;
			ch_label[2].code[4] = 71;
			ch_label[2].code[5] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
						
			NumShow(24,48, ParaArray[OpenDirJourneyLimitFaultCount]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 3:
			//��һ�� �ڶ���ͬ case0
			//�����е�һ��   ��ת�ع��ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 40;
			ch_label[2].code[2] = 41;
			ch_label[2].code[3] = 70;
			ch_label[2].code[4] = 71;
			ch_label[2].code[5] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[CloseDirJourneyLimitFaultCount]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 4:
			//��һ�� �ڶ���ͬ case0
			//�����е�һ��   ���г̹��ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 38;
			ch_label[2].code[2] = 39;
			ch_label[2].code[3] = 70;
			ch_label[2].code[4] = 71;
			ch_label[2].code[5] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[OpenDirTorqueLimitFaultCount] );
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 5:
			//��һ�� �ڶ���ͬ case0
			//�����е�һ��   ���г̹��ϣ�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 38;
			ch_label[2].code[2] = 39;
			ch_label[2].code[3] = 70;
			ch_label[2].code[4] = 71;
			ch_label[2].code[5] = 105;
			ch_label[2].type = 'c';
			//������ ��
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[CloseDirTorqueLimitFaultCount]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;																								
	}	
}				

void page2area4(void){
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У��������
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	��������
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//�����е�һ��   ת��ADֵ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 40;
			ch_label[2].code[1] = 41;
			ch_label[2].type = 'c';
			//�����е�2��   AD
			en_label[0].loc.x = 24;
			en_label[0].loc.y = 32;
			en_label[0].length = 2;
			en_label[0].code[0] = 29;
			en_label[0].code[1] = 30;
			en_label[0].type = 'e';
			//�����е�3��   ֵ
			ch_label[3].loc.x = 32;
			ch_label[3].loc.y = 32;
			ch_label[3].length = 1;
			ch_label[3].code[0] = 48;
			ch_label[3].type = 'c';
			//������ ��
			
			NumShow(24,48, ParaArray[JourneyADCalue] );
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 1;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �ڶ���ͬ case0
			//�����е�һ��   ��������
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 99;
			ch_label[2].code[1] = 100;
			ch_label[2].code[2] = 107;
			ch_label[2].code[3] = 108;
			ch_label[2].type = 'c';
			
			NumShow(24,48, ParaArray[FactoryCode]);
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 3;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;																																		
	}	
}
	
//***********************************************************************************************************
//						
//											ҳ��3  ��������
//
//***********************************************************************************************************
//ҳ��3  �������� area0	  ��������
void page3area0()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	��������
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//�����е�һ�� ������λ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';	  		
			//�����е�һ��  �г�
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 38;
			ch_label[3].code[1] = 39;
			ch_label[3].type = 'c';
			//�����еڶ���  ת��
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 40;
			ch_label[4].code[1] = 41;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area0atom0op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area0atom0op1;
	
			OptionLen = 2;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ������λ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��  �г�
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 38;
			ch_label[3].code[1] = 39;
			ch_label[3].type = 'c';
			//�����еڶ���  ת��
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 40;
			ch_label[4].code[1] = 41;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area0atom1op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area0atom1op1;	
			OptionLen = 2;
			OptionLoc = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//�ر��������ģʽ
			break;
		case 2:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �м�λ��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 27;
			ch_label[2].code[1] = 42;
			ch_label[2].code[2] = 43;
			ch_label[2].code[3] = 10;
			ch_label[2].type = 'c';		  		
			
			LabelLen = 3;
			LabelLenE = 0;
	
	//����Ӧ����һ���������ֵĶ���������
			OptionLen = 0;
			OptionLoc = 0;
			
			//��������4����
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[MidPosition1]/100;
			input_op[0].Func = page3area0atom2inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[MidPosition1]%100/10;
			input_op[1].Func = func_null;
			//����һ��С����  ���Ժ��������һ������8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[MidPosition1]%10;
			input_op[2].Func = func_null;


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ

			break;
		case 3:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ��������ת��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 9;
			ch_label[2].code[3] = 10;
			ch_label[2].code[4] = 40;
			ch_label[2].code[5] = 41;
			ch_label[2].type = 'c';
		  					
			LabelLen = 3;
			LabelLenE = 0;
			OptionLen = 0;
			OptionLoc = 0;

			//�������
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[OpenDirJour]/100;
			input_op[0].Func = page3area0atom3inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[OpenDirJour]%100/10;
			input_op[1].Func = func_null;
			//����һ��С����  ���Ժ��������һ������8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[OpenDirJour]%10;
			input_op[2].Func = func_null;
			//��������õ�.��%


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ

			break;
		case 4:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ��������ת��
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 9;
			ch_label[2].code[3] = 10;
			ch_label[2].code[4] = 40;
			ch_label[2].code[5] = 41;
			ch_label[2].type = 'c';
		  			
			
			LabelLen = 3;
			LabelLenE = 0;

			OptionLen = 0;
			OptionLoc = 0;
//�������
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[CloseDirJour]/100;
			input_op[0].Func = page3area0atom4inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[CloseDirJour]%100/10;
			input_op[1].Func = func_null;
			//����һ��С����  ���Ժ��������һ������8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[CloseDirJour]%10;
			input_op[2].Func = func_null;


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ

			break;
		case 5:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �������Ʒ�ʽ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 44;
			ch_label[2].code[1] = 45;
			ch_label[2].code[2] = 15;
			ch_label[2].code[3] = 16;
			ch_label[2].code[4] = 63;
			ch_label[2].code[5] = 6;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��  ��λ
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 66;
			ch_label[3].code[1] = 37;
			ch_label[3].type = 'c';
			//�����еڶ���  ָ��λ��
			ch_label[4].loc.x = 32;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[2] = 64;
			ch_label[4].code[3] = 65;
			ch_label[4].code[2] = 37;
			ch_label[4].code[3] = 10;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area0atom5op0;
			option[1].loc.x = 24;
			option[1].loc.y = 48;
			option[1].Func = page3area0atom5op1;	
			OptionLen = 2;
			OptionLoc = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//���������ģʽ
			break;											
		}	
}

//ҳ��3 �ն�����  area6 �ն�  ʵ��λ���� ������ ��һ��
void page3area1()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	�ն�λ��
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].type = 'c';
			//�����е�һ�� ��ص�0%
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 121;
			ch_label[2].code[1] = 34;
			ch_label[2].code[2] = 136;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 32;
			en_label[0].loc.y = 32;
			en_label[0].length = 2;
			en_label[0].code[0] = 18;
			en_label[0].code[1] = 40;
			en_label[0].type = 'e';	
			//�����е�һ�� �� ȷ��
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1;
			ch_label[3].code[0] = 137;
			ch_label[3].type = 'c';

			ch_label[4].loc.x = 24;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 138;
			ch_label[4].code[1] = 139;
			ch_label[4].type = 'c';
		
			LabelLen = 5;
			LabelLenE = 1;

			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = func_null;		
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �� 
			//�ڶ��� �ն�λ�����޸� 
			//���˳�ȡ�� 
			//��ȷ�ϱ���
			//�ڶ��е�һ��	�ն�λ�����޸�
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 7;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].code[4] = 140;
			ch_label[1].code[5] = 141;
			ch_label[1].code[6] = 142;
			ch_label[1].type = 'c';
			//������ ���˳�ȡ�� 
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 119;
			ch_label[2].code[2] = 120;
			ch_label[2].code[3] = 144;
			ch_label[2].code[4] = 145;
			ch_label[2].type = 'c';
			//������ ��ȷ�ϱ���
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 5;
			ch_label[3].code[0] = 137;
			ch_label[3].code[1] = 138;
			ch_label[3].code[2] = 139;
			ch_label[3].code[3] = 66;
			ch_label[3].code[4] = 143;
			ch_label[3].type = 'c';	
			
			LabelLen = 4;
			LabelLenE = 0;

			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area3atom0op;		
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 2:
			//��һ�� ͬ case 0 
			//�ڶ��е�һ��	�ն�λ��
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].type = 'c';
			//�����е�һ�� �뿪��100%
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 121;
			ch_label[2].code[1] = 33;
			ch_label[2].code[2] = 136;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 32;
			en_label[0].loc.y = 32;
			en_label[0].length = 4;
			en_label[0].code[0] = 19;
			en_label[0].code[1] = 18;
			en_label[0].code[2] = 18;
			en_label[0].code[3] = 40;
			en_label[0].type = 'e';	
			//�����е�һ�� �� ȷ��
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1;
			ch_label[3].code[0] = 137;
			ch_label[3].type = 'c';

			ch_label[4].loc.x = 24;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 138;
			ch_label[4].code[1] = 139;
			ch_label[4].type = 'c';
		
			LabelLen = 5;
			LabelLenE = 1;

			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = func_null;		
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 3:
						//��һ�� �� 
			//�ڶ��� �ն�λ�����޸� 
			//���˳�ȡ�� 
			//��ȷ�ϱ���
			//�ڶ��е�һ��	�ն�λ�����޸�
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 7;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].code[4] = 140;
			ch_label[1].code[5] = 141;
			ch_label[1].code[6] = 142;
			ch_label[1].type = 'c';
			//������ ���˳�ȡ�� 
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 119;
			ch_label[2].code[2] = 120;
			ch_label[2].code[3] = 144;
			ch_label[2].code[4] = 145;
			ch_label[2].type = 'c';
			//������ ��ȷ�ϱ���
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 5;
			ch_label[3].code[0] = 137;
			ch_label[3].code[1] = 138;
			ch_label[3].code[2] = 139;
			ch_label[3].code[3] = 66;
			ch_label[3].code[4] = 143;
			ch_label[3].type = 'c';	
			
			LabelLen = 4;
			LabelLenE = 0;

			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area3atom1op;		
			OptionLen = 1;
			OptionLoc = 0;
			break;																					
		}	
}

//ҳ��3 ��������  area2	I/O����
void page3area2()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;		
	switch(PageAreaRec[PageNum][AreaNum]%13){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	I/O����
			en_label[0].loc.x = 8;
			en_label[0].loc.y = 16;
			en_label[0].length = 3;
			en_label[0].code[0] = 15;
			en_label[0].code[1] = 16;
			en_label[0].code[2] = 17;
			en_label[0].type = 'e';
			ch_label[1].loc.x = 20;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 7;
			ch_label[1].code[1] = 8;
			ch_label[1].type = 'c';
			//�����е�һ�� �����ź�IOSetRec
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 34;
			ch_label[2].code[2] = 29;
			ch_label[2].code[3] = 30;
			ch_label[2].type = 'c';
			en_label[1].loc.x = 40;
			en_label[1].loc.y = 32;
			en_label[1].length = 1;
			en_label[1].code[0] = 19 + IOSetRec;
			en_label[1].type = 'e';
		  		
			//�����е�һ��  ����
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 67;
			ch_label[3].code[1] = 68;
			ch_label[3].type = 'c';
			//�����еڶ���  ����
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 67;
			ch_label[4].code[1] = 69;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom0op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area2atom0op1;
	
			OptionLen = 2;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����еڶ��� 	 ��ʾ
			ch_label[3].loc.x = 48;
			ch_label[3].loc.y = 32;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 80;
			ch_label[3].code[1] = 81;
			ch_label[3].type = 'c';
		  		
			//�����е�һ��  �ܹ���
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 3;
			ch_label[4].code[0] = 75;
			ch_label[4].code[1] = 70;
			ch_label[4].code[2] = 71;
			ch_label[4].type = 'c';
	
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom1op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 2:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  Զ������
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 74;
			ch_label[4].code[1] = 63;
			ch_label[4].code[2] = 70;
			ch_label[4].code[3] = 71;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom2op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 3:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  ��Դȱ��
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 77;
			ch_label[4].code[1] = 79;
			ch_label[4].code[2] = 82;
			ch_label[4].code[3] = 83;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom3op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 4:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  ��λ����
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 76;
			ch_label[4].code[1] = 43;
			ch_label[4].code[2] = 70;
			ch_label[4].code[3] = 71;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom4op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 5:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �������
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 77;
			ch_label[4].code[1] = 78;
			ch_label[4].code[2] = 84;
			ch_label[4].code[3] = 85;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom5op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 6:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �м�λ��1
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 27;
			ch_label[4].code[1] = 42;
			ch_label[4].code[2] = 43;
			ch_label[4].code[3] = 10;
			ch_label[4].type = 'c';
			en_label[2].loc.x = 48;
			en_label[2].loc.y = 48;
			en_label[2].length = 1;
			en_label[2].code[0] = 19;
			en_label[2].type = 'e';	  		
			
			LabelLen = 5;
			LabelLenE = 3;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom6op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 7:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �м�λ��2
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 4;
			ch_label[4].code[0] = 27;
			ch_label[4].code[1] = 42;
			ch_label[4].code[2] = 43;
			ch_label[4].code[3] = 10;
			ch_label[4].type = 'c';
			en_label[2].loc.x = 48;
			en_label[2].loc.y = 48;
			en_label[2].length = 1;
			en_label[2].code[0] = 20;
			en_label[2].type = 'e';	  		
			
			LabelLen = 5;
			LabelLenE = 3;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom7op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 8:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �����г���λ
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 6;
			ch_label[4].code[0] = 33;
			ch_label[4].code[1] = 35;
			ch_label[4].code[2] = 38;
			ch_label[4].code[3] = 39;
			ch_label[4].code[4] = 36;
			ch_label[4].code[5] = 37;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom8op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 9:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �����г���λ
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 6;
			ch_label[4].code[0] = 34;
			ch_label[4].code[1] = 35;
			ch_label[4].code[2] = 38;
			ch_label[4].code[3] = 39;
			ch_label[4].code[4] = 36;
			ch_label[4].code[5] = 37;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom9op;
			OptionLen = 1;
			OptionLoc = 0;
			break;	
		case 10:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  ����ת����λ
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 6;
			ch_label[4].code[0] = 33;
			ch_label[4].code[1] = 35;
			ch_label[4].code[2] = 40;
			ch_label[4].code[3] = 41;
			ch_label[4].code[4] = 36;
			ch_label[4].code[5] = 37;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom10op;
			OptionLen = 1;
			OptionLoc = 0;
			break;	
		case 11:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  ����ת����λ
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 6;
			ch_label[4].code[0] = 34;
			ch_label[4].code[1] = 35;
			ch_label[4].code[2] = 40;
			ch_label[4].code[3] = 41;
			ch_label[4].code[4] = 36;
			ch_label[4].code[5] = 37;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom11op;
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 12:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ�� �趨ֵ����
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 5;
			ch_label[4].code[0] = 9;
			ch_label[4].code[1] = 32;
			ch_label[4].code[2] = 48;
			ch_label[4].code[3] = 70;
			ch_label[4].code[4] = 71;
			ch_label[4].type = 'c';	  		
			
			LabelLen = 5;
			LabelLenE = 2;
	
			option[0].loc.x = 8;
			option[0].loc.y = 48;
			option[0].Func = page3area2atom12op;
			OptionLen = 1;
			OptionLoc = 0;
			break;																																			
		}
	
}



//ҳ��3  �������� area1 ���Ʋ���
void page3area3()
{
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	���Ʋ��� �͵ز���
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 15;
			ch_label[1].code[1] = 16;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//�����е�һ��   �͵�
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 0;
			ch_label[2].code[1] = 1;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��   Զ�� 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 2;
			ch_label[3].code[1] = 3;
			ch_label[3].type = 'c';
	
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//�ر��������ģʽ
	
			option[0].loc.x = 16;
			option[0].loc.y = 32;
			option[0].Func = page3area1atom0op0;
			option[1].loc.x = 16;
			option[1].loc.y = 48;
			option[1].Func = page3area1atom0op1;
	
			OptionLen = 2;
			OptionLoc = 0;
			break;											
		}
}

void page3area3sub0()
{
	SubAreaLen = 1;		//subareaֻ��1��ҳ��
	SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	�ؾ͵��Ʋ���
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 6;
			ch_label[1].code[0] = 0;
			ch_label[1].code[1] = 1;
			ch_label[1].code[2] = 15;
			ch_label[1].code[3] = 16;
			ch_label[1].code[4] = 7;
			ch_label[1].code[5] = 8;
			ch_label[1].type = 'c';
			//�����е�һ��   ����ʽ
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 59;
			ch_label[2].code[1] = 60;
			ch_label[2].code[2] = 6;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��   ά��ʽ 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 86;
			ch_label[3].code[1] = 87;
			ch_label[3].code[2] = 6;
			ch_label[3].type = 'c';
	
		//�����ѡ��Ӧ���ǽ���sub area	
			LabelLen = 4;
			LabelLenE = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//�ر��������ģʽ
	
			option[0].loc.x = 16;
			option[0].loc.y = 32;
			option[0].Func = page3area1atom0sub0op0;
			option[1].loc.x = 16;
			option[1].loc.y = 48;
			option[1].Func = page3area1atom0sub0op1; 
	
			OptionLen = 2;
			OptionLoc = 0;
		break;
	}	
}

void page3area3sub1()
{
	SubAreaLen = 1;		//subareaֻ��1��ҳ��
	SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	Զ�̿��Ʋ���
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 6;
			ch_label[1].code[0] = 2;
			ch_label[1].code[1] = 3;
			ch_label[1].code[2] = 15;
			ch_label[1].code[3] = 16;
			ch_label[1].code[4] = 7;
			ch_label[1].code[5] = 8;
			ch_label[1].type = 'c';
			//�����е�һ��   ����ʽ
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 59;
			ch_label[2].code[1] = 60;
			ch_label[2].code[2] = 6;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��   ά��ʽ 
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 86;
			ch_label[3].code[1] = 87;
			ch_label[3].code[2] = 6;
			ch_label[3].type = 'c';
			//�����е�2��   ˫λʽ
			//wz ��Ҫ��
			ch_label[5].loc.x = 40;
			ch_label[5].loc.y = 48;
			ch_label[5].length = 3;
			ch_label[5].code[0] = 91;
			ch_label[5].code[1] = 37;
			ch_label[5].code[2] = 6;
			ch_label[5].type = 'c';
		  		
			//�����е�2��   ģ��ʽ 
			ch_label[4].loc.x = 40;
			ch_label[4].loc.y = 32;
			ch_label[4].length = 3;
			ch_label[4].code[0] = 88;
			ch_label[4].code[1] = 89;
			ch_label[4].code[2] = 6;
			ch_label[4].type = 'c';
	
			LabelLen = 5;
			LabelLenE = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//�ر��������ģʽ
	
			option[0].loc.x = 0;
			option[0].loc.y = 32;
			option[0].Func = page3area1atom0sub1op0;
			option[1].loc.x = 0;
			option[1].loc.y = 48;
			option[1].Func = page3area1atom0sub1op1; 
			/*
			option[2].loc.x = 32;
			option[2].loc.y = 32;
			option[2].Func = page3area1atom0sub1op2;
			*/
			option[2].loc.x = 32;
			option[2].loc.y = 32;
			option[2].Func = page3area1atom0sub1op3; 
	
			OptionLen = 3;
			OptionLoc = 0;
			break;
	}	
}


//ҳ��3 ��������  area4  �ź�
void page3area4(){
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	�źű궨
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 29;
			ch_label[1].code[1] = 30;
			ch_label[1].code[2] = 31;
			ch_label[1].code[3] = 32;
			ch_label[1].type = 'c';
			//�����е�һ�� ����ֵ 4mA
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 46;
			ch_label[2].code[1] = 47;
			ch_label[2].code[2] = 48;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 32;
			en_label[0].loc.y = 32;
			en_label[0].length = 3;
			en_label[0].code[0] = 22;
			en_label[0].code[1] = 28;
			en_label[0].code[2] = 29;
			en_label[0].type = 'e';	

			LabelLen = 3;
			LabelLenE = 1;

			option[0].loc.x = 16;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom0op;		
			OptionLen = 1;
			OptionLoc = 0;

			break;
		case 1:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� ����ֵ 20mA
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 46;
			ch_label[2].code[1] = 47;
			ch_label[2].code[2] = 48;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 40;
			en_label[0].loc.y = 32;
			en_label[0].length = 4;
			en_label[0].code[0] = 20;
			en_label[0].code[1] = 18;
			en_label[0].code[2] = 28;
			en_label[0].code[3] = 29;
			en_label[0].type = 'e';	

			LabelLen = 3;
			LabelLenE = 1;
			option[0].loc.x = 16;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom1op;		
			OptionLen = 1;
			OptionLoc = 0;

			break;

		case 2:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ�� �趨ֵ 4mA
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 9;
			ch_label[2].code[1] = 93;
			ch_label[2].code[2] = 48;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 40;
			en_label[0].loc.y = 32;
			en_label[0].length = 3;
			en_label[0].code[0] = 22;
			en_label[0].code[1] = 28;
			en_label[0].code[2] = 29;
			en_label[0].type = 'e';	
			LabelLen = 3;
			LabelLenE = 1;

			option[0].loc.x = 16;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom2op;		
			OptionLen = 1;
			OptionLoc = 0;


			break;
		case 3:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�����е�һ��  �źű궨
			//�����е�һ�� �趨ֵ 20mA
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 9;
			ch_label[2].code[1] = 93;
			ch_label[2].code[2] = 48;
			ch_label[2].type = 'c';
			en_label[0].loc.x = 40;
			en_label[0].loc.y = 32;
			en_label[0].length = 4;
			en_label[0].code[0] = 20;
			en_label[0].code[1] = 18;
			en_label[0].code[2] = 28;
			en_label[0].code[3] = 29;
			en_label[0].type = 'e';
	
			LabelLen = 3;
			LabelLenE = 1;

			option[0].loc.x = 16;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom3op;
				
			OptionLen = 1;
			OptionLoc = 0;

			break;
			/*
		case 4:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�ڶ��е�һ��  �źű궨
			//�����е�һ��  ����ǰλ��
			//�����е�һ��  �趨Ϊ
			//�����еڶ��� 0%
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 124;
			ch_label[2].code[1] = 125;
			ch_label[2].code[2] = 126;
			ch_label[2].code[3] = 127;
			ch_label[2].code[4] = 128;

			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 129;
			ch_label[3].code[1] = 130;
			ch_label[3].code[2] = 131;
			ch_label[3].type = 'c';

			ch_label[4].loc.x = 40;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 76;
			ch_label[4].code[1] = 127;
			ch_label[4].type = 'c';

			en_label[0].loc.x = 32;
			en_label[0].loc.y = 48;
			en_label[0].length = 2;
			en_label[0].code[0] = 18;
			en_label[0].code[1] = 40;
			en_label[0].type = 'e';	
			
			LabelLen = 5;
			LabelLenE = 1;

			option[0].loc.x = 0;
			option[0].loc.y = 32;
			option[0].Func = page3area3atom4op;		
			OptionLen = 1;
			OptionLoc = 0;
			break;
		case 5:
			//��һ�� �� �ڶ��� ������ ͬ case 0 
			//�ڶ��е�һ��  �źű궨
			//�����е�һ��  ����ǰλ��
			//�����е�һ��  �趨Ϊ
			//�����еڶ��� 0%
			en_label[0].loc.x = 32;
			en_label[0].loc.y = 48;
			en_label[0].length = 4;
			en_label[0].code[0] = 19;
			en_label[0].code[1] = 18;
			en_label[0].code[2] = 18;
			en_label[0].code[3] = 40;
			en_label[0].type = 'e';
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 76;
			ch_label[4].code[1] = 127;	
			ch_label[4].type = 'c';
			
			LabelLen = 5;
			LabelLenE = 1;
			option[0].loc.x = 0;
			option[0].loc.y = 32;
			option[0].Func = page3area3atom5op;		
			OptionLen = 1;
			OptionLoc = 0;
			break;	*/																				
		}	
}

//����
void page3area5()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	��������
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 9;
			ch_label[1].code[3] = 10;
			ch_label[1].type = 'c';
			//�����е�һ�� �ָ���������
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 6;
			ch_label[2].code[0] = 97;
			ch_label[2].code[1] = 98;
			ch_label[2].code[2] = 99;
			ch_label[2].code[3] = 100;
			ch_label[2].code[4] = 9;
			ch_label[2].code[5] = 10;
			ch_label[2].type = 'c';	  		
			//�����е�һ��  ���ָ�
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 97;
			ch_label[3].code[2] = 98;
			ch_label[3].type = 'c';
			//�����еڶ���  �ָ�
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 97;
			ch_label[4].code[1] = 98;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom0op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area4atom0op1;
	
			OptionLen = 2;
			OptionLoc = 0;
			break;
		case 1:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �������
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 103;
			ch_label[2].code[1] = 104;
			ch_label[2].code[2] = 101;
			ch_label[2].code[3] = 102;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��  �����
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 101;
			ch_label[3].code[2] = 102;
			ch_label[3].type = 'c';
			//�����еڶ���  ���
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 101;
			ch_label[4].code[1] = 102;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area4atom1op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area4atom1op1;	
			OptionLen = 2;
			OptionLoc = 0;
			break;
		case 2:
			//��һ�� �� �ڶ��� ͬ case 0 
			//�����е�һ�� �޸�����
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 141;
			ch_label[2].code[1] = 142;
			ch_label[2].code[2] = 157;
			ch_label[2].code[3] = 158;
			ch_label[2].type = 'c';
		  		
			//�����е�һ��  ���޸�
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 141;
			ch_label[3].code[2] = 142;
			ch_label[3].type = 'c';
			//�����еڶ���  �޸�
			ch_label[4].loc.x = 48;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 141;
			ch_label[4].code[1] = 142;
			ch_label[4].type = 'c';		
			
			LabelLen = 5;
			LabelLenE = 0;
	
			option[0].loc.x = 0;
			option[0].loc.y = 48;
			option[0].Func = page3area5atom2op0;
			option[1].loc.x = 40;
			option[1].loc.y = 48;
			option[1].Func = page3area5atom2op1;	
			OptionLen = 2;
			OptionLoc = 0;
			
			SubAreaRec = 0;

			break;
		case 3:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	��������
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 9;
			ch_label[1].code[3] = 10;
			ch_label[1].type = 'c';
			//�����е�һ�� ��ȷ�Ͻ���
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 138;
			ch_label[2].code[2] = 139;
			ch_label[2].code[3] = 146;
			ch_label[2].code[4] = 147;
			ch_label[2].type = 'c';	  		
			//�����е�һ��  ������ѧϰģʽ
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 7;
			ch_label[3].code[0] = 7;
			ch_label[3].code[1] = 8;
			ch_label[3].code[2] = 111;
			ch_label[3].code[3] = 112;
			ch_label[3].code[4] = 113;
			ch_label[3].code[5] = 88;
			ch_label[3].code[6] = 6;
			ch_label[3].type = 'c';
	
			
			LabelLen = 4;
			LabelLenE = 0;
	
			option[0].loc.x = 0;
			option[0].loc.y = 32;
			option[0].Func = page3area5atom3op;

	
			OptionLen = 1;
			OptionLoc = 0;
			break;										
		}	
}

//���������
//������֤�����Ƿ���ȷ
void page3area5atom2in0()
{
	int temp = 0 ;
	temp = 	input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	//������ȷ
	if ( temp == ParaArray[Security] )
	{
		SubAreaRec = 1;			//��ת������ҳ��1������������
		SubAreaChangeFlag = 1;		
	}else{						//���벻��ȷ��ˢ�µ�ǰҳ��
		SubAreaRec = 0;
		SubAreaChangeFlag = 1;	
	}		
}
//����������
void page3area5atom2in1()
{
	NewSecurity = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	SubAreaRec = 2;			//��ת������ҳ��2����֤������
	SubAreaChangeFlag = 1;		
}
//�ٴ�����������
void page3area5atom2in2()
{
	NewSecurityAgain = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	if( NewSecurityAgain == NewSecurity){
		ParaArray[Security] = NewSecurity;
		SubAreaRec = 3;			//��ת������ҳ��3���������óɹ�
		SubAreaChangeFlag = 1;			
	}else{
		SubAreaRec = 4;			//��ת������ҳ��4����������ʧ��
		SubAreaChangeFlag = 1;		
	}
}



//�������ý���
void page3area5sub1()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;
		
 	SubAreaLen = 6;		//subarea��4��ҳ��
	//SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:	
		
			//�ڶ��е�һ��  �����������
			ch_label[0].loc.x = 8;
			ch_label[0].loc.y = 16;
			ch_label[0].length = 6;
			ch_label[0].code[0] = 121;
			ch_label[0].code[1] = 155;
			ch_label[0].code[2] = 156;
			ch_label[0].code[3] = 159;
			ch_label[0].code[4] = 157;
			ch_label[0].code[5] = 158;
			ch_label[0].type = 'c';
			LabelLen = 1;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;
		
			//��������4����
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 32;
			input_op[0].num = 0;
			input_op[0].Func = page3area5atom2in0;
			input_op[1].loc.x = 24;
			input_op[1].loc.y = 32;
			input_op[1].num =0;
			input_op[1].Func = func_null;
			input_op[2].loc.x = 32;
			input_op[2].loc.y = 32;
			input_op[2].num = 0;
			input_op[2].Func = func_null;
			input_op[3].loc.x = 40;
			input_op[3].loc.y = 32;
			input_op[3].num = 0;
			input_op[3].Func = func_null;
		
			InputOpLen = 4;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ	
		break;
		case 1:
			//�ڶ��е�һ��  ������������
			ch_label[0].loc.x = 8;
			ch_label[0].loc.y = 16;
			ch_label[0].length = 6;
			ch_label[0].code[0] = 121;
			ch_label[0].code[1] = 9;
			ch_label[0].code[2] = 10;
			ch_label[0].code[3] = 160;
			ch_label[0].code[4] = 157;
			ch_label[0].code[5] = 158;
			ch_label[0].type = 'c';
			LabelLen = 1;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;
		
			//��������4����
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 32;
			input_op[0].num = 0;
			input_op[0].Func = page3area5atom2in1;
			input_op[1].loc.x = 24;
			input_op[1].loc.y = 32;
			input_op[1].num =0;
			input_op[1].Func = func_null;
			input_op[2].loc.x = 32;
			input_op[2].loc.y = 32;
			input_op[2].num = 0;
			input_op[2].Func = func_null;
			input_op[3].loc.x = 40;
			input_op[3].loc.y = 32;
			input_op[3].num = 0;
			input_op[3].Func = func_null;
		
			InputOpLen = 4;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ	
		break;
		case 2:
			//�ڶ��е�һ��  ���ٴ�����
			ch_label[0].loc.x = 8;
			ch_label[0].loc.y = 16;
			ch_label[0].length = 5;
			ch_label[0].code[0] = 121;
			ch_label[0].code[1] = 161;
			ch_label[0].code[2] = 162;
			ch_label[0].code[3] = 155;
			ch_label[0].code[4] = 156;
			ch_label[0].type = 'c';
			LabelLen = 1;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;
		
			//��������4����
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 32;
			input_op[0].num = 0;
			input_op[0].Func = page3area5atom2in2;
			input_op[1].loc.x = 24;
			input_op[1].loc.y = 32;
			input_op[1].num =0;
			input_op[1].Func = func_null;
			input_op[2].loc.x = 32;
			input_op[2].loc.y = 32;
			input_op[2].num = 0;
			input_op[2].Func = func_null;
			input_op[3].loc.x = 40;
			input_op[3].loc.y = 32;
			input_op[3].num = 0;
			input_op[3].Func = func_null;
		
			InputOpLen = 4;
			InputOpRec = 0;
			InputMode = 1;		//���������ģʽ	
		break;
		case 3:
			//�ڶ��е�һ��  �����޸ĳɹ�
			ch_label[0].loc.x = 8;
			ch_label[0].loc.y = 16;
			ch_label[0].length = 6;
			ch_label[0].code[0] = 157;
			ch_label[0].code[1] = 158;
			ch_label[0].code[2] = 141;
			ch_label[0].code[3] = 142;
			ch_label[0].code[4] = 117;
			ch_label[0].code[5] = 163;
			ch_label[0].type = 'c';
			LabelLen = 1;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;

			SubAreaRec = 5;
		
		break;
		case 4:
			//�ڶ��е�һ��  �������벻һ��
			//�����޸�ʧ��
			ch_label[0].loc.x = 8;
			ch_label[0].loc.y = 16;
			ch_label[0].length = 7;
			ch_label[0].code[0] = 164;
			ch_label[0].code[1] = 106;
			ch_label[0].code[2] = 155;
			ch_label[0].code[3] = 156;
			ch_label[0].code[4] = 165;
			ch_label[0].code[5] = 166;
			ch_label[0].code[6] = 167;
			ch_label[0].type = 'c';
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 32;
			ch_label[1].length = 6;
			ch_label[1].code[0] = 157;
			ch_label[1].code[1] = 158;
			ch_label[1].code[2] = 141;
			ch_label[1].code[3] = 142;
			ch_label[1].code[4] = 52;
			ch_label[1].code[5] = 168;
			ch_label[1].type = 'c';
			LabelLen = 2;
			LabelLenE = 0;
		
			OptionLen = 0;
			OptionLoc = 0;

			SubAreaRec  = 5;
		
		break;
					
	}		
}

//����������
void page3area5sub0()
{
	//������
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	

	//��һ�У���������
	/*	 
	ch_label[0].loc.x = 0;
	ch_label[0].loc.y = 0;
	ch_label[0].length = 4;
	ch_label[0].code[0] = 7;
	ch_label[0].code[1] = 8;
	ch_label[0].code[2] = 9;
	ch_label[0].code[3] = 10;
	ch_label[0].type = 'c';
	*/
	//�ڶ��е�һ��  ����������
	ch_label[0].loc.x = 8;
	ch_label[0].loc.y = 16;
	ch_label[0].length = 5;
	ch_label[0].code[0] = 7;
	ch_label[0].code[1] = 8;
	ch_label[0].code[2] = 111;
	ch_label[0].code[3] = 114;
	ch_label[0].code[4] = 115;
	//ch_label[0].code[3] = 10;
	ch_label[0].type = 'c';
	//�����е�һ�� ���Ե�
	
	ch_label[1].loc.x = 8;
	ch_label[1].loc.y = 32;
	ch_label[1].length = 3;
	ch_label[1].code[0] = 121;
	ch_label[1].code[1] = 122;
	ch_label[1].code[2] = 123;
	ch_label[1].type = 'c';	  		

	ch_label[2].loc.x = 48;
	ch_label[2].loc.y = 48;
	ch_label[2].length = 2;
	ch_label[2].code[0] = 119;
	ch_label[2].code[1] = 120;
	ch_label[2].type = 'c';		

	LabelLen = 3;
	LabelLenE = 0;

	
	option[0].loc.x = 40;
	option[0].loc.y = 48;
	option[0].Func = page3area5atom0op0;  //�˳�


	OptionLen = 1;
	OptionLoc = 0;

	auto_set_flag = 1;
	AutoSM = 0;				
}


//���������
void auto_set_exit(void)
{		
	//�����е�һ��  ��ɣ�
	ch_label[0].loc.x = 8;
	ch_label[0].loc.y = 48;
	ch_label[0].length = 3;
	ch_label[0].code[0] = 116;
	ch_label[0].code[1] = 117;
	ch_label[0].code[2] = 118;
	ch_label[0].type = 'c';	

	MenuLabelShow(ch_label[0]);				
}

//****************************************************areaģʽ**************************************************
void area_mode(void)
{
	int cnt_label = 0;
//	int cnt_option = 0;
//	int cnt_inop = 0;
	//area_clear();
	Fill_RAM(0x00);
	//Fill_Block(0,40,0,32,0x00);  //����������кţ��ص��кţ�����кţ��յ��кţ����Ҷ�ֵ
	//page2 �������
	if(PageNum == 2){
		switch(AreaNum){
			case 0:	
				page2area0();//�����������
				break;				
			case 1:
				page2area1();//���Ʋ������	
				break;
			case 2:
				page2area2();
				break;
			case 3:
				page2area3();
				break;
			case 4:
				page2area4();
				break;
		}	
	}
	//page3 ��������
	if(PageNum == 3){
		switch(AreaNum){
			case 0:	
				page3area0();	//������������
				break;
			case 1:
				page3area1();	//�ն˲�������
				break;
			case 2:
				page3area2(); 	//IO��������
				break;		
			case 3:	   			//���Ʋ������ã���������subarea
				if(SubAreaMode == 0)
					page3area3();
				else if(SubAreaMode == 1)
				{
					switch(SubAreaNum){
						case 0:
							page3area3sub0();	
						break;
						case 1:
							page3area3sub1();
						break;
					}
				}							
				break; 
			/*
			case 3:
				page3area3();
				break;
				*/
			case 4:
				page3area4();
				break;
			case 5:				
				if(SubAreaMode == 0)
					page3area5();
				else if(SubAreaMode == 1)
				{
					switch(SubAreaNum){
						case 0:
							page3area5sub0();			//��ѧϰģʽ���
						break;					
						case 1:
							page3area5sub1();			//�����������
						break;
					}
				}							
				break; 

			/*
			case 6:
				page3area6();
				break;*/
		}	
	}
	//��ʾ��ǰҳ��ľ�̬����
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}
	//��ʾ��ǰҳ��ĵ�һ��op
	if(OptionLen > 0)
		OptionShow(option[0]);
	if(InputOpLen > 0){
		InputNumAllShow(input_op , InputOpLen);
		InputNumChangedShow( input_op[0] );
	}			
}

