/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-03-26 20:09:36
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-03-30 13:20:15
 * @FilePath: \MDK-ARMf:\32\hal_experiment\CubeMX\key\project\Core\BSP\bsp_usart.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "bsp_usart.h"
//#include "lay_in.h"
#include <string.h>

uint8_t rx_dat;//���ڽ��յ����ݻ�����
uint8_t rx_Buf[200];
uint8_t rx_flag = 0;//���ܵı�־

uint8_t instruct_flag;//�������ָ��

uint8_t ch;
uint8_t ch_r;

uint8_t n;//��������λ

//��д�������,�ض���printf����������
/*fputc*/
/***/
int fputc(int c, FILE * f)
{
	ch=c;
	HAL_UART_Transmit(&huart1,&ch,1,1000);//���ʹ���
	return c;
}
 

// 
////�ض���scanf����������
///*fgetc*/
//int fgetc(FILE * F)
//{
//	HAL_UART_Receive(&huart1,&ch_r,1,0xffff);//����
//	return ch_r;
//}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//uint8_t pData[20]
	if(huart == &huart1 )
	{
			//rx_flag = 1;//���ܵ�����
			rx_Buf[n] = rx_dat;
			n++;
			if(rx_dat ==  '\n')
			{
					rx_Buf[n-1] = '\0';
					n = 0;
				/*****����******/
					if(strcmp((char *)rx_Buf,"dat")==0)
					{
							instruct_flag = 1;
					}	
					if(strcmp((char *)rx_Buf,"env")==0)//environment
					{
							instruct_flag = 2;
					}
			/****************/
	
			}		
			//__HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_RXNE);
	}
	HAL_UART_Receive_IT(&huart1,&rx_dat,1);
	
}



void Uart_IT_Init()
{
	//__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);//���ܷǿ��ж�
	//__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//���տ����ж�
	HAL_UART_Receive_IT(&huart1,&rx_dat,1);
}


//���ݴ�ӡ
//������ѭ������
void instruct_dat_printf()
{
	if(instruct_flag == 1)
	{
		instruct_flag = 0;
		//EE_20_dat_read();
	}
	if(instruct_flag == 2)
	{
		instruct_flag = 0;
		//environment_out();
		
	}
}

