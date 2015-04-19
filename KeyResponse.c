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

//全局变量
extern Menu_Option option[6];
extern Menu_Label ch_label[8];
extern Menu_InputOp input_op[5];		//用作输入参数
extern M_Page Page[PageSum];

extern int ValveOpenError,ValveCloseError;


//本文件内为各种不同模式下的按键响应函数
void PageKeyResponse(int Key_State )
{
	//第三个按键、ESC键 ，退回上上一个页面
	if(Key_State == KEY_2){
#ifdef DEBUG_MODE	
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		if(PageNum == 2 || PageNum == 6) //从参数浏览界面、密码输入界面可以退回选择界面
			PageNum = Page[PageNum].parent;
		else if(PageNum == 3) //从参数设置界面退出，需要判断是否保存
		{
			if(!BufferCmp(ParaArray,ParaArray_copy,ParaArrayLength))
				PageNum = 5;
			else
				PageNum = 4;
		}
		
		//-----------------------------WZ
		//保存设置
		//这里加入判断参数是否改变的函数，询问是否要保存的页面。
		
		//option[OptionLoc].Func();
		PageChageFlage = 1;			
	}
	//第四个按键、OK键，执行选项的函数
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
	//第5个按键，next键，在当前页面内轮询
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
		if(OptionLen > 0)
			OptionChageFlage = 1;			
	}
	//页面变化处理
	if(PageChageFlage == 1){
		//PageNum = 1 - PageNum;
		PageChageFlage = 0;
		Fill_RAM(0x00);			// Clear Screen
		
		/*
		if(ReturnFromAreaFlag == 0)	{
			OptionLoc = 0; 		//选项归0位置
		}
		if(ReturnFromAreaFlag == 1)
		{
			OptionLoc = AreaNum;
			ReturnFromAreaFlag = 0;
		} */
		page_mode();	
	}
	//选项变化处理
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

	//第三个按键、ESC键 ，退回上一个Area  或者是退出 subarea
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
			AreaMode = 0;  //退出Area模式
			ReturnFromAreaFlag = 1;			//	
			page_mode();	
		}
		else if( SubAreaMode == 1){
			if(SubAreaRec == 0){ 	//退出SubArea模式
				SubAreaMode = 0;
				AreaChageFlag = 1;
			}else{
				SubAreaRec--;
			}
		}
	}
	//第四个按键、OK键，保存当前设置   并且  进入下一个Area   或者进入subarea   
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
					IOSetRec++; //每个IO设置需要13个页面
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
				//当前are所有设置ok后要重置	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[PageNum][AreaNum] = 0;
				//page_esc();
				//PageAreaRec[PageNum][AreaNum]--;
				//AreaChageFlag = 1;
				//SubAreaMode = 0;
				//AreaChageFlag = 1;	
			//如果这里什么都不做的话，就是要一步一步的退出这个area，如果对REC清零的话，会在area内循环
			//PageAreaRec[PageNum][AreaNum] = 0;
			}
		}

		if(SubAreaMode == 1){
			if(SubAreaRec+1 == SubAreaLen){		//subare设置完了 退出设置
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
			//密码设置 在sub3 或者 sub4	都是退出
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
		//wz 考虑是否去掉这里的	 || InputOpLen!=0
		//if( PageNum != 2 )
		
		if(OptionLen != 0 )  
			option[OptionLoc].Func();
		//if(InputOpLen!=0 )	   //wz4-13
			//input_op[0].Func();
	}
	//第5个按键，next键，在当前页面内轮询
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
	//-----------------------------------------------IO设置界面比较特殊
		if( PageNum == 3 && AreaNum == 2 && PageAreaRec[3][2]%13!=0 ){
			if(PageAreaRec[3][2]+1 != PageAreaLen[3][2]){
				PageAreaRec[PageNum][AreaNum]++;				
				if( PageAreaRec[3][2]%13==0 ) IOSetRec++; //每个IO设置需要13个页面
				if(IOSetRec == 6) IOSetRec = 0;
				
				AreaChageFlag = 1;					
			}
			else if(PageAreaRec[3][2]+1 == PageAreaLen[3][2])
			{
				//当前are所有设置ok后要重置	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[3][2] = 0;
				//IOSetRec = 0;
			}		
		}		//----------------------------------------------------------
		//信号标定界面的next键  跳到下一个area
		if( PageNum == 3 && AreaNum == 4)
		{
			if(PageAreaRec[PageNum][AreaNum]+1 != PageAreaLen[PageNum][AreaNum]){
				PageAreaRec[PageNum][AreaNum]++;
				AreaChageFlag = 1;			
			}else if(PageAreaRec[PageNum][AreaNum]+1 == PageAreaLen[PageNum][AreaNum])
			{
				//要重置	PageAreaRec[PageNum][AreaNum]
				AreaMode = 0;
				PageChageFlage = 1;	
				ReturnFromAreaFlag = 1;
				PageAreaRec[PageNum][AreaNum] = 0;	
			}			
		}
			
		else{		//普通的如下如下处理
			OptionChageFlage = 1;			
		}
	}

	//Area变化处理
	if(AreaChageFlag == 1){
		AreaChageFlag = 0;
		area_mode();	
	}
	//subarea变化处理
	if(SubAreaChangeFlag == 1)
	{
		SubAreaChangeFlag = 0;
		area_mode();
	}
	//Option变化处理
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
	//第三个按键、ESC键 ，退回上一个InputOp
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
	//第四个按键、OK键，保存当前设置   并且  进入下一个InputOp  如果全部输入完成，则退出Input模式  
	if(Key_State == KEY_3){
#ifdef DEBUG_MODE
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_15, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_15))));
#endif
		if( InputOpRec+1 < InputOpLen ){
			InputOpLast = InputOpRec;
			InputOpRec ++;
			InputOpChageFlag = 1;	
		}else{			 //全部设置完成了，退出Input模式
			
			input_op[0].Func();	 
			InputMode = 0;
		}
	}
	//第5个按键，next键，当前InputOp增1
	if(Key_State == KEY_4){
#ifdef DEBUG_MODE	
	   	GPIO_WriteBit(GPIOE, GPIO_Pin_14, 
	             (BitAction)((1-GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_14))));
