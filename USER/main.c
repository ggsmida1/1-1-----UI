#include "stm32f10x.h"
#include "app_home.h"
#include "app_launcher.h"
#include "app_menu.h"
#include "app_settings.h"
#include "app_tick.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

/*
 * 程序入口只保留三类职责：
 * 1. 初始化底层和应用模块
 * 2. 调度首页、菜单、设置页这三个顶层页面
 * 3. 在 TIM2 中断里转发 1ms 应用节拍
 */
int main(void)
{
    Delay_Init();
    OLED_Init();
    OLED_Clear();
    App_Init();
    Timer_Init();

    int clkflag1;

    while (1)
    {
        clkflag1 = App_Home();
        if (clkflag1 == 1) { App_Menu(); }
        else if (clkflag1 == 2) { App_Settings(); }
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        /* 1ms 周期任务集中在 APP 层，避免 main.c 直接依赖具体应用细节 */
        App_Tick_1ms();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
