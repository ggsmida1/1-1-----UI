#include "app_home.h"

#include "AD.h"
#include "Key.h"
#include "MyRTC.h"
#include "OLED.h"
#include "board_config.h"

static uint16_t ADValue;
static float VBAT;
static int Battery_Capacity;
static uint8_t KeyNum;
static int clkflag = 1;

/* 首页电池显示：ADC 原始值换算为近似电量百分比，仅作为显示参考 */
static void Show_Battery(void)
{
    int sum = 0;
    for (int i = 0; i < 3000; i++)
    {
        ADValue = AD_GetValue();
        sum += ADValue;
    }
    ADValue = sum / 3000;
    VBAT = (float)ADValue / 4095 * 3.3;
    Battery_Capacity = (ADValue - 3276) * 100 / 819;
    if (Battery_Capacity < 0) Battery_Capacity = 0;

    OLED_ShowNum(85, 4, Battery_Capacity, 3, OLED_6X8);
    OLED_ShowChar(103, 4, '%', OLED_6X8);

    if (Battery_Capacity == 100)
        OLED_ShowImage(110, 0, 16, 16, Battery);
    else if (Battery_Capacity >= 10 && Battery_Capacity < 100)
    {
        OLED_ShowImage(110, 0, 16, 16, Battery);
        OLED_ClearArea((112 + Battery_Capacity / 10), 5, (10 - Battery_Capacity / 10), 6);
        OLED_ClearArea(85, 4, 6, 8);
    }
    else
    {
        OLED_ShowImage(110, 0, 16, 16, Battery);
        OLED_ClearArea(112, 5, 10, 6);
        OLED_ClearArea(85, 4, 12, 8);
    }
}

static void Show_Clock_UI(void)
{
    Show_Battery();
    MyRTC_ReadTime();

    OLED_Printf(0, 0, OLED_6X8, "%04d-%02d-%02d",
                MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);

    OLED_Printf(16, 16, OLED_12X24, "%02d:%02d:%02d",
                MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);

    OLED_ShowString(0, 48, "菜单", OLED_8X16);
    OLED_ShowString(96, 48, "设置", OLED_8X16);
}

int App_Home(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();

        if (KeyNum == 1)
        {
            clkflag--;
            if (clkflag <= 0) clkflag = 2;
        }
        else if (KeyNum == 2)
        {
            clkflag++;
            if (clkflag >= 3) clkflag = 1;
        }
        else if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            return clkflag;
        }
        else if (KeyNum == 4)
        {
            /* 当前硬件无电源 MOSFET 时，这里表现为软关机/停机显示 */
            GPIO_ResetBits(PWR_CTRL1_PORT, PWR_CTRL1_PIN);
            GPIO_SetBits(PWR_CTRL2_PORT, PWR_CTRL2_PIN);
            while (1);
        }

        switch (clkflag)
        {
            case 1:
                Show_Clock_UI();
                OLED_ReverseArea(0, 48, 32, 16);
                OLED_Update();
                break;

            case 2:
                Show_Clock_UI();
                OLED_ReverseArea(96, 48, 32, 16);
                OLED_Update();
                break;
        }
    }
}
