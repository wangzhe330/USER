#include "led.h"
#include "oled.h"
#include "page.h"
#include "list.h"
#include "key.h"
#include "area.h"
#include "atom.h"
#include "para.h"
#include "ssr.h"
#include "current.h"
#include "control.h"
#include "io.h"

//ȫ�ֱ���
extern Menu_Option option[6];
extern Menu_Label ch_label[8];
extern Menu_InputOp input_op[5];		//�����������
extern M_Page Page[PageSum];

extern int ValveOpenError,ValveCloseError;


//���ļ���Ϊ���ֲ�ͬģʽ�µİ�����Ӧ����
void PageKeyResponse(int Key_State )
{
	//������������ESC�� ���˻�����һ��ҳ��
	if(Key_State == KEY_2){
#ifdef DEBUG_MODE	
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		if(PageNum == 2 || PageNum == 6) //�Ӳ���������桢���������������˻�ѡ�����
			PageNum = Page[PageNum].parent;
		else if(PageNum == 3) //�Ӳ������ý����˳�����Ҫ�ж��Ƿ񱣴�
		{
			if(!BufferCmp(ParaArray,ParaArray_copy,ParaArrayLength))
				PageNum = 5;
			else
				PageNum = 4;
		}
		
		//-----------------------------WZ
		//��������
		//��������жϲ����Ƿ�ı�ĺ�����ѯ���Ƿ�Ҫ�����ҳ�档
		
		//option[OptionLoc].Func();
		PageChageFlage = 1;			
	}
	//���ĸ�������OK����ִ��ѡ��ĺ���
	if(Key_State == KEY_3){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		//wz
		if(OptionLen != 0 )
			option[OptionLoc].Func();
		//PageChageFlage = 1;			
	}
	//��5��������next�����ڵ�ǰҳ������ѯ
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
		if(OptionLen > 0)
			OptionChageFlage = 1;			
	}
	//ҳ��仯����
	if(PageChageFlage == 1){
		//PageNum = 1 - PageNum;
		PageChageFlage = 0;
		Fill_RAM(0x00);			// Clear Screen
		
		/*
		if(ReturnFromAreaFlag == 0)	{
			OptionLoc = 0; 		//ѡ���0λ��
		}
		if(ReturnFromAreaFlag == 1)
		{
			OptionLoc = AreaNum;
			ReturnFromAreaFlag = 0;
		} */
		page_mode();	
	}
	//ѡ��仯����
	if(OptionChageFlage == 1){
		if(OptionLoc+1 == OptionLen) {
			OptionChage(option[OptionLoc],option[0]);
		}else{
			OptionChage(option[OptionLoc],option[OptionLoc+1]);
		}
		OptionLoc++;
		if(OptionLoc == OptionLen) 
			OptionLoc = 0;
		OptionChageFlage = 0;
	}	
}

