#include "stm32f10x.h"                  // Device header
#include "board_config.h"
#include "Delay.h"

uint8_t Key_Num;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(KEY1_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_RCC, ENABLE);   // KEY2/3 同一端口

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY2_PIN | KEY3_PIN;
	GPIO_Init(KEY2_PORT, &GPIO_InitStructure);   // KEY2/3 同一端口
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}

int press_time;

void Key3_Tick(void)
{
	if (GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0)
		press_time++;
	else
		press_time = 0;
}

uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN) == 0)
		return 1;

	if (GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN) == 0)
		return 2;

	if ((GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0) && press_time > 1000)
		return 4;	// 长按

	if (GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN) == 0)
		return 3;	// 短按

	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrentState, PreState;
	Count++;
	if (Count >= 20)
	{
		Count = 0;
		PreState = CurrentState;
		CurrentState = Key_GetState();
		if (PreState != 0 && CurrentState == 0)
			Key_Num = PreState;
	}
}
