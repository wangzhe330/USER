#include "page.h"
#include "oled.h"
#include "stdlib.h"
#include "area.h"
#include "para.h"
#include "spi_flash.h"
#include "stdio.h"

//�˵��Ľṹ
// menu:page->area->atom
//				  ->sub_area->atom

Menu_Option option[6];
Menu_Label ch_label[8];
Menu_Label en_label[8];
int LastPageNum = 1;  		//������һ��ҳ���
int PageNum = 1; 			//��0ҳ�治Ҫ�ˣ����Դ�1��ʼ
int PageChageFlage = 0;  	//0 not chaged,  1 changed
int LabelLen = 0;
int LabelLenE = 0;
int OptionLen = 0;
int OptionChageFlage = 0;  		//0 not chaged,  1 changed
int OptionLoc = 0;

int ReturnFromAreaFlag = 0;

Menu_InputOp input_op[5];		//������������
int InputOpLen = 0;
int InputOpRec = 0;
int InputOpLast = 0;			//��¼��һ��inputopλ��
int InputOpChageFlag = 0;	 	//0 not chaned,1 changed
int InputNumChageFlag = 0;	 	//0 not chaned,1 changed
int InputMode = 0;				//0��ʾ����inputģʽ��1��ʾ��


//ÿ��Area��һ���ֲ�atom��ʾ�ı仯											 
int AreaChageFlag = 0;  //0 not chaged,  1 changed
int AreaMode = 0; 		//0 ��ʾ����areaģʽ��1��ʾ��areaģʽ
int AreaNum;			//Area��
int PageAreaRec[4][5];	//��5��Area�ڲ���ָ��	��һ��ά����ҳ��ţ���PageNum��  �ڶ���ά����Area�ţ�����AreaNum��
int PageAreaLen[4][5];	//�ֱ��5��Area�ĳ���	 ��һ��ά����4������Ϊҳ��2��3����Area����Ϊ�˺�PageNum����

int SubAreaMode = 0;
int SubAreaLen = 0;
int SubAreaRec = 0;
int SubAreaChangeFlag = 0;
int SubAreaNum = 0;

//IO����ʱ��ļ���  6��
int	IOSetRec = 0;
#define IOSetLen 6 

int BigMode = 0;		//0,1,2 ��ʾ ���á��͵ء�Զ��

//ÿ��ҳ��������ﾳ��ȷ���ģ����Բ���Ҫ�����ӵ��л�����ֻ��Ҫ���ݵ���PageNum�Ϳ���֪��ҳ������Щ��������Ҫ���µ�
//ÿ��ҳ�湫��һ�׹����Ĵ洢�ռ����ʡ�ռ�  MenuLabel
//MenuOption ÿ��ҳ����Ҫ�Լ������Ĵ洢�ռ䣬 �������Option������һ��struct

M_Page Page[PageSum];


//�����ʼ����page֮��ĸ��ӹ�ϵ
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
	Page[1].parent = 1;		   //�����page1�ĸ�pageָ�����Լ���ȥ����page0
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

void OptionChage(Menu_Option old_option , Menu_Option new_option ) //����һ��ֻ��һ�������ƶ�ѡ�����ǵĳ���	
{
	hz_1616_clear( old_option.loc.x , old_option.loc.y );	//����ɵ�ѡ����
	hz_1616( new_option.loc.x , new_option.loc.y , op ,0);	//�½��µ�ѡ����		
}

