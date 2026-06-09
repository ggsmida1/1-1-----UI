#include "stm32f10x.h"
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "Delay.h"
#include <math.h>

#define MPU6050_ADDRESS     0xD0

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    MyI2C_Stop();

    return Data;
}

void MPU6050_Init(void)
{
    MyI2C_Init();

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x04);
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;
}

/* ===== 姿态解算（互补滤波） ===== */

float Roll, Pitch, Yaw;
static double pi = 3.1415927;

void MPU6050_Calculation(void)
{
    static int16_t ax, ay, az, gx, gy, gz;
    static float roll_g, pitch_g, yaw_g;
    static float roll_a, pitch_a;
    static const float a = 0.9f;
    static const float Delta_t = 0.005f;

    Delay_ms(5);
    MPU6050_GetData(&ax, &ay, &az, &gx, &gy, &gz);

    /* 陀螺仪积分 */
    roll_g  = Roll  + (float)gx * Delta_t;
    pitch_g = Pitch + (float)gy * Delta_t;
    yaw_g   = Yaw   + (float)gz * Delta_t;

    /* 加速度计反算 */
    pitch_a = atan2((-1) * ax, az) * 180 / pi;
    roll_a  = atan2(ay, az) * 180 / pi;

    /* 互补滤波融合 */
    Roll  = a * roll_g  + (1 - a) * roll_a;
    Pitch = a * pitch_g + (1 - a) * pitch_a;
    Yaw   = a * yaw_g;
}
