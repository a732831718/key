### 使用说明
本框架基于于stm32 hal库 （可通过修改移植其他库函数）

1.将 key_meter(); 以1ms调用一次

2.将key_canf();   放入主函数循环调用  

3.创建句柄 如  
`key_TypDef key1;`

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

4.赋值 

```
key1.GPIOx = GPIOA; //选择IO端口
key1.Pin = GPIO_PIN_4;//选择引脚
key1.mode = 0x08|0x01|0x02|0x04;//选择模式 可以调用MODE_SHORT_PRESS MODE_LONG_PRESS MODE_CONTINUONS MODE_DOUBLE_CLICK  
key1.level = KEY_LOW_LEVEL; //选择按下是高电平还是低电平
key1.fun = key_1;//将创建的函数赋值
key_create(&key1);

```
5.创建按键事件
key_create(key1);

### 关注公众号
![输入图片说明](image.png)
