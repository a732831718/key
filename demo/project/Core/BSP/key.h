/**	
  ******************************************************************************
  * @file    key.h
  * @author  代码瑶 
  * @brief   一套可实现短按长按持续双击的按键驱动
  * @version V1.0
  * @date 	 2023年3月30日
  ******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

#define HW_KEYS_NUM 5 //按键数量
#define KEY_SHAKE_TIME   10 //抖动时间ms
#define KEY_LONG_PRESS   500 //长按
#define KEY_INTERVAL 200  //间隔

/**
 * @defgroup 电平选择
 * @brief 选择按键按下时高电平还是低电平
*/
#define KEY_HIGH_LEVEL GPIO_PIN_SET  //高电平
#define KEY_LOW_LEVEL GPIO_PIN_RESET //低电平


/**
 * @defgroup 模式选择
*/
#define MODE_SHORT_PRESS  0x01  //短按
#define MODE_LONG_PRESS   0x02	//长按
#define MODE_CONTINUONS   0x03	//长按持续触发
#define MODE_DOUBLE_CLICK 0x04	//双击

/**
 * @brief 按钮配置结构体
 */
typedef struct 
{
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

/***
 * ***********使用说明***************
 * 本框架基于于stm32 hal库 （可通过修改移植其他库函数）
 * 1.将 key_meter(); 以1ms调用一次
 * 2.将key_canf();   放入主函数循环调用 
 * 3.创建句柄 如  
 * key_TypDef key1;
 * 4.创建处理函数 模板：
void key_1(unsigned char cmd)
{
	if(cmd == 1)
	{
		//单击短按
	}
	if(cmd == 2)
	{
		//长按
	}
	if(cmd == 3)
	{
		//长按持续触发
	}
	if(cmd == 4)
	{
		//双击
	}
}
 * 4.赋值 
	key1.GPIOx = GPIOA; //选择IO端口
	key1.Pin = GPIO_PIN_4;//选择引脚
	key1.mode = 0x08|0x01|0x02|0x04;//选择模式 可以调用MODE_SHORT_PRESS MODE_LONG_PRESS MODE_CONTINUONS MODE_DOUBLE_CLICK  
	key1.level = KEY_LOW_LEVEL; //选择按下是高电平还是低电平
	key1.fun = key_1;//将创建的函数赋值
	key_create(&key1);
 *
 *5.创建按键事件
 *key_create(key1);
*/


#endif

