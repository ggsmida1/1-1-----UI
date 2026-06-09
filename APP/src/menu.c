#include "menu.h"


void Menu_Init(void)
{
    MyRTC_Init();
    Key_Init();
    LED_Init();
    MPU6050_Init();
    AD_Init();
}

/* ========== 首页时钟 ========== */

static uint16_t ADValue;
static float VBAT;
static int Battery_Capacity;

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

void Show_Clock_UI(void) {
    Show_Battery();
    MyRTC_ReadTime();

    OLED_Printf(0, 0, OLED_6X8, "%04d-%02d-%02d",
                MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);

    OLED_Printf(16, 16, OLED_12X24, "%02d:%02d:%02d",
                MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);

    OLED_ShowString(0, 48, "菜单", OLED_8X16);
    OLED_ShowString(96, 48, "设置", OLED_8X16);
}

static uint8_t KeyNum;

static int clkflag = 1;

int First_Page_Clock(void)
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

/* ========== 设置页面 ========== */

static void Show_SettingPage_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_ShowString(0, 16, "日期时间设置", OLED_8X16);
}

static int setflag = 1;

int SettingPage(void)
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
        else if (setflag_temp == 2) { SetTime(); }

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

/* ========== 滑动菜单界面 ========== */

static uint8_t pre_selection;
static uint8_t target_selection;
static uint8_t x_pre = 48;
static uint8_t Speed = 4;
static uint8_t move_flag;

static void Menu_Animation(void)
{
    OLED_Clear();
    OLED_ShowImage(42, 10, 44, 44, Frame);

    if (pre_selection < target_selection)
    {
        x_pre -= Speed;
        if (x_pre == 0)
        {
            pre_selection++;
            move_flag = 0;
            x_pre = 48;
        }
    }

    if (pre_selection > target_selection)
    {
        x_pre += Speed;
        if (x_pre == 96)
        {
            pre_selection--;
            move_flag = 0;
            x_pre = 48;
        }
    }

    if (pre_selection >= 1)
        OLED_ShowImage(x_pre - 48, 16, 32, 32, Menu_Graph[pre_selection - 1]);

    if (pre_selection >= 2)
        OLED_ShowImage(x_pre - 96, 16, 32, 32, Menu_Graph[pre_selection - 2]);

    OLED_ShowImage(x_pre, 16, 32, 32, Menu_Graph[pre_selection]);
    OLED_ShowImage(x_pre + 48, 16, 32, 32, Menu_Graph[pre_selection + 1]);
    OLED_ShowImage(x_pre + 96, 16, 32, 32, Menu_Graph[pre_selection + 2]);

    OLED_Update();
}

static void Set_Selection(uint8_t move_flag, uint8_t Pre_Selection, uint8_t Target_Selection)
{
    if (move_flag == 1)
    {
        pre_selection = Pre_Selection;
        target_selection = Target_Selection;
    }
    Menu_Animation();
}

static void MenuToFunction(void)
{
    for (uint8_t i = 0; i <= 6; i++)
    {
        OLED_Clear();
        if (pre_selection >= 1)
            OLED_ShowImage(x_pre - 48, 16 + 8 * i, 32, 32, Menu_Graph[pre_selection - 1]);

        OLED_ShowImage(x_pre, 16 + 8 * i, 32, 32, Menu_Graph[pre_selection]);
        OLED_ShowImage(x_pre + 48, 16 + 8 * i, 32, 32, Menu_Graph[pre_selection + 1]);

        OLED_Update();
    }
}

static uint8_t menu_flag = 1;

int Menu(void)
{
    move_flag = 1;
    uint8_t DirectFlag = 2;
    while (1)
    {
        KeyNum = Key_GetNum();
        uint8_t menu_flag_temp = 0;

        if (KeyNum == 1)
        {
            DirectFlag = 1;
            move_flag = 1;
            menu_flag--;
            if (menu_flag <= 0) menu_flag = 7;
        }
        else if (KeyNum == 2)
        {
            DirectFlag = 2;
            move_flag = 1;
            menu_flag++;
            if (menu_flag >= 8) menu_flag = 1;
        }
        else if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            menu_flag_temp = menu_flag;
        }

        if (menu_flag_temp == 1) { return 0; }
        else if (menu_flag_temp == 2) { MenuToFunction(); StopWatch(); }
        else if (menu_flag_temp == 3) { MenuToFunction(); LED(); }
        else if (menu_flag_temp == 4) { MenuToFunction(); MPU6050_Func(); }
        else if (menu_flag_temp == 5) { DinoGame_Pos_Init(); DinoGame_Animation(); }
        else if (menu_flag_temp == 6) { Emoji(); }
        else if (menu_flag_temp == 7) { Gradienter(); }

        if (menu_flag == 1)
        {
            if (DirectFlag == 1) Set_Selection(move_flag, 1, 0);
            else if (DirectFlag == 2) Set_Selection(move_flag, 0, 0);
        }
        else
        {
            if (DirectFlag == 1) Set_Selection(move_flag, menu_flag, menu_flag - 1);
            else if (DirectFlag == 2) Set_Selection(move_flag, menu_flag - 2, menu_flag - 1);
        }
    }
}