void AreaKeyResponse(int Key_State)
{
	//int Key_State = 0;	

	//Key_State = myKey_Scan();

	//������������ESC�� ���˻���һ��Area  �������˳� subarea
	if(Key_State == KEY_2){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		
		if(PageAreaRec[PageNum][AreaNum]!=0)
		{	
			PageAreaRec[PageNum][AreaNum]--;
			AreaChageFlag = 1;
		} 
		else if(SubAreaMode == 0)
		{			
			AreaMode = 0;  //�˳�Areaģʽ
			ReturnFromAreaFlag = 1;			//	
			page_mode();	
		}
		else if( SubAreaMode == 1){
			if(SubAreaRec == 0){ 	//�˳�SubAreaģʽ
				SubAreaMode = 0;
				AreaChageFlag = 1;
			}else{
				SubAreaRec--;
			}
		}
	}
	//���ĸ�������OK�������浱ǰ����   ����  ������һ��Area   ���߽���subarea   
	if(Key_State == KEY_3){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		//option[OptionLoc].Func();
		//PageChageFlage = 1;
		if(SubAreaMode == 0){
			if(PageAreaRec[PageNum][AreaNum]+1 != PageAreaLen[PageNum][AreaNum]){
	
				if( (PageNum == 3 && AreaNum == 2 && PageAreaRec[3][2]%13!=0) ){
					IOSetRec++; //ÿ��IO������Ҫ13��ҳ��
					if(IOSetRec == 6)
					{
						IOSetRec = 0;
						AreaMode = 0;
						PageChageFlage = 1;	
						ReturnFromAreaFlag = 1;
						PageAreaRec[PageNum][AreaNum] = 0;
					}
					else{
						PageAreaRec[PageNum][AreaNum] = IOSetRec*13;
						AreaChageFlag = 1;
					}
				}
				else{
					PageAreaRec[PageNum][AreaNum]++;
					AreaChageFlag = 1;
				}
					
			}
			else if(PageAreaRec[PageNum][AreaNum]+1 == PageAreaLen[PageNum][AreaNum])
			{
				//��ǰare��������ok��Ҫ����	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[PageNum][AreaNum] = 0;
				//page_esc();
				//PageAreaRec[PageNum][AreaNum]--;
				//AreaChageFlag = 1;
				//SubAreaMode = 0;
				//AreaChageFlag = 1;	
			//�������ʲô�������Ļ�������Ҫһ��һ�����˳����area�������REC����Ļ�������area��ѭ��
			//PageAreaRec[PageNum][AreaNum] = 0;
			}
		}

		if(SubAreaMode == 1){
			if(SubAreaRec+1 == SubAreaLen){		//subare�������� �˳�����
				SubAreaMode = 0;
				SubAreaNum = 0;
				PageAreaRec[PageNum][AreaNum] = 0;
				AreaChageFlag = 1;
			}
			else
			{
				if( PageNum != 3 && AreaNum != 5 )
					SubAreaRec++;
			}
			//�������� ��sub3 ���� sub4	�����˳�
			if( PageNum == 3 && AreaNum == 5 )
			{
				if(SubAreaRec == 5)
				{
					SubAreaMode = 0;
					SubAreaNum = 0;
					PageAreaRec[PageNum][AreaNum] = 0;
					AreaChageFlag = 1;	
				}	
			}
		}
		//wz �����Ƿ�ȥ�������	 || InputOpLen!=0
		//if( PageNum != 2 )
		
		if(OptionLen != 0 )  
			option[OptionLoc].Func();
		//if(InputOpLen!=0 )	   //wz4-13
			//input_op[0].Func();
	}
	//��5��������next�����ڵ�ǰҳ������ѯ
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
	//-----------------------------------------------IO���ý���Ƚ�����
		if( PageNum == 3 && AreaNum == 2 && PageAreaRec[3][2]%13!=0 ){
			if(PageAreaRec[3][2]+1 != PageAreaLen[3][2]){
				PageAreaRec[PageNum][AreaNum]++;				
				if( PageAreaRec[3][2]%13==0 ) IOSetRec++; //ÿ��IO������Ҫ13��ҳ��
				if(IOSetRec == 6) IOSetRec = 0;
				
				AreaChageFlag = 1;					
			}
			else if(PageAreaRec[3][2]+1 == PageAreaLen[3][2])
			{
				//��ǰare��������ok��Ҫ����	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[3][2] = 0;
				//IOSetRec = 0;
			}		
		}		//----------------------------------------------------------
		//�źű궨�����next��  ������һ��area
		if( PageNum == 3 && AreaNum == 4)
		{
			if(PageAreaRec[PageNum][AreaNum]+1 != PageAreaLen[PageNum][AreaNum]){
				PageAreaRec[PageNum][AreaNum]++;
				AreaChageFlag = 1;			
			}else if(PageAreaRec[PageNum][AreaNum]+1 == PageAreaLen[PageNum][AreaNum])
			{
				//Ҫ����	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[PageNum][AreaNum] = 0;	
			}			
		}
			
		else{		//��ͨ���������´���
			OptionChageFlage = 1;			
		}
	}

	//Area�仯����
	if(AreaChageFlag == 1){
		AreaChageFlag = 0;
		area_mode();	
	}
	//subarea�仯����
	if(SubAreaChangeFlag == 1)
	{
		SubAreaChangeFlag = 0;
		area_mode();
	}
	//Option�仯����
	if(OptionChageFlage == 1){
		if(OptionLen > 0){	
			if(OptionLoc+1 == OptionLen) {
				OptionChage(option[OptionLoc],option[0]);
			}else{
				OptionChage(option[OptionLoc],option[OptionLoc+1]);
			}
			OptionLoc++;
			if(OptionLoc == OptionLen) 
				OptionLoc = 0;
			OptionChageFlage = 0;
		}
	}
}

