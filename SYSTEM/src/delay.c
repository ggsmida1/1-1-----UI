#include "Delay.h"

static uint8_t  fac_us = 0; // us延时倍乘数
static uint16_t fac_ms = 0; // ms延时倍乘数

// 初始化延时函数
void Delay_Init()
{
    // 选择外部时钟，HCLK/8 (72MHz / 8 = 9MHz)
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock / 8000000; // 9
    fac_ms = (uint16_t)fac_us * 1000;   // 9000
}

// 延时 nus (微秒)
void Delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us; // 算出倒计时的总滴答数，装入加载寄存器
    SysTick->VAL = 0x00;          // 清空当前计数值寄存器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 启动定时器开始倒数
    do {
        temp = SysTick->CTRL; // 循环读取控制寄存器的状态
    // 当定时器在运行，且第16位(倒数到0的标志位)没变成1时，一直死等
    } while((temp & 0x01) && !(temp & (1<<16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 时间到，关闭定时器
    SysTick->VAL = 0x00;                       // 清空计数值
}

// 延时 nms (毫秒)
void Delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms; // 装载毫秒级的滴答数
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    } while((temp & 0x01) && !(temp & (1<<16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

void Delay_s(uint32_t ns)
{
    while (ns--)
    {
        Delay_ms(1000);
    }
}
