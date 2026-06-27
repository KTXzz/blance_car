// #include "balance.h"
// #include "motor.h"
// #include "math.h"
// #include "pid.h"
// #include "APP_bluetooth.h"
// /**
//  * @brief 平衡PID初始化
//  */
// void Balance_Init(void)
// {

//     /*
//         平衡车角度环特点：

//         角度偏差越大
//         电机输出越大

//         所以主要靠P和D

//         Ki通常先关闭
//     */

//     PID_Init(&balance_pid,

//              BT_APP_GetData1(),    // Kp
//              BT_APP_GetData2(),    // Ki
//              BT_APP_GetData3(),    // Kd

//              100,

//              1000);

// }



// /**
//  * @brief 平衡控制
//  *
//  * 输入：
//  * 当前角度
//  *
//  * 输出：
//  * 电机PWM
//  */
// void Balance_Control(float angle)
// {

//     float pwm;

//     if(fabsf(angle) > 60.0f)
//     {
//         Motor_Stop();
//         return;
//     }

//     /*
//         目标角度为3

//         3度表示竖直
//     */
//     pwm = PID_PositionCalc(&balance_pid,
//                      3.1f,
//                      angle);



//     /*
//         左右电机同向输出

//         实现前后调整
//     */
//     Motor_SetPWM(pwm,pwm);


// }