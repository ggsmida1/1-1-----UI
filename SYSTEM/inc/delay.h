#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

// 函数声明
void Delay_Init(void);       // 初始化延时函数
void Delay_ms(uint16_t nms); // 毫秒级延时
void Delay_us(uint32_t nus); // 微秒级延时
void Delay_s(uint32_t ns);   // 秒级延时

#endif
