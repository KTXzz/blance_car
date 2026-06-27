//
// Created by wzx on 2026/6/16.
//

#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"

typedef struct
{
    int16_t left;      // 左轮5ms脉冲数
    int16_t right;     // 右轮5ms脉冲数

    int16_t speed;     // 左右轮总速度

    float left_rpm;
    float right_rpm;
    float left_angle;
    float right_angle;
}Encoder_t;

extern Encoder_t encoder;

void Encoder_Init(void);
void Encoder_Update(void);

#endif