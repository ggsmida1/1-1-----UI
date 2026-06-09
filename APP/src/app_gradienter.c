#include "app_gradienter.h"

static void Show_Gradienter_UI(void)
{
    MPU6050_Calculation();
    OLED_DrawCircle(64, 32, 30, OLED_UNFILLED);
    OLED_DrawCircle(64 - (int16_t)Roll, 32 + (int16_t)Pitch, 4, OLED_FILLED);
}

int Gradienter(void)
{
    while (1)
    {
        uint8_t KeyNum = Key_GetNum();
        if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            return 0;
        }
        OLED_Clear();
        Show_Gradienter_UI();
        OLED_Update();
    }
}
