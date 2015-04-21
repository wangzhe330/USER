#include "page.h"
#include "area.h"
#include "oled.h"
#include "atom.h"
#include "para.h"
#include "current.h"
#include "control.h"
#include "Time_test.h"
#include "pwm.h"
#include "fault.h"
#include "key.h"

int ad654rec[10] = {0};
int alram = 0;

extern float set_k;

void AD654Read(void){
	ad654rec[9] = ad654rec[8]; 
	ad654rec[8] = ad654rec[7]; 
	ad654rec[7] = ad654rec[6];		
	ad654rec[6] = ad654rec[5]; 
	ad654rec[5] = ad654rec[4]; 	
	ad654rec[4] = ad654rec[3]; 
	ad654rec[3] = ad654rec[2];		
	ad654rec[2] = ad654rec[1]; 
	ad654rec[1] = ad654rec[0]; 
	ad654rec[0] = AD654F;
	AD654F = 0;	
	/*
	if(ad654rec[0] - ad654rec[1] > 10 || ad654rec[0] - ad654rec[1] < -10)
	{
		ad654rec[9] = 0;
	} */
}

//wz先这么弄吧
//先用4次均值滤波，会造成4*500ms=2s的设置滞后，如果4~20mA很精准的话，可以取消这个均值滤波
float AD654ToCurrent(void){
	float re = 0.0;
	int cnt = 0 , sum = 0;
	//re = ( (float)AD654F - 200.0 )/50.0 + 4.0;
	//re = ( (float)ad654rec[0] - 80.0 )/20.0 + 4.0;
	//ValveSetVal	= ((float)AD654F - 200.0 )*1.25 ;
	if( (ad654rec[0]-ad654rec[1]>5 && ad654rec[1]-ad654rec[2]>5) || (ad654rec[1]-ad654rec[0]>5 && ad654rec[2]-ad654rec[1]>5) )
		sum = ad654rec[0];
	else 
		sum = (ad654rec[0] + ad654rec[1] + ad654rec[2] + ad654rec[3])/4;
		/*
	for(cnt = 0 ; cnt < 4 ; cnt ++){
		sum = sum + ad654rec[cnt]/4 ;
	}	  */
	//ValveSetVal	= ((float)sum - ParaArray[Set8ma] )/()*500.0+250.0 ;
	ValveSetVal	=(int)( ((float)(sum - ParaArray[Set4ma] ))*set_k  );
	
	//

	if( ValveSetVal > 1000 && ValveSetVal < 1190 )
		ValveSetVal = 1000;
	if( ValveSetVal >= 1190 )
	{
		//guzhang  信号超限
		FaultTotal = FaultTotal | FaultSigOverMask;
	}
	if( ValveSetVal < 10 && ValveSetVal > -52 )
	{
		ValveSetVal = 0 ;
	}
	if( ValveSetVal <= -52 && ValveSetVal >= -135  )
	{
		//guzhang 信号超限故障
		FaultTotal = FaultTotal | FaultSigOverMask;
	}
	if( ValveSetVal < -135  )
	{
		//guzhang 信号中断故障
		FaultTotal = FaultTotal | FaultSigBreakMask;
		FaultTotal = FaultTotal & ~FaultSigOverMask;
	}

	//清除故障
	if( ValveSetVal >= -135  )
		FaultTotal = FaultTotal & ~FaultSigBreakMask;
	if(ValveSetVal >= -52 && ValveSetVal <= 1190){
		FaultTotal = FaultTotal & ~FaultSigOverMask;	
	}	
	/*
	ad654rec[9] = ad654rec[8];	
	ad654rec[8] = ad654rec[7]; 
	ad654rec[7] = ad654rec[6];
	ad654rec[6] = ad654rec[5]; 
	ad654rec[5] = ad654rec[4];	
	ad654rec[4] = ad654rec[3]; 
	ad654rec[3] = ad654rec[2];		
	ad654rec[2] = ad654rec[1]; 
	ad654rec[1] = ad654rec[0]; 
	ad654rec[0] = AD654F;
	if( ad654rec[2]-ad654rec[1] > 1 || ad654rec[2]-ad654rec[1] < -1)
	{
		alram = 1;	
	}

	AD654F = 0;	 */
	return re;
}
void ValveSetShow()
{
	//只有当未执行的时候，才可以设置Set这个参数，执行之后，Set这个参数保持，显示也保持
	//if(ValveSetExecuteFlag == 0)
	{
		//ValveSetVal = (int)(sum / 8);	   //平均值	原来是23  这里改成8 wz2015-3-13

		if(ValveSetVal>=0 && ValveSetVal<=1000){	
			if(ValveSetVal == 1000) {
			
				ascii_1608(40,16,english[ValveSetVal/1000+18],0);
				ascii_1608(44,16,english[ValveSetVal%1000/100+18],0);
				ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
				ascii_1608(56,16,english[ValveSetVal%10+18],0);
				ascii_1608(52,16,english[31],0);
				ascii_1608(60,16,english[32],0);	
			}else{
				ascii_1608(40,16,english[41],0); 
				ascii_1608(44,16,english[ValveSetVal/100+18],0);
				ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
				ascii_1608(56,16,english[ValveSetVal%10+18],0);
				ascii_1608(52,16,english[31],0);
				ascii_1608(60,16,english[32],0);				
			}
		}else{
			//guzhang  这里应该触发故障模块
				ascii_1608(40,16,english[41],0);
			
			ascii_1608(44,16,english[33],0);
			ascii_1608(48,16,english[34],0);
			ascii_1608(52,16,english[35],0);
			ascii_1608(56,16,english[36],0);
			ascii_1608(60,16,english[37],0);			
		}
	}	
}

