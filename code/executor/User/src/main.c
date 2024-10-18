#include <reg51.h>
#include "stdint.h"
#include "uart.h"

//变量定义

uint16_t 	RxData[2];		//接收的数据
uint8_t 	Sign;					//存储的运算符号
int32_t 	Result;				//计算结果
uint8_t		Length;				//计算结果长度
uint16_t	TxData[3];		//发送的数据
uint8_t 	Error_Flag;		//错误标志

void main()
{
	Uart_Init();
	while(1){
		Uart_GetRxdata(RxData);
		Sign = RxData[1] >> 14;					//获取运算符号
		RxData[1] = RxData[1] & 0x3FFF;	//消除运算符号
		if(Sign == 0){	//除法
			if(RxData[1] == 0){	//除数为0错误
				Error_Flag = 1;
			}
			else{
				Result = (uint32_t)RxData[0] / (uint32_t)RxData[1];
			}
		}
		else if(Sign == 1)	Result = (uint32_t)RxData[0] * (uint32_t)RxData[1];	//乘法
		else if(Sign == 2)	Result = (uint32_t)RxData[0] - (uint32_t)RxData[1];	//减法
		else if(Sign == 3)	Result = (uint32_t)RxData[0] + (uint32_t)RxData[1];	//加法
		TxData[0] = Result;
		TxData[1] = Result >> 16;
		if(Result < 0){
			Result = (Result - 1) ^ 0xFFFFFFFF;	//将负数变为正数，方便长度计算
		}
		for(Length = 0; Result != 0; Length ++){	//获取得数长度
			Result /= 10;
		}	
		if(Error_Flag == 1){
			Error_Flag = 0;
			TxData[2] = 0x00FF;	//将错误标志存储在得数长度位置
		}
		else{
			TxData[2] = Length;
		}
		Uart_SendFrame(TxData, 3);
	}
}