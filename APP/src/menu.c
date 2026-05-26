#include "menu.h"


void Menu_Init(void)
{
    MyRTC_Init();
}

/* --------------  首页时钟 ----------------*/

void Show_Clock_UI(void) {
    MyRTC_ReadTime();

    OLED_Printf(0, 0, OLED_6X8, "%04d-%02d-%02d",
                MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);

    OLED_Printf(16, 16, OLED_12X24, "%02d:%02d:%02d",
                MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);

    OLED_ShowString(0, 48, "菜单", OLED_8X16);
    OLED_ShowString(96, 48, "设置", OLED_8X16);
}

