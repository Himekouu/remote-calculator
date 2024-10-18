#include <reg51.h>
#include "stdint.h"

//74HC595的引脚

sbit RCK = P0 ^ 5;
sbit SCK = P0 ^ 3;
sbit SER = P0 ^ 4;

//LCD1602的引脚

sbit LCD_RS = P0 ^ 0;
sbit LCD_RW = P0 ^ 1;
sbit LCD_E = P0 ^ 2;

//内部函数声明

void WriteByte(uint8_t BYTE);
void LCD_Delay(void);
uint32_t LCD_Pow(int16_t X,int16_t Y);

//函数定义

/**
  * @brief  用74HC595写入一字节数据
  * @param  BYTE 一字节数据
  * @retval 无
  */
void WriteByte(uint8_t BYTE)
{
  uint8_t i = 0;
  RCK = 0;
  for(i = 0; i < 8; i++){
    SER = BYTE & (0x80 >> i);
    SCK = 1;
    SCK = 0;
  }
  RCK = 1;
  RCK = 0;
}

/**
  * @brief  LCD内置延迟函数
  * @param  无
  * @retval 无
  */
void LCD_Delay(void)
{
  uint8_t i;
  i = 120;
  while(--i);
}

/**
  * @brief  给LCD写入一字节指令
  * @param  COM 一字节指令
  * @retval 无
  */
void LCD_WriteCommand(uint8_t COM)
{
  LCD_RS = 0;
  LCD_RW = 0;
  WriteByte(COM);
  LCD_E = 1;
  LCD_Delay();
  LCD_E = 0;
  LCD_Delay();
}

/**
  * @brief  给LCD写入一字节数据
  * @param  DATA 一字节数据
  * @retval 无
  */
void LCD_WriteData(uint8_t DATA)
{
  LCD_RS = 1;
  LCD_RW = 0;
  WriteByte(DATA);
  LCD_E = 1;
  LCD_Delay();
  LCD_E = 0;
  LCD_Delay();
}

/**
  * @brief  LCD初始化
  * @param  无
  * @retval 无
  */
void LCD_Init(void)
{
  LCD_WriteCommand(0x38); //八位数据接口，两行显示，5 * 7点阵
  LCD_WriteCommand(0x0C); //显示开，光标关，闪烁关
  LCD_WriteCommand(0x06); //数据读写操作后，光标自动加一，画面不动
  LCD_WriteCommand(0x01); //光标复位，清屏
}

/**
  * @brief  LCD在指定位置显示一个字符
  * @param  ROW 行，范围1-2
  * @param  COL 列，范围1-16
  * @param  CHAR 字符
  * @retval 无
  */
void LCD_ShowChar(uint8_t ROW, uint8_t COL, int8_t CHAR)
{
  if (ROW == 1){
    LCD_WriteCommand(0x80 | (COL - 1));
  }
  else{
    LCD_WriteCommand(0xC0 | (COL - 1));
  }
  LCD_WriteData(CHAR);
}

/**
  * @brief  LCD在指定位置显示字符串
  * @param  ROW 行，范围1-2
  * @param  COL 列，范围1-16
  * @param  STR 字符串
  * @retval 无
  */
void LCD_ShowString(uint8_t ROW, uint8_t COL, int8_t *STR)
{
  uint8_t i = 0, len = 0;
  for(i = 0; STR[i] != '\0'; i++)len++;
  for(i = 0; i < len; i++){
    LCD_ShowChar(ROW, COL + i, STR[i]);
  }
}

//返回X的Y次方
/**
  * @brief  计算X的Y次方
  * @param  X 
  * @param  Y
  * @retval X的Y次方
  */
uint32_t LCD_Pow(int16_t X,int16_t Y)
{
	uint8_t i;
	int32_t Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  LCD显示数字
  * @param  ROW 行，范围1-2
  * @param  COL 列，范围1-16
  * @param  LEN 长度
  * @param  NUM 数字
  * @retval 无
  */
void LCD_ShowNumber(uint8_t ROW,uint8_t COL,uint32_t NUM,uint8_t LEN)
{
	uint8_t i;
  if (ROW == 1){
    LCD_WriteCommand(0x80 | (COL - 1));
  }
  else{
    LCD_WriteCommand(0xC0 | (COL - 1));
  }
	for(i=LEN;i>0;i--)
	{
		LCD_WriteData(NUM/LCD_Pow(10,i-1)%10+'0');
	}
}