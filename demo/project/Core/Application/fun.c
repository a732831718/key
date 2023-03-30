#include "fun.h"

struct Timer timer_1;
struct Timer timer_2;

void timer1_callback(void *arg)
{
    //printf("\r\n");
		key_canf();//…®√Ë
		//printf("\r\n");

}

void timer2_callback(void *arg)
{
  //  printf("timer2 timeout!\r\n");
}



key_TypDef key1;
key_TypDef key2;


void key_1(unsigned char cmd)
{
	if(cmd == 1)
	{
		 printf("key1\r\n");
	}
	if(cmd == 2)
	{
		 printf("hehe\r\n");
	}
	if(cmd == 3)
	{
		 printf("h1e\r\n");
	}
	if(cmd == 4)
	{
		 printf("hee\r\n");
		
	}
	
}

void key_2(unsigned char cmd)
{
	if(cmd == 1)
	{
		 printf("key2\r\n");
	}
	if(cmd == 2)
	{
		 printf("heh2\r\n");
	}
	if(cmd == 3)
	{
		 printf("h1e\r\n");
	}
	if(cmd == 4)
	{
		 printf("hee2\r\n");
		
	}
	
}




void start_Init()
{
	Timer_IT_init();
	timer_init(&timer_1, timer1_callback, 0, 2, NULL); 
	timer_start(&timer_1);
	
	timer_init(&timer_2, timer2_callback, 1000, 2000, NULL); 
	timer_start(&timer_2);

	key1.GPIOx = GPIOA;
	key1.Pin = GPIO_PIN_4;
	key1.mode = 0x08|0x01|0x02|0x04;
	key1.level = KEY_LOW_LEVEL;
	key1.fun = key_1;
	key_create(&key1);


	key2.GPIOx = GPIOA;
	key2.Pin = GPIO_PIN_5;
	key2.mode = 0x08|0x01|0x02|0x04;
	key2.level = KEY_LOW_LEVEL;
	key2.fun = key_2;
	key_create(&key2);
}


void fun()
{

	timer_loop();
	

	
}



