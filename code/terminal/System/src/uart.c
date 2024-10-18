#include <reg51.h>
#include "stdint.h"

//宏定义

#define RX_LENGTH 3

//变量定义

uint8_t RX_Flag;
uint8_t RX_Data[6];
uint8_t RX_Length;
uint8_t RX_State;

//函数定义

/**
  * @brief  串口初始化
  * @param  无
  * @retval 无
  */
void Uart_Init(void)
{
  SCON  = 0x50;
  EA    = 1   ;
  ES    = 1   ;
  PCON  = 0   ;
  TMOD  = 0x20;
  TL1   = 0xfd;
  TH1   = 0xfd;
  TR1   = 1   ;
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
  Uart_SendByte(0xFF);
  for(i = 0; i < Length; i ++){
    Uart_Send2Byte(Data[i]);
  }
  Uart_SendByte(0xFE);
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
      if(Data == 0xFF){
        RX_State = 1;
      }
    }
    else if(RX_State == 1){
      RX_Data[RX_Length] = Data;
      RX_Length ++;
      if(RX_Length > (RX_LENGTH * 2 - 1)){
        RX_Length = 0;
        RX_State = 2;
      }
    }
    else if(RX_State == 2){
      if(Data == 0xFE){
        RX_State = 0;
        RX_Flag = 1;
      }
    }
  }
}