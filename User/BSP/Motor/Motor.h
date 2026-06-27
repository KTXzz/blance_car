//
// Created by wzx on 2026/6/4.
//

#ifndef BLANCE_CAR_MOTOR_H
#define BLANCE_CAR_MOTOR_H

#include "main.h"

void Motor_Init(void);

void Motor_SetLeftPWM(int16_t pwm);

void Motor_SetRightPWM(int16_t pwm);

void Motor_SetPWM(int16_t left_pwm,
                  int16_t right_pwm);

void Motor_Stop(void);

#endif //BLANCE_CAR_MOTOR_H