#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h> // 引入标准输入输出库，供重定向使用

// 函数声明
void uart_init(uint32_t bound); // 初始化串口1，bound为波特率

#endif
