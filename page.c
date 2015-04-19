#include "page.h"
#include "oled.h"
#include "stdlib.h"
#include "area.h"
#include "para.h"
#include "spi_flash.h"
#include "stdio.h"

//菜单的结构
// menu:page->area->atom
//				  ->sub_area->atom

Menu_Option option[6];
Menu_Label ch_label[8];
Menu_Label en_label[8];
int LastPageNum = 1;  		//保存上一个页面号
int PageNum = 1; 			//第0页面不要了，所以从1开始
int PageChageFlage = 0;  	//0 not chaged,  1 changed
int LabelLen = 0;
int LabelLenE = 0;
int OptionLen = 0;
int OptionChageFlage = 0;  		//0 not chaged,  1 changed
int OptionLoc = 0;

int ReturnFromAreaFlag = 0;

Menu_InputOp input_op[5];		//用作输入数字
int InputOpLen = 0;
int InputOpRec = 0;
int InputOpLast = 0;			//记录上一次inputop位置
int InputOpChageFlag = 0;	 	//0 not chaned,1 changed
int InputNumChageFlag = 0;	 	//0 not chaned,1 changed
int InputMode = 0;				//0表示不再input模式，1表示在


//每个Area是一串局部atom显示的变化											 
int AreaChageFlag = 0;  //0 not chaged,  1 changed
int AreaMode = 0; 		//0 表示不在area模式，1表示在area模式
int AreaNum;			//Area号
int PageAreaRec[4][5];	//存5个Area内部的指向	第一个维度是页面号（用PageNum）  第二个维度是Area号（即用AreaNum）
int PageAreaLen[4][5];	//分别寸5个Area的长度	 第一个维度用4，是因为页面2和3才有Area，是为了和PageNum兼容

int SubAreaMode = 0;
int SubAreaLen = 0;
int SubAreaRec = 0;
int SubAreaChangeFlag = 0;
int SubAreaNum = 0;

//IO设置时候的计数  6个
int	IOSetRec = 0;
#define IOSetLen 6 

int BigMode = 0;		//0,1,2 表示 设置、就地、远程

//每个页面的上下语境是确定的，所以不需要做复杂的切换程序，只需要根据当的PageNum就可以知道页面中哪些部分是需要更新的
//每个页面公用一套公共的存储空间可以省空间  MenuLabel
//MenuOption 每个页面需要自己单独的存储空间， 用数组和Option个数做一个struct

M_Page Page[PageSum];


//这里初始化了page之间的父子关系
void PageInit(void)
{
	int i = 0;
	for(i = 0 ; i < PageSum ; i ++)
	{
		Page[i].num = i;
	}
	Page[0].child = 1;
	Page[0].parent = 0;
	Page[1].child = 2;
	Page[1].parent = 1;		   //这里把page1的父page指向了自己，去掉了page0
	Page[2].child = 2;
	Page[2].parent = 1;
	Page[3].child = 3;
	Page[3].parent = 1;
	Page[6].parent = 1;
}

void MenuLabelShow(Menu_Label l)
{
	int cnt = 0 ; 
	for(cnt = 0 ; cnt < l.length ; cnt++)
	{
		if(l.type == 'c')
			hz_1616(l.loc.x+cnt*8,l.loc.y,chinese[l.code[cnt]],0);	
		else if(l.type == 'e')
			ascii_1608(l.loc.x+cnt*4,l.loc.y,english[l.code[cnt]],0);
	}	
}

void OptionShow(Menu_Option option)
{
	hz_1616( option.loc.x , option.loc.y , op ,0);		
}

void OptionChage(Menu_Option old_option , Menu_Option new_option ) //这是一个只能一个方向移动选择三角的程序	
{
	hz_1616_clear( old_option.loc.x , old_option.loc.y );	//清除旧的选择项
	hz_1616( new_option.loc.x , new_option.loc.y , op ,0);	//新建新的选择项		
}

