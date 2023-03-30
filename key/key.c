/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑     永不宕机     永无BUG
 * 
**
  ******************************************************************************
  * @file    key.c
  * @author  代码瑶(雨啸青锋)
  * @brief   实现短按长按持续双击的按键驱动
  * @version V1.0
  *	@date 	 2023年3月30日
  ******************************************************************************/
#include "key.h"
#include "stdio.h"


//节拍计时
static unsigned int meter_tim;

//储存 按钮对象的指针的数组
static key_TypDef *key_info[HW_KEYS_NUM];

//储存按钮数量
static unsigned int key_num = 0;//按键数量


/**
 * @brief 按键创建
 * 
 * @return unsigned char 
 * 
 */  
void key_create(key_TypDef *key_dat)
{
	GPIO_InitTypeDef GPIO_InitStruct;//引脚初始化
	GPIO_InitStruct.Pin = key_dat -> Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(key_dat -> GPIOx, &GPIO_InitStruct);
	
	key_info[key_num] = key_dat;
	key_num++;
}


/**
 * @brief 计数
 *	1ms调用一次
 */
void key_meter()
{
	unsigned char n;
	meter_tim++;

	for(n = 0;n <	key_num;n++)//计时
	{
		if(key_info[n] -> flag.timer_flag == 1)//如果timer标志位++
		{
			key_info[n] -> flag.timer++;
			if(key_info[n] -> flag.timer != 0xFFE)
				 key_info[n] -> flag.timer++;//间隔
		}
		if(key_info[n] -> flag.interval_timer_flag == 1)
		{
			if(key_info[n] -> flag.interval_timer != 0xFFE)
				 key_info[n] -> flag.interval_timer++;//间隔
		}
		if(key_info[n] -> flag.sustain_timer_flag == 1)
		{
			if(key_info[n] -> flag.sustain_timer != 0xFE)
				 key_info[n] -> flag.sustain_timer++;//间隔
		}
	}
}





/**
 * @brief 按键处理
 * @param key_dat 
 * @return 
 */
