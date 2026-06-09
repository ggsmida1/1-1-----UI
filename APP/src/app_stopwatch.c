#include "app_stopwatch.h"

static uint8_t hour, min, sec;
static uint8_t start_timing_flag;

static void Show_StopWatch_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_Printf(32, 20, OLED_8X16, "%02d:%02d:%02d", hour, min, sec);
    OLED_ShowString(8, 44, "开始", OLED_8X16);
    OLED_ShowString(48, 44, "停止", OLED_8X16);
    OLED_ShowString(88, 44, "清除", OLED_8X16);
}

void StopWatch_Tick(void)
{
    static uint16_t Count;
    Count++;
    if (Count >= 1000)
    {
        Count = 0;
        if (start_timing_flag == 1)
        {
            sec++;
            if (sec >= 60)
            {
                sec = 0;
                min++;
                if (min >= 60)
                {
                    min = 0;
                    hour++;
                    if (hour > 99) hour = 0;
                }
            }
        }
    }
}

int StopWatch(void)
{
    uint8_t stopwatch_flag = 1;
    while (1)
    {
        uint8_t KeyNum = Key_GetNum();
        uint8_t stopwatch_flag_temp = 0;

        if (KeyNum == 1)
        {
            stopwatch_flag--;
            if (stopwatch_flag <= 0) stopwatch_flag = 4;
        }
        else if (KeyNum == 2)
        {
            stopwatch_flag++;
            if (stopwatch_flag >= 5) stopwatch_flag = 1;
        }
        else if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            stopwatch_flag_temp = stopwatch_flag;
        }

        if (stopwatch_flag_temp == 1) { return 0; }

        switch (stopwatch_flag)
        {
            case 1:
                Show_StopWatch_UI();
                OLED_ReverseArea(0, 0, 16, 16);
                OLED_Update();
                break;

            case 2:
                Show_StopWatch_UI();
                start_timing_flag = 1;
                OLED_ReverseArea(8, 44, 32, 16);
                OLED_Update();
                break;

            case 3:
                Show_StopWatch_UI();
                start_timing_flag = 0;
                OLED_ReverseArea(48, 44, 32, 16);
                OLED_Update();
                break;

            case 4:
                Show_StopWatch_UI();
                start_timing_flag = 0;
                hour = min = sec = 0;
                OLED_ReverseArea(88, 44, 32, 16);
                OLED_Update();
                break;
        }
    }
}
