#include "bsp_timer.h"
#include "key.h"
/**
定时器中断实验配置步骤
1，配置定时器基础工作参数             HAL_TIM_Base_Init()
2，定时器基础MSP初始化            HAL_TIM_Base_MspInit()     配置NVIC、CLOCK等
3，使能更新中断并启动计数器        HAL_TIM_Base_Start_IT()
4，设置优先级，使能中断            HAL_NVIC_SetPriority()、 HAL_NVIC_EnableIRQ()
5，编写中断服务函数                TIMx_IRQHandler()等       HAL_TIM_IRQHandler()
6，编写定时器更新中断回调函数        HAL_TIM_PeriodElapsedCallback()
**/

/**
定时频率= 72M/ [(PSC+1)(ARR+1)]   
				1s  1HZ   
				PSC = 7199		ARR = 9999
				1ms  1000HZ
				PSC = 71      ARR = 999
				72 000 000  /  [(71+1)(999+1)]  = 1000HZ
**/

#include "stdio.h"
/**
* 定时器更新中断回调函数，用户重定义
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)//检测哪个定时器
  {
		timer_ticks();
		key_meter();
		//printf("nn");
	}
}

/**
 * @brief 中断初始化
 *
 */
void Timer_IT_init()
{
	HAL_TIM_Base_Start_IT(&htim6);
}


/**
 * @brief 时间标志位
 *  放在定时器 1ms中断里面
 */
struct Time_flagTypDef time_flag;
static uint8_t ti_1 = 0;
static uint8_t ti_10 = 0;
static uint8_t ti_50 = 0;
static uint8_t ti_100 = 0;
static uint8_t ti_1000 = 0;
static uint8_t ti_500 = 0;
void ti_flag(void) //放在中断 1ms里面
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
*	@brief 测试程序
*/
void time_test()
{

}	


