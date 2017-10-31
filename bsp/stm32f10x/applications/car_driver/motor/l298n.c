/***********************************************************************************
 * �� �� ��   : l298n.c
 * �� �� ��   : skawu
 * ��������   : 2017��10��31��
 * �ļ�����   : L298N�������
 * ��Ȩ˵��   : Copyright (c) 2016-2017   ���ڷ��ǵ������޹�˾
 * ��    ��   : 
                ʹ��TIM2���PWM
 * �޸���־   : 
***********************************************************************************/

#include "l298n.h"


void l298n_tim2_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO), ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);    
}


void l298n_tim2_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    //����360��Ƶ��1KHz,���ϼ�����APB2 72MHz
    TIM_TimeBaseInitStructure.TIM_Period = 199;     //1KHz,16λ��ֵ�����65536
    TIM_TimeBaseInitStructure.TIM_Prescaler = 359;  //360��Ƶ ��Ϊ200KHz��16λ��ֵ�����65536
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x00;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //�������          ___  ___
    TIM_OCInitStructure.TIM_Pulse = 150;   //ռ�ձ�,���������ռ����    150/200=75%        _   _

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��1��Ԥװ�ؼĴ���
    
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��2��Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM2, ENABLE);     //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE); //����ʱ���ж�
    TIM_CtrlPWMOutputs(TIM2, ENABLE);       //ʹ�ܶ�ʱ��PWM���
    TIM_Cmd(TIM2, ENABLE);                  //ʹ�ܶ�ʱ��2
}

