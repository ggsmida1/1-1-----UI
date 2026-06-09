#include "stm32f10x.h"
#include "board_config.h"

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(ADC_RCC_PERIPH, ENABLE);
    RCC_APB2PeriphClockCmd(ADC_GPIO_RCC, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADCx, &ADC_InitStructure);

    ADC_Cmd(ADCx, ENABLE);

    ADC_ResetCalibration(ADCx);
    while (ADC_GetResetCalibrationStatus(ADCx) == SET);
    ADC_StartCalibration(ADCx);
    while (ADC_GetCalibrationStatus(ADCx) == SET);
}

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADCx);
}
