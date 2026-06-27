// #include "pid.h"

// PID_t balance_pid;
// PID_t speed_pid;
// PID_t turn_pid;


// static float PID_Limit(float value,float max)
// {
//     if(value > max)
//         return max;

//     if(value < -max)
//         return -max;

//     return value;
// }
// //初始化
// void PID_Init(PID_t *pid,
//               float kp,
//               float ki,
//               float kd,
//               float integral_limit,
//               float output_limit)
// {
//     pid->kp = kp;
//     pid->ki = ki;
//     pid->kd = kd;

//     pid->integral_limit = integral_limit;
//     pid->output_limit = output_limit;

//     pid->target = 0;
//     pid->feedback = 0;

//     pid->error = 0;
//     pid->last_error = 0;

//     pid->integral = 0;

//     pid->output = 0;
// }

// void PID_Reset(PID_t *pid)
// {
//     pid->error = 0;
//     pid->last_error = 0;

//     pid->integral = 0;

//     pid->output = 0;
// }

// void PID_InitAll(void)
// {
//     PID_Init(&balance_pid,
//              0.0f,
//              0.0f,
//              0.0f,
//              100.0f,
//              1000.0f);

//     PID_Init(&speed_pid,
//              0.0f,
//              0.0f,
//              0.0f,
//              100.0f,
//              1000.0f);

//     PID_Init(&turn_pid,
//              0.0f,
//              0.0f,
//              0.0f,
//              100.0f,
//              1000.0f);
// }

// //位置式PID计算
// float PID_PositionCalc(PID_t *pid,
//                        float target,
//                        float feedback)
// {
//     pid->target = target;
//     pid->feedback = feedback;

//     pid->error = target - feedback;

//     pid->integral += pid->error*0.005f; // 5ms  

//     pid->integral =
//             PID_Limit(pid->integral,
//                       pid->integral_limit);

//     pid->output =
//             pid->kp * pid->error
//           + pid->ki * pid->integral
//           + pid->kd * (pid->error - pid->last_error);

//     pid->output =
//             PID_Limit(pid->output,
//                       pid->output_limit);

//     pid->last_error = pid->error;

//     return pid->output;
// }

#include "pid.h"
#include <math.h>

// 定义全局实例
PID_t balance_pid;
PID_t speed_pid;
PID_t turn_pid;

// 限幅辅助函数
static float PID_Limit(float value, float max) {
    if (value > max) return max;
    if (value < -max) return -max;
    return value;
}

// --- 初始化函数（新增 sep_thresh 和 dt 参数）---
void PID_Init(PID_t *pid, float kp, float ki, float kd, 
              float integral_limit, float output_limit,
              float sep_thresh, float dt) 
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral_limit = integral_limit;
    pid->output_limit = output_limit;
    pid->integral_separation_thresh = sep_thresh;
    pid->dt = dt;

    // 清空工作变量
    pid->target = 0.0f;
    pid->feedback = 0.0f;
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->last_feedback = 0.0f;
    pid->integral = 0.0f;
    pid->output = 0.0f;
}

// --- 重置函数（保留目标值不清零，防止意外；但这里保守清零）---
void PID_Reset(PID_t *pid) {
    pid->error = 0.0f;
    pid->last_error = 0.0f;
    pid->last_feedback = 0.0f;
    pid->integral = 0.0f;
    pid->output = 0.0f;
    // 注意：target 和 feedback 不建议在这里清零，因为外部还在采集；保持原样
}

// --- 初始化所有环（填入理论初值/保底值）---
void PID_InitAll(void) {
    // 1. 平衡环 (PD控制)  -> 理论计算核心，Kd给个保底值
    //    分离阈值设为 5.0 (角度误差小于5度才积分，但通常平衡环Ki=0，这里只是预留)
    PID_Init(&balance_pid, 
             300.0f,   // Kp (根据理论计算填入，暂时保守)
             0.0f,     // Ki (平衡环务必为0，或极小)
             15.0f,    // Kd 
             50.0f,    // 积分限幅 (虽然Ki=0，但留余地)
             1000.0f,   // 输出限幅 (对应PWM满幅，如1000)
             5.0f,     // 积分分离阈值 (只在小误差时积分)
             PID_DT);  // 周期5ms

    // 2. 速度环 (PI控制) -> 不能加D，编码器噪声会被放大
    PID_Init(&speed_pid, 
             10.0f,    // Kp (理论计算值)
             0.5f,     // Ki 
             0.0f,     // Kd (必须为0)
             200.0f,   // 积分限幅 (速度累积不会太大)
             500.0f,   // 输出限幅 (速度环输出的是“期望倾角”，限幅5°~10°左右，这里放个大值占位)
             300.0f,   // 积分分离阈值 (速度误差大时不积分，防止急停猛冲)
             PID_DT);

    // 3. 转向环 (P控制) -> 只用一个比例即可
    PID_Init(&turn_pid, 
             50.0f,    // Kp (根据转向灵敏度调整)
             0.0f,     // Ki 
             0.0f,     // Kd 
             0.0f,     // 积分限幅 (不用就设为0)
             200.0f,   // 输出限幅 (差速补偿不宜超过基础PWM的一半)
             0.0f,     // 积分分离阈值 (没用)
             PID_DT);
}

// --- 核心计算函数（终极修正版）---
float PID_PositionCalc(PID_t *pid, float target, float feedback) {
    // 1. 更新基础变量
    pid->target = target;
    pid->feedback = feedback;
    pid->error = target - feedback;

    // ======== 2. 积分项处理（带 积分分离 + 抗积分饱和） ========
    // 条件A：误差在允许范围内才积分（积分分离）
    if (fabsf(pid->error) < pid->integral_separation_thresh) 
    {
        // 条件B：判断是否处于饱和状态（抗积分饱和 Anti-windup）
        // 如果输出已经达到限幅值，且误差方向与输出方向一致，则冻结积分
        if ( (pid->output >= pid->output_limit && pid->error > 0) ||
             (pid->output <= -pid->output_limit && pid->error < 0) ) 
        {
            // 饱和状态，不累加积分，避免积分过深
        } 
        else 
        {
            // 关键修正：误差累加必须乘以采样周期 dt（消除周期对积分强度的影响）
            pid->integral += pid->error * pid->dt;
        }
    } 
    else 
    {
        // 误差很大（如刚启动或被猛推），清空积分防止超调后猛冲
        pid->integral = 0.0f;
    }

    // 积分项最终限幅（硬件物理极限）
    pid->integral = PID_Limit(pid->integral, pid->integral_limit);

    // ======== 3. 微分项处理（测量微分，完全消除“设定值突变冲击”） ========
    // 使用 feedback - last_feedback，而不是 error - last_error
    // 这样当 target 突变时，微分项不会产生尖峰脉冲
    float derivative = (pid->feedback - pid->last_feedback) / pid->dt;
    // 因为误差 = target - feedback，所以 d(error)/dt = -d(feedback)/dt
    // 故输出 D 项 = -Kd * derivative
    float output_d = -pid->kd * derivative;

    // ======== 4. 总输出计算 ========
    float output_p = pid->kp * pid->error;
    float output_i = pid->ki * pid->integral;
    pid->output = output_p + output_i + output_d;

    // 输出总限幅（对应PWM占空比上限）
    pid->output = PID_Limit(pid->output, pid->output_limit);

    // ======== 5. 更新历史值（为下一帧准备） ========
    pid->last_error = pid->error;
    pid->last_feedback = pid->feedback;

    return pid->output;
}