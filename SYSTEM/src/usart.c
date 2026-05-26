#include "usart.h"

// 串口1初始化，传入参数为波特率 (比如 115200)
void uart_init(uint32_t bound)
{
    // 1. 定义 GPIO 和 USART 结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    // 2. 开启 USART1 和 GPIOA 的时钟 (串口1默认挂载在PA9和PA10上)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 3. 配置引脚：PA9 是 TX (发送)，配置为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置引脚：PA10 是 RX (接收)，配置为浮空输入 (只打印的话配不配RX无所谓，但全配上是好习惯)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4. 配置 USART1 的参数：波特率、数据位(8)、停止位(1)、无奇偶校验
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 5. 激活 USART1
    USART_Cmd(USART1, ENABLE);
}

// 【灵魂核心】重定向 c 库函数 printf 到串口
// 必须包含 <stdio.h> 才能重写此函数
int fputc(int ch, FILE *f)
{      
    // 死等：等待上一个字符发送完毕 (SR寄存器的第6位为1表示发送完成)
    while((USART1->SR & 0x40) == 0); 
    // 把当前字符丢进串口的数据发送寄存器 (DR)
    USART1->DR = (uint8_t)ch;      
    
    return ch;
}
