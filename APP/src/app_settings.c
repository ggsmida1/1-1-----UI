#include "app_settings.h"

#include "Key.h"
#include "OLED.h"
#include "app_set_time.h"

static uint8_t KeyNum;
static int setflag = 1;

static void Show_SettingPage_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_ShowString(0, 16, "日期时间设置", OLED_8X16);
}

int App_Settings(void)
{
    while (1)
    {
        KeyNum = Key_GetNum();
        uint8_t setflag_temp = 0;

        if (KeyNum == 1)
        {
            setflag--;
            if (setflag <= 0) setflag = 2;
        }
        else if (KeyNum == 2)
        {
            setflag++;
            if (setflag >= 3) setflag = 1;
        }
        else if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            setflag_temp = setflag;
        }

        if (setflag_temp == 1) { return 0; }
        else if (setflag_temp == 2) { App_SetTime_Run(); }

        switch (setflag)
        {
            case 1:
                Show_SettingPage_UI();
                OLED_ReverseArea(0, 0, 16, 16);
                OLED_Update();
                break;

            case 2:
                Show_SettingPage_UI();
                OLED_ReverseArea(0, 16, 96, 16);
                OLED_Update();
                break;
        }
    }
}
