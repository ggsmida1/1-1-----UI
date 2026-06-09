#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#include "stm32f10x.h"

/* ============================================================
 *  board_config.h — 所有引脚和外设映射集中管理
 *  换 PCB 时只需修改此文件，不改任何驱动代码
 * ============================================================ */

/* ===== OLED I2C（软件模拟） ===== */
#define OLED_I2C_PORT           GPIOB
#define OLED_I2C_SCL_PIN        GPIO_Pin_8
#define OLED_I2C_SDA_PIN        GPIO_Pin_9
#define OLED_I2C_RCC            RCC_APB2Periph_GPIOB

/* ===== MPU6050 I2C（软件模拟） ===== */
#define MPU_I2C_PORT            GPIOB
#define MPU_I2C_SCL_PIN         GPIO_Pin_10
#define MPU_I2C_SDA_PIN         GPIO_Pin_11
#define MPU_I2C_RCC             RCC_APB2Periph_GPIOB

/* ===== 按键 ===== */
#define KEY1_PORT               GPIOB
#define KEY1_PIN                GPIO_Pin_1
#define KEY1_RCC                RCC_APB2Periph_GPIOB

#define KEY2_PORT               GPIOA
#define KEY2_PIN                GPIO_Pin_6
#define KEY2_RCC                RCC_APB2Periph_GPIOA

#define KEY3_PORT               GPIOA
#define KEY3_PIN                GPIO_Pin_3
#define KEY3_RCC                RCC_APB2Periph_GPIOA

/* ===== 板载 LED ===== */
#define LED_PORT                GPIOC
#define LED_PIN                 GPIO_Pin_13
#define LED_RCC                 RCC_APB2Periph_GPIOC

/* ===== ADC 电池电压检测 ===== */
#define ADC_GPIO_PORT           GPIOA
#define ADC_GPIO_PIN            GPIO_Pin_0
#define ADC_GPIO_RCC            RCC_APB2Periph_GPIOA
#define ADC_RCC_PERIPH          RCC_APB2Periph_ADC1
#define ADC_CHANNEL             ADC_Channel_0
#define ADCx                    ADC1

/* ===== 关机控制引脚（预留，当前板子无电源 MOSFET） ===== */
#define PWR_CTRL1_PORT          GPIOB
#define PWR_CTRL1_PIN           GPIO_Pin_13
#define PWR_CTRL2_PORT          GPIOB
#define PWR_CTRL2_PIN           GPIO_Pin_12

#endif
