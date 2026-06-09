#include "app_mpu6050_ui.h"

static void Show_MPU6050_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_Printf(0, 16, OLED_8X16, "Roll: %.2f", Roll);
    OLED_Printf(0, 32, OLED_8X16, "Pitch:%.2f", Pitch);
    OLED_Printf(0, 48, OLED_8X16, "Yaw:  %.2f", Yaw);
}

int MPU6050_Func(void)
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
        MPU6050_Calculation();
        Show_MPU6050_UI();
        OLED_ReverseArea(0, 0, 16, 16);
        OLED_Update();
    }
}
