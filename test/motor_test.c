//
// Created by wzx on 2026/6/5.
//

#include "motor_test.h"
#include "Motor.h"
//
//@简介：对电机进行测试
//      先使能电机
//    以不同的PWM值测试电机的转动情况
//
void Motor_Test(void) {
   // Motor_Init();

    // %10正转
    Motor_SetLeftPWM(100);
    Motor_SetRightPWM(100);
        HAL_Delay(2000); // 每个PWM值保持1秒

    //%30正转
    Motor_SetLeftPWM(300);
    Motor_SetRightPWM(300);
        HAL_Delay(2000);

    //%10反转
    Motor_SetPWM(-300,-300);
    HAL_Delay(2000);

    Motor_Stop();
}