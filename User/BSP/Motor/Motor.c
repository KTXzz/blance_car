//
// Created by wzx on 2026/6/4.
//

#include "Motor.h"
#include "tim.h"
#include "gpio.h"

/**
 * @brief PWM限幅
 * @param pwm 输入PWM
 * @return 限幅后的PWM
 */
static int16_t Motor_LimitPWM(int16_t pwm)
{
    if (pwm > 999)
        pwm = 999;

    if (pwm < -999)
        pwm = -999;

    return pwm;
}
//
//@简介：电机初始化
//
void Motor_Init(void) {
    // 启动定时器 PWM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    Motor_Stop();
}
/**
 * @brief 左电机PWM控制
 * @param pwm
 * >0 正转
 * <0 反转
 */
void Motor_SetLeftPWM(int16_t pwm)
{
    pwm = Motor_LimitPWM(pwm);

    if (pwm < 0)
    {
        HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port,
                           MOTOR_L_IN1_Pin,
                           GPIO_PIN_SET);

        HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port,
                          MOTOR_L_IN2_Pin,
                          GPIO_PIN_RESET);
        pwm = -pwm;
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port,
                          MOTOR_L_IN1_Pin,
                          GPIO_PIN_RESET);

        HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port,
                          MOTOR_L_IN2_Pin,
                          GPIO_PIN_SET);
    }

    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_1,
                          pwm);
}

/**
 * @brief 右电机PWM控制
 * @param pwm
 * >0 正转
 * <0 反转
 */
void Motor_SetRightPWM(int16_t pwm)
{
    pwm = Motor_LimitPWM(pwm);

    if (pwm >= 0)
    {
        HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port,
                          MOTOR_R_IN1_Pin,
                          GPIO_PIN_SET);

        HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port,
                          MOTOR_R_IN2_Pin,
                          GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port,
                          MOTOR_R_IN1_Pin,
                          GPIO_PIN_RESET);

        HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port,
                          MOTOR_R_IN2_Pin,
                          GPIO_PIN_SET);

        pwm = -pwm;
    }

    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_2,
                          pwm);
}

/**
 * @brief 左右电机统一控制
 */
void Motor_SetPWM(int16_t left_pwm,
                  int16_t right_pwm)
{
    Motor_SetLeftPWM(left_pwm);

    Motor_SetRightPWM(right_pwm);
}

/**
 * @brief 停止电机
 */
void Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_1,
                          0);

    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_2,
                          0);
}