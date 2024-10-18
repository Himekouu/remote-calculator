#include "stdint.h"

//函数定义

/**
  * @brief  延迟N秒
  * @param  N 毫秒数
  * @retval 无
  */
void Delay(uint16_t N)
{
  uint8_t i, j;
	i = 2;
	j = 239;
  while(N--){
    do
    {
      while (--j);
    } while (--i);
  }
}