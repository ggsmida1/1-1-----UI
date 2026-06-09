#include "app_emoji.h"

static void Show_Emoji_UI(void)
{
    /* 睁眼 */
    for (uint8_t i = 0; i < 3; i++)
    {
        OLED_Clear();
        OLED_ShowImage(30, 10 + i, 16, 16, Eyebrow[0]);
        OLED_ShowImage(82, 10 + i, 16, 16, Eyebrow[1]);
        OLED_DrawEllipse(40, 32, 6, 6 - i, OLED_FILLED);
        OLED_DrawEllipse(88, 32, 6, 6 - i, OLED_FILLED);
        OLED_ShowImage(54, 40, 20, 20, Mouth);
        OLED_Update();
        Delay_ms(100);
    }

    /* 闭眼 */
    for (uint8_t i = 0; i < 3; i++)
    {
        OLED_Clear();
        OLED_ShowImage(30, 12 - i, 16, 16, Eyebrow[0]);
        OLED_ShowImage(82, 12 - i, 16, 16, Eyebrow[1]);
        OLED_DrawEllipse(40, 32, 6, 4 + i, OLED_FILLED);
        OLED_DrawEllipse(88, 32, 6, 4 + i, OLED_FILLED);
        OLED_ShowImage(54, 40, 20, 20, Mouth);
        OLED_Update();
        Delay_ms(100);
    }

    Delay_ms(500);
}

int Emoji(void)
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

        Show_Emoji_UI();
    }
}