//输入一个参数用的反显功能
void InputNumAllShow(Menu_InputOp in_op[] , int len)
{
	int cnt = 0;
	if(PageNum == 6 || (PageNum==3&&AreaNum == 5) ){
		for(cnt = 0 ; cnt < len ; cnt++){
			if( in_op[cnt].num >= 0 && in_op[cnt].num <= 9)
				ascii_1608( in_op[cnt].loc.x , in_op[cnt].loc.y , english[42] ,0);			
		}
	}else{
		for(cnt = 0 ; cnt < len ; cnt++){
			if( in_op[cnt].num >= 0 && in_op[cnt].num <= 9)
				ascii_1608( in_op[cnt].loc.x , in_op[cnt].loc.y , english[in_op[cnt].num+18] ,0);
		}
	}
	if(PageNum==3 && AreaNum==4){				//信号标定
		ascii_1608( 40 , 48 , english[31] ,0);	//小数点	
	}
	else if(PageNum != 6 && (!(PageNum==3&&AreaNum == 5))) {
		ascii_1608( 24 , 48 , english[31] ,0);	//小数点	
		ascii_1608( 32 , 48 , english[32] ,0);	//百分号
	}
}
void InputNumChangedShow(Menu_InputOp in_op)
{
	ascii_1608( in_op.loc.x , in_op.loc.y , english[in_op.num+18] ,1);
	if(PageNum==3&&AreaNum==4){				//信号标定
		ascii_1608( 40 , 48 , english[31] ,0);	//小数点	
	}
	else if(PageNum != 6 && !(PageNum==3&&AreaNum == 5)){
		ascii_1608( 24 , 48 , english[31] ,0);	//小数点	
		ascii_1608( 32 , 48 , english[32] ,0);	//百分号
	}				
}
void InputNumShow(Menu_InputOp in_op)
{
	ascii_1608( in_op.loc.x , in_op.loc.y , english[in_op.num+18] ,0);
	if(PageNum==3&&AreaNum==4){				//信号标定
		ascii_1608( 40 , 48 , english[31] ,0);	//小数点	
	}
	else if(PageNum != 6 && !(PageNum==3&&AreaNum == 5)){
		ascii_1608( 24 , 48 , english[31] ,0);	//小数点	
		ascii_1608( 32 , 48 , english[32] ,0);	//百分号
	}
}

//数字显示函数  固定5位
void NumShow(int x,int y,int num)
{
	int temp = num;
	ascii_1608(x , y, english[temp/10000+18],0);	
	temp = num%10000;
	ascii_1608(x+4 , y, english[temp/1000+18],0);
	temp = num%1000;
	ascii_1608(x+8, y, english[temp/100+18],0);
	temp = num%100;
	ascii_1608(x+12 , y, english[temp/10+18],0);
	temp = num%10;
	ascii_1608(x+16 , y, english[temp+18],0);			
}

//就地模式  只有一个界面，不需要跳转，所以单独拎出来写
//就地模式的界面里面应该还有一些报警信息
void local_mode(void )
{
	int cnt_label = 0;
	Fill_RAM(0x00);

	//第一个label    就地
	ch_label[0].loc.x = 0;
	ch_label[0].loc.y = 0;
	ch_label[0].code[0] = 0;
	ch_label[0].code[1] = 1;
	ch_label[0].length = 2;
	ch_label[0].type = 'c';	
	//维持式\脉冲式	 (功能调整 pluse_mode 和 hold_mode换过一次位置)
	if(ParaArray[LocallyMode] == pluse_mode){	
		ch_label[1].loc.x = 0;
		ch_label[1].loc.y = 48;
		ch_label[1].code[0] = 86;
		ch_label[1].code[1] = 87;
		ch_label[1].code[2] = 6;
		ch_label[1].length = 3;
		ch_label[1].type = 'c';
	}
	else if (ParaArray[LocallyMode] == hold_mode){
		ch_label[1].loc.x = 0;						
		ch_label[1].loc.y = 48;
		ch_label[1].code[0] = 4;
		ch_label[1].code[1] = 5;
		ch_label[1].code[2] = 6;
		ch_label[1].length = 3;
		ch_label[1].type = 'c';		
	}

	LabelLen = 2;
	LabelLenE = 0;	
	OptionLen = 0;

	//显示当前页面的静态部分
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}

	
}

