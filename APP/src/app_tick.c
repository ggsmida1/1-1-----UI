#include "app_tick.h"

#include "Key.h"
#include "app_dino.h"
#include "app_stopwatch.h"

/*
 * TIM2 每 1ms 调用一次。
 * ISR 里只做短小的节拍推进和按键扫描，具体页面逻辑仍在各 App_xxx_Run() 中执行。
 */
void App_Tick_1ms(void)
{
    Key3_Tick();
    Key_Tick();
    App_Stopwatch_Tick();
    App_Dino_Tick();
}
