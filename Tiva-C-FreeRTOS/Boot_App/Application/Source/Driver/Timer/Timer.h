#ifndef   TIMER_H
#define   TIMER_H


void Timer_Init();
static void Start_Timer_0();
void Stop_Timer_0();
void Refresh_Timer_0();
void Timer_scheduler();
static void Clear_Timer_Flag();

void Delay_ms(unsigned int delay_value);

#endif