#endif
		input_op[InputOpRec].num++;
		if(input_op[InputOpRec].num == 10) input_op[InputOpRec].num = 0;
		InputNumChageFlag = 1;		
	}
	//InputNum变化处理
	if(InputNumChageFlag == 1){
		InputNumChageFlag = 0;
		InputNumChangedShow(input_op[InputOpRec]);	
	}
	//InputOp变化处理
	if(InputOpChageFlag == 1){
			
		InputOpChageFlag = 0;
		InputNumShow(input_op[InputOpLast]);
		InputNumChangedShow(input_op[InputOpRec]);
		//InuputOpChage(input_op[InputOpLast] , input_op[InputOpRec] );
	}	
}

//就地模式下按键响应函数
void LocalKeyResponse(int Key_State)
{
	//维持式
	if(ParaArray[LocallyMode] == hold_mode){
		//第三个按键、开阀门
		if(Key_State == KEY_2){
			if( ValvePosValue > 1000 - ValveOpenError )
				SSRSTOP();
			else
				OpenValve();//SSR1RUN();		
		}
		//第四个按键、停止
		if(Key_State == KEY_3){
			SSRSTOP();	
		}
		//第五个按键、关阀门
		//bug  如果	ValveCloseError 》 30  那3% 之内就不能关了？

		if(Key_State == KEY_4){
			if( ValvePosValue > 0 )
				CloseValve();//SSR0RUN();			
		}							
	}
	//脉冲式在main中做处理
}

//远程模式下按键响应函数
void FarawayKeyResponse(int Key_State)
{
/*
	//维持式
	if(ParaArray[FarawayMode] == hold_mode){
		//开阀门
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
	//脉冲式在main中做处理			*/
}

