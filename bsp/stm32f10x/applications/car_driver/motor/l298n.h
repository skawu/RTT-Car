#ifndef __L298N_H__
#define __L298N_H__

#include "stm32f10x.h"
#include <rtthread.h>


#define L298N_GPIO      GPIOC
#define L298N_IN1       GPIO_Pin_0
#define L298N_IN2       GPIO_Pin_1
#define L298N_IN3       GPIO_Pin_2
#define L298N_IN4       GPIO_Pin_3
#define L298N_IN5       GPIO_Pin_6
#define L298N_IN6       GPIO_Pin_7
#define L298N_IN7       GPIO_Pin_8
#define L298N_IN8       GPIO_Pin_9


#define IS_MOTER_PULSE_VALUE(value)	((0 < value) && (value <= 200))
extern void motor_init(void);
extern int motor_pulse_update(uint16_t value);

#endif

