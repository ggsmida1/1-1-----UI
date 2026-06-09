#include "stm32f10x.h"
#include "board_config.h"
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(MPU_I2C_PORT, MPU_I2C_SCL_PIN, (BitAction)BitValue);
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(MPU_I2C_PORT, MPU_I2C_SDA_PIN, (BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(MPU_I2C_PORT, MPU_I2C_SDA_PIN);
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(MPU_I2C_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = MPU_I2C_SCL_PIN | MPU_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MPU_I2C_PORT, &GPIO_InitStructure);

    GPIO_SetBits(MPU_I2C_PORT, MPU_I2C_SCL_PIN | MPU_I2C_SDA_PIN);
}

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(!!(Byte & (0x80 >> i)));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA()) { Byte |= (0x80 >> i); }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
