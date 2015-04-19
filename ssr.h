#ifndef SSR_H
#define SSR_H

void SSRPowerOff(void);
void SSRPowerOn(void);

void SSR_GPIO_Config(void);
void SSR0RUN(void);
void SSR1RUN(void);
void SSRSTOP(void);

void OpenValve(void);
void CloseValve(void);

#endif