void InputKeyResponse(int Key_State)
{
	//������������ESC�� ���˻���һ��InputOp
	if(Key_State == KEY_2){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
			
		if(InputOpRec != 0){	
			InputOpLast = InputOpRec;
			InputOpRec -- ;
			InputOpChageFlag = 1;
		}else if(InputOpRec == 0){
			InputMode = 0;
			SubAreaMode = 0;   //wz
		} 
	}
	//���ĸ�������OK�������浱ǰ����   ����  ������һ��InputOp  ���ȫ��������ɣ����˳�Inputģʽ  
	if(Key_State == KEY_3){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		if( InputOpRec+1 < InputOpLen ){
			InputOpLast = InputOpRec;
			InputOpRec ++;
			InputOpChageFlag = 1;	
		}else{			 //ȫ����������ˣ��˳�Inputģʽ
			
			input_op[0].Func();	 
			InputMode = 0;
		}
	}
	//��5��������next������ǰInputOp��1
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE	
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
		input_op[InputOpRec].num++;
		if(input_op[InputOpRec].num == 10) input_op[InputOpRec].num = 0;
		InputNumChageFlag = 1;		
	}
	//InputNum�仯����
	if(InputNumChageFlag == 1){
		InputNumChageFlag = 0;
		InputNumChangedShow(input_op[InputOpRec]);	
	}
	//InputOp�仯����
	if(InputOpChageFlag == 1){
			
		InputOpChageFlag = 0;
		InputNumShow(input_op[InputOpLast]);
		InputNumChangedShow(input_op[InputOpRec]);
		//InuputOpChage(input_op[InputOpLast] , input_op[InputOpRec] );
	}	
}

//�͵�ģʽ�°�����Ӧ����
void LocalKeyResponse(int Key_State)
{
	//ά��ʽ
	if(ParaArray[LocallyMode] == hold_mode){
		//������������������
		if(Key_State == KEY_2){
			if( ValvePosValue > 1000 - ValveOpenError )
				SSRSTOP();
			else
				OpenValve();//SSR1RUN();		
		}
		//���ĸ�������ֹͣ
		if(Key_State == KEY_3){
			SSRSTOP();	
		}
		//������������ط���
		//bug  ���	ValveCloseError �� 30  ��3% ֮�ھͲ��ܹ��ˣ�

		if(Key_State == KEY_4){
			if( ValvePosValue > 0 )
				CloseValve();//SSR0RUN();			
		}							
	}
	//����ʽ��main��������
}

//Զ��ģʽ�°�����Ӧ����
void FarawayKeyResponse(int Key_State)
{
/*
	//ά��ʽ
	if(ParaArray[FarawayMode] == hold_mode){
		//������
		if(	IO1State() == LINK && IO0State() == NOLINK){
			if( ValvePosValue > 1000 - ValveOpenError )
				SSRSTOP();
			else
				OpenValve();//SSR1RUN();		
		}
		if( IO1State() == LINK && IO1State() == NOLINK){
			if( ValvePosValue > 0 )
				CloseValve();//SSR0RUN();			
		}							
	}
	//����ʽ��main��������			*/
}

