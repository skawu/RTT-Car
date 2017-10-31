/***********************************************************************************
 * 文 件 名   : l298n.c
 * 负 责 人   : skawu
 * 创建日期   : 2017年10月31日
 * 文件描述   : L298N电机驱动
 * 版权说明   : Copyright (c) 2016-2017   
 * 其    他   : 
                使用TIM2输出PWM
 * 修改日志   : 
***********************************************************************************/

#include "l298n.h"


static void l298n_tim2_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO), ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);    
}


static void l298n_tim2_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    //设置360分频，1KHz,向上计数，APB2 72MHz
    TIM_TimeBaseInitStructure.TIM_Period = 199;     //1KHz,16位的值，最大65536
    TIM_TimeBaseInitStructure.TIM_Prescaler = 359;  //360分频 即为200KHz，16位的值，最大65536
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0x00;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef   TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性          ___  ___
    TIM_OCInitStructure.TIM_Pulse = 150;   //占空比,输出极性所占多少    150/200=75%        _   _

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能1的预装载寄存器
    
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能2的预装载寄存器

    TIM_ARRPreloadConfig(TIM2, ENABLE);     //使能TIM2在ARR上的预装载寄存器
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE); //开定时器中断
    TIM_CtrlPWMOutputs(TIM2, ENABLE);       //使能定时器PWM输出
    TIM_Cmd(TIM2, ENABLE);                  //使能定时器2
}

static void motor_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = L298N_IN1 | L298N_IN2 | L298N_IN3 | L298N_IN4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(L298N_GPIO, L298N_IN1);
    GPIO_SetBits(L298N_GPIO, L298N_IN2);
    GPIO_SetBits(L298N_GPIO, L298N_IN3);
    GPIO_SetBits(L298N_GPIO, L298N_IN4);
}

void motor_init(void)
{
    motor_gpio_init();
    l298n_tim2_gpio_init();
    l298n_tim2_init();
}