//����һ�������õķ��Թ���
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
	if(PageNum==3 && AreaNum==4){				//�źű궨
		ascii_1608( 40 , 48 , english[31] ,0);	//С����	
	}
	else if(PageNum != 6 && (!(PageNum==3&&AreaNum == 5))) {
		ascii_1608( 24 , 48 , english[31] ,0);	//С����	
		ascii_1608( 32 , 48 , english[32] ,0);	//�ٷֺ�
	}
}
void InputNumChangedShow(Menu_InputOp in_op)
{
	ascii_1608( in_op.loc.x , in_op.loc.y , english[in_op.num+18] ,1);
	if(PageNum==3&&AreaNum==4){				//�źű궨
		ascii_1608( 40 , 48 , english[31] ,0);	//С����	
	}
	else if(PageNum != 6 && !(PageNum==3&&AreaNum == 5)){
		ascii_1608( 24 , 48 , english[31] ,0);	//С����	
		ascii_1608( 32 , 48 , english[32] ,0);	//�ٷֺ�
	}				
}
void InputNumShow(Menu_InputOp in_op)
{
	ascii_1608( in_op.loc.x , in_op.loc.y , english[in_op.num+18] ,0);
	if(PageNum==3&&AreaNum==4){				//�źű궨
		ascii_1608( 40 , 48 , english[31] ,0);	//С����	
	}
	else if(PageNum != 6 && !(PageNum==3&&AreaNum == 5)){
		ascii_1608( 24 , 48 , english[31] ,0);	//С����	
		ascii_1608( 32 , 48 , english[32] ,0);	//�ٷֺ�
	}
}

//������ʾ����  �̶�5λ
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

//�͵�ģʽ  ֻ��һ�����棬����Ҫ��ת�����Ե��������д
//�͵�ģʽ�Ľ�������Ӧ�û���һЩ������Ϣ
void local_mode(void )
{
	int cnt_label = 0;
	Fill_RAM(0x00);

	//��һ��label    �͵�
	ch_label[0].loc.x = 0;
	ch_label[0].loc.y = 0;
	ch_label[0].code[0] = 0;
	ch_label[0].code[1] = 1;
	ch_label[0].length = 2;
	ch_label[0].type = 'c';	
	//ά��ʽ\����ʽ	 (���ܵ��� pluse_mode �� hold_mode����һ��λ��)
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

	//��ʾ��ǰҳ��ľ�̬����
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}

	
}

//Զ�̿���ģʽ
void faraway_mode(void)
{
	int cnt_label = 0;
	Fill_RAM(0x00);

	//��һ��label    Զ��
	ch_label[0].loc.x = 0;
	ch_label[0].loc.y = 0;
	ch_label[0].code[0] = 2;
	ch_label[0].code[1] = 3;
	ch_label[0].length = 2;
	ch_label[0].type = 'c';

	if( ParaArray[FarawayMode] == pluse_mode )
	{
		//����ʽ
		ch_label[1].code[0] = 4;
		ch_label[1].code[1] = 5;
		ch_label[1].code[2] = 6; 
	}
	if( ParaArray[FarawayMode] == hold_mode )
	{
		//ά��
		ch_label[1].code[0] = 86;
		ch_label[1].code[1] = 87;
		ch_label[1].code[2] = 6; 
	} 
	if( ParaArray[FarawayMode] == double_mode )
	{
		//˫λʽ
		ch_label[1].code[0] = 91;
		ch_label[1].code[1] = 127;
		ch_label[1].code[2] = 6; 
	} 
	if( ParaArray[FarawayMode] == analog_mode )
	{
		//ģ��ʽ
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
		//�趨ֵ
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

	//��ʾ��ǰҳ��ľ�̬����
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}
			
}