//远程模式下4-20mA阀位设定值滤波、显示
void ValveSetShow_old()
{
	int i = 0 , j = 0;
	int temp = 0;
	int max = 0 , min = 65535;//去掉最大值和最小值
	long int sum = 0;
	/*
	for(i = 0 ; i < 10 ;i ++)
	{
		if(ValveSetValTemp[i] > max) max = ValveSetValTemp[i];
		if(ValveSetValTemp[i] < min) min = ValveSetValTemp[i];
	}
	for(i = 0 ; i < 10 ;i ++)
	{
		sum += ValveSetValTemp[i];
	}
	sum =sum - max - min; //去掉最大和最小的值
	*/
	for(i = 0 ; i < 25 ; i ++){
		for(j = 0 ; j < 25 ; j ++){
			if(ValveSetValTemp[j] < ValveSetValTemp[i]){
				temp = ValveSetValTemp[i];
				ValveSetValTemp[i] = ValveSetValTemp[j];
				ValveSetValTemp[j] = temp; 	
			}
		}
	}
	for(i = 8 ; i < 16 ;i ++)
	{
		sum += ValveSetValTemp[i];
	}


	//只有当未执行的时候，才可以设置Set这个参数，执行之后，Set这个参数保持，显示也保持
	if(ValveSetExecuteFlag == 0)
	{
		ValveSetVal = (int)(sum / 8);	   //平均值	原来是23  这里改成8 wz2015-3-13

		if(ValveSetVal>=0 && ValveSetVal<=1000){
			ascii_1608(44,16,english[ValveSetVal/100+18],0);
			ascii_1608(48,16,english[ValveSetVal%100/10+18],0);
			ascii_1608(56,16,english[ValveSetVal%10+18],0);
			ascii_1608(52,16,english[31],0);
			ascii_1608(60,16,english[32],0);	
		}else{
			ascii_1608(44,16,english[33],0);
			ascii_1608(48,16,english[34],0);
			ascii_1608(52,16,english[35],0);
			ascii_1608(56,16,english[36],0);
			ascii_1608(60,16,english[37],0);			
		}
	}
}

//远程模式下 控制误差显示
void ValveErrorShow()
{
	int temp = 0;
	if( ValveError >= 0){
		ascii_1608(40,32,english[39],0);
		ascii_1608(44,32,english[ValveError/100+18],0);
		ascii_1608(48,32,english[ValveError%100/10+18],0);
		ascii_1608(56,32,english[ValveError%10+18],0);
	}else{
		temp = 0-ValveError; 
		
		ascii_1608(40,32,english[38],0);
		ascii_1608(44,32,english[ValveError/100+18],0);
		ascii_1608(48,32,english[ValveError%100/10+18],0);
		ascii_1608(56,32,english[ValveError%10+18],0);		
	}
	ascii_1608(52,32,english[31],0);
	ascii_1608(60,32,english[32],0);	
}

