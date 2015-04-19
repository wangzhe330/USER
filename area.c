#include "page.h"
#include "area.h"
#include "oled.h"
#include "atom.h"
#include "para.h"
#include "current.h"

extern int auto_set_flag;
extern int AutoSM ;

//area参数初始化
//记录每个页面的每个area的长度
//二维数组的第一个维度表示page号，第二个维度表示area号，数组中存放的是area的长度
void AreaInit(void)
{
	PageAreaLen[2][0] = 6;//先做两个：开向限位、关向限位
	PageAreaLen[2][1] = 1;
	PageAreaLen[2][2] = 6;//实际是6个 先测试1个
	PageAreaLen[2][3] = 6;
	PageAreaLen[2][4] = 2;	
	PageAreaLen[3][0] = 6;
	PageAreaLen[3][1] = 4; //终端设置	 4个页面
	PageAreaLen[3][2] = 78; //io设置  每个iO13个页面 共6个IO设置  所以最大78 
	PageAreaLen[3][3] = 2;// 控制  一个area 还有subarea
	PageAreaLen[3][4] = 4;//信号
	PageAreaLen[3][5] = 4;//其他
	//PageAreaLen[3][6] = 4;

	PageAreaRec[2][0] = 0;//指向初始化为0		
}

//***********************************************************************************************************
//						
//											页面2  参数浏览
//
//***********************************************************************************************************


//页面2  参数浏览 area0 基本
void page2area0()
{
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数浏览	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//第二行第一列	基本参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 7;
			ch_label[1].code[1] = 8;
			ch_label[1].code[2] = 13;
			ch_label[1].code[3] = 14;
			ch_label[1].type = 'c';
			//第三行第一列 开向限位
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';	  		
			//第四行第一列  
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].type = 'c';

			if(ParaArray[OpenDirLimit] == journey){  //行程
				ch_label[3].code[0] = 38;
				ch_label[3].code[1] = 39;
			}else if(ParaArray[OpenDirLimit] == torque)	//  转矩
			{	  
				ch_label[3].code[0] = 40;
				ch_label[3].code[1] = 41;
			}
			else
			{
				//这里加入设置出错故障
			}			
			LabelLen = 4;
			LabelLenE = 0;
	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 关向限位
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';
		  		
			//第四行第一列  行程
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].type = 'c';
			if( ParaArray[CloseDirLimit] == journey){  //行程
				ch_label[3].code[0] = 38;
				ch_label[3].code[1] = 39;
			}else if( ParaArray[CloseDirLimit] == torque ) //  转矩
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 中间位置
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
			NumShow(24,48, ParaArray[MidPosition1] );		//这个显示可以挪到到showlabel后面，不然num会先出现
	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 3:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 开向设置转矩
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 关向设置转矩
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 紧急控制方式
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
		  		
			//第四行第一列 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].type = 'c';
			if( ParaArray[EmergencyMode] == hold){	 // 保位
				ch_label[3].length = 2;
				ch_label[3].code[0] = 66;
				ch_label[3].code[1] = 37;
			}else if(ParaArray[EmergencyMode] == assign ){		   //指定位置
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
			//第一行：参数浏览
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//第二行第一列	控制参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 15;
			ch_label[1].code[1] = 16;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//第三行第一列   就地
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 0;
			ch_label[2].code[1] = 1;
			ch_label[2].type = 'c';
			//第三行第二列   就地模式：脉冲式、维持式
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
		  		
			//第四行第一列   远程 
			ch_label[4].loc.x = 16;
			ch_label[4].loc.y = 48;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 2;
			ch_label[4].code[1] = 3;
			ch_label[4].type = 'c';
			//第四行第二列   远程模式：脉冲式、维持式、双位式、模拟量
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
	
		//这里的选择应该是进入sub area	
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

//I/O部分
void page2area2(void){
		
		//switch(PageAreaRec[PageNum][AreaNum]){
			//case 0:
				//第一行：参数浏览	 
				ch_label[0].loc.x = 0;
				ch_label[0].loc.y = 0;
				ch_label[0].length = 4;
				ch_label[0].code[0] = 7;
				ch_label[0].code[1] = 8;
				ch_label[0].code[2] = 11;
				ch_label[0].code[3] = 12;
				ch_label[0].type = 'c';
				//第二行第一列	I/O参数
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
				//第三行第一列 开关信号n
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
			  		
				//第四行第一列  常开\常闭
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
					//bug 缺少默认项 
				}				
				
				LabelLenE = 2;
				//第四行第二列  表示的含义
				ch_label[4].type = 'c';
				switch( ParaArray[OutExpState0 + PageAreaRec[PageNum][AreaNum]] )
				{
					case NoDefined:			//未定义
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;
						ch_label[4].length = 3;
						ch_label[4].code[0] = 109;
						ch_label[4].code[1] = 93;
						ch_label[4].code[2] = 110;
						break;					
					case TotalFault:		//总故障
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 3;
						ch_label[4].code[0] = 75;
						ch_label[4].code[1] = 17;
						ch_label[4].code[2] = 18;
						break;
					case FarawayFault:		//远方故障	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 74;
						ch_label[4].code[1] = 63;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;
					case LackPhaseFault:		//电源缺相   单相电源  不存在这个问题! !!!	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;
						ch_label[4].length = 4;
						ch_label[4].code[0] = 77;
						ch_label[4].code[1] = 79;
						ch_label[4].code[2] = 82;
						ch_label[4].code[3] = 83;
						break;
					case ValveLocationFault:		//阀位故障	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 76;
						ch_label[4].code[1] = 43;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;	
					case MotorOverHeartFault:		//电机过热	
						ch_label[4].loc.x = 24;
						ch_label[4].loc.y = 48;						
						ch_label[4].length = 4;
						ch_label[4].code[0] = 77;
						ch_label[4].code[1] = 78;
						ch_label[4].code[2] = 17;
						ch_label[4].code[3] = 18;
						break;
					case MidPosition1Fault:			//中间位置1故障
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
					case MidPosition2Fault:			//中间位置2故障
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
					case OpenDirJourneyLimit:  		//开向行程限位
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
					//bug 缺少默认项 									
				}
				LabelLen = 5;
				
				OptionLen = 0;
				OptionLoc = 0;
				//break;
			
	//	}
}

