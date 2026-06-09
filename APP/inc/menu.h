#ifndef __MENU_H
#define __MENU_H

#include "board_config.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Key.h"
#include "LED.h"
#include "SetTime.h"
#include "MPU6050.h"
#include "dino.h"
#include "Delay.h"
#include "AD.h"

#include "app_stopwatch.h"
#include "app_flashlight.h"
#include "app_mpu6050_ui.h"
#include "app_emoji.h"
#include "app_gradienter.h"

void Menu_Init(void);
void Show_Clock_UI(void);
int  First_Page_Clock(void);
int  SettingPage(void);
int  Menu(void);

#endif