//远程控制模式
void faraway_mode(void)
{
	int cnt_label = 0;
	Fill_RAM(0x00);

	//第一个label    远程
	ch_label[0].loc.x = 0;
	ch_label[0].loc.y = 0;
	ch_label[0].code[0] = 2;
	ch_label[0].code[1] = 3;
	ch_label[0].length = 2;
	ch_label[0].type = 'c';

	if( ParaArray[FarawayMode] == pluse_mode )
	{
		//脉冲式
		ch_label[1].code[0] = 4;
		ch_label[1].code[1] = 5;
		ch_label[1].code[2] = 6; 
	}
	if( ParaArray[FarawayMode] == hold_mode )
	{
		//维持
		ch_label[1].code[0] = 86;
		ch_label[1].code[1] = 87;
		ch_label[1].code[2] = 6; 
	} 
	if( ParaArray[FarawayMode] == double_mode )
	{
		//双位式
		ch_label[1].code[0] = 91;
		ch_label[1].code[1] = 127;
		ch_label[1].code[2] = 6; 
	} 
	if( ParaArray[FarawayMode] == analog_mode )
	{
		//模拟式
		ch_label[1].code[0] = 88;
		ch_label[1].code[1] = 89;
		ch_label[1].code[2] = 6; 
	} 			 
	ch_label[1].loc.x = 0;
	ch_label[1].loc.y = 48;
	ch_label[1].length = 3;
	ch_label[1].type = 'c';
	if(ParaArray[FarawayMode] == analog_mode)
	{
		//设定值
		ch_label[2].loc.x = 16;
		ch_label[2].loc.y = 16;
		ch_label[2].code[0] = 9;
		ch_label[2].code[1] = 32;
		ch_label[2].code[2] = 48;
		ch_label[2].length = 3;
		ch_label[2].type = 'c';
	
		LabelLen = 3;
	}else {
		LabelLen = 2;
	}
	LabelLenE = 0;	
	OptionLen = 0;

	//显示当前页面的静态部分
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}
			
}