//****************************************************pageģʽ**************************************************
void page_mode(void)
{
	//Option_List PageOptionList;
	int cnt_label = 0;
	
	Fill_RAM(0x00);
	//�����������ľֲ������ķ�������Ҫ��case����ȷ��ÿ������ķ�Χ���ٶ�������ܻ��һ�㣬����ʾ��һ������
	//Fill_Block(0,40,0,32,0x00);  //����������кţ��ص��кţ�����кţ��յ��кţ����Ҷ�ֵ

	//��־λ����
	LabelLen = 0;
	LabelLenE = 0;				
	OptionLen = 0;
	InputOpLen = 0;
	InputOpRec = 0;
	InputMode = 0;		
	//����ҳ���ȷ��һЩÿ��ҳ�湫����
	switch (PageNum)
	{
		case 0:
			//��һ��label    ����ѡ�� ��Ӣ��
			//����ѡ�� ��һ��
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].code[0] = 21;
			ch_label[0].code[1] = 22;
			ch_label[0].code[2] = 23;
			ch_label[0].code[3] = 24;
			ch_label[0].length = 4;
			ch_label[0].type = 'c';
			//Language �ڶ���
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
			//����  ������	
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y =32;
			ch_label[1].code[0] = 27;
			ch_label[1].code[1] = 28;
			ch_label[1].length = 2;
			ch_label[1].type = 'c';
			//english ������
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
			//page1   �������\��������
			//��һ�� ��̬
			//�ڶ��� ����
			//������ ����
			//������ ���
			//��һ�У���̬	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 2;
			ch_label[0].code[0] = 132;
			ch_label[0].code[1] = 133;
			ch_label[0].type = 'c';
			//�ڶ��У����� 
			ch_label[1].loc.x = 0;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 7;
			ch_label[1].code[1] = 8;
			ch_label[1].type = 'c';
			//������ ����
			ch_label[2].loc.x = 24;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 9;
			ch_label[2].code[1] = 10;
			ch_label[2].type = 'c';
			//������ ���
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

		//page2������� ҳ��
		case 2:
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
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].type = 'c';
			//�����е�һ��	 ����
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			ch_label[2].code[0] = 15;
			ch_label[2].code[1] = 16;
			ch_label[2].type = 'c';
		  	//�ڶ��еڶ���	 ����
			ch_label[3].loc.x = 32;
			ch_label[3].loc.y = 16;
			ch_label[3].length = 2;
			ch_label[3].code[0] = 17;
			ch_label[3].code[1] = 18;
			ch_label[3].type = 'c';
			//�����еڶ���	 ����	
			ch_label[4].loc.x = 32;
			ch_label[4].loc.y = 32;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 19;
			ch_label[4].code[1] = 20;
			ch_label[4].type = 'c';			
			//�����е�һ��  I/O
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
		//page3 �������� ҳ��
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
			//�ڶ��е�һ��	   ����
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 2;
			ch_label[1].code[0] = 13;
			ch_label[1].code[1] = 14;
			ch_label[1].type = 'c';
			//�����е�һ��		����  �������˵����Ҫ�ĳ� �ն�
			ch_label[2].loc.x = 8;
			ch_label[2].loc.y = 32;
			ch_label[2].length = 2;
			//ch_label[2].code[0] = 15;//����
			//ch_label[2].code[1] = 16;
			ch_label[2].code[0] = 134;//�ն�
			ch_label[2].code[1] = 135;
			ch_label[2].type = 'c';
		  	//�ڶ��еڶ���  �źű궨  //����˵����ĳ� ����
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
			//�����еڶ���		����  //����˵���� �ĳ� �ź�
			ch_label[4].loc.x = 32;
			ch_label[4].loc.y = 32;
			ch_label[4].length = 2;
			ch_label[4].code[0] = 29;
			ch_label[4].code[1] = 30;	
			//ch_label[4].code[0] = 19;
			//ch_label[4].code[1] = 20;
			ch_label[4].type = 'c';			
			//�����е�һ��  I/O
			en_label[0].loc.x = 8;
			en_label[0].loc.y = 48;
			en_label[0].length = 3;
			en_label[0].code[0] = 15;
			en_label[0].code[1] = 16;
			en_label[0].code[2] = 17;
			en_label[0].type = 'e';	
			//�����еڶ���		   ��ѧϰ  //����˵���� �޸ĳ� ����
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
			option[0].Func = page3op0; 	//��һ��ѡ��  ����
			option[1].loc.x = 0;
			option[1].loc.y = 32;
			option[1].Func = page3op1;	//�ڶ���ѡ��  ����
			option[2].loc.x = 0;
			option[2].loc.y = 48;
			option[2].Func = page3op2;  //������ѡ��  I/O
			option[3].loc.x = 24;
			option[3].loc.y = 16;
			option[3].Func = page3op3;	//���ĸ�ѡ��  �źű궨
			option[4].loc.x = 24;
			option[4].loc.y = 32;
			option[4].Func = page3op4;	//�����ѡ��  ����
			option[5].loc.x = 24;
			option[5].loc.y = 48;
			option[5].Func = page3op5;	//������ѡ��  ��ѧϰ
				
			OptionLen = 6;
			break;
		//case 4 ��case 5Ϊ ���ý����˳���ѯ���Ƿ񱣴��õģ��������Ǳ��������flash�ĺ���

		 case 4:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	   �����øı�
			ch_label[1].loc.x = 8;
			ch_label[1].loc.y = 16;
			ch_label[1].length = 5;
			ch_label[1].code[0] = 149;
			ch_label[1].code[1] = 129;
			ch_label[1].code[2] = 128;
			ch_label[1].code[3] = 142;
			ch_label[1].code[4] = 150;
			ch_label[1].type = 'c';
			//�����޸ĳ�������op�Ľ��棬    ����op�������޸�  �������޸�
			//�����е�һ��	��ȷ�ϱ����޸�
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
		  	//�ڶ��еڶ���  	��ȡ���򲻱���
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
			option[0].Func = page4op0; 	//��һ��ѡ��  �����޸�
			option[1].loc.x = 8;
			option[1].loc.y = 48;
			option[1].Func = page4op1;	//�ڶ���ѡ��  �������޸�
				
			OptionLen = 2 ;
			break;
		 case 5:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	   û�����øı�
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
			//�����е�һ��	�˳�����
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
			option[0].Func = page5op0; 	//�˳�����
				
			OptionLen = 1 ;
			break;
		case 6:
			//��һ�У���������	 
			ch_label[0].loc.x = 0;
			ch_label[0].loc.y = 0;
			ch_label[0].length = 4;
			ch_label[0].code[0] = 7;
			ch_label[0].code[1] = 8;
			ch_label[0].code[2] = 9;
			ch_label[0].code[3] = 10;							  
			ch_label[0].type = 'c';
			//�ڶ��е�һ��	   ��������
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

			//��������4����
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
			InputMode = 1;		//���������ģʽ
									
			break;			
	}
	//��ʾ��ǰҳ��ľ�̬����
	for(cnt_label=0;cnt_label<LabelLen;cnt_label++){
		MenuLabelShow(ch_label[cnt_label]);
	}
	for(cnt_label=0;cnt_label<LabelLenE;cnt_label++){
		MenuLabelShow(en_label[cnt_label]);
	}

	if(ReturnFromAreaFlag == 0)	{
		OptionLoc = 0; 		//ѡ���0λ��
	}
	if(ReturnFromAreaFlag == 1)
	{
		OptionLoc = AreaNum;
		ReturnFromAreaFlag = 0;
	}
	//��ʾ��ǰҳ��ĵ�һ��op
	if(OptionLen > 0)
		OptionShow(option[OptionLoc]);
	//��ʾ��ǰҳ��Ķ�̬����	

	//��ʾ���벿��
	if(InputOpLen > 0){
		InputNumAllShow(input_op , InputOpLen);
		InputNumChangedShow( input_op[0] );
	}			
}

