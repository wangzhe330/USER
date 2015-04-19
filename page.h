#ifndef PAGE_INLCUDE
#define PAGE_INCLUDE

#include "oled.h"


#define PageSum 4

extern int PageNum ;
extern int PageChageFlage ;  //0 not chaged,  1 changed
extern int LabelLen ;
extern int LabelLenE;
extern int OptionLen ;
extern int OptionChageFlage;  //0 not chaged,  1 changed
extern int OptionLoc;

extern int ReturnFromAreaFlag;

//每个Area是一串局部显示的变化											 
extern int AreaChageFlag;  //0 not chaged,  1 changed
extern int AreaMode; //0 表示不在area模式，1表示在area模式
extern int AreaNum;		//Area号
extern int PageAreaRec[4][5];	//存5个Area内部的指向	第一个维度是页面号（用PageNum）  第二个维度是Area号（即用AreaNum）
extern int PageAreaLen[4][5];	//分别寸5个Area的长度


extern int InputOpLen ;
extern int InputOpRec ;
extern int InputOpLast;
extern int InputOpChageFlag;
extern int InputNumChageFlag ;	 //0 not chaned,1 changed
extern int InputMode ;

extern int SubAreaMode ;
extern int SubAreaLen ;
extern int SubAreaRec ;
extern int SubAreaChangeFlag ;
extern int SubAreaNum ;
//int SubAreaNum = 0;

extern int	IOSetRec;

extern int BigMode;



void func_null(void);		//空函数
void page0op0(void);		//一级主页面的选项函数
void page1op0(void);
void page1op1(void);

void page3op0(void);
void page3op1(void);
void page3op2(void);
void page3op3(void);
void page3op4(void);
void page3op5(void);

void page2op0(void);
void page2op1(void);
void page2op2(void);
void page2op3(void);
void page2op4(void);

void page4op0(void);
void page4op1(void);

void page5op0(void);

void page6inop(void);

//统一的显示函数
typedef struct Point{
	int x;
	int y;
}Menu_Point;

typedef struct Label{
	Menu_Point loc;		//位置
	char type; 			//类型
	int length;			//长度
	int code[10];
	struct Label *next;
}Menu_Label;

typedef struct Option{
	Menu_Point loc;
	struct Option *next;
	void (*Func)();
}Menu_Option;

typedef struct InputOp{
	Menu_Point loc;
	void (*Func)();
	int num;
}Menu_InputOp;

typedef struct M_Page{
	int num;
	int child;
	int parent;
}M_Page;

typedef struct Menu_Area{
	int num;
	int child;
	int parent;
}Menu_Area;


//typedef struct Menu_Page *Menu_Page;
typedef struct Menu_Page{
	int page_num;			//页面号
	int page_class;			//页面分级0\1\2\3四个级别	

	Menu_Option	option[10];	//每个页面暂时分配十个选项
	int option_rec; 		//记录当前选项小三角所在的位置
	int option_sum;			//页面里面选择项的总数

	Menu_Label *label_head;	//静态字符串链表的头指针
	Menu_Option	*option_head;//option 链表的表头
	Menu_Option	*option_now; //当前位置
	Menu_Option	*option_end;//option 链表的表头

}Menu_Page;



typedef struct Option_List{
	int length;
	Menu_Option *head; 
}Option_List;


void MenuLabelShow(Menu_Label l);
void PageShow(Menu_Page p);
void OptionShow(Menu_Option option);
void OptionChage(Menu_Option old_option , Menu_Option new_option);
void PageChage(Menu_Page *p);
void page_mode(void);
void local_mode(void );
void faraway_mode(void );
void PageInit(void);
void NumShow(int x,int y,int num);
void InputNumChangedShow(Menu_InputOp in_op);
void InputNumShow(Menu_InputOp in_op);
void InputNumAllShow(Menu_InputOp in_op[] , int len);

void test(void);

extern Menu_Option option[6];
extern Menu_Label ch_label[8];
extern Menu_Label en_label[8];
extern Menu_InputOp input_op[5];		//用作输入数字


#endif

