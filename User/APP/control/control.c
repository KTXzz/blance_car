//
// Created by wzx on 2026/6/10.
//
#include "Motor.h"
#include "main.h"
#include "usart.h"
#include "control.h"
#include "mpu6050.h"
#include "encoder.h"
#include "bluetooth.h"
#include "APP_bluetooth.h"
#include "pid.h"

volatile uint8_t control_flag = 0;
uint8_t motor_enable = 0;
float target_speed = 0;
float target_turn  = 0;

static float Control_Limit(float value, float max)
{
    if(value > max)
        return max;

    if(value < -max)
        return -max;

    return value;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)  // 判断是哪个定时器
    {
        control_flag = 1;        // 置标志位
    }
}

void Control_Task(void)
{
    float angle;
    float speed_fb;
    float speed_target_angle;
    float balance_out;
    float turn_fb;
    float turn_out;
    float left_pwm;
    float right_pwm;

    BT_APP_proc();
    MPU6050_Proc();
    Encoder_Update();

            /* 3. 急停 */
        static uint8_t last_enable = 0;

        if(motor_enable == 0)
        {
            Motor_Stop();
            last_enable = 0;
            return;
        }

        if(last_enable == 0)   // 上升沿触发：0 -> 1
        {
            // 1. 复位所有 PID 内部历史状态（积分、微分）
            PID_Reset(&balance_pid);
            PID_Reset(&speed_pid);
            PID_Reset(&turn_pid);
            
            // 2. 【新增】强制清除外设目标值，防止“带油门启动”猛冲
            target_speed = 0.0f;
            target_turn = 0.0f;
            
            // 3. （可选）顺便把电机PWM清一下，防止残余值
            Motor_SetPWM(0, 0);
        }

        last_enable = 1;



    angle = MPU6050_GetPitch();
    speed_fb = (encoder.left_rpm + encoder.right_rpm) * 0.5f;

    /* 速度环：目标速度 -> 期望角度 */
    speed_target_angle = PID_PositionCalc(&speed_pid,
                                          target_speed,
                                          speed_fb);
    speed_target_angle = Control_Limit(speed_target_angle, 8.0f);

    /* 平衡环：期望角度 -> 期望底座输出 */
    balance_out = PID_PositionCalc(&balance_pid,
                                   speed_target_angle,
                                   angle);
    balance_out = Control_Limit(balance_out, 1000.0f);

    /* 转向环：目标转向 -> 左右轮差速 */
    turn_fb = MPU6050_g_z(); //先使用z轴角速度encoder.left_rpm - encoder.right_rpm;
    turn_out = PID_PositionCalc(&turn_pid,
                                target_turn,
                                turn_fb);
    turn_out = Control_Limit(turn_out, 300.0f);

    /* 5. 输出 */
    left_pwm = balance_out - turn_out;
    right_pwm = balance_out + turn_out;

    Motor_SetPWM((int16_t)left_pwm,
                 (int16_t)right_pwm);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart2)
    {
        Bluetooth_RxCallback();
    }
}
