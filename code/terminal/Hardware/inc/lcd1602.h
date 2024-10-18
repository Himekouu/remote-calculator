#ifndef __LCD1602_H__
#define __LCD1602_H__

//函数声明

void LCD_Init(void);
void LCD_ShowChar(uint8_t ROW, uint8_t COL, int8_t CHAR);
void LCD_ShowString(uint8_t ROW, uint8_t COL, int8_t *STR);
void LCD_ShowNumber(uint8_t ROW,uint8_t COL,uint32_t NUM,uint8_t LEN);

#endif