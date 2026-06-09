#include "stm32f10x.h"
#include "menu.h"
#include "Timer.h"

int main(void)
{
    Delay_Init();
    OLED_Init();
    OLED_Clear();
    Menu_Init();
    Timer_Init();

    int clkflag1;

    while (1)
    {
        clkflag1 = First_Page_Clock();
        if (clkflag1 == 1) { Menu(); }
        else if (clkflag1 == 2) { SettingPage(); }
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Key3_Tick();
        Key_Tick();
        StopWatch_Tick();
        Dino_Tick();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
