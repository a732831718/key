#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_


/*CuMx ���ɵ� ��ʱ����ʼ���ļ�*/
#include "tim.h" 
#include "multi_timer.h"

//HAL_TIM_Base_Start_IT �ж�ʹ��

/**ʱ���־λ flag**/
struct Time_flagTypDef
{
    unsigned
    t1ms:1,
    t10ms:1,
		t50ms:1,
    t100ms:1,
		t500ms:1,
    t1s:1,
    t5s:1;
};
extern struct Time_flagTypDef time_flag;

void ti_flag(void);
void Timer_IT_init(void);
#endif
