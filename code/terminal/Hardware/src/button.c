#include <reg51.h>
#include "stdint.h"
#include "delay.h"

//宏定义

#define BUTTON P1
sbit BUT0 = BUTTON ^ 0;
sbit BUT1 = BUTTON ^ 1;
sbit BUT2 = BUTTON ^ 2;
sbit BUT3 = BUTTON ^ 3;
sbit BUT4 = BUTTON ^ 4;
sbit BUT5 = BUTTON ^ 5;
sbit BUT6 = BUTTON ^ 6;
sbit BUT7 = BUTTON ^ 7;

//函数定义

/**
  * @brief  获取当前按下按键的键值
  * @param  无
  * @retval 键值
  */
uint8_t Button_Press(void)
{
	uint8_t Value = 0;
	BUTTON = 0xff;
	BUT3 = 0;
	if(BUT7 == 0){Delay(1);while(BUT7 == 0);Delay(1);Value = 16;}	//	+
	if(BUT6 == 0){Delay(1);while(BUT6 == 0);Delay(1);Value = 12;}	//	=
	if(BUT5 == 0){Delay(1);while(BUT5 == 0);Delay(1);Value = 11;}	//	Back
	if(BUT4 == 0){Delay(1);while(BUT4 == 0);Delay(1);Value = 1;	}	//	0
	P1 = 0xff;
	BUT2 = 0;
	if(BUT7 == 0){Delay(1);while(BUT7 == 0);Delay(1);Value = 15;}	//	-
	if(BUT6 == 0){Delay(1);while(BUT6 == 0);Delay(1);Value = 4;	}	//	3
	if(BUT5 == 0){Delay(1);while(BUT5 == 0);Delay(1);Value = 3;	}	//	2
	if(BUT4 == 0){Delay(1);while(BUT4 == 0);Delay(1);Value = 2;	}	//	1
	P1 = 0xff;
	BUT1 = 0;
	if(BUT7 == 0){Delay(1);while(BUT7 == 0);Delay(1);Value = 14;}	//	×
	if(BUT6 == 0){Delay(1);while(BUT6 == 0);Delay(1);Value = 7;	}	//	6
	if(BUT5 == 0){Delay(1);while(BUT5 == 0);Delay(1);Value = 6;	}	//	5
	if(BUT4 == 0){Delay(1);while(BUT4 == 0);Delay(1);Value = 5;	}	//	4
	P1 = 0xff;
	BUT0 = 0;
	if(BUT7 == 0){Delay(1);while(BUT7 == 0);Delay(1);Value = 13;}	//	/
	if(BUT6 == 0){Delay(1);while(BUT6 == 0);Delay(1);Value = 10;}	//	9
	if(BUT5 == 0){Delay(1);while(BUT5 == 0);Delay(1);Value = 9;	}	//	8
	if(BUT4 == 0){Delay(1);while(BUT4 == 0);Delay(1);Value = 8;	}	//	7
	//仿真中消抖时长设为1ms效果比较好
	//实际消抖时长应设为20ms
	return Value;
}