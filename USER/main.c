#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "menu.h"

int main(void)
{
    OLED_Init();
    OLED_Clear();
    Menu_Init();

    while (1)
    {
        Show_Clock_UI();
        OLED_Update();
    }
}