//参数浏览  故障部分
void page2area3(void){
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数浏览
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//第二行第一列	故障
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
			//第三行第一列   电机过热故障！
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
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
			
			NumShow(24,48,ParaArray[MotorOverHeartFaultCount]);
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//第一行 第二行同 case0

			//第三行第一列   阀位故障！
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 76;
			ch_label[2].code[1] = 43;
			ch_label[2].code[2] = 70;
			ch_label[2].code[3] = 71;
			ch_label[2].code[4] = 105;
			ch_label[2].type = 'c';
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
			
			NumShow(24,48, ParaArray[ValveLocationFaultCount]);
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;														
		case 2:
			//第一行 第二行同 case0
			//第三行第一列   开转矩故障！
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
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';
						
			NumShow(24,48, ParaArray[OpenDirJourneyLimitFaultCount]);
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 3:
			//第一行 第二行同 case0
			//第三行第一列   关转矩故障！
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
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[CloseDirJourneyLimitFaultCount]);
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 4:
			//第一行 第二行同 case0
			//第三行第一列   开行程故障！
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
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[OpenDirTorqueLimitFaultCount] );
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 5:
			//第一行 第二行同 case0
			//第三行第一列   关行程故障！
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
			//第四行 次
			ch_label[3].loc.x = 44;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 1 ;
			ch_label[3].code[0] = 106;
			ch_label[3].type = 'c';			
			NumShow(24,48, ParaArray[CloseDirTorqueLimitFaultCount]);
		//这里的选择应该是进入sub area	
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
			//第一行：参数浏览
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//第二行第一列	其他参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//第三行第一列   转矩AD值
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 40;
			ch_label[2].code[1] = 41;
			ch_label[2].type = 'c';
			//第三行第2列   AD
			en_label[0].loc.x = 24;
			en_label[0].loc.y = 32;
			en_label[0].length = 2;
			en_label[0].code[0] = 29;
			en_label[0].code[1] = 30;
			en_label[0].type = 'e';
			//第三行第3列   值
			ch_label[3].loc.x = 32;
			ch_label[3].loc.y = 32;
			ch_label[3].length = 1;
			ch_label[3].code[0] = 48;
			ch_label[3].type = 'c';
			//第四行 次
			
			NumShow(24,48, ParaArray[JourneyADCalue] );
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 1;	
			OptionLen = 0;
			OptionLoc = 0;
			break;
		case 1:
			//第一行 第二行同 case0
			//第三行第一列   出厂编码
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 99;
			ch_label[2].code[1] = 100;
			ch_label[2].code[2] = 107;
			ch_label[2].code[3] = 108;
			ch_label[2].type = 'c';
			
			NumShow(24,48, ParaArray[FactoryCode]);
		//这里的选择应该是进入sub area	
			LabelLen = 3;
			LabelLenE = 0;	
			OptionLen = 0;
			OptionLoc = 0;
			break;																																		
	}	
}
	
