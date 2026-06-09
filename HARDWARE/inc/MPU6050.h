#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

/* 姿态解算结果（互补滤波后的欧拉角，单位：度） */
extern float Roll, Pitch, Yaw;
void MPU6050_Calculation(void);

#endif
