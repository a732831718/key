/**	
  ******************************************************************************
  * @file    key.h
	* @author  代码瑶(雨啸青锋)
  * @brief   一套可实现短按长按持续双击的按键驱动
  * @version V1.0
  * @date 	 2023年3月30日
  ******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"


#define KEY_HIGH_LEVEL GPIO_PIN_SET
#define KEY_LOW_LEVEL GPIO_PIN_RESET


#define HW_KEYS_NUM 5 //按键数量
#define KEY_SHAKE_TIME   10 //抖动时间ms
#define KEY_LONG_PRESS   500 //长按
#define KEY_INTERVAL 200  //间隔


/**
 * @brief 结构体
 */
typedef struct 
{
	unsigned char key_id;
	GPIO_TypeDef *GPIOx;
	unsigned char Pin;
	unsigned char mode;//模式 0000 0000 双击 连续 长按 短按
	GPIO_PinState level;//电平
	void (*fun)(unsigned char cmd);
	struct
	{
		unsigned key_state:4;//状态
		unsigned timer:12;//计时
		unsigned interval_timer:12;//间隔时间
		unsigned interval_timer_flag:1;//间隔标志位
		unsigned sustain_timer:8;//持续
		unsigned sustain_timer_flag:1;
		unsigned timer_flag:1;//定时标志位		
		unsigned key_state_old:1;
		unsigned Value:1;
		unsigned key_count:4;
	}flag;
}key_TypDef;


void key_meter(void);//计数
void key_create(key_TypDef *key_dat);//创建按键
void key_canf(void);


#endif