//****************************************************page模式**************************************************
void page_mode(void)
{
	//Option_List PageOptionList;
	int cnt_label = 0;
	
	Fill_RAM(0x00);
	//如果改用下面的局部清屏的方法，需要在case里面确定每次清除的范围，速度上面可能会快一点，少显示了一部分字
	//Fill_Block(0,40,0,32,0x00);  //参数：起点列号，重点列号，起点行号，终点行号，填充灰度值

	//标志位清零
	LabelLen = 0;
	LabelLenE = 0;				
	OptionLen = 0;
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;		
	//根据页面号确定一些每个页面公共的
	switch (PageNum)
	{
		case 0:
			//第一个label    语言选择 中英文
			//语言选择 第一行
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].code[0] = 21;
			ch_label[0].code[1] = 22;
			ch_label[0].code[2] = 23;
			ch_label[0].code[3] = 24;
			ch_label[0].length = 4;
			ch_label[0].type = 'c';
			//Language 第二行
			en_label[0].loc.x = 0;
			en_label[0].loc.y = 16;
			en_label[0].code[0] = 0;
			en_label[0].code[1] = 1;
			en_label[0].code[2] = 2;
			en_label[0].code[3] = 3;
			en_label[0].code[4] = 4;
			en_label[0].code[5] = 5;
			en_label[0].code[6] = 6;
			en_label[0].code[7] = 7;
			en_label[0].length = 8;
			en_label[0].type = 'e';
			//中文  第三行	
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y =32;
			ch_label[1].code[0] = 27;
			ch_label[1].code[1] = 28;
			ch_label[1].length = 2;
			ch_label[1].type = 'c';
			//english 第四行
			en_label[1].loc.x = 8;
			en_label[1].loc.y = 48;
			en_label[1].code[0] = 8;
			en_label[1].code[1] = 9;
			en_label[1].code[2] = 10;
			en_label[1].code[3] = 11;
			en_label[1].code[4] = 12;
			en_label[1].code[5] = 13;
			en_label[1].code[6] = 14;
			en_label[1].length = 7;
			en_label[1].type = 'e';

			LabelLen = 2;
			LabelLenE = 2;

			option[0].loc.x = 0;
			option[0].loc.y = 32;
			option[0].Func = page0op0;

			OptionLen = 1;

			break;

		case 1:
			//page1   参数浏览\参数设置
			//第一行 组态
			//第二行 参数
			//第三行 设置
			//第四行 浏览
			//第一行：组态	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 2;
			ch_label[0].code[0] = 132;
			ch_label[0].code[1] = 133;
			ch_label[0].type = 'c';
			//第二行：参数 
			ch_label[1].loc.x = 0;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 7;
			ch_label[1].code[1] = 8;
			ch_label[1].type = 'c';
			//第三行 设置
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 9;
			ch_label[2].code[1] = 10;
			ch_label[2].type = 'c';
			//第四行 浏览
			ch_label[3].loc.x = 24;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 11;
			ch_label[3].code[1] = 12;
			ch_label[3].type = 'c';
		
			LabelLen = 4;
			LabelLenE = 0;

			option[0].loc.x = 16;
			option[0].loc.y = 32;
			option[0].Func = page1op0;
			option[1].loc.x = 16;
			option[1].loc.y = 48;
			option[1].Func = page1op1;
				
			OptionLen = 2;

			break;

		//page2参数浏览 页面
		case 2:
			//第一行：参数浏览	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 11;
			ch_label[0].code[3] = 12;
			ch_label[0].type = 'c';
			//第二行第一列	基本
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].type = 'c';
			//第三行第一列	 控制
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 15;
			ch_label[2].code[1] = 16;
			ch_label[2].type = 'c';
		  	//第二行第二列	 故障
			ch_label[3].loc.x = 32;
			ch_label[3].loc.y = 16;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 17;
			ch_label[3].code[1] = 18;
			ch_label[3].type = 'c';
			//第三行第二列	 其他	
			ch_label[4].loc.x = 32;
			ch_label[4].loc.y = 32;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 19;
			ch_label[4].code[1] = 20;
			ch_label[4].type = 'c';			
			//第四行第一列  I/O
			en_label[0].loc.x = 8;
			en_label[0].loc.y = 48;
			en_label[0].length = 3;
			en_label[0].code[0] = 15;
			en_label[0].code[1] = 16;
			en_label[0].code[2] = 17;
			en_label[0].type = 'e';	
			
			LabelLen = 5;
			LabelLenE = 1;

			option[0].loc.x = 0;
			option[0].loc.y = 16;
			option[0].Func = page2op0;
			option[1].loc.x = 0;
			option[1].loc.y = 32;
			option[1].Func = page2op1;
			option[2].loc.x = 0;
			option[2].loc.y = 48;
			option[2].Func = page2op2;
			option[3].loc.x = 24;
			option[3].loc.y = 16;
			option[3].Func = page2op3;
			option[4].loc.x = 24;
			option[4].loc.y = 32;
			option[4].Func = page2op4;
				
			OptionLen = 5;

			break;
		//page3 参数设置 页面
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
			//第二行第一列	   基本
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].type = 'c';
			//第三行第一列		控制  这里根据说明书要改成 终端
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			//ch_label[2].code[0] = 15;//控制
			//ch_label[2].code[1] = 16;
			ch_label[2].code[0] = 134;//终端
			ch_label[2].code[1] = 135;
			ch_label[2].type = 'c';
		  	//第二行第二列  信号标定  //根据说明书改成 控制
			ch_label[3].loc.x = 32;
			ch_label[3].loc.y = 16;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 15;
			ch_label[3].code[1] = 16;
			//ch_label[3].code[0] = 29;
			//ch_label[3].code[1] = 30;
			//ch_label[3].code[2] = 31;
			//ch_label[3].code[3] = 32;
			ch_label[3].type = 'c';
			//第三行第二列		其他  //根据说明书 改成 信号
			ch_label[4].loc.x = 32;
			ch_label[4].loc.y = 32;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 29;
			ch_label[4].code[1] = 30;	
			//ch_label[4].code[0] = 19;
			//ch_label[4].code[1] = 20;
			ch_label[4].type = 'c';			
			//第四行第一列  I/O
			en_label[0].loc.x = 8;
			en_label[0].loc.y = 48;
			en_label[0].length = 3;
			en_label[0].code[0] = 15;
			en_label[0].code[1] = 16;
			en_label[0].code[2] = 17;
			en_label[0].type = 'e';	
			//第四行第二列		   自学习  //根据说明书 修改成 其他
			ch_label[5].loc.x = 32;
			ch_label[5].loc.y = 48;
			ch_label[5].length = 2;
			ch_label[5].code[0] = 19;
			ch_label[5].code[1] = 20;
			//ch_label[5].code[0] = 111;
			//ch_label[5].code[1] = 112;
			//ch_label[5].code[2] = 113;
			ch_label[5].type = 'c';		
			
			LabelLen = 6;
			LabelLenE = 1;

			option[0].loc.x = 0;
			option[0].loc.y = 16;
			option[0].Func = page3op0; 	//第一个选项  基本
			option[1].loc.x = 0;
			option[1].loc.y = 32;
			option[1].Func = page3op1;	//第二个选项  控制
			option[2].loc.x = 0;
			option[2].loc.y = 48;
			option[2].Func = page3op2;  //第三个选项  I/O
			option[3].loc.x = 24;
			option[3].loc.y = 16;
			option[3].Func = page3op3;	//第四个选项  信号标定
			option[4].loc.x = 24;
			option[4].loc.y = 32;
			option[4].Func = page3op4;	//第五个选项  其他
			option[5].loc.x = 24;
			option[5].loc.y = 48;
			option[5].Func = page3op5;	//第六个选项  自学习
				
			OptionLen = 6;
			break;
		//case 4 和case 5为 设置界面退出后询问是否保存用的，关联的是保存参数到flash的函数

		 case 4:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//第二行第一列	   有设置改变
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 5;
			ch_label[1].code[0] = 149;
			ch_label[1].code[1] = 129;
			ch_label[1].code[2] = 128;
			ch_label[1].code[3] = 142;
			ch_label[1].code[4] = 150;
			ch_label[1].type = 'c';
			//或者修改成有两个op的界面，    两个op：保存修改  不保存修改
			//第三行第一列	按确认保存修改
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			//ch_label[2].code[0] = 137;
			//ch_label[2].code[1] = 138;
			//ch_label[2].code[2] = 139;
			ch_label[2].code[0] = 66;
			ch_label[2].code[1] = 143;
			ch_label[2].code[2] = 141;
			ch_label[2].code[3] = 142;
			ch_label[2].type = 'c';
		  	//第二行第二列  	按取消则不保存
			ch_label[3].loc.x = 16;
			ch_label[3].loc.y = 48;
			ch_label[3].length = 3;
			//ch_label[3].code[0] = 137;
			//ch_label[3].code[1] = 144;
			//ch_label[3].code[2] = 145;
			//ch_label[3].code[3] = 151;
			ch_label[3].code[0] = 96;
			ch_label[3].code[1] = 66;
			ch_label[3].code[2] = 143;
			ch_label[3].type = 'c';
			
			LabelLen = 4;
			LabelLenE = 0;

			option[0].loc.x = 8;
			option[0].loc.y = 32;
			option[0].Func = page4op0; 	//第一个选项  保存修改
			option[1].loc.x = 8;
			option[1].loc.y = 48;
			option[1].Func = page4op1;	//第二个选项  不保存修改
				
			OptionLen = 2 ;
			break;
		 case 5:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//第二行第一列	   没有设置改变
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 6;
			ch_label[1].code[0] = 148;
			ch_label[1].code[1] = 149;
			ch_label[1].code[2] = 129;
			ch_label[1].code[3] = 128;
			ch_label[1].code[4] = 142;
			ch_label[1].code[5] = 150;
			ch_label[1].type = 'c';
			//第三行第一列	退出设置
			ch_label[2].loc.x = 16;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 4;
			//ch_label[2].code[0] = 137;
			//ch_label[2].code[1] = 138;
			//ch_label[2].code[2] = 139;
			ch_label[2].code[0] = 119;
			ch_label[2].code[1] = 120;
			ch_label[2].code[2] = 129;
			ch_label[2].code[3] = 128;
			ch_label[2].type = 'c';
			
			LabelLen = 3;
			LabelLenE = 0;

			option[0].loc.x = 8;
			option[0].loc.y = 32;
			option[0].Func = page5op0; 	//退出设置
				
			OptionLen = 1 ;
			break;
		case 6:
			//第一行：参数设置	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//第二行第一列	   输入密码
			ch_label[1].loc.x = 16;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 4;
			ch_label[1].code[0] = 155;
			ch_label[1].code[1] = 156;
			ch_label[1].code[2] = 157;
			ch_label[1].code[3] = 158;
			ch_label[1].type = 'c';			
			
			LabelLen = 2;
			LabelLenE = 0;				
			OptionLen = 0;

			//横坐标以4递增
			input_op[0].loc.x = 16;
			input_op[0].loc.y = 32;
			input_op[0].num = 0;
			input_op[0].Func = page6inop;
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
	}
	//显示当前页面的静态部分
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}

	if(ReturnFromAreaFlag == 0)	{
		OptionLoc = 0; 		//选项归0位置
	}
	if(ReturnFromAreaFlag == 1)
	{
		OptionLoc = AreaNum;
		ReturnFromAreaFlag = 0;
	}
	//显示当前页面的第一个op
	if(OptionLen > 0)
		OptionShow(option[OptionLoc]);
	//显示当前页面的动态部分	

	//显示输入部分
	if(InputOpLen > 0){
		InputNumAllShow(input_op , InputOpLen);
		InputNumChangedShow( input_op[0] );
	}			
}

