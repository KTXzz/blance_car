//
// Created by wzx on 2026/6/16.
//

#include "encoder.h"
#include "tim.h"

#include "encoder.h"
#include "tim.h"

Encoder_t encoder;

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

    __HAL_TIM_SET_COUNTER(&htim3, 0);
    __HAL_TIM_SET_COUNTER(&htim4, 0);

    encoder.left = 0;
    encoder.right = 0;
    encoder.speed = 0;
    encoder.left_angle = 0.0f;
    encoder.right_angle = 0.0f;
}

void Encoder_Update(void)
{
    encoder.left =
        (int16_t)__HAL_TIM_GET_COUNTER(&htim3);

    encoder.right =
        (int16_t)__HAL_TIM_GET_COUNTER(&htim4);

   __HAL_TIM_SET_COUNTER(&htim3, 0);
   __HAL_TIM_SET_COUNTER(&htim4, 0);

    encoder.speed = (encoder.left +encoder.right)/2;
    encoder.left_rpm = encoder.left * 9.0909f;

    encoder.right_rpm = encoder.right * 9.0909f;

    encoder.left_angle += encoder.left * 360.0f / 1320.0f;

    encoder.right_angle += encoder.right * 360.0f / 1320.0f;
}