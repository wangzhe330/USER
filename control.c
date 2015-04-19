#include <control.h>

int SPEED = 0;	//两次编码器的差值
int ValveError = 0;  //记录停下之后的差值

//1表示电机停转信号给出，0表示电机转动信号给出
int STOP_FLAG = 1; //停止信号给出之后的标志位,给出停止信号之后，每20ms检测一下，停止没有，如果电机停下了，则记录下实际位置和目标值的差值

int FarawaySM = 0; //远程模式的状态机
//struct 

//void 