//右上角的实时显示阀位功能
void ValvePosShow()
{
	int temp = 0;
	int ValvePosValueShow = 0;		//显示的值
	int ValvePosValueShowBits = 0; //位数
	int PlusOrMinus = 0;
	/*
	if(Main_KEY_State == 0)	   //设置模式 不论正负、不论是否超过100% 都显示出来
	{
		if(ValvePosValue > 999)
		{
			ValvePosValueShow  =  ValvePosValue;
			ValvePosValueShowBits = 4;
			PlusOrMinus = 1;
		}
		else if(ValvePosValue >= 0)
		{
			ValvePosValueShow  =  ValvePosValue;
			ValvePosValueShowBits = 3;
			PlusOrMinus = 1;		
		}
		else if( ValvePosValue < 0 )
		{
			ValvePosValueShow  =  -ValvePosValue;
			ValvePosValueShowBits = 3;
			PlusOrMinus = -1;

		}

		if(ValvePosValueShowBits == 3 )
		{
			if(PlusOrMinus == 1)
				ascii_1608(40,0,english[41],0);	
			else if(PlusOrMinus == -1)
				ascii_1608(40,0,english[38],0);
		}
		else if(ValvePosValueShowBits == 4 )
		{
		   ascii_1608(40,0,english[ValvePosValueShow/1000+18],0);
		}
		ascii_1608(44,0,english[ValvePosValueShow%1000/100+18],0);
		ascii_1608(48,0,english[ValvePosValueShow%100/10+18],0);
		ascii_1608(56,0,english[ValvePosValueShow%10+18],0);
		ascii_1608(52,0,english[31],0);
		ascii_1608(60,0,english[32],0);	
	}
	
	if(Main_KEY_State == 1 || Main_KEY_State == 2 )
	{ */
	
	/*	
		if( ValvePosValue > 1030 )
		{
			//超限报错
			//guzhang
			//FaultTotal  = FaultTotal | FaultJourneyOverMask;
		}
		else */
		if(  ValvePosValue > 995 )
		{
			temp = 1000;
		}
		else if( ValvePosValue <= 995 && ValvePosValue >= 0)
		{
			temp = ValvePosValue;
			//FaultTotal  = FaultTotal & ~FaultJourneyOverMask;
		}
		else if( ValvePosValue <0 &&  ValvePosValue >= -25)
		{
			temp = 0 ;	
			FaultTotal  = FaultTotal & ~FaultJourneyOverMask;
		}
		else if( ValvePosValue < -15 )
		{
			//guzhang
			//阀位超限，小于0%的不报错了
			//FaultTotal  = FaultTotal | FaultJourneyOverMask;			
		}

		if( (FaultTotal & FaultJourneyOverMask) == 0 )
		{
			if(temp > 999)
			{
			   	ascii_1608(40,0,english[temp/1000+18],0);
				ascii_1608(44,0,english[temp%1000/100+18],0);
				ascii_1608(48,0,english[temp%100/10+18],0);
				ascii_1608(56,0,english[temp%10+18],0);
				ascii_1608(52,0,english[31],0);
				ascii_1608(60,0,english[32],0);	
			}else if(temp >= 0){
				ascii_1608(40,0,english[41],0);
				ascii_1608(44,0,english[temp%1000/100+18],0);
				ascii_1608(48,0,english[temp%100/10+18],0);
				ascii_1608(56,0,english[temp%10+18],0);
				ascii_1608(52,0,english[31],0);
				ascii_1608(60,0,english[32],0);	
			}
		}
		else
		{
			ascii_1608(40,0,english[41],0);
			ascii_1608(44,0,english[33],0);
			ascii_1608(48,0,english[34],0);
			ascii_1608(52,0,english[35],0);
			ascii_1608(56,0,english[36],0);
			ascii_1608(60,0,english[37],0);
		}
	//}  

}

//显示参数  在第四行
void ParaShow(int x)
{
	//ascii_1608(16,48,english[x/100000000+18],0);
	//ascii_1608(20,48,english[x%100000000/10000000+18],0);
	//ascii_1608(24,48,english[x%10000000/10000+18],0);
	ascii_1608(24,48,english[x%100000/10000+18],0);	
	ascii_1608(28,48,english[x%10000/1000+18],0);
	ascii_1608(32,48,english[x%1000/100+18],0);
	ascii_1608(36,48,english[x%100/10+18],0);
	ascii_1608(40,48,english[x%10+18],0);	
}

//页面3area0 基本参数设置  开向限位：行程
void page3area0atom0op0(){
	ParaArray[OpenDirLimit] = journey;
}
//页面3area0 基本参数设置  开向限位：转矩
void page3area0atom0op1(){
	ParaArray[OpenDirLimit] = torque;
}

