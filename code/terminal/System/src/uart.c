#include <reg51.h>
#include "stdint.h"

//宏定义

#define RX_LENGTH 3             //接收数据的长度

//变量定义

uint8_t RX_Flag;                //接收数据标志
uint8_t RX_Data[RX_LENGTH * 2]; //接收数据数组
uint8_t RX_Length;              //已接收数据的长度
uint8_t RX_State;               //接收状态

//函数定义

/**
  * @brief  串口初始化
  * @param  无
  * @retval 无
  */
void Uart_Init(void)
{
  SCON  = 0x50; //8位UART，波特率可变
  EA    = 1   ; //开放中断
  ES    = 1   ; //允许串行口中断
  PCON  = 0x00; //设置SMOD = 0, SMOD0 = 0
  TMOD  = 0x20; //设置定时器/计数器1定时、工作方式2
  TL1   = 0xfd; //设置波特率低8位
  TH1   = 0xfd; //设置波特率高8位
  TR1   = 1   ; //启动定时器/计数器1
}

/**
  * @brief  串口发送一字节数据
  * @param  Byte 一字节数据
  * @retval 无
  */
void Uart_SendByte(uint8_t Byte)
{
  SBUF = Byte;
  while(TI == 0);
  TI = 0;
}

/**
  * @brief  串口发送两字节数据
  * @param  _2Byte 两字节数据
  * @retval 无
  */
void Uart_Send2Byte(uint16_t _2Byte)
{
  Uart_SendByte(_2Byte);
  Uart_SendByte(_2Byte >> 8);
}

/**
  * @brief  串口发送一帧带帧头帧尾的数据
  * @param  Data 两字节长数据数组
  * @param  Length 数组长度
  * @retval 无
  */
void Uart_SendFrame(uint16_t* Data, uint8_t Length)
{
  uint8_t i;
  Uart_SendByte(0xFF);          //发送帧头
  for(i = 0; i < Length; i ++){ //发送数据
    Uart_Send2Byte(Data[i]);
  }
  Uart_SendByte(0xFE);          //发送帧尾
}

/**
  * @brief  获取串口接收的数据
  * @param  ReceivedData 接收数据存放数组
  * @retval 无
  */
void Uart_GetRxdata(uint16_t* ReceivedData)
{
  uint8_t i;
  while(RX_Flag == 0);
  RX_Flag = 0;
  for(i = 0; i < RX_LENGTH; i ++){
    ReceivedData[i] = (RX_Data[i * 2 + 1] << 8) + RX_Data[i * 2];
  }
}

/**
  * @brief  串口接收数据中断
  * @param  无
  * @retval 无
  */
void Uart_ReceiveByte(void) interrupt 4
{
  uint8_t Data;
  if(RI){
    RI = 0;
    Data = SBUF;
    if(RX_State == 0){
      if(Data == 0xFF){     //识别帧头
        RX_State = 1;
      }
    }
    else if(RX_State == 1){ //接收数据
      RX_Data[RX_Length] = Data;
      RX_Length ++;
      if(RX_Length > (RX_LENGTH * 2 - 1)){
        RX_Length = 0;
        RX_State = 2;
      }
    }
    else if(RX_State == 2){ //识别帧尾
      if(Data == 0xFE){
        RX_State = 0;
        RX_Flag = 1;
      }
    }
  }
}