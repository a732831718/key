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
 *            ���汣��     ����崻�     ����BUG
 * 
**
  ******************************************************************************
  * @file    key.c
  * @author  ������(��Х���)
  * @brief   ʵ�ֶ̰���������˫���İ�������
  * @version V1.0
  *	@date 	 2023��3��30��
  ******************************************************************************/
#include "key.h"
#include "stdio.h"


//���ļ�ʱ
static unsigned int meter_tim;

//���� ��ť�����ָ�������
static key_TypDef *key_info[HW_KEYS_NUM];

//���水ť����
static unsigned int key_num = 0;//��������


/**
 * @brief ��������
 * 
 * @return unsigned char 
 * 
 */  
void key_create(key_TypDef *key_dat)
{
	GPIO_InitTypeDef GPIO_InitStruct;//���ų�ʼ��
	GPIO_InitStruct.Pin = key_dat -> Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(key_dat -> GPIOx, &GPIO_InitStruct);
	
	key_info[key_num] = key_dat;
	key_num++;
}


/**
 * @brief ����
 *	1ms����һ��
 */
void key_meter()
{
	unsigned char n;
	meter_tim++;

	for(n = 0;n <	key_num;n++)//��ʱ
	{
		if(key_info[n] -> flag.timer_flag == 1)//���timer��־λ++
		{
			key_info[n] -> flag.timer++;
			if(key_info[n] -> flag.timer != 0xFFE)
				 key_info[n] -> flag.timer++;//���
		}
		if(key_info[n] -> flag.interval_timer_flag == 1)
		{
			if(key_info[n] -> flag.interval_timer != 0xFFE)
				 key_info[n] -> flag.interval_timer++;//���
		}
		if(key_info[n] -> flag.sustain_timer_flag == 1)
		{
			if(key_info[n] -> flag.sustain_timer != 0xFE)
				 key_info[n] -> flag.sustain_timer++;//���
		}
	}
}





/**
 * @brief ��������
 * @param key_dat 
 * @return 
 */