//ÿ��ҳ���е�ÿ��ѡ���Ӧ�ĺ��� 
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
	//��ת��ҳ��1
	LastPageNum = PageNum;
	PageNum = 1;
	PageChageFlage = 1;
}

//page1�� һ���˵� ѡ�񣺲����������������
void page1op0(void)
{
	LastPageNum = PageNum;
	PageNum = 6;				//��ת����ҳ��2����������ҳ��
	PageChageFlage = 1;
}
void page1op1(void)
{
	LastPageNum = PageNum;
	PageNum = 2;			   //��ת����ҳ��3���������ҳ��
	PageChageFlage = 1;
}

//�ڶ���ҳ���ѡ�����
//����Page2Area0
void page3op0(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 0;				//page3��area0	 ��������
	area_mode();
}

void page3op1(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 1;				//page3��area1	 ���Ʋ�������   
	//AreaNum = 6;				//����˵������޸�   �ն�
	area_mode();
}

void page3op2(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 2;				//page3��area2	 I\O�ź�����
	area_mode();
}

void page3op3(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 3;				//page3��area3	 �źű궨  //����˵���� �����Ϊ����
	area_mode();
}
void page3op4(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 4;				//page3��area4  ����
	area_mode();
}
void page3op5(void)
{

	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 5;				//page3��area5  ��ѧϰ\������ģʽ  ��ʾһ������
	area_mode();	
}

