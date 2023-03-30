/**	
  ******************************************************************************
  * @file    key.h
	* @author  ������(��Х���)
  * @brief   һ�׿�ʵ�ֶ̰���������˫���İ�������
  * @version V1.0
  * @date 	 2023��3��30��
  ******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"


#define KEY_HIGH_LEVEL GPIO_PIN_SET
#define KEY_LOW_LEVEL GPIO_PIN_RESET


#define HW_KEYS_NUM 5 //��������
#define KEY_SHAKE_TIME   10 //����ʱ��ms
#define KEY_LONG_PRESS   500 //����
#define KEY_INTERVAL 200  //���


/**
 * @brief �ṹ��
 */
typedef struct 
{
	unsigned char key_id;
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


#endif

