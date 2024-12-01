#ifndef __UART_H__
#define __UART_H__

//函数声明

void Uart_Init(void);
void Uart_SendByte(uint8_t Byte);
void Uart_Send2Byte(uint16_t _2Byte);
void Uart_SendFrame(uint16_t* Data, uint8_t Length);
void Uart_GetRxdata(uint16_t* ReceivedData);
void Uart_ClearRxFlag(void);

#endif