//每个页面中的每个选项对应的函数 
void page_esc(void)
{
	PageNum = LastPageNum ;
	PageChageFlage = 1;		
}

void func_null(void)
{

}

void page0op0(void)
{
	//跳转到页面1
	LastPageNum = PageNum;
	PageNum = 1;
	PageChageFlage = 1;
}

//page1是 一级菜单 选择：参数浏览、参数设置
void page1op0(void)
{
	LastPageNum = PageNum;
	PageNum = 6;				//跳转到了页面2，参数设置页面
	PageChageFlage = 1;
}
void page1op1(void)
{
	LastPageNum = PageNum;
	PageNum = 2;			   //跳转到了页面3，参数浏览页面
	PageChageFlage = 1;
}

//第二个页面的选项处理函数
//进入Page2Area0
void page3op0(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 0;				//page3的area0	 基本参数
	area_mode();
}

void page3op1(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 1;				//page3的area1	 控制参数设置   
	//AreaNum = 6;				//根据说明书的修改   终端
	area_mode();
}

void page3op2(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 2;				//page3的area2	 I\O信号设置
	area_mode();
}

void page3op3(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 3;				//page3的area3	 信号标定  //根据说明书 这里改为控制
	area_mode();
}
void page3op4(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 4;				//page3的area4  其他
	area_mode();
}
void page3op5(void)
{

	AreaMode = 1; 				//打开Area模式
	AreaNum = 5;				//page3的area5  自学习\自整定模式  显示一个界面
	area_mode();	
}

