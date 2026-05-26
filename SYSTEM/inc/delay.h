#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

// 函数声明
void delay_init(void);       // 初始化延时函数
void delay_ms(uint16_t nms); // 毫秒级延时
void delay_us(uint32_t nus); // 微秒级延时

#endif