//***********************************************************************************************************
//						
//											页面3  参数设置
//
//***********************************************************************************************************
//页面3  参数设置 area0	  基本参数
void page3area0()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	基本参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//第三行第一列 开向限位
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 33;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';	  		
			//第四行第一列  行程
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 38;
			ch_label[3].code[1] = 39;
			ch_label[3].type = 'c';
			//第四行第二列  转矩
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 关向限位
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 34;
			ch_label[2].code[1] = 35;
			ch_label[2].code[2] = 36;
			ch_label[2].code[3] = 37;
			ch_label[2].type = 'c';
		  		
			//第四行第一列  行程
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 38;
			ch_label[3].code[1] = 39;
			ch_label[3].type = 'c';
			//第四行第二列  转矩
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
			InputMode = 0;		//关闭输入参数模式
			break;
		case 2:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 中间位置
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
	
	//这里应该用一个输入数字的东西！！！
			OptionLen = 0;
			OptionLoc = 0;
			
			//横坐标以4递增
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[MidPosition1]/100;
			input_op[0].Func = page3area0atom2inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[MidPosition1]%100/10;
			input_op[1].Func = func_null;
			//加了一个小数点  所以横坐标比上一个增加8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[MidPosition1]%10;
			input_op[2].Func = func_null;


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//打开输入参数模式

			break;
		case 3:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 开向设置转矩
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

			//输入参数
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[OpenDirJour]/100;
			input_op[0].Func = page3area0atom3inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[OpenDirJour]%100/10;
			input_op[1].Func = func_null;
			//加了一个小数点  所以横坐标比上一个增加8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[OpenDirJour]%10;
			input_op[2].Func = func_null;
			//输入参数用的.和%


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//打开输入参数模式

			break;
		case 4:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 关向设置转矩
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
//输入参数
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 48;
			input_op[0].num = ParaArray[CloseDirJour]/100;
			input_op[0].Func = page3area0atom4inop;
			input_op[1].loc.x = 20;
			input_op[1].loc.y = 48;
			input_op[1].num = ParaArray[CloseDirJour]%100/10;
			input_op[1].Func = func_null;
			//加了一个小数点  所以横坐标比上一个增加8
			input_op[2].loc.x = 28;
			input_op[2].loc.y = 48;
			input_op[2].num = ParaArray[CloseDirJour]%10;
			input_op[2].Func = func_null;


			InputOpLen = 3;
			InputOpRec = 0;
			InputMode = 1;		//打开输入参数模式

			break;
		case 5:
			//第一行 与 第二行 同 case 0 
			//第三行第一列 紧急控制方式
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
		  		
			//第四行第一列  保位
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 66;
			ch_label[3].code[1] = 37;
			ch_label[3].type = 'c';
			//第四行第二列  指定位置
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
			InputMode = 0;		//打开输入参数模式
			break;											
		}	
}

