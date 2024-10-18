#include <reg51.h>
#include "stdint.h"
#include "lcd1602.h"
#include "button.h"
#include "uart.h"

//变量定义

uint8_t 	Key;					//按下按键的键值
uint8_t 	State;				//终端机状态
uint8_t 	Length;				//数字长度
uint8_t 	Cursor;				//LCD指针位置
uint16_t 	Number[2];		//存储的数字
uint8_t 	Sign;					//存储的运算符号
int32_t 	Result;				//计算结果
uint8_t		Minus;				//计算结果的正负
uint16_t 	RxData[3];		//接收的数据

void main()
{
	LCD_Init();
	Uart_Init();
	while(1){
		if(Key = Button_Press()){	//获取键值
			if(State == 0){				//初始状态
				if((Key >= 1) && (Key <= 10)){
					LCD_ShowString(1, 1, "                ");	//清屏
					LCD_ShowString(2, 1, "                ");	
					State = 1;
					Length = 1;
					Cursor = 1;
					Number[0] = Key - 1;
					Number[1] = 0;
					LCD_ShowNumber(1, Cursor, Key - 1, 1);
				}
			}
			else if(State == 1){	//输入第一个数字状态
				if((Key >= 1) && (Key <= 10) && (Length <= 3)){	//输入数字
					if((Number[0] == 0) && (Length == 1));	//在数字为0时阻止输入第二个0
					else{
						Length ++;
						Cursor ++;
						Number[0] = Number[0] * 10 + Key - 1;
						LCD_ShowNumber(1, Cursor, Key - 1, 1);
					}
				}
				else if(Key == 11){	//回退
					Length --;
					LCD_ShowChar(1, Cursor, ' ');
					Cursor --;
					Number[0] = Number[0] / 10;
				}
				else if((Key >= 13) && (Key <= 16) && (Length != 0)){	//输入符号
					State = 2;
					Length = 0;
					Cursor ++;
					switch (Key)
					{
					case 13:
						Sign = '/';
						break;
					case 14:
						Sign = '*';
						break;
					case 15:
						Sign = '-';
						break;
					case 16:
						Sign = '+';
						break;
					}
					LCD_ShowChar(1, Cursor, Sign);
					Sign = Key - 13;
				}
			}
			else if(State == 2){	//输入第二个数字状态
				if((Key >= 1) && (Key <= 10) && (Length <= 3)){	//输入数字
					if((Number[1] == 0) && (Length == 1));	//在数字为0时组织输入第二个0
					else{
						Length ++;
						Cursor ++;
						Number[1] = Number[1] * 10 + Key - 1;
						LCD_ShowNumber(1, Cursor, Key - 1, 1);
					}
				}
				else if(Key == 11){	//回退
					if(Length == 0){	//回退符号回到上一个状态
						State = 1;
						Length = 4;
					}
					else{
						Length --;
						Number[1] = Number[1] / 10;
					}
					LCD_ShowChar(1, Cursor, ' ');
					Cursor --;
				}
				else if((Key == 12) && (Length != 0)){	//输入等号
					State = 0;
					Cursor ++;
					LCD_ShowChar(1, Cursor, '=');
					Number[1] |= Sign << 14;	//在传输数据中存储运算符号
					Uart_SendFrame(Number, 2);
					Uart_GetRxdata(RxData);
					Result = ((uint32_t)RxData[1] << 16) + RxData[0];
					Length = RxData[2];
					if(Length == 0xFF){	//如果长度位置是错误标志
						LCD_ShowString(2, 1, "Error!");
					}
					else if(Result < 0){
						LCD_ShowChar(2, 1, '-');
						Result = (Result - 1) ^ 0xFFFFFFFF;
						LCD_ShowNumber(2, 2, Result, Length);
					}
					else if(Result == 0){
						LCD_ShowChar(2, 1, '0');
					}
					else{
						LCD_ShowNumber(2, 1, Result, Length);
					}
				}
			}
		}
	}
}