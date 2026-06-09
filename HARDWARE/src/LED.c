#include "stm32f10x.h"                  // Device header
#include "board_config.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED1_ON(void)
{
	GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED1_OFF(void)
{
	GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_PORT, LED_PIN) == 0)
		GPIO_SetBits(LED_PORT, LED_PIN);
	else
		GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED2_ON(void)
{
	GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED2_OFF(void)
{
	GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED_PORT, LED_PIN) == 0)
		GPIO_SetBits(LED_PORT, LED_PIN);
	else
		GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED_ON(void)
{
	GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LED_OFF(void)
{
	GPIO_SetBits(LED_PORT, LED_PIN);
}
