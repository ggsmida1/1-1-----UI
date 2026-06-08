#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "menu.h"
#include "Timer.h"
#include "Key.h"

int main(void)
{
    OLED_Init();
    OLED_Clear();
    Menu_Init();
    Timer_Init();

    int clkflag1;

    while (1)
    {
        clkflag1 = First_Page_Clock();
        if (clkflag1 == 1) {}    // 菜单
        else if (clkflag1 == 2) { SettingPage(); } // 设置
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Key_Tick();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
