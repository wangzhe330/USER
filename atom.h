#ifndef ATOM_INCLUDE
#define ATOM_INCLUDE

float AD654ToCurrent(void);

void AD654Read(void);

void ValveSetShow(void);

//右上角的实时显示阀位功能
void ValvePosShow(void);

void ValveErrorShow(void);

//显示参数  在第四行
void ParaShow(int x);

//页面3area0 基本参数设置  开向限位：行程
void page3area0atom0op0(void);
//页面3area0 基本参数设置  开向限位：转矩
void page3area0atom0op1(void);

//页面3area1 基本参数设置  关向限位：行程
void page3area0atom1op0(void);

//页面3area1 基本参数设置  关向限位：转矩
void page3area0atom1op1(void);
//页面3area2 基本参数设置   中间位置，这里是没有选项的，应该是一个数字输入函数
void page3area0atom2op0(void);
void page3area0atom2inop(void);

//页面3area3 基本参数设置   开向设置转矩，这里是没有选项的，应该是一个数字输入函数
void page3area0atom3inop(void);

//页面3area4 基本参数设置   开向设置转矩，这里是没有选项的，应该是一个数字输入函数
void page3area0atom4inop(void);

//页面3area5 基本参数设置   紧急控制方式：保位
void page3area0atom5op0(void);
//页面3area5 基本参数设置   紧急控制方式：转矩位置
void page3area0atom5op1(void);


//页面3area1 控制参数设置 ：就地  这两个应该要进入subarea
void page3area1atom0op0(void);
//就地控制参数设置，选为脉冲式
void page3area1atom0sub0op0(void);
//就地控制参数设置，选为维持式
void page3area1atom0sub0op1(void);


//页面3area1 控制参数设置 ：远程
void page3area1atom0op1(void);

//远程控制参数设置，选为脉冲式
void page3area1atom0sub1op0(void);
//远程控制参数选择，选为维持式
void page3area1atom0sub1op1(void);
//远程控制参数选择，选为双位式
void page3area1atom0sub1op2(void);
//远程控制参数选择，选为模拟量式
void page3area1atom0sub1op3(void);


//页面3area2 I/O参数设置    开关信号1；常开
void page3area2atom0op0(void);
//页面3area2 I/O参数设置    开关信号1；常闭
void page3area2atom0op1(void);

//页面3area2 I/O参数设置    开关信号1；表示  12个报错信息
void page3area2atom1op(void);
void page3area2atom2op(void);
void page3area2atom3op(void);
void page3area2atom4op(void);
void page3area2atom5op(void);
void page3area2atom6op(void);
void page3area2atom7op(void);
void page3area2atom8op(void);
void page3area2atom9op(void);
void page3area2atom10op(void);
void page3area2atom11op(void);
void page3area2atom12op(void);

//终端
void page3area3atom0op(void);
void page3area3atom1op(void);


void page3area4atom0op(void);
void page3area4atom1op(void);
//页面3area3 标定参数       标定参数 设定值8ma 
void page3area4atom2op(void);
//页面3area3 标定参数       标定参数 设定值16ma    
void page3area4atom3op(void);

//页面3area3atom4op 标定参数  设定当前位置为阀位0%
void page3area3atom4op(void);


//页面3area3op5 标定参数  设定当前位置为阀位100%  
void page3area3atom5op(void); 

//页面3area4 其他设置       恢复出厂设置；不恢复
void page3area4atom0op0(void);
//页面3area4 其他设置       恢复出厂设置；恢复 
void page3area4atom0op1(void);
//页面3area4 其他设置       报警清除；不清除
void page3area4atom1op0(void);
//页面3area4 其他设置       报警清除；清除 
void page3area4atom1op1(void);

//页面3area5 其他设置       修改密码：不修改 
void page3area5atom2op0(void);
//页面3area5 其他设置       修改密码：修改 
void page3area5atom2op1(void);

//其他设置里面的进入自学习模式
void page3area5atom3op(void);
void page3area5atom0op0(void); //自整定退出

#endif
