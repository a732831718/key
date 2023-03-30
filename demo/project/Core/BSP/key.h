/**	
  ******************************************************************************
  * @file    key.h
  * @author  ������ 
  * @brief   һ�׿�ʵ�ֶ̰���������˫���İ�������
  * @version V1.0
  * @date 	 2023��3��30��
  ******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

#define HW_KEYS_NUM 5 //��������
#define KEY_SHAKE_TIME   10 //����ʱ��ms
#define KEY_LONG_PRESS   500 //����
#define KEY_INTERVAL 200  //���

/**
 * @defgroup ��ƽѡ��
 * @brief ѡ�񰴼�����ʱ�ߵ�ƽ���ǵ͵�ƽ
*/
#define KEY_HIGH_LEVEL GPIO_PIN_SET  //�ߵ�ƽ
#define KEY_LOW_LEVEL GPIO_PIN_RESET //�͵�ƽ


/**
 * @defgroup ģʽѡ��
*/
#define MODE_SHORT_PRESS  0x01  //�̰�
#define MODE_LONG_PRESS   0x02	//����
#define MODE_CONTINUONS   0x03	//������������
#define MODE_DOUBLE_CLICK 0x04	//˫��

/**
 * @brief ��ť���ýṹ��
 */
typedef struct 
{
	GPIO_TypeDef *GPIOx;
	unsigned char Pin;
	unsigned char mode;//ģʽ 0000 0000 ˫�� ���� ���� �̰�
	GPIO_PinState level;//��ƽ
	void (*fun)(unsigned char cmd);
	struct
	{
		unsigned key_state:4;//״̬
		unsigned timer:12;//��ʱ
		unsigned interval_timer:12;//���ʱ��
		unsigned interval_timer_flag:1;//�����־λ
		unsigned sustain_timer:8;//����
		unsigned sustain_timer_flag:1;
		unsigned timer_flag:1;//��ʱ��־λ		
		unsigned key_state_old:1;
		unsigned Value:1;
		unsigned key_count:4;
	}flag;
}key_TypDef;


void key_meter(void);//����
void key_create(key_TypDef *key_dat);//��������
void key_canf(void);

/***
 * ***********ʹ��˵��***************
 * ����ܻ�����stm32 hal�� ����ͨ���޸���ֲ�����⺯����
 * 1.�� key_meter(); ��1ms����һ��
 * 2.��key_canf();   ����������ѭ������ 
 * 3.������� ��  
 * key_TypDef key1;
 * 4.���������� ģ�壺
void key_1(unsigned char cmd)
{
	if(cmd == 1)
	{
		//�����̰�
	}
	if(cmd == 2)
	{
		//����
	}
	if(cmd == 3)
	{
		//������������
	}
	if(cmd == 4)
	{
		//˫��
	}
}
 * 4.��ֵ 
	key1.GPIOx = GPIOA; //ѡ��IO�˿�
	key1.Pin = GPIO_PIN_4;//ѡ������
	key1.mode = 0x08|0x01|0x02|0x04;//ѡ��ģʽ ���Ե���MODE_SHORT_PRESS MODE_LONG_PRESS MODE_CONTINUONS MODE_DOUBLE_CLICK  
	key1.level = KEY_LOW_LEVEL; //ѡ�����Ǹߵ�ƽ���ǵ͵�ƽ
	key1.fun = key_1;//�������ĺ�����ֵ
	key_create(&key1);
 *
 *5.���������¼�
 *key_create(key1);
*/


#endif

