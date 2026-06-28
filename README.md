# STM32 智能手表

> 基于 STM32F103C8T6 + 0.96" OLED (I2C) 的多功能智能手表项目。

## 硬件配置

| 组件 | 型号/引脚 | 说明 |
|:---|:---|:---|
| MCU | STM32F103C8T6 | 72MHz, 64KB Flash, 20KB SRAM |
| 屏幕 | 0.96" OLED 128x64 | 软件 I2C: PB8=SCL, PB9=SDA |
| 按键 | 3 个 | Key1=PB1, Key2=PA6, Key3=PA3，Key3 长按约 1s 触发关机逻辑 |
| 板载 LED | 1 个 | PC13，低电平有效 |
| RTC | LSE 32.768KHz 晶振 | 掉电后由后备电池保持 |
| 姿态传感器 | MPU6050 (GY-521) | 软件 I2C: PB10=SCL, PB11=SDA |
| ADC | PA0 | 电池电压检测 |

## 开发环境

| 项目 | 配置 |
|:---|:---|
| IDE | Keil MDK 5 / uVision |
| 编译器 | ARM Compiler 6.22 (ARMCLANG) |
| 工程文件 | `USER/Template.uvprojx` |
| 输出目录 | `OBJ/` |
| 可烧录文件 | `OBJ/Template.hex` |
| 源文件编码 | UTF-8 |
| 字库字符集 | `OLED_CHARSET_UTF8` |
| 系统节拍 | TIM2, 1ms 中断 |

## 功能列表

| 功能 | 说明 |
|:---|:---|
| 首页时钟 UI | 日期 + 大字体时间，实时刷新 |
| 日期时间设置 | 年/月/日/时/分/秒 独立调节 |
| 滑动菜单界面 | 7 个图标横向滑动，帧动画 |
| 秒表 | 由 1ms tick 推进，显示时:分:秒 |
| 手电筒 | ON/OFF 控制板载 LED (PC13) |
| MPU6050 姿态显示 | 互补滤波，实时 Roll/Pitch/Yaw |
| 小恐龙游戏 | 躲避障碍物，sin 跳跃轨迹 |
| 动态表情包 | 循环眨眼动画 |
| 水平仪 | MPU6050 驱动的实时水平气泡 |
| 电池检测 & 长按关机 | ADC 读电压 + 电量百分比显示 |

## 项目结构

```text
stm32手表/
├── APP/                         应用层：页面、菜单、小游戏、应用入口
│   ├── inc/
│   │   ├── app_launcher.h       应用层初始化入口
│   │   ├── app_tick.h           1ms 应用节拍调度入口
│   │   ├── app_home.h           首页时钟
│   │   ├── app_menu.h           横向菜单
│   │   ├── app_settings.h       设置入口页
│   │   ├── app_set_time.h       日期时间设置页
│   │   ├── app_stopwatch.h      秒表
│   │   ├── app_flashlight.h     手电筒
│   │   ├── app_mpu6050_ui.h     姿态显示
│   │   ├── app_dino.h           小恐龙游戏
│   │   ├── app_emoji.h          动态表情
│   │   └── app_gradienter.h     水平仪
│   └── src/
│       └── app_*.c              各应用实现
├── HARDWARE/                    硬件驱动层
│   ├── inc/
│   │   ├── board_config.h       引脚和外设映射
│   │   ├── OLED.h               OLED 驱动接口
│   │   ├── Key.h                按键扫描
│   │   ├── LED.h                板载 LED
│   │   ├── AD.h                 ADC 电池检测
│   │   ├── MPU6050.h            MPU6050 驱动
│   │   ├── MPU6050_Reg.h        MPU6050 寄存器定义
│   │   └── MyI2C.h              MPU6050 软件 I2C
│   └── src/
│       ├── OLED.c
│       ├── Key.c
│       ├── LED.c
│       ├── AD.c
│       ├── MPU6050.c
│       └── MyI2C.c
├── RES/                         UI 资源层
│   ├── inc/ui_resources.h       字模、图标、位图数据声明
│   └── src/ui_resources.c       字模、图标、位图数据定义
├── SYSTEM/                      系统基础模块
│   ├── inc/Delay.h, MyRTC.h, Timer.h, usart.h
│   └── src/Delay.c, MyRTC.c, Timer.c, usart.c
├── USER/                        Keil 工程入口
│   ├── main.c                   主函数 + TIM2_IRQHandler
│   ├── Template.uvprojx         Keil 工程文件
│   ├── stm32f10x_conf.h
│   └── stm32f10x_it.c
├── CORE/                        CMSIS 启动文件
├── STM32F10x_FWLib/             STM32 标准外设库
├── docs/                        学习笔记
├── OBJ/                         Keil 编译输出
└── README.md
```

## 模块职责约定

1. `USER/main.c` 只负责初始化、顶层页面调度和中断入口。
2. `APP/app_launcher.c` 集中维护应用初始化顺序。
3. `APP/app_tick.c` 集中维护 1ms 周期任务，目前包含按键扫描、秒表节拍和小恐龙节拍。
4. `APP/app_menu.c` 只负责菜单动画和应用入口调度，不直接实现具体应用逻辑。
5. `HARDWARE/` 只放硬件驱动，不放具体页面或小游戏逻辑。
6. `RES/` 只放 UI 资源数据，不放 OLED 驱动逻辑。

## 头文件约定

1. `.c` 文件第一行优先 include 自己对应的 `.h`。
2. `.h` 只包含“接口必须依赖”的头文件。
3. `.c` 内部用到的驱动或资源头文件，放在 `.c` 里 include。
4. 头文件应尽量自包含，单独 include 不应缺少类型定义。

## 关键流程

```text
main()
├── Delay_Init()
├── OLED_Init()
├── App_Init()
├── Timer_Init()
└── while (1)
    ├── App_Home()
    ├── App_Menu()
    └── App_Settings()

TIM2_IRQHandler()
└── App_Tick_1ms()
    ├── Key3_Tick()
    ├── Key_Tick()
    ├── App_Stopwatch_Tick()
    └── App_Dino_Tick()
```

## 已知限制

- 关机逻辑当前偏软关机/停机显示，硬件上没有真正切断电源的 MOSFET 电路时无法完全断电。
- 电池百分比由 ADC 原始值估算，未做完整校准，仅作显示参考。
- 板载只有一个 LED，手电筒效果有限。
- Yaw 轴没有磁力计或其他绝对参考，长时间运行会漂移。
- UI 和应用循环仍是裸机阻塞式页面模型，适合当前项目体量；若后续功能继续增多，可再考虑状态机化。

## 构建验证

最近一次结构调整后使用 Keil 命令行全量 rebuild 验证：

```text
Using Compiler 'V6.22'
Program Size: Code=30976 RO-data=9880 RW-data=36 ZI-data=3028
"..\OBJ\Template.axf" - 0 Error(s), 0 Warning(s).
```
