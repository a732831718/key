### 介绍
 本框架实现了短按，长按，持续，双击，可随意添加按钮

### 使用说明

 * ***********使用说明***************

本框架基于于stm32 hal库 （可通过修改移植其他库函数）

1.将 key_meter(); 以1ms调用一次

2.将key_canf();   放入主函数循环调用 

3.创建句柄 如  
    
 ` key_TypDef key1;`

4.创建处理函数 模板：

```
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
```

 
 5.创建按钮
```key_create(&key1,GPIOA,GPIO_PIN_4,MODE_SHORT_PRESS|MODE_LONG_PRESS|MODE_CONTINUONS|MODE_DOUBLE_CLICK,KEY_LOW_LEVEL,&key_1);```


在key.h文件里的宏定义修改按键数量，消抖时间，单击长按，以及长按持续触发的间隔时间。


### 关注公众号
![输入图片说明](image.png)
