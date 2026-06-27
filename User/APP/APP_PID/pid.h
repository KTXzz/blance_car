#ifndef __PID_H
#define __PID_H

typedef struct
{
    float kp;
    float ki;
    float kd;

    float target;         //目标值
    float feedback;       //反馈值

    float error;          //当前误差
    float last_error;     //上次误差

    float integral;       //积分项

    float output;         //输出

    float integral_limit; //积分限幅
    float output_limit;   //输出限幅

} PID_t;

/* PID实例（如果你是结构体形式） */
extern PID_t balance_pid;
extern PID_t speed_pid;
extern PID_t turn_pid;

/* 初始化 */
void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float integral_limit,
              float output_limit);

/* 位置式PID */
float PID_PositionCalc(PID_t *pid,
                       float target,
                       float feedback);

/* 清空PID状态 */
void PID_Reset(PID_t *pid);

#endif