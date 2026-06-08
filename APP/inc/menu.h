#ifndef __MENU_H
#define __MENU_H

#include "OLED.h"
#include "MyRTC.h"

void Menu_Init(void);
void Show_Clock_UI(void);
int First_Page_Clock(void);
int SettingPage(void);
int Menu(void);
void StopWatch_Tick(void);


#endif
