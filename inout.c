#include "inout.h"
#include "oled.h"

int InNumFlag = 0;
int StateMachine = 0;

//形参表示输入几位
void IuputNum(int len)
{
	int Key_State = 0;	
/*
	while(InNumFlag)
	{
		Key_State = myKey_Scan();
		//第三个按键、ESC键 ，退回上一个Area
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
				AreaMode = 0;  //退出Area模式					
			}
		}
		//第四个按键、OK键，保存当前设置   并且  进入下一个Area   
		if(Key_State == KEY_3){
		   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
			//option[OptionLoc].Func();
			//PageChageFlage = 1;
			if(PageAreaRec[PageNum][AreaNum]+1 != PageAreaLen[PageNum][AreaNum]){
				PageAreaRec[PageNum][AreaNum]++;
				AreaChageFlag = 1;	
			}else{
				//如果这里什么都不做的话，就是要一步一步的退出这个area，如果对REC清零的话，会在area内循环
				//PageAreaRec[PageNum][AreaNum] = 0;
			}
			
		}
		//第5个按键，next键，在当前页面内轮询
		if(Key_State == KEY_4){
		   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
		             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
			OptionChageFlage = 1;			
		}
	}
	*/
}