//页面3area0 基本参数设置  关向限位：行程
void page3area0atom1op0(){	
	ParaArray[CloseDirLimit] = journey;
}
//页面3area0 基本参数设置  关向限位：转矩
void page3area0atom1op1(){	
	ParaArray[CloseDirLimit] = torque;
}
//页面3area0 基本参数设置   中间位置，这里是没有选项的，应该是一个数字输入函数   3位数
//void page3area0atom2op0(){}

void page3area0atom2inop(){
	ParaArray[MidPosition1] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

void page3area0atom2op1(){	}
//页面3area3 基本参数设置   开向设置转矩，这里是没有选项的，应该是一个数字输入函数
void page3area0atom3inop(){
	ParaArray[OpenDirJour] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

//页面3area4 基本参数设置   开向设置转矩，这里是没有选项的，应该是一个数字输入函数
void page3area0atom4inop(){
	ParaArray[CloseDirJour] = input_op[0].num*100 + input_op[1].num*10 + input_op[2].num;		
}

//页面3area5 基本参数设置   紧急控制方式：保位
void page3area0atom5op0(){
	ParaArray[EmergencyMode] = hold;	
}
//页面3area5 基本参数设置   紧急控制方式：转矩位置
void page3area0atom5op1(){
	ParaArray[EmergencyMode] = assign;
}


//控制参数
//wz 因为area1现在更新为 终端，而原来的函数挂钩没有改变，所以下面的函数名先不改
//页面3area1 控制参数设置 ：就地  这两个应该要进入subarea
void page3area1atom0op0(){	
	SubAreaMode = 1;
	SubAreaNum = 0;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;
}

//页面3area1 控制参数设置   打开subarea
void page3area1atom0op1(){	
	SubAreaMode = 1;
	SubAreaNum = 1;
	AreaChageFlag = 0;  //消除显示两遍subarea
	SubAreaChangeFlag = 1;
}
//就地控制参数设置，选为脉冲式（将脉冲式与维持式的实现换一下，直接换这里。）
void page3area1atom0sub0op0(){
	ParaArray[LocallyMode] = hold_mode;//pluse_mode ;	
}
//就地控制参数设置，选为维持式 （将脉冲式与维持式的实现换一下，直接换这里。//不光这里换了一下，显示的地方也要换）
void page3area1atom0sub0op1(){
	ParaArray[LocallyMode] = pluse_mode;
}

//远程控制参数设置，选为脉冲式	 （将脉冲式与维持式的实现换一下，直接换这里。）
void page3area1atom0sub1op0(){
	ParaArray[FarawayMode] = hold_mode;
}
//远程控制参数选择，选为维持式	（将脉冲式与维持式的实现换一下，直接换这里。）
void page3area1atom0sub1op1(){
	ParaArray[FarawayMode] = pluse_mode;	
}
//远程控制参数选择，选为计算机式
void page3area1atom0sub1op2(){
	ParaArray[FarawayMode] = pc_mode;
}
//远程控制参数选择，选为模拟量式
void page3area1atom0sub1op3(){
	ParaArray[FarawayMode] = analog_mode;
}


//页面3area2 I/O参数设置    开关信号IOSetRec；常开
void page3area2atom0op0(){	
	ParaArray[OutNormalState0 + IOSetRec] = OutNormalState_open;
}
//页面3area2 I/O参数设置    开关信号IOSetRec；常闭
void page3area2atom0op1(){	
	ParaArray[OutNormalState0 + IOSetRec] = OutNormalState_close;
}

//页面3area2 I/O参数设置    开关信号1；表示  这里应该要进入6个subarea的选择

//开关信号IOSetRec；总故障
void page3area2atom1op(){	
	ParaArray[OutExpState0 + IOSetRec] = TotalFault;
}
//远方故障
void page3area2atom2op(){	
	ParaArray[OutExpState0 + IOSetRec] = FarawayFault;
}		  
//缺相故障
void page3area2atom3op(){	
	ParaArray[OutExpState0 + IOSetRec] = LackPhaseFault;
}
//阀位故障
void page3area2atom4op(){	
	ParaArray[OutExpState0 + IOSetRec] = ValveLocationFault;
}
//电机过热
void page3area2atom5op(){	
	ParaArray[OutExpState0 + IOSetRec] = MotorOverHeartFault;
}
//中间位置1
void page3area2atom6op(){	
	ParaArray[OutExpState0 + IOSetRec] = MidPosition1Fault;
}
//中间位置2
void page3area2atom7op(){	
	ParaArray[OutExpState0 + IOSetRec] = MidPosition2Fault;
}
//开向行程限位
void page3area2atom8op(){	
	ParaArray[OutExpState0 + IOSetRec] = OpenDirJourneyLimit;
}
//关向行程限位
void page3area2atom9op(){	
	ParaArray[OutExpState0 + IOSetRec] = CloseDirJourneyLimit;
}
//开向转矩限位
void page3area2atom10op(){	
	ParaArray[OutExpState0 + IOSetRec] = OpenDirTorqueLimit;
}
//关向转矩限位
void page3area2atom11op(){	
	ParaArray[OutExpState0 + IOSetRec] = CloseDirTorqueLimit;
}
//设定值故障
void page3area2atom12op(){	
	ParaArray[OutExpState0 + IOSetRec] = SetValueFault;
}
/*
//页面3area4 标定参数       标定参数 反馈值8mA  
void page3area4atom0inop(){
	ParaArray[Feedback8ma] = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;		
}

//页面3area4 标定参数       标定参数 反馈值16mA   
void page3area4atom1inop(){
	ParaArray[Feedback16ma] = input_op[0].num*1000 + input_op[1].num*100 + input_op[2].num*10 + input_op[3].num;		
} */
//页面3area4 标定参数       标定参数 反馈4mA  记录下反馈4ma对应的编码器值 
void page3area4atom0op(void){								   
	ParaArray[Feedback4maPos] = EncoderData;			
}
//页面3area4 标定参数       标定参数 反馈20mA  记录下反馈4ma对应的编码器值 
void page3area4atom1op(void){
	ParaArray[Feedback20maPos] = EncoderData;
}
//页面3area4 标定参数       标定参数 设定值4mA    设定当前值为4ma
void page3area4atom2op(void){
	int max = 0 ;
	int min = 65535;
	int cnt = 0;
	/*
	for(cnt = 0 ; cnt < 10 ; cnt ++){
		if()
	} */

	//bug 这里应该也要做滤波
	int sum = 0;
	for(cnt = 0 ; cnt < 4 ; cnt++)
		sum = sum + ad654rec[cnt]/4;
	ParaArray[Set4ma] = sum;//TIM1_FREQ;		
}
//页面3area4 标定参数       标定参数 设定值20mA   设定当前值为20ma
void page3area4atom3op(void){
	int cnt = 0;
	int sum = 0;
	for(cnt = 0 ; cnt < 4 ; cnt++)
		sum = sum + ad654rec[cnt]/4;
	ParaArray[Set20ma] = sum;	
}

//终端
//页面3area3atom4op 标定参数  设定当前位置为阀位0%
void page3area3atom0op(){

	//获取编码器当前值   这里先直接用每20ms读取一次的编码器值EncoderData
	ParaArray[SetValveMin] = EncoderData;
	//不应该这样直接操作，应该还是将 EncoderData 存入FLASH中的 SetValveMin_ , 然后SetValveMin，每次开机或者设置完毕之后从FLASH里面更新过来

}
//页面3area3op5 标定参数  设定当前位置为阀位100%  
void page3area3atom1op(){	
	ParaArray[SetValveMax] = EncoderData;

	ParaArray[SelfLearnRec] = NoHavedSelfLearn ;	
}  


//页面3area4 其他设置       恢复出厂设置；不恢复
void page3area4atom0op0(){	}
//页面3area4 其他设置       恢复出厂设置；恢复 
void page3area4atom0op1(){	}
//页面3area4 其他设置       报警清除；不清除
void page3area4atom1op0(){	}
//页面3area4 其他设置       报警清除；清除 
void page3area4atom1op1(){
	FaultTotal = 0;
}

//页面3area5 其他设置       修改密码：不修改 
void page3area5atom2op0(){}
//页面3area5 其他设置       修改密码：修改 
void page3area5atom2op1(){
	SubAreaMode = 1;
	SubAreaNum = 1;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;	
}

//其他设置里面的进入自学习模式
void page3area5atom3op(){
	SubAreaMode = 1;
	SubAreaNum = 0;
	AreaChageFlag = 0;
	SubAreaChangeFlag = 1;		
}

void page3area5atom0op0(){ } //自整定退出



