#include "bsp_timer.h"
#include "key.h"
/**
��ʱ���ж�ʵ�����ò���
1�����ö�ʱ��������������             HAL_TIM_Base_Init()
2����ʱ������MSP��ʼ��            HAL_TIM_Base_MspInit()     ����NVIC��CLOCK��
3��ʹ�ܸ����жϲ�����������        HAL_TIM_Base_Start_IT()
4���������ȼ���ʹ���ж�            HAL_NVIC_SetPriority()�� HAL_NVIC_EnableIRQ()
5����д�жϷ�����                TIMx_IRQHandler()��       HAL_TIM_IRQHandler()
6����д��ʱ�������жϻص�����        HAL_TIM_PeriodElapsedCallback()
**/

/**
��ʱƵ��= 72M/ [(PSC+1)(ARR+1)]   
				1s  1HZ   
				PSC = 7199		ARR = 9999
				1ms  1000HZ
				PSC = 71      ARR = 999
				72 000 000  /  [(71+1)(999+1)]  = 1000HZ
**/

#include "stdio.h"
/**
* ��ʱ�������жϻص��������û��ض���
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)//����ĸ���ʱ��
  {
		timer_ticks();
		key_meter();
		//printf("nn");
	}
}

/**
 * @brief �жϳ�ʼ��
 *
 */
void Timer_IT_init()
{
	HAL_TIM_Base_Start_IT(&htim6);
}


/**
 * @brief ʱ���־λ
 *  ���ڶ�ʱ�� 1ms�ж�����
 */
struct Time_flagTypDef time_flag;
static uint8_t ti_1 = 0;
static uint8_t ti_10 = 0;
static uint8_t ti_50 = 0;
static uint8_t ti_100 = 0;
static uint8_t ti_1000 = 0;
static uint8_t ti_500 = 0;
void ti_flag(void) //�����ж� 1ms����
{
  ti_1++;
	time_flag.t1ms = 1;
	if (ti_1 == 10)
	{
		ti_1 = 0;
		ti_10++;
		time_flag.t10ms = 1;
		if (ti_10 == 5)
		{	
			ti_10 = 0;
			time_flag.t50ms = 1;
			ti_50++;
			if(ti_50 == 2)
			{
				ti_50 = 0;
				ti_100++;
				time_flag.t100ms = 1;
				if(ti_100 == 5)
				{
					ti_100 = 0;
					ti_500++;
					time_flag.t500ms = 1;
					if(ti_500 == 2)
					{
						ti_500 = 0;
						time_flag.t1s = 1;
						ti_1000++;
						if(ti_1000 == 5)
						{
							time_flag.t5s = 1;
							ti_1000 = 0;					
						}
					}
				}
			}
		}
	}
}
  





/**
*	@brief ���Գ���
*/
void time_test()
{

}	


