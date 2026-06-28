#include "app_launcher.h"

#include "AD.h"
#include "Key.h"
#include "LED.h"
#include "MPU6050.h"
#include "MyRTC.h"

/*
 * 应用层统一初始化入口。
 * main.c 不直接关心每个硬件模块的初始化顺序，后续新增应用依赖时在这里集中维护。
 */
void App_Init(void)
{
    MyRTC_Init();
    Key_Init();
    LED_Init();
    MPU6050_Init();
    AD_Init();
}
