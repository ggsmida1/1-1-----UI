#include "app_menu.h"

#include "Key.h"
#include "OLED.h"
#include "app_dino.h"
#include "app_emoji.h"
#include "app_flashlight.h"
#include "app_gradienter.h"
#include "app_mpu6050_ui.h"
#include "app_stopwatch.h"

static uint8_t pre_selection;
static uint8_t target_selection;
static uint8_t x_pre = 48;
static uint8_t Speed = 4;
static uint8_t move_flag;
static uint8_t KeyNum;
static uint8_t menu_flag = 1;

/* 横向滑动菜单动画：pre_selection 逐步追赶 target_selection */
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

int App_Menu(void)
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

        /* menu_flag_temp 只在确认键释放后生效，避免选中项在滑动时被立即执行 */
        if (menu_flag_temp == 1) { return 0; }
        else if (menu_flag_temp == 2) { MenuToFunction(); App_Stopwatch_Run(); }
        else if (menu_flag_temp == 3) { MenuToFunction(); App_Flashlight_Run(); }
        else if (menu_flag_temp == 4) { MenuToFunction(); App_MPU6050_Run(); }
        else if (menu_flag_temp == 5) { App_Dino_Run(); }
        else if (menu_flag_temp == 6) { App_Emoji_Run(); }
        else if (menu_flag_temp == 7) { App_Gradienter_Run(); }

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