unsigned char key_drive(key_TypDef *key_dat)
{
	if (HAL_GPIO_ReadPin(key_dat -> GPIOx,key_dat -> Pin) == key_dat->level)//���̰���
	{
		if(key_dat->flag.key_state_old == 0)//�ж���һ�ΰ�ť��û�б����� ���û�� ���ǰ���˲��
		{
			key_dat->flag.timer = 0;//��ʱʱ����0
			key_dat->flag.timer_flag = 1;//��ʼ��ʱ
			if((key_dat->mode&0x08) == 0x08)//�п����ఴ
			{
				key_dat->flag.interval_timer_flag = 0;//��ʱֹͣ
			
				if(key_dat->flag.interval_timer <=  KEY_INTERVAL)
				{
					key_dat->flag.key_count++;
					
				}
				else//��ʱ
				{
					//key_dat->flag.interval_timer = 0;//ֹͣ��ʱ ��0
					key_dat->flag.key_count = 0;//������0
				}
				key_dat->flag.interval_timer = 0;
			}
		}
		if(key_dat->flag.key_state_old == 1)//�����һ�ΰ����а��� ����
		{
			if((key_dat->mode & 0x04) == 0x04) //���ģʽ�п�����������
			{
				/***************/
				//printf("%u\n",key_dat->flag.timer);
				if(key_dat -> flag.timer >= KEY_LONG_PRESS*2)//�ﵽ������������ʱ�� * 2
				{
					key_dat -> flag.sustain_timer_flag = 1;
					//	printf("%u\n",key_dat->flag.sustain_timer);
					if(key_dat -> flag.sustain_timer > 100)//��������
					{
					
						key_dat -> flag.sustain_timer = 0;
						key_dat->flag.key_state = 3;
						/* ���������¼�*/
						(key_dat ->fun)(key_dat->flag.key_state);
						key_dat->flag.key_state = 0;
					}
					//return 0;
				}
			}
		}
		key_dat->flag.key_state_old  = 1;
		
	}
	
	if (HAL_GPIO_ReadPin(key_dat -> GPIOx,key_dat -> Pin) != key_dat->level)//�����ťû�а���
	{
		key_dat->flag.timer_flag = 0;//ֹͣ��ʱ
		
		if(key_dat->flag.key_state_old == 1)//�ж���һ�ΰ�ť��û�б����� �����  �ɿ�˲��
		{
				
			if((key_dat->mode & 0x01) == 1) //���ģʽ�п����̰�
			{	
		
					if(key_dat->flag.timer >= KEY_SHAKE_TIME && key_dat->flag.timer < KEY_LONG_PRESS)//�������
					{
						key_dat->flag.key_state = 1;//�̰�	
					}
			}
			
			if((key_dat->mode & 0x02) == 0x02) //���ģʽ�п�������
			{
				if(key_dat -> flag.timer >= KEY_LONG_PRESS)//�ﵽ����ʱ��
				{	
					if((key_dat->mode & 0x04) == 0x04)
					{
						if(key_dat -> flag.timer < KEY_LONG_PRESS*2)
						{
							key_dat->flag.key_state = 2;//������־λ
						}
					}					
					else 
						key_dat->flag.key_state = 2;//������־λ
				}
		
			}
			if((key_dat->mode&0x04) == 0x04)//���ģʽ�п�������
			{
					key_dat -> flag.interval_timer = 0;
					key_dat -> flag.sustain_timer_flag = 0;
					
			}
			
			if((key_dat->mode & 0x08) == 0x08)//���ģʽ����˫�� �����ʼ
			{
				//printf("%u\n",key_dat->flag.key_count);
					key_dat->flag.interval_timer = 0;//��0
					key_dat->flag.interval_timer_flag = 1;//��ʼ�����ʱ
					
					if(key_dat->flag.key_count == 2)
					{
						key_dat->flag.key_state = 4;//״̬4˫��		
						key_dat->flag.key_count = 0;
					}
				}
		}
		
		if(key_dat->flag.key_state_old == 0) //���û�а���
		{
			
			if((key_dat->mode&0x08) == 0x08)//����п����ఴ
			{	
				if(key_dat->flag.interval_timer > KEY_INTERVAL)//���ڼ��ʱ��
				{
					
					if(key_dat->flag.key_state == 1 || key_dat->flag.key_state == 2 || key_dat->flag.key_state == 4)//״̬1�̰�ʱ�򴥷��¼�
					{	
							(key_dat ->fun)(key_dat->flag.key_state);//�ص�����
							key_dat->flag.timer = 0;
					}
//					if(key_dat->flag.key_state == 2)//״̬2����ʱ�򴥷��¼�
//					{	
//							(key_dat ->fun)(key_dat->flag.key_state);//�ص�����
//							key_dat->flag.timer = 0;
//					}
//					if(key_dat->flag.key_state == 4)//˫��
//					{
//						(key_dat ->fun)(key_dat->flag.key_state);//�ص�����
//					
//					}
					key_dat->flag.key_state = 0;//״̬��0
					key_dat->flag.interval_timer = 0;//��0
					key_dat->flag.interval_timer_flag = 0;//��־λ��0
					key_dat->flag.key_count = 0;//��0
				}
			}
			else //���û�п����ఴ
			{
				if(key_dat->flag.key_state == 1 || key_dat->flag.key_state == 2 || key_dat->flag.key_state == 4)//״̬1�̰�ʱ�򴥷��¼�
				{	
					key_dat->flag.timer = 0;
					(key_dat ->fun)(key_dat->flag.key_state);
				}
				
//				if(key_dat->flag.key_state == 2)//״̬2����ʱ�򴥷��¼�
//				{	
//						key_dat->flag.timer = 0;
//						(key_dat ->fun)(key_dat->flag.key_state);//�ص�����
//				}	
//				if(key_dat->flag.key_state == 4)//˫��
//				{						
//						(key_dat ->fun)(key_dat->flag.key_state);//�ص�����
//				}
				key_dat->flag.key_state = 0;//״̬��0
			}
			
		}
			key_dat->flag.key_state_old = 0;
	}
	
	return 0;
}
	

	
/**
 * @brief ����ɨ��
 */
void key_canf()
{
	for(int n = 0;n <	key_num;n++)
	{
		key_drive(key_info[n]);//ɨ��
	}
}

