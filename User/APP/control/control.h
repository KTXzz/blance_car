//
// Created by wzx on 2026/6/10.
//

#ifndef BLANCE_CAR_CONTROL_H
#define BLANCE_CAR_CONTROL_H

extern volatile uint8_t control_flag;

/* 电机使能 */
extern uint8_t motor_enable;
//速度和方向
extern float target_speed;
extern float target_turn;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);//蓝牙接收
void Control_Task(void);

#endif //BLANCE_CAR_CONTROL_H