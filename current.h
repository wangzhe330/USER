#ifndef CURRENT_H
#define CURRENT_H

extern float current_in_val;
extern int current_set_valve_val;
extern int ValveSetVal;

extern int ValveSetValTemp[25];
extern int ValveSetValTempCnt ;
extern int ValveSetExecuteFlag ;

extern int ad654cnt;
#define ad654len 50

//阀位反馈 输出电流
void ValveFreedback(int v);

int current_out(float x);
float get_current_in(void);
int current_to_valve(void);

#endif
