#include "inout.h"
#include "oled.h"

int InNumFlag = 0;
int StateMachine = 0;

//�βα�ʾ���뼸λ
void IuputNum(int len)
{
	int Key_State = 0;	
/*
	while(InNumFlag)
	{
		Key_State = myKey_Scan();
		//������������ESC�� ���˻���һ��Area
		if(Key_State == KEY_2){
		   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
			
			if(PageAreaRec[PageNum][AreaNum]!=0)
			{	
				PageAreaRec[PageNum][AreaNum]--;
				AreaChageFlag = 1;
			} 
			else
			{
				page_mode();
				AreaMode = 0;  //�˳�Areaģʽ					
			}
		}
		//���ĸ�������OK�������浱ǰ����   ����  ������һ��Area   
		if(Key_State == KEY_3){
		   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
			//option[OptionLoc].Func();
			//PageChageFlage = 1;
			if(PageAreaRec[PageNum][AreaNum]+1 != PageAreaLen[PageNum][AreaNum]){
				PageAreaRec[PageNum][AreaNum]++;
				AreaChageFlag = 1;	
			}else{
				//�������ʲô�������Ļ�������Ҫһ��һ�����˳����area�������REC����Ļ�������area��ѭ��
				//PageAreaRec[PageNum][AreaNum] = 0;
			}
			
		}
		//��5��������next�����ڵ�ǰҳ������ѯ
		if(Key_State == KEY_4){
		   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
			OptionChageFlage = 1;			
		}
	}
	*/
}

