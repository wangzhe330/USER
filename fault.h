#ifndef _FAULT_H_
#define _FAULT_H_
#include "stdint.h"

#define FaultSigBreakMask                ((uint16_t)0x0001)  
#define FaultSigOverMask                 ((uint16_t)0x0002)
#define FaultJourneyOverMask			 ((uint16_t)0x0004)	


extern int FaultTotal;
extern int FaultShowFlag;
//¹ÊÕÏ¼ì²âº¯Êý
void FaultDetect(void);

void FaultShow(int x );



#endif