//页面3 终端设置  area6 终端  实际位置在 第三行 第一列
void page3area1()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	终端位置
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].type = 'c';
			//第三行第一列 请关到0%
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
			//第四行第一列 按 确认
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
			//第一行 与 
			//第二行 终端位置已修改 
			//按退出取消 
			//按确认保存
			//第二行第一列	终端位置已修改
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
			//第三行 按退出取消 
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 119;
			ch_label[2].code[2] = 120;
			ch_label[2].code[3] = 144;
			ch_label[2].code[4] = 145;
			ch_label[2].type = 'c';
			//第四行 按确认保存
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
			//第一行 同 case 0 
			//第二行第一列	终端位置
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 134;
			ch_label[1].code[1] = 135;
			ch_label[1].code[2] = 127;
			ch_label[1].code[3] = 128;
			ch_label[1].type = 'c';
			//第三行第一列 请开到100%
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
			//第四行第一列 按 确认
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
						//第一行 与 
			//第二行 终端位置已修改 
			//按退出取消 
			//按确认保存
			//第二行第一列	终端位置已修改
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
			//第三行 按退出取消 
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 119;
			ch_label[2].code[2] = 120;
			ch_label[2].code[3] = 144;
			ch_label[2].code[4] = 145;
			ch_label[2].type = 'c';
			//第四行 按确认保存
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