void page3op6(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 6;				//page3的area6	 终端设置  根据说明书加入的   
	area_mode();
}

//-------------------------保存设置的修改 
void page4op0(void)
{
	ParaArray[HaveSetTimes]++;
	SPI_FLASH_SectorErase(FLASH_SectorToErase);	
	SPI_FLASH_BufferWrite( (u8*)ParaArray , FLASH_WriteAddress , ParaArrayLength*4 );
	//SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//将ParaArray[]保存
	IOInit();
	ParaInit();
	//ParaInit();


	LastPageNum = PageNum;
	PageNum = 1;				//跳转到了页面1
	PageChageFlage = 1;
}

//不保存修改  从flash更新ParaArray[]
void page4op1(void)
{
	int i = 0;
	SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//在ParaArray_copy[]中做一个备份，用来对比
	for( i=0;i<ParaArrayLength ; i++ ){
		*(ParaArray_copy+i) = *(ParaArray+i);
	}		
	LastPageNum = PageNum;
	PageNum = 1;				//跳转到了页面1
	PageChageFlage = 1;
}

//啥都没改，退出设置
void page5op0(void)
{
	LastPageNum = PageNum;
	PageNum = 1;				//跳转到了页面2，参数浏览页面
	PageChageFlage = 1;	
}

//输入密码的界面，如果密码正确则进入设置界面
//如果密码正确，重新输入
void page6inop()
{
	int temp = 0 ;
	temp = 	input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	
	//初始状态
	if(ParaArray[Security] == 0xffff )
	{
		ParaArray[Security] = 0;
		//密码正确
		if ( temp == ParaArray[Security] )
		{
			LastPageNum = PageNum;
			PageNum = 3;			//跳转到了页面3，参数设置页面
			PageChageFlage = 1;		
		}else{						//密码不正确，刷新当前页面
			PageChageFlage = 1;	
		}			
	}
	else
	{
		//密码正确
		if ( temp == ParaArray[Security] )
		{
			LastPageNum = PageNum;
			PageNum = 3;			//跳转到了页面3，参数设置页面
			PageChageFlage = 1;		
		}else{						//密码不正确，刷新当前页面
			PageChageFlage = 1;	
		}
	}
}

void page2op0(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 0;				//page2的area0   参数浏览：基本
	area_mode();		
}
void page2op1(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 1;				//page2的area1   参数浏览：控制
	area_mode();			
}
void page2op2(void)
{
	AreaMode = 1;				//打开Area模式
	AreaNum = 2;				//page2的area2   参数浏览：I/O
	area_mode();			
}
void page2op3(void)
{
	AreaMode = 1; 				//打开Area模式
	AreaNum = 3;				//page2的area3   参数浏览：故障
	area_mode();			
}
void page2op4(void)
{
	AreaMode = 1;		 		//打开Area模式
	AreaNum = 4;				//page2的area4   参数浏览：其他
	area_mode();			
}


