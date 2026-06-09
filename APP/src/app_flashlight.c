#include "app_flashlight.h"

static void Show_LED_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_ShowString(20, 20, "OFF", OLED_12X24);
    OLED_ShowString(72, 20, "ON", OLED_12X24);
}

int LED(void)
{
    uint8_t led_flag = 1;
    while (1)
    {
        uint8_t KeyNum = Key_GetNum();
        uint8_t led_flag_temp = 0;

        if (KeyNum == 1)
        {
            led_flag--;
            if (led_flag <= 0) led_flag = 3;
        }
        else if (KeyNum == 2)
        {
            led_flag++;
            if (led_flag >= 4) led_flag = 1;
        }
        else if (KeyNum == 3)
        {
            OLED_Clear();
            OLED_Update();
            led_flag_temp = led_flag;
        }

        if (led_flag_temp == 1) { return 0; }

        switch (led_flag)
        {
            case 1:
                Show_LED_UI();
                OLED_ReverseArea(0, 0, 16, 16);
                OLED_Update();
                break;

            case 2:
                Show_LED_UI();
                LED_OFF();
                OLED_ReverseArea(20, 20, 36, 24);
                OLED_Update();
                break;

            case 3:
                Show_LED_UI();
                LED_ON();
                OLED_ReverseArea(72, 20, 24, 24);
                OLED_Update();
                break;
        }
    }
}
