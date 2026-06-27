#ifndef __PID_H
#define __PID_H

#define PID_DT 0.005f

typedef struct
{
    float kp;
    float ki;
    float kd;

    float target;         //目标值
    float feedback;       //反馈值

    float error;          //当前误差
    float last_error;     //上次误差
    float last_feedback;  //上次反馈值

    float integral;       //积分项

    float output;         //输出

    float integral_limit; //积分限幅
    float output_limit;   //输出限幅

    float integral_separation_thresh; //积分分离阈值
    float dt;                          //采样周期

} PID_t;


/* 初始化 */
void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float integral_limit,
              float output_limit,
              float sep_thresh,
              float dt);

/* 初始化三个控制环 */
void PID_InitAll(void);

/* 位置式PID */
float PID_PositionCalc(PID_t *pid,
                       float target,
                       float feedback);

/* 清空PID状态 */
void PID_Reset(PID_t *pid);

#endif