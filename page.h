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

//ÿ��Area��һ���ֲ���ʾ�ı仯											 
extern int AreaChageFlag;  //0 not chaged,  1 changed
extern int AreaMode; //0 ��ʾ����areaģʽ��1��ʾ��areaģʽ
extern int AreaNum;		//Area��
extern int PageAreaRec[4][5];	//��5��Area�ڲ���ָ��	��һ��ά����ҳ��ţ���PageNum��  �ڶ���ά����Area�ţ�����AreaNum��
extern int PageAreaLen[4][5];	//�ֱ��5��Area�ĳ���


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



void func_null(void);		//�պ���
void page0op0(void);		//һ����ҳ���ѡ���
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

//ͳһ����ʾ����
typedef struct Point{
	int x;
	int y;
}Menu_Point;

typedef struct Label{
	Menu_Point loc;		//λ��
	char type; 			//����
	int length;			//����
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
	int page_num;			//ҳ���
	int page_class;			//ҳ��ּ�0\1\2\3�ĸ�����	

	Menu_Option	option[10];	//ÿ��ҳ����ʱ����ʮ��ѡ��
	int option_rec; 		//��¼��ǰѡ��С�������ڵ�λ��
	int option_sum;			//ҳ������ѡ���������

	Menu_Label *label_head;	//��̬�ַ��������ͷָ��
	Menu_Option	*option_head;//option ����ı�ͷ
	Menu_Option	*option_now; //��ǰλ��
	Menu_Option	*option_end;//option ����ı�ͷ

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
extern Menu_InputOp input_op[5];		//������������


#endif

