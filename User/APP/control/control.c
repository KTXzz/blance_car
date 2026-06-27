//
// Created by wzx on 2026/6/10.
//
#include "Motor.h"
#include "main.h"
#include "usart.h"
#include "control.h"
#include "mpu6050.h"
#include "encoder.h"
#include "balance.h"
#include "bluetooth.h"
#include "APP_bluetooth.h"

volatile uint8_t control_flag = 0;
uint8_t motor_enable = 0;
float target_speed = 0;
float target_turn  = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)  // 判断是哪个定时器
    {
        control_flag = 1;        // 置标志位
    }
}

void Control_Task(void)
{
    BT_APP_proc();
    MPU6050_Proc();
    Encoder_Update();
     /* 3. 急停 */
    if(motor_enable == 0)
    {
        Motor_Stop();
        return;
    }//蓝牙控制是否使能电机
    Balance_Control(MPU6050_GetPitch());
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
       if(huart == &huart2)
    {
        Bluetooth_RxCallback();
    }
}
