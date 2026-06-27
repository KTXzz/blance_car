#include "pid.h"

PID_t balance_pid;
PID_t speed_pid;
PID_t turn_pid;


static float PID_Limit(float value,float max)
{
    if(value > max)
        return max;

    if(value < -max)
        return -max;

    return value;
}
//初始化
void PID_Init(PID_t *pid,
              float kp,
              float ki,
              float kd,
              float integral_limit,
              float output_limit)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->integral_limit = integral_limit;
    pid->output_limit = output_limit;

    pid->target = 0;
    pid->feedback = 0;

    pid->error = 0;
    pid->last_error = 0;

    pid->integral = 0;

    pid->output = 0;
}

void PID_Reset(PID_t *pid)
{
    pid->error = 0;
    pid->last_error = 0;

    pid->integral = 0;

    pid->output = 0;
}
//位置式PID计算
float PID_PositionCalc(PID_t *pid,
                       float target,
                       float feedback)
{
    pid->target = target;
    pid->feedback = feedback;

    pid->error = target - feedback;

    pid->integral += pid->error;

    pid->integral =
            PID_Limit(pid->integral,
                      pid->integral_limit);

    pid->output =
            pid->kp * pid->error
          + pid->ki * pid->integral
          + pid->kd * (pid->error - pid->last_error);

    pid->output =
            PID_Limit(pid->output,
                      pid->output_limit);

    pid->last_error = pid->error;

    return pid->output;
}