unsigned char key_drive(key_TypDef *key_dat)
{
	if (HAL_GPIO_ReadPin(key_dat -> GPIOx,key_dat -> Pin) == key_dat->level)//键盘按下
	{
		if(key_dat->flag.key_state_old == 0)//判断上一次按钮有没有被按下 如果没有 就是按下瞬间
		{
			key_dat->flag.timer = 0;//定时时间清0
			key_dat->flag.timer_flag = 1;//开始定时
			if((key_dat->mode&0x08) == 0x08)//有开启多按
			{
				key_dat->flag.interval_timer_flag = 0;//计时停止
			
				if(key_dat->flag.interval_timer <=  KEY_INTERVAL)
				{
					key_dat->flag.key_count++;
					
				}
				else//超时
				{
					//key_dat->flag.interval_timer = 0;//停止计时 清0
					key_dat->flag.key_count = 0;//次数清0
				}
				key_dat->flag.interval_timer = 0;
			}
		}
		if(key_dat->flag.key_state_old == 1)//如果上一次按键有按下 长按
		{
			if((key_dat->mode & 0x04) == 0x04) //如果模式有开启连续触发
			{
				/***************/
				//printf("%u\n",key_dat->flag.timer);
				if(key_dat -> flag.timer >= KEY_LONG_PRESS*2)//达到持续（长按）时间 * 2
				{
					key_dat -> flag.sustain_timer_flag = 1;
					//	printf("%u\n",key_dat->flag.sustain_timer);
					if(key_dat -> flag.sustain_timer > 100)//持续触发
					{
					
						key_dat -> flag.sustain_timer = 0;
						key_dat->flag.key_state = 3;
						/* 触发连续事件*/
						(key_dat ->fun)(key_dat->flag.key_state);
						key_dat->flag.key_state = 0;
					}
					//return 0;
				}
			}
		}
		key_dat->flag.key_state_old  = 1;
		
	}
	
	if (HAL_GPIO_ReadPin(key_dat -> GPIOx,key_dat -> Pin) != key_dat->level)//如果按钮没有按下
	{
		key_dat->flag.timer_flag = 0;//停止计时
		
		if(key_dat->flag.key_state_old == 1)//判断上一次按钮有没有被按下 如果有  松开瞬间
		{
				
			if((key_dat->mode & 0x01) == 1) //如果模式有开启短按
			{	
		
					if(key_dat->flag.timer >= KEY_SHAKE_TIME && key_dat->flag.timer < KEY_LONG_PRESS)//消抖完成
					{
						key_dat->flag.key_state = 1;//短按	
					}
			}
			
			if((key_dat->mode & 0x02) == 0x02) //如果模式有开启长按
			{
				if(key_dat -> flag.timer >= KEY_LONG_PRESS)//达到长按时间
				{	
					if((key_dat->mode & 0x04) == 0x04)
					{
						if(key_dat -> flag.timer < KEY_LONG_PRESS*2)
						{
							key_dat->flag.key_state = 2;//长按标志位
						}
					}					
					else 
						key_dat->flag.key_state = 2;//长按标志位
				}
		
			}
			if((key_dat->mode&0x04) == 0x04)//如果模式有开启持续
			{
					key_dat -> flag.interval_timer = 0;
					key_dat -> flag.sustain_timer_flag = 0;
					
			}
			
			if((key_dat->mode & 0x08) == 0x08)//如果模式开启双击 间隔开始
			{
				//printf("%u\n",key_dat->flag.key_count);
					key_dat->flag.interval_timer = 0;//清0
					key_dat->flag.interval_timer_flag = 1;//开始间隔计时
					
					if(key_dat->flag.key_count == 2)
					{
						key_dat->flag.key_state = 4;//状态4双击		
						key_dat->flag.key_count = 0;
					}
				}
		}
		
		if(key_dat->flag.key_state_old == 0) //如果没有按下
		{
			
			if((key_dat->mode&0x08) == 0x08)//如果有开启多按
			{	
				if(key_dat->flag.interval_timer > KEY_INTERVAL)//大于间隔时间
				{
					
					if(key_dat->flag.key_state == 1 || key_dat->flag.key_state == 2 || key_dat->flag.key_state == 4)//状态1短按时候触发事件
					{	
							(key_dat ->fun)(key_dat->flag.key_state);//回调函数
							key_dat->flag.timer = 0;
					}
//					if(key_dat->flag.key_state == 2)//状态2长按时候触发事件
//					{	
//							(key_dat ->fun)(key_dat->flag.key_state);//回调函数
//							key_dat->flag.timer = 0;
//					}
//					if(key_dat->flag.key_state == 4)//双击
//					{
//						(key_dat ->fun)(key_dat->flag.key_state);//回调函数
//					
//					}
					key_dat->flag.key_state = 0;//状态清0
					key_dat->flag.interval_timer = 0;//清0
					key_dat->flag.interval_timer_flag = 0;//标志位清0
					key_dat->flag.key_count = 0;//清0
				}
			}
			else //如果没有开启多按
			{
				if(key_dat->flag.key_state == 1 || key_dat->flag.key_state == 2 || key_dat->flag.key_state == 4)//状态1短按时候触发事件
				{	
					key_dat->flag.timer = 0;
					(key_dat ->fun)(key_dat->flag.key_state);
				}
				
//				if(key_dat->flag.key_state == 2)//状态2长按时候触发事件
//				{	
//						key_dat->flag.timer = 0;
//						(key_dat ->fun)(key_dat->flag.key_state);//回调函数
//				}	
//				if(key_dat->flag.key_state == 4)//双击
//				{						
//						(key_dat ->fun)(key_dat->flag.key_state);//回调函数
//				}
				key_dat->flag.key_state = 0;//状态清0
			}
			
		}
			key_dat->flag.key_state_old = 0;
	}
	
	return 0;
}
	

	
/**
 * @brief 按键扫描
 */
void key_canf()
{
	for(int n = 0;n <	key_num;n++)
	{
		key_drive(key_info[n]);//扫描
	}
}

