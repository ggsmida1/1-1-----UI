# STM32 智能手表

> 基于 STM32F103C8T6 + 0.96" OLED (I2C) 的多功能智能手表项目

## 硬件配置

| 组件 | 型号/引脚 | 说明 |
|:---|:---|:---|
| MCU | STM32F103C8T6 | 72MHz, 64KB Flash, 20KB SRAM |
| 屏幕 | 0.96" OLED 128×64 | I2C 软件模拟: PB8=SCL, PB9=SDA |
| 按键 | 3 个 | Key1=PB1, Key2=PA6, Key3=PA3（长按>1s=关机） |
| 板载 LED | 1 个 | PC13（低电平有效） |
| RTC | LSE 32.768KHz 晶振 | 掉电后备电池保持 |
| 姿态传感器 | MPU6050 (GY-521) | I2C 软件模拟: PB10=SCL, PB11=SDA |
| ADC | PA0 | 电池电压检测 |

## 开发环境

| 项目 | 配置 |
|:---|:---|
| IDE | Keil MDK 5 |
| 编译器 | ARM Compiler 6 (V6.22) |
| 源文件编码 | **统一 UTF-8** |
| 字库字符集 | `OLED_CHARSET_UTF8` |
| 定时器 | TIM2, 1ms 中断 |
| 需排除编译 | `CORE/core_cm3.c`（V6 内置替代） |

## 功能列表（10 课全部完成）

| 编号 | 功能 | 说明 |
|:---:|:---|:---|
| 1-1 | 首页时钟 UI | 日期 + 大字体时间，实时刷新 |
| 1-2 | 日期时间设置 | 年/月/日/时/分/秒 独立调节 |
| 1-3 | 滑动菜单界面 | 7 个图标横向滑动，帧动画 |
| 1-4 | 秒表 | ISR 驱动，时:分:秒 计时 |
| 1-5 | 手电筒 | ON/OFF 控制板载 LED (PC13) |
| 1-6 | MPU6050 姿态解算 | 互补滤波，实时 Roll/Pitch/Yaw |
| 1-7 | 谷歌小恐龙游戏 | 躲避障碍物，sin 跳跃物理 |
| 1-8 | 动态表情包 | 循环眨眼动画 |
| 1-9 | 水平仪 | MPU6050 驱动的实时水平气泡 |
| 1-10 | 电池检测 & 长按关机 | ADC 读电压 + 电量百分比显示 |

## 项目结构

```
stm32手表/
├── README.md
├── .gitignore
├── docs/                          ← 学习笔记（11 个 .md）
│   ├── 单片机基础知识补全笔记.md
│   ├── 1-1-首页时钟UI-学习笔记.md
│   ├── ...
│   └── 1-10-ADC电池电压检测&长按关机-学习笔记.md
├── APP/                           ← 应用层
│   ├── inc/menu.h
│   └── src/menu.c                 ← 全部应用逻辑（~580 行）
├── HARDWARE/                      ← 硬件驱动层
│   ├── inc/                       ← 头文件
│   │   ├── OLED.h, OLED_Data.h    ← 显示驱动 + 字库/图像
│   │   ├── Key.h, LED.h           ← 按键 + LED 驱动
│   │   ├── MPU6050.h, MPU6050_Reg.h ← 姿态传感器
│   │   ├── MyI2C.h               ← MPU6050 专用软件 I2C
│   │   ├── AD.h                  ← ADC 电池电压采集
│   │   ├── dino.h                ← 小恐龙游戏
│   │   └── SetTime.h             ← 日期时间设置
│   └── src/                       ← 源文件
│       ├── OLED.c, OLED_Data.c, Key.c, LED.c
│       ├── MPU6050.c, MyI2C.c, AD.c
│       ├── dino.c, SetTime.c
├── SYSTEM/                        ← 系统驱动
│   ├── inc/Delay.h, MyRTC.h, Timer.h
│   └── src/Delay.c, MyRTC.c, Timer.c
├── STM32F10x_FWLib/               ← STM32 标准外设库
├── CORE/                          ← CMSIS 启动文件
└── USER/                          ← 用户入口
    ├── main.c                     ← 主函数 + ISR
    ├── Template.uvprojx           ← Keil 工程文件
    └── stm32f10x_conf.h
```

## 关键设计约定

1. **编码**: 所有源文件 UTF-8，字库宏 `OLED_CHARSET_UTF8`
2. **命名**: 延迟函数大写 `Delay_Init()`, `Delay_us()`, `Delay_ms()`, `Delay_s()`
3. **LED**: 板载 PC13，`LED_ON()`/`LED_OFF()` 控制
4. **按键**: Key3 (PA3) 短按=确认，长按>1s=关机
5. **ISR**: `Key3_Tick()` → `Key_Tick()` → `StopWatch_Tick()` → `Dino_Tick()`（顺序不能变）
6. **OLED**: OLED_DisplayBuf 全局可访问（`extern`），游戏地面直接写缓冲
7. **menu.c**: 所有应用逻辑在一个文件，后续可解耦

## 已知限制

- 关机是软关机（冻屏死循环），板子上无电源 MOSFET 电路
- 电池电压检测用内部参考，未校准，百分比仅供参考
- 板载只有一个 LED，手电筒效果有限
- Yaw 轴无加速度计参考，会漂移