//页面3 参数设置  area2	I/O设置
void page3area2()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;		
	switch(PageAreaRec[PageNum][AreaNum]%13){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	I/O参数
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
			//第三行第一列 开关信号IOSetRec
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
		  		
			//第四行第一列  常开
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 67;
			ch_label[3].code[1] = 68;
			ch_label[3].type = 'c';
			//第四行第二列  常闭
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
			//第一行 与 第二行 同 case 0 
			//第三行第二列 	 表示
			ch_label[3].loc.x = 48;
			ch_label[3].loc.y = 32;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 80;
			ch_label[3].code[1] = 81;
			ch_label[3].type = 'c';
		  		
			//第四行第一列  总故障
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  远方故障
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  电源缺相
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  阀位故障
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  电机过热
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  中间位置1
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  中间位置2
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  开向行程限位
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  关向行程限位
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  开向转矩限位
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  开向转矩限位
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列 设定值故障
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



//页面3  参数设置 area1 控制参数
void page3area3()
{
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	控制参数 就地参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 15;
			ch_label[1].code[1] = 16;
			ch_label[1].code[2] = 7;
			ch_label[1].code[3] = 8;
			ch_label[1].type = 'c';
			//第三行第一列   就地
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 0;
			ch_label[2].code[1] = 1;
			ch_label[2].type = 'c';
		  		
			//第四行第一列   远程 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 2;
			ch_label[3].code[1] = 3;
			ch_label[3].type = 'c';
	
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//关闭输入参数模式
	
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
	SubAreaLen = 1;		//subarea只有1个页面
	SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	控就地制参数
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
			//第三行第一列   脉冲式
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 59;
			ch_label[2].code[1] = 60;
			ch_label[2].code[2] = 6;
			ch_label[2].type = 'c';
		  		
			//第四行第一列   维持式 
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 86;
			ch_label[3].code[1] = 87;
			ch_label[3].code[2] = 6;
			ch_label[3].type = 'c';
	
		//这里的选择应该是进入sub area	
			LabelLen = 4;
			LabelLenE = 0;
			InputOpLen = 0;
			InputOpRec = 0;
			InputMode = 0;		//关闭输入参数模式
	
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
	SubAreaLen = 1;		//subarea只有1个页面
	SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	远程控制参数
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
			//第三行第一列   脉冲式
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 3;
			ch_label[2].code[0] = 59;
			ch_label[2].code[1] = 60;
			ch_label[2].code[2] = 6;
			ch_label[2].type = 'c';
		  		
			//第四行第一列   维持式 
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 86;
			ch_label[3].code[1] = 87;
			ch_label[3].code[2] = 6;
			ch_label[3].type = 'c';
			//第三行第2列   双位式
			//wz 不要了
			ch_label[5].loc.x = 40;
			ch_label[5].loc.y = 48;
			ch_label[5].length = 3;
			ch_label[5].code[0] = 91;
			ch_label[5].code[1] = 37;
			ch_label[5].code[2] = 6;
			ch_label[5].type = 'c';
		  		
			//第四行第2列   模拟式 
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
			InputMode = 0;		//关闭输入参数模式
	
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


//页面3 参数设置  area4  信号
void page3area4(){
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	信号标定
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 29;
			ch_label[1].code[1] = 30;
			ch_label[1].code[2] = 31;
			ch_label[1].code[3] = 32;
			ch_label[1].type = 'c';
			//第三行第一列 反馈值 4mA
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 反馈值 20mA
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
			//第一行 与 第二行 第三行 同 case 0 
			//第三行第一列 设定值 4mA
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
			//第一行 与 第二行 第三行 同 case 0 
			//第四行第一列  信号标定
			//第三行第一列 设定值 20mA
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
			//第一行 与 第二行 第三行 同 case 0 
			//第二行第一列  信号标定
			//第三行第一列  将当前位置
			//第四行第一列  设定为
			//第四行第二列 0%
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
			//第一行 与 第二行 第三行 同 case 0 
			//第二行第一列  信号标定
			//第三行第一列  将当前位置
			//第四行第一列  设定为
			//第四行第二列 0%
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

//其他
void page3area5()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	
	switch(PageAreaRec[PageNum][AreaNum]){
		case 0:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	其他参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 9;
			ch_label[1].code[3] = 10;
			ch_label[1].type = 'c';
			//第三行第一列 恢复出厂设置
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
			//第四行第一列  不恢复
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 97;
			ch_label[3].code[2] = 98;
			ch_label[3].type = 'c';
			//第四行第二列  恢复
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 报警清除
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 103;
			ch_label[2].code[1] = 104;
			ch_label[2].code[2] = 101;
			ch_label[2].code[3] = 102;
			ch_label[2].type = 'c';
		  		
			//第四行第一列  不清除
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 101;
			ch_label[3].code[2] = 102;
			ch_label[3].type = 'c';
			//第四行第二列  清除
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
			//第一行 与 第二行 同 case 0 
			//第三行第一列 修改密码
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			ch_label[2].code[0] = 141;
			ch_label[2].code[1] = 142;
			ch_label[2].code[2] = 157;
			ch_label[2].code[3] = 158;
			ch_label[2].type = 'c';
		  		
			//第四行第一列  不修改
			ch_label[3].loc.x = 8;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 141;
			ch_label[3].code[2] = 142;
			ch_label[3].type = 'c';
			//第四行第二列  修改
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
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;
			ch_label[0].type = 'c';
			//第二行第一列	其他参数
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 94;
			ch_label[1].code[1] = 95;
			ch_label[1].code[2] = 9;
			ch_label[1].code[3] = 10;
			ch_label[1].type = 'c';
			//第三行第一列 按确认进入
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 5;
			ch_label[2].code[0] = 137;
			ch_label[2].code[1] = 138;
			ch_label[2].code[2] = 139;
			ch_label[2].code[3] = 146;
			ch_label[2].code[4] = 147;
			ch_label[2].type = 'c';	  		
			//第四行第一列  参数自学习模式
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

//输入旧密码
//并且验证密码是否正确
void page3area5atom2in0()
{
	int temp = 0 ;
	temp = 	input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	//密码正确
	if ( temp == ParaArray[Security] )
	{
		SubAreaRec = 1;			//跳转到了子页面1，设置新密码
		SubAreaChangeFlag = 1;		
	}else{						//密码不正确，刷新当前页面
		SubAreaRec = 0;
		SubAreaChangeFlag = 1;	
	}		
}
//输入新密码
void page3area5atom2in1()
{
	NewSecurity = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	SubAreaRec = 2;			//跳转到了子页面2，验证新密码
	SubAreaChangeFlag = 1;		
}
//再次输入新密码
void page3area5atom2in2()
{
	NewSecurityAgain = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	if( NewSecurityAgain == NewSecurity){
		ParaArray[Security] = NewSecurity;
		SubAreaRec = 3;			//跳转到了子页面3，密码设置成功
		SubAreaChangeFlag = 1;			
	}else{
		SubAreaRec = 4;			//跳转到了子页面4，密码设置失败
		SubAreaChangeFlag = 1;		
	}
}



//密码设置界面
void page3area5sub1()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;
		
 	SubAreaLen = 6;		//subarea有4个页面
	//SubAreaRec = 0;
	switch(SubAreaRec){
		case 0:	
		
			//第二行第一列  请输入旧密码
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
		
			//横坐标以4递增
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
			InputMode = 1;		//打开输入参数模式	
		break;
		case 1:
			//第二行第一列  请设置新密码
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
		
			//横坐标以4递增
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
			InputMode = 1;		//打开输入参数模式	
		break;
		case 2:
			//第二行第一列  请再次输入
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
		
			//横坐标以4递增
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
			InputMode = 1;		//打开输入参数模式	
		break;
		case 3:
			//第二行第一列  密码修改成功
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
			//第二行第一列  两次输入不一致
			//密码修改失败
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

//自整定界面
void page3area5sub0()
{
	//先清零
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;	

	//第一行：参数设置
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
	//第二行第一列  参数自整定
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
	//第三行第一列 请稍等
	
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
	option[0].Func = page3area5atom0op0;  //退出


	OptionLen = 1;
	OptionLoc = 0;

	auto_set_flag = 1;
	AutoSM = 0;				
}


//自整定完成
void auto_set_exit(void)
{		
	//第四行第一列  完成！
	ch_label[0].loc.x = 8;
	ch_label[0].loc.y = 48;
	ch_label[0].length = 3;
	ch_label[0].code[0] = 116;
	ch_label[0].code[1] = 117;
	ch_label[0].code[2] = 118;
	ch_label[0].type = 'c';	

	MenuLabelShow(ch_label[0]);				
}

//****************************************************area模式**************************************************
void area_mode(void)
{
	int cnt_label = 0;
//	int cnt_option = 0;
//	int cnt_inop = 0;
	//area_clear();
	Fill_RAM(0x00);
	//Fill_Block(0,40,0,32,0x00);  //参数：起点列号，重点列号，起点行号，终点行号，填充灰度值
	//page2 参数浏览
	if(PageNum == 2){
		switch(AreaNum){
			case 0:	
				page2area0();//基本参数浏览
				break;				
			case 1:
				page2area1();//控制参数浏览	
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
	//page3 参数设置
	if(PageNum == 3){
		switch(AreaNum){
			case 0:	
				page3area0();	//基本参数设置
				break;
			case 1:
				page3area1();	//终端参数设置
				break;
			case 2:
				page3area2(); 	//IO参数设置
				break;		
			case 3:	   			//控制参数设置，这里会进入subarea
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
							page3area5sub0();			//自学习模式入口
						break;					
						case 1:
							page3area5sub1();			//密码设置入口
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
	//显示当前页面的静态部分
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}
	//显示当前页面的第一个op
	if(OptionLen > 0)
		OptionShow(option[0]);
	if(InputOpLen > 0){
		InputNumAllShow(input_op , InputOpLen);
		InputNumChangedShow( input_op[0] );
	}			
}