void page3op6(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 6;				//page3��area6	 �ն�����  ����˵��������   
	area_mode();
}

//-------------------------�������õ��޸� 
void page4op0(void)
{
	ParaArray[HaveSetTimes]++;
	SPI_FLASH_SectorErase(FLASH_SectorToErase);	
	SPI_FLASH_BufferWrite( (u8*)ParaArray , FLASH_WriteAddress , ParaArrayLength*4 );
	//SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//��ParaArray[]����
	IOInit();
	ParaInit();
	//ParaInit();


	LastPageNum = PageNum;
	PageNum = 1;				//��ת����ҳ��1
	PageChageFlage = 1;
}

//�������޸�  ��flash����ParaArray[]
void page4op1(void)
{
	int i = 0;
	SPI_FLASH_BufferRead( (u8*)ParaArray , FLASH_ReadAddress, ParaArrayLength*4 );
	//��ParaArray_copy[]����һ�����ݣ������Ա�
	for( i=0;i<ParaArrayLength ; i++ ){
		*(ParaArray_copy+i) = *(ParaArray+i);
	}		
	LastPageNum = PageNum;
	PageNum = 1;				//��ת����ҳ��1
	PageChageFlage = 1;
}

//ɶ��û�ģ��˳�����
void page5op0(void)
{
	LastPageNum = PageNum;
	PageNum = 1;				//��ת����ҳ��2���������ҳ��
	PageChageFlage = 1;	
}

//��������Ľ��棬���������ȷ��������ý���
//���������ȷ����������
void page6inop()
{
	int temp = 0 ;
	temp = 	input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;
	
	//��ʼ״̬
	if(ParaArray[Security] == 0xffff )
	{
		ParaArray[Security] = 0;
		//������ȷ
		if ( temp == ParaArray[Security] )
		{
			LastPageNum = PageNum;
			PageNum = 3;			//��ת����ҳ��3����������ҳ��
			PageChageFlage = 1;		
		}else{						//���벻��ȷ��ˢ�µ�ǰҳ��
			PageChageFlage = 1;	
		}			
	}
	else
	{
		//������ȷ
		if ( temp == ParaArray[Security] )
		{
			LastPageNum = PageNum;
			PageNum = 3;			//��ת����ҳ��3����������ҳ��
			PageChageFlage = 1;		
		}else{						//���벻��ȷ��ˢ�µ�ǰҳ��
			PageChageFlage = 1;	
		}
	}
}

void page2op0(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 0;				//page2��area0   �������������
	area_mode();		
}
void page2op1(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 1;				//page2��area1   �������������
	area_mode();			
}
void page2op2(void)
{
	AreaMode = 1;				//��Areaģʽ
	AreaNum = 2;				//page2��area2   ���������I/O
	area_mode();			
}
void page2op3(void)
{
	AreaMode = 1; 				//��Areaģʽ
	AreaNum = 3;				//page2��area3   �������������
	area_mode();			
}
void page2op4(void)
{
	AreaMode = 1;		 		//��Areaģʽ
	AreaNum = 4;				//page2��area4   �������������
	area_mode();			
}


